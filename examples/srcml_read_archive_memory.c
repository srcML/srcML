/*
  srcml_read_archive_full.c

  Copyright (C) 2013  SDML (www.srcML.org)

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

  Take an archive and extract the invidual units and write to a filesystem.
*/

#include "srcml.h"
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    const char* language;
    const char* filename;
    struct srcml_archive* archive;
    struct srcml_unit* unit;
    int srcml_input;
    char s[1024];

    /* create a new srcml archive structure */
    archive = srcml_create_archive();

    /* open a srcML archive for input */
    srcml_input = open("project.xml", O_RDONLY);
    read(srcml_input, s, 1024);
    close(srcml_input);
    srcml_read_open_memory(archive, s, strlen(s));

    /* add all the files to the archive */
    while (unit = srcml_read_unit(archive)) {

        /* can inquire about the current unit */
        language = srcml_unit_get_language(unit);
        filename = srcml_unit_get_filename(unit);

        /* uparse and write to a file */
        char * buffer;
        srcml_unparse_unit_memory(unit, &buffer);
        printf("%s", buffer);
        free(buffer);
        srcml_free_unit(unit);
    }

    /* close the srcML archive */
    srcml_close_archive(archive);

    /* free the srcML archive data */
    srcml_free_archive(archive);

    return 0;
}
