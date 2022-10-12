/**
 * @file srcml_archive_create_fd.cpp
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

  Create an archive, file by file, with an output file descriptor
*/

#include <srcml.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef _MSC_BUILD
#ifndef S_IRUSR
#define S_IRUSR _S_IREAD
#endif
#ifndef S_IWUSR
#define S_IWUSR _S_IWRITE
#endif
#include <io.h>
#else
#include <unistd.h>
#endif

int main(int argc, char* argv[]) {

    // create a new srcml archive structure
    srcml_archive* archive = srcml_archive_create();

    // setup our output file using a file descriptor
    int srcml_output = open("project.xml", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    // open a srcML archive for output
    srcml_archive_write_open_fd(archive, srcml_output);

    // add all the files to the archive
    for (int i = 1; i < argc; ++i) {

        srcml_unit* unit = srcml_unit_create(archive);

        srcml_unit_set_language(unit, srcml_archive_check_extension(archive, argv[i]));

        // Translate to srcml
        int srcml_input = open(argv[i], O_RDONLY, 0);
        srcml_unit_parse_fd(unit, srcml_input);

        // Append to the archive
        srcml_archive_write_unit(archive, unit);

        srcml_unit_free(unit);
        close(srcml_input);
    }

    // close the srcML archive
    srcml_archive_close(archive);

    // file can now be closed also
    close(srcml_output);

    // free the archives
    srcml_archive_free(archive);

    return 0;
}
