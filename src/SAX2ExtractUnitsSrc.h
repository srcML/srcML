/*
  SAX2ExtractUnitsSrc.h

  Copyright (C) 2008  SDML (www.sdml.info)

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

  Class for straightforward translation
*/

#ifndef INCLUDED_SAX2EXTRACTUNITSSRC
#define INCLUDED_SAX2EXTRACTUNITSSRC

#include <libxml/parser.h>
#include "srcMLUtility.h"
#include "ProcessUnit.h"

class SAX2ExtractUnitsSrc {
 public:
  static xmlSAXHandler factory();

  ProcessUnit* pprocess;
  OPTION_TYPE* poptions;
  int unit;
  long count;

  // start a new output buffer and corresponding file for a
  // output all characters to output buffer
  static void characters(void* user_data, const xmlChar* ch, int len);

  // handle root unit of compound document
  static void startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                 int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                 const xmlChar** attributes);

  // unit element
  static void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                             int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                             const xmlChar** attributes);

  static void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);

  static void startElementNsEscape(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                   int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                   const xmlChar** attributes);

  static void startElementEscape(void* ctx, const xmlChar* localname, const xmlChar** attributes);
};

#endif
