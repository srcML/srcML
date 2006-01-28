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

  void process_PI(std::istream& is);

  void process_comment(std::istream& is);
};

xmlTextReaderPtr xmlNewTextReaderFilename(const char* filename) {
  xmlTextReaderPtr reader = new xmlTextReader;

  if (filename[0] == '-' && filename[1] == '\0')
    reader->pin = &std::cin;
  else
    reader->pin = new std::ifstream(filename);

  return reader; 
}

int xmlTextReaderRead(xmlTextReaderPtr reader) {

    char c;
    std::istream& is = *(reader->pin);

    if (is >> c) {

      // processing instruction
      if (c == '<' && is.peek() == '?') {

	process_PI(is);

      // comment
      } else if (c == '<' && is.peek() == '!') {

	process_comment(is);

      // tag
      } else if (c == '<') {

	std::string tagname;
	std::vector<std::pair<std::string, std::string> > m;

	process_tag(is, tagname, m);

      // entity
      } else if (c == '&') {

	process_entity(is);

      // regular text
      } else {

	// add to current text
      }

      return 1;
    } else
      return -1; 
}

int xmlTextReaderNodeType(xmlTextReaderPtr) {
  return 1;
}

int xmlTextReaderReadState(xmlTextReaderPtr) {
  return 1;
}

int xmlTextReaderDepth(xmlTextReaderPtr) {
  return 1; 
}

int xmlTextReaderMoveToNextAttribute(xmlTextReaderPtr) {
  return 0;
}

const xmlChar* xmlTextReaderConstEncoding(xmlTextReaderPtr) {
  return BAD_CAST ""; 
}

xmlChar* xmlTextReaderGetAttribute(xmlTextReaderPtr, const xmlChar*) {
  return BAD_CAST "";
}

const xmlChar* xmlTextReaderConstName(xmlTextReaderPtr) {
  return BAD_CAST "";
}

const xmlChar* xmlTextReaderConstValue(xmlTextReaderPtr) {
  return BAD_CAST "";
}

int xmlTextReaderIsEmptyElement(xmlTextReaderPtr) {
  return false; 
}

void xmlFreeTextReader(xmlTextReaderPtr reader) {

  //  delete reader->pin;

  delete reader;
}

void xmlFree(xmlChar* p) {
  //  delete p;
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

  void process_comment(std::istream& is) {
    char c = '<';

    is.get();
    is.get();
    is.get();

    char last = c;
    c = is.get();
    while (!(last == '-' && c == '-' && is.peek() == '>')) {
      last = c;
      c = is.get();
    }
    is.get();
  }
  /*
  bool processTag(xmlTextReaderPtr reader) {
      std::string tagname;
      std::vector<std::pair<std::string, std::string> > m;

      process_tag(is, tagname, m);

      return true;
  }
  */
  void process_PI(std::istream& is) {
      is.ignore(1000, '>');
      is.get();
  }

  void processEntity() {
    //      if (!entity(process_entity(is)))
	return;
  }
}
