// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_direct_src2srcml.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * A straightforward translation of source code to the srcML format.
 * Translates the file "decl_stmt.cpp" to the srcML format in "decl_stmt.cpp.xml":
 *
 * * The language is determined automatically from the source file extension
 * * This creates a single-unit srcML file, i.e., a non-archive srcML
 * * The srcML attribute filename will be the name of the file passed as the first
 * parameter.
*/

#include <srcml.h>
#include <iostream>

int main(int argc, char* argv[]) {

    // translate from a source-code file to a srcML file
    int error = srcml("decl_stmt.cpp", "decl_stmt.cpp.xml");

    // output the error
    if (error)
        std::cerr << srcml_error_string();

    return 0;
}
