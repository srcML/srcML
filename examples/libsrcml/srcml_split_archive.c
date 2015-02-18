/**
 * @file srcml_split_archive.c
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

  Split an archive into two, one for .h files and one for other extensions
*/

#include "srcml.h"
#include <string.h>

int main(int argc, char* argv[]) {
    int i;
    struct srcml_archive* iarchive;
    struct srcml_archive* includearchive;
    struct srcml_archive* otherarchive;
    struct srcml_unit* unit;

    /* open up an existing archive */
    iarchive = srcml_archive_create();
    srcml_archive_read_open_filename(iarchive, "project.xml");

    /* create a new srcml archive structure */
    /* options and attributes of cloned archive start the same as
       the original archive */
    includearchive = srcml_archive_clone(iarchive);
    otherarchive = srcml_archive_clone(iarchive);

    /* open a srcML archive for output */
    srcml_archive_write_open_filename(includearchive, "project_include.xml", 0);
    srcml_archive_write_open_filename(otherarchive, "project_other.xml", 0);

    /* copy the files from the input archive to the output archive */
    while ((unit = srcml_read_unit(iarchive))) {

        /* Get the filename */
        const char* filename = srcml_unit_get_filename(unit);
        if (strcmp(".h", filename + strlen(filename) - 2) == 0)
            srcml_write_unit(includearchive, unit);
        else
            srcml_write_unit(otherarchive, unit);

        srcml_unit_free(unit);
    }

    /* close the archives */
    srcml_archive_close(includearchive);
    srcml_archive_close(otherarchive);
    srcml_archive_close(iarchive);

    /* free the srcML archive data */
    srcml_archive_free(includearchive);
    srcml_archive_free(otherarchive);
    srcml_archive_free(iarchive);

    return 0;
}
