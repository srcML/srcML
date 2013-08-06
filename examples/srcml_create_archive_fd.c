/*
  srcml_create_archive_fd.c

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

  Create an archive, file by file, with an output file descriptor
*/

#include "srcml.h"
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int i;
    int srcml_output;

    /* create a new srcml archive structure */
    struct srcml_archive* archive = src2srcml_new_archive();

    /* setup our output file using a file descriptor */
    srcml_output = open("project.xml", O_WRONLY | O_CREAT);

    /* open a srcML archive for output */
    src2srcml_open_fd(archive, srcml_output);

    /* add all the files to the archive */
    for (i = 0; i < argc; ++i) {

        /* Translate to srcml and append to the archive */
        src2srcml_unit_filename(archive, argv[i]);
    }

    /* close the srcML archive */
    src2srcml_close(archive);

    /* file can now be closed also */
    close(srcml_output);

    /* free the srcML archive data */
    src2srcml_free(archive);

    return 0;
}
