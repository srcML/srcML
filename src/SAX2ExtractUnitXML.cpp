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

#include "SAX2TextWriter.h"
#include "SAX2ExtractUnitXML.h"
#include "SAX2Utilities.h"
#include "srcmlns.h"

#include <cstdio>
#include <cstring>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include "Options.h"
#include "srcmlns.h"

SAX2ExtractUnitXML::SAX2ExtractUnitXML(const char* ofilename, int unit, int& options, PROPERTIES_TYPE& nsv, PROPERTIES_TYPE& attrv):
  SAX2TextWriter(ofilename, options, unit), nsv(nsv), attrv(attrv), placescount(0), placesunit(0) {

}

xmlSAXHandler SAX2ExtractUnitXML::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;
  sax.startElementNs = &startElementNsRoot;
  sax.startDocument  = &SAX2TextWriter::startDocument;

  return sax;
}

// handle root unit of compound document
void SAX2ExtractUnitXML::startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

    SAX2ExtractUnitXML* pstate = (SAX2ExtractUnitXML*) ctx;

    // start counting units after the root
    pstate->count = 0;

    // collect namespaces
    collect_namespaces(nb_namespaces, namespaces, pstate->nsv);

    // collect attributes
    collect_attributes(nb_attributes, attributes, pstate->attrv);

    // handle nested units
    pstate->ctxt->sax->startElementNs = pstate->unit == 1 ? &startElementNsUnit : 0;
    pstate->ctxt->sax->endElementNs   = pstate->unit == 1 ? 0 : &endElementNs;

    pstate->placescount = 1;
    pstate->placesunit = 10;

    // output file status message if in verbose mode
    if (isoption(pstate->options, OPTION_VERBOSE))
      fprintf(stderr, "Count:  ");
}

// start a new output buffer and corresponding file for a
// unit element
void SAX2ExtractUnitXML::startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    SAX2ExtractUnitXML* pstate = (SAX2ExtractUnitXML*) ctx;

    // mostly we count the end elements for units, but now count the start
    ++(pstate->count);

    // start element with proper prefix
    const char* name = qname((const char*) prefix, (const char*) localname);
    xmlTextWriterStartElement(pstate->writer, BAD_CAST name);

    // merge this units namespaces
    collect_namespaces(nb_namespaces, namespaces, pstate->nsv);

    // output the merged namespaces
    if (!isoption(pstate->options, OPTION_XMLDECL))
      for (PROPERTIES_TYPE::const_iterator iter = pstate->nsv.begin(); iter != pstate->nsv.end(); ++iter)
	xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST iter->second.c_str(), BAD_CAST iter->first.c_str());

    // merge this units attributes
    collect_attributes(nb_attributes, attributes, pstate->attrv);

    // output the merged attributes
    for (PROPERTIES_TYPE::const_iterator iter = pstate->attrv.begin(); iter != pstate->attrv.end(); iter++)
      xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST iter->first.c_str(), BAD_CAST iter->second.c_str());

    // now really start
    pstate->ctxt->sax->endDocument    = &SAX2TextWriter::endDocument;
    pstate->ctxt->sax->startElementNs = &SAX2TextWriter::startElementNs;
    pstate->ctxt->sax->endElementNs   = &SAX2TextWriter::endElementNs;
    pstate->ctxt->sax->characters     = &SAX2TextWriter::characters;
    pstate->ctxt->sax->comment        = &SAX2TextWriter::comments;
}

// end unit element and current file/buffer (started by startElementNs
void SAX2ExtractUnitXML::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    SAX2ExtractUnitXML* pstate = (SAX2ExtractUnitXML*) ctx;

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
    if (isoption(pstate->options, OPTION_VERBOSE)) {
      for (int i = 0; i < pstate->placescount; ++i)
	fprintf(stderr, "\b");
      fprintf(stderr, "%ld", pstate->count);

      if (pstate->count == pstate->placesunit) {
	pstate->placesunit *= 10;
	++pstate->placescount;
      }
    }

    if (pstate->count < pstate->unit - 1)
      return;

    // now ready for the next unit, to treat as root
    pstate->ctxt->sax->startElementNs = &startElementNsUnit;
    pstate->ctxt->sax->endElementNs = 0;
}
