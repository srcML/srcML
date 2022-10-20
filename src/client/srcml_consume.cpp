// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_consume.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * Thread-designed function to parse and create a unit.
 * Output is scheduled for writing on the write queue
 */

#include <srcml_consume.hpp>
#include <srcml.h>
#include <ParseRequest.hpp>
#include <WriteQueue.hpp>
#include <srcml_options.hpp>
#include <srcml_cli.hpp>
#include <string>
#include <SRCMLStatus.hpp>
#include <Timer.hpp>

// creates initial unit, parses, and then sends unit to write queue
void srcml_consume(int /* thread_pool_id */, ParseRequest& request, WriteQueue* write_queue) {

    // error passthrough to output for proper output in trace
    if (request.status) {
        request.unit.reset(0);
        write_queue->schedule(std::move(request));
        return;
    }

    std::string original_filename;

    // construct and parse the unit
    request.status = SRCML_STATUS_OK;

    // create the unit if needed
    if (!request.unit) {
        request.unit.reset(srcml_unit_create(request.srcml_arch));
        if (!request.unit) {
            request.status = SRCML_STATUS_ERROR;
            write_queue->schedule(std::move(request));
            return;
        }
    }

    // language attribute, required if from memory
    if (srcml_unit_get_language(request.unit.get()) == 0 || srcml_unit_get_language(request.unit.get())[0] == '\0')
        if ((request.status = srcml_unit_set_language(request.unit.get(), request.language.c_str())) != SRCML_STATUS_OK) {
            request.unit.reset();
            write_queue->schedule(std::move(request));
            return;
        }

    // (optional) filename attribute
    if (request.filename) {

        original_filename = *request.filename;

        // Cleanup filename
        auto it = request.filename->begin();
        while (*it == '.' && std::next(it) != request.filename->end() && *std::next(it) == '/') {
            request.filename->erase(it, std::next(std::next(it)));
            it = request.filename->begin();
        }

        if ((request.status = srcml_unit_set_filename(request.unit.get(), request.filename->c_str())) != SRCML_STATUS_OK) {
            request.unit.reset();
            write_queue->schedule(std::move(request));
            return;
        }
    }

    // (optional) version attribute
    if (request.version && ((request.status = srcml_unit_set_version(request.unit.get(), request.version->c_str())) != SRCML_STATUS_OK)) {
        request.unit.reset();
        write_queue->schedule(std::move(request));
        return;
    }

    // (optional) timestamp attribute
    if (request.time_stamp)
        srcml_unit_set_timestamp(request.unit.get(), request.time_stamp->c_str());

    // parse the buffer/file, timing as we go
    Timer parsetime;

    if (request.disk_filename) {
        request.status = srcml_unit_parse_filename(request.unit.get(), request.disk_filename->c_str());
    }
    else if (request.needsparsing) {

        request.status = srcml_unit_parse_memory(request.unit.get(), request.buffer.data(), request.buffer.size());

    }
    if (request.status == SRCML_STATUS_INVALID_ARGUMENT) {
        request.status = SRCML_STATUS_IO_ERROR;
        request.errormsg = "";
        request.unit.reset();
        write_queue->schedule(std::move(request));
        exit(1);
    }
    if (request.status != SRCML_STATUS_OK) {
        request.errormsg = "srcml: Unable to open file " + original_filename;
        request.unit.reset();
        write_queue->schedule(std::move(request));
        return;
    }

    request.runtime = parsetime.cpu_time_elapsed();

    // perform any transformations and add them to the request
    srcml_unit_apply_transforms(request.srcml_arch, request.unit.get(), &(request.results));
    if (request.results && srcml_transform_get_type(request.results) == SRCML_RESULT_NONE) {
        request.unit.reset();
    }

    // schedule unit for output
    write_queue->schedule(std::move(request));
}
