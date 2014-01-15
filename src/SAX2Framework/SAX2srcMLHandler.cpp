/*
  @file SAX2srcMLHandler.cpp

  Copyright (C) 2004-2013  SDML (www.sdml.info)

  This file is part of the srcML SAX2 Framework.

  The srcML SAX2 Framework is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML SAX2 Framework is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML SAX2 Framework; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <SAX2srcMLHandler.hpp>
#include <SAX2Framework_utilities.hpp>

#include <cstring>

/**
 * factory
 *
 * Create SAX handler.
 */
xmlSAXHandler factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument = &startDocument;
  sax.endDocument = &endDocument;

  sax.startElementNs = &startRoot;
  sax.endElementNs = &endElementNs;

  sax.characters = &charactersFirst;

  sax.comment = &comment;
  sax.cdataBlock = &cdataBlock;

  return sax;
}

/**
 * startDocument
 * @param ctx an xmlParserCtxtPtr
 *
 * SAX handler function for start of document.
 * Immediately calls supplied handlers function.
 */
void startDocument(void * ctx) {

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  state->process->init(ctxt);
  state->process->startDocument();

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

}

/**
 * endDocument
 * @param ctx an xmlParserCtxtPtr
 *
 * SAX handler function for end of document.
 * Calls endRoot if needed then
 * immediately calls supplied handlers function.
 */
void endDocument(void * ctx) {

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  if(ctxt->sax->startElementNs)
      state->process->endRoot(state->root.localname, state->root.prefix, state->root.URI);

  state->process->endDocument();

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

}

/**
 * startRoot
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of root element.
 * Caches root info and immediately calls supplied handlers function.
 */
void startRoot(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                           int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                    const xmlChar ** attributes) {

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  // need to record that we actually found something besides the root element
  //state->rootonly = true;

  // save all the info in case this is not a srcML archive
  state->root.localname = localname ? (xmlChar*) strdup((const char*) localname) : 0;
  CHECK_COPY(localname, state->root.localname);

  state->root.prefix = prefix ? (xmlChar*) strdup((const char*) prefix) : 0;
  CHECK_COPY(prefix, state->root.prefix);

  state->root.URI = URI ? (xmlChar*) strdup((const char*) URI) : 0;
  CHECK_COPY(URI, state->root.URI);

  state->root.nb_namespaces = nb_namespaces;
  int ns_length = nb_namespaces * 2;
  state->root.namespaces = (const xmlChar**) malloc(ns_length * sizeof(namespaces[0]));
  CHECK_COPY(namespaces, state->root.namespaces);
  memset(state->root.namespaces, 0, ns_length);

  for (int i = 0; i < ns_length; ++i) {
    state->root.namespaces[i] = namespaces[i] ? (xmlChar*) strdup((const char*) namespaces[i]) : 0;
    CHECK_COPY(namespaces[i], state->root.namespaces[i]);
  }

  state->root.nb_attributes = nb_attributes;
  state->root.nb_defaulted = nb_defaulted;

  int nb_length = nb_attributes * 5;
  state->root.attributes = (const xmlChar**) malloc(nb_length * sizeof(attributes[0]));
  CHECK_COPY(attributes, state->root.attributes);

  memset(state->root.attributes, 0, nb_length);

  for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
    state->root.attributes[index] = attributes[index] ? (xmlChar*) strdup((const char*) attributes[index]) : 0;
    CHECK_COPY(attributes[index], state->root.attributes[index]);
    state->root.attributes[index + 1] = attributes[index + 1] ? (xmlChar*) strdup((const char*) attributes[index + 1]) : 0;
    CHECK_COPY(attributes[index + 1], state->root.attributes[index + 1]);
    state->root.attributes[index + 2] = attributes[index + 2] ? (xmlChar*) strdup((const char*) attributes[index + 2]) : 0;
    CHECK_COPY(attributes[index + 2], state->root.attributes[index + 2]);

    int vallength = attributes[index + 4] - attributes[index + 3];
    state->root.attributes[index + 3] = (const xmlChar*) malloc(vallength);
    CHECK_COPY(attributes[index + 3], state->root.attributes[index + 3]);

    strncpy((char *) state->root.attributes[index + 3], (const char*) attributes[index + 3], vallength);
    state->root.attributes[index + 4] = state->root.attributes[index + 3] + vallength;
  }

  state->process->startRoot(localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
  // handle nested units
  ctxt->sax->startElementNs = &startElementNsFirst;

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

}

/**
 * startElementNsFirst
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of first element after root
 * Detects archive and acts accordingly.
 */
