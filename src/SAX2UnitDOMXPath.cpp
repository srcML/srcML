/*
  SAX2UnitDOMXPath.cpp

  Copyright (C) 2010  SDML (www.sdml.info)

  This file is part of the srcML translator.

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

#include "SAX2TextWriter.h"
#include "SAX2UnitDOMXPath.h"
#include "SAX2Utilities.h"
#include "srcmlns.h"

#include <cstring>
#include "Options.h"
#include "srcmlns.h"

#include "srcexfun.h"
#include <cassert>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libexslt/exslt.h>

#include <libxml/SAX2.h>

#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s

SAX2UnitDOMXPath::SAX2UnitDOMXPath(const char* a_context_element, const char* a_fxpath[], const char* a_ofilename, const char* params[], int paramcount, int options) 
  : SAX2UnitDOM(a_context_element, a_ofilename, params, paramcount, options), fxpath(a_fxpath) {
}

xmlSAXHandler SAX2UnitDOMXPath::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument  = &SAX2UnitDOMXPath::startDocument;
  sax.endDocument    = &SAX2UnitDOMXPath::endDocument;
  sax.startElementNs = &SAX2UnitDOM::startElementNsRoot;
  sax.endElementNs   = &SAX2UnitDOMXPath::endElementNs;
  sax.characters     = xmlSAX2Characters;
  sax.ignorableWhitespace = xmlSAX2Characters;
  sax.comment        = xmlSAX2Comment;
  sax.processingInstruction = xmlSAX2ProcessingInstruction;

  return sax;
}

// start document
void SAX2UnitDOMXPath::startDocument(void *ctx) {
  
  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOMXPath* pstate = (SAX2UnitDOMXPath*) ctxt->_private;

  // standard unit dom handling
  SAX2UnitDOM::startDocument(ctx);

  // allow for all exslt functions
  //    exsltRegisterAll();

  //    xsltsrcMLRegister();

  //    if (!pstate->fxpath[0][0])
  //      return;

  // compile the xpath that will be applied to each unit
  pstate->compiled_xpath = xmlXPathCompile(BAD_CAST pstate->fxpath[0]);
  if (pstate->compiled_xpath == 0) {
    return;
  }

  pstate->context = xmlXPathNewContext(ctxt->myDoc);

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
    if (xmlXPathRegisterNs(pstate->context, BAD_CAST prefixes[i + 1], BAD_CAST prefixes[i]) == -1)
      fprintf(stderr, "Unable to register prefix %s for namespace %s\n", prefixes[i + 1], prefixes[i]);

  // initialize total for floating point values
  pstate->total = 0;
}

// end unit element and current file/buffer (started by startElementNs
void SAX2UnitDOMXPath::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2UnitDOMXPath* pstate = (SAX2UnitDOMXPath*) ctxt->_private;

  // DOM building end element
  xmlSAX2EndElementNs(ctx, localname, prefix, URI);

  // only handle unit elements
  if (strcmp((const char*) localname, "unit") != 0)
    return;

  // evaluate the xpath on the context from the current document
  xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(pstate->compiled_xpath, pstate->context);
  if (result_nodes == 0) {
    fprintf(stderr, "ERROR\n");
    return;
  }

  int result_size = 0;

  bool outputunit = false;

  xmlNodePtr onode = 0;

  xmlNodePtr a_node = xmlDocGetRootElement(ctxt->myDoc);

  const char* unit_filename = (const char*) xmlGetProp(a_node, BAD_CAST "filename");
  const char* unit_directory =  (const char*) xmlGetProp(a_node, BAD_CAST "dir");

  char s[1000] = { 0 };

  // process the resulting nodes
  pstate->nodetype = result_nodes->type;

  switch (pstate->nodetype) {

    // node set result
  case XPATH_NODESET:

    if (!pstate->needroot) {
      xmlOutputBufferWrite(pstate->buf, pstate->rootbuf->use, (const char*) pstate->rootbuf->content);
      pstate->needroot = true;
    }

    // may not have any values
    if (!result_nodes->nodesetval)
      break;

    // may not have any results
    result_size = xmlXPathNodeSetGetLength(result_nodes->nodesetval);
    if (result_size == 0)
      break;

    // first time found a node result, so close root unit start tag
    if (!pstate->found) {
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(">\n\n"));
      pstate->found = true;
    }

    // output all the found nodes
    for (int i = 0; i < xmlXPathNodeSetGetLength(result_nodes->nodesetval); ++i) {

      onode = xmlXPathNodeSetItem(result_nodes->nodesetval, i);

      // output a unit element around the fragment, unless
      // is is already a unit
      outputunit = strcmp("unit", (const char*) onode->name) != 0;

      // if we need a unit, output the start tag.  Line number starts at 1, not 0
      if (outputunit) {

	// unit start tag
	xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("<unit"));

	// directory attribute
	if (unit_directory) {
	  //	  sprintf(s, " dir=\"%s\"", unit_directory);
	  xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(" dir=\""));
	  xmlOutputBufferWriteString(pstate->buf, (const char*) unit_directory);
	  xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\""));
	  //	  xmlOutputBufferWriteString(pstate->buf, (const char*) s);
	}

	// filename attribute
	if (unit_filename) {
	  //	  sprintf(s, " dir=\"%s\"", unit_filename);
	  xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL((" filename=\"")));
	  xmlOutputBufferWriteString(pstate->buf, (const char*) unit_filename);
	  xmlOutputBufferWrite(pstate->buf, 1, "\"");
	  //	  xmlOutputBufferWriteString(pstate->buf, (const char*) s);
	}

	// line number and clost unit start tag
	sprintf(s, " line=\"%ld\">", xmlGetLineNo(onode));
	xmlOutputBufferWriteString(pstate->buf, s);
      }

      // save the result, but temporarily hide the namespaces
      xmlNsPtr savens = onode ? onode->nsDef : 0;
      if (savens && !isoption(pstate->options, OPTION_XSLT_ALL))
	onode->nsDef = 0;
      xmlNodeDumpOutput(pstate->buf, ctxt->myDoc, onode, 0, 0, 0);
      if (savens && !isoption(pstate->options, OPTION_XSLT_ALL))
	onode->nsDef = savens;

      // if we need a unit, output the end tag
      if (outputunit)
	xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("</unit>"));

      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\n\n"));
    }

    break;

    // numeric result
  case XPATH_NUMBER:
    pstate->total += result_nodes->floatval;
    break;

    // boolean result
  case XPATH_BOOLEAN:
    pstate->result_bool |= result_nodes->boolval;
    break;

  default:
    fprintf(stderr, "Unhandled type\n");
    break;
  };

  // finished with the result nodes
  xmlXPathFreeObject(result_nodes);

  SAX2UnitDOM::endElementNs(ctx, localname, prefix, URI);
}

// end document
void SAX2UnitDOMXPath::endDocument(void *ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOMXPath* pstate = (SAX2UnitDOMXPath*) ctxt->_private;

  xmlSAX2EndDocument(ctx);

  // finalize results
  switch (pstate->nodetype) {
  case XPATH_NODESET:

    // root unit end tag
    if (!isoption(pstate->options, OPTION_XSLT_ALL)) {
      if (pstate->found)
	xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("</unit>" "\n"));
      else
	xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("/>" "\n"));
    }
    break;

  case XPATH_NUMBER:
    printf((int)pstate->total == pstate->total ? "%.0lg\n" : "%lg\n", pstate->total);
    break;

  // boolean result
  case XPATH_BOOLEAN:
    puts(pstate->result_bool ? "true\n" : "false\n");
    break;

  default:
    break;
  }

  // standard end document
  SAX2UnitDOM::endDocument(ctx);
}
