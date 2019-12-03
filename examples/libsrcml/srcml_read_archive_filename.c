/**
 * @file srcml_read_archive_filename.c
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

  Take an archive and extract the invidual units and write to a filesystem.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    /* create a new srcml archive structure */
    struct srcml_archive* archive = srcml_archive_create();

    /* open a srcML archive for input */
    srcml_archive_read_open_filename(archive, "project.xml");

    /* add all the files to the archive */
    struct srcml_unit* unit = 0;
    while ((unit = srcml_archive_read_unit(archive))) {

        /* can inquire about the current unit */
        const char* language = srcml_unit_get_language(unit);
        const char* filename = srcml_unit_get_filename(unit);

        /* uparse and write to a file */
        srcml_unit_unparse_filename(unit, filename);

        srcml_unit_free(unit);
    }

    /* close the srcML archive */
    srcml_archive_close(archive);

    /* free the srcML archive data */
    srcml_archive_free(archive);

    return 0;
}
