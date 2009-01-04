/*
  srcMLUtility.cpp

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

#include "SAX2CountUnits.h"
#include "SAX2Utilities.h"

#include <iostream>
#include <iomanip>
#include "Options.h"

namespace SAX2CountUnits {

  static int placescount = 1;
  static int placesunit = 10;
  static int fieldwidth = 5;

  xmlSAXHandler factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized    = XML_SAX2_MAGIC;
    sax.endElementNs = &endElementNs;
    //    sax.endDocument  = &endDocument;

    return sax;
  }

  // end document
  void endDocument(void *user_data) {

    State* pstate = (State*) user_data;

    // output file status message if in verbose mode
    if (pstate->verbose)
      std::cerr << '\r';
  }

  // end unit element and current file/buffer (started by startElementNs
  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    State* pstate = (State*) ctx;

    if (pstate->ctxt->nameNr != 2)
      return;

    ++(pstate->count);

    // output file status message if in verbose mode
    if (pstate->verbose)
      std::cerr << '\r' << std::setw(fieldwidth) << pstate->count;
  }
};
