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

#include <iostream>
#include <cstring>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include "Options.h"
#include "srcmlns.h"

/* srcML unit attributes */
const char* const UNIT_ATTRIBUTE_LANGUAGE = "language";
const char* const UNIT_ATTRIBUTE_DIRECTORY = "dir";
const char* const UNIT_ATTRIBUTE_FILENAME = "filename";
const char* const UNIT_ATTRIBUTE_VERSION = "version";

namespace SAX2ExtractUnitXML {

  static int placescount = 0;
  static int placesunit = 0;

  xmlSAXHandler factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized    = XML_SAX2_MAGIC;
    sax.startElementNs = &startElementNsRoot;
    sax.startDocument  = &SAX2TextWriter::startDocument;

    return sax;
  }

  // handle root unit of compound document
  void startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    // start counting units after the root
    pstate->count = 0;

    // collect namespaces
    for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2)
      pstate->nsv.insert(std::make_pair((const char*) namespaces[index + 1],
					xmlnsprefix((const char*) namespaces[index])));

    // collect attributes
    collect_attributes(nb_attributes, attributes, pstate->attrv);

    // handle nested units
    pstate->ctxt->sax->startElementNs = pstate->unit == 1 ? &startElementNsUnit : 0;
    pstate->ctxt->sax->endElementNs   = pstate->unit == 1 ? 0 : &endElementNs;

    placescount = 1;
    placesunit = 10;

    // output file status message if in verbose mode
    if (isoption(*(pstate->poptions), OPTION_VERBOSE))
      std::cerr << "Count:  ";
  }

  // start a new output buffer and corresponding file for a
  // unit element
  void startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    // start element with proper prefix
    const char* name = qname((const char*) prefix, (const char*) localname);
    xmlTextWriterStartElement(pstate->writer, BAD_CAST name);

    // collect namespaces
    for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2)
      pstate->nsv[(const char*) namespaces[index + 1]] = xmlnsprefix((const char*) namespaces[index]);

    // output the standard namespaces, if they exist
    const char* stdns[] = { SRCML_SRC_NS_URI, SRCML_CPP_NS_URI, SRCML_ERR_NS_URI };
    for (int i = 0; i < (int) (sizeof(stdns) / sizeof(stdns[0])); ++i) {

      // handle standard namespaces
      std::map<std::string, std::string>::iterator pos = pstate->nsv.find(stdns[i]);
      if (pos != pstate->nsv.end()) {

	// output the standard namespace
	xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST pos->second.c_str(), BAD_CAST pos->first.c_str());

	// remove it so that only non-standard namespaces are left
	pstate->nsv.erase(pos);
      }
    }

    // output any other namespaces that may exist
    for (std::map<std::string, std::string>::const_iterator iter = pstate->nsv.begin(); iter != pstate->nsv.end(); ++iter)
      xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST iter->second.c_str(), BAD_CAST iter->first.c_str());

    // copy attributes
    collect_attributes(nb_attributes, attributes, pstate->attrv);

    // put back the standard attributes based on a merge of the root unit and this unit
    const char* stdattr[] = { UNIT_ATTRIBUTE_LANGUAGE, UNIT_ATTRIBUTE_DIRECTORY,
			    UNIT_ATTRIBUTE_FILENAME, UNIT_ATTRIBUTE_VERSION };
    for (int i = 0; i < (int) (sizeof(stdattr) / sizeof(stdattr[0])); ++i) {

      std::map<std::string, std::string>::iterator pos = pstate->attrv.find(stdattr[i]);
      if (pos != pstate->attrv.end()) {

	xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST pos->first.c_str(), BAD_CAST pos->second.c_str());

	pstate->attrv.erase(pos);
      }
    }

    // put in the rest of the attributes
    for (std::map<std::string, std::string>::const_iterator iter = pstate->attrv.begin(); iter != pstate->attrv.end(); iter++)
      xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST iter->first.c_str(), BAD_CAST iter->second.c_str());

    // now really start
    pstate->ctxt->sax->endDocument    = &SAX2TextWriter::endDocument;
    pstate->ctxt->sax->startElementNs = &SAX2TextWriter::startElementNs;
    pstate->ctxt->sax->endElementNs   = &SAX2TextWriter::endElementNs;
    pstate->ctxt->sax->characters     = &SAX2TextWriter::characters;
  }

  // end unit element and current file/buffer (started by startElementNs
  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    State* pstate = (State*) ctx;

    if (pstate->ctxt->nameNr != 2)
      return;

    ++(pstate->count);

    // output file status message if in verbose mode
    if (isoption(*(pstate->poptions), OPTION_VERBOSE)) {
      for (int i = 0; i < placescount; ++i)
	std::cerr << '\b';
      std::cerr << pstate->count;

      if (pstate->count == placesunit) {
	placesunit *= 10;
	++placescount;
      }
    }

    if (pstate->count < pstate->unit - 1)
      return;

    // now ready for the next unit, to treat as root
    pstate->ctxt->sax->startElementNs = &startElementNsUnit;
    pstate->ctxt->sax->endElementNs = 0;
  }
};
