/*
  SAX2ExtractUnitsSrc.cpp

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


*/

#include "SAX2ExtractUnitsSrc.h"
#include "srcmlns.h"
#include "srcmlapps.h"
#include "ProcessUnit.h"
#include "ExtractUnitsSrc.h"

using namespace SAX2ExtractUnitsSrc;

namespace SAX2ExtractUnitsSrc {

  xmlSAXHandler factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized    = XML_SAX2_MAGIC;
    sax.startElementNs = &startElementNsRoot;

    return sax;
  }

  // output all characters to output buffer
  void characters(void* ctx, const xmlChar* ch, int len) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    State* pstate = (State*) ctxt->_private;

    pstate->pprocess->charactersUnit(ctx, ch, len);
  }

  // handle root unit of compound document
  void startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    State* pstate = (State*) ctxt->_private;

    // start counting units after the root
    pstate->count = 0;

    // handle nested units
    pstate->ctxt->sax->startElementNs = &startElementNs;
  }

  // start a new output buffer and corresponding file for a unit element
  void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    State* pstate = (State*) ctxt->_private;

    /*
    // check that this is a nested file
    if (pstate->count == 0 && !(strcmp((const char*) localname, "unit") == 0 &&
	  strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0)) {
      fprintf(stderr, "Options only valid for nested srcML documents\n");
      xmlStopParser(pstate->ctxt);
      return;
    }
    */
    ++(pstate->count);

    // process the start of this unit
    pstate->pprocess->startUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted,
                        attributes);

    // next state is to copy the unit contents, finishing when needed
    pstate->ctxt->sax->startElementNs = &startElementNsEscape;
    //    pstate->ctxt->sax->startElement = &startElementEscape;
    pstate->ctxt->sax->characters = &characters;
    pstate->ctxt->sax->ignorableWhitespace = &characters;
    pstate->ctxt->sax->endElementNs = &endElementNs;
  }

  // end unit element and current file/buffer (started by startElementNs
  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    State* pstate = (State*) ctxt->_private;

    // only process nested unit start elements
    if (pstate->ctxt->nameNr != 2)
      return;

    // process the end of the unit
    pstate->pprocess->endUnit(ctx, localname, prefix, URI);

    // now waiting for start of next unit
    pstate->ctxt->sax->startElementNs = &startElementNs;
    pstate->ctxt->sax->characters = 0;
    pstate->ctxt->sax->endElementNs = 0;
  }

  // escape control character elements
  void startElementNsEscape(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
			    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
			    const xmlChar** attributes) {

    // only reason for this handler is that the escape element
    // needs to be expanded to the equivalent character.
    // So make it as quick as possible, since this is rare
    if (localname[0] == 'e' && localname[1] == 's' &&
	strcmp((const char*) localname, "escape") == 0 &&
 	strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0) {
      
      // convert from the escaped to the unescaped value
      char value = strtod((const char*) attributes[3], NULL);

      characters(ctx, BAD_CAST &value, 1);
    }
  }
};
