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

  static std::string tag;

  if (prefix) {
    tag = prefix;
    tag += ':';
    tag += localname;
    return tag.c_str();
  } else
    return localname;
}

/*
PROPERTIES_TYPE::const_iterator find(const PROPERTIES_TYPE& pv, const char* name) {

  for (PROPERTIES_TYPE::const_iterator pos = pv.begin(); pos != pv.end(); ++pos)
    if (pos->first == name)
      return pos;

  return pv.end();
}
*/

const char* find(PROPERTIES_TYPE& pv, const char* name) {

  for (int i = 0; i < 32; ++i) {
    if (pv[i].first == "")
      return 0;

    if (pv[i].first == name)
      return pv[i].second.c_str();
  }

  return 0;
}

// collect attributes
void collect_attributes(int nb_attributes, const xmlChar** attributes, PROPERTIES_TYPE& attrv) {

  for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {

      const char* name = qname((const char*) attributes[index + 1], (const char*) attributes[index]);

      // look for it
      int i;
      for (i = 0; i < 32; ++i)
	if (attrv[i].first == "" || attrv[i].first == name)
	  break;

      // if new, then stick in the name
      if (attrv[i].first == "")
	attrv[i].first = name;

      // stick in the value
      attrv[i].second.assign((const char*) attributes[index + 3], (const char*)  attributes[index + 4]);
    }
}

// collect namespaces
void collect_namespaces(int nb_namespaces, const xmlChar** namespaces, PROPERTIES_TYPE& nsv) {

  for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2) {

    const char* uri = (const char*) namespaces[index + 1];

    // look for it
    int i;
    for (i = 0; i < 32; ++i)
      if (nsv[i].first == "" || nsv[i].first == uri)
	  break;

    // if new, then stick in the name
    if (nsv[i].first == "")
      nsv[i].first = uri;

    nsv[i].second = xmlnsprefix((const char*) namespaces[index]);
  }
}
