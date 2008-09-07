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
#include <cstring>
#include "xmloutput.h"

#ifdef __GNUC__
#include <sys/stat.h>
#include <sys/errno.h>
#else
#include <direct.h>
#endif

#include "Options.h"

// directory permission for expand
#ifdef __GNUC__
const int EXPAND_DIR_PERM = S_IRWXU | S_IRWXG;
#endif

const char* XML_DECLARATION_STANDALONE = "yes";
const char* XML_VERSION = "1.0";

/* srcML unit attributes */
const char* UNIT_ATTRIBUTE_LANGUAGE = "language";
const char* UNIT_ATTRIBUTE_DIRECTORY = "dir";
const char* UNIT_ATTRIBUTE_FILENAME = "filename";
const char* UNIT_ATTRIBUTE_VERSION = "version";

// check if encoding is supported
bool srcMLUtility::checkEncoding(const char* encoding) {

  return xmlFindCharEncodingHandler(encoding) != 0;

}

void skiptonextunit(xmlTextReaderPtr reader) throw (LibXMLError);
void skiptounit(xmlTextReaderPtr reader, const char* filename) throw (LibXMLError);
void skiptounit(xmlTextReaderPtr reader, int number) throw (LibXMLError);

// constructor
srcMLUtility::srcMLUtility(const char* infilename, const char* encoding, int& op)
  : infile(infilename), output_encoding(encoding), options(op), reader(0), handler(0), nsv(0), attrv(0),
    unit_language(0), unit_directory(0), unit_filename(0), unit_version(0), moved(false) {

  // empty filename indicates standard input
  if (infile == 0)
    infile = "-";

  // create the reader
  reader = xmlNewTextReaderFilename(infile);
  if (reader == NULL) {
    throw LibXMLError(0);
  }

  // read the outer unit
  try {
    skiptounit(reader, 1);
  } catch (...) {
    throw "Unable to find starting unit element";
  }

  // record the current attributes for use in subunits
  unit_filename = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_FILENAME);
  unit_directory = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_DIRECTORY);
  unit_version = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_VERSION);
  unit_language = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_LANGUAGE);

  // record all attributes for future use
  while (xmlTextReaderMoveToNextAttribute(reader)) {
    if (xmlTextReaderIsNamespaceDecl(reader))
      nsv.push_back(std::make_pair((const char*) xmlTextReaderConstValue(reader), (const char*) xmlTextReaderConstName(reader)));
    else
      attrv.push_back(std::make_pair((const char*) xmlTextReaderConstValue(reader), (const char*) xmlTextReaderConstName(reader)));
  }
}

// destructor
srcMLUtility::~srcMLUtility() {

  // free reader
  xmlFreeTextReader(reader);
}

// attribute
std::string srcMLUtility::attribute(const char* attribute_name, bool& nonnull) {

  if (moved) {
    // extract attribute from unit tag
    xmlChar* value = xmlTextReaderGetAttribute(reader, BAD_CAST attribute_name);
 
    // return the extracted attribute or a blank string if it doesn't exist
    std::string s(value != 0 ? (const char*)value : "");
 
    nonnull = value != 0;

    return s;
  }

  // extract attribute from unit tag
  int pos = -1;
  for (unsigned int i = 0 ; i < attrv.size(); ++i)
    if (attrv[i].second == attribute_name) {
      pos = i;
      break;
    }

  if (pos < 0 || attrv[pos].second != attribute_name) {
    nonnull = false;
    return "";
  }

  nonnull = true;
  return attrv[pos].first;
}

