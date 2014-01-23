/*
  srcml_create_archive_fd.c

  Copyright (C) 2013-2014  SDML (www.srcML.org)

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
  Example program of the use of the C API for srcML.

  Create an archive, file by file, with an output file descriptor
*/

#include "srcml.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int i;
    int srcml_output;
    int srcml_input;
    struct srcml_archive* archive;
    struct srcml_unit* unit;

    /* create a new srcml archive structure */
    archive = srcml_create_archive();

    /* setup our output file using a file descriptor */
    srcml_output = open("project.xml", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    /* open a srcML archive for output */
    srcml_write_open_fd(archive, srcml_output);

    /* add all the files to the archive */
    for (i = 1; i < argc; ++i) {

        unit = srcml_create_unit(archive);
     
        srcml_unit_set_language(unit, srcml_archive_check_extension(archive, argv[i]));

        /* Translate to srcml */
        srcml_input = open(argv[i], O_RDONLY);
        srcml_parse_unit_fd(unit, srcml_input);

        /* Append to the archive */
        srcml_write_unit(archive, unit);

        srcml_free_unit(unit);
        close(srcml_input);
    }

    /* close the srcML archive */
    srcml_close_archive(archive);

    /* file can now be closed also */
    close(srcml_output);

    /* free the srcML archive data */
    srcml_free_archive(archive);

    return 0;
}
