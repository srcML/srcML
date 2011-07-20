/*
  UnitDOM.cpp

  Copyright (C) 2008-2011  SDML (www.sdml.info)

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

#include <sstream>

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libexslt/exslt.h>

#include <libxml/SAX2.h>

#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s
#include "srcexfun.hpp"

#include "ProcessUnit.hpp"

class UnitDOM : public ProcessUnit {
public :

  UnitDOM() {
  }

  ~UnitDOM() {
  }

  virtual void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                             int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                             const xmlChar** attributes) {

    xmlSAX2StartDocument(ctx);
  }

  virtual void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                         int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar** attributes) {

    xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
  }

  virtual void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                              int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                              const xmlChar** attributes) {

    xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
  }

  virtual void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    // DOM building end element
    xmlSAX2EndElementNs(ctx, localname, prefix, URI);
  }

  virtual void characters(void* ctx, const xmlChar* ch, int len) {

    xmlSAX2Characters(ctx, ch, len);
  }

  // comments
  virtual void comments(void* ctx, const xmlChar* ch) {

    xmlSAX2Comment(ctx, ch);
  }

  virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    // DOM building end element
    xmlSAX2EndElementNs(ctx, localname, prefix, URI);

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlNodePtr thisnode = xmlDocGetRootElement(ctxt->myDoc);

    xmlUnlinkNode(thisnode);
    xmlFreeNode(thisnode);

    // unhook the unit tree from the document, leaving an empty document
    ctxt->node = 0;
  }

  virtual void endRootUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    xmlSAX2EndDocument(ctx);
  }

private :
  const char* context_element;
  const char* ofilename;
  int options;
  const char** fxpath;
  xmlXPathContextPtr context;
  xmlXPathCompExprPtr compiled_xpath;
  double total;
  bool result_bool;
  int nodetype;
  char* prev_unit_filename;
  int itemcount;
  bool found;
  xmlOutputBufferPtr buf;
};

#endif
