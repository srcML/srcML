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

SAX2UnitDOM::SAX2UnitDOM(const char* a_context_element, const char* a_fxslt, const char* a_ofilename, const char* params[], int paramcount, int options) 
  : context_element(a_context_element), fxslt(a_fxslt), ofilename(a_ofilename), params(params), paramcount(paramcount), options(options) {

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

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  xmlSAX2EndElementNs(ctx, localname, prefix, URI);

  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

  if (ctxt->nodeNr == 1) {

    ctxt->sax->startElementNs = &SAX2UnitDOM::startElementNs;

    // keep track if we actually get any results
    bool found = false;

       // apply the style sheet to the extracted doc
       xmlDocPtr res = xsltApplyStylesheet(pstate->xslt, ctxt->myDoc, NULL);

       if (res) {

	 // if in per-unit mode and this is the first result found
	 if (!found && !isoption(pstate->options, OPTION_XSLT_ALL)) {
	   xmlOutputBufferWrite(pstate->buf, 3, ">\n\n");
	   found = true;
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

	 // put some space between this unit and the next one
	 if (!isoption(pstate->options, OPTION_XSLT_ALL))
	   xmlOutputBufferWrite(pstate->buf, 1, "\n");
       }

       // finished with the result of the transformation
       xmlFreeDoc(res);

    //    fprintf(stderr, "Line: %d\n", unitnode->line);

    //    xmlDocPtr doc = xmlNewDoc(NULL);

    //    xmlDocSetRootElement(doc, pstate->unitnode);

    xmlUnlinkNode(pstate->unitnode);
    
    xmlFreeNode(pstate->unitnode);

    pstate->unitnode = 0;
  }
}
