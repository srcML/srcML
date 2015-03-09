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

void srcml_input_srcml(ParseQueue& queue,
                       srcml_archive* srcml_output_archive,
                       const srcml_input_src& srcml_input) {

    // open the srcml input archive
    srcml_archive* srcml_input_archive = srcml_archive_create();
    if (contains<int>(srcml_input))
        srcml_archive_read_open_fd(srcml_input_archive, srcml_input);
    else if (contains<FILE*>(srcml_input))
        srcml_archive_read_open_FILE(srcml_input_archive, srcml_input);
    else
        srcml_archive_read_open_filename(srcml_input_archive, srcml_input.c_str());

    if (SRCML_COMMAND_XML & SRCMLOptions::get()) {
        if (SRCML_OPTION_ARCHIVE & srcml_archive_get_options(srcml_input_archive) && srcml_input.unit == 0) {
            srcml_archive_enable_option(srcml_output_archive, SRCML_OPTION_ARCHIVE);
        }

        size_t nsSize = srcml_archive_get_namespace_size(srcml_input_archive);
        
        for (size_t i = 0; i < nsSize; ++i) {
            srcml_archive_register_namespace(srcml_output_archive,
                srcml_archive_get_namespace_prefix(srcml_input_archive, i),
                srcml_archive_get_namespace_uri(srcml_input_archive, i));
        }
    }

    // move to the correct unit
    for (int i = 1; i < srcml_input.unit; ++i) {
        srcml_unit* unit = srcml_read_unit_header(srcml_input_archive);
        srcml_unit_free(unit);
    }

    bool unitPresent = false;

    // process each entry in the input srcml archive
    while (srcml_unit* unit = srcml_read_unit(srcml_input_archive)) {
        unitPresent = true;
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

    if (!unitPresent) {
        std::cerr << "Requested unit " << srcml_input.unit << " out of range.\n";
        exit(4);
    }

}
