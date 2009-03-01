/*
  xpathproc

  A simple XPath expression evaluator.

  Usage:  xpathproc xpath-expression xml-file
*/

#include "srcpatheval.h"
#include "srceval.h"

#include <cstring>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

xmlChar* unit_directory = 0;
xmlChar* unit_filename = 0;

void outputresult(xmlDocPtr doc, xmlNodePtr onode, xmlOutputBufferPtr buf) {

	   // output a unit element around the fragment, unless
	   // is is already a unit
           bool outputunit = strcmp("unit", (const char*) onode->name) != 0;

	   // if we need a unit, output the start tag
	   if (outputunit) {

	     // unit start tag
	     xmlOutputBufferWrite(buf, 5, "<unit");

	     if (unit_directory) {
	       xmlOutputBufferWrite(buf, 6, " dir=\"");
	       xmlOutputBufferWriteString(buf, (const char*) unit_directory);
	       xmlOutputBufferWrite(buf, 1, "\"");
	     }

	     if (unit_filename) {
	       xmlOutputBufferWrite(buf, 11, " filename=\"");
	       xmlOutputBufferWriteString(buf, (const char*) unit_filename);
	       xmlOutputBufferWrite(buf, 1, "\"");
	     }

	     /*
	     // TODO:  fix line numbering problem
	     xmlOutputBufferWrite(buf, 7, " line=\"");
	     char s[50] = { 0 };
	     sprintf(s, "%d", onode->line);
	     xmlOutputBufferWriteString(buf, s);
	     xmlOutputBufferWrite(buf, 1, "\"");
	     */

	     xmlOutputBufferWrite(buf, 1, ">");
	   }

	   // xpath result
	   xmlNodeDumpOutput(buf, doc, onode, 0, 0, 0);

	   // if we need a unit, output the end tag
	   if (outputunit) {

	     // unit end tag
	     xmlOutputBufferWrite(buf, 9, "</unit>");
	   }

}

int srcpatheval(const char* xpath, xmlTextReaderPtr reader, const char* ofilename) {

  // compile the xpath that will be applied to each unit
  xmlXPathCompExprPtr compiled_xpath = xmlXPathCompile(BAD_CAST xpath);
  if (compiled_xpath == 0) {
    return 1;
  }

  // read the first node
  int ret = xmlTextReaderRead(reader);
  if (ret != 1)
    return 1;

  // setup the context up on which the xpath will be evaluated on
  xmlXPathContextPtr context = xmlXPathNewContext(xmlTextReaderCurrentDoc(reader));

  // setup output
  xmlOutputBufferPtr buf = xmlOutputBufferCreateFilename(ofilename, NULL, 0);

  // register src since it probably isn't.  Do so first, so that it can be overridden
  const char* stdprefix = "src";
  const char* stdurl = "http://www.sdml.info/srcML/src";
  if (xmlXPathRegisterNs(context, BAD_CAST stdprefix, BAD_CAST stdurl) == -1)
    fprintf(stderr, "Unable to register prefix %s for namespace %s\n", stdprefix, stdurl);

  // register the namespaces on the root element
  for (xmlNsPtr pAttr = xmlTextReaderCurrentNode(reader)->nsDef; pAttr; pAttr = pAttr->next) {

	if (xmlXPathRegisterNs(context, pAttr->prefix ? pAttr->prefix : BAD_CAST "",
			       BAD_CAST pAttr->href) == -1)
	  fprintf(stderr, "Unable to register prefix %s for namespace %s\n",
		  pAttr->prefix, pAttr->href);
  }

  bool first = true;

  // type of the xpath
  int nodetype = 0;

  // total of all counts for the separate units
  double total = 0;

  // resulting boolean
  bool result_bool = false;

  while (!result_bool) {

     // read a node
     int ret = xmlTextReaderRead(reader);
     if (ret != 1)
       break;

     // contine on until we reach a unit tag at the proper depth
     if (xmlTextReaderDepth(reader) == 1 &&
	 xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
	 xmlTextReaderConstName(reader)[0] == 'u') {

       unit_directory = xmlTextReaderGetAttribute(reader, BAD_CAST "dir");
       unit_filename = xmlTextReaderGetAttribute(reader, BAD_CAST "filename");

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

       // process the resulting nodes
       switch (nodetype) {

       // node set result
       case XPATH_NODESET:

	 // may not have any values
	 if (!result_nodes->nodesetval)
	   break;

	 // copy the start tag of the root element unit (if we haven't already)
	 if (first) {
	   xmlUnitDumpOutputBuffer(buf, xmlTextReaderCurrentNode(reader));
	   first = false;
	 }

	 // output all the found nodes
	 for (int i = 0; i < xmlXPathNodeSetGetLength(result_nodes->nodesetval); ++i) {
	   outputresult(xmlTextReaderCurrentDoc(reader), xmlXPathNodeSetItem(result_nodes->nodesetval, i), buf);
	   xmlOutputBufferWrite(buf, 2, "\n\n");
	 }

	 break;

       // numeric result
       case XPATH_NUMBER:
	 total += result_nodes->floatval;
	 break;

       // boolean result
       case XPATH_BOOLEAN:
         result_bool |= result_nodes->boolval;
	 break;

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
  case XPATH_NODESET:

    xmlOutputBufferWrite(buf, 8, "</unit>\n");
    break;

  case XPATH_NUMBER:
    if ((int)total == total)
      printf("%d\n", (int)total);
    else
      printf("%f\n", total);
    break;

  // boolean result
  case XPATH_BOOLEAN:
    if (result_bool)
      puts("true\n");
    else
      puts("false\n");
    break;

  default:
    break;
  }

  xmlOutputBufferClose(buf);

  return 0;
}
