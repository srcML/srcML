/**
 * @file srcml_read_archive_memory.cpp
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

  Take an archive and extract the invidual units and write to a filesystem.
*/

#include <srcml.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    // input archive contents
    std::string input = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp"><decl_stmt><decl><type><name>int</name></type> <name>a</name></decl>;</decl_stmt>
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp"><decl_stmt><decl><type><name>int</name></type> <name>b</name></decl>;</decl_stmt>
</unit>

</unit>)";

    // create a new archive from the string
    srcml_archive* archive = srcml_archive_create();
    srcml_archive_read_open_memory(archive, input.data(), input.size());

    // add all the files to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(archive))) {

        // output the current filename
        std::cout << srcml_unit_get_filename(unit) << '\n';

        // uparse into a buffer
        char* buffer = nullptr;
        size_t size = 0;
        srcml_unit_unparse_memory(unit, &buffer, &size);

        // output the source in the buffer
        std::cout << buffer;

        // free the buffer
        free((void *)buffer);

        // free the unit
        srcml_unit_free(unit);
    }

    // close the archive
    srcml_archive_close(archive);

    // free the archive
    srcml_archive_free(archive);

    return 0;
}
