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
    const char * s = "<unit/>";
    assert(srcml_extract_text(s, strlen(s), xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0), 0, 0) == SRCML_STATUS_OK);
    unlink("project.xml");
  }

  {
    const char * s = "<unit/>";
    assert(srcml_extract_text(0, strlen(s), xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0), 0, 0) == SRCML_STATUS_ERROR);
  }

  {
    const char * s = "<unit/>";
    assert(srcml_extract_text(s, 0, xmlOutputBufferCreateFilename("project.xml", xmlFindCharEncodingHandler("ISO-8859-1"), 0), 0, 0) == SRCML_STATUS_ERROR);
  }

  {
    const char * s = "<unit/>";
    assert(srcml_extract_text(s, strlen(s), 0, 0, 0) == SRCML_STATUS_ERROR);
  }

  {
    const char * s = "<unit/>";
    std::ofstream file("input.xml");
    file << s;
    file.close();
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename(s, xmlParseCharEncoding(0));
    const char * xpaths[2] = {"//src:unit", 0 };
    int fd = open("project.xml", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(srcml_xpath(buffer_input, "src:unit", xpaths, fd, 0) == SRCML_STATUS_OK);
  }

  return 0;

}
