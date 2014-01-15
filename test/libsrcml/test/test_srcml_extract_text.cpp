/*
  test_srcml_extract_text.cpp

  Copyright (C) 2013-2014  SDML (www.srcML.org)

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*

  Test cases for srcml_extract_text
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

#include "dassert.hpp"

int main() {

  /* 
     srcml_extract_text
   */

  {
    const char * s = "<unit>a;</unit>";
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0);
    dassert(srcml_extract_text(s, strlen(s), output_buffer, 0, 0), SRCML_STATUS_OK);
    std::ifstream in("project.xml");
    std::string output;
    std::string temp;
    while(in >> temp)
      output += temp;
    dassert(output, "a;");
    unlink("project.xml");
  }

  {
    const char * s = "<unit>a;</unit>";
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0);
    dassert(srcml_extract_text(0, strlen(s), output_buffer, 0, 0), SRCML_STATUS_ERROR);
    xmlOutputBufferClose(output_buffer);
  }

  {
    const char * s = "<unit>a;</unit>";
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0);
    dassert(srcml_extract_text(s, 0, output_buffer, 0, 0), SRCML_STATUS_ERROR);
    xmlOutputBufferClose(output_buffer);
  }

  {
    const char * s = "<unit>a;</unit>";
    dassert(srcml_extract_text(s, strlen(s), 0, 0, 0), SRCML_STATUS_ERROR);
  }

  srcml_cleanup_globals();

  return 0;

}
