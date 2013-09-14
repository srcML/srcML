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

  //xmlParserCtxtPtr  srcMLCreateParserCtxt(xmlParserInputBufferPtr buffer_input)
  //srcMLParseDocument(xmlParserCtxtPtr ctxt, bool allowendearly)

  return 0;

}
