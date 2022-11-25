// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_write.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
#include <ParserTest.hpp>
#include <OpenFileLimiter.hpp>
#include <srcml_utilities.hpp>
#include <cmath>
#include <string_view>
#include <filesystem>

using namespace ::std::literals::string_view_literals;

// Public consumption thread function
void srcml_write_request(std::shared_ptr<ParseRequest> prequest, TraceLog& log, const srcml_output_dest& /* destination */) {

    if (!prequest)
        return;

    if (prequest->status == SRCML_STATUS_UNSET_LANGUAGE) {

        if (option(SRCML_COMMAND_VERBOSE)) {
            if (!option(SRCML_COMMAND_QUIET)) {
                std::ostringstream outs;
                outs << std::setw(option(SRCML_DEBUG_MODE) ? 52 + 14 : 52) << ' ' << (prequest->filename ? *prequest->filename : "");
                log << '-' << outs.str();
            } else {
                log.skip();
            }
        } else if (prequest->disk_filename) {
            SRCMLstatus(WARNING_MSG, "srcml: Extension not supported %s", *(prequest->disk_filename));
        } else {
            SRCMLstatus(WARNING_MSG, "srcml: Extension not supported");
        }

        return;
    }

    if (option(SRCML_COMMAND_PARSER_TEST)) {
        ParserTest::entry(prequest.get(), prequest->srcml_arch, prequest->unit.get());
        return;
    }

    srcml_archive* output_archive = prequest->srcml_arch;

    // created for per-unit archive, close() and free() automatic
    std::unique_ptr<srcml_archive> cloned;

    // open the archive (if per-unit)
    if (prequest->unit && option(SRCML_COMMAND_NOARCHIVE)) {

        // full path include directory, filename (with existing extension), and xml extension
        std::filesystem::path path;
        if (option(SRCML_COMMAND_TO_DIRECTORY)) {
            path /= *prequest->disk_dir;
        }
        path /= *prequest->filename;
        path += ".xml"sv;

        // create the output path
        auto directory = path;
        directory.remove_filename();
        std::error_code ec;
        if (!std::filesystem::create_directories(directory, ec) && ec.value() != 0) {
            SRCMLstatus(ERROR_MSG, "Unable to create directory %s", directory.c_str());
        }

        // call file limiter now that we are actually putting a value into cloned
        OpenFileLimiter::open();
        cloned.reset(srcml_archive_clone(output_archive));
        output_archive = cloned.get();

        // per-unit archive
        srcml_archive_enable_solitary_unit(output_archive);
        srcml_archive_disable_hash(output_archive);

        srcml_archive_write_open_filename(output_archive, path.string().c_str());
    }

    // output scalar results
    if (prequest->results) {
        switch (srcml_transform_get_type(prequest->results)) {
        case SRCML_RESULT_BOOLEAN:
            {
                // output as true/false with newline after every results
                std::string_view boolresult = srcml_transform_get_bool(prequest->results) ? "true\n" : "false\n";
                srcml_archive_write_string(output_archive, boolresult.data(), (int) boolresult.size());
            }
            srcml_transform_free(prequest->results);
            return;

        case SRCML_RESULT_NUMBER:
            {
                std::string s;
                if (srcml_transform_get_number(prequest->results) != (int) srcml_transform_get_number(prequest->results))
                    s = std::to_string(srcml_transform_get_number(prequest->results));
                else
                    s = std::to_string((int) srcml_transform_get_number(prequest->results));

                srcml_archive_write_string(output_archive, s.data(), (int) s.size());

                // output a newline after every result
                srcml_archive_write_string(output_archive, "\n", 1);
            }
            srcml_transform_free(prequest->results);
            return;

        case SRCML_RESULT_STRING:
            std::string_view s = srcml_transform_get_string(prequest->results);
            srcml_archive_write_string(output_archive, s.data(), (int) s.size());

            // if the string does not end in a newline, output one
            if (s.back() != '\n')
                srcml_archive_write_string(output_archive, "\n", 1);

            srcml_transform_free(prequest->results);
            return;
        };
    }

    // write the unit
    if (prequest->status == SRCML_STATUS_OK) {

        log.totalLOC(static_cast<size_t>(srcml_unit_get_loc(prequest->unit.get())));

        // chance that a solo unit archive was the input, but transformation was
        // done, so output has to be a full archive
        if (prequest->results && srcml_transform_get_unit_size(prequest->results) > 1) {
            srcml_archive_disable_solitary_unit(output_archive);
        }

        // write out any transformed units
        if (prequest->results) {
            for (int i = 0; i < srcml_transform_get_unit_size(prequest->results); ++i) {
                srcml_archive_write_unit(output_archive, srcml_transform_get_unit(prequest->results, i));
            }
        }
        // if no transformed units, write the main unit
        if ((!prequest->results || srcml_transform_get_unit_size(prequest->results) == 0) && prequest->unit) {
            int status = SRCML_STATUS_OK;
            if (option(SRCML_COMMAND_XML_FRAGMENT)) {
                std::string_view s = srcml_unit_get_srcml_outer(prequest->unit.get());
                status = srcml_archive_write_string(output_archive, s.data(), (int) s.size());
                if (s.back() != '\n') {
                    srcml_archive_write_string(output_archive, "\n", 1);
                }
            } else if (option(SRCML_COMMAND_XML_RAW)) {
                std::string_view s = srcml_unit_get_srcml_inner(prequest->unit.get());
                status = srcml_archive_write_string(output_archive, s.data(), (int) s.size());
                // when non-blank and does not end in newline, add one in
                if (!s.empty() && s.back() != '\n') {
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
                    std::unique_ptr<srcml_unit> aunit(srcml_unit_clone(prequest->unit.get()));
                    srcml_unit_parse_memory(aunit.get(), "", 0);
                    srcml_archive_write_unit(sarchive, aunit.get());
                    srcml_archive_close(sarchive);
                    status = srcml_archive_write_string(output_archive, buffer, (int) size - 51);
                    status = srcml_archive_write_string(output_archive, ">", 1);

                    first = false;
                }
                std::string_view s = srcml_unit_get_srcml_inner(prequest->unit.get());
                status = srcml_archive_write_string(output_archive, s.data(), (int) s.size());
                // when non-blank and does not end in newline, add one in
                if (!s.empty() && s.back() != '\n') {
                    srcml_archive_write_string(output_archive, "\n", 1);
                }
            } else {
                status = srcml_archive_write_unit(output_archive, prequest->unit.get());
            }
            if (status != SRCML_STATUS_OK) {
                SRCMLstatus(ERROR_MSG) << "Error in writing parsed unit to archive" << '\n';
            }
        }

        if (prequest->results)
            srcml_transform_free(prequest->results);

        // logging
        if (option(SRCML_COMMAND_VERBOSE)) {
            std::ostringstream outs;
            outs << std::setw(4) << std::left << prequest->language;
            outs << ' ' << std::setw(5) << std::right << srcml_unit_get_loc(prequest->unit.get());
            if (option(SRCML_DEBUG_MODE)) {
                auto runtime = std::round(prequest->runtime * 10) / 10;
                outs << ' ' << std::setw(6) << std::right << std::fixed << std::setprecision(1) << runtime << "ms";
                auto kloc = std::round(prequest->runtime > 0 ? (10 * srcml_unit_get_loc(prequest->unit.get()) / prequest->runtime) : 0) / 10;
                outs << ' ' << std::setw(4) << std::right << std::fixed << std::setprecision(1) << kloc;
            }
            const char* hash = srcml_unit_get_hash(prequest->unit.get());
            if (hash)
                outs << ' ' << hash;
            if (prequest->filename)
                outs << ' ' << *prequest->filename;

            log << 'a' << outs.str();
        }

    } else if (prequest->errormsg) {
        if (!prequest->errormsg->empty())
            SRCMLstatus(WARNING_MSG, *(prequest->errormsg));

    } else {
        SRCMLstatus(WARNING_MSG, "Internal error " + std::to_string(prequest->status));
    }
}
