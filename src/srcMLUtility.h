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

#include "Language.h"
#include "Options.h"

#include "SAX2Utilities.h"

class LibXMLError;
class TerminateLibXMLError;
class OutOfRangeUnitError;

class srcMLUtility {
 public:

  // constructor
  srcMLUtility(const char* infilename, const char* encoding, int& op);

  // attribute
  const char* attribute(const char* attribute_name);

  // namespace
  const char* namespace_ext(const char* uri);

  // move to a particular nested unit
  void move_to_unit(int unitnumber);

  // namespaces and prefixes
  const PROPERTIES_TYPE& getNS() const;

  // count of nested units
  int unit_count();

  // extract (intact) current unit as XML
  void extract_xml(const char* ofilename, int unit = 0);

  // extract (intact) current unit as text
  void extract_text(const char* ofilename, int unit = 0);

  // expand the compound srcML to individual files
  void expand(const char* root_filename = "");

  // perform xpath evaluation
  void xpath(const char* ofilename, const char* xpaths);

  // perform xslt evaluation
  void xslt(const char* ofilename, const char* xslts);

  // perform relaxng evaluation
  void relaxng(const char* ofilename, const char* xslts);

  static bool checkEncoding(const char* encoding) {

    return xmlFindCharEncodingHandler(encoding) != 0;
  }

  int curunits() const;

  // destructor
  ~srcMLUtility();

 private:
  const char* infile;
  const char* output_encoding;
  int& options;
  PROPERTIES_TYPE nsv;
  PROPERTIES_TYPE attrv;
  int units;
}; 

class LibXMLError {
 public:
  LibXMLError(int errnum)
    : error(errnum) {}

  int getErrorNum() const { return error; }

 private:
  int error;
};

class TerminateLibXMLError : public LibXMLError {
 public:
  TerminateLibXMLError() : LibXMLError(0) {}
};

class OutOfRangeUnitError : public LibXMLError {
 public:
  OutOfRangeUnitError(int s) : LibXMLError(0), size(s) {}

  int size;
};

#endif
