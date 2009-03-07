/*
  xpathproc

  A simple XPath expression evaluator.

  Usage:  xpathproc xpath-expression xml-file
*/

#include "srcpatheval.h"
#include "srceval.h"
#include "srcmlns.h"

#include <cstring>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/parserInternals.h>

xmlChar* unit_directory = 0;
xmlChar* unit_filename = 0;

void outputendunit(xmlOutputBufferPtr buf) {
	     xmlOutputBufferWrite(buf, 7, "</unit>");
}

void outputstartunit(xmlOutputBufferPtr buf, int line) {

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

	     // TODO:  fix line numbering problem
	     xmlOutputBufferWrite(buf, 7, " line=\"");
	     char s[50] = { 0 };
	     sprintf(s, "%d", line);
	     xmlOutputBufferWriteString(buf, s);
	     xmlOutputBufferWrite(buf, 1, "\"");

	     xmlOutputBufferWrite(buf, 1, ">");
}

void outputresult(xmlDocPtr doc, xmlNodePtr onode, xmlOutputBufferPtr buf, int line) {

	   // output a unit element around the fragment, unless
	   // is is already a unit
           bool outputunit = strcmp("unit", (const char*) onode->name) != 0;

	   // if we need a unit, output the start tag
	   if (outputunit)
	     outputstartunit(buf, line);

	   // xpath result
	   xmlNodeDumpOutput(buf, doc, onode, 0, 0, 0);

	   // if we need a unit, output the end tag
	   if (outputunit)
	     outputendunit(buf);
}

int srcpatheval(const char* context_element, const char* xpath, xmlTextReaderPtr reader, const char* ofilename) {

  int line = 0;

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

  // register standard prefixes for standard namespaces
  const char* prefixes[] = {
    SRCML_SRC_NS_URI, "src",
    SRCML_CPP_NS_URI, SRCML_CPP_NS_PREFIX_DEFAULT,
    SRCML_ERR_NS_URI, SRCML_ERR_NS_PREFIX_DEFAULT,
    SRCML_EXT_LITERAL_NS_URI, SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,
    SRCML_EXT_OPERATOR_NS_URI, SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT,
    SRCML_EXT_MODIFIER_NS_URI, SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT,
  };

  for (unsigned int i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]) / 2; i += 2)
    if (xmlXPathRegisterNs(context, BAD_CAST prefixes[i + 1], BAD_CAST prefixes[i]) == -1)
      fprintf(stderr, "Unable to register prefix %s for namespace %s\n", prefixes[i + 1], prefixes[i]);
  
  // register any additional namespaces on the root element
  for (xmlNsPtr pAttr = xmlTextReaderCurrentNode(reader)->nsDef; pAttr; pAttr = pAttr->next) {

    // get out if the prefix is already defined
    if (xmlXPathNsLookup(context, pAttr->prefix))
      continue;

    if (xmlXPathRegisterNs(context, pAttr->prefix ? pAttr->prefix : BAD_CAST "",
			   BAD_CAST pAttr->href) == -1)
      fprintf(stderr, "Unable to register prefix %s for namespace %s\n",
	      pAttr->prefix, pAttr->href);
  }

  // find the url of the prefix for the context
  char* context_prefix;
  const char* context_name = (const char*) xmlSplitQName((xmlParserCtxtPtr)context,
			     BAD_CAST context_element, (xmlChar**) &context_prefix);
  if (!context_prefix)
    context_prefix = (char*) "";
  const char* context_uri = (const char*) xmlXPathNsLookup(context, BAD_CAST context_prefix);

  // output wrapping unit
  xmlUnitDumpOutputBuffer(buf, xmlTextReaderCurrentNode(reader));

  // type of the xpath
  int nodetype = 0;

  // total of all counts for the separate units
  double total = 0;

  // resulting boolean
  bool result_bool = false;

  // did we get any results?
  bool found = false;

  while (!result_bool) {

     // read a node
     int ret = xmlTextReaderRead(reader);
     if (ret != 1)
       break;

     // continue until we reach the context tag with the proper namespace
     if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
	 strcmp((const char*) xmlTextReaderConstName(reader), context_name) == 0 &&
	 strcmp((const char*) xmlTextReaderConstNamespaceUri(reader), context_uri) == 0 ) {

       line = xmlTextReaderGetParserLineNumber(reader);

       unit_directory = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_DIRECTORY);
       unit_filename = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_FILENAME);

       // expand this unit to make it the context
       context->node = xmlTextReaderExpand(reader);

       // save the next pointer
       xmlNodePtr next = xmlTextReaderCurrentNode(reader)->next;
       xmlTextReaderCurrentNode(reader)->next = 0;

       int startline = xmlTextReaderCurrentNode(reader)->line;

       // evaluate the xpath on the context from the current document
       xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
       if (result_nodes == 0) {
	 fprintf(stderr, "ERROR\n");
	 return 1;
       }
       
       // update the node type
       nodetype = result_nodes->type;

       int result_size = 0;

       xmlNodePtr onode = 0;

       bool outputunit = false;

       // process the resulting nodes
       switch (nodetype) {

       // node set result
       case XPATH_NODESET:

	 // may not have any values
	 if (!result_nodes->nodesetval)
	   break;

	 // may not have any results
	 result_size = xmlXPathNodeSetGetLength(result_nodes->nodesetval);
	 if (result_size == 0)
	   break;

	 // first time found a result, so close root unit start tag
	 if (!found) {
	   xmlOutputBufferWrite(buf, 3, ">\n\n");
	   found = true;
	 }

	 onode = xmlXPathNodeSetItem(result_nodes->nodesetval, 0);

	 // output a unit element around the fragment, unless
	 // is is already a unit
         outputunit = strcmp("unit", (const char*) onode->name) != 0;

	 // if we need a unit, output the start tag
	 if (outputunit)
	   outputstartunit(buf, onode->line - startline);

	 // output all the found nodes
	 for (int i = 0; i < xmlXPathNodeSetGetLength(result_nodes->nodesetval); ++i) {

	   onode = xmlXPathNodeSetItem(result_nodes->nodesetval, i);

	   // xpath result
	   xmlNodeDumpOutput(buf, xmlTextReaderCurrentDoc(reader), onode, 0, 0, 0);
	 }

	 // if we need a unit, output the end tag
	 if (outputunit)
	   outputendunit(buf);

	 xmlOutputBufferWrite(buf, 2, "\n\n");

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

       xmlTextReaderCurrentNode(reader)->next = next;

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

    if (found)
      xmlOutputBufferWrite(buf, 8, "</unit>\n");
    else
      xmlOutputBufferWrite(buf, 3, "/>\n");

    xmlOutputBufferClose(buf);
    break;

  case XPATH_NUMBER:
    if ((int)total == total)
      printf("%d\n", (int)total);
    else
      printf("%f\n", total);

    xmlFree(buf);
    break;

  // boolean result
  case XPATH_BOOLEAN:
    if (result_bool)
      puts("true\n");
    else
      puts("false\n");

    xmlFree(buf);
    break;

  default:
    break;
  }

  return 0;
}
