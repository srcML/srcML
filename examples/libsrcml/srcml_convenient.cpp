// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_convenient.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the convenience funcion C API for srcML.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    // setup options and attributes
    srcml_set_version("211");
    srcml_set_tabstop(4);

    // treat "*.h" as C++
    srcml_register_file_extension("h", SRCML_LANGUAGE_CXX);

    // change prefix of standard namespace
    srcml_register_namespace("s", "http://www.sdml.info/srcML/src");

    // default prefix is now for cpp namespace
    srcml_register_namespace("", "http://www.sdml.info/srcML/cpp");

    // new prefix for further processing
    srcml_register_namespace("doc", "http://www.sdml.info/srcML/doc");

    /*
      Translates source code file "main.cpp" to srcML file "main.cpp.xml".
      - Translate using the above global options
      - The language is automatica and based on the extension of the input (first) filename
      - Since there is only a single input file, the output file will be a non-archive by default.
      Convenience function can also convert to archive
    */
    srcml("main.cpp", "main.cpp.xml");

    return 0;
}
