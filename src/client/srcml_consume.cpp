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
#include <parse_request.hpp>
#include <write_queue.hpp>
#include <boost/static_assert.hpp>
#include <srcml_options.hpp>
#include <srcml_cli.hpp>
#include <string>
#include <srcml_logger.hpp>
#include <timer.hpp>

// creates initial unit, parses, and then sends unit to write queue
void srcml_consume(ParseRequest* request, WriteQueue* write_queue) {

    // error passthrough to output for proper output in trace
    if (request->status) {
        request->unit = 0;
        write_queue->schedule(request);
        return;
    }

    // NOTE: thread task cannot throw exception

    // global access to options
    bool isseparatearchive = option(SRCML_COMMAND_NOARCHIVE);

    // a clone of the intended srcML archive is created
    // the only purpose is to allow files to be parsed, without opening
    // the real destination archive.
    srcml_archive* srcml_arch = srcml_archive_clone(request->srcml_arch);

    if (isseparatearchive) {
        request->srcml_arch = srcml_arch;
        srcml_archive_disable_full_archive(srcml_arch);
        srcml_archive_enable_hash(srcml_arch);

        //Ensure that the directory path has a final "/" when appended to filename
        //Build the output filename        
        //Mirror input filesystem
        std::string xml_filename = "";
        
        if (request->disk_dir->back() != '/') {
            if (request->total_num_inputs == 1) {
                xml_filename = *request->filename + ".xml";
            }
            else {
                xml_filename = *request->disk_dir + '/' + *request->filename + ".xml";    
            }
        }
        else {
            xml_filename = *request->disk_dir + *request->filename + ".xml";
        }

        /*Flat filesystem
        size_t pos = request->filename->find_last_of("/\\");

        if (pos != std::string::npos) {
            ++pos;
        }
        else {
            pos = 0;
        }
        std::string xml_filename = *request->disk_dir + request->filename->substr(pos) + ".xml";*/

        srcml_archive_write_open_filename(srcml_arch, xml_filename.c_str(), 0);
        request->srcml_arch = srcml_arch;
    } else {
        char buffer[100];
        size_t size;
        srcml_archive_write_open_memory(srcml_arch, (char**) &buffer, &size);
    }

    std::string original_filename;

    // construct and parse the unit
    srcml_unit* unit = request->unit;
    int status = SRCML_STATUS_OK;
    try {

        // create the unit start tag
        if (!unit) {
            if (!(unit = srcml_unit_create(srcml_arch)))
                throw SRCML_STATUS_ERROR;
        }

        // language attribute, required if from memory
        if ((status = srcml_unit_set_language(unit, request->language.c_str())) != SRCML_STATUS_OK)
            throw status;

        // (optional) filename attribute
        if (request->filename) {

            original_filename = *request->filename;
            
            // Cleanup filename
            while ((*request->filename)[0] == '.' || (*request->filename)[0] == '/') {
                request->filename->erase(0,1);
            }
            
            if ((status = srcml_unit_set_filename(unit, request->filename->c_str())) != SRCML_STATUS_OK)
                throw status;
        }

        // (optional) version attribute
        if (request->version && ((status = srcml_unit_set_version(unit, request->version->c_str())) != SRCML_STATUS_OK))
            throw status;

        if (request->time_stamp)
            srcml_unit_set_timestamp(unit, request->time_stamp->c_str());

        // parse the buffer/file (unless it is already form a srcml archive)
        Timer parsetime;

        if (request->disk_filename)
            status = srcml_unit_parse_filename(unit, request->disk_filename->c_str());
        else if (!request->unit)
            status = srcml_unit_parse_memory(unit, &request->buffer.front(), request->buffer.size());
        request->runtime = parsetime.cpu_time_elapsed();

        if (status != SRCML_STATUS_OK) {
            // FIXME: Cannot throw exception from thread
            throw status;
        }

    } catch (...) {

        request->errormsg = "srcml: Unable to open file " + original_filename;

        SRCMLlog(WARNING_MSG, "srcml: Unable to open file " + original_filename);

//        if (unit)
//            srcml_unit_free(unit);
//        unit = 0;
//        if (request)
//            delete request;
//        request = 0;
//        return;
    }

    // schedule unit for output
    request->unit = unit;
    request->status = status;
    write_queue->schedule(request);
}
