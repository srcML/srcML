/*
  SAX2Properties.cpp

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

#include "SAX2TextWriter.h"
#include "SAX2Properties.h"
#include "SAX2CountUnits.h"
#include "SAX2Utilities.h"

#include "Options.h"
#include <iostream>
#include <cstring>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include "srcmlns.h"

// constructor
SAX2Properties::SAX2Properties(int unit, int& options, PROPERTIES_TYPE& nsv, PROPERTIES_TYPE& attrv) 
  : unit(unit), options(options), nsv(nsv), attrv(attrv)
{
  verbose = isoption(options, OPTION_VERBOSE);
}

  xmlSAXHandler SAX2Properties::factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized    = XML_SAX2_MAGIC;
    sax.startElementNs = &startElementNsRoot;
    //    sax.startDocument  = &SAX2TextWriter::startDocument;

    return sax;
  }

  // handle root unit of compound document
  void SAX2Properties::startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    SAX2Properties* pstate = (SAX2Properties*) ctx;

    // start counting units after the root
    pstate->count = 0;

    // collect namespaces
    collect_namespaces(nb_namespaces, namespaces, pstate->nsv);

    // collect attributes
    collect_attributes(nb_attributes, attributes, pstate->attrv);

    // extract from nested unit if needed
    if (pstate->unit) {
      pstate->ctxt->sax->startElementNs = pstate->unit == 1 ? &startElementNsUnit : 0;
      pstate->ctxt->sax->endElementNs   = pstate->unit == 1 ? 0 : &endElementNs;
      return;
    }

    // done if only from root
    pstate->ctxt->sax->startDocument  = 0;
    pstate->ctxt->sax->endDocument    = 0;
    pstate->ctxt->sax->startElementNs = 0;
    pstate->ctxt->sax->endElementNs   = 0;
    pstate->ctxt->sax->characters     = 0;

    if (isoption(pstate->options, OPTION_LONG_INFO)) {

      pstate->ctxt->sax->endElementNs = &SAX2CountUnits::endElementNs;
      return;
    }

    xmlStopParser(pstate->ctxt);
  }

  // start a new output buffer and corresponding file for a
  // unit element
  void SAX2Properties::startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    SAX2Properties* pstate = (SAX2Properties*) ctx;

    // mostly we count the end elements for units, but now count the start
    ++(pstate->count);

    // collect namespaces
    collect_namespaces(nb_namespaces, namespaces, pstate->nsv);

    // collect attributes
    collect_attributes(nb_attributes, attributes, pstate->attrv);
    /*
    pstate->ctxt->sax->startDocument  = 0;
    pstate->ctxt->sax->endDocument    = 0;
    pstate->ctxt->sax->startElementNs = 0;
    pstate->ctxt->sax->endElementNs   = 0;
    pstate->ctxt->sax->characters     = 0;
    */
    xmlStopParser(pstate->ctxt);
  }

  // end unit element and current file/buffer (started by startElementNs
  void SAX2Properties::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    SAX2Properties* pstate = (SAX2Properties*) ctx;

    if (pstate->ctxt->nameNr != 2)
      return;

    // check that this is a nested file
    if (pstate->count == 0 && !(strcmp((const char*) localname, "unit") == 0 &&
	  strcmp((const char*) URI, "http://www.sdml.info/srcML/src") == 0)) {
      xmlStopParser(pstate->ctxt);
      return;
    }

    ++(pstate->count);

    if (pstate->count < pstate->unit - 1)
      return;

    // now ready for the next unit, to treat as root
    pstate->ctxt->sax->startElementNs = &startElementNsUnit;
    pstate->ctxt->sax->endElementNs = 0;
  }
