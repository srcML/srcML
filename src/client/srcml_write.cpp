/**
 * @file srcml_write.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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
#include <stdio.h>
#include <cstring>
#include <ParserTest.hpp>
#include <OpenFileLimiter.hpp>
#include <srcml_utilities.hpp>
#include <mkDir.hpp>
#include <cmath>

// Public consumption thread function
void srcml_write_request(std::shared_ptr<ParseRequest> request, TraceLog& log, const srcml_output_dest& /* destination */) {

    if (!request)
        return;

    if (request->status == SRCML_STATUS_UNSET_LANGUAGE) {

        if (option(SRCML_COMMAND_VERBOSE)) {
            if (!option(SRCML_COMMAND_QUIET)) {
                std::ostringstream outs;
                outs << std::setw(option(SRCML_DEBUG_MODE) ? 52 + 14 : 52) << ' ' << (request->filename ? *request->filename : "");
                log << '-' << outs.str();
            } else {
                log.skip();
            }
        } else if (request->disk_filename) {
            SRCMLstatus(WARNING_MSG, "srcml: Extension not supported %s", *(request->disk_filename));
        } else {
            SRCMLstatus(WARNING_MSG, "srcml: Extension not supported");
        }

        return;
    }

    if (option(SRCML_COMMAND_PARSER_TEST)) {
        ParserTest::entry(request.get(), request->srcml_arch, request->unit.get());
        return;
    }

    srcml_archive* output_archive = request->srcml_arch;

    // created for per-unit archive, close() and free() automatic
    std::unique_ptr<srcml_archive> cloned;

    // open the archive (if per-unit)
    if (request->unit && option(SRCML_COMMAND_NOARCHIVE)) {

        std::string filename;
        if (option(SRCML_COMMAND_TO_DIRECTORY)) {
            filename += *request->disk_dir;
            filename += "/";
        }
        filename += *request->filename;
        filename += ".xml";

        // create the output directory
        auto path = filename.substr(0, filename.find_last_of('/'));
        {
            mkDir dir;
            dir.mkdir(path);
        }

        // call file limiter now that we are actually putting a value into cloned
        OpenFileLimiter::open();
        cloned.reset(srcml_archive_clone(output_archive));
        output_archive = cloned.get();

        // per-unit archive
        srcml_archive_enable_solitary_unit(output_archive);
        srcml_archive_disable_hash(output_archive);

        srcml_archive_write_open_filename(output_archive, filename.c_str());
    }

    // output scalar results
    if (request->results) {
        switch (srcml_transform_get_type(request->results)) {
        case SRCML_RESULT_BOOLEAN:
            {
                // output as true/false with newline after every results
                const char* boolresult = srcml_transform_get_bool(request->results) ? "true\n" : "false\n";
                srcml_archive_write_string(output_archive, boolresult, (int) strlen(boolresult));
            }
            srcml_transform_free(request->results);
            return;

        case SRCML_RESULT_NUMBER:
            {
                std::string s;
                if (srcml_transform_get_number(request->results) != (int) srcml_transform_get_number(request->results))
                    s = std::to_string(srcml_transform_get_number(request->results));
                else
                    s = std::to_string((int) srcml_transform_get_number(request->results));

                srcml_archive_write_string(output_archive, s.c_str(), (int) s.size());

                // output a newline after every result
                srcml_archive_write_string(output_archive, "\n", 1);
            }
            srcml_transform_free(request->results);
            return;

        case SRCML_RESULT_STRING:
            const char* s = (const char*) srcml_transform_get_string(request->results);
            srcml_archive_write_string(output_archive, s, (int) strlen(s));

            // if the string does not end in a newline, output one
            if (s[strlen(s) - 1] != '\n')
                srcml_archive_write_string(output_archive, "\n", 1);

            srcml_transform_free(request->results);
            return;
        };
    }

    // write the unit
    if (request->status == SRCML_STATUS_OK) {

        log.totalLOC(srcml_unit_get_loc(request->unit.get()));

        // chance that a solo unit archive was the input, but transformation was
        // done, so output has to be a full archive
        if (request->results && srcml_transform_get_unit_size(request->results) > 1) {
            srcml_archive_disable_solitary_unit(output_archive);
        }

        // write out any transformed units
        if (request->results) {
            for (int i = 0; i < srcml_transform_get_unit_size(request->results); ++i) {
                srcml_archive_write_unit(output_archive, srcml_transform_get_unit(request->results, i));
            }
        }
        // if no transformed units, write the main unit
        if ((!request->results || srcml_transform_get_unit_size(request->results) == 0) && request->unit) {
            int status = SRCML_STATUS_OK;
            if (option(SRCML_COMMAND_XML_FRAGMENT)) {
                const char* s = srcml_unit_get_srcml_outer(request->unit.get());
                status = srcml_archive_write_string(output_archive, s, (int) strlen(s));
                if (s[strlen(s) - 1] != '\n') {
                    srcml_archive_write_string(output_archive, "\n", 1);
                }
            } else if (option(SRCML_COMMAND_XML_RAW)) {
                const char* s = srcml_unit_get_srcml_inner(request->unit.get());
                status = srcml_archive_write_string(output_archive, s, (int) strlen(s));
                // when non-blank and does not end in newline, add one in
                if (s[0] != '\0' && s[strlen(s) - 1] != '\n') {
                    srcml_archive_write_string(output_archive, "\n", 1);
                }
            } else if (option(SRCML_COMMAND_CAT_XML)) {
                static bool first = true;
                if (first) {
                    auto sarchive = srcml_archive_clone(output_archive);
                    srcml_archive_enable_solitary_unit(sarchive);
                    srcml_archive_disable_hash(sarchive);
                    char* buffer = 0;
                    size_t size = 0;
                    srcml_archive_write_open_memory(sarchive, &buffer, &size);
                    std::unique_ptr<srcml_unit> aunit(srcml_unit_clone(request->unit.get()));
                    srcml_unit_parse_memory(aunit.get(), "", 0);
                    srcml_archive_write_unit(sarchive, aunit.get());
                    srcml_archive_close(sarchive);
                    status = srcml_archive_write_string(output_archive, buffer, (int) size - 51);
                    status = srcml_archive_write_string(output_archive, ">", 1);

                    first = false;
                }
                const char* s = srcml_unit_get_srcml_inner(request->unit.get());
                status = srcml_archive_write_string(output_archive, s, (int) strlen(s));
                // when non-blank and does not end in newline, add one in
                if (s[0] != '\0' && s[strlen(s) - 1] != '\n') {
                    srcml_archive_write_string(output_archive, "\n", 1);
                }
            } else {
                status = srcml_archive_write_unit(output_archive, request->unit.get());
            }
            if (status != SRCML_STATUS_OK) {
                SRCMLstatus(ERROR_MSG) << "Error in writing parsed unit to archive" << '\n';
            }
        }

        if (request->results)
            srcml_transform_free(request->results);

        // logging
        if (option(SRCML_COMMAND_VERBOSE)) {
            std::ostringstream outs;
            outs << std::setw(4) << std::left << request->language;
            outs << ' ' << std::setw(5) << std::right << srcml_unit_get_loc(request->unit.get());
            if (option(SRCML_DEBUG_MODE)) {
                auto runtime = std::round(request->runtime * 10) / 10;
                outs << ' ' << std::setw(6) << std::right << std::fixed << std::setprecision(1) << runtime << "ms";
                auto kloc = std::round(request->runtime > 0 ? (10 * srcml_unit_get_loc(request->unit.get()) / request->runtime) : 0) / 10;
                outs << ' ' << std::setw(4) << std::right << std::fixed << std::setprecision(1) << kloc;
            }
            const char* hash = srcml_unit_get_hash(request->unit.get());
            if (hash)
                outs << ' ' << hash;
            if (request->filename)
                outs << ' ' << *request->filename;

            log << 'a' << outs.str();
        }

    } else if (request->errormsg) {
        if (!request->errormsg->empty())
            SRCMLstatus(WARNING_MSG, *(request->errormsg));

    } else {
        SRCMLstatus(WARNING_MSG, "Internal error " + std::to_string(request->status));
    }
}
