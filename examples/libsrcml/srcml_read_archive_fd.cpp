// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_read_archive_fd.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * Take an archive and extract the invidual units and write to a filesystem.
*/

#include <srcml.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef _MSC_BUILD
#ifndef S_IRUSR
#define S_IRUSR _S_IREAD
#endif
#ifndef S_IWUSR
#define S_IWUSR _S_IWRITE
#endif
#pragma warning(disable : 4996)
#include <io.h>
#else
#include <unistd.h>
#endif

int main(int argc, char* argv[]) {

    // create a new srcml archive structure
    srcml_archive* archive = srcml_archive_create();

    // open a srcML archive for input
    int srcml_input = open("project.xml", O_RDONLY, 0);
    srcml_archive_read_open_fd(archive, srcml_input);

    // add all the files to the archive
    srcml_unit* unit = nullptr;
    while ((unit = srcml_archive_read_unit(archive))) {

        // inquire about the current unit
        std::string language = srcml_unit_get_language(unit);
        std::string filename = srcml_unit_get_filename(unit);

        // uparse and write to a file
        int srcml_output = open(filename.data(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        srcml_unit_unparse_fd(unit, srcml_output);
        close(srcml_output);

        srcml_unit_free(unit);
    }

    // close the srcML archive
    srcml_archive_close(archive);

    // free the archive
    srcml_archive_free(archive);

    return 0;
}
