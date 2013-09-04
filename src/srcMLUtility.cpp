/*
  srcMLUtility.cpp

  Copyright (C) 2004-2013  SDML (www.sdml.info)

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

#include "srcMLUtility.hpp"
#include <cstring>
#include "srcmlns.hpp"
#include <sys/stat.h>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <sys/errno.h>
#else
#include <direct.h>
#endif

#include <libxml/parserInternals.h>

#include "Options.hpp"

#include "ProcessUnit.hpp"
#include "ExtractUnitsSrc.hpp"
#include "ExtractUnitsDiffSrc.hpp"
#include "CountUnits.hpp"
#include "Properties.hpp"
#include "ListUnits.hpp"
#include "ExtractUnitsXML.hpp"
#include "ExtractUnitsDiffXML.hpp"
#include "ExtractUnitsDiffXMLPreserve.hpp"
#include "XPathQueryUnits.hpp"
#include "XSLTUnits.hpp"
#include "RelaxNGUnits.hpp"

#include "SAX2ExtractUnitsSrc.hpp"

#include "SAX2UnitDOMRelaxNG.hpp"

#include "srcexfun.hpp"

#include <libexslt/exslt.h>
#include "libxml_archive_read.hpp"
#include "libxml_archive_write.hpp"

#if defined(__GNUG__) && !defined(__MINGW32__)
#include <dlfcn.h>
#endif

// local function forward declarations
static xmlParserCtxtPtr srcMLCreateURLParserCtxt(const char * infile);
static xmlParserCtxtPtr srcMLCreateMemoryParserCtxt(const char * buffer, int size);
static void srcMLParseDocument(xmlParserCtxtPtr ctxt, bool allowendearly);
static xmlParserCtxtPtr srcMLCreateParserCtxt(xmlParserInputBufferPtr buffer_input);

static bool incount = false;

// constructor
srcMLUtility::srcMLUtility(const char* infilename, const char* encoding, OPTION_TYPE& op, const char* diff_version)
  : infile(infilename), output_encoding(encoding), options(op), units(0), diff_version(diff_version), buffer(0), size(0), buffer_input(0) {


  // assume totaling for numeric results
  op |= OPTION_XPATH_TOTAL;
  options |= OPTION_XPATH_TOTAL;

  // empty filename indicates standard input
  if (infile == 0)
    infile = "-";
}

// constructor
srcMLUtility::srcMLUtility(const char * buffer, int size, const char* encoding, OPTION_TYPE& op, const char* diff_version)
  : infile(0), output_encoding(encoding), options(op), units(0), diff_version(diff_version), buffer(buffer), size(size), buffer_input(0) {

  // assume totaling for numeric results
  op |= OPTION_XPATH_TOTAL;
  options |= OPTION_XPATH_TOTAL;
}

// constructor
srcMLUtility::srcMLUtility(xmlParserInputBufferPtr buffer_input, const char* encoding, OPTION_TYPE& op, const char* diff_version)
  : infile(0), output_encoding(encoding), options(op), units(0), diff_version(diff_version), buffer(0), size(0), buffer_input(buffer_input) {

  // assume totaling for numeric results
  op |= OPTION_XPATH_TOTAL;
  options |= OPTION_XPATH_TOTAL;
}

// destructor
srcMLUtility::~srcMLUtility() {

}

int srcMLUtility::curunits() const {
  return units;
}

// attribute
const char* srcMLUtility::attribute(const char* attribute_name) {

  const char* pos = find(attrv, attribute_name);

  return pos ? pos : 0;
}

// prefix of given namespace
// return blank for default namespace
const char* srcMLUtility::namespace_ext(const char* uri) {

  const char* pos = find(nsv, uri);

  if (!pos)
    return 0;

  if (strncmp(pos, "xmlns:", 6) == 0)
    return pos + 6;
  else if (strncmp(pos, "xmlns", 5) == 0)
    return pos + 5;
  else
    return pos;
}

// move to a particular nested unit
void srcMLUtility::move_to_unit(int unitnumber, srcMLUtility&su, OPTION_TYPE options, int optioncount, int optionorder[], FILE * output) {

  // setup parser
  xmlParserCtxtPtr ctxt = srcMLCreateURLParserCtxt(infile);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  Properties process(su, nsv, attrv, optioncount, optionorder, output);

  incount = unitnumber == 0;

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unitnumber, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit >= 1 && state.count != state.unit)
    throw OutOfRangeUnitError(state.count);

  units = state.count;
}

// move to a particular nested unit
void srcMLUtility::move_to_unit(int unitnumber, srcMLUtility&su, OPTION_TYPE options, int optioncount, int optionorder[], std::vector<std::string> & output_array) {

  // setup parser
  xmlParserCtxtPtr ctxt = srcMLCreateURLParserCtxt(infile);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  Properties process(su, nsv, attrv, optioncount, optionorder, &output_array);

  incount = unitnumber == 0;

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unitnumber, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit >= 1 && state.count != state.unit)
    throw OutOfRangeUnitError(state.count);

  units = state.count;
}

const char * srcMLUtility::long_info(srcMLUtility & su) {

  int unitnumber = 0;
  int optioncount = 5;
  int optionorder[] = {OPTION_XML_ENCODING, OPTION_LANGUAGE, OPTION_DIRECTORY, OPTION_FILENAME, OPTION_VERSION};
  OPTION_TYPE options = OPTION_LONG_INFO;

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateMemoryParserCtxt(buffer, size);
  if (ctxt == NULL) return 0;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  std::ostringstream buffer;
  Properties process(su, nsv, attrv, optioncount, optionorder, &buffer);

  incount = unitnumber == 0;

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unitnumber, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit >= 1 && state.count != state.unit)
    throw OutOfRangeUnitError(state.count);

  units = state.count;

  static std::string s;
  s = buffer.str();

  return s.c_str();
}

// count of nested units
int srcMLUtility::unit_count(FILE* output) {

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateMemoryParserCtxt(buffer, size);
  if (ctxt == NULL) return -1;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ProcessUnit* pprocess = 0;
  if(output)
    pprocess = isatty(fileno(output)) ? new CountUnits(output) : new ProcessUnit;
  else
    pprocess = new CountUnits(output);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(pprocess, &options, -1, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, false);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  delete pprocess;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  return state.count;
}


// extract a given unit
void srcMLUtility::extract_xml(const char* ofilename, int unit) {

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateMemoryParserCtxt(buffer, size);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsXML process(0 /* to_directory is stdout */, ofilename, output_encoding);

  if (isoption(options, OPTION_NULL))
    unit = -1;

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);
}

