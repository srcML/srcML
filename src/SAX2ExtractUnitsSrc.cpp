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
#include "srcmlapps.h"

#ifdef __GNUC__
#define EOL "\n"
#define EOL_SIZE 1
#else
#define EOL "\r\n"
#define EOL_SIZE 2
#endif

using namespace SAX2ExtractUnitsSrc;

class ProcessUnit {
public :
  virtual void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                 const xmlChar** attributes) = 0;

  virtual void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                 const xmlChar** attributes) = 0;

  virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) = 0;

  virtual void endRootUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) = 0;
};

class ExtractUnitsSrc : public ProcessUnit {
public :
  virtual void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                             const xmlChar** attributes) {

  }

  virtual void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar** attributes) {

    State* pstate = (State*) ctx;

    std::string& path = pstate->whole_path;

    // start the path with the (optional) target directory
    path = pstate->to_directory;

    // append the directory attribute
    int dir_index = find_attribute_index(nb_attributes, attributes, UNIT_ATTRIBUTE_DIRECTORY);
    if (dir_index != -1) {
	
	if (!path.empty() && path[path.size() - 1] != '/')
	  path += '/';

	path.append((const char*) attributes[dir_index + 3], (const char*) attributes[dir_index + 4]);
    }

    // find the filename attribute
    int filename_index = find_attribute_index(nb_attributes, attributes, UNIT_ATTRIBUTE_FILENAME);
    bool foundfilename = filename_index != -1;

    // filename is required
    if (!foundfilename) {
      fprintf(stderr, "Missing filename attribute\n");
      return;
    }

    // append the filename
    if (!path.empty() && path[path.size() - 1] != '/')
      path += '/';
    path.append((const char*) attributes[filename_index + 3], (const char*) attributes[filename_index + 4]);

    // output file status message if in verbose mode
    if (isoption(*(pstate->poptions), OPTION_VERBOSE))
      fprintf(stderr, "%ld\t%s\n", pstate->count, path.c_str());

    // now create the file itself
    pstate->output = xmlOutputBufferCreateFilename(path.c_str(), pstate->handler, 0);
    if (pstate->output == NULL) {
      fprintf(stderr, "Output buffer error\n");
      xmlStopParser(pstate->ctxt);
    }
  }

  virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  }

  virtual void endRootUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  }
};

static ExtractUnitsSrc extractor;

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

#ifdef __GNUC__
    xmlOutputBufferWrite(pstate->output, len, (const char*) ch);
#else
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
#endif
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
    /*
    // check that this is a nested file
    if (pstate->count == 0 && !(strcmp((const char*) localname, "unit") == 0 &&
	  strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0)) {
      fprintf(stderr, "Options only valid for nested srcML documents\n");
      xmlStopParser(pstate->ctxt);
      return;
    }
    */
    ++(pstate->count);

    // process the start of this unit
    extractor.startUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted,
                        attributes);

    // next state is to copy the unit contents, finishing when needed
    pstate->ctxt->sax->startElementNs = &startElementNsEscape;
    //    pstate->ctxt->sax->startElement = &startElementEscape;
    pstate->ctxt->sax->characters = &characters;
    pstate->ctxt->sax->ignorableWhitespace = &characters;
    pstate->ctxt->sax->endElementNs = &endElementNs;
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

    // only reason for this handler is that the escape element
    // needs to be expanded to the equivalent character.
    // So make it as quick as possible, since this is rare
    if (localname[0] == 'e' && localname[1] == 's' &&
	strcmp((const char*) localname, "escape") == 0 &&
 	strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0) {
      
      // convert from the escaped to the unescaped value
      char value = strtod((const char*) attributes[3], NULL);

      xmlOutputBufferWrite(((State*) ctx)->output, 1, &value);
    }
  }
};
