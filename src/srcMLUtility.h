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

class XMLProcess {
 public:
  virtual void process() = 0;
};

class XSLTProcess : public XMLProcess {

};

class srcMLUtility {
 public:

  // constructor
  srcMLUtility(const char* infilename, const char* encoding, OPTION_TYPE& op);

  // set the input filename after the constructor
  void set_input_filename(const char* infilename);

  // attribute
  const char* attribute(const char* attribute_name);

  // namespace
  const char* namespace_ext(const char* uri);

  // move to a particular nested unit
  //  void move_to_unit(int unitnumber);
  void move_to_unit(int unitnumber, srcMLUtility&su, OPTION_TYPE options, int optioncount, int optionorder[]);

  // namespaces and prefixes
  const PROPERTIES_TYPE* getNS() const;

  // count of nested units
  int unit_count();

  // list the elements
  void list();

  // extract (intact) current unit as XML
  void extract_xml(const char* ofilename, int unit = 0);

  // extract (intact) current unit as text
  void extract_text(const char* to_dir, const char* ofilename, int unit = 0);

  // extract (intact) specific elements
  void extract_element(const char* filename, const char* element);

  // expand the compound srcML to individual files
  void expand(const char* root_filename = "", const char* output_format = 0,
	      const char* to_directory = "");

  // perform xpath evaluation
  void xpath(const char* context_element, const char* ofilename, const char* xpaths[]);

  // perform xslt evaluation
  void xslt(const char* context_element, const char* ofilename, const char* xslts[], const char* params[], int paramcount);

  // perform relaxng evaluation
  void relaxng(const char* ofilename, const char** xslts);

  static bool checkEncoding(const char* encoding) {

    return xmlFindCharEncodingHandler(encoding) != 0;
  }

  int curunits() const;

  // destructor
  ~srcMLUtility();

 private:
  const char* infile;
  const char* output_encoding;
  OPTION_TYPE& options;
 public:
  PROPERTIES_TYPE nsv;;
  PROPERTIES_TYPE attrv;
 private:
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
