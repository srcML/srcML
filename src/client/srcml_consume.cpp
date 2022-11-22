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
void srcml_consume(int /* thread_pool_id */, std::shared_ptr<ParseRequest> prequest, WriteQueue* write_queue) {

    // error passthrough to output for proper output in trace
    if (prequest->status) {
        prequest->unit.reset(0);
        write_queue->schedule(std::move(prequest));
        return;
    }

    std::string original_filename;

    // construct and parse the unit
    prequest->status = SRCML_STATUS_OK;

    // create the unit if needed
    if (!prequest->unit) {
        prequest->unit.reset(srcml_unit_create(prequest->srcml_arch));
        if (!prequest->unit) {
            prequest->status = SRCML_STATUS_ERROR;
            write_queue->schedule(std::move(prequest));
            return;
        }
    }

    // language attribute, required if from memory
    if (srcml_unit_get_language(prequest->unit.get()) == 0 || srcml_unit_get_language(prequest->unit.get())[0] == '\0')
        if ((prequest->status = srcml_unit_set_language(prequest->unit.get(), prequest->language.data())) != SRCML_STATUS_OK) {
            prequest->unit.reset();
            write_queue->schedule(std::move(prequest));
            return;
        }

    // (optional) filename attribute
    if (prequest->filename) {

        original_filename = *prequest->filename;

        // Cleanup filename
        auto it = prequest->filename->begin();
        while (*it == '.' && std::next(it) != prequest->filename->end() && *std::next(it) == '/') {
            prequest->filename->erase(it, std::next(std::next(it)));
            it = prequest->filename->begin();
        }

        if ((prequest->status = srcml_unit_set_filename(prequest->unit.get(), prequest->filename->data())) != SRCML_STATUS_OK) {
            prequest->unit.reset();
            write_queue->schedule(std::move(prequest));
            return;
        }
    }

    // (optional) version attribute
    if (prequest->version && ((prequest->status = srcml_unit_set_version(prequest->unit.get(), prequest->version->data())) != SRCML_STATUS_OK)) {
        prequest->unit.reset();
        write_queue->schedule(std::move(prequest));
        return;
    }

    // (optional) timestamp attribute
    if (prequest->time_stamp)
        srcml_unit_set_timestamp(prequest->unit.get(), prequest->time_stamp->data());

    // parse the buffer/file, timing as we go
    Timer parsetime;

    if (prequest->disk_filename) {
        prequest->status = srcml_unit_parse_filename(prequest->unit.get(), prequest->disk_filename->data());
    }
    else if (prequest->needsparsing) {

        prequest->status = srcml_unit_parse_memory(prequest->unit.get(), prequest->buffer.data(), prequest->buffer.size());

    }
    if (prequest->status == SRCML_STATUS_INVALID_ARGUMENT) {
        prequest->status = SRCML_STATUS_IO_ERROR;
        prequest->errormsg = "";
        prequest->unit.reset();
        write_queue->schedule(std::move(prequest));
        exit(1);
    }
    if (prequest->status != SRCML_STATUS_OK) {
        prequest->errormsg = "srcml: Unable to open file " + original_filename;
        prequest->unit.reset();
        write_queue->schedule(std::move(prequest));
        return;
    }

    prequest->runtime = parsetime.cpu_time_elapsed();

    // perform any transformations and add them to the request
    srcml_unit_apply_transforms(prequest->srcml_arch, prequest->unit.get(), &(prequest->results));
    if (prequest->results && srcml_transform_get_type(prequest->results) == SRCML_RESULT_NONE) {
        prequest->unit.reset();
    }

    // schedule unit for output
    write_queue->schedule(std::move(prequest));
}
