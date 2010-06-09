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
#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/parserInternals.h>

#include <libexslt/exslt.h>

#include <libxml/SAX2.h>

#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s

SAX2UnitDOMXPath::SAX2UnitDOMXPath(const char* a_context_element, const char* a_fxpath[], const char* a_ofilename, const char* params[], int paramcount, int options) 
  : context_element(a_context_element), fxpath(a_fxpath), ofilename(a_ofilename), params(params), paramcount(paramcount), options(options), found(false), nb_ns(0), ns(0), context(0) {

}

xmlSAXHandler SAX2UnitDOMXPath::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument  = &SAX2UnitDOMXPath::startDocument;
  sax.endDocument    = &SAX2UnitDOMXPath::endDocument;
  sax.startElementNs = &SAX2UnitDOMXPath::startElementNsRoot;
  sax.endElementNs   = &SAX2UnitDOMXPath::endElementNs;
  sax.characters     = 0; //xmlSAX2Characters;
  sax.ignorableWhitespace = xmlSAX2Characters;
  sax.comment        = xmlSAX2Comment;
  sax.processingInstruction = xmlSAX2ProcessingInstruction;

  return sax;
}

// start document
void SAX2UnitDOMXPath::startDocument(void *ctx) {
  
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

    SAX2UnitDOMXPath* pstate = (SAX2UnitDOMXPath*) ctxt->_private;

    // allow for all exslt functions
    //    exsltRegisterAll();

    //    xsltsrcMLRegister();

    if (!pstate->fxpath[0][0])
      return;

    // compile the xpath that will be applied to each unit
    pstate->compiled_xpath = xmlXPathCompile(BAD_CAST pstate->fxpath[0]);
    if (pstate->compiled_xpath == 0) {
	return;
    }

    // setup the context up on which the xpath will be evaluated on
    pstate->context = xmlXPathNewContext(ctxt->myDoc);

    // parse the stylesheet
    //    pstate->xslt = xsltParseStylesheetFile(BAD_CAST pstate->fxslt[0]);

    // setup output
    pstate->buf = xmlOutputBufferCreateFilename(pstate->ofilename, NULL, 0);

    xmlSAX2StartDocument(ctx);
}

// end document
void SAX2UnitDOMXPath::endDocument(void *ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOMXPath* pstate = (SAX2UnitDOMXPath*) ctxt->_private;

  xmlSAX2EndDocument(ctx);

  // root unit end tag
  if (!isoption(pstate->options, OPTION_XSLT_ALL)) {
    if (pstate->found)
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("</unit>" "\n"));
    else
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("/>" "\n"));
  }

  // all done with the buffer
  xmlOutputBufferClose(pstate->buf);
}

// handle unit elements (only) of compound document
void SAX2UnitDOMXPath::startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOMXPath* pstate = (SAX2UnitDOMXPath*) ctxt->_private;

  pstate->nb_ns = nb_namespaces;
  pstate->ns = (char**) malloc((2 * nb_namespaces + 2) * sizeof(char*));

  for (int i = 0; i < 2 * nb_namespaces; ++i)
    pstate->ns[i] = namespaces[i] ? strdup((char*) namespaces[i]) : 0;

  // output the root node
  xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("<"));
  if (prefix != NULL) {
    xmlOutputBufferWriteString(pstate->buf, (const char *) prefix);
    xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(":"));
  }

  xmlOutputBufferWriteString(pstate->buf, (const char *) localname);

  for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2) {

    const char* prefix = (const char*) namespaces[index];
    const char* uri = (const char*) namespaces[index + 1];

    xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(" "));
    xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("xmlns"));
    if (prefix) {
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(":"));
      xmlOutputBufferWriteString(pstate->buf, prefix);
    }
    xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("=\""));
    xmlOutputBufferWriteString(pstate->buf, uri);
    xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\""));
  }

  for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {

    const char* name = qname((const char*) attributes[index + 1], (const char*) attributes[index]);

    xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(" "));
    if (attributes[index + 1]) {
      xmlOutputBufferWriteString(pstate->buf, (const char*) attributes[index + 1]);
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(":"));
    }
    xmlOutputBufferWriteString(pstate->buf, (const char*) attributes[index]);
    xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("=\""));
    xmlOutputBufferWrite(pstate->buf, attributes[index + 4] - attributes[index + 3], (const char*) attributes[index + 3]);
    xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\""));
  }

  xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(">"));

  // look for nested unit
  ctxt->sax->startElementNs = &SAX2UnitDOMXPath::startElementNsUnit;

  //  if (depth == 0 && !isoption(pstate->options, OPTION_XSLT_ALL))
  //    xmlNodeDumpOutput(pstate->buf, ctxt->myDoc, ctxt->node, 0, 0, 0);
}

// handle unit elements (only) of compound document
void SAX2UnitDOMXPath::startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOMXPath* pstate = (SAX2UnitDOMXPath*) ctxt->_private;

  // reset the line to agree with the line of the original text file
  ctxt->input->line = 1;

  // build the individual unit start element, but use the namespaces from the outer unit
  xmlSAX2StartElementNs(ctx, localname, prefix, URI, pstate->nb_ns, (const xmlChar**) pstate->ns, nb_attributes,
  			nb_defaulted, attributes);

  // turn tree building start element back on (instead of this one)
  ctxt->sax->startElementNs = xmlSAX2StartElementNs;
  ctxt->sax->characters     = xmlSAX2Characters;
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


  // evaluate the xpath on the context from the current document
  xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(pstate->compiled_xpath, pstate->context);
  if (result_nodes == 0) {
    fprintf(stderr, "ERROR\n");
    return;
  }

  // update the node type
  int nodetype = result_nodes->type;

  int result_size = 0;

  bool outputunit = false;

  xmlNodePtr onode = 0;

  const char* unit_directory = "";
  const char* unit_filename = "";
  char s[50] = { 0 };
  int line = 0;

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
    if (!pstate->found) {
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(">\n\n"));
      pstate->found = true;
    }

    onode = xmlXPathNodeSetItem(result_nodes->nodesetval, 0);

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
	  xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(" dir=\""));
	  xmlOutputBufferWriteString(pstate->buf, (const char*) unit_directory);
	  xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\""));
	}

	// filename attribute
	if (unit_filename) {
	  xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL((" filename=\"")));
	  xmlOutputBufferWriteString(pstate->buf, (const char*) unit_filename);
	  xmlOutputBufferWrite(pstate->buf, 1, "\"");
	}

	// line number
	// TODO:  fix line numbering problem
	xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(" line=\""));
	sprintf(s, "%d", line);
	xmlOutputBufferWriteString(pstate->buf, s);
	xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\""));

	// end of unit start tag
	xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(">"));
      }

      // xpath result
      xmlNodeDumpOutput(pstate->buf, ctxt->myDoc, onode, 0, 0, 0);

      // if we need a unit, output the end tag
      if (outputunit) {
	xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("</unit>"));
	xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\n\n"));
      }
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

  xmlXPathFreeContext(pstate->context);

  onode = xmlDocGetRootElement(ctxt->myDoc);

  // unhook the unit tree from the document, leaving an empty document
  xmlDocSetRootElement(ctxt->myDoc, NULL);
  ctxt->node = 0;

  // done with this unit
  xmlUnlinkNode(onode);
  xmlFreeNode(onode);

  // now need to detect the start of the next unit
  ctxt->sax->startElementNs = &SAX2UnitDOMXPath::startElementNsUnit;
  ctxt->sax->characters     = 0;
}
