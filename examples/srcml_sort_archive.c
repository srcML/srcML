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
  int num_units = 0;
  struct srcml_unit* units[10];
  const char* inputfile;
  const char* outputfile;

  inputfile = "project.xml";
  outputfile = "project_tmp.xml";

  /* open up an existing archive */
  iarchive = srcml_create_archive();

  /* create a new srcml archive structure */
  /* options and attributes of cloned archive start the same as
     the original archive */
  oarchive = srcml_clone_archive(iarchive);

  while (true) {

    srcml_read_open_filename(iarchive, inputfile);

    units[num_units] = srcml_read_unit(iarchive);
    if (units[num_units] == 0)
      break;
    ++num_units;

  }

  for(int i = 1; i < num_units; ++i) {

    for(int j = i; j > 0; --j) {

      if(strcmp(srcml_unit_get_filename(units[j]), srcml_unit_get_filename(units[j - 1])) < 0) {

        srcml_unit * tmp_unit = units[j];
        units[j] = units[j - 1];
        units[j - 1] = tmp_unit;

      } else
        break;

    }

  }

  /* open a srcML archive for output */
  srcml_write_open_filename(oarchive, outputfile);

  for(int i = 0; i < num_units; ++i) {

    /* copy the files from the input archive to the output archive */
    /* Translate to srcml and append to the archive */
    srcml_write_unit(oarchive, units[i]);

    srcml_free_unit(units[i]);

  }

  /* close the archives */
  srcml_close_archive(oarchive);
  srcml_close_archive(iarchive);

  /* free the srcML archive data */
  srcml_free_archive(oarchive);
  srcml_free_archive(iarchive);

return 0;
}
