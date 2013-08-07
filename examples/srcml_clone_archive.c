/*
  srcml_clone_archive.c

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

  Clone an archive
*/

#include "srcml.h"

int main(int argc, char* argv[]) {
    int i;
    struct srcml_archive* iarchive;
    struct srcml_archive* oarchive;
    struct srcml_entry* entry;

    /* open up an existing archive */
    iarchive = srcml_read_archive();
    srcml_read_open_filename(iarchive, "project.xml");

    /* create a new srcml archive structure */
    /* options and attributes of cloned archive start the same as
       the original archive */
    oarchive = srcml_clone_archive(iarchive);

    /* open a srcML archive for output */
    srcml_write_open_filename(oarchive, "project2.xml");

    /* copy the files from the input archive to the output archive */
    while (entry = srcml_read_entry_archive(iarchive)) {

        /* Translate to srcml and append to the archive */
        srcml_write_entry_archive(oarchive, entry);

        srcml_free_archive_entry(entry);
    }

    /* close the archives */
    srcml_write_close(oarchive);
    srcml_read_close(iarchive);

    /* free the srcML archive data */
    srcml_write_free(oarchive);
    srcml_write_free(iarchive);

    return 0;
}
