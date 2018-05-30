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
#include <ParseRequest.hpp>
#include <iostream>
#include <srcml_options.hpp>
#include <TraceLog.hpp>
#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>
#include <SRCMLStatus.hpp>

static bool createdsrcml = false;

// Public consumption thread function
void srcml_write_request(ParseRequest* request, TraceLog& log, const srcml_output_dest& destination) {

    if (!request)
        return;

    if (request->results.type == SRCML_STRING || request->results.type == SRCML_NUMBER || request->results.type == SRCML_BOOLEAN) {

        const char * s = nullptr;
        if (request->results.type == SRCML_BOOLEAN)
            s = request->results.boolValue ? "true" : "false";
        else if (request->results.type == SRCML_NUMBER)
            s = request->results.numberValue != (int) request->results.numberValue ? std::to_string(request->results.numberValue).c_str()
                : std::to_string((int) request->results.numberValue).c_str();
        else if (request->results.type == SRCML_STRING)
            s = (char*) request->results.stringValue;

        if (contains<int>(destination)) {

            write(*destination.fd, s, strlen(s));
            write(*destination.fd, "\n", 1);

//        } else {

//            status = srcml_archive_write_open_filename(request->srcml_arch, destination.c_str(), 0);
        }

        return;
    }

    // write the unit
    if (request->status == SRCML_STATUS_OK) {

        log.totalLOC(request->loc);

        // we don't create the output srcml archive until we are going to write to it
        // Why? Well if we did, then we get an empty srcml archive, and that is not
        // what we want if there were errors along the way
        if (!createdsrcml && !option(SRCML_COMMAND_NOARCHIVE)) {

            int status = 0;
            
            if (contains<int>(destination)) {

                status = srcml_archive_write_open_fd(request->srcml_arch, *destination.fd);

            } else {

                status = srcml_archive_write_open_filename(request->srcml_arch, destination.c_str(), 0);
            }
            if (status != SRCML_STATUS_OK)
                return;

            createdsrcml = true;
        }

        // chance that a solo unit archive was the input, but transformation was
        // done, so output has to be a full archive
        // @todo Make sure it is only an xpath transformation
        if (!request->units.empty())
            srcml_archive_enable_full_archive(request->srcml_arch);

        // write out any transformed units
        for (const auto unit : request->units) {
            srcml_archive_write_unit(request->srcml_arch, unit);
        }

        // if no transformed units, write the main unit
        if (request->units.empty() && request->unit) {
            int status = srcml_archive_write_unit(request->srcml_arch, request->unit);
            if (status != SRCML_STATUS_OK) {
                SRCMLstatus(ERROR_MSG) << "Error in writing parsed unit to archive" << '\n';
            }
        }

        // logging
        // @todo Do we want logging for each xpath result? Or only for main unit?
        std::ostringstream outs;
        outs << (request->filename ? *request->filename : "") << '\t' << request->language << '\t' << request->loc;
        const char* hash = srcml_unit_get_hash(request->unit);
        if (hash)
            outs << '\t' << hash;
        if (option(SRCML_DEBUG_MODE)) {
            outs << '\t' << request->runtime << " ms";
            outs << '\t' << (request->runtime > 0 ? (request->loc / request->runtime) : 0) << " KLOC/s";
        }

        log << 'a' << outs.str();

    } else if (request->status == SRCML_STATUS_UNSET_LANGUAGE) {

        log << '-' << (request->filename ? *request->filename : "");

        if (!option(SRCML_COMMAND_VERBOSE))
            SRCMLstatus(ERROR_MSG, "Extension not supported");

    } else if (request->errormsg) {
        SRCMLstatus(WARNING_MSG, *(request->errormsg));

    } else {
        SRCMLstatus(WARNING_MSG, "Internal eror " + std::to_string(request->status));
    }

    // free the unit
    if (request->unit) {
        srcml_unit_free(request->unit);

        // close the archive (if per-unit)
        if (option(SRCML_COMMAND_NOARCHIVE)) {
            srcml_archive_close(request->srcml_arch);
            srcml_archive_free(request->srcml_arch);
        }
    }

    delete request;
    request = 0;
}
