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
#include <boost/static_assert.hpp>
#include <srcml_options.hpp>
#include <srcml_cli.hpp>
#include <string>
#include <SRCMLStatus.hpp>
#include <Timer.hpp>

// creates initial unit, parses, and then sends unit to write queue
void srcml_consume(ParseRequest* request, WriteQueue* write_queue) {

    // error passthrough to output for proper output in trace
    if (request->status) {
        request->unit = 0;
        write_queue->schedule(request);
        return;
    }

    // NOTE: thread task cannot throw exception
    if (option(SRCML_COMMAND_NOARCHIVE)) {

        srcml_archive_disable_full_archive(request->srcml_arch);
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

        srcml_archive_write_open_filename(request->srcml_arch, xml_filename.c_str(), 0);
    }

    std::string original_filename;

    // construct and parse the unit
    int status = SRCML_STATUS_OK;
    try {

        // create the unit start tag
        if (!request->unit) {
            if (!(request->unit = srcml_unit_create(request->srcml_arch))) {
                throw SRCML_STATUS_ERROR;
            }
        }

        // language attribute, required if from memory
        // @todo if request has a language different from input, need to srcml->src->srcml 
        if (srcml_unit_get_language(request->unit) == 0 || srcml_unit_get_language(request->unit)[0] == '\0')
            if ((status = srcml_unit_set_language(request->unit, request->language.c_str())) != SRCML_STATUS_OK)
                throw status;

        // (optional) filename attribute
        if (request->filename) {

            original_filename = *request->filename;
            
            // Cleanup filename
            while ((*request->filename)[0] == '.' || (*request->filename)[0] == '/') {
                request->filename->erase(0,1);
            }
            
            if ((status = srcml_unit_set_filename(request->unit, request->filename->c_str())) != SRCML_STATUS_OK)
                throw status;
        }

        // (optional) version attribute
        if (request->version && ((status = srcml_unit_set_version(request->unit, request->version->c_str())) != SRCML_STATUS_OK))
            throw status;

        // (optional) timestamp attribute
        if (request->time_stamp)
            srcml_unit_set_timestamp(request->unit, request->time_stamp->c_str());

        // parse the buffer/file, timing as we go
        Timer parsetime;

        if (request->disk_filename) {
            status = srcml_unit_parse_filename(request->unit, request->disk_filename->c_str());
        }
        else if (request->needsparsing) {
            status = srcml_unit_parse_memory(request->unit, &request->buffer.front(), request->buffer.size());
        }
        if (status != SRCML_STATUS_OK) {
            request->status = status;
            throw status;
        }

        request->runtime = parsetime.cpu_time_elapsed();

        // perform any transformations and add them to the request
        srcml_unit** results = nullptr;
        double doubleResult;
        srcml_unit_apply_transforms(request->srcml_arch, request->unit, &results, 0, 0, 0);
        if (results) {
            for (auto p = results; *p; ++p) {
                request->units.push_back(*p);
            }
            if (results[0] == nullptr) {
                srcml_unit_free(request->unit);
                request->unit = 0;
            }
        } else {
            srcml_unit_free(request->unit);
            request->unit = 0;
        }

    } catch (...) {

        request->errormsg = "srcml: Unable to open file " + original_filename;
        if (request->unit)
            srcml_unit_free(request->unit);
        request->unit = 0;
    }

    // schedule unit for output
    request->status = status;
    write_queue->schedule(request);
}
