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
#include "srcmlns.h"
#include <boost/filesystem.hpp>

#include <iostream>

#ifdef __GNUC__
#include <sys/stat.h>
#include <sys/errno.h>
#else
#include <direct.h>
#endif

#ifdef __GNUC__
#define EOL "\n"
#define EOL_SIZE 1
#else
#define EOL "\r\n"
#define EOL_SIZE 2
#endif

// directory permission for expand
#ifdef __GNUC__
const int EXPAND_DIR_PERM = S_IRWXU | S_IRWXG;
#endif

using namespace SAX2ExtractUnitsSrc;

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

    const char* c = (const char*) ch;
    int pos = 0;
    const char* chend = (const char*) ch + len;
    while (c < chend) {

      switch (*c) {
      case '\n' :
	xmlOutputBufferWrite(pstate->output, pos, (const char*)(BAD_CAST c - pos));
	pos = 0;
	xmlOutputBufferWrite(pstate->output, EOL_SIZE, EOL);
	break;

      default :
	++pos;
	break;
      };
      ++c;
    }

    xmlOutputBufferWrite(pstate->output, pos, (const char*)(BAD_CAST c - pos));
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
	  strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0)) {
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

    // find the directory
    int dir_index = -1;
    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5)
      if (strcmp((const char*) attributes[index], "dir") == 0) {

	dir_index = index;
	break;
      }
    int dir_size = dir_index != -1 ? (const char*) attributes[dir_index + 4] - (const char*) attributes[dir_index + 3] : 0;

    // find the filename
    int filename_index = -1;
    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5)
      if (strcmp((const char*) attributes[index], "filename") == 0) {

	filename_index = index;
	break;
      }
    int filename_size = filename_index != -1 ? (const char*) attributes[filename_index + 4] - (const char*) attributes[filename_index + 3] : 0;

    // filename is required
    if (filename_size <= 0) {
      fprintf(stderr, "Missing filename attribute\n");
      return;
    }

    // create a complete path from the two separate directories and filename attributes
    realloc(pstate->whole_path, dir_size + filename_size + 1);

    // if there is a directory, then we need to construct each part of the path
    if (dir_size > 0) {

      // put the directory into the whole path
      memcpy(pstate->whole_path, attributes[dir_index + 3], dir_size);
      pstate->whole_path[dir_size] = '\0';

      // construct the directory subpath by subpath
      for (char* c = pstate->whole_path; *c; ++c)

	// replace the path delimiter with a null, mkdir, then put back
	if (*c == '/') {
	  *c = '\0';
	  mkdir(pstate->whole_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	  *c = '/';
	}

      mkdir(pstate->whole_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

      // delimiter for following filename
      pstate->whole_path[dir_size] = '/';
    }

    // add on the filename
    strncpy(pstate->whole_path + dir_size + (dir_size > 0 ? 1 : 0), (const char*) attributes[filename_index + 3], filename_size);
    /*
	  fprintf(stderr, "Error in creating directory:  %s\n", directory_path.string().c_str());
    */

    // output file status message if in verbose mode
    if (isoption(*(pstate->poptions), OPTION_VERBOSE))
      fprintf(stderr, "%ld\t%s\n", pstate->count, pstate->whole_path);

    pstate->output = xmlOutputBufferCreateFilename(pstate->whole_path, pstate->handler, 0);
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

    // in the proper unit
    if (strcmp((const char*) localname, "escape") == 0 &&
 	strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0) {
      
      // convert from the escaped to the unescaped value
      char value = strtod((const char*) attributes[3], NULL);
      xmlOutputBufferWrite(pstate->output, 1, &value);

    } else if (strcmp((const char*) localname, "formfeed") == 0 &&
	       strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0) {
   
      char value = '\f';
      xmlOutputBufferWrite(pstate->output, 1, &value);
    }
  }

};
