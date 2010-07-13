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
#include "srcmlns.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include "Options.h"

static const int fieldwidth = 5;

// constructor
SAX2CountUnits::SAX2CountUnits(int unit, OPTION_TYPE& options)
  : unit(unit), options(options), count(0)
{
  verbose = isoption(options, OPTION_VERBOSE);
}

xmlSAXHandler SAX2CountUnits::factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized  = XML_SAX2_MAGIC;
    sax.endElementNs = &endElementNs;

    return sax;
}

// end unit element and current file/buffer (started by startElementNs
void SAX2CountUnits::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  SAX2CountUnits* pstate = (SAX2CountUnits*) ctx;

  if (pstate->ctxt->nameNr != 2)
    return;

  // check that this is a nested file
  if (pstate->count == 0 && !(strcmp((const char*) localname, "unit") == 0 &&
			      strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0)) {
    xmlStopParser(pstate->ctxt);
    return;
  }

  ++(pstate->count);

  // output file status message if in verbose mode
  if (pstate->verbose && !isoption(pstate->options, OPTION_LONG_INFO)) {
    fprintf(stderr, "\r%ld", pstate->count);
    fflush(stdout);
  } else if (isoption(pstate->options, OPTION_LONG_INFO) && isatty(STDOUT_FILENO)) {

    // back up over the previous display
    // yes, this is a hack, but it works
    int c = pstate->count - 1;
    putchar('\b');
    if (c >= 10)
      putchar('\b');
    if (c >= 100)
      putchar('\b');
    if (c >= 1000)
      putchar('\b');
    if (c >= 10000)
      putchar('\b');
    if (c >= 100000)
      putchar('\b');
    if (c >= 1000000)
      putchar('\b');
    if (c >= 10000000)
      putchar('\b');
    if (c >= 100000000)
      putchar('\b');
    if (c >= 1000000000)
      putchar('\b');
    printf("%ld", pstate->count);
    fflush(stdout);
  }
}
