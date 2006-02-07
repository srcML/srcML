/*
  srcMLUtility.h

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

  Class for extracting basic information from srcML documents, including equivalent source code.
*/

#ifndef SRCMLUTILITY_H
#define SRCMLUTILITY_H

#include <iostream>
#include <string>
#include <vector>
#include "Language.h"
#include "Options.h"

#ifdef LIBXML_ENABLED
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#else
#include "xmlreader.h"
#include "xmlwriter.h"
#endif

class TranslateCompoundError {};

class LibXMLError {
 public:
  LibXMLError(int errnum)
    : error(errnum) {}

  int getErrorNum() const { return error; }

 private:
  int error;
};

class srcMLUtility {
 public:

  // constructor
  srcMLUtility(const char* infilename, const char* encoding, int op = 0);

  // translate from input stream to output stream as text
  virtual void translate(const char* ofilename);

  int process_tag(std::istream& in, std::string& tagname, std::vector<std::pair<std::string, std::string> >& m);

  // attribute
  std::string attribute(const char* attribute_name);

  // attribute of a nested unit
  std::string unit_attribute(int unitnumber, const char* attribute_name);

  // move to a particular nested unit
  void move_to_unit(int unitnumber);

  // encoding
  const char* getencoding();

  // count of nested units
  int unit_count();

  // extract (intact) a given unit
  virtual void extract_xml(const char* ofilename, int);

  // extract (intact) a given unit specified by filename
  virtual void extract_xml(const char* ofilename, const char*);

  // extract (intact) a given unit
  virtual void extract_text(const char* ofilename, int);

  // expand the compound srcML to individual files
  virtual void expand(const char* root_filename = "");

  static bool checkEncoding(const char*);

  // destructor
  virtual ~srcMLUtility();

 protected:

  // output current unit element in XML
  void outputUnit(const char* filename, xmlTextReaderPtr reader);

  // output current unit element as text
  void outputSrc(const char* ofilename, xmlTextReaderPtr reader);

  // output text in proper format
  void outputText(const xmlChar* s, bool escape = false);

  // output current XML node in reader
  void outputXML(xmlTextReaderPtr reader, xmlTextWriterPtr writer);

  // output current text-only XML node in reader
  void outputText(xmlTextReaderPtr reader);

  // output current text-only XML node in reader
  void outputText(const xmlChar* s, std::ostream& out);

 private:
  const char* infile;
  const char* encoding;
  int options;
  xmlTextReaderPtr reader;
  xmlCharEncodingHandlerPtr handler;
}; 

#endif
