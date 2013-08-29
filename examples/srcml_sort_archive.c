/*
  srcml_sort_archive.c

  Copyright (C) 2013  SDML (www.sdml.info)

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
  Example program of the use of the C API for srcML.

  Sorts an archive in alphabetical filename order.
  Not especially useful, but does show how units can be rearranged.
*/

#include "srcml.h"
#include <string.h>

int main(int argc, char* argv[]) {
    int i;
    struct srcml_archive* iarchive;
    struct srcml_archive* oarchive;
    struct srcml_unit* unit_one;
    struct srcml_unit* unit_two;
    const char* inputfile;
    const char* outputfile;
    bool sorted = false;

    inputfile = "project.xml";
    outputfile = "project_tmp.xml";

    /* open up an existing archive */
    iarchive = srcml_create_archive();

    /* create a new srcml archive structure */
    /* options and attributes of cloned archive start the same as
       the original archive */
    oarchive = srcml_clone_archive(iarchive);

    while (!sorted) {
      sorted = 1;
        srcml_read_open_filename(iarchive, inputfile);

        /* open a srcML archive for output */
        srcml_write_open_filename(oarchive, outputfile);

        /* copy the files from the input archive to the output archive */
        while (true) {

            unit_one = srcml_read_unit(iarchive);
            if (unit_one == 0)
              break;

            unit_two = srcml_read_unit(iarchive);
            if (unit_two == 0) {

              srcml_write_unit(oarchive, unit_one);
              srcml_free_unit(unit_one);
              break;

            }

            if(strcmp(srcml_unit_get_filename(unit_one), srcml_unit_get_filename(unit_two)) <=0) {
              srcml_write_unit(oarchive, unit_one);
              srcml_write_unit(oarchive, unit_two);

            } else {

              sorted = 0;
              srcml_write_unit(oarchive, unit_two);
              srcml_write_unit(oarchive, unit_one);

            }
              

            /* Translate to srcml and append to the archive */

            srcml_free_unit(unit_one);
            srcml_free_unit(unit_two);
        }

        /* close the archives */
        srcml_close_archive(oarchive);
        srcml_close_archive(iarchive);
    }

    /* free the srcML archive data */
    srcml_free_archive(oarchive);
    srcml_free_archive(iarchive);

    return 0;
}
