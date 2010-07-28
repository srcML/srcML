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
#include "ProcessUnit.h"
#include "ExtractUnitsSrc.h"

namespace {

  struct Element {
    const xmlChar* localname;
    const xmlChar* prefix;
    const xmlChar* URI;
    int nb_namespaces;
    const xmlChar** namespaces;
    int nb_attributes;
    int nb_defaulted;
    const xmlChar** attributes;
  };

}

static Element root;
static const xmlChar* firstcharacters;
static int firstlen;
static bool isarchive = false;

xmlSAXHandler SAX2ExtractUnitsSrc::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;
  sax.startElementNs = &startElementNsRoot;

  return sax;
}

// output all characters to output buffer
void SAX2ExtractUnitsSrc::characters(void* ctx, const xmlChar* ch, int len) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  if (!firstcharacters) {
    firstcharacters = (const xmlChar*) malloc(len);
    strncpy((char*) firstcharacters, (const char*) ch, len);
    firstlen = len;
  } else
    pstate->pprocess->charactersUnit(ctx, ch, len);
}

// handle root unit of compound document
void SAX2ExtractUnitsSrc::startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                             int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                             const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // start counting units after the root
  pstate->count = 0;

  // save all the info in case this is not a srcML archive
  root.localname = localname ? (xmlChar*) strdup((const char*) localname) : 0;
  root.prefix = prefix ? (xmlChar*) strdup((const char*) prefix) : 0;
  root.URI = URI ? (xmlChar*) strdup((const char*) URI) : 0;

  root.nb_namespaces = nb_namespaces;
  int ns_length = nb_namespaces * 2;
  root.namespaces = (const xmlChar**) malloc(ns_length * sizeof(namespaces[0]));
  for (int i = 0; i < ns_length; ++i)
    root.namespaces[i] = namespaces[i] ? (xmlChar*) strdup((const char*) namespaces[i]) : 0;

  root.nb_attributes = nb_attributes;
  root.nb_defaulted = nb_defaulted;

  int nb_length = nb_attributes * 5;
  root.attributes = (const xmlChar**) malloc(nb_length * sizeof(attributes[0]));
  for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
    root.attributes[index] = attributes[index] ? (xmlChar*) strdup((const char*) attributes[index]) : 0;
    root.attributes[index + 1] = attributes[index + 1] ? (xmlChar*) strdup((const char*) attributes[index + 1]) : 0;
    root.attributes[index + 2] = attributes[index + 2] ? (xmlChar*) strdup((const char*) attributes[index + 2]) : 0;
    int vallength = attributes[index + 4] - attributes[index + 3];
    root.attributes[index + 3] = (const xmlChar*) malloc(vallength);
    strncpy((char *) root.attributes[index + 3], (const char*) attributes[index + 3], vallength);
    root.attributes[index + 4] = root.attributes[index + 3] + vallength;
  }

  firstcharacters = 0;
  firstlen = 0;

  // handle nested units
  ctxt->sax->startElementNs = &startElementNs;
  ctxt->sax->characters = &characters;
}

// start a new output buffer and corresponding file for a unit element
void SAX2ExtractUnitsSrc::startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                         int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                         const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // next state is to copy the unit contents, finishing when needed
  ctxt->sax->startElementNs = &startElementNsEscape;
  ctxt->sax->characters = &characters;
  ctxt->sax->ignorableWhitespace = &characters;
  ctxt->sax->endElementNs = &endElementNs;

  // see if this is really a nested unit.  If not, then we have an individual
  // unit (not a srcML archive) and need to process the cached root
  if (pstate->count == 0 && !(strcmp((const char*) localname, "unit") == 0 &&
                              strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0)) {

    ++(pstate->count);

    // should have made this call earlier, makeup for it now
    pstate->pprocess->startUnit(ctx, root.localname, root.prefix, root.URI, root.nb_namespaces,
                                root.namespaces, root.nb_attributes, root.nb_defaulted, root.attributes);

    if (firstcharacters)
      characters(ctx, firstcharacters, firstlen);

    isarchive = false;

  } else {

    ++(pstate->count);

    isarchive = true;

    // process the start of this unit
    pstate->pprocess->startUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted,
                              attributes);
  }
}

// end unit element and current file/buffer (started by startElementNs
void SAX2ExtractUnitsSrc::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // only process nested unit start elements
  if (ctxt->nameNr != (isarchive ? 2 : 1))
    return;

  // process the end of the unit
  pstate->pprocess->endUnit(ctx, localname, prefix, URI);

  // now waiting for start of next unit
  ctxt->sax->startElementNs = &startElementNs;
  ctxt->sax->characters = 0;
  ctxt->sax->endElementNs = 0;
}

// escape control character elements
void SAX2ExtractUnitsSrc::startElementNsEscape(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
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

    characters(ctx, BAD_CAST &value, 1);
  }
}
