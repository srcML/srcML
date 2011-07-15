/*
  XPathQueryUnits.cpp

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

#ifndef INCLUDED_XPATHQUERYUNITS_HPP
#define INCLUDED_XPATHQUERYUNITS_HPP

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "ProcessUnit.hpp"

class XPathQueryUnits : public ProcessUnit {
 public :

XPathQueryUnits(const char* a_context_element, const char* a_fxpath[], const char* a_ofilename, int options)
  : context_element(a_context_element), ofilename(a_ofilename), options(options), fxpath(a_fxpath), total(0),
    prev_unit_filename(0), itemcount(0) {
}

~XPathQueryUnits() {

  if (prev_unit_filename)
    free(prev_unit_filename);
}

 public :

  virtual void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                 const xmlChar** attributes) {}

  virtual void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                 const xmlChar** attributes) {}

  virtual void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                 const xmlChar** attributes) {}

  virtual void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {}

  virtual void characters(void* ctx, const xmlChar* ch, int len) {}

  // comments
  virtual void comments(void* ctx, const xmlChar* ch) {}

  virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {}

  virtual void endRootUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {}

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
};

#endif
