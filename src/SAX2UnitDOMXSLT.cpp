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

#include "SAX2TextWriter.h"
#include "SAX2UnitDOMXSLT.h"
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

#include <libexslt/exslt.h>

#include <libxml/SAX2.h>

#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s

SAX2UnitDOMXSLT::SAX2UnitDOMXSLT(const char* a_context_element, const char* a_fxslt[], const char* a_ofilename, const char* params[], int paramcount, int options) 
  : SAX2UnitDOM(a_context_element, a_ofilename, params, paramcount, options), fxslt(a_fxslt) {
}

xmlSAXHandler SAX2UnitDOMXSLT::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument  = &SAX2UnitDOMXSLT::startDocument;
  sax.endDocument    = &SAX2UnitDOMXSLT::endDocument;
  sax.startElementNs = &SAX2UnitDOMXSLT::startElementNsRoot;
  sax.endElementNs   = &SAX2UnitDOMXSLT::endElementNs;
  sax.characters     = 0; //xmlSAX2Characters;
  sax.ignorableWhitespace = xmlSAX2Characters;
  sax.comment        = xmlSAX2Comment;
  sax.processingInstruction = xmlSAX2ProcessingInstruction;

  return sax;
}

// start document
void SAX2UnitDOMXSLT::startDocument(void *ctx) {
  
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

    SAX2UnitDOMXSLT* pstate = (SAX2UnitDOMXSLT*) ctxt->_private;

    // allow for all exslt functions
    exsltRegisterAll();

    xsltsrcMLRegister();

    // parse the stylesheet
    pstate->xslt = xsltParseStylesheetFile(BAD_CAST pstate->fxslt[0]);

    // setup output
    pstate->buf = xmlOutputBufferCreateFilename(pstate->ofilename, NULL, 0);

    xmlSAX2StartDocument(ctx);
}

// handle unit elements (only) of compound document
void SAX2UnitDOMXSLT::startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOMXSLT* pstate = (SAX2UnitDOMXSLT*) ctxt->_private;

  pstate->nb_ns = nb_namespaces;
  pstate->ns = (char**) malloc((2 * nb_namespaces + 2) * sizeof(char*));

  for (int i = 0; i < 2 * nb_namespaces; ++i)
    pstate->ns[i] = namespaces[i] ? strdup((char*) namespaces[i]) : 0;

  // output the root node
  xmlOutputBufferWrite(pstate->buf, 1, "<");
  if (prefix != NULL) {
    xmlOutputBufferWriteString(pstate->buf, (const char *) prefix);
    xmlOutputBufferWrite(pstate->buf, 1, ":");
  }

  xmlOutputBufferWriteString(pstate->buf, (const char *) localname);

  for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2) {

    const char* prefix = (const char*) namespaces[index];
    const char* uri = (const char*) namespaces[index + 1];

    xmlOutputBufferWrite(pstate->buf, 1, " ");
    xmlOutputBufferWrite(pstate->buf, 5, "xmlns");
    if (prefix) {
      xmlOutputBufferWrite(pstate->buf, 1, ":");
      xmlOutputBufferWriteString(pstate->buf, prefix);
    }
    xmlOutputBufferWrite(pstate->buf, 2, "=\"");
    xmlOutputBufferWriteString(pstate->buf, uri);
    xmlOutputBufferWrite(pstate->buf, 1, "\"");
  }

  for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {

    const char* name = qname((const char*) attributes[index + 1], (const char*) attributes[index]);

    xmlOutputBufferWrite(pstate->buf, 1, " ");
    if (attributes[index + 1]) {
      xmlOutputBufferWriteString(pstate->buf, (const char*) attributes[index + 1]);
      xmlOutputBufferWrite(pstate->buf, 1, ":");
    }
    xmlOutputBufferWriteString(pstate->buf, (const char*) attributes[index]);
    xmlOutputBufferWrite(pstate->buf, 2, "=\"");
    xmlOutputBufferWrite(pstate->buf, attributes[index + 4] - attributes[index + 3], (const char*) attributes[index + 3]);
    xmlOutputBufferWrite(pstate->buf, 1, "\"");
  }

  //  xmlOutputBufferWrite(pstate->buf, 1, ">");

  // look for nested unit
  ctxt->sax->startElementNs = &SAX2UnitDOMXSLT::startElementNsUnit;

  //  if (depth == 0 && !isoption(pstate->options, OPTION_XSLT_ALL))
  //    xmlNodeDumpOutput(pstate->buf, ctxt->myDoc, ctxt->node, 0, 0, 0);
}

// handle unit elements (only) of compound document
void SAX2UnitDOMXSLT::startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOMXSLT* pstate = (SAX2UnitDOMXSLT*) ctxt->_private;

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
void SAX2UnitDOMXSLT::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2UnitDOMXSLT* pstate = (SAX2UnitDOMXSLT*) ctxt->_private;

  // DOM building end element
  xmlSAX2EndElementNs(ctx, localname, prefix, URI);

  // only handle unit elements
  if (strcmp((const char*) localname, "unit") != 0)
    return;

  // apply the style sheet to the document, which is the individual unit
  xmlDocPtr res = xsltApplyStylesheetUser(pstate->xslt, ctxt->myDoc, pstate->params, 0, 0, 0);

  // has to be a result, and a non-empty result
  if (res && res->children) {

    // if in per-unit mode and this is the first result found
    if (!pstate->found && !isoption(pstate->options, OPTION_XSLT_ALL)) {
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(">\n\n"));
      pstate->found = true;
    }

    // save the result, but temporarily hide the namespaces
    xmlNodePtr resroot = xmlDocGetRootElement(res);
    xmlNsPtr savens = resroot ? resroot->nsDef : 0;
    if (savens && !isoption(pstate->options, OPTION_XSLT_ALL))
      resroot->nsDef = 0;
    xsltSaveResultTo(pstate->buf, res, pstate->xslt);
    if (savens && !isoption(pstate->options, OPTION_XSLT_ALL))
      resroot->nsDef = savens;

    // put some space between this unit and the next one
    if (!isoption(pstate->options, OPTION_XSLT_ALL))
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\n"));

    // finished with the result of the transformation
    xmlFreeDoc(res);
  }

  // unhook the unit tree from the document, leaving an empty document
  ctxt->node = 0;

  // done with this unit
  xmlNodePtr onode = xmlDocGetRootElement(ctxt->myDoc);
  xmlUnlinkNode(onode);
  xmlFreeNode(onode);

  // now need to detect the start of the next unit
  ctxt->sax->startElementNs = &SAX2UnitDOMXSLT::startElementNsUnit;
  ctxt->sax->characters     = 0;
}

// end document
void SAX2UnitDOMXSLT::endDocument(void *ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOMXSLT* pstate = (SAX2UnitDOMXSLT*) ctxt->_private;

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
