/*
  SAX2UnitDOMRelaxNG.hpp

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

#ifndef INCLUDED_SAX2UNITDOMRELAXNG_HPP
#define INCLUDED_SAX2UNITDOMRELAXNG_HPP

#include <libxml/relaxng.h>

#include "SAX2UnitDOM.hpp"

class SAX2UnitDOMRelaxNG : public SAX2UnitDOM {
 public:

  SAX2UnitDOMRelaxNG(const char* context_element, const char** fxslt, const char* ofilename, int options);

  static xmlSAXHandler factory();

  const char** fxslt;
  xmlRelaxNGParserCtxtPtr relaxng;
  xmlRelaxNGPtr rng;
  xmlRelaxNGValidCtxtPtr rngptr;

  // end document
  static void endDocument(void *ctx);

  // start unit elements
  static void startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes);

  // end unit elements
  static void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);
};

#endif
