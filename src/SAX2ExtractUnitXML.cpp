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

#include <iostream>
#include <cstring>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include "srcmlns.h"

namespace SAX2ExtractUnitXML {

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

    // collect namespaces and attributes
    // copy namespaces
    int index = 0;
    for (int i = 0; i < nb_namespaces; ++i, index += 2) {

      if (namespaces[index]) {
	static char xmlns[256] = "xmlns:";
	strcpy(xmlns + 6, (const char*) namespaces[index]);

	pstate->nsv.insert(std::make_pair<std::string, std::string>((const char*) namespaces[index + 1], xmlns));
      } else
	pstate->nsv.insert(std::make_pair<std::string, std::string>((const char*) namespaces[index + 1], "xmlns"));
    }


    // record all namespaces for future use
    //    for (xmlNsPtr pAttr = xmlTextReaderCurrentNode(reader)->nsDef; pAttr; pAttr = pAttr->next)
    //      pstate->attrv();

    // handle nested units
    pstate->ctxt->sax->startElementNs = pstate->unit == 1 ? &startElementNsUnit : 0;
    pstate->ctxt->sax->endElementNs   = pstate->unit == 1 ? 0 : &endElementNs;
  }

  // start a new output buffer and corresponding file for a
  // unit element
  void startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    // start element with proper prefix
    if (prefix) {
      static char tag[256];
      strcpy(tag, (const char*) prefix);
      strcat(tag, ":");
      strcat(tag, (const char*) localname);
      xmlTextWriterStartElement(pstate->writer, BAD_CAST tag);
    } else
      xmlTextWriterStartElement(pstate->writer, localname);

    // copy namespaces
    int index = 0;
    for (int i = 0; i < nb_namespaces; ++i, index += 2) {

      if (namespaces[index]) {
	static char xmlns[256] = "xmlns:";
	strcpy(xmlns + 6, (const char*) namespaces[index]);

	xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST xmlns, namespaces[index + 1]);
      } else
	xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST "xmlns", namespaces[index + 1]);
    }

    // output the standard namespaces, if they exist
    const char* stdns[] = { SRCML_SRC_NS_URI, SRCML_CPP_NS_URI, SRCML_ERR_NS_URI };
    for (int i = 0; i < (int) (sizeof(stdns) / sizeof(stdns[0])); ++i) {

      std::map<std::string, std::string>::iterator pos = pstate->nsv.find(stdns[i]);
      if (pos != pstate->nsv.end()) {
	xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST pos->second.c_str(), BAD_CAST pos->first.c_str());

	pstate->nsv.erase(pos);
      }
    }

    // output any other namespaces that may exist
    for (std::map<std::string, std::string>::const_iterator iter = pstate->nsv.begin(); iter != pstate->nsv.end(); ++iter) {
      std::string uri = (*iter).first;
      std::string prefix = (*iter).second;
      
      xmlTextWriterWriteAttribute(pstate->writer, BAD_CAST prefix.c_str(), BAD_CAST uri.c_str());
    }

    // copy attributes
    index = 0;
    for (int i = 0; i < nb_attributes; ++i, index += 5) {

      // write the attribute raw so we don't have to convert
      // the begin/end pointers of the attribute value to a string
      xmlTextWriterStartAttribute(pstate->writer, attributes[index]);
      xmlTextWriterWriteRawLen(pstate->writer, attributes[index + 3],
			       attributes[index + 4] - attributes[index + 3]);
      xmlTextWriterEndAttribute(pstate->writer);
    }

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

    if (pstate->count < pstate->unit - 1)
      return;

    // now ready for the next unit, to treat as root
    pstate->ctxt->sax->startElementNs = &startElementNsUnit;
    pstate->ctxt->sax->endElementNs = 0;
  }
};
