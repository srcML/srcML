/**
 * @file test_srcml_global.cpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <string>
#include <fstream>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include "dassert.hpp"

extern srcml_archive global_archive;


std::string src = "int a;\n";

std::string asrcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><decl_stmt><decl><type><name>int</name></type> <name>a</name></decl>;</decl_stmt>\n"
    "</unit>\n";

std::string srcml_c = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"a.cpp\"><decl_stmt><decl><type><name>int</name></type> <name>a</name></decl>;</decl_stmt>\n"
    "</unit>\n";

std::string srcml_full = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
    "<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"dir\" filename=\"file\" version=\"1\"><s:decl_stmt><s:decl><s:type><s:name>int</s:name></s:type> <s:name>a</s:name></s:decl>;</s:decl_stmt>\n"
    "</s:unit>\n";

int main() {

    std::ofstream src_file("a.cpp");
    src_file << src;
    src_file.close();

    std::ofstream asrcml_file("project.xml");
    asrcml_file << asrcml;
    asrcml_file.close();

    std::ofstream srcml_c_file("project_c.xml");
    srcml_c_file << srcml_c;
    srcml_c_file.close();

    std::ofstream srcml_xml_file("project");
    srcml_xml_file << asrcml;
    srcml_xml_file.close();

    std::ofstream srcml_full_file("project_full.xml");
    srcml_full_file << srcml_full;
    srcml_full_file.close();

    /*
      srcml
    */

    {
        srcml("a.cpp", "project.cpp.xml");
        std::string res_srcml;
        std::ifstream project("project.cpp.xml");
        char c = 0;
        while(project.get(c)) {
            res_srcml += c;
        }

        dassert(res_srcml, asrcml);

    }

    {
        srcml_set_language(SRCML_LANGUAGE_C);
        srcml("a.cpp", "project.c.xml");
        std::string res_srcml;
        std::ifstream project("project.c.xml");
        char c = 0;
        while(project.get(c)) {
            res_srcml += c;
        }

        dassert(res_srcml, srcml_c);

        srcml_set_language(SRCML_LANGUAGE_NONE);

    }

    {

        srcml_set_filename("file");
        srcml_set_directory("dir");
        srcml_set_version("1");
        srcml_register_namespace("s", "http://www.sdml.info/srcML/src");
        srcml("a.cpp", "project_full.cpp.xml");
        std::string res_srcml;
        std::ifstream project("project_full.cpp.xml");
        char c = 0;
        while(project.get(c)) {
            res_srcml += c;
        }

        dassert(res_srcml, srcml_full);

    }

    {
        srcml("project.xml", "inta.cpp");
        std::string res_src;
        std::ifstream project("inta.cpp");
        char c = 0;
        while(project.get(c)) {
            res_src += c;
        }

        dassert(res_src, src);

    }

    {
        srcml_set_language(SRCML_LANGUAGE_XML);
        srcml("project", "inta.cpp");
        std::string res_src;
        std::ifstream project("inta.cpp");
        char c = 0;
        while(project.get(c)) {
            res_src += c;
        }

        dassert(res_src, src);

        srcml_set_language(SRCML_LANGUAGE_NONE);

    }

    {
        srcml("project_c.xml", "inta.cpp");
        std::string res_src;
        std::ifstream project("inta.cpp");
        char c = 0;
        while(project.get(c)) {
            res_src += c;
        }

        dassert(res_src, src);

    }

    {
        srcml("project_full.xml", "inta.cpp");
        std::string res_src;
        std::ifstream project("inta.cpp");
        char c = 0;
        while(project.get(c)) {
            res_src += c;
        }

        dassert(res_src, src);

    }

    {
        dassert(srcml("foo.c", "foo.xml"), SRCML_STATUS_ERROR);
    }

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
        dassert(srcml_check_language("Java"), 8);
    }

    {
        dassert(srcml_check_language("C#"), 32);
    }

    /*
      srcml_get_language_list_size
    */

    {
        dassert(srcml_get_language_list_size(), 4);
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
        dassert(srcml_get_language_list(4), 0);
    }

    /*
      srcml_check_extension
    */

    {
        srcml_register_file_extension("cpp", "C++");
        dassert(srcml_check_extension("a.cpp"), std::string("C++"));
    }

    {
        dassert(srcml_check_extension("a.cpp.gz"), std::string("C++"));
    }

    {
        dassert(srcml_check_extension("a.foo"), 0);
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
