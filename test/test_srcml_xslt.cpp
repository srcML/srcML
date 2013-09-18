/*
  test_srcml_xslt.cpp

  Copyright (C) 2013  SDML (www.sdml.info)

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*

  Test cases for srcml_xslt
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>

#include <srcml_sax2_utilities.hpp>
#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  /* 
     srcml_xslt
   */

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    const char * xslts[2] = {"copy.xsl", 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xslt(buffer_input, "src:unit", xslts, 0, 0, fd, 0) == SRCML_STATUS_OK);
    std::ifstream in("project.xml");
    std::string output;
    std::string temp;
    while(in >> temp)
      output += temp;
    assert(output == "<?xmlversion=\"1.0\"encoding=\"\"standalone=\"yes\"?><unit>a;</unit>");
    unlink("input.xml");
    unlink("project.xml");
  }

  {
    const char * xslts[2] = {"copy.xsl", 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xslt(0, "src:unit", xslts, 0, 0, fd, 0) == SRCML_STATUS_ERROR);
    unlink("project.xml");
  }

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    const char * xslts[2] = {"copy.xsl", 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xslt(buffer_input, 0, xslts, 0, 0, fd, 0) == SRCML_STATUS_ERROR);
    unlink("input.xml");
    unlink("project.xml");
  }

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xslt(buffer_input, "src:unit", 0, 0, 0, fd, 0) == SRCML_STATUS_ERROR);
    unlink("input.xml");
    unlink("project.xml");
  }

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    const char * xslts[2] = {0, 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xslt(buffer_input, "src:unit", xslts, 0, 0, fd, 0) == SRCML_STATUS_ERROR);
    unlink("input.xml");
    unlink("project.xml");
  }

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    const char * xslts[2] = {"copy.xsl", 0 };
    assert(srcml_xslt(buffer_input, "src:unit", xslts, 0, 0, -1, 0) == SRCML_STATUS_ERROR);
    unlink("input.xml");
  }

  return 0;

}
