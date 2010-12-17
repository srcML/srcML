/*
  SAX2ExtractRootSrc.cpp

  Copyright (C) 2004-2006  SDML (www.sdml.info)

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

#include "SAX2ExtractRootSrc.hpp"
#include "Options.hpp"
#include "srcmlns.hpp"

namespace SAX2ExtractRootSrc {

  xmlSAXHandler factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized    = XML_SAX2_MAGIC;
    sax.startElementNs = &startElementNs;

    return sax;
  }

  // start a new output buffer and corresponding file for a unit element
  void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    // update count for this buffer
    ++(pstate->count);

    pstate->output = xmlOutputBufferCreateFilename(pstate->ofilename,
						   pstate->handler, 
						   isoption(*(pstate->poptions), OPTION_COMPRESSED) ? 1 : 0);
    if (pstate->output == NULL) {
      fprintf(stderr, "HERE %s %p\n", pstate->ofilename, pstate->handler);
      fprintf(stderr, "Output buffer error\n");
      xmlStopParser(pstate->ctxt);
    }

    // output source encoding
    if (isoption(*(pstate->poptions), OPTION_VERBOSE)) {
      fprintf(stderr, "XML encoding:  %s\n", pstate->ctxt->encoding ? pstate->ctxt->encoding : pstate->ctxt->input->encoding);
      fprintf(stderr, "Output encoding:  %s\n", pstate->handler->name);
    }

    // next state is to copy the unit contents, finishing when needed
    pstate->ctxt->sax->startElementNs = &startElementNsEscape;
    pstate->ctxt->sax->characters = &characters;
    pstate->ctxt->sax->endElementNs = &endElementNs;
  }

  // end unit element and current file/buffer (started by startElementNsUnit
  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    State* pstate = (State*) ctx;

    // only need to process end element of current unit
    int stop = 1;
    if (pstate->unit)
      ++stop;
    if (pstate->ctxt->nameNr > stop)
      return;

    xmlOutputBufferClose(pstate->output);

    pstate->ctxt->sax->startElementNs = 0;
    pstate->ctxt->sax->characters = 0;
    pstate->ctxt->sax->ignorableWhitespace = 0;
    pstate->ctxt->sax->endElementNs = 0;

    xmlStopParser(pstate->ctxt);
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

  // output all characters to output buffer
  void characters(void* ctx, const xmlChar* ch, int len) {

    State* pstate = (State*) ctx;

#ifdef __GNUC__
    xmlOutputBufferWrite(pstate->output, len, (const char*) ch);
#else
    const char* c = (const char*) ch;
    int pos = 0;
    const char* chend = (const char*) ch + len;
    while (c < chend) {

      switch (*c) {
      case '\n' :
	xmlOutputBufferWrite(pstate->output, pos, (const char*)(BAD_CAST c - pos));
	pos = 0;
	xmlOutputBufferWrite(pstate->output, EOL_SIZE, EOL);
	break;

      default :
	++pos;
	break;
      };
      ++c;
    }

    xmlOutputBufferWrite(pstate->output, pos, (const char*)(BAD_CAST c - pos));
#endif
  }

};
