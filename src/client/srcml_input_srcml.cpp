/**
 * @file srcml_input_srcml.cpp
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

#include <srcml_input_srcml.hpp>
#include <srcml_utilities.hpp>
#include <ParseQueue.hpp>
#include <srcml_input_src.hpp>
#include <srcml.h>
#include <srcml_options.hpp>
#include <srcml_cli.hpp>
#include <srcmlns.hpp>
#include <SRCMLStatus.hpp>
#include <OpenFileLimiter.hpp>

int srcml_input_srcml(ParseQueue& queue,
                       srcml_archive* srcml_output_archive,
                       const srcml_request_t& srcml_request,
                       const srcml_input_src& srcml_input,
                       const boost::optional<size_t> & revision) {

    // open the srcml input archive
    OpenFileLimiter::open();
    std::shared_ptr<srcml_archive> srcml_input_archive(srcml_archive_create(), srcml_archive_deleter);
    if (!srcml_input_archive) {
        SRCMLstatus(WARNING_MSG, "srcml: Internal libsrcml error");
        return -1;
    }

    int open_status = SRCML_STATUS_OK;
    if (revision)
        open_status = srcml_archive_set_srcdiff_revision(srcml_input_archive.get(), *revision);

    open_status = srcml_archive_read_open(srcml_input_archive.get(), srcml_input);
    if (open_status != SRCML_STATUS_OK) {
        if (srcml_input.protocol == "file" )
            SRCMLstatus(ERROR_MSG, "srcml: Unable to open srcml file %s", src_prefix_resource(srcml_input.filename));
        else
            SRCMLstatus(ERROR_MSG, "srcml: Unable to open srcml URL %s", srcml_input.filename);

        return -1;
    }

    // output is in srcML
    if (option(SRCML_COMMAND_XML)) {

        if (!srcml_archive_is_solitary_unit(srcml_input_archive.get()) && srcml_input.unit == 0)
            srcml_archive_disable_solitary_unit(srcml_output_archive);

        size_t nsSize = srcml_archive_get_namespace_size(srcml_input_archive.get());
        for (size_t i = 0; i < nsSize; ++i) {

            // ignore srcDiff URL, since it will not be on the output
            if (revision && srcml_archive_get_namespace_uri(srcml_input_archive.get(), i) == std::string(SRCML_DIFF_NS_URI))
                continue;

            // register the input srcml archive namespace
            srcml_archive_register_namespace(srcml_output_archive,
                srcml_archive_get_namespace_prefix(srcml_input_archive.get(), i),
                srcml_archive_get_namespace_uri(srcml_input_archive.get(), i));
        }
    }

    // move to the correct unit (if needed)
    for (int i = 1; i < (option(SRCML_COMMAND_PARSER_TEST) ? srcml_request.unit : srcml_input.unit); ++i) {
        if (!srcml_archive_skip_unit(srcml_input_archive.get())) {
            SRCMLstatus(ERROR_MSG, "Requested unit %s out of range.", srcml_input.unit);
            exit(1);
        }
    }

    // if we found a valid unit
    bool unitFound = false;

    // process each entry in the input srcml archive
    while (std::unique_ptr<srcml_unit> unit{ srcml_archive_read_unit(srcml_input_archive.get())}) {

        unitFound = true;

        // if in parser test mode, check if language specified
        if (option(SRCML_COMMAND_PARSER_TEST) && srcml_request.att_language) {

            std::string unit_language;
            if (srcml_unit_get_language(unit.get()))
                unit_language = srcml_unit_get_language(unit.get());
            if (srcml_request.att_language != unit_language) {
                break;
            }
        }

        // form the parsing request
        std::shared_ptr<ParseRequest> prequest(new ParseRequest);
        prequest->srcml_arch = srcml_output_archive;
        prequest->unit.swap(unit);
        prequest->needsparsing = false;
        prequest->input_archive = srcml_input_archive;
        prequest->parsertest_filename = srcml_input.resource;

        if (srcml_archive_get_url(prequest->input_archive.get()))
            prequest->url = srcml_archive_get_url(prequest->input_archive.get());

        // if the archive has a language (set by the user) then use that
        // this is a way of converting language
        if (srcml_archive_get_language(srcml_output_archive))
            srcml_unit_set_language(prequest->unit.get(), srcml_archive_get_language(srcml_output_archive));

        // hand request off to the processing queue
        queue.schedule(prequest);

        // one-time through for individual unit
        if (option(SRCML_COMMAND_PARSER_TEST) ? srcml_request.unit : srcml_input.unit)
            break;
    }

    if (!unitFound) {
        SRCMLstatus(ERROR_MSG, "Requested unit %d out of range.", srcml_input.unit);
        exit(1);
    }

    return 1;
}
