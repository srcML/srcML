/*
  SAX2ListUnits.cpp

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

#include "SAX2ListUnits.h"
#include "srcmlns.h"
#include "srcmlapps.h"

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

using namespace SAX2ListUnits;

namespace SAX2ListUnits {

  xmlSAXHandler factory() {

    xmlSAXHandler sax = { 0 };

    sax.initialized    = XML_SAX2_MAGIC;
    sax.startElementNs = &startElementNsRoot;

    return sax;
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

    // find the directory
    int dir_index = -1;
    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5)
      if (strcmp((const char*) attributes[index], UNIT_ATTRIBUTE_DIRECTORY) == 0) {

	dir_index = index;
	break;
      }
    int dir_size = dir_index != -1 ? (const char*) attributes[dir_index + 4] - (const char*) attributes[dir_index + 3] : 0;

    // find the filename
    int filename_index = -1;
    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5)
      if (strcmp((const char*) attributes[index], UNIT_ATTRIBUTE_FILENAME) == 0) {

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
    //    realloc(pstate->whole_path, dir_size + filename_size + 1);
    pstate->whole_path[0] = '\0';
    int size = 0;
    // if there is a directory, then we need to construct each part of the path
    if (dir_size > 0) {

      // put the directory into the whole path
      strncat(pstate->whole_path, (char*) attributes[dir_index + 3], dir_size);
      //      pstate->whole_path[dir_size] = '\0';
      size = dir_size;

      strcat(pstate->whole_path, "/");
    }

    // add on the filename
    strncat(pstate->whole_path, (const char*) attributes[filename_index + 3], filename_size);
    size += filename_size;

    // output file status message if in verbose mode
    fprintf(stderr, "%ld\t%s\n", pstate->count, pstate->whole_path);
}
};
