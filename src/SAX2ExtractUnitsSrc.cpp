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


*/

#include "SAX2ExtractUnitsSrc.h"

// directory permission for expand
#ifdef __GNUC__
const int EXPAND_DIR_PERM = S_IRWXU | S_IRWXG;
#endif

using namespace SAX2ExtractUnitsSrc;

extern xmlCharEncodingHandlerPtr ghandler;

static int mkpath(const char* path
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

namespace SAX2ExtractUnitsSrc {

  xmlSAXHandler factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized    = XML_SAX2_MAGIC;
    sax.startElementNs = &startElementNsRoot;

    return sax;
  }

  // output all characters to output buffer
  void characters(void* user_data, const xmlChar* ch, int len) {

    State* pstate = (State*) user_data;

    xmlOutputBufferWrite(pstate->output, len, (const char*) ch);
  }

  // handle root unit of compound document
  void startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    // start counting units after the root
    pstate->count = 0;

    // handle nested units
    if (pstate->unit == -1)
      pstate->ctxt->sax->startElementNs = &startElementNsUnit;
    else
      startElementNsSingleUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
  }

  // start a new output buffer and corresponding file for a unit element
  void startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    ++(pstate->count);

    // start up the output unit
    startUnit(pstate, nb_attributes, attributes);

    // next state is to copy the unit contents, finishing when needed
    pstate->ctxt->sax->startElementNs = &startElementNsEscape;
    pstate->ctxt->sax->characters = &characters;
    pstate->ctxt->sax->ignorableWhitespace = &characters;
    pstate->ctxt->sax->endElementNs = &endElementNsUnit;
  }

  // start a new output buffer and corresponding file for a unit element
  void startElementNsSingleUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    pstate->output = xmlOutputBufferCreateFilename(pstate->ofilename, ghandler, 0);
    if (pstate->output == NULL) {
      std::cerr << "Output buffer error" << std::endl;
      xmlStopParser(pstate->ctxt);
    }

    // next state is to copy the unit contents, finishing when needed
    pstate->ctxt->sax->startElementNs = &startElementNsEscape;
    pstate->ctxt->sax->characters = &characters;
    pstate->ctxt->sax->ignorableWhitespace = &characters;
    pstate->ctxt->sax->endElementNs = &endElementNsSingleUnit;
  }

  // start a new output buffer and corresponding file for a unit element
  void startUnit(State* pstate, int nb_attributes, const xmlChar** attributes) {

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
    if (isoption(*(pstate->poptions), OPTION_VERBOSE))
      std::cerr << pstate->count << '\t' << output_filename << '\n';

    pstate->output = xmlOutputBufferCreateFilename(output_filename.c_str(), ghandler, 0);
    if (pstate->output == NULL) {
      std::cerr << "Output buffer error" << std::endl;
      xmlStopParser(pstate->ctxt);
    }
}

  // end unit element and current file/buffer (started by startElementNsUnit
  void endElementNsSingleUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    State* pstate = (State*) ctx;

    if (pstate->ctxt->nameNr != 1)
      return;

    // found the end of this unit
    //  if (strcmp((const char*) localname, "unit") == 0 &&
    //      strcmp((const char*) URI, "http://www.sdml.info/srcML/src") == 0) {

    xmlOutputBufferClose(pstate->output);

    pstate->ctxt->sax->startElementNs = 0;
    pstate->ctxt->sax->characters = 0;
    pstate->ctxt->sax->ignorableWhitespace = 0;
    pstate->ctxt->sax->endElementNs = 0;
    //  }
  }

  // end unit element and current file/buffer (started by startElementNsUnit
  void endElementNsUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    State* pstate = (State*) ctx;

    if (pstate->ctxt->nameNr != 2)
      return;

    // found the end of this unit
    //  if (strcmp((const char*) localname, "unit") == 0 &&
    //      strcmp((const char*) URI, "http://www.sdml.info/srcML/src") == 0) {

    xmlOutputBufferClose(pstate->output);

    // stop after this file (and end gracefully) with ctrl-c
    if (isoption(*(pstate->poptions), OPTION_TERMINATE)) {
      xmlStopParser(pstate->ctxt);
      throw TerminateLibXMLError();
    }

    pstate->ctxt->sax->startElementNs = &startElementNsUnit;
    pstate->ctxt->sax->characters = 0;
    pstate->ctxt->sax->endElementNs = 0;
    //  }
  }

  // escape control character elements
  void startElementNsEscape(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
			    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
			    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

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

};