// prefix of given namespace
// return blank for default namespace
// return blank
std::string srcMLUtility::namespace_ext(const std::string& uri, bool& nonnull) {

  // find the raw prefix
  std::string raw_prefix;
  for (std::vector<std::pair<std::string, std::string> >::const_iterator iter = nsv.begin(); iter != nsv.end(); iter++) {
	std::string vuri = (*iter).first;
	std::string vprefix = (*iter).second;

	if (vuri == uri) {
	  raw_prefix = vprefix;
	  break;
	}
  }

  // calculate the real prefix
  std::string prefix;

  if (raw_prefix.size() == 0) {
    nonnull = false;
    prefix = "";
  } else if (raw_prefix.size() == 5) {
    nonnull = true;
    prefix = "";
  } else {
    nonnull = true;
    prefix = raw_prefix.substr(6);
  }

  return prefix;
}

// move to a particular nested unit
void srcMLUtility::move_to_unit(int unitnumber) {

  moved = true;

  // skip to the proper nested unit
  skiptounit(reader, unitnumber);
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

    // process unit
    if (xmlTextReaderDepth(reader) == 1 && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)) {

      // make sure first nested element is unit
      if (count == 0 && strcmp((const char*) xmlTextReaderConstName(reader), "unit") != 0)
         break;
      
      // found another unit
      ++count;

      // skip past this unit
      xmlTextReaderNext(reader);

      // stop after this file (and end gracefully) with ctrl-c
      if (isoption(options, OPTION_TERMINATE))
	break;
    }
  }

  return count;
}

// extract a given unit
void srcMLUtility::extract_xml(const char* ofilename) {

  // Set the encoding to that of the outer, root unit element
  output_encoding = (const char*) xmlTextReaderConstEncoding(reader);

  // output entire unit element
  outputUnit(ofilename, reader);
}

// extract a given unit
void srcMLUtility::extract_text(const char* ofilename) {

  // output entire unit element as text
  outputSrc(ofilename, reader);
}

