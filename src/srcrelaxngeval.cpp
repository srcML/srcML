/*
  srcrelaxngeval.cpp
*/

#include "srcrelaxngeval.h"
#include "srceval.h"

#include <iostream>

#include <string>
#include <list>

#include <libxml/xpath.h>
#include <libxml/xmlsave.h>
#include <libxml/xpathInternals.h>

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>
#include <libexslt/exsltconfig.h>

int srcrelaxngeval(const char* xpath, xmlTextReaderPtr reader, const char* ofilename) {

  // read the first node
  xmlTextReaderRead(reader);

  // setup output
  xmlOutputBufferPtr buf = xmlOutputBufferCreateFilename(ofilename, NULL, 0);

  // copy the start tag of the root element unit
  xmlUnitDumpOutputBuffer(buf, xmlTextReaderCurrentNode(reader));
  xmlOutputBufferWrite(buf, 3, ">\n\n");

  xmlRelaxNGParserCtxtPtr relaxng = xmlRelaxNGNewParserCtxt(xpath);

  xmlRelaxNGPtr rng = xmlRelaxNGParse(relaxng);

  xmlRelaxNGValidCtxtPtr rngptr = xmlRelaxNGNewValidCtxt(rng);

  while (1) {

     // read a node
     int ret = xmlTextReaderRead(reader);
     if (ret != 1)
       break;

     // nested unit tag
     if (xmlTextReaderDepth(reader) == 1 &&
	 xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
	 xmlTextReaderConstName(reader)[0] == 'u') {

       // expand this unit to make it the context
       xmlNodePtr node = xmlTextReaderExpand(reader);

       // save the next pointer
       xmlNodePtr next = xmlTextReaderCurrentNode(reader)->next;
       xmlTextReaderCurrentNode(reader)->next = 0;

       // validate
       int n = xmlRelaxNGValidateDoc(rngptr, node->doc);

       // output if it validates
       if (n == 0) {
	 xmlNodeDumpOutput(buf, xmlTextReaderCurrentDoc(reader), node, 0, 0, 0);
	 xmlOutputBufferWrite(buf, 2, "\n\n");
       }

       // restore overparsing
       xmlTextReaderCurrentNode(reader)->next = next;

       // move over this expanded node
       xmlTextReaderNext(reader);
     }
  }

  // root unit end tag
  xmlOutputBufferWrite(buf, 8, "</unit>\n");

  // all done with the buffer
  xmlOutputBufferClose(buf);

  return 0;
}
