/*
  CountUnits.cpp

  Copyright (C) 2008-2010  SDML (www.sdml.info)

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

#ifndef INCLUDED_COUNTUNITS_H
#define INCLUDED_COUNTUNITS_H

#include <cstdio>
#include "SAX2ExtractUnitsSrc.hpp"
#include "ProcessUnit.hpp"

class CountUnits : public ProcessUnit {
 public :
  CountUnits(FILE* poutput = stdout)
    : output(poutput)
  {}

 public :

  virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    if (!isatty(fileno(output)))
      return;

    // output file status message if in verbose mode
    if (!isoption(*(pstate->poptions), OPTION_LONG_INFO)) {
      fprintf(output, "\r%ld", pstate->count);
      fflush(output);
    } else if (isoption(*(pstate->poptions), OPTION_LONG_INFO)) {

      // back up over the previous display
      // yes, this is a hack, but it works
      int c = pstate->count - 1;
      fputc('\b', output);
      if (c >= 10)
        fputc('\b', output);
      if (c >= 100)
        fputc('\b', output);
      if (c >= 1000)
        fputc('\b', output);
      if (c >= 10000)
        fputc('\b', output);
      if (c >= 100000)
        fputc('\b', output);
      if (c >= 1000000)
        fputc('\b', output);
      if (c >= 10000000)
        fputc('\b', output);
      if (c >= 100000000)
        fputc('\b', output);
      if (c >= 1000000000)
        fputc('\b', output);
      fprintf(output, "%ld", pstate->count);
      fflush(output);
    }
  }
private:
  FILE* output;
};

#endif
