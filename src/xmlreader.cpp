/*
  xmlreader.cpp

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

#include "xmlreader.h"
#include <iostream>
#include <fstream>
#include <vector>

namespace {

  // push the character on the back of the string
  // needed because some implementations do not have this method
  void push_back(std::string& s, const char& c) {
    s.resize(s.size() + 1);
    s[s.size() - 1] = c;
  }

void skipws(std::istream&in) {
  while (pstd::isspace(in.peek()))
    in.get();
}

  int process_tag(std::istream& in, std::string& tagname, std::vector<std::pair<std::string, std::string> >& m);

  void read_attribute(std::istream& in, std::string& attrname, std::string& attrvalue);

  std::string process_entity(std::istream& in);

  int process_pi(std::istream& is, std::vector<std::pair<std::string, std::string> >& m);

  void process_comment(std::istream& is, std::string& content);
};

xmlTextReaderPtr xmlNewTextReaderFilename(const char* filename) {
  xmlTextReaderPtr reader = new xmlTextReader;

  if (filename[0] == '-' && filename[1] == '\0')
    reader->pin = &std::cin;
  else
    reader->pin = new std::ifstream(filename);

  if (reader->pin->fail())
    return 0;

  reader->pin->unsetf(std::ios::skipws);

  reader->depth = 0;
  reader->encoding = "UTF-8";

  return reader; 
}

int xmlTextReaderRead(xmlTextReaderPtr reader) {

    char c;
    std::istream& is = *(reader->pin);

    bool hastext = false;
    if (reader->incrementnext) {
      reader->incrementnext = false;
      reader->depth += 1;
    }

    bool firsttext = true;
    while (is >> c) {

      // processing instruction
      if (c == '<' && is.peek() == '?') {

	if (hastext) {
	  reader->type = XML_READER_TYPE_TEXT;
	  is.putback(c);
	  return 1;
	}

	std::vector<std::pair<std::string, std::string> > m;
	process_pi(is, m);

	for (unsigned int i = 0; i < m.size(); ++i) {
	  if (m[i].first == "encoding") {
	    reader->encoding = m[i].second.c_str();
	  }
	}

	reader->value = "";

	reader->state = 1;
	return 1;

      // comment
      } else if (c == '<' && is.peek() == '!') {

	if (hastext) {
	  reader->type = XML_READER_TYPE_TEXT;
	  is.putback(c);
	  return 1;
	}

	reader->type = XML_READER_TYPE_COMMENT;
	process_comment(is, reader->value);

	reader->state = 1;
	return 1;

	break;

      // tag
      } else if (c == '<') {

	if (hastext) {
	  reader->type = XML_READER_TYPE_TEXT;
	  is.putback(c);
	  return 1;
	}

	reader->attribute_count = -1;
	reader->inattribute = false;
	switch (process_tag(is, reader->tagname, reader->attributes)) {
	case 0:
	  reader->type = XML_READER_TYPE_ELEMENT;
	  reader->isempty = false;
	  reader->incrementnext = true;
	  break;

	case 1:
	  reader->type = XML_READER_TYPE_END_ELEMENT;
	  reader->depth -= 1;
	  break;

	case 2:
	  reader->type = XML_READER_TYPE_ELEMENT;
	  reader->isempty = true;
	  break;
	}

	reader->value = "";

	reader->state = 1;
	return 1;

	break;

      // entity
      } else if (c == '&') {

	if (firsttext) {
	  reader->value = "";
	  firsttext = false;
	}

	reader->value += process_entity(is);
	reader->type = XML_READER_TYPE_TEXT;
	reader->tagname = "#text";
	hastext = true;

      // regular text
      } else {

	if (firsttext) {
	  reader->value = "";
	  firsttext = false;
	}

	// add to current text
	reader->value += c;

	reader->type = XML_READER_TYPE_TEXT;
	reader->tagname = "#text";
	hastext = true;

      }
    }

    reader->state = 0;
    return -1; 
}

// nop in this mode
void xmlTextReaderNext(xmlTextReaderPtr reader) {
}

int xmlTextReaderNodeType(xmlTextReaderPtr reader) {
  return reader->type;
}

int xmlTextReaderReadState(xmlTextReaderPtr reader) {
  return reader->state;
}

int xmlTextReaderDepth(xmlTextReaderPtr reader) {
  return reader->depth; 
}

int xmlTextReaderMoveToNextAttribute(xmlTextReaderPtr reader) {

  reader->attribute_count += 1;

  reader->inattribute = (unsigned int) reader->attribute_count < reader->attributes.size();

  return reader->inattribute;
}

const xmlChar* xmlTextReaderConstEncoding(xmlTextReaderPtr reader) {
  return BAD_CAST reader->encoding.c_str(); 
}

xmlChar* xmlTextReaderGetAttribute(xmlTextReaderPtr reader, const xmlChar* attr_name) {

  for (unsigned int i = 0; i < reader->attributes.size(); ++i) {
      if (reader->attributes[i].first == attr_name) {
	const char* s = reader->attributes[i].second.c_str();
	char* ns = new char[strlen(s) + 1];
	std::strcpy(ns, s);
	return BAD_CAST ns;
      }
  }

  return 0;
}

const xmlChar* xmlTextReaderConstName(xmlTextReaderPtr reader) {
  if (reader->inattribute)
    return BAD_CAST reader->attributes[reader->attribute_count].first.c_str();
  else
    return BAD_CAST reader->tagname.c_str();
}

const xmlChar* xmlTextReaderConstValue(xmlTextReaderPtr reader) {
  if (reader->inattribute)
    return BAD_CAST reader->attributes[reader->attribute_count].second.c_str();
  else
    return BAD_CAST reader->value.c_str();
}

int xmlTextReaderIsEmptyElement(xmlTextReaderPtr reader) {
  return reader->isempty; 
}

void xmlFreeTextReader(xmlTextReaderPtr reader) {

  // delete the input file object, unless it is standard input
  if (reader->pin != &std::cin)
    delete reader->pin;

  delete reader;
}

void xmlFree(xmlChar* p) {
  delete p;
}

namespace {

int process_tag(std::istream& in, std::string& tagname, std::vector<std::pair<std::string, std::string> >& m) {

  // assume start tag
  int tagtype = 0;

  // skip whitespace
  skipws(in);

  // check for endtag
  if (in.peek() == '/') {
    tagtype = 1;
    in.get();
  }

  // skip whitespace
  skipws(in);

  // read in tag name
  tagname = "";
  while (!pstd::isspace(in.peek()) && in.peek() != '/' && in.peek() != '>')
    push_back(tagname, in.get());

  // skip whitespace
  skipws(in);

  // process any attributes
  m.clear();
  while (in.peek() != '>' && in.peek() != '/') {
    std::string attrname;
    std::string attrvalue;
    read_attribute(in, attrname, attrvalue);

    m.push_back(std::pair<std::string, std::string>(attrname, attrvalue));
  }

  // skip until end of tag
  while (in.peek() != '>' && in.peek() != '/')
    in.get();

  // check for empty element
  if (in.peek() == '/') {
    tagtype = 2;

    // skip '/'
    in.get();
  }
    
  // skip end of tag, i.e., '>'
  in.get();

  // result is the type of the tag
  return tagtype;
}

void read_attribute(std::istream& in, std::string& attrname, std::string& attrvalue) {

  // skip whitespace
  skipws(in);

  // read in attribute name
  while (!pstd::isspace(in.peek()) && in.peek() != '=' && in.peek() != '>')
    push_back(attrname, in.get());

  // skip whitespace
  skipws(in);

  // read in attribute value (if there is one)
  if (in.peek() == '=') {

    // skip '='
    in.get();

    // skip whitespace
    skipws(in);

    // skip string delimiter
    in.get();

    // read in attribute value
    while (in.peek() != '\"')
      push_back(attrvalue, in.get());

    // skip string delimiter
    in.get();
  }
}

std::string process_entity(std::istream& in) {

  std::string ename;
  ename.reserve(3);
  while (in.peek() != ';')
    push_back(ename, in.get());
  in.get();

  if (ename == "amp")
    return "&";
  
  else if (ename == "lt")
    return "<";

  else if (ename == "gt")
    return ">";

  else
    return "&" + ename + ";";
}

  void process_comment(std::istream& is, std::string& content) {
    char c = '<';

    is.get();
    is.get();
    is.get();

    char last = c;
    content = "";
    c = is.get();
    while (!(last == '-' && c == '-' && is.peek() == '>')) {
      push_back(content, c);
      last = c;
      c = is.get();
    }

    // remove the last two characters
    content = content.substr(0, content.size() - 1);

    // read last '>'
    is.get();
  }

int process_pi(std::istream& in, std::vector<std::pair<std::string, std::string> >& m) {

  // assume start tag
  int tagtype = 0;

  // skip whitespace
  skipws(in);

  // read '?'
  in.get();

  // skip whitespace
  skipws(in);

  // read in tag name
  std::string tagname = "";
  while (!pstd::isspace(in.peek()) && in.peek() != '/' && in.peek() != '>')
    push_back(tagname, in.get());

  // skip whitespace
  skipws(in);

  // process any attributes
  m.clear();
  while (in.peek() != '>' && in.peek() != '/') {
    std::string attrname;
    std::string attrvalue;
    read_attribute(in, attrname, attrvalue);

    m.push_back(std::pair<std::string, std::string>(attrname, attrvalue));
  }

  // skip whitespace
  skipws(in);

  // skip end of tag, i.e., '?'
  in.get();

  // skip end of tag, i.e., '>'
  in.get();

  // result is the type of the tag
  return tagtype;
}
}
