/**
 * @file srcml_write.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

// Public consumption thread function
void srcml_write_request(ParseRequest* request, TraceLog& log) {

    if (!request)
        return;

    bool isarchive = (srcml_archive_get_options(request->srcml_arch) & SRCML_OPTION_ARCHIVE) != 0;

    // write the unit
    if (request->status == SRCML_STATUS_OK) {

        srcml_write_unit(request->srcml_arch, request->unit);

        if (isarchive)
            log << 'a' << (request->filename ? *request->filename : "");

    } else if (request->status == SRCML_STATUS_UNSET_LANGUAGE) {

        if (isarchive)
            log << '-' << (request->filename ? *request->filename : "");
        else
            std::cerr << "Extension not supported\n";

    } else {
        std::cerr << "Internal eror " << request->status << "\n";
    }

    // free the unit
    if (request->unit) {
        srcml_free_unit(request->unit);

        // close the archive (if per-unit)
        if (SRCML_COMMAND_NOARCHIVE & SRCMLOptions::get()) {
            fprintf(stderr, "DEBUG:  %s %s %d\n", __FILE__,  __FUNCTION__, __LINE__);

            srcml_close_archive(request->srcml_arch);
            srcml_free_archive(request->srcml_arch);
        }
    }

    delete request;
    request = 0;
}
