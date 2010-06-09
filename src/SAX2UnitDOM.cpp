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

SAX2UnitDOM::SAX2UnitDOM(const char* a_context_element, const char* a_fxslt[], const char* a_ofilename, const char* params[], int paramcount, int options) 
  : context_element(a_context_element), fxslt(a_fxslt), ofilename(a_ofilename), params(params), paramcount(paramcount), options(options), found(false), nsv() {

}

xmlSAXHandler SAX2UnitDOM::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument  = &SAX2UnitDOM::startDocument;
  sax.endDocument    = &SAX2UnitDOM::endDocument;
  sax.startElementNs = &SAX2UnitDOM::startElementNs;
  sax.endElementNs   = &SAX2UnitDOM::endElementNs;
  sax.characters     = 0; //xmlSAX2Characters;
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
    pstate->xslt = xsltParseStylesheetFile(BAD_CAST pstate->fxslt[0]);

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

bool first = true;
int nb_ns;
char** ns;

// handle unit elements (only) of compound document
void SAX2UnitDOM::startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

  int depth = ctxt->nodeNr;

  // collect the namespaces from the outer unit.  They will be used with nested units
  if (first) {

    nb_ns = nb_namespaces;
    ns = (char**) malloc((2 * nb_namespaces + 2) * sizeof(char*));

    for (int i = 0; i < 2 * nb_namespaces; ++i)
      ns[i] = namespaces[i] ? strdup((char*) namespaces[i]) : 0;

    first = false;

    return;
  }

  ctxt->input->line = 1;

  xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_ns, (const xmlChar**) ns, nb_attributes,
  			nb_defaulted, attributes);

  // turn tree building start element back on (instead of this one)

  ctxt->sax->startElementNs = xmlSAX2StartElementNs;
  ctxt->sax->characters     = xmlSAX2Characters;

  // copy the start tag of the root element unit for per-unit processing
  //  if (depth == 0 && !isoption(pstate->options, OPTION_XSLT_ALL))
  //    xmlNodeDumpOutput(pstate->buf, ctxt->myDoc, ctxt->node, 0, 0, 0);
}

// end unit element and current file/buffer (started by startElementNs
void SAX2UnitDOM::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

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

    // save the result
    xsltSaveResultTo(pstate->buf, res, pstate->xslt);

    // finished with the result of the transformation
    xmlFreeDoc(res);
    
    // put some space between this unit and the next one
    if (!isoption(pstate->options, OPTION_XSLT_ALL))
      xmlOutputBufferWrite(pstate->buf, SIZEPLUSLITERAL("\n"));
  }

  xmlNodePtr onode = xmlDocGetRootElement(ctxt->myDoc);

  // unhook the unit tree from the document, leaving an empty document
  xmlDocSetRootElement(ctxt->myDoc, NULL);
  ctxt->node = 0;

  // done with this unit
  xmlUnlinkNode(onode);
  xmlFreeNode(onode);

  // now need to detect the start of the next unit
  ctxt->sax->startElementNs = &SAX2UnitDOM::startElementNs;
  ctxt->sax->characters     = 0;
}
