/*
  srcMLUtility.cpp

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

  Class for straightforward translation
*/

#include "srcMLUtility.h"
#include <iostream>
#include <fstream>
#include "srcmlns.h"
#include <sys/stat.h>
#include <sys/errno.h>

#include "Options.h"

// directory permission for expand
const int EXPAND_DIR_PERM = S_IRWXU | S_IRWXG;

const char* XML_DECLARATION_STANDALONE = "yes";
const char* XML_VERSION = "1.0";

// check if encoding is supported
bool srcMLUtility::checkEncoding(const char* encoding) {

  return xmlFindCharEncodingHandler(encoding) != 0;

}

xmlChar* unit_filename = 0;
xmlChar* unit_directory = 0;
xmlChar* unit_version = 0;
xmlChar* unit_language = 0;

void skiptonextunit(xmlTextReaderPtr reader) throw (LibXMLError);
void skiptounit(xmlTextReaderPtr reader, const char* filename) throw (LibXMLError);
void skiptounit(xmlTextReaderPtr reader, int number) throw (LibXMLError);

// constructor
srcMLUtility::srcMLUtility(const char* infilename, const char* enc, int op)
  : infile(infilename), encoding(enc), options(op), reader(0), handler(0) {

  // setup an output handler
  handler = xmlFindCharEncodingHandler(enc);

  // empty filename indicates standard input
  if (infile == 0)
    infile = "-";

  // create the reader
  reader = xmlNewTextReaderFilename(infile);
  if (reader == NULL)
    throw "Unable to open file as XML";

  // read the outer unit
  try {
    skiptounit(reader, 1);
  } catch (...) {
    throw "Unable to find starting unit element";
  }

  // record the current attributes for use in subunits
  unit_filename = xmlTextReaderGetAttribute(reader, BAD_CAST "filename");
  unit_directory = xmlTextReaderGetAttribute(reader, BAD_CAST "dir");
  unit_version = xmlTextReaderGetAttribute(reader, BAD_CAST "version");
  unit_language = xmlTextReaderGetAttribute(reader, BAD_CAST "language");
}

// destructor
srcMLUtility::~srcMLUtility() {

  // free reader
  xmlFreeTextReader(reader);
}

// translate from input stream to output stream
void srcMLUtility::translate(const char* ofilename) {

  // output entire unit element as text
  outputSrc(ofilename, reader);
}


// translate from input stream to output stream
void srcMLUtility::translate_xml(const char* ofilename) {
  // Set the encoding to that of the outer, root unit element
  encoding = (const char*) xmlTextReaderConstEncoding(reader);

  while (xmlTextReaderReadState(reader) == 1) {

    // output entire unit element
    outputUnit(ofilename, reader);

    // output separator
    //    xmlTextWriterWriteRawLen(writer, BAD_CAST "\n\n", 2);
  }
}

// attribute
std::string srcMLUtility::attribute(const char* attribute_name) {

  // extract attribute from unit tag
  xmlChar* value = xmlTextReaderGetAttribute(reader, BAD_CAST attribute_name);

  // return the extracted attribute or a blank string if it doesn't exist
  std::string s(value != 0 ? (const char*)value : "");

  xmlFree(value);

  return s;
}

// attribute
std::string srcMLUtility::unit_attribute(int unitnumber, const char* attribute_name) {

  // skip to the proper nested unit
  skiptounit(reader, unitnumber);

  // extract attribute from unit tag
  return attribute(attribute_name);
}

// count of nested units
int srcMLUtility::unit_count() {

  // process all nodes counting units
  int count = 0;
  while (1) {

    // read a node
    int ret = xmlTextReaderRead(reader);
    if (ret != 1)
      break;

    if (xmlTextReaderDepth(reader) == 1 && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
	&& strcmp((const char*) xmlTextReaderConstName(reader), "unit") == 0)
      ++count;
  }

  return count;
}

// extract a given unit
void srcMLUtility::extract_xml(const char* ofilename, int unitnumber) {

  // Set the encoding to that of the outer, root unit element
  encoding = (const char*) xmlTextReaderConstEncoding(reader);

  // skip to the proper nested unit
  skiptounit(reader, unitnumber);

  // output entire unit element
  outputUnit(ofilename, reader);
}

// extract a given unit specified by filename
void srcMLUtility::extract_xml(const char* ofilename, const char* filename) {

  // Set the encoding to that of the outer, root unit element
  encoding = (const char*) xmlTextReaderConstEncoding(reader);

  // skip to the proper nested unit
  skiptounit(reader, filename);

  // output entire unit element
  outputUnit(ofilename, reader);
}

// extract a given unit
void srcMLUtility::extract_text(const char* ofilename, int unitnumber) {

  // skip to the proper nested unit
  skiptounit(reader, unitnumber);

  // output entire unit element as text
  outputSrc(ofilename, reader);
}

