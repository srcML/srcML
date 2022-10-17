// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_archive_check_extension.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_archive_check_extension()
 */

#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    /*
      srcml_check_extension
    */

    srcml_archive* archive = srcml_archive_create();
    srcml_archive_register_file_extension(archive, "h", "C++");

    {
        dassert(srcml_archive_check_extension(archive, "a.h"), std::string("C++"));
    }

    {
        dassert(srcml_archive_check_extension(archive, "a.h.gz"), std::string("C++"));
    }

    {
        dassert(srcml_archive_check_extension(archive, "a.foo"), 0);
    }

    {
        dassert(srcml_archive_check_extension(archive, 0), 0);
    }

    {
        dassert(srcml_archive_check_extension(0, "a.h"), 0);
    }

    srcml_archive_free(archive);

    return 0;
}
