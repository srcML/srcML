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

xmlSAXHandler SAX2ExtractUnitsSrc::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;
  sax.startElementNs = &startElementNsRoot;
  sax.characters = &charactersPre;  // catch first text of single unit

  return sax;
}

/*
  Called right after the root unit.

  Characters right after the first <unit element have to be preserved
  until we know if it is an archive or not.  If an archive, this text
  will be thrown away.  If not an archive, then we need these
  characters since they form the white space before the first element
  of the code.

  This callback will only be called once.
*/
void SAX2ExtractUnitsSrc::charactersPre(void* ctx, const xmlChar* ch, int len) {

  // fprintf(stderr, "HERE: %s\n", __FUNCTION__);
  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  pstate->firstcharacters.append((const char*) ch, len);
}

// output all characters to output buffer
void SAX2ExtractUnitsSrc::charactersUnit(void* ctx, const xmlChar* ch, int len) {

  // fprintf(stderr, "HERE: %s\n", __FUNCTION__);
  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  pstate->pprocess->characters(ctx, ch, len);
}

/*
  Handle root unit.

  The root unit may be a true root, as in an archive, or the complete
  unit.

  Store the attributes/namespaces from the element.  This information
  is buffered so that when the first nested element is found (and
  therefore know if it is an archive or not) we can act accordingly.

  This handler does NOT call startUnit() or startRootUnit().
*/
void SAX2ExtractUnitsSrc::startElementNsRoot(void* ctx, const xmlChar* localname,
      const xmlChar* prefix, const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces,
      int nb_attributes, int nb_defaulted, const xmlChar** attributes) {

  // fprintf(stderr, "HERE: %s\n", __FUNCTION__);
  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // start counting units after the root
  pstate->count = 0;

  // need to record that we actually found something besides the root element
  pstate->rootonly = true;

  // save all the info in case this is not a srcML archive
  pstate->root.localname = localname ? (xmlChar*) strdup((const char*) localname) : 0;
  pstate->root.prefix = prefix ? (xmlChar*) strdup((const char*) prefix) : 0;
  pstate->root.URI = URI ? (xmlChar*) strdup((const char*) URI) : 0;

  pstate->root.nb_namespaces = nb_namespaces;
  int ns_length = nb_namespaces * 2;
  pstate->root.namespaces = (const xmlChar**) malloc(ns_length * sizeof(namespaces[0]));
  for (int i = 0; i < ns_length; ++i)
    pstate->root.namespaces[i] = namespaces[i] ? (xmlChar*) strdup((const char*) namespaces[i]) : 0;

  pstate->root.nb_attributes = nb_attributes;
  pstate->root.nb_defaulted = nb_defaulted;

  int nb_length = nb_attributes * 5;
  pstate->root.attributes = (const xmlChar**) malloc(nb_length * sizeof(attributes[0]));
  for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
    pstate->root.attributes[index] = attributes[index] ? (xmlChar*) strdup((const char*) attributes[index]) : 0;
    pstate->root.attributes[index + 1] = attributes[index + 1] ? (xmlChar*) strdup((const char*) attributes[index + 1]) : 0;
    pstate->root.attributes[index + 2] = attributes[index + 2] ? (xmlChar*) strdup((const char*) attributes[index + 2]) : 0;
    int vallength = attributes[index + 4] - attributes[index + 3];
    pstate->root.attributes[index + 3] = (const xmlChar*) malloc(vallength);
    strncpy((char *) pstate->root.attributes[index + 3], (const char*) attributes[index + 3], vallength);
    pstate->root.attributes[index + 4] = pstate->root.attributes[index + 3] + vallength;
  }

  // handle nested units
  ctxt->sax->startElementNs = &startElementNsFirst;
  ctxt->sax->endElementNs = &endElementNsSkip;
}

/*
  Called first for the first nested element (right after the root).
  If this is an archive, then call startRootUnit() with the cached
  data and throw away the cached characters, then proceed normally.
  If this is not an archive, then call startUnit() with the cached
  data, process the cached characters, then proceed with the normal
  start element processing.
*/
void SAX2ExtractUnitsSrc::startElementNsFirst(void* ctx, const xmlChar* localname,
    const xmlChar* prefix, const xmlChar* URI,
    int nb_namespaces, const xmlChar** namespaces,
    int nb_attributes, int nb_defaulted,
    const xmlChar** attributes) {

  // fprintf(stderr, "HERE: %s\n", __FUNCTION__);
  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // so we have an element inside of the unit
  pstate->rootonly = false;

  // see if this is really a nested unit.  If not, then we have an individual
  // unit (not a srcML archive) and need to process the cached root
  if (!(strcmp((const char*) localname, "unit") == 0 && strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0)) {

    // this is not an archive
    pstate->isarchive = false;

    // should have made this call earlier, makeup for it now
    pstate->pprocess->startUnit(ctx, pstate->root.localname, pstate->root.prefix, pstate->root.URI, pstate->root.nb_namespaces,
                                pstate->root.namespaces, pstate->root.nb_attributes, pstate->root.nb_defaulted, pstate->root.attributes);

    // output cached characters if we found any
    if (!pstate->firstcharacters.empty())
      charactersUnit(ctx, BAD_CAST pstate->firstcharacters.c_str(), pstate->firstcharacters.length());
    pstate->firstcharacters.clear();

    // process using the normal startElementNs
    startElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);    

    // next state is to copy the unit contents, finishing when needed
    ctxt->sax->startElementNs = &startElementNsUnit;
    ctxt->sax->characters = &charactersUnit;
    ctxt->sax->ignorableWhitespace = &charactersUnit;
    ctxt->sax->endElementNs = &endElementNs;

  } else {

    // this is an archive
    pstate->isarchive = true;

    // should have made this call earlier, makeup for it now
    pstate->pprocess->startRootUnit(ctx, pstate->root.localname, pstate->root.prefix, pstate->root.URI, pstate->root.nb_namespaces, pstate->root.namespaces, pstate->root.nb_attributes, pstate->root.nb_defaulted, pstate->root.attributes);

    // throw away the characters (they are ignorable between root and nested unit)
    pstate->firstcharacters.clear();

    // process using the normal startElementNs
    startElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);    
  }
}

