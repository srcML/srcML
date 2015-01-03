/**
 * @file test_srcml_global.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*

  Test cases for global functions
*/

#include <srcml_macros.hpp>

#include <stdio.h>
#include <string.h>
#include <cassert>
#include <string>
#include <fstream>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

extern srcml_archive global_archive;

int main() {

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
        dassert(srcml_check_language("C#"), 16);
    }

    {
        dassert(srcml_check_language("Objective-C"), 33);
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
        dassert(srcml_get_language_list(3), std::string("Objective-C"));
    }

    {
        dassert(srcml_get_language_list(4), std::string("Java"));
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
        assert(srcml_check_encoding("UTF-8"));
    }

    {
        assert(!srcml_check_encoding("foo"));
    }

    {
        assert(!srcml_check_encoding(0));
    }

    srcml_cleanup_globals();

    UNLINK("a.cpp");
    UNLINK("project.xml");
    UNLINK("project_c.xml");
    UNLINK("project");
    UNLINK("project_full.xml");
    UNLINK("project.cpp.xml");
    UNLINK("project.c.xml");
    UNLINK("inta.cpp");
    UNLINK("project_full.cpp.xml");

    return 0;

}
