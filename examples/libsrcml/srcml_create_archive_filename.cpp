// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_archive_create_filename.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * Create an archive, file by file, with an output filename.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    // create a new srcml archive structure
    srcml_archive* archive = srcml_archive_create();

    // open a srcML archive for output
    srcml_archive_write_open_filename(archive, "newProject.xml");

    // add all the files to the archive
    for (int i = 1; i < argc; ++i) {

        srcml_unit* unit = srcml_unit_create(archive);

        // set the filename
        srcml_unit_set_filename(unit, argv[i]);

        // translate to srcml
        srcml_unit_parse_filename(unit, argv[i]);

        // append to the archive
        srcml_archive_write_unit(archive, unit);

        srcml_unit_free(unit);
    }

    // close the srcML archive
    srcml_archive_close(archive);

    // free the archives
    srcml_archive_free(archive);

    return 0;
}
