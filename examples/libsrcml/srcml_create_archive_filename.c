/**
 * @file srcml_archive_create_filename.c
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

/*
  Example program of the use of the C API for srcML.

  Create an archive, file by file, with an output filename.
*/

#include "srcml.h"

int main(int argc, char* argv[]) {
    int i;
    struct srcml_archive* archive;
    struct srcml_unit* unit;

    /* create a new srcml archive structure */
    archive = srcml_archive_create();

    /* open a srcML archive for output */
    srcml_archive_write_open_filename(archive, "project.xml", 0);

    /* add all the files to the archive */
    for (i = 1; i < argc; ++i) {

        unit = srcml_unit_create(archive);

        srcml_unit_set_filename(unit, argv[i]);

        /* Translate to srcml and append to the archive */
        srcml_unit_parse_filename(unit, argv[i]);

        /* Translate to srcml and append to the archive */
        srcml_write_unit(archive, unit);

        srcml_unit_free(unit);
    }

    /* close the srcML archive */
    srcml_archive_close(archive);

    /* free the srcML archive data */
    srcml_archive_free(archive);

    return 0;
}
