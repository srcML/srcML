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

int main(int argc, char* argv[]) {

    /* create a new srcml archive structure */
    struct srcml_archive* archive = srcml_archive_create();

    /* setup our output file using a FILE* */
    FILE* srcml_output = fopen("project.xml", "w");

    /* open a srcML archive for output */
    srcml_archive_write_open_FILE(archive, srcml_output);

    /* add all the files to the archive */
    for (int i = 1; i < argc; ++i) {

        struct srcml_unit* unit = srcml_unit_create(archive);

        srcml_unit_set_language(unit, srcml_archive_check_extension(archive, argv[i]));

        FILE* srcml_input = fopen(argv[i], "r");
        srcml_unit_parse_FILE(unit, srcml_input);

        srcml_archive_write_unit(archive, unit);

        srcml_unit_free(unit);
        fclose(srcml_input);
    }

    /* close the srcML archive */
    srcml_archive_close(archive);

    /* file can now be closed also */
    fclose(srcml_output);

    /* free the srcML archive data */
    srcml_archive_free(archive);

    return 0;
}