int mkpath(const char* path, mode_t mode) {
 
  const std::string spath = path;

  unsigned pos = 0;
  while ((pos = spath.find('/', pos + 1)) != std::string::npos) {

    // make the directory path so far
    if (spath.substr(0, pos).c_str() != ".") {

      int ret = mkdir(spath.substr(0, pos).c_str(), mode);
      if (ret != 0 && errno != EEXIST)
	return ret;
    }

    // move beyond this mark
    ++pos;
  }

  // make the directory path if there is one
  return mkdir(spath.c_str(), mode);
}

// expand the compound srcML to individual files
void srcMLUtility::expand(const char* root_filename) {

  // process each nested unit element individually
  while (1) {

    // skip to the proper nested unit
    skiptounit(reader, 1);

    // extract the attributes from the unit for filename and directory
    xmlChar* filename = xmlTextReaderGetAttribute(reader, BAD_CAST "filename");
    xmlChar* directory = xmlTextReaderGetAttribute(reader, BAD_CAST "dir");

    if (!filename) {
      std::cout << "Missing filename" << '\n';
      continue;
    }

    // construct the directory if needed
    std::string directory_filename = root_filename;
    if (directory) {
      directory_filename += (const char*) directory;

      // make the directory path if there is one
      int ret = mkpath(directory_filename.c_str(), EXPAND_DIR_PERM);
      if (ret != 0 && errno != EEXIST) {
	std::cout << "Error " << errno  << " creating directory:  " << directory_filename << '\n';
      }
    }

    // filename is based on directory
    std::string output_filename = directory_filename;
    if (output_filename != "")
      output_filename += "/";
    output_filename += (const char*) filename;

    // output file status message if in verbose mode
    if (isoption(options, OPTION_VERBOSE))
      std::cerr << "Creating file:  " << output_filename << '\n';

    // output this particular unit
    outputSrc(output_filename.c_str(), reader);

    xmlFree(filename);
    xmlFree(directory);
  }
}

const char* srcMLUtility::getencoding() {

  return (const char*) xmlTextReaderConstEncoding(reader);
}

// output current unit element in XML
void srcMLUtility::outputUnit(const char* filename, xmlTextReaderPtr reader) {

  // open the output text writer stream
  // "-" filename is standard output
  xmlTextWriterPtr writer = xmlNewTextWriterFilename(filename, isoption(options, OPTION_COMPRESSED));

  // issue the xml declaration
  xmlTextWriterStartDocument(writer, XML_VERSION, encoding, XML_DECLARATION_STANDALONE);

  // output main unit tag
  xmlTextWriterStartElement(writer, BAD_CAST "unit");

  // output src namespace
  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST SRCML_SRC_NS_URI);

  // output cpp namespace
  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:cpp", BAD_CAST SRCML_CPP_NS_URI);

  // output debugging namespace
  if (isoption(options, OPTION_DEBUG))
      xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:srcerr", BAD_CAST SRCML_ERR_NS_URI);

  xmlChar* attribute = 0;
  attribute = xmlTextReaderGetAttribute(reader, BAD_CAST "filename");
  if (attribute)
    unit_filename = attribute;

  attribute = xmlTextReaderGetAttribute(reader, BAD_CAST "dir");
  if (attribute)
    unit_directory = attribute;

  attribute = xmlTextReaderGetAttribute(reader, BAD_CAST "version");
  if (attribute)
    unit_version = attribute;

  attribute = xmlTextReaderGetAttribute(reader, BAD_CAST "language");
  if (attribute)
    unit_language = attribute;
  /*
  // copy all attributes from current unit (may be main unit)
  while (xmlTextReaderMoveToNextAttribute(reader)) {
    xmlTextWriterWriteAttribute(writer, xmlTextReaderConstName(reader), xmlTextReaderConstValue(reader));

    if (strcmp((char*) xmlTextReaderConstName(reader), "language") == 0)
      language_flag = true;

    if (strcmp((char*) xmlTextReaderConstName(reader), "version") == 0)
      version_flag = true;
  }
  */
  if (unit_language) {
    xmlTextWriterWriteAttribute(writer, BAD_CAST "language", unit_language);
    xmlFree(unit_language);
  }

  if (unit_directory) {
    xmlTextWriterWriteAttribute(writer, BAD_CAST "dir", unit_directory);
    xmlFree(unit_directory);
  }

  if (unit_filename) {
    xmlTextWriterWriteAttribute(writer, BAD_CAST "filename", unit_filename);
    xmlFree(unit_filename);
  }

  if (unit_version) {
    xmlTextWriterWriteAttribute(writer, BAD_CAST "version", unit_version);
    xmlFree(unit_version);
  }

  
  // process the nodes in this unit
  while (1) {

    // read a node
    int ret = xmlTextReaderRead(reader);
    if (ret != 1)
      return;

    // output text and all elements
    outputXML(reader, writer);

    // found end element of this unit
    if (xmlTextReaderDepth(reader) == 1 && xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT)
      break;
  }

  xmlTextWriterEndDocument(writer);

  xmlFreeTextWriter(writer);
}

