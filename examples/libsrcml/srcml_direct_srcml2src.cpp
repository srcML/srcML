// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_direct_srcml2src.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * A straightforward translation from the srcML format back to source code.
 * Translates the srcML file "decl_stmt.cpp.xml" to the source-code file "decl_stmt.cpp":
 *
 * * This creates a single-unit srcML file, i.e., a non-archive srcML
 * * The srcML attribute filename will be the name of the file passed as the first
 * parameter.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    // translate from a srcML file to a source-code file
    srcml("decl_stmt.cpp.xml", "decl_stmt.cpp");

    return 0;
}