int mkpath(const char* path
#ifdef __GNUC__		   
		   , mode_t mode
#endif		   
		   ) {
 
  const std::string spath = path;

  int pos = 0;
#ifdef __GNUC__
  while ((unsigned int) (pos = spath.find('/', pos + 1)) != std::string::npos) {
#else
  while ((pos = (int) spath.find('/', pos + 1)) != std::string::npos) {
#endif

    // make the directory path so far
    if (strcmp(spath.substr(0, pos).c_str(), ".") != 0) {

#ifdef __GNUC__
      int ret = mkdir(spath.substr(0, pos).c_str(), mode);
#else
      int ret = mkdir(spath.substr(0, pos).c_str());
#endif
      if (ret != 0 && errno != EEXIST)
	return ret;
    }

    // move beyond this mark
    ++pos;
  }

  // make the directory path if there is one
#ifdef __GNUC__
  return mkdir(spath.c_str(), mode);
#else
  return mkdir(spath.c_str());
#endif
}

// expand the compound srcML to individual files
void srcMLUtility::expand(const char* root_filename) {

  moved = true;

  // process each nested unit element individually
  while (1) {

    // skip to the proper nested unit
    skiptounit(reader, 1);

    // extract the attributes from the unit for filename and directory
    xmlChar* filename = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_FILENAME);
    xmlChar* directory = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_DIRECTORY);

    if (!filename) {
      std::cerr << "Missing filename" << '\n';
      continue;
    }

    // construct the directory if needed
    std::string directory_filename = root_filename;
    if (directory) {
      directory_filename += (const char*) directory;

      // make the directory path if there is one
#ifdef __GNUC__
      int ret = mkpath(directory_filename.c_str(), EXPAND_DIR_PERM);
#else
      int ret = mkpath(directory_filename.c_str());
#endif
	  if (ret != 0 && errno != EEXIST) {
	std::cerr << "Error " << errno  << " creating directory:  " << directory_filename << '\n';
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

    // stop after this file (and end gracefully) with ctrl-c
    if (isoption(options, OPTION_TERMINATE))
      throw TerminateLibXMLError();
  }
}

const char* srcMLUtility::getencoding() {

  return (const char*) xmlTextReaderConstEncoding(reader);
}

// namespaces and prefixes
const std::vector<std::pair<std::string, std::string> > srcMLUtility::getNS() const {
  return nsv;
}

// output current unit element in XML
void srcMLUtility::outputUnit(const char* filename, xmlTextReaderPtr reader) {

  // open the output text writer stream
  // "-" filename is standard output
  xmlTextWriterPtr writer = xmlNewTextWriterFilename(filename, isoption(options, OPTION_COMPRESSED));

  // issue the xml declaration
  if (isoption(options, OPTION_XMLDECL))
    xmlTextWriterStartDocument(writer, XML_VERSION, output_encoding, XML_DECLARATION_STANDALONE);

  // output main unit tag
  xmlTextWriterStartElement(writer, BAD_CAST "unit");

  // output namespaces from outer unit tag
  if (isoption(options, OPTION_NAMESPACEDECL)) {
    for (std::vector<std::pair<std::string, std::string> >::const_iterator iter = nsv.begin(); iter != nsv.end(); iter++) {
      std::string uri = (*iter).first;
      std::string prefix = (*iter).second;

      xmlTextWriterWriteAttribute(writer, BAD_CAST prefix.c_str(), BAD_CAST uri.c_str());
    }
  }

  // output src namespace
  //  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST SRCML_SRC_NS_URI);

  // output cpp namespace
  //  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:cpp", BAD_CAST SRCML_CPP_NS_URI);

  // output debugging namespace
  //  if (isoption(options, OPTION_DEBUG))
  //      xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:srcerr", BAD_CAST SRCML_ERR_NS_URI);

  // update attributes from root unit element with attributes from this nested unit
  xmlChar* attribute = 0;

  // updated attribute language
  attribute = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_LANGUAGE);
  if (attribute)
    unit_language = attribute;

  if (unit_language)
    xmlTextWriterWriteAttribute(writer, BAD_CAST UNIT_ATTRIBUTE_LANGUAGE, unit_language);

  if (attribute)
    xmlFree(attribute);

  // updated attribute directory
  attribute = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_DIRECTORY);
  if (attribute)
    unit_directory = attribute;

  if (unit_directory)
    xmlTextWriterWriteAttribute(writer, BAD_CAST UNIT_ATTRIBUTE_DIRECTORY, unit_directory);

  if (attribute)
    xmlFree(attribute);

  // updated attribute filename
  attribute = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_FILENAME);

  if (attribute)
    unit_filename = attribute;

  if (unit_filename) {
    xmlTextWriterWriteAttribute(writer, BAD_CAST UNIT_ATTRIBUTE_FILENAME, unit_filename);
  }

  if (attribute)
    xmlFree(attribute);

  // updated attribute version
  attribute = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_VERSION);
  if (attribute)
    unit_version = attribute;

  if (unit_version)
    xmlTextWriterWriteAttribute(writer, BAD_CAST UNIT_ATTRIBUTE_VERSION, unit_version);

  if (attribute)
    xmlFree(attribute);

  // copy all other attributes from current unit (may be main unit)
  while (xmlTextReaderMoveToNextAttribute(reader)) {

    const char* name = (const char*) xmlTextReaderConstName(reader);

    // skip standard attributes since they are already output
    if ((strcmp(name, UNIT_ATTRIBUTE_LANGUAGE) == 0) ||
	(strcmp(name, UNIT_ATTRIBUTE_DIRECTORY) == 0) ||
	(strcmp(name, UNIT_ATTRIBUTE_FILENAME) == 0) ||
	(strcmp(name, UNIT_ATTRIBUTE_VERSION) == 0))
      continue;

    // output current attribute
    xmlTextWriterWriteAttribute(writer, BAD_CAST name, xmlTextReaderConstValue(reader));
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

  // setup an output handler
  handler = xmlFindCharEncodingHandler(output_encoding);

#ifdef LIBXML_ENABLED
  // no need for encoding change
  if (strcmp(handler->name, "UTF-8") == 0)
    options |= OPTION_SKIP_ENCODING;
#endif

  // starting depth tells us when to end
  int startingDepth = moved ? 1 : 0;
  
  // point to standard input or open file
  std::ostream* pout = &std::cout;
  if (!(ofilename[0] == '-' && ofilename[1] == 0)) {
    pout = new std::ofstream(ofilename);
  }

  bool first = true;
  // process the nodes in this unit
  while (1) {

    // read a node
    int ret = xmlTextReaderRead(reader);
    if (ret != 1)
      break;

    // output text
    switch (xmlTextReaderNodeType(reader)) {

    case XML_READER_TYPE_ELEMENT:

      // check that we really have a nested unit
      if (first && strcmp((const char*) xmlTextReaderConstName(reader), "unit") == 0)
	throw TranslateCompoundError();
      first = false;

      // special case for formfeed element
      if (xmlTextReaderIsEmptyElement(reader) > 0 &&
	  strcmp((const char*) xmlTextReaderConstName(reader), "formfeed") == 0)
      	outputText(BAD_CAST "\f", *pout);
      break;
    case XML_READER_TYPE_TEXT:
    case XML_READER_TYPE_WHITESPACE:
    case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
      outputText(xmlTextReaderConstValue(reader), *pout);
      break;
    default:
      break;
    };

    // stop when it is the same depth as the start
    if (xmlTextReaderDepth(reader) == startingDepth)
      break;
  }

  // delete ofstream if not standard input
  if (!(ofilename[0] == '-' && ofilename[1] == 0))
    delete pout;
}

