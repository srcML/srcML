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

// directory permission for expand
#ifdef __GNUC__
const int EXPAND_DIR_PERM = S_IRWXU | S_IRWXG;
#endif

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

void skiptonextunit(xmlTextReaderPtr reader) throw (LibXMLError);
void skiptounit(xmlTextReaderPtr reader, const char* filename) throw (LibXMLError);
void skiptounit(xmlTextReaderPtr reader, int number) throw (LibXMLError);

struct ParsingState {
  long count;
  const char * root_filename;
  int options;
  xmlOutputBufferPtr output;
  xmlParserCtxtPtr ctxt;
};

// start a new output buffer and corresponding file for a
// output all characters to output buffer
static void characters_copy(void* user_data, const xmlChar* ch, int len);

// handle root unit of compound document
static void startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes);

// unit element
static void startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes);

// start a new output buffer and corresponding file for a
// unit element
 static void startUnit(ParsingState* pstate, int nb_attributes, const xmlChar** attributes);

static void endElementNsUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);

static void startElementNsEscape(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes);

xmlCharEncodingHandlerPtr ghandler;

// constructor
srcMLUtility::srcMLUtility(const char* infilename, const char* encoding, int& op)
  : infile(infilename), output_encoding(encoding), options(op), reader(0), handler(0), moved(false) {

  // empty filename indicates standard input
  if (infile == 0)
    infile = "-";

  // setup an output handler
  ghandler = handler = xmlFindCharEncodingHandler(output_encoding);

  // get out now for expand since switched to sax handling
  if (isoption(options, OPTION_EXPAND))
    return;

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

    xmlFree(value);

    return s;
  }

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

  // skip to the proper nested unit
  skiptounit(reader, unitnumber);
}


