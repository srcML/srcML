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
#include <xpath.h>
#include <xpathInternals.h>

#ifdef __GNUC__
#include <sys/stat.h>
#include <sys/errno.h>
#else
#include <direct.h>
#endif

#include "Options.h"

static const char* SRCML_SRC_NS_PREFIX = "src";

xmlXPathCompExprPtr srcMLUtility::xpath_formfeed;
xmlXPathCompExprPtr srcMLUtility::xpath_escape;

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
  : infile(infilename), output_encoding(encoding), options(op), reader(0), handler(0), context(0), moved(false) {

  // empty filename indicates standard input
  if (infile == 0)
    infile = "-";

  // create the reader
  reader = xmlNewTextReaderFilename(infile);
  if (reader == NULL)
    throw LibXMLError(0);

  // read the outer unit
  try {
    skiptounit(reader, 1);
  } catch (...) {
    throw "Unable to find starting unit element";
  }

  // record all attributes for future use
  // don't use the TextReaderMoveToNextAttribute as it messes up the future expand
  for (xmlAttrPtr pAttr = xmlTextReaderCurrentNode(reader)->properties; pAttr; pAttr = pAttr->next)
      attrv[(const char*) pAttr->name] = (const char*) xmlGetProp(xmlTextReaderCurrentNode(reader), pAttr->name);

  // record all namespaces for future use
  // don't use the TextReaderMoveToNextAttribute as it messes up the future expand
  for (xmlNsPtr pAttr = xmlTextReaderCurrentNode(reader)->nsDef; pAttr; pAttr = pAttr->next)
    nsv[(const char*) pAttr->href] = pAttr->prefix ? (const char*) pAttr->prefix : "";

  // setupt a context for xpath if conversion to src
  if (!isoption(options, OPTION_XML)) {

    xpath_formfeed = xmlXPathCompile(BAD_CAST "//src:formfeed");
    if (!xpath_formfeed)
      throw LibXMLError(0);

    xpath_escape = xmlXPathCompile(BAD_CAST "//src:escape");
    if (!xpath_escape)
      throw LibXMLError(0);

    context = xmlXPathNewContext(xmlTextReaderCurrentDoc(reader));
    if (!context)
      throw LibXMLError(0);

    if (xmlXPathRegisterNs(context, BAD_CAST SRCML_SRC_NS_PREFIX , BAD_CAST SRCML_SRC_NS_URI) == -1)
      throw "Unable to register srcML namespace";
  }
}

// destructor
srcMLUtility::~srcMLUtility() {

  // free xpath context (if it exists)
  if (context)
    xmlXPathFreeContext(context);

  if (xpath_formfeed)
    xmlXPathFreeCompExpr(xpath_formfeed);

  if (xpath_escape)
    xmlXPathFreeCompExpr(xpath_escape);

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

    xmlFree(value);

    return s;
  }

  // extract attribute from unit tag
  if (attrv.count(attribute_name)) {
    nonnull = true;
    return attrv[attribute_name];
  } else {
    nonnull = false;
    return "";
  }
}