// extract a given unit
const char * srcMLUtility::extract_xml(int unit) {

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateMemoryParserCtxt(buffer, size);
  if (ctxt == NULL) return 0;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  xmlBufferPtr buffer = xmlBufferCreate();
  ExtractUnitsXML process(buffer, output_encoding);

  if (isoption(options, OPTION_NULL))
    unit = -1;

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);

  const char * content = strdup((const char *)buffer->content);
  xmlBufferFree(buffer);
  return content;

}

// extract a given unit
void srcMLUtility::extract_diff_xml(const char* ofilename, int unit, const char* version) {

  // setup parser
  xmlParserCtxtPtr ctxt = srcMLCreateURLParserCtxt(infile);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsDiffXML process(0 /* to_directory is stdout */, ofilename, output_encoding, version);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);
}

// extract a given unit
void srcMLUtility::extract_xml_uri(const char* ofilename, int unit, const char* uri) {

  // setup parser
  xmlParserCtxtPtr ctxt = srcMLCreateURLParserCtxt(infile);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsDiffXMLPreserve process(0 /* to_directory is stdout */, ofilename, output_encoding, uri);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);
}

// extract a given unit
void srcMLUtility::extract_text(const char* to_dir, const char* ofilename, int unit) {

#if 0
  if (xmlRegisterOutputCallbacks(archiveWriteMatch_src2srcml, archiveWriteOpen, archiveWrite, archiveWriteClose) < 0) {
    fprintf(stderr, "%s: failed to register archive handler\n", "FOO");
    exit(1);
  }

  if (archiveWriteMatch_src2srcml(ofilename)) {
    archiveWriteOutputFormat(ofilename);

    archiveWriteRootOpen(ofilename);
  }
#endif

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateMemoryParserCtxt(buffer, size);

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsSrc process(0 /* to_directory is stdout */, ofilename, output_encoding);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

#if 0
  if (archiveWriteMatch_src2srcml(ofilename))
    archiveWriteRootClose(0);
#endif

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);
}

