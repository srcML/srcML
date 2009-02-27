/*
  xpathproc

  A simple XPath expression evaluator.

  Usage:  xpathproc xpath-expression xml-file
*/

#include "srcxslteval.h"
#include "srceval.h"

#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>

int srcxslteval(const char* xpath, xmlTextReaderPtr reader, const char* ofilename) {

  // allow for all exslt functions
  exsltRegisterAll();

  // parse the stylesheet
  xsltStylesheetPtr xslt = xsltParseStylesheetFile(BAD_CAST xpath);

  // read the first node
  xmlTextReaderRead(reader);

  // setup output
  xmlOutputBufferPtr buf = xmlOutputBufferCreateFilename(ofilename, NULL, 0);

  // copy the start tag of the root element unit
  xmlUnitDumpOutputBuffer(buf, xmlTextReaderCurrentNode(reader));

  // doc for applying stylesheet to
  xmlDocPtr doc = xmlNewDoc(NULL);

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

       // create a separate document with the expanded unit
       xmlDocSetRootElement(doc, xmlCopyNode(node, 1));

       // apply the style sheet to the extracted doc
       xmlDocPtr res = xsltApplyStylesheet(xslt, doc, NULL);

       // remove and store the namespace (if we can)
       xmlNodePtr resroot = xmlDocGetRootElement(res); 
       xmlNsPtr savens = 0;
       if (resroot) {
	 savens = xmlDocGetRootElement(res)->nsDef;
	 xmlDocGetRootElement(res)->nsDef = 0;
       }
       
       // save the transformed tree
       xsltSaveResultTo(buf, res, xslt);
       xmlOutputBufferWriteString(buf, "\n");

       // put the namespace back in
       if (savens)
	 xmlDocGetRootElement(res)->nsDef = savens;

       // finished with the result of the transformation
       xmlFreeDoc(res);

       // cleanup the extracted doc
       xmlNodePtr oldnode = xmlDocGetRootElement(doc);
       xmlUnlinkNode(oldnode);
       xmlFreeNode(oldnode);

       // move over this expanded node
       xmlTextReaderNext(reader);
     }
  }

  // root unit end tag
  xmlOutputBufferWriteString(buf, "</unit>\n");

  // all done with the buffer
  xmlOutputBufferClose(buf);

  // all done with the doc
  xmlFreeDoc(doc);

  return 0;
}
