/**
 * @file srcml_consume.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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
void srcml_consume(int /* thread_pool_id */, std::shared_ptr<ParseRequest> request, WriteQueue* write_queue) {

    // error passthrough to output for proper output in trace
    if (request->status) {
        request->unit.reset(0);
        write_queue->schedule(request);
        return;
    }

    if (option(SRCML_COMMAND_NOARCHIVE)) {

        srcml_archive_enable_solitary_unit(request->srcml_arch);
        srcml_archive_enable_hash(request->srcml_arch);

        // build the output filename mirroring input filesystem
        // ensure that the directory path has a final "/" when appended to filename
        std::string xml_filename;
        if (request->disk_dir) {
            xml_filename += *request->disk_dir;
            if (request->disk_dir->back() != '/')
                xml_filename += '/';
        }
        // @todo Should this be .srcml?
        xml_filename += *request->filename + ".xml";

        srcml_archive_write_open_filename(request->srcml_arch, xml_filename.c_str());
    }

    std::string original_filename;

    // construct and parse the unit
    request->status = SRCML_STATUS_OK;

    // create the unit if needed
    if (!request->unit) {
        request->unit.reset(srcml_unit_create(request->srcml_arch));
        if (!request->unit) {
            request->status = SRCML_STATUS_ERROR;
            write_queue->schedule(request);
            return;
        }
    }

    // language attribute, required if from memory
    // @todo if request has a language different from input, need to srcml->src->srcml 
    if (srcml_unit_get_language(request->unit.get()) == 0 || srcml_unit_get_language(request->unit.get())[0] == '\0')
        if ((request->status = srcml_unit_set_language(request->unit.get(), request->language.c_str())) != SRCML_STATUS_OK) {
            request->unit.reset();
            write_queue->schedule(request);
            return;
        }

    // (optional) filename attribute
    if (request->filename) {

        original_filename = *request->filename;
        
        // Cleanup filename
        auto it = request->filename->begin();
        while (*it == '.' && std::next(it) != request->filename->end() && *std::next(it) == '/') {
            request->filename->erase(it, std::next(std::next(it)));
            it = request->filename->begin();
        }
        
        if ((request->status = srcml_unit_set_filename(request->unit.get(), request->filename->c_str())) != SRCML_STATUS_OK) {
            request->unit.reset();
            write_queue->schedule(request);
            return;
        }
    }

    // (optional) version attribute
    if (request->version && ((request->status = srcml_unit_set_version(request->unit.get(), request->version->c_str())) != SRCML_STATUS_OK)) {
        request->unit.reset();
        write_queue->schedule(request);
        return;
    }

    // (optional) timestamp attribute
    if (request->time_stamp)
        srcml_unit_set_timestamp(request->unit.get(), request->time_stamp->c_str());

    // parse the buffer/file, timing as we go
    Timer parsetime;

    if (request->disk_filename) {
        request->status = srcml_unit_parse_filename(request->unit.get(), request->disk_filename->c_str());
    }
    else if (request->needsparsing) {
        request->status = srcml_unit_parse_memory(request->unit.get(), request->buffer.data(), request->buffer.size());
    }
    if (request->status != SRCML_STATUS_OK) {
        request->errormsg = "srcml: Unable to open file " + original_filename;
        request->unit.reset();
        write_queue->schedule(request);
        return;
    }

    request->runtime = parsetime.cpu_time_elapsed();
    request->results.type = SRCML_RESULTS_UNITS;

    // perform any transformations and add them to the request
    srcml_unit_apply_transforms(request->srcml_arch, request->unit.get(), &(request->results));
    if (request->results.type == SRCML_RESULTS_NONE) {
        request->unit.reset();
    }

    // schedule unit for output
    write_queue->schedule(request);
}
