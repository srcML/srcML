#ifndef INCLUDED_SAX2UTILITIES_H
#define INCLUDED_SAX2UTILITIES_H

#include <string>
#include <list>
#include <libxml/parser.h>

typedef std::list<std::pair<std::string, std::string> > PROPERTIES_TYPE;

// expanded prefix for xmlns declaration
const char* xmlnsprefix(const char* prefix);

const char* qname(const char* prefix, const char* localname);

PROPERTIES_TYPE::const_iterator find(const PROPERTIES_TYPE& pv, const char* name);

PROPERTIES_TYPE::iterator find(PROPERTIES_TYPE& pv, const char* name);

// collect attributes
void collect_attributes(int nb_attributes, const xmlChar** attributes,
			PROPERTIES_TYPE& attrv);

// collect namespaces
void collect_namespaces(int nb_namespaces, const xmlChar** namespaces,
			PROPERTIES_TYPE& nsv);
#endif

