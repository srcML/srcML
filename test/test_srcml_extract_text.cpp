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

int main(int argc, char * argv[]) {

  /* 
     srcml_extract_text
   */

  {
    const char * s = "<unit>a;</unit>";
    assert(srcml_extract_text(s, strlen(s), xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0), 0, 0) == SRCML_STATUS_OK);
    std::ifstream in("project.xml");
    std::string output;
    std::string temp;
    while(in >> temp)
      output += temp;
    assert(output == "a;");
    unlink("project.xml");
  }

  {
    const char * s = "<unit>a;</unit>";
    assert(srcml_extract_text(0, strlen(s), xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0), 0, 0) == SRCML_STATUS_ERROR);
  }

  {
    const char * s = "<unit>a;</unit>";
    assert(srcml_extract_text(s, 0, xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0), 0, 0) == SRCML_STATUS_ERROR);
  }

  {
    const char * s = "<unit>a;</unit>";
    assert(srcml_extract_text(s, strlen(s), 0, 0, 0) == SRCML_STATUS_ERROR);
  }

  return 0;

}
