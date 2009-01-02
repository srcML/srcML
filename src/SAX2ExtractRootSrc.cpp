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

#include "SAX2ExtractRootSrc.h"
#include "Options.h"

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

    pstate->output = xmlOutputBufferCreateFilename(pstate->ofilename, pstate->handler, 
			      isoption(*(pstate->poptions), OPTION_COMPRESSED) ? 1 : 0);

    if (pstate->output == NULL) {
      std::cerr << "Output buffer error" << std::endl;
      xmlStopParser(pstate->ctxt);
    }

    // next state is to copy the unit contents, finishing when needed
    pstate->ctxt->sax->startElementNs = &startElementNsEscape;
    pstate->ctxt->sax->characters = &characters;
    pstate->ctxt->sax->endElementNs = &endElementNs;
  }

  // end unit element and current file/buffer (started by startElementNsUnit
  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    State* pstate = (State*) ctx;

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
    //  }
  }
};
