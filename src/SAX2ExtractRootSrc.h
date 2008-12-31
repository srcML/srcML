/*
  SAX2ExtractRootSrc.h

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

#ifndef INCLUDED_SAX2EXTRACTROOTSRC
#define INCLUDED_SAX2EXTRACTROOTSRC

#include "srcMLUtility.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "xmloutput.h"
#include <xmlsave.h>
#include "srcmlns.h"
#include <libxml/parser.h>
#include <libxml/parserInternals.h>

#include "SAX2ExtractUnitsSrc.h"

namespace SAX2ExtractRootSrc {

  xmlSAXHandler factory();

  struct State {
    int unit;
    long count;
    const char * root_filename;
    const char * ofilename;
    xmlCharEncodingHandlerPtr handler;
    int* poptions;
    xmlOutputBufferPtr output;
    xmlParserCtxtPtr ctxt;
  };

  using SAX2ExtractUnitsSrc::characters;
  using SAX2ExtractUnitsSrc::startElementNsEscape;

  void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
				     const xmlChar** attributes);

  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);
};

#endif
