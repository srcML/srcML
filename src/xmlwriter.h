/*
  xmlwriter.h

  Copyright (C) 2005  SDML (www.sdml.info)

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

  A standalone xml writing package based on the interface that the libxml
  xmlTextWriter provides.  Designed to mimic the interface of xmlTextWriter so
  that libxml is not a required part of the package.

  This is not a replacement for xmlTextWriter.  Only the interface used is implemented.
  Also, many of the encoding features are not provided.
*/

#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <stack>
#include <string>

#define BAD_CAST

struct xmlTextWriter {

  std::ostream* pout;
  std::stack<std::string> elements;
  bool open_start;
};

typedef xmlTextWriter* xmlTextWriterPtr;

struct xmlCharEncodingHandler {
  int input(unsigned char [512], int*, const unsigned char*, int*);
  int output(unsigned char [512], int*, const unsigned char*, int*);
};

typedef xmlCharEncodingHandler* xmlCharEncodingHandlerPtr;

xmlCharEncodingHandlerPtr xmlFindCharEncodingHandler(const char*);

void xmlTextWriterStartDocument(xmlTextWriterPtr, const char* xml_version, const char* xml_encoding, const char* xml_declaration_standalone);

void xmlTextWriterEndDocument(xmlTextWriterPtr);

void xmlTextWriterStartElement(xmlTextWriterPtr, const char* element_name);

void xmlTextWriterEndElement(xmlTextWriterPtr);

void xmlTextWriterWriteAttribute(xmlTextWriterPtr, const char* attribute_name, const char* attribute_value);

void xmlTextWriterWriteRawLen(xmlTextWriterPtr, unsigned char* data, int size);

void xmlTextWriterFlush(xmlTextWriterPtr);

void xmlFreeTextWriter(xmlTextWriterPtr writer);

xmlTextWriterPtr xmlNewTextWriterFilename(const char* filename, int);

void xmlTextWriterSetIndent(xmlTextWriterPtr, int);

#endif
