/**
 * @file test_srcml_xpath.cpp
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

  Test cases for srcml_xpath
*/

#include <srcml_macros.hpp>

#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include <srcml_sax2_utilities.hpp>
#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

int main() {

    /*
      srcml_xpath
    */

    {
        const char * s = "<unit>a;</unit>";
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", 0, 0, 0, 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        std::string temp;
        while(in >> temp)
            output += temp;
        dassert(output, "<?xmlversion=\"1.0\"encoding=\"\"standalone=\"yes\"?><unit/>");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(0, "src:unit", "//src:unit", 0, 0, 0, 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_INVALID_ARGUMENT);
        UNLINK("project.xml");
    }

    {
        const char * s = "<unit>a;</unit>";
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, 0, "//src:unit", 0, 0, 0, 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_INVALID_ARGUMENT);
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<unit>a;</unit>";
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", 0, 0, 0, 0, 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_INVALID_ARGUMENT);
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<unit>a;</unit>";
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", 0, 0, 0, 0, 0, 0, 0, -1, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_INVALID_ARGUMENT);
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
    }

    /**
        simple xpath generation
     */

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl", 0, 0, 0, 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\" item=\"1\" location=\"/src:function_decl[1]\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl></unit>\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\" item=\"2\" location=\"/src:function_decl[2]\"><function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl></unit>\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\" item=\"3\" location=\"/src:function_decl[3]\"><function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl", 0, 0, 0, 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"
                        "<unit language=\"C++\" filename=\"a.cpp\" item=\"1\" location=\"/src:function_decl[1]\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl></unit>\n\n"
                        "<unit language=\"C++\" filename=\"a.cpp\" item=\"2\" location=\"/src:function_decl[2]\"><function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl></unit>\n\n"
                        "<unit language=\"C++\" filename=\"a.cpp\" item=\"3\" location=\"/src:function_decl[3]\"><function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl[src:name='g']", 0, 0, 0, 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\" item=\"1\" location=\"/src:function_decl[2]\"><function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl//src:param[src:decl/src:name='b']", 0, 0, 0, 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\" item=\"1\" location=\"/src:function_decl[2]/src:parameter_list[1]/src:param[2]\"><param><decl><type><name>double</name></type> <name>b</name></decl></param></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    /*
        srcml_xpath inserting attribute
    */

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", 0, 0, 0, "foo", "bar", "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\" foo:name=\"value\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"

        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>\n\n"
        "</unit>\n";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", 0, 0, 0, "foo", "bar", "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\" foo:name=\"value\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl", 0, 0, 0, "foo", "bar", "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl foo:name=\"value\"><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl foo:name=\"value\"><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl foo:name=\"value\"><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl", 0, 0, 0, 0, "http://www.srcML.org/srcML/src", "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl name=\"value\"><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl name=\"value\"><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl name=\"value\"><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl", 0, 0, 0, "cpp", "http://www.srcML.org/srcML/cpp", "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl cpp:name=\"value\"><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl cpp:name=\"value\"><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl cpp:name=\"value\"><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    /*
        srcml_xpath inserting element
    */

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", "foo", "bar", "result", 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><foo:result><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</foo:result></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"

        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>\n\n"
        "</unit>\n";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", "foo", "bar", "result", 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><foo:result><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</foo:result></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl", "foo", "bar", "result", 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><foo:result><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl></foo:result>\n\n"

                        "<foo:result><function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl></foo:result>\n\n"

                        "<foo:result><function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl></foo:result>\n\n"

                        "</unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl", 0, "http://www.srcML.org/srcML/src", "result", 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><result><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl></result>\n\n"

                        "<result><function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl></result>\n\n"

                        "<result><function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl></result>\n\n"

                        "</unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:function_decl", "cpp", "http://www.srcML.org/srcML/src", "result", 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><cpp:result><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl></cpp:result>\n\n"

                        "<cpp:result><function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl></cpp:result>\n\n"

                        "<cpp:result><function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl></cpp:result>\n\n"

                        "</unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }



    /*
        srcml_xpath inserting element with attributes
    */

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", "foo", "bar", "result", 0, 0, "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><foo:result foo:name=\"value\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</foo:result></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", "foo", "bar", "result", "foobar", "foobar", "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\" xmlns:foobar=\"foobar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><foo:result foobar:name=\"value\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</foo:result></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", 0, 0, "result", "foo", "bar", "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><result foo:name=\"value\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</result></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", 0, 0, "result", 0, 0, "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><result name=\"value\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</result></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

        "</unit>";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", "foo", "bar", "result", "foo", "bar", "name", "value", fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n"
                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><foo:result foo:name=\"value\"><function_decl><type><name>void</name></type> <name>f</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>g</name><parameter_list>(<param><decl><type><name>int</name></type> <name>a</name></decl></param>, <param><decl><type><name>double</name></type> <name>b</name></decl></param>)</parameter_list>;</function_decl>\n\n"

                        "<function_decl><type><name>void</name></type> <name>h</name><parameter_list>()</parameter_list>;</function_decl>\n\n"

                        "</foo:result></unit>\n\n"
                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }


    {
        const char * s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                         "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\"> <empty_stmt>;</empty_stmt>\n"
                          "</unit>\n";
        
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_xpath(buffer_input, "src:unit", "//src:unit", 0, 0, 0, 0, 0, 0, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        char temp;
        while(in.get(temp))
            output += temp;

        dassert(output, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
                        "<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n"

                        "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\"> <empty_stmt>;</empty_stmt>\n"
                        "</unit>\n\n"

                        "</unit>\n");
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }    

    srcml_cleanup_globals();

    return 0;

}
