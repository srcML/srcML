/*
  srcMLUtility.hpp

  Copyright (C) 2004-2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Class for extracting basic information from srcML documents, including equivalent source code.
*/

#ifndef SRCMLUTILITY_HPP
#define SRCMLUTILITY_HPP

#include <Language.hpp>
#include <Options.hpp>

#include <SAX2Utilities.hpp>

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
    srcMLUtility(const char* infilename, const char* encoding, OPTION_TYPE& op, const char* diff_version = "");
    srcMLUtility(const char * buffer, int size, const char* encoding, OPTION_TYPE& op, const char* diff_version = "");
    srcMLUtility(xmlParserInputBufferPtr buffer_input, const char* encoding, OPTION_TYPE& op, const char* diff_version = "");

    // set the input filename after the constructor
    void set_input_filename(const char* infilename);

    // attribute
    const char* attribute(const char* attribute_name);

    // namespace
    const char* namespace_ext(const char* uri);

    // move to a particular nested unit
    //  void move_to_unit(int unitnumber);
    void move_to_unit(int unitnumber, srcMLUtility&su, OPTION_TYPE options, int optioncount, int optionorder[], FILE * output);
    void move_to_unit(int unitnumber, srcMLUtility&su, OPTION_TYPE options, int optioncount, int optionorder[], std::vector<std::string> & output_array);
    const char * long_info(srcMLUtility & su);

    // namespaces and prefixes
    const PROPERTIES_TYPE* getNS() const;

    // count of nested units
    int unit_count(FILE* output);

    // list the elements
    void list();

    // extract (intact) current unit as XML
    void extract_xml(const char* ofilename, int unit = 0);

    // extract (intact) current unit as text
    void extract_text(const char* to_dir, const char* ofilename, int unit = 0);
    void extract_text(xmlOutputBufferPtr output_buffer, int unit = 0);

    // extract a particular srcML version from srcDiff format
    void extract_diff_xml(const char* ofilename, int unit, const char* version);

    // extract (intact) current unit as XML only preserving the URI
    void extract_xml_uri(const char* ofilename, int unit, const char* uri);

    // extract a particular version from srcDiff format
    void extract_diff_text(const char* to_dir, const char* ofilename, int unit, const char* version);

    // expand the compound srcML to individual files
    void expand(const char* root_filename = "", const char* output_format = 0,
                const char* to_directory = "");

    // perform xpath evaluation
  void xpath(const char* context_element, const char* ofilename, const char* xpaths[], int fd = 0);

    // perform xslt evaluation
  void xslt(const char* context_element, const char* ofilename, const char* xslts[], const char* params[], int paramcount, int fd = 0);

    // perform relaxng evaluation
  void relaxng(const char* ofilename, const char** xslts, int fd = 0);

    static bool checkEncoding(const char* encoding) {

        return xmlFindCharEncodingHandler(encoding) != 0;
    }

    int curunits() const;

    // destructor
    ~srcMLUtility();

private:
    const char* infile;
    const char* output_encoding;
    OPTION_TYPE options;
    long units;
    const char* diff_version;
    const char * buffer;
    int size;
    xmlParserInputBufferPtr buffer_input;
public:
    PROPERTIES_TYPE nsv;
    PROPERTIES_TYPE attrv;
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
    OutOfRangeUnitError(long s) : LibXMLError(0), size(s) {}

    long size;
};

#endif
