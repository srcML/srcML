/**
 * @file srcml_input_srcml.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

#include <srcml_input_srcml.hpp>
#include <parse_queue.hpp>
 
void srcml_input_srcml(ParseQueue&,
                       srcml_archive* srcml_output_archive,
                       const srcml_input_src& srcml_input) {

    // open the srcml input archive
    srcml_archive* srcml_input_archive = srcml_create_archive();
    if (contains<int>(srcml_input))
        srcml_read_open_fd(srcml_input_archive, srcml_input);
    else if (contains<FILE*>(srcml_input))
        srcml_read_open_FILE(srcml_input_archive, srcml_input);
    else
        srcml_read_open_filename(srcml_input_archive, srcml_input.c_str());

    // process each entry in the input srcml archive
    while (srcml_unit* unit = srcml_read_unit(srcml_input_archive)) {

        // write the just-read unit to the output srcml archive
        srcml_write_unit(srcml_output_archive, unit);

        srcml_free_unit(unit);
    }

    // done with the input srcml archive
    srcml_close_archive(srcml_input_archive);
    srcml_free_archive(srcml_input_archive);
}
