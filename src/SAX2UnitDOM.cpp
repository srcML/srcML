/*
  SAX2UnitDOM.cpp

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
#include "SAX2UnitDOM.h"
#include "SAX2Utilities.h"
#include "srcmlns.h"

#include <cstdio>
#include <cstring>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include "Options.h"
#include "srcmlns.h"

#include "srcexfun.h"

#include <libxml/parserInternals.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>

#include <libxml/SAX2.h>

#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s

SAX2UnitDOM::SAX2UnitDOM(const char* a_context_element, const char* a_fxslt, const char* a_ofilename, const char* params[], int paramcount, int options) 
  : context_element(a_context_element), fxslt(a_fxslt), ofilename(a_ofilename), params(params), paramcount(paramcount), options(options), found(false) {

}

xmlSAXHandler SAX2UnitDOM::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument  = &SAX2UnitDOM::startDocument;
  sax.endDocument    = &SAX2UnitDOM::endDocument;
  sax.startElementNs = &SAX2UnitDOM::startElementNs;
  sax.endElementNs   = &SAX2UnitDOM::endElementNs;
  sax.characters     = xmlSAX2Characters;
  sax.ignorableWhitespace = xmlSAX2Characters;
  sax.comment        = xmlSAX2Comment;
  sax.processingInstruction = xmlSAX2ProcessingInstruction;

  return sax;
}

// start document
void SAX2UnitDOM::startDocument(void *ctx) {
  
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

    SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

    // allow for all exslt functions
    exsltRegisterAll();

    xsltsrcMLRegister();

    // parse the stylesheet
    pstate->xslt = xsltParseStylesheetFile(BAD_CAST pstate->fxslt);

    // setup output
    pstate->buf = xmlOutputBufferCreateFilename(pstate->ofilename, NULL, 0);

    xmlSAX2StartDocument(ctx);
}

// end document
void SAX2UnitDOM::endDocument(void *ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

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

// handle root unit of compound document
void SAX2UnitDOM::startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

  int depth = ctxt->nodeNr;

  if (depth == 1)
    ctxt->input->line = 0;


  xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes,
  			nb_defaulted, attributes);
  if (depth == 1) {
    pstate->unitnode = ctxt->node;
    ctxt->sax->startElementNs = xmlSAX2StartElementNs;
  }
}

// end unit element and current file/buffer (started by startElementNs
void SAX2UnitDOM::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  // DOM building end element
  xmlSAX2EndElementNs(ctx, localname, prefix, URI);

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

  // only care about processing a nested unit
  if (ctxt->nodeNr != 1)
    return;

  // apply the style sheet to the document, which is the root element
  // along with the tree of the just-ended unit
  xmlDocPtr res = xsltApplyStylesheet(pstate->xslt, ctxt->myDoc, pstate->params);

  if (res && xmlDocGetRootElement(res)) {

    // if in per-unit mode and this is the first result found
    if (!pstate->found && !isoption(pstate->options, OPTION_XSLT_ALL)) {
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL(">\n\n"));
      pstate->found = true;
    }

    // output the result of the stylesheet
    xmlNodePtr resroot = xmlDocGetRootElement(res);
    xmlNsPtr savens = resroot ? resroot->nsDef : 0;
    if (savens && !isoption(pstate->options, OPTION_XSLT_ALL))
      resroot->nsDef = 0;

    //	 xsltSaveResultTo(buf, res, xslt);
    xmlNodeDumpOutput(pstate->buf, res, resroot, 0, 0, 0);

    if (savens && !isoption(pstate->options, OPTION_XSLT_ALL))
      resroot->nsDef = savens;

    // finished with the result of the transformation
    xmlFreeDoc(res);

    // put some space between this unit and the next one
    if (!isoption(pstate->options, OPTION_XSLT_ALL))
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\n"));
  }

  // done with this unit
  xmlUnlinkNode(pstate->unitnode);
  xmlFreeNode(pstate->unitnode);
  pstate->unitnode = 0;

  // now need to detect the start of the next unit
  ctxt->sax->startElementNs = &SAX2UnitDOM::startElementNs;
}
