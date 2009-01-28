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
#include "srcmlns.h"
#include <cstring>
#include <libxml/parser.h>

// constructor
SAX2Properties::SAX2Properties(int unit, int& options, PROPERTIES_TYPE& nsv, PROPERTIES_TYPE& attrv) 
  : SAX2CountUnits(unit, options), nsv(nsv), attrv(attrv)
{
  verbose = isoption(options, OPTION_VERBOSE);
}

  xmlSAXHandler SAX2Properties::factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized    = XML_SAX2_MAGIC;
    sax.startElementNs = &startElementNsRoot;

    return sax;
  }

// extract namespace and attributes from root unit element
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

    // if a unit is specified, then move to that unit
    if (pstate->unit) {
      pstate->ctxt->sax->startElementNs = pstate->unit == 1 ? &startElementNsUnit : 0;
      pstate->ctxt->sax->endElementNs   = pstate->unit == 1 ? 0 : &endElementNs;
      return;
    }

    // done collecting unit information
    if (isoption(pstate->options, OPTION_LONG_INFO)) {

      pstate->ctxt->sax->startElementNs = 0;
      pstate->ctxt->sax->endElementNs = &SAX2CountUnits::endElementNs;
      return;
    }

    // really done, only wanted the root unit
    xmlStopParser(pstate->ctxt);
  }

// collect namespace and attribute information about this unit
void SAX2Properties::startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    SAX2Properties* pstate = (SAX2Properties*) ctx;

    // mostly we count the end elements for units, but now count the start
    // so that we know we got here
    ++(pstate->count);

    // merge namespace info with root namespace info
    collect_namespaces(nb_namespaces, namespaces, pstate->nsv);

    // merge attribute info with root attribute info
    collect_attributes(nb_attributes, attributes, pstate->attrv);

    // reached the unit we wanted, so we are finished
    xmlStopParser(pstate->ctxt);
}

// end unit element and current file/buffer (started by startElementNs)
void SAX2Properties::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    SAX2Properties* pstate = (SAX2Properties*) ctx;

    // only counting unit elements nested at depth of 2
    if (pstate->ctxt->nameNr != 2)
      return;

    // double check that this is a nested file (only done the first time)
    if (pstate->count == 0 && !(strcmp((const char*) localname, "unit") == 0 &&
	  strcmp((const char*) URI, "http://www.sdml.info/srcML/src") == 0)) {
      xmlStopParser(pstate->ctxt);
      return;
    }

    // found another unit
    ++(pstate->count);

    // still not to the right unit yet (need to pause right before)
    if (pstate->count < pstate->unit - 1)
      return;

    // next unit is the one we want, treat as root
    pstate->ctxt->sax->startElementNs = &startElementNsUnit;
    pstate->ctxt->sax->endElementNs = 0;
  }
