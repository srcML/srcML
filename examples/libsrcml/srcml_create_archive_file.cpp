// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_archive_create_file.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * Create an archive, file by file, with an output FILE*
*/

#include <srcml.h>
#include <stdio.h>

#ifdef _MSC_BUILD
#pragma warning(disable : 4996)
#endif

int main(int argc, char* argv[]) {

    // create a new srcml archive structure
    srcml_archive* archive = srcml_archive_create();

    // setup our output file using a FILE*
    FILE* srcml_output = fopen("newProject.xml", "w");

    // open a srcML archive for output
    srcml_archive_write_open_FILE(archive, srcml_output);

    // add all the files to the archive
    for (int i = 1; i < argc; ++i) {

        // creae a new unit
        srcml_unit* unit = srcml_unit_create(archive);

        // base the language on the source filename extension
        srcml_unit_set_language(unit, srcml_archive_check_extension(archive, argv[i]));

        // parse the source file
        FILE* srcml_input = fopen(argv[i], "r");
        srcml_unit_parse_FILE(unit, srcml_input);
        fclose(srcml_input);

        // add the srcML of the source file to the archive
        srcml_archive_write_unit(archive, unit);

        // free
        srcml_unit_free(unit);
    }

    // close the srcML archive
    srcml_archive_close(archive);

    // file can now be closed also
    fclose(srcml_output);

    // free the archives
    srcml_archive_free(archive);

    return 0;
}
