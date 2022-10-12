/**
 * @file srcml_read_archive_fd.cpp
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
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef _MSC_BUILD
#include <io.h>
#else
#include <unistd.h>
#endif

int main(int argc, char* argv[]) {

    // create a new srcml archive structure
    srcml_archive* archive = srcml_archive_create();

    // open a srcML archive for input
    int srcml_input = open("project.xml", O_RDONLY, 0);
    srcml_archive_read_open_fd(archive, srcml_input);

    // add all the files to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(archive))) {

        // inquire about the current unit
        std::string language = srcml_unit_get_language(unit);
        std::string filename = srcml_unit_get_filename(unit);

        // uparse and write to a file
        int srcml_output = open(filename.data(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        srcml_unit_unparse_fd(unit, srcml_output);
        close(srcml_output);

        srcml_unit_free(unit);
    }

    // close the srcML archive
    srcml_archive_close(archive);
    close(srcml_input);

    // free the archive
    srcml_archive_free(archive);

    return 0;
}
