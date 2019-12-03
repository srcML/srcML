/**
 * @file srcml_archive_create_full.c
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

  Create an archive, file by file, with an output filename, showing
  most of the option features
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    /*
      Setup archive
    */

    /* create a new srcml archive structure */
    struct srcml_archive* archive = srcml_archive_create();

    /* setup options and attributes */
    srcml_archive_set_version(archive, "211");
    srcml_archive_set_tabstop(archive, 4);

    /* treat "*.h" as C++ */
    srcml_archive_register_file_extension(archive, "h", SRCML_LANGUAGE_CXX);

    /* change prefix of standard namespace */
    srcml_archive_register_namespace(archive, "s", "http://www.sdml.info/srcML/src");

    /* default prefix is now for cpp namespace */
    srcml_archive_register_namespace(archive, "", "http://www.sdml.info/srcML/cpp");

    /* new prefix for further processing */
    srcml_archive_register_namespace(archive, "doc", "http://www.sdml.info/srcML/doc");

    /*
      Open and write to the archive
    */

    /* open a srcML archive for output */
    srcml_archive_write_open_filename(archive, "project.xml");

    /* add all files on the command line to the archive */
    for (int i = 1; i < argc; ++i) {

        /* Setup this entry */
        struct srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, SRCML_LANGUAGE_C);
        srcml_unit_set_filename(unit, argv[i]);

        /* Translate the entry to srcML */
        srcml_unit_parse_filename(unit, argv[i]);

        /* Append unit to the archive */
        srcml_archive_write_unit(archive, unit);

        /* Done with the entry for now */
        srcml_unit_free(unit);
    }

    /*
      Finish up
    */

    /* close the srcML archive */
    srcml_archive_close(archive);

    /* free the srcML archive data */
    srcml_archive_free(archive);

    return 0;
}