void startElementNsFirst(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                           int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                    const xmlChar ** attributes) {

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  int ns_length = state->root.nb_namespaces * 2;
  for (int i = 0; i < ns_length; i += 2)
    if(prefix && state->root.namespaces[i] && strcmp((const char *)state->root.namespaces[i], (const char *)prefix) == 0)
      prefix = state->root.namespaces[i];

  for (int i = 1; i < ns_length; i += 2)
    if(URI && state->root.namespaces[i] && strcmp((const char *)state->root.namespaces[i], (const char *)URI) == 0)
      URI = state->root.namespaces[i];

  state->is_archive = strcmp((const char *)localname, "unit") == 0;

  if(!state->is_archive) {

    state->process->startUnit(state->root.localname, state->root.prefix, state->root.URI,
                               state->root.nb_namespaces, state->root.namespaces, state->root.nb_attributes,
                               state->root.nb_defaulted, state->root.attributes);
    state->process->charactersUnit((const xmlChar *)state->root.characters.c_str(), state->root.characters.size());
    state->process->startElementNs(localname, prefix, URI,
                               nb_namespaces, namespaces, nb_attributes,
                               nb_defaulted, attributes);
  } else {

    state->process->charactersRoot((const xmlChar *)state->root.characters.c_str(), state->root.characters.size());
    state->process->startUnit(localname, prefix, URI,
                               nb_namespaces, namespaces, nb_attributes,
                               nb_defaulted, attributes);


  }

  ctxt->sax->startElementNs = &startElementNs;    
  ctxt->sax->characters = &charactersUnit;

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

}

/**
 * startUnit
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of an unit.
 * Immediately calls supplied handlers function.
 */
void startUnit(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                           int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                    const xmlChar ** attributes) {

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  int ns_length = state->root.nb_namespaces * 2;
  for (int i = 0; i < ns_length; i += 2)
    if(prefix && state->root.namespaces[i] && strcmp((const char *)state->root.namespaces[i], (const char *)prefix) == 0)
      prefix = state->root.namespaces[i];

  for (int i = 1; i < ns_length; i += 2)
    if(URI && state->root.namespaces[i] && strcmp((const char *)state->root.namespaces[i], (const char *)URI) == 0)
      URI = state->root.namespaces[i];

  state->process->startUnit(localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
  ctxt->sax->startElementNs = &startElementNs;    
  ctxt->sax->characters = &charactersUnit;

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

}

/**
 * startElementNs
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of an element.
 * Immediately calls supplied handlers function.
 */
void startElementNs(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                           int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                    const xmlChar ** attributes) {

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  int ns_length = state->root.nb_namespaces * 2;
  for (int i = 0; i < ns_length; i += 2)
    if(prefix && state->root.namespaces[i] && strcmp((const char *)state->root.namespaces[i], (const char *)prefix) == 0)
      prefix = state->root.namespaces[i];

  for (int i = 1; i < ns_length; i += 2)
    if(URI && state->root.namespaces[i] && strcmp((const char *)state->root.namespaces[i], (const char *)URI) == 0)
      URI = state->root.namespaces[i];

  state->process->startElementNs(localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

}

/**
 * endElementNs
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 *
 * SAX handler function for end of an element.
 * Detects end of unit and calls correct functions
 * for either endRoot endUnit or endElementNs.
 */
void endElementNs(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  if(strcmp((const char *)localname, "unit") == 0) {

    if(ctxt->sax->startElementNs == &startElementNsFirst) {

      state->process->startUnit(state->root.localname, state->root.prefix, state->root.URI,
                                state->root.nb_namespaces, state->root.namespaces, state->root.nb_attributes,
                                state->root.nb_defaulted, state->root.attributes);
      

    } 

    if(ctxt->sax->startElementNs == &startUnit) {

      state->process->endRoot(localname, prefix, URI);
      ctxt->sax->startElementNs = 0;    

    } else {

      state->process->endUnit(localname, prefix, URI);
      ctxt->sax->startElementNs = &startUnit;    
      ctxt->sax->characters = &charactersRoot;

    }

  } else
    state->process->endElementNs(localname, prefix, URI);

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

}

/**
 * charactersFirst
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling before we
 * know if we have an archive or not.
 * Immediately calls supplied handlers function.
 */
void charactersFirst(void * ctx, const xmlChar * ch, int len) {

#ifdef DEBUG
  std::string chars;
  chars.append((const char *)ch, len);
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  state->root.characters.append((const char *)ch, len);

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

}

/**
 * charactersRoot
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling at the root level.
 * Immediately calls supplied handlers function.
 */
void charactersRoot(void * ctx, const xmlChar * ch, int len) {

#ifdef DEBUG
  std::string chars;
  chars.append((const char *)ch, len);
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  state->process->charactersRoot(ch, len);

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

}

/**
 * charactersUnit
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling within a unit.
 * Immediately calls supplied handlers function.
 */
void charactersUnit(void * ctx, const xmlChar * ch, int len) {

#ifdef DEBUG
  std::string chars;
  chars.append((const char *)ch, len);
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  state->process->charactersUnit(ch, len);

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

}

/**
 * comment
 * @param ctx an xmlParserCtxtPtr
 * @param value the comment content
 *
 * A comment has been parsed.
 * Immediately calls supplied handlers function.
 */
void comment(void * ctx, const xmlChar * value) {

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  state->process->comment(value);

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

}

/**
 * cdataBlock
 * @param ctx an xmlParserCtxtPtr
 * @param value the pcdata content
 * @param len the block length
 *
 * Called when a pcdata block has been parsed.
 * Immediately calls supplied handlers function.
 */
void cdataBlock(void * ctx, const xmlChar * value, int len) {

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

  if(ctx == NULL) return;

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2srcMLHandler * state = (SAX2srcMLHandler *) ctxt->_private;

  state->process->cdataBlock(value, len);

#ifdef DEBUG
  fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

}

