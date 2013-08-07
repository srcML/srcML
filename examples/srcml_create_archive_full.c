/*
  srcml_create_archive_full.c

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

  Create an archive, file by file, with an output filename, showing
  most of the option features
*/

#include "srcml.h"

int main(int argc, char* argv[]) {
    int i;

    /* create a new srcml archive structure */
    struct srcml_archive* archive = srcml_write_new_archive();

    /* setup some options and attributes */
    srcml_set_options(archive, SRCML_OPTION_LITERAL | SRCML_OPTION_MODIFIER | SRCML_OPTION_POSITION);
    srcml_set_language(archive, SRCML_LANGUAGE_CXX);
    srcml_set_version(archive, "211");
    srcml_set_tabstop(archive, 4);

    /* treat "*.h" as C++ */
    srcml_register_file_extension(archive, "h", SRCML_LANGUAGE_CXX);

    /* change prefix of standard namespace */
    srcml_register_namespace(archive, "s", "http://www.sdml.info/srcML/src");
    
    /* default prefix is now for cpp namespace */
    srcml_register_namespace(archive, "", "http://www.sdml.info/srcML/cpp");

    /* new prefix for further processing */
    srcml_register_namespace(archive, "doc", "http://www.sdml.info/srcML/doc");

    /* open a srcML archive for output */
    srcml_write_open_filename(archive, "project.xml");

    /* add all the files to the archive */
    for (i = 0; i < argc; ++i) {

        /* setup this unit.  may be different for each entry */
        srcml_entry_set_language(archive, SRCML_LANGUAGE_C);
        srcml_entry_set_filename(archive, argv[i]);

        /* Translate to srcml and append to the archive */
        srcml_write_entry_filename(archive, argv[i]);
    }

    /* close the srcML archive */
    srcml_write_close(archive);

    /* free the srcML archive data */
    srcml_write_free(archive);

    return 0;
}
