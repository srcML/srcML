/*
  srcrelaxngeval.cpp
*/

#include "srcrelaxngeval.h"

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

  // register the namespaces on the root element
  xmlOutputBufferWrite(buf, 5, "<unit");
  for (xmlNsPtr pAttr = xmlTextReaderCurrentNode(reader)->nsDef; pAttr; pAttr = pAttr->next) {

        xmlOutputBufferWrite(buf, 6, " xmlns");
        if (pAttr->prefix)
	  xmlOutputBufferWrite(buf, 1, ":");
	xmlOutputBufferWriteString(buf, (const char*) pAttr->prefix);
	xmlOutputBufferWrite(buf, 2, "=\"");
	xmlOutputBufferWriteString(buf, (const char*) pAttr->href);
	xmlOutputBufferWrite(buf, 2, "\"");
  }

  // copy all attributes
  for (xmlAttrPtr pAttr = xmlTextReaderCurrentNode(reader)->properties; pAttr; pAttr = pAttr->next) {

        char* ac = (char*) xmlGetProp(xmlTextReaderCurrentNode(reader), pAttr->name);

	xmlOutputBufferWrite(buf, 1, " ");
	xmlOutputBufferWriteString(buf, (const char*) pAttr->name);
	xmlOutputBufferWrite(buf, 2, "=\"");
	xmlOutputBufferWriteString(buf, (const char*) ac);
	xmlOutputBufferWrite(buf, 1, "\"");
  }
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

       // validate
       int n = xmlRelaxNGValidateDoc(rngptr, node->doc);

       // output if it validates
       if (n == 0) {
	 xmlNodeDumpOutput(buf, xmlTextReaderCurrentDoc(reader), node, 0, 0, 0);
	 xmlOutputBufferWrite(buf, 2, "\n\n");
       }

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
