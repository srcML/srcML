/*
  xpathproc

  A simple XPath expression evaluator.

  Usage:  xpathproc xpath-expression xml-file
*/

#include "srcpatheval.h"
#include <iostream>
#include <string>
#include <list>

#include <libxml/xpath.h>
#include <libxml/xmlsave.h>
#include <libxml/xpathInternals.h>

#include "xmlsavebuf.h"

/* srcML unit attributes */
/*
const char* UNIT_ATTRIBUTE_LANGUAGE = "language";
const char* UNIT_ATTRIBUTE_DIRECTORY = "dir";
const char* UNIT_ATTRIBUTE_FILENAME = "filename";
const char* UNIT_ATTRIBUTE_VERSION = "version";
*/

//typedef std::list<std::pair<std::string, std::string> > PROPERTIES_TYPE;

int srcpatheval(const char* xpath, xmlTextReaderPtr reader, const char* ofilename) {

  // compile the xpath that will be applied to each unit
  xmlXPathCompExprPtr compiled_xpath = xmlXPathCompile(BAD_CAST xpath);
  if (compiled_xpath == 0) {
    return 1;
  }

  // read the first node
  xmlTextReaderRead(reader);

  // record the current attributes for use in subunits
  //  std::string unit_filename = (char*) xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_FILENAME);
  //  std::string unit_directory = (char*) xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_DIRECTORY);
  //  std::string unit_version = (char*) xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_VERSION);
  //  std::string unit_language = (char*) xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_LANGUAGE);

  // record all attributes for future use
  // don't use the TextReaderMoveToNextAttribute as it messes up the future expand
  /*
  PROPERTIES_TYPE attrv;
  for (xmlAttrPtr pAttr = xmlTextReaderCurrentNode(reader)->properties; pAttr; pAttr = pAttr->next) {

      char* ac = (char*) xmlGetProp(xmlTextReaderCurrentNode(reader), pAttr->name);

      attrv.push_back(std::make_pair((const char*) ac, (const char*) pAttr->name));
  }
  */

  // setup the context up on which the xpath will be evaluated on
  xmlXPathContextPtr context = xmlXPathNewContext(xmlTextReaderCurrentDoc(reader));

  xmlSaveCtxtPtr ctxt = xmlSaveToFilename(ofilename, (const char*) xmlTextReaderConstEncoding(reader), 
					  XML_SAVE_NO_DECL);

  // register the namespaces on the root element
  xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "<unit");
  for (xmlNsPtr pAttr = xmlTextReaderCurrentNode(reader)->nsDef; pAttr; pAttr = pAttr->next) {

	if (xmlXPathRegisterNs(context, pAttr->prefix ? pAttr->prefix : BAD_CAST "",
			       BAD_CAST pAttr->href) == -1)
	  fprintf(stderr, "Unable to register prefix %s for namespace %s\n",
		  pAttr->prefix, pAttr->href);

	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), " xmlns");
	if (pAttr->prefix)
	  xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), ":");
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), (const char*) pAttr->prefix);
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "=\"");
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), (const char*) pAttr->href);
	xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "\"");
  }
  xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), ">\n\n");

  // register src since it probably isn't
  const char* stdprefix = "src";
  const char* stdurl = "http://www.sdml.info/srcML/src";
  if (xmlXPathRegisterNs(context, BAD_CAST stdprefix, BAD_CAST stdurl) == -1)
    fprintf(stderr, "Unable to register prefix %s for namespace %s\n", stdprefix, stdurl);

  // type of the xpath
  int nodetype = 0;

  // total of all counts for the separate units
  double total = 0;

  /*
  // resulting boolean
  bool result_bool = false;
  */

  while (1) {

     // read a node
     int ret = xmlTextReaderRead(reader);
     if (ret != 1)
       break;

     // contine on until we reach a unit tag at the proper depth
     if (xmlTextReaderDepth(reader) == 1 &&
	 xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
	 xmlTextReaderConstName(reader)[0] == 'u') {

       xmlChar* unit_directory = xmlTextReaderGetAttribute(reader, BAD_CAST "dir");
       xmlChar* unit_filename = xmlTextReaderGetAttribute(reader, BAD_CAST "filename");

       // expand this unit to make it the context
       context->node = xmlTextReaderExpand(reader);

       // evaluate the xpath on the context from the current document
       xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
       if (result_nodes == 0) {
	 fprintf(stderr, "ERROR\n");
	 return 1;
       }

       // update the node type
       nodetype = result_nodes->type;

       bool outputunit = true;
       xmlNodePtr onode;

       // process the resulting nodes
       switch (nodetype) {

       // node set result
       case XPATH_NODESET:

	 // may not have any values
	 if (!result_nodes->nodesetval)
	   break;

	 // output all the found nodes
	 for (int i = 0; i < xmlXPathNodeSetGetLength(result_nodes->nodesetval); ++i) {

	   onode = xmlXPathNodeSetItem(result_nodes->nodesetval, i);

	   outputunit = strcmp("unit", (const char*) onode->name) != 0;

	   if (outputunit) {

	     // unit start tag
	     xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "<unit");

	     if (unit_directory) {
	       xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), " dir=\"");
	       xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), (const char*) unit_directory);
	       xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "\"");
	     }

	     if (unit_filename) {
	       xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "\" filename=\"");
	       xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), (const char*) unit_filename);
	       xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "\"");
	     }

	     xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), ">");
	   }

	   // xpath result
	   xmlSaveTree(ctxt, onode);

	   if (outputunit) {

	     // unit end tag
	     xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "</unit>\n");
	     xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "\n");
	   }
	 }

	 break;

       // numeric result
       case XPATH_NUMBER:
	 total += result_nodes->floatval;
	 break;

	 /*
       // boolean result
       case XPATH_BOOLEAN:
         result_bool |= result_nodes->boolval;
	 break;
	 */
       default:
	 fprintf(stderr, "Unhandled type\n");
	 break;
       };

       // finished with the result nodes
       xmlXPathFreeObject(result_nodes);

       // move over this expanded node
       xmlTextReaderNext(reader);
     }
  }

  // all done with xpath
  xmlXPathFreeCompExpr(compiled_xpath);

  // finalize results
  switch (nodetype) {
  case XPATH_NUMBER:
    printf("%f\n", total);
    break;
    /*
  // boolean result
  case XPATH_BOOLEAN:
    if (result_bool)
      puts("true\n");
    else
      puts("false\n");
    break;
    */
  default:
    break;
  }

  xmlOutputBufferWriteString(xmlSaveGetBuffer(ctxt), "</unit>\n");

  xmlSaveClose(ctxt);

  return 0;
}
