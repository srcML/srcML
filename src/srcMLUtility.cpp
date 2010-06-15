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
#include <libxml/xmlsave.h>
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

#include "SAX2ExtractUnitsSrc.h"
#include "SAX2ExtractUnitSrc.h"
#include "SAX2ExtractRootSrc.h"
#include "SAX2ExtractUnitXML.h"
#include "SAX2CountUnits.h"
#include "SAX2Properties.h"

#include "SAX2UnitDOMXPath.h"
#include "SAX2UnitDOMXSLT.h"
#include "SAX2UnitDOMRelaxNG.h"

// constructor
srcMLUtility::srcMLUtility(const char* infilename, const char* encoding, int& op)
  : infile(infilename), output_encoding(encoding), options(op), units(0) {

  // empty filename indicates standard input
  if (infile == 0)
    infile = "-";
}

// destructor
srcMLUtility::~srcMLUtility() {

}

int srcMLUtility::curunits() const {
  return units;
}

// attribute
const char* srcMLUtility::attribute(const char* attribute_name) {

  // extract attribute from unit tag
  PROPERTIES_TYPE::const_iterator pos = find(attrv, attribute_name);

  return pos != attrv.end() ? pos->second.c_str() : 0;
}

// prefix of given namespace
// return blank for default namespace
const char* srcMLUtility::namespace_ext(const char* uri) {

  PROPERTIES_TYPE::const_iterator pos = find(nsv, uri);

  return pos != nsv.end() ? pos->second.substr(pos->second.size() > 5 ? 6 : 5).c_str() : 0;
}

// move to a particular nested unit
void srcMLUtility::move_to_unit(int unitnumber) {

  // output entire unit element
  xmlSAXHandler sax = SAX2Properties::factory();

  SAX2Properties state(unitnumber, options, nsv, attrv);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  // encoding is entered as a property
  const char* encoding = (const char*) (state.ctxt->encoding ? state.ctxt->encoding : state.ctxt->input->encoding);
  if (encoding)
    attrv.insert(attrv.end(), PROPERTIES_TYPE::value_type(".encoding", encoding));

  // don't let the context free the static sax handler
  ctxt->sax = NULL;

  // now free the context
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count != state.unit)
     throw OutOfRangeUnitError(state.count);

  units = state.count;
}


// count of nested units
int srcMLUtility::unit_count() {

  // output entire unit element
  xmlSAXHandler sax = SAX2CountUnits::factory();

  SAX2CountUnits state(0, options);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return -1;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);

  return state.count;
}

// extract a given unit
void srcMLUtility::extract_xml(const char* filename, int unit) {

  // output entire unit element
  xmlSAXHandler sax = SAX2ExtractUnitXML::factory();

  SAX2ExtractUnitXML state(filename, unit, options, nsv, attrv);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count != state.unit)
    throw OutOfRangeUnitError(state.count);
}

// extract a given unit
void srcMLUtility::extract_element(const char* element, const char* filename) {
  /*
  xmlTextReaderPtr reader = xmlNewTextReaderFilename(infile);

  const char* paths[] = { NULL };

  // perform xpath evaluation
  //  srceval(element, paths, "", "", reader, filename);

  xmlFreeTextReader(reader);
  */
  /*
  int unit;

  // output entire unit element
  xmlSAXHandler sax = SAX2CopyElement::factory();

  SAX2CopyElement state(element, filename, unit, options);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
  */
}

// extract a given unit
void srcMLUtility::extract_text(const char* ofilename, int unit) {

  xmlSAXHandler sax = unit == 0 ? SAX2ExtractRootSrc::factory() : SAX2ExtractUnitSrc::factory();

  SAX2ExtractRootSrc::State state;
  state.ofilename = ofilename;
  state.poptions = &options;
  state.handler = xmlFindCharEncodingHandler(output_encoding);
  state.unit = unit;

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count != state.unit)
    throw OutOfRangeUnitError(state.count);
}

// expand the compound srcML to individual files
void srcMLUtility::expand(const char* root_filename) {

  SAX2ExtractUnitsSrc::State state;
  state.root_filename = root_filename;
  state.poptions = &options;
  state.handler = xmlFindCharEncodingHandler(output_encoding);
  state.whole_path = (char*) malloc(0);
  state.unit = -1;

  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  free(state.whole_path);

  xmlFreeParserCtxt(ctxt);
}

// namespaces and prefixes
const PROPERTIES_TYPE& srcMLUtility::getNS() const {
  return nsv;
}

// xpath evaluation of the nested units
void srcMLUtility::xpath(const char* ofilename, const char* context_element, const char* xpaths[]) {

  xmlSAXHandler sax = SAX2UnitDOMXPath::factory();
  
  SAX2UnitDOMXPath state(context_element, xpaths, ofilename, 0, 0, options);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->_private = &state;
  //state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
}

// xslt evaluation of the nested units
void srcMLUtility::xslt(const char* context_element, const char* ofilename, const char* xslts[], const char* params[], int paramcount) {

  xmlSAXHandler sax = SAX2UnitDOMXSLT::factory();
  
  SAX2UnitDOMXSLT state(context_element, xslts, ofilename, params, paramcount, options);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->_private = &state;
  //state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
}

// relaxng evaluation of the nested units
void srcMLUtility::relaxng(const char* ofilename, const char** xslts) {

  xmlSAXHandler sax = SAX2UnitDOMXSLT::factory();
  
  SAX2UnitDOMRelaxNG state(0, xslts, ofilename, 0, 0, 0);

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->_private = &state;
  //state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
}

