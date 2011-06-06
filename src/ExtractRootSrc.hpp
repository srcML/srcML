/*
  ExtractRootSrc.cpp

  Copyright (C) 2010  SDML (www.sdml.info)

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

#ifndef INCLUDED_EXTRACTUNITSSRC_H
#define INCLUDED_EXTRACTUNITSSRC_H

#include "ExtractUnitsSrc.hpp"

using namespace SAX2ExtractRootSrc;

class ExtractRootSrc : public ExtractUnitsSrc {
public :
  virtual void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                             const xmlChar** attributes) {

  }

  virtual void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
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

  }

  virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    State* pstate = (State*) ctx;

    // finish up this file
    xmlOutputBufferClose(output_buffer);

    // stop after this file (and end gracefully) with ctrl-c
    if (isoption(*(pstate->poptions), OPTION_TERMINATE)) {
      xmlStopParser(pstate->ctxt);
      throw TerminateLibXMLError();
    }

    xmlStopParser(pstate->ctxt);
  }

  virtual void endRootUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  }

 private :
  std::string path;
  xmlOutputBufferPtr output_buffer;
};

#endif
