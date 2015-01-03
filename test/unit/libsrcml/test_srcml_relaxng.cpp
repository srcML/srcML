/**
 * @file test_srcml_relaxng.cpp
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

  Test cases for srcml_xslt
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
      srcml_relaxng
    */

    {
        const char * s = "<unit>a;</unit>";
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        xmlDocPtr doc = xmlReadFile("schema.rng", 0, 0);
        dassert(srcml_relaxng(buffer_input, doc, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_OK);
        std::ifstream in("project.xml");
        std::string output;
        std::string temp;
        while(in >> temp)
            output += temp;
        dassert(output, "<?xmlversion=\"1.0\"encoding=\"\"standalone=\"yes\"?><unit>a;</unit>");
        xmlFreeDoc(doc);
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
        UNLINK("project.xml");
    }

    {
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        xmlDocPtr doc = xmlReadFile("schema.rng", 0, 0);
        dassert(srcml_relaxng(0, doc, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_INVALID_ARGUMENT);

        xmlFreeDoc(doc);
        UNLINK("project.xml");
    }

    {
        const char * s = "<unit>a;</unit>";
        std::ofstream file("input.xml");
        file << s;
        file.close();
        xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
        int fd = OPEN("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        dassert(srcml_relaxng(buffer_input, 0, fd, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_INVALID_ARGUMENT);

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
        xmlDocPtr doc = xmlReadFile("schema.rng", 0, 0);
        dassert(srcml_relaxng(buffer_input, doc, -1, SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL), SRCML_STATUS_INVALID_ARGUMENT);
        xmlFreeDoc(doc);
        xmlFreeParserInputBuffer(buffer_input);
        UNLINK("input.xml");
    }

    srcml_cleanup_globals();

    return 0;

}
