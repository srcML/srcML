/**
 * @file test_srcml_extract_text.cpp
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

  Test cases for srcml_extract_text
*/

#include <srcml_macros.hpp>

#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#endif
#include <fcntl.h>

#include <srcml_sax2_utilities.hpp>
#include <srcml.h>
// #include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <libxml/xmlIO.h>

#include <unit_tests.hpp>

int main(int, char* []) {

#if 0
    /*
      srcml_extract_text
    */

    {
        const char * s = "<unit>a;</unit>";

        xmlOutputBufferPtr output_handler = xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0);
//        dassert(srcml_extract_text(s, strlen(s), output_handler, 0, boost::optional<size_t>(), 0), SRCML_STATUS_OK);
        xmlOutputBufferClose(output_handler);

        std::ifstream in("project.xml");
        std::string output;
        std::string temp;
        while(in >> temp)
            output += temp;

        dassert(output, "a;");
        UNLINK("project.xml");

    }

    {
        const char * s = "<unit xmlns:diff=\"http://www.srcML.org/srcDiff\"><diff:delete></diff:delete>a;<diff:insert>b;</diff:insert></unit>";

        xmlOutputBufferPtr output_handler = xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0);
//        dassert(srcml_extract_text(s, strlen(s), output_handler, 0, boost::optional<size_t>(), 0), SRCML_STATUS_OK);
        xmlOutputBufferClose(output_handler);

        std::ifstream in("project.xml");
        std::string output;
        std::string temp;
        while(in >> temp)
            output += temp;

        dassert(output, "a;b;");
        UNLINK("project.xml");

    }

    {
        const char * s = "<unit xmlns:diff=\"http://www.srcML.org/srcDiff\"><diff:delete></diff:delete>a;<diff:insert>b;</diff:insert></unit>";

        xmlOutputBufferPtr output_handler = xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0);
//        dassert(srcml_extract_text(s, strlen(s), output_handler, 0, boost::optional<size_t>(SRCDIFF_REVISION_ORIGINAL), 0), SRCML_STATUS_OK);
        xmlOutputBufferClose(output_handler);

        std::ifstream in("project.xml");
        std::string output;
        std::string temp;
        while(in >> temp)
            output += temp;

        dassert(output, "a;");
        UNLINK("project.xml");

    }

    {
        const char * s = "<unit xmlns:diff=\"http://www.srcML.org/srcDiff\"><diff:delete></diff:delete>a;<diff:insert>b;</diff:insert></unit>";

        xmlOutputBufferPtr output_handler = xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0);
//        dassert(srcml_extract_text(s, strlen(s), output_handler, 0, boost::optional<size_t>(SRCDIFF_REVISION_MODIFIED), 0), SRCML_STATUS_OK);
        xmlOutputBufferClose(output_handler);

        std::ifstream in("project.xml");
        std::string output;
        std::string temp;
        while(in >> temp)
            output += temp;

        dassert(output, "a;b;");
        UNLINK("project.xml");

    }

    {
        const char * s = "<unit>a;</unit>";
        xmlOutputBufferPtr output_handler = xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0);
//        dassert(srcml_extract_text(0, strlen(s), output_handler, 0, boost::optional<size_t>(), 0), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        const char * s = "<unit>a;</unit>";
        xmlOutputBufferPtr output_handler = xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0);
//        dassert(srcml_extract_text(s, 0, output_handler, 0, boost::optional<size_t>(), 0), SRCML_STATUS_INVALID_ARGUMENT);
    }

    srcml_cleanup_globals();
#endif
    
    return 0;

}
