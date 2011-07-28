/*
  SAX2ElementInfo.cpp

  Michael J. Decker
  mjd52@zips.uakron.edu
*/

#include <stdio.h>
#include "SAX2ElementInfo.hpp"

xmlSAXHandler factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument = &startDocument;
  sax.endDocument = &endDocument;

  sax.startElementNs = &startElementNs;
  sax.endElementNs = &endElementNs;

  sax.characters = &characters;

  return sax;
}

void startDocument(void* ctx) {

  fprintf(stderr, "%s\n", __FUNCTION__);
}

void endDocument(void* ctx) {

  fprintf(stderr, "%s\n", __FUNCTION__);
}

void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                            int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                            const xmlChar** attributes) {

  fprintf(stderr, "%s\n", __FUNCTION__);
}

void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  fprintf(stderr, "%s\n", __FUNCTION__);
}

void characters(void* ctx, const xmlChar* ch, int len) {

  fprintf(stderr, "%s\n", __FUNCTION__);
}

void comments(void* ctx, const xmlChar* ch) {

  fprintf(stderr, "%s\n", __FUNCTION__);
}