// extract a given unit
void srcMLUtility::extract_text(xmlOutputBufferPtr output_buffer, int unit) {

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateMemoryParserCtxt(buffer, size);

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsSrc process(output_buffer);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);
}

// extract a given unit
const char * srcMLUtility::extract_text(int unit) {

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateMemoryParserCtxt(buffer, size);

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  xmlBufferPtr buffer = xmlBufferCreate();
  ExtractUnitsSrc process(buffer, output_encoding);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

#if 0
  if (archiveWriteMatch_src2srcml(ofilename))
    archiveWriteRootClose(0);
#endif

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);

  const char * content = strdup((const char *)buffer->content);
  xmlBufferFree(buffer);
  return content;

}

// extract a given unit
void srcMLUtility::extract_diff_text(const char* to_dir, const char* ofilename, int unit, const char* version) {

  // setup parser
  xmlParserCtxtPtr ctxt = srcMLCreateURLParserCtxt(infile);

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsDiffSrc process(0 /* to_directory is stdout */, ofilename, output_encoding, version);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);
}

// expand the compound srcML to individual files
void srcMLUtility::expand(const char* root_filename, const char* format, const char* to_directory) {

  if (xmlRegisterOutputCallbacks(archiveWriteMatch_srcml2src, archiveWriteOpen, archiveWrite, archiveWriteClose) < 0) {
    fprintf(stderr, "%s: failed to register archive handler\n", "FOO");
    exit(1);
  }

  archiveWriteOutputFormat(format);

  archiveWriteRootOpen(root_filename);

  // setup parser
  xmlParserCtxtPtr ctxt = srcMLCreateURLParserCtxt(infile);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsSrc process(to_directory, root_filename, output_encoding);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, false);

  archiveWriteRootClose(0);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);
}

// list the elements
void srcMLUtility::list() {

  // setup parser
  xmlParserCtxtPtr ctxt = srcMLCreateURLParserCtxt(infile);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ListUnits process;

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, false);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);
}
/*
// namespaces and prefixes
const PROPERTIES_TYPE* srcMLUtility::getNS() const {
return nsv;
}
*/

// set the input filename after the constructor
void srcMLUtility::set_input_filename(const char* infilename) {
  infile = infilename;
}

// xpath evaluation of the nested units
void srcMLUtility::xpath(const char* ofilename, const char* context_element, const char* xpaths[], int fd) {

  // relative xpath changed to at any level
  std::string s = xpaths[0];
  //  if (s[0] != '/')
  //    s = "//" + s;

  // compile the xpath that will be applied to each unit
  xmlXPathCompExprPtr compiled_xpath = xmlXPathCompile(BAD_CAST s.c_str());
  if (compiled_xpath == 0) {
    fprintf(stderr, "srcml2src:  Unable to compile XPath '%s'\n", s.c_str());
    return;
  }

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateParserCtxt(buffer_input);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  XPathQueryUnits process(context_element, ofilename, options, compiled_xpath, fd);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, false);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  if(buffer_input) inputPop(ctxt);
  xmlFreeParserCtxt(ctxt);
}

// allow for all exslt functions
void dlexsltRegisterAll() {

#if defined(__GNUG__) && !defined(__MINGW32__)
  typedef void (*exsltRegisterAll_function)();

  void* handle = dlopen("libexslt.so", RTLD_LAZY);
  if (!handle) {
    handle = dlopen("libexslt.dylib", RTLD_LAZY);
    if (!handle) {
      fprintf(stderr, "Unable to open libexslt library\n");
      return;
    }
  }

  dlerror();
  exsltRegisterAll_function exsltRegisterAll = (exsltRegisterAll_function)dlsym(handle, "exsltRegisterAll");
  char* error;
  if ((error = dlerror()) != NULL) {
    dlclose(handle);
    return;
  }

  // allow for all exslt functions
  exsltRegisterAll();

#endif


#if defined(__GNUG__) && !defined(__MINGW32__)
  dlclose(handle);
#endif
}

