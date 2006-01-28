/*
  xmlreader.h

  Copyright (C) 2006  SDML (www.sdml.info)

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

  A standalone xml reading package based on the interface that the libxml
  xmlTextReader provides.  Designed to mimic the interface of xmlTextReader so
  that libxml is not a required part of the package.

  This is not a replacement for xmlTextReader.  Only the interface used is implemented.
  Also, many of the encoding features are not provided.
*/

#ifndef XMLREADER_H
#define XMLREADER_H

#include <stack>
#include <string>
#include <vector>
#include "project.h"

#define BAD_CAST

typedef char xmlChar;

struct xmlTextReader {
  std::istream* pin;

  std::string value;
  int type;
  std::string tagname;
  bool isempty;
  int depth;
  std::vector<std::pair<std::string, std::string> > attributes;
  int state;
};

typedef xmlTextReader* xmlTextReaderPtr;

const int XML_READER_TYPE_ELEMENT = 1;
const int XML_READER_TYPE_END_ELEMENT = 15;
const int XML_READER_TYPE_TEXT = 3;
const int XML_READER_TYPE_SIGNIFICANT_WHITESPACE = 14;

xmlTextReaderPtr xmlNewTextReaderFilename(const char*);

xmlChar* xmlTextReaderGetAttribute(xmlTextReaderPtr, const xmlChar*);

int xmlTextReaderReadState(xmlTextReaderPtr);

const xmlChar* xmlTextReaderConstEncoding(xmlTextReaderPtr);

int xmlTextReaderRead(xmlTextReaderPtr);

int xmlTextReaderDepth(xmlTextReaderPtr);

int xmlTextReaderNodeType(xmlTextReaderPtr);

int xmlTextReaderMoveToNextAttribute(xmlTextReaderPtr);

const xmlChar* xmlTextReaderConstName(xmlTextReaderPtr);

const xmlChar* xmlTextReaderConstValue(xmlTextReaderPtr);

int xmlTextReaderRead(xmlTextReaderPtr);

int xmlTextReaderIsEmptyElement(xmlTextReaderPtr);

void xmlFreeTextReader(xmlTextReaderPtr);

void xmlFree(xmlChar*);

#endif