// prefix of given namespace
// return blank for default namespace
std::string srcMLUtility::namespace_ext(const std::string& uri, bool& nonnull) {

  // find the raw prefix
  std::string raw_prefix;
  if (nsv.count(uri)) {
    nonnull = true;
    return nsv[uri];
  } else {
    nonnull = false;
    return "";
  }
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
      std::cerr << "Missing filename attribute" << '\n';
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
const std::map<std::string, std::string> srcMLUtility::getNS() const {
  return nsv;
}

// output current unit element in XML
void srcMLUtility::outputUnit(const char* filename, xmlTextReaderPtr reader) {

  // new document
  xmlDocPtr doc;
  xmlTextWriterPtr writer = xmlNewTextWriterDoc(&doc, isoption(options, OPTION_COMPRESSED));

  // issue the xml declaration
  int save_options = XML_SAVE_NO_DECL;
  if (isoption(options, OPTION_XMLDECL)) {
      xmlTextWriterStartDocument(writer, XML_VERSION, output_encoding, XML_DECLARATION_STANDALONE);
      save_options = 0;
  }

  // end the document so we get the correct xml declaration
  xmlTextWriterEndDocument(writer);

  // generate the full tree in the reader of the unit
  // so that we can move it to the output
  xmlTextReaderExpand(reader);

  // set the expanded reader as the root element of the new document
  xmlDocSetRootElement(doc, xmlTextReaderCurrentNode(reader));

  // output namespaces from outer unit tag
  if (isoption(options, OPTION_NAMESPACEDECL)) {

    // starting with the namespaces on the root element, update the namespaces
    // with the ones from this unit
    std::map<std::string, std::string> nnsv = nsv;
    for (xmlNsPtr pAttr = xmlTextReaderCurrentNode(reader)->nsDef; pAttr; pAttr = pAttr->next)
      nnsv[(const char*) pAttr->href] = pAttr->prefix ? (const char*) pAttr->prefix : "";

    // output the standard namespaces, if they exist
    const char* stdns[] = { SRCML_SRC_NS_URI, SRCML_CPP_NS_URI, SRCML_ERR_NS_URI };
    for (int i = 0; i < 3; ++i) {

      if (nnsv.count(stdns[i])) {

	xmlNewNs(xmlDocGetRootElement(doc), BAD_CAST stdns[i],
		 nnsv[stdns[i]] == "" ? NULL : BAD_CAST nnsv[stdns[i]].c_str());

	nnsv.erase(stdns[i]);
      }
    }

    // output any other namespaces that may exist
    for (std::map<std::string, std::string>::const_iterator iter = nsv.begin(); iter != nsv.end(); ++iter) {
      std::string uri = (*iter).first;
      std::string prefix = (*iter).second;

      xmlNewNs(xmlDocGetRootElement(doc), BAD_CAST uri.c_str(), prefix == "" ? NULL : BAD_CAST prefix.c_str());
    }
  }

  // starting with the attributes on the root element, update the attributes
  // with the ones from this unit
  std::map<std::string, std::string> nattrv = attrv;
  for (xmlAttrPtr pAttr = xmlDocGetRootElement(doc)->properties; pAttr; pAttr = pAttr->next)
    nattrv[(const char*) pAttr->name] = (const char*) xmlGetProp(xmlDocGetRootElement(doc), pAttr->name);

  // wipe out all the attributes
  xmlAttrPtr pAttr;
  while ((pAttr = xmlDocGetRootElement(doc)->properties)) {
    xmlDocGetRootElement(doc)->properties = pAttr->next;
    xmlRemoveProp(pAttr);
  }

  // put back the standard attributes based on a merge of the root unit and this unit
  const char* stdattr[] = { UNIT_ATTRIBUTE_LANGUAGE, UNIT_ATTRIBUTE_DIRECTORY,
			    UNIT_ATTRIBUTE_FILENAME, UNIT_ATTRIBUTE_VERSION };
  for (int i = 0; i < 4; ++i) {

    if (nattrv.count(stdattr[i])) {
      xmlSetProp(xmlDocGetRootElement(doc), BAD_CAST stdattr[i],
		 BAD_CAST nattrv[stdattr[i]].c_str());
      nattrv.erase(stdattr[i]);
    }
  }

  // put in the rest of the attributes
  for (std::map<std::string, std::string>::const_iterator iter = nattrv.begin(); iter != nattrv.end(); iter++)
      xmlSetProp(xmlDocGetRootElement(doc), BAD_CAST (*iter).first.c_str(), BAD_CAST (*iter).second.c_str());

  // save the created document
  // "-" filename is standard output
  xmlSaveCtxtPtr saver = xmlSaveToFilename(filename, output_encoding, save_options);
  xmlSaveDoc(saver, doc);
  xmlSaveClose(saver);
}

// output current unit element as text
void srcMLUtility::outputSrc(const char* ofilename, xmlTextReaderPtr reader) {

  // setup an output handler
  handler = xmlFindCharEncodingHandler(output_encoding);

  // generate the full tree in the reader of the unit
  // so that we can move it to the output
  xmlTextReaderExpand(reader);

#ifdef LIBXML_ENABLED
  // no need for encoding change
  if (strcmp(handler->name, "UTF-8") == 0)
    options |= OPTION_SKIP_ENCODING;
#endif

  // find the old markup for formfeed nodes and replace them with text nodes with the
  // formfeed character
  xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(xpath_formfeed, context);
  for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {
    xmlNodePtr formfeed = xmlNewText(BAD_CAST "\f");
    xmlReplaceNode(result_nodes->nodesetval->nodeTab[i], formfeed);
  }

  // find the escaped element nodes and replace them with a text nodes with the
  // unescaped value of the attribute char
  result_nodes = xmlXPathCompiledEval(xpath_escape, context);
  for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

    // from the char attribute, find out which character was escaped
    char* ac = (char*) xmlGetProp(result_nodes->nodesetval->nodeTab[i], BAD_CAST "char");

    // convert from the escaped to the unescaped value
    char values[2] = { strtod(ac, NULL), '\0' };

    // replace the escape element node with a text node of the unescaped value
    xmlNodePtr escape = xmlNewText(BAD_CAST values);
    xmlReplaceNode(result_nodes->nodesetval->nodeTab[i], escape);
  }

  // output all the content
  xmlChar* s = xmlNodeGetContent(xmlTextReaderCurrentNode(reader));

  /*                                                                                                            
   * save the content to a temp buffer.                                                                         
   */
  xmlOutputBufferPtr buf;
  buf = xmlOutputBufferCreateFilename(ofilename, handler, 0);
  if (buf == NULL) return;
  xmlOutputBufferWrite(buf, strlen((char*) s), (char*) s);
  xmlOutputBufferClose(buf);
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
