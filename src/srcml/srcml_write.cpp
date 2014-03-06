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

static int count = 0;

// Public consumption thread function
void srcml_write(WriteQueue* queue) {

    int isarchive = -5;

    WriteRequest pr;
    while (true) {

        // write request in the queue
        queue->pop(pr);

        // check if done
        if (!pr.position)
            break;

        if (isarchive == -5)
            isarchive = (srcml_archive_get_options(pr.srcml_arch) & SRCML_OPTION_ARCHIVE) > 0;

        if (pr.status == -300) {

            if (isarchive) 
                std::cerr << std::setw(5) << "-" << " " << *pr.filename << '\n';
            else
                std::cerr << "Extension not supported\n";
            continue;
        }

        // write the unit
        srcml_write_unit(pr.srcml_arch, pr.unit);
        ++count;

        if (isarchive)
            std::cerr << std::setw(5) << count << " " << *pr.filename << '\n';

        // free the unit
        srcml_free_unit(pr.unit);
    }
}