// xslt evaluation of the nested units
void srcMLUtility::xslt(const char* context_element, const char* ofilename, const char* xslts[], const char* params[], int paramcount, int fd) {

  xmlMemSetup(xmlMemFree, xmlMemMalloc, xmlMemRealloc, xmlMemoryStrdup);
  xmlInitParser();

  // allow for all exstl functions
  dlexsltRegisterAll();

#if defined(__GNUG__) && !defined(__MINGW32__)
  typedef xsltStylesheetPtr (*xsltParseStylesheetFile_function) (const xmlChar*);

  void* handle = dlopen("libexslt.so", RTLD_LAZY);
  if (!handle) {
    handle = dlopen("libexslt.dylib", RTLD_LAZY);
    if (!handle) {
      fprintf(stderr, "Unable to open libexslt library\n");
      return;
    }
  }

  dlerror();
  xsltParseStylesheetFile_function xsltParseStylesheetFile = (xsltParseStylesheetFile_function)dlsym(handle, "xsltParseStylesheetFile");
  char* error;
  if ((error = dlerror()) != NULL) {
    dlclose(handle);
    return;
  }
#endif

  // parse the stylesheet
  xsltStylesheetPtr stylesheet = xsltParseStylesheetFile(BAD_CAST xslts[0]);
  if (!stylesheet)
    return;

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateParserCtxt(buffer_input);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  XSLTUnits process(context_element, ofilename, options, stylesheet, params, fd);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1, diff_version);
  ctxt->_private = &state;

  xsltsrcMLRegister();

  // process the document
  srcMLParseDocument(ctxt, false);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  if(buffer_input) inputPop(ctxt);
  // all done with parsing
  xmlFreeParserCtxt(ctxt);
}

// relaxng evaluation of the nested units
void srcMLUtility::relaxng(const char* ofilename, const char** xslts, int fd) {

  xmlSAXHandler sax = SAX2UnitDOMRelaxNG::factory();

  xmlRelaxNGParserCtxtPtr relaxng;
  xmlRelaxNGPtr rng;
  xmlRelaxNGValidCtxtPtr rngptr;
  relaxng = xmlRelaxNGNewParserCtxt(xslts[0]);
  rng = xmlRelaxNGParse(relaxng);
  rngptr = xmlRelaxNGNewValidCtxt(rng);
  //SAX2UnitDOMRelaxNG state(0, xslts, ofilename, 0, fd);
  RelaxNGUnits state(ofilename, options, rngptr, fd);

  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateParserCtxt(buffer_input);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->_private = &state;
  //state.ctxt = ctxt;

  // parse the stylesheet
  //state.relaxng = xmlRelaxNGNewParserCtxt(state.fxslt[0]);
  //state.rng = xmlRelaxNGParse(state.relaxng);
  //state.rngptr = xmlRelaxNGNewValidCtxt(state.rng);

  srcMLParseDocument(ctxt, false);

  ctxt->sax = NULL;

  if(buffer_input) inputPop(ctxt);
  xmlFreeParserCtxt(ctxt);
}


// process srcML document with error reporting
static void srcMLParseDocument(xmlParserCtxtPtr ctxt, bool allowendearly) {

  // process the document
  int status;
  if ((status = xmlParseDocument(ctxt)) == -1) {

    xmlErrorPtr ep = xmlCtxtGetLastError(ctxt);

    // special case
    if (allowendearly && (ep->code == XML_ERR_EXTRA_CONTENT || ep->code == XML_ERR_DOCUMENT_END))
      return;

    if (incount)
      fprintf(stderr, "\n");

    // report error
    char* partmsg = strdup(ep->message);
    partmsg[strlen(partmsg) - 1] = '\0';
    fprintf(stderr, "%s: %s in '%s'\n", "srcml2src", partmsg, ep->file);
    exit(STATUS_INPUTFILE_PROBLEM);
  }
}

// create srcml parser with error reporting
static xmlParserCtxtPtr srcMLCreateURLParserCtxt(const char * infile) {

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT | XML_PARSE_HUGE);
  if (ctxt == NULL) {

    // report error
    xmlErrorPtr ep = xmlGetLastError();
    fprintf(stderr, "%s: %s", "srcml2src", ep->message);
    exit(STATUS_INPUTFILE_PROBLEM);
  }

  return ctxt;
}