// count of nested units
int srcMLUtility::unit_count() {

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

  ParsingState state;
  state.root_filename = root_filename;
  state.options = options;

  xmlSAXHandler sax = { 0 };
  sax.initialized = XML_SAX2_MAGIC;
  sax.startElementNs = &startElementNsRoot;

  xmlParserCtxtPtr ctxt = xmlCreateFileParserCtxt(infile);
  if (ctxt == NULL) return;
  ctxt->sax = &sax;
  ctxt->userData = &state;
  state.ctxt = ctxt;

  xmlParseDocument(ctxt);

  ctxt->sax = NULL;

  xmlFreeParserCtxt(ctxt);
  /*
    // stop after this file (and end gracefully) with ctrl-c
    if (isoption(options, OPTION_TERMINATE))
      throw TerminateLibXMLError();
  */
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
    for (int i = 0; i < (int) (sizeof(stdns) / sizeof(stdns[0])); ++i) {
      
      std::map<std::string, std::string>::iterator pos = nnsv.find(stdns[i]);
      if (pos != nnsv.end()) {

	xmlNewNs(xmlDocGetRootElement(doc), BAD_CAST stdns[i],
		 pos->second == "" ? NULL : BAD_CAST pos->second.c_str());

	nnsv.erase(pos);
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
  for (int i = 0; i < (int) (sizeof(stdattr) / sizeof(stdattr[0])); ++i) {

    std::map<std::string, std::string>::iterator pos = nattrv.find(stdattr[i]);
    if (pos != nattrv.end()) {

	xmlSetProp(xmlDocGetRootElement(doc), BAD_CAST stdattr[i], BAD_CAST pos->second.c_str());

	nattrv.erase(pos);
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

#ifdef LIBXML_ENABLED
  // no need for encoding change
  if (strcmp(handler->name, "UTF-8") == 0)
    options |= OPTION_SKIP_ENCODING;
#endif

  // output all the content
  xmlOutputBufferPtr buf;
  buf = xmlOutputBufferCreateFilename(ofilename, handler, 0);
  if (buf == NULL) return;

  std::string s;
  while (1) {

    // read a node
    int ret = xmlTextReaderRead(reader);
    if (ret != 1)
      throw LibXMLError(ret);

    if (xmlTextReaderDepth(reader) <= 0 + moved)
      break;

    xmlNode* node = xmlTextReaderCurrentNode(reader);
    if (node->type == XML_TEXT_NODE) {

      const char* s = (const char*) node->content;
      xmlOutputBufferWrite(buf, strlen(s), s);

    } else if (node->type == XML_ELEMENT_NODE && xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) {

      if (strcmp((const char*) node->name, "escape") == 0) {
      
	// convert from the escaped to the unescaped value
	char value = strtod((char*) xmlTextReaderGetAttribute(reader, BAD_CAST "char"), NULL);

	xmlOutputBufferWrite(buf, 1, &value);

      } else if (strcmp((const char*) node->name, "formfeed") == 0) {
      
	xmlOutputBufferWrite(buf, 1, "\f");
      }
    }
  }

  /*                                                                                                            
   * save the content to a temp buffer.                                                                         
   */
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

// output all characters to output buffer
static void characters_copy(void* user_data, const xmlChar* ch, int len) {

  ParsingState* pstate = (ParsingState*) user_data;

  xmlOutputBufferWrite(pstate->output, len, (const char*) ch);
}

// handle root unit of compound document
static void startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

  ParsingState* pstate = (ParsingState*) ctx;

  // start counting units after the root
  pstate->count = 0;

  // handle nested units
  pstate->ctxt->sax->startElementNs = &startElementNsUnit;
}

// start a new output buffer and corresponding file for a
// unit element
static void startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

  ParsingState* pstate = (ParsingState*) ctx;

  // start up the output unit
  startUnit(pstate, nb_attributes, attributes);

  // next state is to copy the unit contents, finishing when needed
  pstate->ctxt->sax->startElementNs = &startElementNsEscape;
  pstate->ctxt->sax->characters = &characters_copy;
  pstate->ctxt->sax->endElementNs = &endElementNsUnit;
}

// start a new output buffer and corresponding file for a
// unit element
static void startUnit(ParsingState* pstate, int nb_attributes, const xmlChar** attributes) {

  ++(pstate->count);

  std::string filename;
  bool foundfilename = false;
  std::string directory;
  bool founddirectory = false;

  // extract the attributes from the unit for filename and directory
  unsigned int index = 0;
  for (int i = 0; i < nb_attributes; ++i, index += 5)
    if (strcmp((const char*) attributes[index], "filename") == 0) {

      filename.assign((const char*) attributes[index + 3], (const char*) attributes[index + 4]);
      foundfilename = true;

      if (founddirectory)
	break;

    } else if (strcmp((const char*) attributes[index], "dir") == 0) {

      directory.assign((const char*) attributes[index + 3], (const char*) attributes[index + 4]);
      founddirectory = true;

      if (foundfilename)
	break;
    }

    if (!foundfilename) {
      std::cerr << "Missing filename attribute" << '\n';
      return;
    }

    // construct the directory if needed
    const char* root_filename = "";
    std::string directory_filename = root_filename;
    if (founddirectory) {
      directory_filename += directory;

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
      output_filename += filename;

      // output file status message if in verbose mode
      if (isoption(pstate->options, OPTION_VERBOSE))
	std::cerr << pstate->count << '\t' << output_filename << '\n';

      pstate->output = xmlOutputBufferCreateFilename(output_filename.c_str(), ghandler, 0);
      if (pstate->output == NULL) {
	std::cerr << "Output buffer error" << std::endl;
	xmlStopParser(pstate->ctxt);
      }
}

// end unit element and current file/buffer (started by startElementNsUnit
static void endElementNsUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  ParsingState* pstate = (ParsingState*) ctx;

  if (pstate->ctxt->nameNr != 2)
    return;

  // found the end of this unit
  //  if (strcmp((const char*) localname, "unit") == 0 &&
  //      strcmp((const char*) URI, "http://www.sdml.info/srcML/src") == 0) {

    xmlOutputBufferClose(pstate->output);

    pstate->ctxt->sax->startElementNs = &startElementNsUnit;
    pstate->ctxt->sax->characters = 0;
    pstate->ctxt->sax->endElementNs = 0;
    //  }
}

// escape control character elements
static void startElementNsEscape(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

  ParsingState* pstate = (ParsingState*) ctx;

  // in the proper unit
  if (localname[0] == 'e' && strcmp((const char*) localname, "escape") == 0 &&
      strcmp((const char*) URI, "http://www.sdml.info/srcML/src") == 0) {
      
    // convert from the escaped to the unescaped value
    std::string avalue((const char*) attributes[3], (const char*) attributes[4]);
    char value = strtod((const char*) avalue.c_str(), NULL);
    xmlOutputBufferWrite(pstate->output, 1, &value);

  } else if (localname[0] == 'f' && strcmp((const char*) localname, "formfeed") == 0 &&
	     strcmp((const char*) URI, "http://www.sdml.info/srcML/src") == 0) {
   
    char value = '\f';
    xmlOutputBufferWrite(pstate->output, 1, &value);
  }
}
