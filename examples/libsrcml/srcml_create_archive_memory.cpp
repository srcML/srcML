/**
 * @file srcml_archive_create_memory.cpp
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

  Create an archive, file by file, with an output memory.
*/

#include <srcml.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>

int main(int argc, char* argv[]) {

    // open a srcML archive for output
    srcml_archive* archive = srcml_archive_create();
    char* s = nullptr;
    size_t size;
    srcml_archive_write_open_memory(archive, &s, &size);

    // add all the files to the archive
    for (int i = 1; i < argc; ++i) {

        // read the contents of the input file
        std::ifstream ifs(argv[i]);
        std::string contents(std::istreambuf_iterator<char>{ifs}, {});

        // Translate to srcml and append to the archive
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, srcml_archive_check_extension(archive, argv[i]));
        srcml_unit_parse_memory(unit, contents.data(), contents.size());
        srcml_archive_write_unit(archive, unit);

        srcml_unit_free(unit);
    }

    // close the srcML archive
    srcml_archive_close(archive);

    // free the archives
    srcml_archive_free(archive);

    // now dump the contents of the archive
    std::cout << s;
    free(s);

    return 0;
}
