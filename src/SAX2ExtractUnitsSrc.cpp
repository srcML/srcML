/*
  SAX2ExtractUnitsSrc.cpp

  Copyright (C) 2012  SDML (www.sdml.info)

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

#include "SAX2ExtractUnitsSrc.hpp"
#include "srcmlns.hpp"
#include "srcmlapps.hpp"
#include "ProcessUnit.hpp"
#include "ExtractUnitsSrc.hpp"
#include "srcexfun.hpp"

static bool diff_filename = true;
static bool setupDiff(SAX2ExtractUnitsSrc* pstate,
                      int& nb_namespaces, const xmlChar** namespaces,
                      int& nb_attributes, const xmlChar** attributes);

// split the attribute according to the revision of the diff
static int split_diff_attribute(SAX2ExtractUnitsSrc* pstate, const char* attribute_name, int& nb_attributes, const xmlChar** attributes);

xmlSAXHandler SAX2ExtractUnitsSrc::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;
  sax.startElementNs = &startElementNsRoot;
  sax.characters = &charactersPre;  // catch first text of single unit
  sax.startDocument = &startDocument;
  sax.endDocument = &endDocument;

  return sax;
}

void SAX2ExtractUnitsSrc::startDocument(void *ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  pstate->pprocess->startDocument(ctx);
}

void SAX2ExtractUnitsSrc::endDocument(void *ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  pstate->pprocess->endDocument(ctx);
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

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  pstate->firstcharacters.append((const char*) ch, len);
}

// output all characters to output buffer
void SAX2ExtractUnitsSrc::charactersUnit(void* ctx, const xmlChar* ch, int len) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // skipping when in deleted or inserted file
  if (!diff_filename)
    return;

  // diff extraction
  if (isoption(*(pstate->poptions), OPTION_DIFF) && pstate->st.back() != DIFF_COMMON && pstate->st.back() != pstate->status)
    return;

  pstate->pprocess->characters(ctx, ch, len);
}

// output all characters to output buffer
void SAX2ExtractUnitsSrc::cdatablockUnit(void* ctx, const xmlChar* ch, int len) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // skipping when in deleted or inserted file
  if (!diff_filename)
    return;

  // diff extraction
  if (isoption(*(pstate->poptions), OPTION_DIFF) && pstate->st.back() != DIFF_COMMON && pstate->st.back() != pstate->status)
    return;

  pstate->pprocess->cdatablock(ctx, ch, len);
}

// output all characters to output buffer
void SAX2ExtractUnitsSrc::commentUnit(void* ctx, const xmlChar* ch) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // skipping when in deleted or inserted file
  if (!diff_filename)
    return;

  // diff extraction
  if (isoption(*(pstate->poptions), OPTION_DIFF) && pstate->st.back() != DIFF_COMMON && pstate->st.back() != pstate->status)
    return;

  pstate->pprocess->comments(ctx, ch);
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

  // see if we have an undetected srcDiff
  bool founddiff = false;
  for (int diffpos = 1; diffpos < ns_length; diffpos += 2) {
    if (pstate->root.namespaces[diffpos] && strcmp((const char*) pstate->root.namespaces[diffpos], "http://www.sdml.info/srcDiff") == 0) {
      founddiff = true;
      break;
    }
  }

  // potential issues with specification of option for diff, but no namespace, and vice-versa
  if (isoption(*(pstate->poptions), OPTION_DIFF) && !founddiff) {

    // found option_diff specified, but no namespace
    //fprintf(stderr, "srcml2src warning:  option --revision used on a document with no srcDiff namespace declared\n");

  } else if (!isoption(*(pstate->poptions), OPTION_DIFF) && founddiff) {

    // found no option_diff specified, but diff namespace
    //fprintf(stderr, "srcml2src warning:  option --revision not used on a document with srcDiff namespace declared\n");

  }

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

  setRootAttributes(pstate->root.attributes, pstate->root.nb_attributes);

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

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // so we have an element inside of the unit
  pstate->rootonly = false;

  // see if this is really a nested unit.  If not, then we have an individual
  // unit (not a srcML archive) and need to process the cached root
  pstate->isarchive = !isoption(pstate->pprocess->getOptions(), OPTION_XSLT_ALL) && strcmp((const char*) localname, "unit") == 0 && strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0;
  if (!pstate->isarchive) {

    pstate->count = 0;

    // setup for diff tracking
    diff_filename = true;
    if (isoption(*(pstate->poptions), OPTION_DIFF)) {
      diff_filename = setupDiff(pstate, pstate->root.nb_namespaces, pstate->root.namespaces, pstate->root.nb_attributes, pstate->root.attributes);
    }

    // should have made this call earlier, makeup for it now
    if (diff_filename) {

      pstate->count = 1;

      pstate->pprocess->startUnit(ctx, pstate->root.localname, pstate->root.prefix, pstate->root.URI, pstate->root.nb_namespaces,
                                  pstate->root.namespaces, pstate->root.nb_attributes, pstate->root.nb_defaulted, pstate->root.attributes);

      // all done
      if (pstate->stop)
        return;

      // output cached characters if we found any
      if (!pstate->firstcharacters.empty())
        charactersUnit(ctx, BAD_CAST pstate->firstcharacters.c_str(), pstate->firstcharacters.length());

      // all done
      if (pstate->stop)
        return;

    }

    // process using the normal startElementNs
    startElementNsUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);

    // next state is to copy the unit contents, finishing when needed
    ctxt->sax->startElementNs = &startElementNsUnit;
    ctxt->sax->characters = &charactersUnit;
    ctxt->sax->cdataBlock = &cdatablockUnit;
    ctxt->sax->ignorableWhitespace = &charactersUnit;
    ctxt->sax->endElementNs = &endElementNs;
    ctxt->sax->comment = &commentUnit;

  } else {

    if (isoption(*(pstate->poptions), OPTION_DIFF)) {

      // get the proper version of this revision
      split_diff_attribute(pstate, UNIT_ATTRIBUTE_VERSION, pstate->root.nb_attributes, pstate->root.attributes);

      // get the proper dir of this revision
      split_diff_attribute(pstate, UNIT_ATTRIBUTE_DIRECTORY, pstate->root.nb_attributes, pstate->root.attributes);
    }

    // should have made this call earlier, makeup for it now
    pstate->pprocess->startRootUnit(ctx, pstate->root.localname, pstate->root.prefix, pstate->root.URI, pstate->root.nb_namespaces, pstate->root.namespaces, pstate->root.nb_attributes, pstate->root.nb_defaulted, pstate->root.attributes);

    // all done
    if (pstate->stop)
      return;

    // process using the normal startElementNs
    startElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
  }

  // all done
  if (pstate->stop)
    return;

  pstate->firstcharacters.clear();
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

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // so we did find another element in the root
  pstate->rootonly = false;

  // setup for diff tracking
  diff_filename = true;
  if (isoption(*(pstate->poptions), OPTION_DIFF)) {

    diff_filename = setupDiff(pstate, nb_namespaces, namespaces, nb_attributes, attributes);
  }

  if (diff_filename)
    ++(pstate->count);

  // call startUnit if I want to see all the units, or want to see this unit
  if (diff_filename && (pstate->unit == -1 || pstate->count == pstate->unit)) {

    // process the start of this unit
    pstate->pprocess->startUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);

    // next state is to copy the unit contents, finishing when needed
    ctxt->sax->startElementNs = &startElementNsUnit;
    ctxt->sax->characters = &charactersUnit;
    ctxt->sax->cdataBlock = &cdatablockUnit;
    ctxt->sax->ignorableWhitespace = &charactersUnit;
    ctxt->sax->endElementNs = &endElementNs;
    ctxt->sax->comment = &commentUnit;

  } else {

    // we are going to skip processing this element
    ctxt->sax->startElementNs = 0;
    ctxt->sax->characters = 0;
    ctxt->sax->cdataBlock = 0;
    ctxt->sax->comment = 0;
    ctxt->sax->ignorableWhitespace = 0;
    ctxt->sax->endElementNs = &endElementNsSkip;
  }
}

void SAX2ExtractUnitsSrc::endElementNsUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  if (isoption(*(pstate->poptions), OPTION_DIFF) && (strcmp((const char*) URI, "http://www.sdml.info/srcDiff") == 0 && (
                                                                                                                        strcmp((const char*) localname, "insert") == 0
                                                                                                                        || strcmp((const char*) localname, "delete") == 0
                                                                                                                        || strcmp((const char*) localname, "common") == 0))) {

    pstate->st.pop_back();
    return;
  }

  // diff extraction
  if (isoption(*(pstate->poptions), OPTION_DIFF) && pstate->st.back() != DIFF_COMMON && pstate->st.back() != pstate->status)
    return;

  pstate->pprocess->endElementNs(ctx, localname, prefix, URI);
}

// end unit element and current file/buffer (started by startElementNs
void SAX2ExtractUnitsSrc::endElementNsSkip(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  if (pstate->isarchive && ctxt->nameNr == 1) {

    pstate->pprocess->endRootUnit(ctx, localname, prefix, URI);
    return;
  }

  if (!pstate->rootonly && ctxt->nameNr != (pstate->isarchive ? 2 : 1))
    return;

  // got here without ever seeing a nested element of any kind,
  // so have to do the whole process
  if (pstate->rootonly) {

    // setup for diff tracking
    diff_filename = true;
    if (isoption(*(pstate->poptions), OPTION_DIFF)) {

      diff_filename = setupDiff(pstate, pstate->root.nb_namespaces, pstate->root.namespaces, pstate->root.nb_attributes, pstate->root.attributes);
    }

    // process unit
    if (diff_filename) {

      // should have made this call earlier, makeup for it now
      pstate->pprocess->startUnit(ctx, pstate->root.localname, pstate->root.prefix,
                                  pstate->root.URI, pstate->root.nb_namespaces,
                                  pstate->root.namespaces, pstate->root.nb_attributes,
                                  pstate->root.nb_defaulted, pstate->root.attributes);

      // all done
      if (pstate->stop)
        return;

      // first characters
      if (!pstate->firstcharacters.empty())
        charactersUnit(ctx, BAD_CAST pstate->firstcharacters.c_str(), pstate->firstcharacters.length());
      pstate->firstcharacters.clear();

      // all done
      if (pstate->stop)
        return;

      // end the unit
      pstate->pprocess->endUnit(ctx, localname, prefix, URI);
      return;
    }
  }

  // process the end of this unit
  if (diff_filename && (pstate->unit == -1 || (!pstate->isarchive && pstate->unit == 0) || pstate->count == pstate->unit))
    pstate->pprocess->endUnit(ctx, localname, prefix, URI);

  // done if we are only stopping on this one
  if (pstate->count == pstate->unit) {

    stopUnit(ctx);
    return;
  }

  // all done
  if (pstate->stop)
    return;

  // now waiting for start of next unit
  ctxt->sax->startElementNs = &startElementNs;
  ctxt->sax->characters = 0;
}

// end unit element and current file/buffer (started by startElementNs
void SAX2ExtractUnitsSrc::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // only process nested unit start elements
  if (ctxt->nameNr > (pstate->isarchive ? 2 : 1)) {
    endElementNsUnit(ctx, localname, prefix, URI);
    return;
  }

  endElementNsSkip(ctx, localname, prefix, URI);
}

// escape control character elements
void SAX2ExtractUnitsSrc::startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                             int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                             const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  // skipping when in deleted or inserted file
  if (!diff_filename)
    return;

  if (isoption(*(pstate->poptions), OPTION_DIFF) && strcmp((const char*) URI, "http://www.sdml.info/srcDiff") == 0) {

    if (strcmp((const char*) localname, "delete") == 0) {

      pstate->st.push_back(DIFF_OLD);
      return;
    } else if (strcmp((const char*) localname, "insert") == 0) {

      pstate->st.push_back(DIFF_NEW);
      return;
    } else if (strcmp((const char*) localname, "common") == 0) {

      pstate->st.push_back(DIFF_COMMON);
      return;
    }
  }

  // diff extraction
  if (isoption(*(pstate->poptions), OPTION_DIFF) && pstate->st.back() != DIFF_COMMON && pstate->st.back() != pstate->status)
    return;

  pstate->pprocess->startElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
}

// stop all processing
void SAX2ExtractUnitsSrc::stopUnit(void* ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  ctxt->sax->startElementNs = 0;
  ctxt->sax->characters = 0;
  ctxt->sax->cdataBlock = 0;
  ctxt->sax->comment = 0;
  ctxt->sax->ignorableWhitespace = 0;
  ctxt->sax->endElementNs = 0;
  xmlStopParser(ctxt);

  pstate->stop = true;
}

// split the attribute according to the revision of the diff
int split_diff_attribute(SAX2ExtractUnitsSrc* pstate, const char* attribute_name, int& nb_attributes, const xmlChar** attributes) {

  int attr_index = find_attribute_index(nb_attributes, attributes, attribute_name);
  if (attr_index != -1) {

    for (char* pc = (char*) attributes[attr_index + 3]; pc < (char*) attributes[attr_index + 4]; ++pc)
      if (*pc == '|') {
        *pc = 0;
        if (pstate->status == pstate->DIFF_NEW)
          attributes[attr_index + 3] = (const xmlChar*) (pc + 1);
        else
          attributes[attr_index + 4] = (const xmlChar*) pc;
        break;
      }
  }

  return attr_index;
}

// setup the attributes and namespaces for a revision extraction
bool setupDiff(SAX2ExtractUnitsSrc* pstate,
               int& nb_namespaces, const xmlChar** namespaces,
               int& nb_attributes, const xmlChar** attributes) {

  pstate->st.clear();
  pstate->st.push_back(pstate->DIFF_COMMON);

  // get the proper filename for this revision
  int filename_index = split_diff_attribute(pstate, UNIT_ATTRIBUTE_FILENAME, nb_attributes, attributes);

  // gotta have a filename with srcDiff
  if (filename_index == -1)
    return false;

  // make sure the filename is not blank (indicating there is no file for this revision)
  if (attributes[filename_index + 3][0] == '\0' || attributes[filename_index + 3] == attributes[filename_index + 4])
    return false;

  // get the proper version of this revision
  split_diff_attribute(pstate, UNIT_ATTRIBUTE_VERSION, nb_attributes, attributes);

  // get the proper dir of this revision
  split_diff_attribute(pstate, UNIT_ATTRIBUTE_DIRECTORY, nb_attributes, attributes);

  // clear out the diff namespace from the array before passing it on
  bool found = false;
  int deccount = 0;
  for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2) {

    const char* uri = (const char*) namespaces[index + 1];

    if (!found && strcmp(uri, "http://www.sdml.info/srcDiff") == 0) {

      found = true;
      deccount = 1;
    } else if (found) {
      namespaces[index - 2] = namespaces[index];
      namespaces[index + 1 - 2] = namespaces[index + 1];
    }
  }
  nb_namespaces -= deccount;

  return true;
}
