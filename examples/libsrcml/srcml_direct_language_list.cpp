// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_direct_language_list.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * Example program of the use of the libsrcml C API.
 *
 * A null-terminated list of the supported srcML source-code language.
*/

#include <srcml.h>
#include <iostream>

int main(int argc, char* argv[]) {

    // output the supported srcML source-code languages
    for (int i = 0; i < srcml_get_language_list_size(); ++i)
        std::cout << srcml_get_language_list(i) << '\n';

    return 0;
}
