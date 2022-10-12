/**
 * @file srcml_read_archive_file.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
  Example program of the use of the libsrcml C API.

  Extract the archive to the filesystem.
*/

#include <srcml.h>
#include <stdio.h>
#include <string>

int main(int argc, char* argv[]) {

    // create a new srcml archive structure
    srcml_archive* inputArchive = srcml_archive_create();

    // open a srcML archive for input
    FILE* srcml_input = fopen("project.xml", "r");
    srcml_archive_read_open_FILE(inputArchive, srcml_input);

    // add all the files to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(inputArchive))) {

        // filename of the unit
        std::string filename = srcml_unit_get_filename(unit);

        // uparse and write to a file
        FILE* srcml_output = fopen(filename.data(), "w");
        srcml_unit_unparse_FILE(unit, srcml_output);
        fclose(srcml_output);

        // free the unit
        srcml_unit_free(unit);
    }

    // close the srcML archive
    srcml_archive_close(inputArchive);

    // close the srcML archive file
    fclose(srcml_input);

    // free the archive
    srcml_archive_free(inputArchive);

    return 0;
}
