/**
 * @file srcml_write.cpp
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
 */

#include <srcml_write.hpp>
#include <srcml.h>
#include <parse_request.hpp>
#include <iostream>
#include <srcml_options.hpp>
#include <trace_log.hpp>
#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>
#include <srcml_logger.hpp>

srcml_output_dest gdestination;

extern bool createdsrcml;

extern srcml_archive* gsrcml_arch;

// Public consumption thread function
void srcml_write_request(ParseRequest* request, TraceLog& log) {

    if (!request)
        return;

    bool isarchive = srcml_archive_is_full_archive(request->srcml_arch);
    // if the input is an archive, then the output should be an archive
    if (isarchive)
       srcml_archive_enable_full_archive(gsrcml_arch);

    isarchive = 1; //Print output for all processed files

    // write the unit
    if (request->status == SRCML_STATUS_OK) {

        if (!createdsrcml && !(srcmlOption(SRCML_COMMAND_NOARCHIVE))) {
            createdsrcml = true;

            int status = 0;
            if (contains<int>(gdestination)) {

                status = srcml_archive_write_open_fd(gsrcml_arch, *gdestination.fd);

            } else {

                status = srcml_archive_write_open_filename(gsrcml_arch, gdestination.c_str(), 0);
            }
            if (status != SRCML_STATUS_OK)
                return;
        }

        srcml_archive_write_unit((srcmlOption(SRCML_COMMAND_NOARCHIVE)) ? request->srcml_arch : gsrcml_arch, request->unit);

        if (isarchive) {
            std::string s = request->filename ? *request->filename : "";
            s += "\t";
            s += request->language;
            s += "\t";
            s += std::to_string(request->loc);
            //s += "\t";
            //s += "10"; //This will be processing time
            s += "\t";

            const char* hash = srcml_unit_get_hash(request->unit);
            s += hash ? hash : "";

            log << 'a' << s;
        }

    } else if (request->status == SRCML_STATUS_UNSET_LANGUAGE) {

        if (isarchive)
            log << '-' << (request->filename ? *request->filename : "");
        else
            SRCMLLogger::log(SRCMLLogger::WARNING_MSG, "Extension not supported");

    } else if (request->errormsg) {
        SRCMLLogger::log(SRCMLLogger::WARNING_MSG, *(request->errormsg));

    } else {
        SRCMLLogger::log(SRCMLLogger::WARNING_MSG, "Internal eror " + std::to_string(request->status));
    }

    // free the unit
    if (request->unit) {
        srcml_unit_free(request->unit);

        // close the archive (if per-unit)
        if (srcmlOption(SRCML_COMMAND_NOARCHIVE)) {
            srcml_archive_close(request->srcml_arch);
            srcml_archive_free(request->srcml_arch);
        }
    }

    delete request;
    request = 0;
}
