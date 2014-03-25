/**
 * @file srcml_input_srcml.cpp
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

#include <srcml_input_srcml.hpp>

void srcml_input_srcml(const srcml_input_src& input_filename,
                       srcml_archive* srcml_outarch) {

    // open the input srcml archive
    srcml_archive* srcml_inarch = srcml_create_archive();

    if (contains<int>(input_filename))
        srcml_read_open_fd(srcml_inarch, input_filename);
    else if (contains<FILE*>(input_filename))
        srcml_read_open_FILE(srcml_inarch, input_filename);
    else
        srcml_read_open_filename(srcml_inarch, input_filename.c_str());

    // process each entry in the input srcml archive
    while (srcml_unit* unit = srcml_read_unit(srcml_inarch)) {

        // write the just-read unit to the output srcml archive
        srcml_write_unit(srcml_outarch, unit);

        srcml_free_unit(unit);
    }

    // done with the input srcml archive
    srcml_close_archive(srcml_inarch);
}
