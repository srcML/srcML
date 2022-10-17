// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_read_archive_file.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * Extract the archive to the filesystem.
*/

#include <srcml.h>
#include <stdio.h>
#include <string>

#ifdef _MSC_BUILD
#pragma warning(disable : 4996)
#endif

int main(int argc, char* argv[]) {

    // create a new srcml archive structure
    srcml_archive* inputArchive = srcml_archive_create();

    // open a srcML archive for input
    FILE* srcml_input = fopen("project.xml", "r");
    srcml_archive_read_open_FILE(inputArchive, srcml_input);

    // add all the files to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(inputArchive))) {

        // filename of the unit
        std::string filename = srcml_unit_get_filename(unit);

        // uparse and write to a file
        FILE* srcml_output = fopen(filename.data(), "w");
        srcml_unit_unparse_FILE(unit, srcml_output);
        fclose(srcml_output);

        // free the unit
        srcml_unit_free(unit);
    }

    // close the srcML archive
    srcml_archive_close(inputArchive);

    // close the srcML archive file
    fclose(srcml_input);

    // free the archive
    srcml_archive_free(inputArchive);

    return 0;
}
