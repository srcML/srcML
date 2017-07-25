/**
 * @file srcml_input_srcml.cpp
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

#include <srcml_input_srcml.hpp>
#include <parse_queue.hpp>
#include <srcml_input_src.hpp>
#include <srcml.h>
#include <srcml_options.hpp>
#include <srcml_cli.hpp>
#include <srcmlns.hpp>
#include <srcml_logger.hpp>

int srcml_input_srcml(ParseQueue& queue,
                       srcml_archive* srcml_output_archive,
                       const srcml_input_src& srcml_input,
                       const boost::optional<size_t> & revision) {

    // open the srcml input archive
    srcml_archive* srcml_input_archive = srcml_archive_create();
    if (!srcml_input_archive) {
        SRCMLlog(WARNING_MSG, "srcml: Internal libsrcml error");
        return 0;
    }

    int open_status = SRCML_STATUS_OK;
    if (revision)
        open_status = srcml_archive_set_srcdiff_revision(srcml_input_archive, *revision);

    open_status = srcml_archive_read_open(srcml_input_archive, srcml_input);
    if (open_status != SRCML_STATUS_OK) {
        if (srcml_input.protocol == "file" )
            SRCMLlog(WARNING_MSG, "srcml: Unable to open srcml file " + src_prefix_resource(srcml_input.filename));
        else
            SRCMLlog(WARNING_MSG, "srcml: Unable to open srcml URL " + srcml_input.filename);
        srcml_archive_close(srcml_input_archive);
        return 0;
    }

    // output is in srcML
    if (srcmlOption(SRCML_COMMAND_XML)) {

        if (srcml_archive_is_full_archive(srcml_input_archive) && srcml_input.unit == 0)
            srcml_archive_enable_full_archive(srcml_output_archive);

        size_t nsSize = srcml_archive_get_namespace_size(srcml_input_archive);
        for (size_t i = 0; i < nsSize; ++i) {

            // ignore srcDiff URL, since it will not be on the output
            if (revision && srcml_archive_get_namespace_uri(srcml_input_archive, i) == std::string(SRCML_DIFF_NS_URI))
                continue;

            // register the input srcml archive namespace
            srcml_archive_register_namespace(srcml_output_archive,
                srcml_archive_get_namespace_prefix(srcml_input_archive, i),
                srcml_archive_get_namespace_uri(srcml_input_archive, i));
        }
    }

    // move to the correct unit
    for (int i = 1; i < srcml_input.unit; ++i) {
        srcml_unit* unit = srcml_archive_read_unit_header(srcml_input_archive);
        srcml_unit_free(unit);
    }

    // if we found a valid unit
    bool unitFound = false;

    // process each entry in the input srcml archive
    while (srcml_unit* unit =  srcml_archive_read_unit_header(srcml_input_archive)) {

        // must cache the body of the unit before we read the next one
        srcml_unit_read_body(unit);

        unitFound = true;
        // form the parsing request
        ParseRequest* prequest = new ParseRequest;
        prequest->srcml_arch = srcml_output_archive;
        prequest->unit = unit;

        // hand request off to the processing queue
        queue.schedule(prequest);

        // one-time through for individual unit
        if (srcml_input.unit)
            break;
    }

    if (!unitFound) {
        SRCMLlog(CRITICAL_MSG, "Requested unit " + std::to_string(srcml_input.unit) + " out of range.");
        exit(4);
    }

    return 1;
}
