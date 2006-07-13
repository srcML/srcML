/*
  xmlrw.cpp

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

  A set of convenience functions for libxml2
*/

#ifndef __INCLUDED_XMLRW_CPP
#define __INCLUDED_XMLRW_CPP

#include "xmlrw.h"
#include <iostream>

bool operator==(const xmlNode& n1, const xmlNode& n2) {

  return n1.type == n2.type && 
    (strcmp((char*) n1.name, (char*) n2.name) == 0) && (
    (n1.type != XML_READER_TYPE_TEXT && n1.type != XML_READER_TYPE_SIGNIFICANT_WHITESPACE) ||
    (strcmp((char*) n1.content, (char*) n2.content) == 0)
    );
}

xmlNode* getRealCurrentNode(xmlTextReaderPtr reader) {

  xmlNode* pnode = getCurrentNode(reader);

  pnode->extra = xmlTextReaderIsEmptyElement(reader);

  return pnode;
}

xmlNode* getCurrentNode(xmlTextReaderPtr reader) {

  xmlNode* node = xmlCopyNode(xmlTextReaderCurrentNode(reader), 1);

  node->type = (xmlElementType) xmlTextReaderNodeType(reader);

  return node;
}

void eat_element(xmlTextReaderPtr& reader) {
  int depth = xmlTextReaderDepth(reader);
  xmlTextReaderRead(reader);
  while (xmlTextReaderDepth(reader) > depth)
    xmlTextReaderRead(reader);
  xmlTextReaderRead(reader);
}

  // output current XML node in reader
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
	xmlTextWriterWriteRawLen(writer, s, strlen((const char*) s));
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

    default:
      break;
    }
  }

  // output current XML node in reader
  void outputXML(xmlTextReaderPtr reader, xmlTextWriterPtr writer, const char* name) {

    bool isemptyelement = false;

    switch (xmlTextReaderNodeType(reader)) {
    case XML_READER_TYPE_ELEMENT:

      // record if this is an empty element since it will be erased by the attribute copying
      isemptyelement = xmlTextReaderIsEmptyElement(reader) > 0;

      // start the element
      xmlTextWriterStartElement(writer, BAD_CAST name);

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

    case XML_READER_TYPE_TEXT:
    case XML_READER_TYPE_WHITESPACE:
    case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
      {

      // output the UTF-8 buffer escaping the characters.  Note that the output encoding
      // is handled by libxml
      unsigned char* p = (unsigned char*) xmlTextReaderConstValue(reader);
      unsigned char* startp = p;
      for (; *p != 0; ++p) {
	if (*p == '&') {
	  xmlTextWriterWriteRawLen(writer, BAD_CAST startp, p - startp);
	  xmlTextWriterWriteRawLen(writer, BAD_CAST "&amp;", 5);
	  startp = p + 1;
	} else if (*p == '<') {
	  xmlTextWriterWriteRawLen(writer, BAD_CAST startp, p - startp);
	  xmlTextWriterWriteRawLen(writer, BAD_CAST "&lt;", 4);
	  startp = p + 1;
	} else if (*p == '>') {
	  xmlTextWriterWriteRawLen(writer, BAD_CAST startp, p - startp);
	  xmlTextWriterWriteRawLen(writer, BAD_CAST "&gt;", 4);
	  startp = p + 1;
	}
      }

      xmlTextWriterWriteRawLen(writer, BAD_CAST startp, p - startp);

      }
      break;

    default:
      break;
    }
  }

  // output current XML node in reader
  void outputNode(xmlNode& node, xmlTextWriterPtr writer) {

    bool isemptyelement = false;

    switch (node.type) {
    case XML_READER_TYPE_ELEMENT:

      // record if this is an empty element since it will be erased by the attribute copying
      isemptyelement = node.extra & 0x1;

      // start the element
      if (node.ns && node.ns->prefix) {

	std::string s = ((char*) node.ns->prefix);
	s += ":";
	s += (char*) node.name;

	xmlTextWriterStartElement(writer, BAD_CAST s.c_str());

      } else

	xmlTextWriterStartElement(writer, node.name);

      if (xmlHasProp(&node, BAD_CAST "type"))
	xmlTextWriterWriteAttribute(writer, BAD_CAST "type", xmlGetProp(&node, BAD_CAST "type"));
      /*
      // copy all the attributes
      while (xmlTextReaderMoveToNextAttribute(reader)) {
	xmlTextWriterWriteAttribute(writer, node.name, name.content);
      }
      */
      // end now if this is an empty element
      if (isemptyelement) {
	xmlTextWriterEndElement(writer);
      }

      break;

    case XML_READER_TYPE_END_ELEMENT:
      xmlTextWriterEndElement(writer);
      break;

    case XML_READER_TYPE_COMMENT:
      xmlTextWriterWriteComment(writer, node.content);
      break;

    case XML_READER_TYPE_TEXT:
    case XML_READER_TYPE_WHITESPACE:
    case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:

      // output the UTF-8 buffer escaping the characters.  Note that the output encoding
      // is handled by libxml
      for (unsigned char* p = (unsigned char*) node.content; *p != 0; ++p) {
	  if (*p == '&')
	    xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&amp;", 5);
	  else if (*p == '<')
	    xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&lt;", 4);
	  else if (*p == '>')
	    xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&gt;", 4);
	  else
	    xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) p, 1);
      }
      break;

    default:
      break;
    }
  }

#endif
