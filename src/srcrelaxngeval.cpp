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

#include "xmlsavebuf.h"

/* srcML unit attributes */
/*
const char* UNIT_ATTRIBUTE_LANGUAGE = "language";
const char* UNIT_ATTRIBUTE_DIRECTORY = "dir";
const char* UNIT_ATTRIBUTE_FILENAME = "filename";
const char* UNIT_ATTRIBUTE_VERSION = "version";
*/

int srcrelaxngeval(const char* xpath, xmlTextReaderPtr reader, const char* ofilename) {

  // read the first node
  xmlTextReaderRead(reader);

  // setup output
  xmlSaveCtxtPtr ctxt = xmlSaveToFilename(ofilename, (const char*) xmlTextReaderConstEncoding(reader),
					  XML_SAVE_NO_DECL);

  // register the namespaces on the root element
  xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "<unit");
  for (xmlNsPtr pAttr = xmlTextReaderCurrentNode(reader)->nsDef; pAttr; pAttr = pAttr->next) {

	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), " xmlns");
	if (pAttr->prefix)
	  xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), ":");
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), (const char*) pAttr->prefix);
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "=\"");
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), (const char*) pAttr->href);
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "\"");
  }

  // copy all attributes
  for (xmlAttrPtr pAttr = xmlTextReaderCurrentNode(reader)->properties; pAttr; pAttr = pAttr->next) {

        char* ac = (char*) xmlGetProp(xmlTextReaderCurrentNode(reader), pAttr->name);

	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), " ");
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), (const char*) pAttr->name);
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "=\"");
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), (const char*) ac);
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "\"");
  }
  xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), ">\n\n");

  // doc for applying stylesheet to
  xmlDocPtr doc = xmlNewDoc(NULL);

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

       xmlDocSetRootElement(doc, xmlCopyNode(node, 1));
       int n = xmlRelaxNGValidateDoc(rngptr, doc);

       if (n == 0)
	 // xpath result
	 xmlSaveTree(ctxt, node);

       // cleanup our doc
       xmlNodePtr oldnode = xmlDocGetRootElement(doc);
       xmlUnlinkNode(oldnode);
       xmlFreeNode(oldnode);

       // move over this expanded node
       xmlTextReaderNext(reader);
     }
  }

  // root unit end tag
  xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "</unit>\n");

  // all done with the buffer
  xmlOutputBufferClose(xmlSaveGetBuffer(ctxt));

  // all done with the doc
  xmlFreeDoc(doc);

  return 0;
}
