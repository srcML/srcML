/*
  test_srcml_sax2_utilities.cpp

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

  Test cases for other srcml_sax2 functions
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
     srcMLCreateMemoryParserCtxt
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

  /* 
     srcMLCreateParserCtxt
   */

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

  /* 
     srcMLParseDocument
   */

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

  return 0;

}
