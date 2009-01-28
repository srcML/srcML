/*
  SAX2ExtractUnitsSrc.cpp

  Copyright (C) 2008  SDML (www.sdml.info)

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
#include <iostream>

#ifdef __GNUC__
#include <sys/stat.h>
#include <sys/errno.h>
#else
#include <direct.h>
#endif

// directory permission for expand
#ifdef __GNUC__
const int EXPAND_DIR_PERM = S_IRWXU | S_IRWXG;
#endif

using namespace SAX2ExtractUnitsSrc;

static int mkpath(char* path
#ifdef __GNUC__		   
		   , mode_t mode
#endif		   
		   ) {

  for (char* c = path; *c; ++c) {

    // make the directory path so far
    if (*c == '/') {

      *c = '\0';

      int ret = mkdir(path
#ifdef __GNUC__		   
		      , mode
#endif		   
		      );

      if (ret != 0 && errno != EEXIST)
	return ret;

      *c = '/';
    }
  }

  return 0;
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
    pstate->ctxt->sax->startElementNs = &startElementNs;
  }

  // start a new output buffer and corresponding file for a unit element
  void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
		    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    // check that this is a nested file
    if (pstate->count == 0 && !(strcmp((const char*) localname, "unit") == 0 &&
	  strcmp((const char*) URI, "http://www.sdml.info/srcML/src") == 0)) {
      fprintf(stderr, "Options only valid for nested srcML documents\n");
      xmlStopParser(pstate->ctxt);
      return;
    }

    ++(pstate->count);

    // start up the output unit
    startUnit(pstate, nb_attributes, attributes);

    // next state is to copy the unit contents, finishing when needed
    pstate->ctxt->sax->startElementNs = &startElementNsEscape;
    pstate->ctxt->sax->characters = &characters;
    pstate->ctxt->sax->ignorableWhitespace = &characters;
    pstate->ctxt->sax->endElementNs = &endElementNs;
  }

  // start a new output buffer and corresponding file for a unit element
  void startUnit(State* pstate, int nb_attributes, const xmlChar** attributes) {

    char path[512] = "";

    // find the directory
    bool founddirectory = false;
    int startfilename = 0;
    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5)
      if (strcmp((const char*) attributes[index], "dir") == 0) {

	int filename_size = (const char*) attributes[index + 4] - (const char*) attributes[index + 3];
	if (filename_size > 0) {
	  strncpy(path, (const char*) attributes[index + 3], filename_size);
	  path[filename_size] = '/';
	  path[filename_size + 1] = '\0';
	  startfilename = filename_size + 1;
	}

	founddirectory = true;
	break;
      }

    // find the filename
    bool foundfilename = false;
    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5)
      if (strcmp((const char*) attributes[index], "filename") == 0) {

	strncpy(path + startfilename, (const char*) attributes[index + 3],
		(const char*) attributes[index + 4] - (const char*) attributes[index + 3]);

	foundfilename = true;
	break;
      }

    // filename is required
    if (!foundfilename) {
      fprintf(stderr, "Missing filename attribute\n");
      return;
    }

    // construct the directory if needed
    if (founddirectory) {

      // make the directory path if there is one
      int ret = mkpath(path
#ifdef __GNUC__
		       , EXPAND_DIR_PERM
#endif
		       );

      if (ret != 0 && errno != EEXIST) {
	fprintf(stderr, "Error %d creating directory:  %s\n", errno, path);
      }
    }

    // output file status message if in verbose mode
    if (isoption(*(pstate->poptions), OPTION_VERBOSE))
      fprintf(stderr, "%ld\t%s\n", pstate->count, path);

    pstate->output = xmlOutputBufferCreateFilename(path, pstate->handler, 0);
    if (pstate->output == NULL) {
      fprintf(stderr, "Output buffer error\n");
      xmlStopParser(pstate->ctxt);
    }

}

  // end unit element and current file/buffer (started by startElementNs
  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    State* pstate = (State*) ctx;

    // only process nested unit start elements
    if (pstate->ctxt->nameNr != 2)
      return;

    // finish up this file
    xmlOutputBufferClose(pstate->output);

    // stop after this file (and end gracefully) with ctrl-c
    if (isoption(*(pstate->poptions), OPTION_TERMINATE)) {
      xmlStopParser(pstate->ctxt);
      throw TerminateLibXMLError();
    }

    // now waiting for start of next unit
    pstate->ctxt->sax->startElementNs = &startElementNs;
    pstate->ctxt->sax->characters = 0;
    pstate->ctxt->sax->endElementNs = 0;
  }

  // escape control character elements
  void startElementNsEscape(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
			    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
			    const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    // quick tests for start of element name
    if ((localname[0] != 'e' || localname[1] != 's') && localname[0] != 'f')
      return;

    // correct prefix part of srcML namespace
    if (strcmp((const char*) URI + 29, "src"))
      return;

    // correct overall namespace
    if (strcmp((const char*) URI, "http://www.sdml.info/srcML/src"))
      return;

    // in the proper unit
    if (strcmp((const char*) localname, "escape") == 0) {
      
      // convert from the escaped to the unescaped value
      char value = strtod((const char*) attributes[3], NULL);
      xmlOutputBufferWrite(pstate->output, 1, &value);

    } else if (strcmp((const char*) localname, "formfeed") == 0) {
   
      char value = '\f';
      xmlOutputBufferWrite(pstate->output, 1, &value);
    }
  }

};
