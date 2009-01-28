/*
  SAX2CountUnits.h

  Copyright (C) 2008 SDML (www.sdml.info)

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

  A SAX2 handler for libxml2 that converts input to CountUnits.  It performs
  a (attempted) identical copy of the input.  Typically used for part of
  an xml document, e.g., an individual subpart.  It can also be customized
  by overriding individual callbacks.
*/

#ifndef INCLUDED_SAX2COUNTUNITS_H
#define INCLUDED_SAX2COUNTUNITS_H

#include <libxml/parser.h>
#include <libxml/xmlwriter.h>

class SAX2CountUnits {
 public:

  // constructor
  SAX2CountUnits(int unit, int& options);

  static xmlSAXHandler factory();

  xmlParserCtxtPtr ctxt;
  int unit;
  int options;
  long count;
  bool verbose;

  static void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);
};

#endif
