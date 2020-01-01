/**
 * @file srcml_archive_create_file.c
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
  Example program of the use of the C API for srcML.

  Create an archive, file by file, with an output FILE*
*/

#include <srcml.h>
#include <stdio.h>

int main() {

    const char* source[] = { "input/h.cpp", "input/g.cpp", "input/f.cpp" };

    /* create a new srcml archive structure */
    struct srcml_archive* archive = srcml_archive_create();

    /* setup our output file using a file descriptor */
    int srcml_output = fopen("project.xml", "w");

    /* open a srcML archive for output */
    srcml_archive_write_open_FILE(archive, srcml_output);

    /* add all the files to the archive */
    for (int i = 0; i < 3; ++i) {

        struct srcml_unit* unit = srcml_unit_create(archive);

        /* set the language based on the file extension */
        srcml_unit_set_language(unit, srcml_archive_check_extension(archive, source[i]));

        /* set the filename */
        srcml_unit_set_filename(unit, source[i]);

        /* open the source file */
        FILE* srcml_input = fopen(source[i], "r");

        /* translate to srcml */
        srcml_unit_parse_fd(unit, srcml_input);

        /* close the source file */
        fclose(srcml_input);

        /* append to the archive */
        srcml_archive_write_unit(archive, unit);

        /* free the created unit */
        srcml_unit_free(unit);
    }

    /* close the srcML archive */
    srcml_archive_close(archive);

    /* free the srcML archive data */
    srcml_archive_free(archive);

    /* file can now be closed */
    fclose(srcml_output);

    return 0;
}
