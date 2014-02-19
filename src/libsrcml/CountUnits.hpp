/*
  CountUnits.cpp

  Copyright (C) 2008-2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef INCLUDED_COUNTUNITS_HPP
#define INCLUDED_COUNTUNITS_HPP

#include <cstdio>
#include <sstream>
#include <SAX2ExtractUnitsSrc.hpp>
#include <ProcessUnit.hpp>


class CountUnits : public ProcessUnit {
public :
    CountUnits(FILE* poutput = stdout)
        : output(poutput), output_array(0)
    {}

    CountUnits(std::vector<std::string> * output_array)
        : output(0), output_array(output_array)
    {}

public :

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

        xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
        SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

        // check if file to output to
        if(!output && !output_array) return;

        // back up over the previous display
        // yes, this is a hack, but it works
        if(output) {
            if(pstate->count == 1)
                fputc('\b', output);

            for (long place = pstate->count - 1; place > 0; place /= 10) {

                fputc('\b', output);
            }

            fprintf(output, "%ld", pstate->count);
            fflush(output);

        } else {

            std::ostringstream ostream;
            ostream << "units=\"" << pstate->count << "\"";
            output_array->back() = ostream.str();

        }
    }

#pragma GCC diagnostic pop

private:
    FILE* output;
    std::vector<std::string> * output_array;
};

#endif
