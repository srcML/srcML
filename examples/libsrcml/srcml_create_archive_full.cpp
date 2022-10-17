// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_archive_create_full.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * Create an archive, file by file, with an output filename, showing
 * most of the option features
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    // create a new srcml archive
    srcml_archive* archive = srcml_archive_create();

    // setup options and attributes
    srcml_archive_set_version(archive, "211");
    srcml_archive_set_tabstop(archive, 4);

    // treat "*.h" as C++
    srcml_archive_register_file_extension(archive, "h", SRCML_LANGUAGE_CXX);

    // change prefix of standard namespace
    srcml_archive_register_namespace(archive, "s", "http://www.sdml.info/srcML/src");

    // default prefix is now for cpp namespace
    srcml_archive_register_namespace(archive, "", "http://www.sdml.info/srcML/cpp");

    // new prefix for further processing
    srcml_archive_register_namespace(archive, "doc", "http://www.sdml.info/srcML/doc");

    // open a srcML archive for output
    srcml_archive_write_open_filename(archive, "newProject.xml");

    // add all files on the command line to the archive
    for (int i = 1; i < argc; ++i) {

        // setup this entry
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, argv[i]);

        // translate the entry to srcML
        srcml_unit_parse_filename(unit, argv[i]);

        // append unit to the archive
        srcml_archive_write_unit(archive, unit);

        // done with the entry
        srcml_unit_free(unit);
    }

    // close the srcML archive
    srcml_archive_close(archive);

    // free the archives
    srcml_archive_free(archive);

    return 0;
}