// output current unit element as text
void srcMLUtility::outputSrc(const char* ofilename, xmlTextReaderPtr reader) {

  // open the output text writer stream
  // "-" filename is standard output
  xmlTextWriterPtr writer = xmlNewTextWriterFilename(ofilename, isoption(options, OPTION_COMPRESSED));

  bool first = true;
  // process the nodes in this unit
  while (1) {

    // read a node
    int ret = xmlTextReaderRead(reader);
    if (ret != 1)
      break;

    // output text
    switch (xmlTextReaderNodeType(reader)) {

      // special case for formfeed element
    case XML_READER_TYPE_ELEMENT:

      if (first && strcmp((const char*) xmlTextReaderConstName(reader), "unit") == 0)
	throw TranslateCompoundError();
      first = false;

      if (strcmp((const char*) xmlTextReaderConstName(reader), "formfeed") == 0)
	    xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "\f", 1);
      break;
    case XML_READER_TYPE_TEXT:
    case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
      outputText(xmlTextReaderConstValue(reader), writer, false);
      //    xmlTextWriterWriteString(writer, xmlTextReaderConstValue(reader));
      break;
    default:
      break;
    };

    // found end element of this unit
    if (xmlTextReaderDepth(reader) == 1 && xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT
	&& strcmp((const char*) xmlTextReaderConstName(reader), "unit") == 0)
      break;
  }

  // Problem with call to xmlTextWriterEndDocument.  When indentation is 0 it wants to write out a final newline.
  // Setting the indentation will only affect the end
  xmlTextWriterSetIndent(writer, 1);

  xmlTextWriterEndDocument(writer);

  xmlFreeTextWriter(writer);
}

// output text in proper format
void srcMLUtility::outputText(const xmlChar* s, xmlTextWriterPtr writer, bool escape) {

    // buffer of output utf8 characters
    const int UTF8BUFFER_MAXSIZE = 512;
    static unsigned char utf8buffer[UTF8BUFFER_MAXSIZE];

    // amount of space for expanded characters.  assume two bytes for every original one byte
    const int UTF8BUFFER_SPACE = UTF8BUFFER_MAXSIZE / 2;

    // extract c string from c++ string
    const xmlChar* inputbuffer = s;
    int inputbuffer_size = strlen((const char*) s);

    // write out all of input buffer converted to utf8
    int pos = 0;
    while (pos < inputbuffer_size) {

      int utf8buffer_size = UTF8BUFFER_MAXSIZE;
      int buffer_left = inputbuffer_size - pos;
      int partialinputbuffer_size = buffer_left > UTF8BUFFER_SPACE ? UTF8BUFFER_SPACE : buffer_left;
      int utf8buffer_newsize = handler->output(utf8buffer, &utf8buffer_size,
					       (const unsigned char*) (inputbuffer + pos), &partialinputbuffer_size);
      utf8buffer[utf8buffer_newsize] = 0;

      if (escape) {
      for (unsigned char* p = utf8buffer; *p != 0; ++p) {
	  if (*p == '&')
	    xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&amp;", 5);
	  else if (*p == '<')
	    xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&lt;", 4);
	  else if (*p == '>')
	    xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&gt;", 4);
	  else
	    xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) p, 1);
      }
      } else {
	xmlTextWriterWriteRawLen(writer, BAD_CAST utf8buffer, utf8buffer_newsize);
      }

      pos += partialinputbuffer_size;
    }
}

  // output current XML node in reader
  void srcMLUtility::outputXML(xmlTextReaderPtr reader, xmlTextWriterPtr writer) {

    bool isemptyelement = false;

    switch (xmlTextReaderNodeType(reader)) {
    case XML_READER_TYPE_ELEMENT:

      // record if this is an empty element since it will be erased by the attribute copying
      isemptyelement = xmlTextReaderIsEmptyElement(reader);

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

    case XML_READER_TYPE_TEXT:
    case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
      outputText(xmlTextReaderConstValue(reader), writer);
      break;

    default:
      break;
    }
  }

  // skip to the next unit
  void skiptonextunit(xmlTextReaderPtr reader) throw (LibXMLError) {

    // skip to the correct unit
    while (1) {
      // read a node
      int ret = xmlTextReaderRead(reader);
      if (ret != 1)
	throw LibXMLError(ret);

      if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
	  strcmp((const char*) xmlTextReaderConstName(reader), "unit") == 0) {
	break;
      }
    }
  }

  // skip to a particular unit
  void skiptounit(xmlTextReaderPtr reader, int number) throw (LibXMLError) {

    // skip to the correct unit
    int count = 0;
    while (1) {
      skiptonextunit(reader);

      ++count;

      // did we find it?
      if (count == number)
	break;
    }
  }

  // skip to a particular unit
  void skiptounit(xmlTextReaderPtr reader, const char* filename) throw (LibXMLError) {

    // skip to the correct unit
    while (1) {
      skiptonextunit(reader);

      // did we find it?
      if (strcmp((const char*) xmlTextReaderGetAttribute(reader, BAD_CAST "filename"), filename) == 0)
	break;
    }
  }
