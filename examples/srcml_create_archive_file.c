/*
  srcml_create_archive_file.c

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

  Create an archive, file by file, with an output FILE*
*/

#include "srcml.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    int i;
    struct srcml_archive* archive;
    FILE* srcml_output;
    FILE* srcml_input;

    /* create a new srcml archive structure */
    archive = srcml_create_archive();

    /* setup our output file using a FILE* */
    srcml_output = fopen("project.xml", "w");

    /* open a srcML archive for output */
    srcml_write_open_FILE(archive, srcml_output);

    /* add all the files to the archive */
    for (i = 1; i < argc; ++i) {

        struct srcml_unit* unit = srcml_create_unit(archive);

        srcml_unit_set_language(unit, srcml_archive_check_extension(archive, argv[i]));

        srcml_input = fopen(argv[i], "r");
        srcml_parse_unit_FILE(unit, srcml_input);

        srcml_write_unit(archive, unit);

        srcml_free_unit(unit);
        fclose(srcml_input);
    }

    /* close the srcML archive */
    srcml_close_archive(archive);

    /* file can now be closed also */
    fclose(srcml_output);

    /* free the srcML archive data */
    srcml_free_archive(archive);

    return 0;
}
