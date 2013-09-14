/*

  Test cases for srcml_xpath
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
     srcml_xpath
   */

  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    assert(ctxt != NULL);
    xmlFreeParserCtxt(ctxt);
  }

  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, 0);
    assert(ctxt == NULL);
  }

  {
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(0, 1);
    assert(ctxt == NULL);
  }

  {
    const char * s = "test_srcml_xpath.cpp";
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename(s, xmlParseCharEncoding(0));
    xmlParserCtxtPtr ctxt = srcMLCreateParserCtxt(buffer_input);
    assert(ctxt != NULL);
    xmlFreeParserCtxt(ctxt);
  }

  {
    xmlParserCtxtPtr ctxt = srcMLCreateParserCtxt(0);
    assert(ctxt == NULL);
  }

  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    assert(srcMLParseDocument(ctxt, true) == SRCML_STATUS_OK);
    xmlFreeParserCtxt(ctxt);
  }

  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    assert(srcMLParseDocument(ctxt, false) == SRCML_STATUS_OK);
    xmlFreeParserCtxt(ctxt);
  }

  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    assert(srcMLParseDocument(0, false) == SRCML_STATUS_ERROR);
    xmlFreeParserCtxt(ctxt);
  }

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

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    const char * xpaths[2] = {"//src:unit", 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xpath(buffer_input, "src:unit", xpaths, fd, 0) == SRCML_STATUS_OK);
    std::ifstream in("project.xml");
    std::string output;
    std::string temp;
    while(in >> temp)
      output += temp;
    assert(output == "<?xmlversion=\"1.0\"encoding=\"\"standalone=\"yes\"?><unit/>");
    unlink("input.xml");
    unlink("project.xml");
  }

  {
    const char * xpaths[2] = {"//src:unit", 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xpath(0, "src:unit", xpaths, fd, 0) == SRCML_STATUS_ERROR);
    unlink("project.xml");
  }

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    const char * xpaths[2] = {"//src:unit", 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xpath(buffer_input, 0, xpaths, fd, 0) == SRCML_STATUS_ERROR);
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
    assert(srcml_xpath(buffer_input, "src:unit", 0, fd, 0) == SRCML_STATUS_ERROR);
    unlink("input.xml");
    unlink("project.xml");
  }

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    const char * xpaths[2] = {0, 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xpath(buffer_input, "src:unit", xpaths, fd, 0) == SRCML_STATUS_ERROR);
    unlink("input.xml");
    unlink("project.xml");
  }

  {
    const char * s = "<unit>a;</unit>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename("input.xml", xmlParseCharEncoding(0));
    const char * xpaths[2] = {"//src:unit", 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xpath(buffer_input, "src:unit", xpaths, -1, 0) == SRCML_STATUS_ERROR);
    unlink("input.xml");
  }

  return 0;

}
