/**
 * @file test_srcml_global.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*

  Test cases for global functions
*/

#include <srcml.h>

#include <macros.hpp>

#include <string>
#include <fstream>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include <dassert.hpp>

extern srcml_archive global_archive;
extern srcml_unit global_unit;

std::string src = "int a;\n";

std::string asrcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp" hash="56f54d1636dfec63c3e1586e5e4bdc9a455bb9f6"><decl_stmt><decl><type><name>int</name></type> <name>a</name></decl>;</decl_stmt>
</unit>
)";

std::string srcml_c = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C" filename="a.cpp" hash="56f54d1636dfec63c3e1586e5e4bdc9a455bb9f6"><decl_stmt><decl><type><name>int</name></type> <name>a</name></decl>;</decl_stmt>
</unit>
)";

std::string srcml_full = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" url="url" filename="file" version="1" hash="56f54d1636dfec63c3e1586e5e4bdc9a455bb9f6"><s:decl_stmt><s:decl><s:type><s:name>int</s:name></s:type> <s:name>a</s:name></s:decl>;</s:decl_stmt>
</s:unit>
)";

int main(int, char* argv[]) {

    std::ofstream src_file("a.cpp");
    src_file << src;
    src_file.close();

    std::ofstream asrcml_file("project.xml");
    asrcml_file << asrcml;
    asrcml_file.close();

    std::ofstream srcml_ext_file("project.srcML");
    srcml_ext_file << asrcml;
    srcml_ext_file.close();

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
        std::ifstream project("project.cpp.xml");
        std::string res_srcml((std::istreambuf_iterator<char>(project)), std::istreambuf_iterator<char>());

        dassert(res_srcml, asrcml);
    }

    {
        srcml_set_language(SRCML_LANGUAGE_C);
        srcml("a.cpp", "project.c.xml");
        std::ifstream project("project.c.xml");
        std::string res_srcml((std::istreambuf_iterator<char>(project)), std::istreambuf_iterator<char>());

        dassert(res_srcml, srcml_c);

        srcml_set_language(SRCML_LANGUAGE_NONE);
    }

    {
        srcml_set_filename("file");
        srcml_set_url("url");
        srcml_set_version("1");
        srcml_register_namespace("s", "http://www.srcML.org/srcML/src");
        srcml("a.cpp", "project_full.cpp.xml");
        std::ifstream project("project_full.cpp.xml");
        std::string res_srcml((std::istreambuf_iterator<char>(project)), std::istreambuf_iterator<char>());

        dassert(res_srcml, srcml_full);
    }

    {
        srcml("project.xml", "inta.cpp");
        std::ifstream project("inta.cpp");
        std::string res_src((std::istreambuf_iterator<char>(project)), std::istreambuf_iterator<char>());

        dassert(res_src, src);
    }

    {
        srcml("project.srcML", "inta.cpp");
        std::ifstream project("inta.cpp");
        std::string res_src((std::istreambuf_iterator<char>(project)), std::istreambuf_iterator<char>());

        dassert(res_src, src);
    }

    {
        srcml_set_language(SRCML_LANGUAGE_XML);
        srcml("project", "inta.cpp");
        std::ifstream project("inta.cpp");
        std::string res_src((std::istreambuf_iterator<char>(project)), std::istreambuf_iterator<char>());

        dassert(res_src, src);

        srcml_set_language(SRCML_LANGUAGE_NONE);
    }

    {
        srcml("project_c.xml", "inta.cpp");
        std::ifstream project("inta.cpp");
        std::string res_src((std::istreambuf_iterator<char>(project)), std::istreambuf_iterator<char>());

        dassert(res_src, src);
    }

    {
        srcml("project_full.xml", "inta.cpp");
        std::ifstream project("inta.cpp");
        std::string res_src((std::istreambuf_iterator<char>(project)), std::istreambuf_iterator<char>());

        dassert(res_src, src);
    }

    {
        dassert(srcml("foo.c", "foo.xml"), SRCML_STATUS_IO_ERROR);
    }

    srcml_cleanup_globals();

    UNLINK("a.cpp");
    UNLINK("project.xml");
    UNLINK("project.srcML");
    UNLINK("project_c.xml");
    UNLINK("project");
    UNLINK("project_full.xml");
    UNLINK("project.cpp.xml");
    UNLINK("project.c.xml");
    UNLINK("inta.cpp");
    UNLINK("project_full.cpp.xml");

    return 0;
}