/*
  Handle startElement (except for root element).

  This is called first for the first nested element (right after the root):
*/
void SAX2ExtractUnitsSrc::startElementNs(void* ctx, const xmlChar* localname,
           const xmlChar* prefix, const xmlChar* URI,
           int nb_namespaces, const xmlChar** namespaces,
           int nb_attributes, int nb_defaulted,
           const xmlChar** attributes) {

  // fprintf(stderr, "HERE: %s\n", __FUNCTION__);
  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // so we did find another element in the root
  pstate->rootonly = false;

  ++(pstate->count);

  // call startUnit if I want to see all the units, or want to see this unit
  if (pstate->unit == -1 || pstate->count == pstate->unit) {

    // process the start of this unit
    pstate->pprocess->startUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);

    // next state is to copy the unit contents, finishing when needed
    ctxt->sax->startElementNs = &startElementNsUnit;
    ctxt->sax->characters = &charactersUnit;
    ctxt->sax->ignorableWhitespace = &charactersUnit;
    ctxt->sax->endElementNs = &endElementNs;

  } else {

    // we are going to skip processing this element
    ctxt->sax->startElementNs = 0;
    ctxt->sax->characters = 0;
    ctxt->sax->ignorableWhitespace = 0;
    ctxt->sax->endElementNs = &endElementNsSkip;
  }
}

/*
  // free up the saved element
  free((void*) pstate->root.localname);
  free((void*) pstate->root.prefix);
  free((void*) pstate->root.URI);

  int ns_length = pstate->root.nb_namespaces * 2;
  for (int i = 0; i < ns_length; ++i)
    free((void*) pstate->root.namespaces[i]);
  free((void*) pstate->root.namespaces);

  int nb_length = nb_attributes * 5;
  for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
    free((void*) pstate->root.attributes[index]);
    free((void*) pstate->root.attributes[index + 1]);
    free((void*) pstate->root.attributes[index + 2]);
    free((void*) pstate->root.attributes[index + 3]);
  }
  free((void*) pstate->root.attributes);
  */

void SAX2ExtractUnitsSrc::endElementNsUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  // fprintf(stderr, "HERE: %s %s\n", __FUNCTION__, localname);
  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  pstate->pprocess->endElementNs(ctx, localname, prefix, URI);
}

// end unit element and current file/buffer (started by startElementNs
void SAX2ExtractUnitsSrc::endElementNsSkip(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  // fprintf(stderr, "HERE: %s %s\n", __FUNCTION__, localname);
  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  if (!pstate->rootonly && ctxt->nameNr != (pstate->isarchive ? 2 : 1))
    return;

  // got here without ever seeing a nested element of any kind,
  // so have to do the whole process
  if (pstate->rootonly) {

    // should have made this call earlier, makeup for it now
    pstate->pprocess->startUnit(ctx, pstate->root.localname, pstate->root.prefix,
                                pstate->root.URI, pstate->root.nb_namespaces,
                                pstate->root.namespaces, pstate->root.nb_attributes,
                                pstate->root.nb_defaulted, pstate->root.attributes);

    // first characters
    if (!pstate->firstcharacters.empty())
      charactersUnit(ctx, BAD_CAST pstate->firstcharacters.c_str(), pstate->firstcharacters.length());
    pstate->firstcharacters.clear();

    // end the unit
    pstate->pprocess->endUnit(ctx, localname, prefix, URI);
    return;
  }

  // process the end of this unit
  if (pstate->unit == -1 || (!pstate->isarchive && pstate->unit == 0) || pstate->count == pstate->unit)
    pstate->pprocess->endUnit(ctx, localname, prefix, URI);

  // done if we are only stopping on this one
  if (pstate->count == pstate->unit) {

    ctxt->sax->startElementNs = 0;
    ctxt->sax->characters = 0;
    ctxt->sax->ignorableWhitespace = 0;
    ctxt->sax->endElementNs = 0;
    xmlStopParser(ctxt);
  }

  // now waiting for start of next unit
  ctxt->sax->startElementNs = &startElementNs;
  ctxt->sax->characters = 0;
}

// end unit element and current file/buffer (started by startElementNs
void SAX2ExtractUnitsSrc::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // fprintf(stderr, "HERE: %s %s %d %d\n", __FUNCTION__, localname, ctxt->nameNr, pstate->isarchive);

  // only process nested unit start elements
  if (ctxt->nameNr != (pstate->isarchive ? 2 : 1)) {
    endElementNsUnit(ctx, localname, prefix, URI);
    return;
  }

  endElementNsSkip(ctx, localname, prefix, URI);
}

// escape control character elements
void SAX2ExtractUnitsSrc::startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                                int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                                const xmlChar** attributes) {

  // fprintf(stderr, "HERE: %s %s\n", __FUNCTION__, localname);
  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;
  
  pstate->pprocess->startElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
}
