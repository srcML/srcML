/**
 * @file srcml_info.cpp
 * @copyright
 *
 * Copyright (C) 2013-2014  SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Tookit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <srcml.h>
#include <srcml_types.hpp>

/**
 * srcml_list
 * @param srcml_filename name of a file
 *
 * List all of the filenames in the srcML Archive srcml_filename
 *
 * @returns Return NULL terminated array on success and 0 on failure.
 */
const char** srcml_list(const char* srcml_filename) {

    if(srcml_filename == NULL) return 0;

    srcml_archive * archive = srcml_create_archive();
    if(srcml_read_open_filename(archive, srcml_filename) == SRCML_STATUS_ERROR) return 0;
    std::vector<std::string> output_array;

    srcml_unit * unit;
    while((unit = srcml_read_unit(archive))) {
        output_array.push_back(srcml_unit_get_filename(unit));
        srcml_free_unit(unit);
    }
    const char ** output_carray = (const char **)malloc((output_array.size() + 1) * sizeof(const char *));
    if(!output_carray) return 0;

    try {

        for(std::vector<std::string>::size_type i = 0; i < output_array.size(); ++i) {

            output_carray[i] = strdup(output_array.at(i).c_str());
            if(!output_carray[i]) {

                for(unsigned int j = 0; j < i; ++j)
                    free((void *)output_carray[j]);
                return 0;

            }

        }

    } catch(...) {}
    output_carray[output_array.size()] = 0;

    srcml_close_archive(archive);
    srcml_free_archive(archive);

    return output_carray;

}

