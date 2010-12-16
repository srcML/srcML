/*
  SAX2UnitDOMXPath.h

  Copyright (C) 2010 SDML (www.sdml.info)

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

  A SAX2 handler for libxml2 that converts input to ExtractUnitXML.  It performs
  a (attempted) identical copy of the input.  Typically used for part of
  an xml document, e.g., an individual subpart.  It can also be customized
  by overriding individual callbacks.
*/

#ifndef INCLUDED_SAX2UNITDOMXPATH_H
#define INCLUDED_SAX2UNITDOMXPATH_H

#include "SAX2UnitDOM.hpp"

#include <libxml/parser.h>
#include <libxml/xpath.h>

class SAX2UnitDOMXPath : public SAX2UnitDOM {
 public:

  SAX2UnitDOMXPath(const char* context_element, const char** fxpath, const char* ofilename, int options);

  ~SAX2UnitDOMXPath();

  static xmlSAXHandler factory();

  const char** fxpath;
  xmlXPathContextPtr context;
  xmlXPathCompExprPtr compiled_xpath;
  double total;
  bool result_bool;
  int nodetype;
  char* prev_unit_filename;
  int itemcount;

  // start document
  static void startDocument(void *ctx);

  // end document
  static void endDocument(void *ctx);

  // end unit elements
  static void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);
};

#endif
