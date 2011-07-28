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

  fprintf(stderr, "%s\n\n", __FUNCTION__);
}

void endDocument(void* ctx) {

  fprintf(stderr, "%s\n\n", __FUNCTION__);
}

void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                    const xmlChar** attributes) {

  if(prefix)
    fprintf(stderr, "%s:\t%s:%s\t%s\n", __FUNCTION__, prefix, localname, URI);
  else
    fprintf(stderr, "%s:\t%s\t%s\n", __FUNCTION__, localname, URI);

  if(nb_namespaces) {
    fprintf(stderr, "\t\tNamespaces:\n");

    for(int i = 0; i < nb_namespaces; i +=2)
      if(namespaces[i])
        fprintf(stderr, "\t\t\t%d. %s:%s\n", (i + 1) / 2 + 1, namespaces[i], namespaces[i + 1]);
      else
        fprintf(stderr, "\t\t\t%d. %s\n", (i + 1) / 2 + 1, namespaces[i + 1]);

  }

  if(nb_attributes) {
    fprintf(stderr, "\t\tAttributes:\n");

    for(int i = 0; i < nb_attributes; ++i)
      if(attributes[i * 5 + 1])
        fprintf(stderr, "\t\t\t%d. %s:%s=%s:%s\n", i + 1, attributes[i * 5 + 1], attributes[i * 5], attributes[i * 5 + 3], attributes[i * 5 + 4]);
      else
        fprintf(stderr, "\t\t\t%d. %s=%s\n", i + 1, attributes[i * 5], attributes[i * 5 + 4]);

  }

}

void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  fprintf(stderr, "%s\n\n", __FUNCTION__);
}

void characters(void* ctx, const xmlChar* ch, int len) {

  fprintf(stderr, "%s\n\n", __FUNCTION__);
}
