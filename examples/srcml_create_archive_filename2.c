/*
  srcml_create_archive_filename.c

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

  Create an archive, file by file, with an output filename.
*/

#include "srcml.h"

int main(int argc, char* argv[]) {
    int i;

    /* create a new srcml archive structure */
    struct srcml_archive* archive = srcml_write_new_archive();

    /* open a srcML archive for output */
    srcml_write_open_filename(archive, "project.xml");

    /* add all the files to the archive */
    for (i = 0; i < argc; ++i) {

        /* Translate to srcml and append to the archive */
        srcml_write_unit_filename(archive, argv[i]);
    }

    /* close the srcML archive */
    srcml_write_close(archive);

    /* free the srcML archive data */
    srcml_write_free(archive);

    return 0;
}
