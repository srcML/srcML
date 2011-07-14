/*
  SAX2UnitDOMXSLT.cpp

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

#include "SAX2TextWriter.hpp"
#include "SAX2UnitDOMXSLT.hpp"
#include "SAX2Utilities.hpp"
#include "srcmlns.hpp"

#include <cstring>
#include "Options.hpp"
#include "srcmlns.hpp"

#include "srcexfun.hpp"
#include <cassert>
#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>

#include <libxml/SAX2.h>

#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s

static void apply(xmlParserCtxtPtr ctxt) {

  SAX2UnitDOMXSLT* pstate = (SAX2UnitDOMXSLT*) ctxt->_private;

  setPosition(pstate->count);

  // apply the style sheet to the document, which is the individual unit
  xmlDocPtr res = xsltApplyStylesheetUser(pstate->xslt, ctxt->myDoc, pstate->params, 0, 0, 0);
  if (!res) {
    fprintf(stderr, "srcml2src:  Error in applying stylesheet\n");
    exit(1);
  }

  // only interestd in non-empty results
  if (res && res->children) {

    // determine the type of data that is going to be output
    if (!pstate->found) {
      pstate->result_type = res->children->type;
    }

    // finish the end of the root unit start tag
    // this is only if in per-unit mode and this is the first result found
    // have to do so here because it may be empty
    if (pstate->result_type == XML_ELEMENT_NODE && !pstate->isnested && !pstate->found && !isoption(pstate->options, OPTION_XSLT_ALL)) {
      xmlOutputBufferWriteString(pstate->buf, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
    }

    if (pstate->result_type == XML_ELEMENT_NODE && pstate->isnested && !pstate->found && !isoption(pstate->options, OPTION_XSLT_ALL)) {
      // TODO:  STATIC, should be based on context
      xmlOutputBufferWrite(pstate->buf, pstate->rootbuf->use, (const char*) pstate->rootbuf->content);
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(">\n\n"));

      // TODO: have to free this regardless if not used
      xmlBufferFree(pstate->rootbuf);
    }
    pstate->found = true;

    // save the result, but temporarily hide the namespaces since we only want them on the root element
    xmlNodePtr resroot = xmlDocGetRootElement(res);
    xmlNsPtr savens = resroot ? resroot->nsDef : 0;
    bool turnoff_namespaces = savens && pstate->isnested && !isoption(pstate->options, OPTION_XSLT_ALL);
    if (turnoff_namespaces)
      resroot->nsDef = 0;
    xsltSaveResultTo(pstate->buf, res, pstate->xslt);
    if (turnoff_namespaces)
      resroot->nsDef = savens;

    // put some space between this unit and the next one if compound
    if (pstate->result_type == XML_ELEMENT_NODE && pstate->isnested && !isoption(pstate->options, OPTION_XSLT_ALL))
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\n"));

    // finished with the result of the transformation
    xmlFreeDoc(res);
  }
}

SAX2UnitDOMXSLT::SAX2UnitDOMXSLT(const char* a_context_element, const char* a_fxslt[], const char* a_ofilename, const char* a_params[], int a_paramcount, int options)
  : SAX2UnitDOM(a_context_element, a_ofilename, options), params(a_params), paramcount(a_paramcount), fxslt(a_fxslt) {
}

xmlSAXHandler SAX2UnitDOMXSLT::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument  = &SAX2UnitDOM::startDocument;
  sax.endDocument    = &SAX2UnitDOMXSLT::endDocument;
  sax.startElementNs = &SAX2UnitDOM::startElementNsRoot;
  sax.endElementNs   = &SAX2UnitDOMXSLT::endElementNs;
  sax.characters     = xmlSAX2Characters;
  sax.ignorableWhitespace = xmlSAX2Characters;
  sax.comment        = xmlSAX2Comment;
  sax.processingInstruction = xmlSAX2ProcessingInstruction;

  return sax;
}

// end unit element and current file/buffer (started by startElementNs
void SAX2UnitDOMXSLT::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {
  // DOM building end element
  xmlSAX2EndElementNs(ctx, localname, prefix, URI);

  // only handle unit elements
  if (strcmp((const char*) localname, "unit") != 0)
    return;

  // apply the stylesheet
  apply((xmlParserCtxtPtr) ctx);

  // UnitDom end
  SAX2UnitDOM::endElementNs(ctx, localname, prefix, URI);
}

// end document
void SAX2UnitDOMXSLT::endDocument(void *ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOMXSLT* pstate = (SAX2UnitDOMXSLT*) ctxt->_private;

  xmlSAX2EndDocument(ctx);

  // root unit end tag
  if (pstate->result_type == XML_ELEMENT_NODE && pstate->found && !isoption(pstate->options, OPTION_XSLT_ALL) && pstate->isnested)
    xmlOutputBufferWriteString(pstate->buf, pstate->found ? "</unit>\n" : "/>\n");

  // standard end document
  SAX2UnitDOM::endDocument(ctx);
}