// create srcml parser with error reporting
static xmlParserCtxtPtr srcMLCreateMemoryParserCtxt(const char * buffer, int size) {

  xmlParserCtxtPtr ctxt = xmlCreateMemoryParserCtxt(buffer, size);

  if (ctxt == NULL) {

    // report error
    xmlErrorPtr ep = xmlGetLastError();
    fprintf(stderr, "%s: %s", "srcml2src", ep->message);
    exit(STATUS_INPUTFILE_PROBLEM);
  }

  return ctxt;
}

#ifdef LIBXML2_NEW_BUFFER 
struct xmlBuf {
  xmlChar *content;           /* The buffer content UTF8 */
  unsigned int compat_use;    /* for binary compatibility */
  unsigned int compat_size;   /* for binary compatibility */
  xmlBufferAllocationScheme alloc; /* The realloc method */
  xmlChar *contentIO;         /* in IO mode we may have a different base */
  size_t use;                 /* The buffer size used */
  size_t size;                /* The buffer size */
  xmlBufferPtr buffer;        /* wrapper for an old buffer */
  int error;                  /* an error code if a failure occured */
};
#define CHECK_COMPAT(buf)                                   \
  if (buf->size != (size_t) buf->compat_size)            \
    if (buf->compat_size < INT_MAX)                    \
      buf->size = buf->compat_size;                  \
  if (buf->use != (size_t) buf->compat_use)              \
    if (buf->compat_use < INT_MAX)                     \
      buf->use = buf->compat_use;

int
xmlBufResetInput(xmlBuf * buf, xmlParserInputPtr input) {
  if ((input == NULL) || (buf == NULL) || (buf->error))
    return(-1);
    CHECK_COMPAT(buf)
    input->base = input->cur = buf->content;
    input->end = &buf->content[buf->use];
    return(0);

}
#else
int
xmlBufResetInput(xmlBuffer * buf, xmlParserInputPtr input) {
  input->base = input->buf->buffer->content;
  input->cur = input->buf->buffer->content;
  input->end = &input->buf->buffer->content[input->buf->buffer->use];
}

#endif

xmlParserCtxtPtr
srcMLCreateParserCtxt(xmlParserInputBufferPtr buffer_input) {
  xmlParserCtxtPtr ctxt;
  xmlParserInputPtr input;
  xmlParserInputBufferPtr buf;

  ctxt = xmlNewParserCtxt();
  if (ctxt == NULL)
    return(NULL);

  buf = buffer_input;
  if (buf == NULL) {
    xmlFreeParserCtxt(ctxt);
    return(NULL);
  }

  input = xmlNewInputStream(ctxt);
  if (input == NULL) {
    xmlFreeParserCtxt(ctxt);
    return(NULL);
  }

  input->filename = NULL;
  input->buf = buf;
  xmlBufResetInput(input->buf->buffer, input);

  inputPush(ctxt, input);
  return(ctxt);
}


extern "C" {

  // constructor
  srcMLUtility * srcml_utility_file_new(const char* infilename, const char* encoding, OPTION_TYPE op, const char* diff_version) {

    return new srcMLUtility(infilename, encoding, op, diff_version);

  }

  srcMLUtility * srcml_utility_memory_new(const char * buffer, int size, const char* encoding, OPTION_TYPE op, const char* diff_version) {

    return new srcMLUtility(buffer, size, encoding, op, diff_version);

  }

  // extract (intact) current unit as text
  void srcml_extract_text_file(srcMLUtility * su, const char* to_dir, const char* ofilename, int unit) {

    su->extract_text(to_dir, ofilename, unit);

  }


  // TODO:  Why does this not return a value?
  const char * srcml_extract_text_buffer(srcMLUtility * su, int unit) {

    su->extract_text(unit);

  }

  // count of nested units
  int srcml_unit_count(srcMLUtility * su, FILE* output) {

    return su->unit_count(output);

  }

  const char * srcml_extract_xml_buffer(srcMLUtility * su, int unit) {

    return su->extract_xml(unit);

  }

  const char * srcml_long_info(srcMLUtility * su) {

    return su->long_info(*su);

  }

  void srcml_utility_delete(srcMLUtility * su) {

    delete su;

  }


}
