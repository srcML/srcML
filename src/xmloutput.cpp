/*
  xmloutput.cpp

  Copyright (C) 2006  SDML (www.sdml.info)

  The extractor is free software; you can redistribute it and/or modify
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

  An identity output function for libxml2 TextReader and TextWriter.
*/

#include "xmloutput.h"
#include <iostream>
#include <cstring>

// output current reader node to the writer in XML
void outputXML(xmlTextReaderPtr reader, xmlTextWriterPtr writer) {

  bool isemptyelement = false;

  switch (xmlTextReaderNodeType(reader)) {
  case XML_READER_TYPE_ELEMENT:

    // record if this is an empty element since it will be erased by the attribute copying
    isemptyelement = xmlTextReaderIsEmptyElement(reader) > 0;

    // start the element
    xmlTextWriterStartElement(writer, xmlTextReaderConstName(reader));

    // copy all the attributes
    while (xmlTextReaderMoveToNextAttribute(reader)) {
      xmlTextWriterWriteAttribute(writer, xmlTextReaderConstName(reader), xmlTextReaderConstValue(reader));
    }

    // end now if this is an empty element
    if (isemptyelement) {
      xmlTextWriterEndElement(writer);
    }

    break;

  case XML_READER_TYPE_END_ELEMENT:
    xmlTextWriterEndElement(writer);
    break;

  case XML_READER_TYPE_COMMENT:
    xmlTextWriterWriteComment(writer, xmlTextReaderConstValue(reader));
    break;

  case XML_READER_TYPE_WHITESPACE:
  case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
    {
      const xmlChar* s = xmlTextReaderConstValue(reader);
      xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) s, strlen((const char*) s));
    }
    break;

  case XML_READER_TYPE_TEXT:
    {

      // output the UTF-8 buffer escaping the characters.  Note that the output encoding
      // is handled by libxml
      unsigned char* p = (unsigned char*) xmlTextReaderConstValue(reader);
      unsigned char* startp = p;
      for (; *p != 0; ++p) {
	if (*p == '&') {
	  xmlTextWriterWriteRawLen(writer, BAD_CAST startp, p - startp);
	  xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&amp;", 5);
	  startp = p + 1;
	} else if (*p == '<') {
	  xmlTextWriterWriteRawLen(writer, BAD_CAST startp, p - startp);
	  xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&lt;", 4);
	  startp = p + 1;
	} else if (*p == '>') {
	  xmlTextWriterWriteRawLen(writer, BAD_CAST startp, p - startp);
	  xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&gt;", 4);
	  startp = p + 1;
	}
      }

      // write anything left over
      xmlTextWriterWriteRawLen(writer, BAD_CAST startp, p - startp);

    }
    break;

    // catch all, just output the node
  default:
    //    outputNode(*xmlTextReaderCurrentNode(reader), writer);
    break;
  }
}
