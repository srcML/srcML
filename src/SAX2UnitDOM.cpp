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

#include "SAX2UnitDOM.h"
#include "SAX2Utilities.h"
#include "srcmlns.h"

#include "Options.h"
#include "srcexfun.h"

#include <libxml/parserInternals.h>
#include <libxml/SAX2.h>

#include <cstring>
#include <cassert>

SAX2UnitDOM::SAX2UnitDOM(const char* a_context_element, const char* a_ofilename, int options) 
  : context_element(a_context_element), ofilename(a_ofilename), options(options), found(false), nb_ns(0), ns(0), isnested(false), count(0) {

}

xmlSAXHandler SAX2UnitDOM::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument  = &SAX2UnitDOM::startDocument;
  sax.endDocument    = &SAX2UnitDOM::endDocument;
  sax.startElementNs = &SAX2UnitDOM::startElementNsRoot;
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

    // setup output
    pstate->buf = xmlOutputBufferCreateFilename(pstate->ofilename, NULL, 0);

    // TODO:  STATIC, should be based on context
    xmlOutputBufferWriteString(pstate->buf, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
    pstate->rootbuf = xmlBufferCreate();

    pstate->needroot = false;

    xmlSAX2StartDocument(ctx);
}

// handle unit elements (only) of compound document
void SAX2UnitDOM::startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

  // store the namespaces as we will need them later (with the nested unit elements)
  pstate->nb_ns = nb_namespaces;
  pstate->ns = (char**) malloc((2 * (nb_namespaces + 10) + 2) * sizeof(char*));

  for (int i = 0; i < 2 * nb_namespaces; ++i)
    pstate->ns[i] = namespaces[i] ? strdup((char*) namespaces[i]) : 0;

  xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes,
  			nb_defaulted, attributes);

  ctxt->input->line = 1;

  // build the individual unit start element, but use the namespaces from the outer unit
  xmlNodePtr onode = xmlDocGetRootElement(ctxt->myDoc);

  // store the root start element
  xmlBufferCat(pstate->rootbuf, BAD_CAST "<");
  if (prefix != NULL) {
    xmlBufferCat(pstate->rootbuf, prefix);
    xmlBufferCat(pstate->rootbuf, BAD_CAST ":");
  }
  xmlBufferCat(pstate->rootbuf, localname);

  // output the namespaces
  for (xmlNsPtr pAttr =  onode->nsDef; pAttr != 0; pAttr = pAttr->next)
    if(pAttr->prefix == 0 || strcmp((const char *) pAttr->prefix, "cpp") != 0)
      xmlNodeDump(pstate->rootbuf, ctxt->myDoc, (xmlNodePtr) pAttr, 0, 0);

  // output the attributes
  for (xmlAttrPtr pAttr = onode->properties; pAttr; pAttr = pAttr->next)
    if(strcmp((const char *) pAttr->name, "language") != 0)
      xmlNodeDump(pstate->rootbuf, onode->doc, (xmlNodePtr) pAttr, 0, 0);

  collect_attributes(nb_attributes, attributes, pstate->root_attributes);

  setRootAttributes(pstate->root_attributes);

  // look for nested unit
  ctxt->sax->startElementNs = &SAX2UnitDOM::startElementNsFirstUnit;
}

// handle unit elements (only) of compound document
void SAX2UnitDOM::startElementNsFirstUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

  // if reached a non-unit as the first nested element, then we have a non-nested xml file
  // so we need to process normally
  if (isoption(pstate->options, OPTION_XSLT_ALL) || strcmp((const char*) localname, "unit") != 0) {

    pstate->count = 1;
    setPosition(pstate->count);
    xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes,
  			nb_defaulted, attributes);
    ctxt->sax->startElementNs = xmlSAX2StartElementNs;
    return;
  }

  pstate->isnested = true;

  // unhook the unit tree from the document, leaving an empty document
  xmlNodePtr onode = xmlDocGetRootElement(ctxt->myDoc);
  xmlUnlinkNode(onode);
  ctxt->node = 0;
  xmlFreeNode(onode);

  // standard unit element handling
  startElementNsUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes,
  			nb_defaulted, attributes);
}

// handle unit elements (only) of compound document
void SAX2UnitDOM::startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

  ++pstate->count;
  setPosition(pstate->count);

  // reset the line to agree with the line of the original text file
  ctxt->input->line = 1;

  // tack on the namespaces from this unit
  // TODO:  Have a limit here, don't we?
  for (int i = 0; i < 2 * nb_namespaces; ++i)
    pstate->ns[pstate->nb_ns * 2 + i] = (char*) namespaces[i];
  
  xmlSAX2StartElementNs(ctx, localname, prefix, URI, pstate->nb_ns + nb_namespaces, (const xmlChar**) pstate->ns, nb_attributes,
  			nb_defaulted, attributes);

  // turn tree building start element back on (instead of this one)
  ctxt->sax->startElementNs = xmlSAX2StartElementNs;
  ctxt->sax->characters     = xmlSAX2Characters;
}

// end unit element and current file/buffer (started by startElementNs
void SAX2UnitDOM::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  xmlNodePtr onode = xmlDocGetRootElement(ctxt->myDoc);

  xmlUnlinkNode(onode);
  xmlFreeNode(onode);

  // unhook the unit tree from the document, leaving an empty document
  ctxt->node = 0;

  // now need to detect the start of the next unit
  ctxt->sax->startElementNs = &SAX2UnitDOM::startElementNsUnit;
  ctxt->sax->characters     = 0;
}

// end document
void SAX2UnitDOM::endDocument(void *ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctxt->_private;

  // all done with the buffer
  xmlOutputBufferClose(pstate->buf);
}
