/*
  SAX2TextWriter.hpp

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

#ifndef INCLUDED_SAX2TEXTWRITER_H
#define INCLUDED_SAX2TEXTWRITER_H

#include "SAX2Utilities.hpp"
#include <libxml/parser.h>
#include <libxml/xmlwriter.h>
#include "Options.hpp"

class SAX2TextWriter {

 public:

  // constructor
  SAX2TextWriter(const char* ofilename, OPTION_TYPE& options, int unit);

  // sax factory
  static xmlSAXHandler factory();

  // start document
  static void startDocument(void *ctx);

  // end document
  static void endDocument(void *ctx);

  // characters
  static void characters(void* ctx, const xmlChar* ch, int len);

  // startElement
  static void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes);

  // endElement
  static void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);

  // comments
  static void comments(void* ctx, const xmlChar* ch);

  xmlParserCtxtPtr ctxt;
  int unit;

 protected:

  OPTION_TYPE options;
  const char* filename;      // output filename
  xmlTextWriterPtr writer;   // output text writer

};

#endif
