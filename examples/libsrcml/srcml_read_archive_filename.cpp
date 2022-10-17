// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_read_archive_filename.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * Take an archive and extract the invidual units and write to a filesystem.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    // open a srcML archive for input
    srcml_archive* archive = srcml_archive_create();
    srcml_archive_read_open_filename(archive, "project.xml");

    // add all the files to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(archive))) {

        // can inquire about the current unit
        const char* language = srcml_unit_get_language(unit);
        const char* filename = srcml_unit_get_filename(unit);

        // uparse and write to a file
        srcml_unit_unparse_filename(unit, filename);

        srcml_unit_free(unit);
    }

    // close the srcML archive
    srcml_archive_close(archive);

    // free the archives
    srcml_archive_free(archive);

    return 0;
}
