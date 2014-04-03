/**
 * @file srcml_write.cpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
  srcml_write calls appropriate libsrcml functions for processing srcml
  or source file data respectively
*/

#include <srcml_write.hpp>
#include <srcml.h>
#include <write_queue.hpp>
#include <iostream>
#include <srcml_options.hpp>
#include <trace_log.hpp>

// Public consumption thread function
void srcml_write_request(WriteRequest* ppr) {

    boost::optional<bool> isarchive;

    TraceLog log(std::cerr, SRCMLOptions::get());

        // first time through need to set if this is an archive or not
    if (!isarchive)
        isarchive = (srcml_archive_get_options(ppr->srcml_arch) & SRCML_OPTION_ARCHIVE) > 0;

        // write the unit
    if (ppr->status == SRCML_STATUS_OK) {

        srcml_write_unit(ppr->srcml_arch, ppr->unit);

        if (*isarchive)
            log << 'a' << *ppr->filename;

    } else if (ppr->status == SRCML_STATUS_UNSET_LANGUAGE) {

        if (*isarchive)
            log << '-' << *ppr->filename;
        else
            std::cerr << "Extension not supported\n";

    } else {
        std::cerr << "Internal eror " << ppr->status << "\n";
    }

    // free the unit
    if (ppr->unit)
        srcml_free_unit(ppr->unit);
    ppr->unit = 0;

    delete ppr;
}
