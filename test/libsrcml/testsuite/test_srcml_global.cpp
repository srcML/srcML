// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_global.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for global functions
 */

#include <srcml.h>

#include <string>
#include <fstream>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif

#include <dassert.hpp>

int main(int, char* argv[]) {

    /*
      srcml_check_language
    */

    {
        dassert(srcml_check_language("C"), 1);
    }

    {
        dassert(srcml_check_language("C++"), 2);
    }

    {
        dassert(srcml_check_language("Java"), 4);
    }

    {
        dassert(srcml_check_language("C#"), 8);
    }

    {
        dassert(srcml_check_language("Objective-C"), 17);
    }

    /*
      srcml_get_language_list_size
    */

    {
        dassert(srcml_get_language_list_size(), 5);
    }

    /*
      srcml_get_language_list
    */

    {
        dassert(srcml_get_language_list(0), std::string("C"));
    }

    {
        dassert(srcml_get_language_list(1), std::string("C++"));
    }

    {
        dassert(srcml_get_language_list(2), std::string("C#"));
    }

    {
        dassert(srcml_get_language_list(3), std::string("Java"));
    }

    {
        dassert(srcml_get_language_list(4), std::string("Objective-C"));
    }

    {
        dassert(srcml_get_language_list(5), 0);
    }

    /*
      srcml_check_extension
    */

    {
        dassert(srcml_check_extension("a.cpp"), std::string("C++"));
    }

    {
        dassert(srcml_check_extension("a.cpp.gz"), std::string("C++"));
    }

    {
        srcml_register_file_extension("foo", "C++");
        dassert(srcml_check_extension("a.foo"), std::string("C++"));
    }

    {
        dassert(srcml_check_extension("a.foo.gz"), std::string("C++"));
    }

    {
        dassert(srcml_check_extension("a.bar"), 0);
    }

    {
        dassert(srcml_check_extension(0), 0);
    }

    /*
      srcml_check_encoding
    */

    {
        dassert(srcml_check_encoding("UTF-8"), 1);
    }

    {
        /*
           valgrind produces the error "Conditional jump or move depends on uninitialised value(s)"
           based on a strcmp() inside of xml. For a long, invalid name, no error is produced
        */
        dassert(srcml_check_encoding("UTF-64"), 0);
    }

    {
        dassert(srcml_check_encoding(0), 0);
    }

    srcml_cleanup_globals();

    unlink("a.cpp");
    unlink("project.xml");
    unlink("project_c.xml");
    unlink("project");
    unlink("project_full.xml");
    unlink("project.cpp.xml");
    unlink("project.c.xml");
    unlink("inta.cpp");
    unlink("project_full.cpp.xml");

    return 0;
}
