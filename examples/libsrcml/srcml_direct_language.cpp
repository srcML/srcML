// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_direct_language.cpp
 *
 * @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * A straightforward translation of source code to the srcML format.
 * Translates the file "decl_stmt.cpp" to the srcML format in "decl_stmt.cpp.xml", however
 * in this case the language is specified:
 *
 * * This creates a single-unit srcML file, i.e., a non-archive srcML
 * * The srcML attribute filename is the name of the file passed as the first
 * parameter.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    // translate from a source-code file to a srcML file
    srcml_set_language(SRCML_LANGUAGE_CXX);
    srcml("decl_stmt.cpp", "decl_stmt.cpp.xml");

    return 0;
}
