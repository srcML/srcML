/**
 * @file srcml_sort_archive.c
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

  Sorts an archive in alphabetical filename order.
  Not especially useful, but does show how units can be rearranged.
*/

#include <srcml.h>
#include <string.h>

int main(int argc, char* argv[]) {

    int num_units = 0;
    struct srcml_unit* units[10];
    const char* inputfile = "project.xml";
    const char* outputfile = "project_tmp.xml";

    /* open up an existing archive */
    struct srcml_archive* iarchive = srcml_archive_create();

    /* create a new srcml archive structure */
    /* options and attributes of cloned archive start the same as
       the original archive */
    struct srcml_archive* oarchive = srcml_archive_clone(iarchive);
    srcml_archive_read_open_filename(iarchive, inputfile);
    while (1) {

        units[num_units] = srcml_archive_read_unit(iarchive);
        if (units[num_units] == 0)
            break;
        ++num_units;
    }
    int i;
    for(i = 1; i < num_units; ++i) {

        int j;
        for(j = i; j > 0; --j) {

            if(strcmp(srcml_unit_get_filename(units[j]), srcml_unit_get_filename(units[j - 1])) < 0) {

                struct srcml_unit* tmp_unit = units[j];
                units[j] = units[j - 1];
                units[j - 1] = tmp_unit;

            } else
                break;

        }

    }

    /* open a srcML archive for output */
    srcml_archive_write_open_filename(oarchive, outputfile);

    for(i = 0; i < num_units; ++i) {

        /* copy the files from the input archive to the output archive */
        /* Translate to srcml and append to the archive */
        srcml_archive_write_unit(oarchive, units[i]);

        srcml_unit_free(units[i]);

    }

    /* close the archives */
    srcml_archive_close(oarchive);
    srcml_archive_close(iarchive);

    /* free the srcML archive data */
    srcml_archive_free(oarchive);
    srcml_archive_free(iarchive);

    return 0;
}
