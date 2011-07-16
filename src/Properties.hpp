/*
  Properties.hpp

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

#ifndef INCLUDED_PROPERTIES_HPP
#define INCLUDED_PROPERTIES_HPP

void output_info(srcMLUtility& su, int options, int optioncount, int optionorder[]);

#include "ProcessUnit.hpp"
#include "srcmlapps.hpp"

class Properties : public CountUnits {
 public :
  Properties(srcMLUtility& su, PROPERTIES_TYPE&nsv, PROPERTIES_TYPE& attrv, int optioncount, int optionorder[], FILE * output = stdout)
    : su(su), nsv(nsv), attrv(attrv), optioncount(optioncount), optionorder(optionorder), output(output)
    {}

  srcMLUtility& su;
  PROPERTIES_TYPE& nsv;
  PROPERTIES_TYPE& attrv;
  int optioncount;
  int* optionorder;
  FILE * output;

 public :

  // extract namespace and attributes from root unit element
  void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    // encoding is entered as a property
    const char* encoding = (const char*) (ctxt->encoding ? ctxt->encoding : ctxt->input->encoding);

    if (encoding) {
      int i;
      // TODO: Use constant here
      for (i = 0; i < MAXPROPERTIES; ++i)
        if (attrv[i].first == "")
          break;

      attrv[i].first = ".encoding";
      attrv[i].second = encoding;
    }

    // collect namespaces
    collect_namespaces(nb_namespaces, namespaces, nsv);

    // collect attributes
    collect_attributes(nb_attributes, attributes, attrv);

    // output the current data except for the completion of the nested unit count
    if (pstate->unit < 1) {
      output_info(su, *(pstate->poptions), optioncount, optionorder);
    }

    if (pstate->unit < 1 && !isoption(*(pstate->poptions), OPTION_LONG_INFO)) {
      ctxt->sax->startElementNs = 0;
      ctxt->sax->characters = 0;
      ctxt->sax->ignorableWhitespace = 0;
      ctxt->sax->endElementNs = 0;
      xmlStopParser(ctxt);
      return;
    }

    if (pstate->unit < 1 && isoption(*(pstate->poptions), OPTION_LONG_INFO)) {
      pstate->unit = -1;
      CountUnits* pcount = new CountUnits(output);
      pstate->pprocess = pcount;
    }
  }

  // extract namespace and attributes from root unit element
  void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                 int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                 const xmlChar** attributes) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    // collect namespaces
    collect_namespaces(nb_namespaces, namespaces, nsv);

    // collect attributes
    collect_attributes(nb_attributes, attributes, attrv);

    // encoding is entered as a property
    const char* encoding = (const char*) (ctxt->encoding ? ctxt->encoding : ctxt->input->encoding);

    if (encoding) {
      int i;
      for (i = 0; i < MAXPROPERTIES; ++i)
        if (attrv[i].first == "")
          break;

      attrv[i].first = ".encoding";
      attrv[i].second = encoding;
    }

    // output the current data except for the completion of the nested unit count
    output_info(su, *(pstate->poptions), optioncount, optionorder);

    ctxt->sax->startElementNs = 0;
    ctxt->sax->characters = 0;
    ctxt->sax->ignorableWhitespace = 0;
    ctxt->sax->endElementNs = 0;
    xmlStopParser(ctxt);
  }
};

#endif
