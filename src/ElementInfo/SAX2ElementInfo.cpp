/*
  SAX2ElementInfo.cpp

  Michael J. Decker
  mjd52@zips.uakron.edu
*/

#include <stdio.h>
#include <string.h>
#include "SAX2ElementInfo.hpp"

xmlSAXHandler factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument = &startDocument;
  sax.endDocument = &endDocument;

  sax.startElementNs = &startElementNs;
  sax.endElementNs = &endElementNs;

  sax.characters = &characters;
  sax.comment = &comment;

  return sax;
}

void startDocument(void* ctx) {

  // fprintf(stderr, "%s\n\n", __FUNCTION__);
}

void endDocument(void* ctx) {

  // fprintf(stderr, "%s\n\n", __FUNCTION__);
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

    int index;
    for(int i = 0, index = 0; i < nb_namespaces; ++i, index += 2)
      if(namespaces[i])
        fprintf(stderr, "\t\t\t%d. %s:%s\n", (i + 1) / 2 + 1, namespaces[index], namespaces[index + 1]);
      else
        fprintf(stderr, "\t\t\t%d. %s\n", (i + 1) / 2 + 1, namespaces[index + 1]);

  }

  if(nb_attributes) {
    fprintf(stderr, "\t\tAttributes:\n");

    int index;
    for(int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {

      int end = attributes[index + 4] - attributes[index + 3];
      char * value = strndup((const char *)attributes[index + 3], end);

      if(attributes[index + 1])
        fprintf(stderr, "\t\t\t%d. %s:%s=%s", i + 1, attributes[index + 1], attributes[index], value);
      else
        fprintf(stderr, "\t\t\t%d. %s=%s", i + 1, attributes[index], value);

      if(attributes[index + 2])
        fprintf(stderr, "\t%s\n", attributes[index + 2]);
      else
        fprintf(stderr, "\n");

    }

  }

  fprintf(stderr, "\n");

}

void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  if(prefix)
    fprintf(stderr, "%s:\t%s:%s\t%s\n", __FUNCTION__, prefix, localname, URI);
  else
    fprintf(stderr, "%s:\t%s\t%s\n", __FUNCTION__, localname, URI);
}

void characters(void* ctx, const xmlChar* ch, int len) {

  // fprintf(stderr, "%s\n\n", __FUNCTION__);
}

void comments(void* ctx, const xmlChar* ch) {

  // fprintf(stderr, "%s\n\n", __FUNCTION__);
}
