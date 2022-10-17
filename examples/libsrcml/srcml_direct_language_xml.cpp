// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_direct_language_xml.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * A straightforward translation from the srcML format back to source code.
 * Translates the srcML file "decl_stmt.cpp.xml" to the source-code file "decl_stmt.cpp", however
 * in this case the language is specified:
 *
 * * This creates a single-unit srcML file, i.e., a non-archive srcML
 * * The srcML attribute filename is the name of the file passed as the first
 * parameter.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    // translate from a source-code file to a srcML file
    srcml_set_language(SRCML_LANGUAGE_XML);
    srcml("decl_stmt.cpp.xml", "decl_stmt.cpp");

    return 0;
}
