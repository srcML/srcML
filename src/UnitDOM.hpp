/*
  UnitDOM.cpp

  Copyright (C) 2011  SDML (www.sdml.info)

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

#ifndef INCLUDED_UNITDOM_HPP
#define INCLUDED_UNITDOM_HPP

#include <libxml/SAX2.h>

#include "ProcessUnit.hpp"

class UnitDOM : public ProcessUnit {
public :

  UnitDOM() : first(true) {}

  virtual ~UnitDOM() {}

  virtual void apply(void* ctx) = 0;

  virtual void startOutput(void* ctx) = 0;

  virtual void endOutput(void* ctx) = 0;

  virtual void startDocument(void* ctx) {

    // fprintf(stderr, "%s\n", __FUNCTION__);

    // start the document
    xmlSAX2StartDocument(ctx);

    // setup output
    startOutput(ctx);
  }

  bool first;

  virtual void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                             int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                             const xmlChar** attributes) {
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

    // fprintf(stderr, "%s\n", __FUNCTION__);

    xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
  }

  virtual void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                         int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar** attributes) {

    // fprintf(stderr, "%s\n", __FUNCTION__);

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    if (first){
      // unhook the unit tree from the document, leaving an empty document
      xmlNodePtr onode = xmlDocGetRootElement(ctxt->myDoc);
      xmlUnlinkNode(onode);
      xmlFreeNode(onode);
      ctxt->node = 0;
    }
    first = false;

    xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_namespaces,
			  namespaces, nb_attributes, nb_defaulted, attributes);
  }

  virtual void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                              int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                              const xmlChar** attributes) {

    // fprintf(stderr, "%s\n", __FUNCTION__);

    xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
  }

  virtual void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    // fprintf(stderr, "%s\n", __FUNCTION__);

    xmlSAX2EndElementNs(ctx, localname, prefix, URI);
  }

  virtual void characters(void* ctx, const xmlChar* ch, int len) {

    // fprintf(stderr, "%s\n", __FUNCTION__);

    xmlSAX2Characters(ctx, ch, len);
  }

  // comments
  virtual void comments(void* ctx, const xmlChar* ch) {

    // fprintf(stderr, "%s\n", __FUNCTION__);

    xmlSAX2Comment(ctx, ch);
  }

  virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    // fprintf(stderr, "%s\n", __FUNCTION__);

    // finish building the unit tree
    xmlSAX2EndElementNs(ctx, localname, prefix, URI);

    // apply the necessary processing
    apply(ctx);

    // unhook the unit tree from the document, leaving an empty document
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlNodePtr onode = xmlDocGetRootElement(ctxt->myDoc);
    xmlUnlinkNode(onode);
    xmlFreeNode(onode);
    ctxt->node = 0;
  }

  virtual void endRootUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    // fprintf(stderr, "%s\n", __FUNCTION__);

  }

  virtual void endDocument(void *ctx) {

    // fprintf(stderr, "%s\n", __FUNCTION__);

    // end the entire input document
    xmlSAX2EndDocument(ctx);

    // end the output
    endOutput(ctx);
  }

private :
  PROPERTIES_TYPE nsv;
  PROPERTIES_TYPE attrv;
};

#endif
