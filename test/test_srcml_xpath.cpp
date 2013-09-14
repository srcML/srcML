/*

  Test cases for srcml_xpath
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
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
  }

  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    assert(srcMLParseDocument(ctxt, false) == SRCML_STATUS_OK);
  }

  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    assert(srcMLParseDocument(0, false) == SRCML_STATUS_ERROR);
  }

  return 0;

}