#ifdef LIBXML_ENABLED
// buffer of output utf8 characters
const int UTF8BUFFER_MAXSIZE = 4;

xmlBufferPtr poutbuffer = xmlBufferCreateSize(UTF8BUFFER_MAXSIZE);

// amount of space for expanded characters.  assume a maximum of four bytes for every original single byte
const int UTF8BUFFER_SPACE = UTF8BUFFER_MAXSIZE / 4;
#endif

// output text in proper format
void srcMLUtility::outputText(const xmlChar* s, std::ostream& out) {

  // no encoding needed for conversion from UTF-8
#ifdef LIBXML_ENABLED
  if (isoption(options, OPTION_SKIP_ENCODING)) {
#endif
    out << s;
    return;
#ifdef LIBXML_ENABLED
  }

  unsigned int len = strlen((const char*) s);

  // input buffer created from C++ string
  xmlBufferPtr pinbuffer = xmlBufferCreateStatic((char*) s, len);

  // convert all of the UTF-8 to output encoding in chunks
  unsigned int pos = 0;
  while (pos < len) {

    // reset resusable output buffer
    poutbuffer->use = 0;

    int buffer_left = pinbuffer->size - pos;
    int partialinputbuffer_size = buffer_left < UTF8BUFFER_SPACE ? buffer_left : UTF8BUFFER_SPACE;

    pinbuffer->content += pos;
    pinbuffer->size -= pos;

    xmlCharEncOutFunc(handler, poutbuffer, pinbuffer);
    
    out.write((char*) poutbuffer->content, poutbuffer->use);

    pos += partialinputbuffer_size;
  }
#endif
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
	  xmlTextReaderDepth(reader) <= 1)
	break;
    }
  }

  // skip to a particular unit
  void skiptounit(xmlTextReaderPtr reader, int number) throw (LibXMLError) {

    // skip to the correct unit
    int count = 0;
    try {
    while (1) {
      skiptonextunit(reader);

      ++count;

      // did we find it?
      if (count == number)
	break;

      // skip past this unit
      xmlTextReaderNext(reader);
    }
    } catch (...) {
      throw OutOfRangeUnitError(count);
    }
  }

  // skip to a particular unit
  void skiptounit(xmlTextReaderPtr reader, const char* filename) throw (LibXMLError) {

    // skip to the correct unit
    while (1) {
      skiptonextunit(reader);

      // did we find it?
      if (strcmp((const char*)
		 xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_FILENAME), filename) == 0)
	break;

      // skip past this unit
      xmlTextReaderNext(reader);
    }
  }
