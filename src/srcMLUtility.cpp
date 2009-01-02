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
#include <xmlsave.h>
#include "srcmlns.h"
#include <libxml/parser.h>
#include <libxml/parserInternals.h>

#ifdef __GNUC__
#include <sys/stat.h>
#include <sys/errno.h>
#else
#include <direct.h>
#endif

#include "Options.h"

const char* const XML_DECLARATION_STANDALONE = "yes";
const char* const XML_VERSION = "1.0";

/* srcML unit attributes */
const char* const UNIT_ATTRIBUTE_LANGUAGE = "language";
const char* const UNIT_ATTRIBUTE_DIRECTORY = "dir";
const char* const UNIT_ATTRIBUTE_FILENAME = "filename";
const char* const UNIT_ATTRIBUTE_VERSION = "version";

// check if encoding is supported
bool srcMLUtility::checkEncoding(const char* encoding) {

  return xmlFindCharEncodingHandler(encoding) != 0;

}

#include "SAX2ExtractUnitsSrc.h"
#include "SAX2ExtractUnitSrc.h"
#include "SAX2ExtractRootSrc.h"
#include "SAX2ExtractUnitXML.h"
#include "SAX2Properties.h"

// constructor
srcMLUtility::srcMLUtility(const char* infilename, const char* encoding, int& op)
  : infile(infilename), output_encoding(encoding), options(op), reader(0), handler(0), moved(false) {

  // empty filename indicates standard input
  if (infile == 0)
    infile = "-";

  // setup an output handler
  handler = xmlFindCharEncodingHandler(output_encoding);
}

// destructor
srcMLUtility::~srcMLUtility() {

  // free reader
  xmlFreeTextReader(reader);
}

// attribute
std::string srcMLUtility::attribute(const char* attribute_name, bool& nonnull) {

  // extract attribute from unit tag
  nonnull = attrv.count(attribute_name) > 0;

  return nonnull ? attrv[attribute_name] : "";
}

// prefix of given namespace
// return blank for default namespace
std::string srcMLUtility::namespace_ext(const std::string& uri, bool& nonnull) {

  // find the raw prefix
  nonnull = nsv.count(uri) > 0;

  return nonnull ? nsv[uri] : "";
}

// move to a particular nested unit
void srcMLUtility::move_to_unit(int unitnumber) {

  moved = true;

  // Set the encoding to that of the outer, root unit element
  //  output_encoding = (const char*) xmlTextReaderConstEncoding(reader);

  // output entire unit element
  xmlSAXHandler sax = SAX2Properties::factory();

  SAX2Properties::State state;
  state.ofilename = infile;
  state.filename = infile;
  state.poptions = &options;
  state.unit = unitnumber;

  xmlParserCtxtPtr ctxt = xmlCreateFileParserCtxt(infile);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  encoding = (const char*) state.ctxt->encoding;

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);

  nsv = state.nsv;
  attrv = state.attrv;
}


// count of nested units
int srcMLUtility::unit_count() {
  /*
  // process all nodes counting units
  int count = 0;
  while (1) {

    try {
      skiptonextunit(reader);
    } catch(LibXMLError) {
      break;
    }

    // make sure first nested element is unit
    if (count == 0 && strcmp((const char*) xmlTextReaderConstLocalName(reader), "unit") != 0)
      break;

    // found another unit
    ++count;

    // skip past this unit node
    xmlTextReaderNext(reader);

    // stop after this file (and end gracefully) with ctrl-c
    if (isoption(options, OPTION_TERMINATE))
      break;
  }

  return count;
  */
  return 0;
}

// extract a given unit
void srcMLUtility::extract_xml(const char* ofilename, int unit) {

  // Set the encoding to that of the outer, root unit element
  output_encoding = (const char*) xmlTextReaderConstEncoding(reader);

  // output entire unit element
  xmlSAXHandler sax = SAX2ExtractUnitXML::factory();

  SAX2ExtractUnitXML::State state;
  state.ofilename = ofilename;
  state.filename = ofilename;
  state.poptions = &options;
  state.handler = handler;
  state.unit = unit;

  xmlParserCtxtPtr ctxt = xmlCreateFileParserCtxt(infile);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
}

// extract a given unit
void srcMLUtility::extract_text(const char* ofilename, int unit) {

  xmlSAXHandler sax = unit == 0 ? SAX2ExtractRootSrc::factory() : SAX2ExtractUnitSrc::factory();

  SAX2ExtractRootSrc::State state;
  state.ofilename = ofilename;
  state.poptions = &options;
  state.handler = handler;
  state.unit = unit;

  xmlParserCtxtPtr ctxt = xmlCreateFileParserCtxt(infile);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
}

// expand the compound srcML to individual files
void srcMLUtility::expand(const char* root_filename) {

  SAX2ExtractUnitsSrc::State state;
  state.root_filename = root_filename;
  state.poptions = &options;
  state.unit = -1;

  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();

  xmlParserCtxtPtr ctxt = xmlCreateFileParserCtxt(infile);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
}

const char* srcMLUtility::getencoding() {

  return encoding.c_str();
}

// namespaces and prefixes
const std::map<std::string, std::string> srcMLUtility::getNS() const {
  return nsv;
}
