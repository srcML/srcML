#include "SAX2Utilities.h"
#include <cstring>
#include <iostream>

// expanded prefix for xmlns declaration
const char* xmlnsprefix(const char* prefix) {

      if (prefix) {
	static char xmlns[256] = "xmlns:";
	strcpy(xmlns + 6, (const char*) prefix);

	return xmlns;

      } else

	return "xmlns";
}

const char* qname(const char* prefix, const char* localname) {

      if (prefix) {
	static char tag[256];
	strcpy(tag, prefix);
	strcat(tag, ":");
	strcat(tag, localname);
	return tag;
      } else
	return localname;
}

PROPERTIES_TYPE::const_iterator find(const PROPERTIES_TYPE& pv, const char* name) {

  for (PROPERTIES_TYPE::const_iterator pos = pv.begin(); pos != pv.end(); ++pos)
    if (pos->first == name)
      return pos;

  return pv.end();
}

PROPERTIES_TYPE::iterator find(PROPERTIES_TYPE& pv, const char* name) {

  for (PROPERTIES_TYPE::iterator pos = pv.begin(); pos != pv.end(); ++pos)
    if (pos->first == name)
      return pos;

  return pv.end();
}

// collect attributes
void collect_attributes(int nb_attributes, const xmlChar** attributes, PROPERTIES_TYPE& attrv) {

    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {

      const char* name = qname((const char*) attributes[index + 1], (const char*) attributes[index]);
      PROPERTIES_TYPE::iterator pos = find(attrv, name);
      if (pos == attrv.end())
	pos = attrv.insert(attrv.end(), PROPERTIES_TYPE::value_type(name, ""));

      pos->second.assign((const char*) attributes[index + 3], (const char*)  attributes[index + 4]);
    }
}

// collect namespaces
void collect_namespaces(int nb_namespaces, const xmlChar** namespaces, PROPERTIES_TYPE& nsv) {

  for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2) {

    const char* uri = (const char*) namespaces[index + 1];
    PROPERTIES_TYPE::iterator pos = find(nsv, uri);
    if (pos == nsv.end())
      pos = nsv.insert(nsv.end(), PROPERTIES_TYPE::value_type(uri, ""));

    pos->second = xmlnsprefix((const char*) namespaces[index]);
  }
}
