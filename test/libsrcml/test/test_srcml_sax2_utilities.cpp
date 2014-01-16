/*
  test_srcml_sax2_utilities.cpp

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

#include "dassert.hpp"

int main(int argc, char * argv[]) {

  /* 
     srcMLCreateMemoryParserCtxt
   */
  using namespace std;
  
  cout << "Test 1" << endl;
  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    dassert(!ctxt, 0);
    xmlFreeParserCtxt(ctxt);
  }
  
  cout << "Test 2" << endl;
  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, 0);
    dassert(ctxt, 0);
  }
  
  cout << "Test 3" << endl;
  {
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(0, 1);
    dassert(ctxt, 0);
  }

  /* 
     srcMLCreateParserCtxt
   */
  cout << "Test 4" << endl;
  {
    const char * s = "test_srcml_xpath.cpp";
    xmlParserInputBufferPtr buffer_input = xmlParserInputBufferCreateFilename(s, xmlParseCharEncoding(0));
    xmlParserCtxtPtr ctxt = srcMLCreateParserCtxt(buffer_input);
    dassert(!ctxt, 0);
    xmlFreeParserCtxt(ctxt);
  }
  
  cout << "Test 5" << endl;
  {
    xmlParserCtxtPtr ctxt = srcMLCreateParserCtxt(0);
    dassert(ctxt, 0);
  }

  /* 
     srcMLParseDocument
   */
  
  cout << "Test 6" << endl;
  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    dassert(srcMLParseDocument(ctxt, true), SRCML_STATUS_OK);
    xmlFreeDoc(ctxt->myDoc);
    xmlFreeParserCtxt(ctxt);
  }
  
  cout << "Test 7" << endl;
  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    dassert(srcMLParseDocument(ctxt, false), SRCML_STATUS_OK);
    xmlFreeDoc(ctxt->myDoc);
    xmlFreeParserCtxt(ctxt);
  }
  
  cout << "Test 8" << endl;
  {
    const char * s = "<unit/>";
    xmlParserCtxtPtr ctxt = srcMLCreateMemoryParserCtxt(s, strlen(s));
    dassert(srcMLParseDocument(0, false), SRCML_STATUS_ERROR);
    xmlFreeDoc(ctxt->myDoc);
    xmlFreeParserCtxt(ctxt);
  }

  srcml_cleanup_globals();

  return 0;

}
