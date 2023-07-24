// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_archive_create_memory.cpp
 *
 * @copyright Copyright (C) 2013-2023 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * Create an archive, file by file, with an output memory.
*/

#include <srcml.h>
#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {

    // open a srcML archive for memory output
    srcml_archive* archive = srcml_archive_create();
    char* s = nullptr;
    size_t size;
    srcml_archive_write_open_memory(archive, &s, &size);

    // translate to the code to srcml and append to the archive
    const char* code = "int n = 0;";
    srcml_unit* unit = srcml_unit_create(archive);
    srcml_unit_set_language(unit, "C++");
    srcml_unit_parse_memory(unit, code, strlen(code));
    srcml_archive_write_unit(archive, unit);

    // free the unit
    srcml_unit_free(unit);

    // close the srcML archive
    srcml_archive_close(archive);

    // free the archive
    srcml_archive_free(archive);

    // output the contents of the archive
    std::cout << s;

    // free the generated archive
    free(s);

    return 0;
}
