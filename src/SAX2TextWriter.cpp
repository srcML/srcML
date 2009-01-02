/*
  SAX2TextWriter.cpp

  Copyright (C) 2008 SDML (www.sdml.info)

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

  A SAX2 handler for libxml2 that converts input to TextWriter.  It performs
  a (attempted) identical copy of the input.  Typically used for part of
  an xml document, e.g., an individual subpart.  It can also be customized
  by overriding individual callbacks.
*/

#include <cstring>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <iostream>

#include "SAX2TextWriter.h"

namespace SAX2TextWriter {

  xmlSAXHandler factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized    = XML_SAX2_MAGIC;
    sax.startDocument  = &SAX2TextWriter::startDocument;
    sax.endDocument    = &SAX2TextWriter::endDocument;
    sax.startElementNs = &SAX2TextWriter::startElementNs;
    sax.endElementNs   = &SAX2TextWriter::endElementNs;
    sax.characters     = &SAX2TextWriter::characters;

    return sax;
  }

  // start document
  void startDocument(void *user_data) {

    State* pstate = (State*) user_data;

    // open the output text writer stream
    // "-" filename is standard output
    pstate->writer = xmlNewTextWriterFilename(pstate->filename, 0);

    // start this document the same as the current document
    xmlTextWriterStartDocument(pstate->writer,
			       (const char*) pstate->ctxt->version,
			       (const char*) pstate->ctxt->encoding,
			       pstate->ctxt->standalone ? "yes" : "no");
  }

  // end document
  void endDocument(void *user_data) {

    State* pstate = (State*) user_data;

    xmlTextWriterEndDocument(pstate->writer);

    xmlFreeTextWriter(pstate->writer);
  }

  // characters
  void characters(void* user_data, const xmlChar* ch, int len) {

    State* pstate = (State*) user_data;

    const char* c = (const char*) ch;
    int pos = 0;
    const char* chend = (const char*) ch + len;
    while (c < chend) {

      switch (*c) {
      case '<' :
	xmlTextWriterWriteRawLen(pstate->writer, BAD_CAST c - pos, pos);
	pos = 0;
	xmlTextWriterWriteRawLen(pstate->writer, BAD_CAST "&lt;", 4);
	break;

      case '>' :
	xmlTextWriterWriteRawLen(pstate->writer, BAD_CAST c - pos, pos);
	pos = 0;
	xmlTextWriterWriteRawLen(pstate->writer, BAD_CAST "&gt;", 4);
	break;

      case '&' :
	xmlTextWriterWriteRawLen(pstate->writer, BAD_CAST c - pos, pos);
	pos = 0;
	xmlTextWriterWriteRawLen(pstate->writer, BAD_CAST "&amp;", 5);
	break;

      default :
	++pos;
	break;
      };
      ++c;
    }

    xmlTextWriterWriteRawLen(pstate->writer, BAD_CAST c - pos, pos);
  }

  // start a new output buffer and corresponding file for a
  // unit element
  void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
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
  }

  // end unit element and current file/buffer (started by startElementNs
  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    State* pstate = (State*) ctx;

    xmlTextWriterEndElement(pstate->writer);

    if (pstate->unit > 0 && pstate->ctxt->nameNr == 2) {

      pstate->ctxt->sax->startDocument  = 0;
      pstate->ctxt->sax->endDocument    = &SAX2TextWriter::endDocument;
      pstate->ctxt->sax->startElementNs = 0;
      pstate->ctxt->sax->endElementNs   = 0;
      pstate->ctxt->sax->characters     = 0;

      xmlStopParser(pstate->ctxt);
    }
  }

};
