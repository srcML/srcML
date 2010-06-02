#include "srceval.h"
#include "srcmlns.h"

#include <cstring>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlreader.h>

static xmlChar* unit_directory = 0;
static xmlChar* unit_filename = 0;

#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s

void xmlUnitDumpOutputBuffer(xmlOutputBufferPtr buf, xmlNodePtr node) {

  xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<unit"));
  //  xmlOutputBufferWrite(buf, 5, "<unit");

  // register the namespaces on the root element
  for (xmlNsPtr pAttr =  node->nsDef; pAttr; pAttr = pAttr->next)
    xmlNodeDumpOutput(buf, node->doc, (xmlNodePtr) pAttr, 0, 0, 0);

  // copy all attributes
  for (xmlAttrPtr pAttr = node->properties; pAttr; pAttr = pAttr->next)
    xmlNodeDumpOutput(buf, node->doc, (xmlNodePtr) pAttr, 0, 0, 0);
}

static void outputendunit(xmlOutputBufferPtr buf) {
  xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("</unit>"));
}

static void outputstartunit(xmlOutputBufferPtr buf, int line) {

  // unit start tag
  xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<unit"));

  // directory attribute
  if (unit_directory) {
    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" dir=\""));
    xmlOutputBufferWriteString(buf, (const char*) unit_directory);
    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
  }

  // filename attribute
  if (unit_filename) {
    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL((" filename=\"")));
    xmlOutputBufferWriteString(buf, (const char*) unit_filename);
    xmlOutputBufferWrite(buf, 1, "\"");
  }

  // line number
  // TODO:  fix line numbering problem
  xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" line=\""));
  char s[50] = { 0 };
  sprintf(s, "%d", line);
  xmlOutputBufferWriteString(buf, s);
  xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));

  xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">"));
}

static void outputresult(xmlDocPtr doc, xmlNodePtr onode, xmlOutputBufferPtr buf, int line) {

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

int srceval(const char* context_element,
	    const char* xpath[], 
	    const char* xslt_filename,
	    const char* relaxng_filename,
	    xmlTextReaderPtr reader, const char* ofilename) {

  int line = 0;

  // read the first node
  int ret = xmlTextReaderRead(reader);
  if (ret != 1)
    return 1;

  // setup output
  xmlOutputBufferPtr buf = xmlOutputBufferCreateFilename(ofilename, NULL, 0);

  // setup the context up on which the xpath will be evaluated on
  xmlXPathContextPtr context = 0;

  // compile the xpath that will be applied to each unit
  xmlXPathCompExprPtr compiled_xpath = 0;
  if (xpath[0][0]) {

    compiled_xpath = xmlXPathCompile(BAD_CAST xpath[0]);
    if (compiled_xpath == 0) {
      return 1;
    }

    // setup the context up on which the xpath will be evaluated on
    context = xmlXPathNewContext(xmlTextReaderCurrentDoc(reader));

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

  // preserve the previous part of the tree for the next unit
  xmlNodePtr prev = xmlTextReaderCurrentNode(reader)->prev;

  // type of the xpath
  int nodetype = 0;

  // total of all counts for the separate units
  double total = 0;

  // resulting boolean
  bool result_bool = false;

  // did we get any results?
  bool found = false;

  bool firstelement = true;

  while (!result_bool) {

     // read a node
     int ret = xmlTextReaderRead(reader);
     if (ret != 1)
       break;

     // non-unit first element inside the root element indicates a non-nested file
     if (firstelement) {

       if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
	   strcmp((const char*) xmlTextReaderConstName(reader), "unit") != 0) {
	 fprintf(stderr, "Non-nested document.\n");
	 return 1;
       }

       firstelement = false;
     }

     // continue until we reach the context tag with the proper namespace
     if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
	 strcmp((const char*) xmlTextReaderConstName(reader), context_name) == 0 &&
	 strcmp((const char*) xmlTextReaderConstNamespaceUri(reader), context_uri) == 0 ) {

       line = xmlTextReaderGetParserLineNumber(reader);

       unit_directory = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_DIRECTORY);
       unit_filename = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_FILENAME);

       // make it seem like this node is from the root
       xmlTextReaderCurrentNode(reader)->prev = prev;

       // expand this unit to make it the context
       context->node = xmlTextReaderExpand(reader);

       // temporarily remove the next node in case parsing has gone on 
       // that far, i.e., only want this part of the tree
       xmlNodePtr next = xmlTextReaderCurrentNode(reader)->next;

       // preserve the previous part of the tree for the next unit
       //       xmlNodePtr prev = xmlTextReaderCurrentNode(reader)->prev;

       xmlTextReaderCurrentNode(reader)->next = 0;

       // evaluate the xpath on the context from the current document
       xmlXPathObjectPtr result_nodes = 0;

       if (compiled_xpath) {
	 result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
	 if (result_nodes == 0) {
	   fprintf(stderr, "ERROR\n");
	   return 1;
	 }
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
	   xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">\n\n"));
	   found = true;
	 }

	 onode = xmlXPathNodeSetItem(result_nodes->nodesetval, 0);

	 // output a unit element around the fragment, unless
	 // is is already a unit
         outputunit = strcmp("unit", (const char*) onode->name) != 0;

	 // if we need a unit, output the start tag.  Line number starts at 1, not 0
	 if (outputunit)
	   outputstartunit(buf, xmlGetLineNo(onode) + 1);

	 // output all the found nodes
	 for (int i = 0; i < xmlXPathNodeSetGetLength(result_nodes->nodesetval); ++i) {

	   onode = xmlXPathNodeSetItem(result_nodes->nodesetval, i);

	   // xpath result
	   xmlNodeDumpOutput(buf, xmlTextReaderCurrentDoc(reader), onode, 0, 0, 0);
	 }

	 // if we need a unit, output the end tag
	 if (outputunit){
	   outputendunit(buf);

	   xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n\n"));
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

       // reset next to preserve normal parsing
       xmlTextReaderCurrentNode(reader)->next = next;

       xmlNodePtr pnode = xmlTextReaderCurrentNode(reader);

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
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("</unit>\n"));
    else
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("/>\n"));

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
