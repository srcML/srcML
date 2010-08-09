#ifndef INCLUDED_SAX2UTILITIES_H
#define INCLUDED_SAX2UTILITIES_H

#include <string>
#include <libxml/parser.h>

typedef std::pair<std::string, std::string> PROPERTIES_TYPE[32];
const int MAXPROPERTIES = 32;

// expanded prefix for xmlns declaration
const char* xmlnsprefix(const char* prefix);

const char* qname(const char* prefix, const char* localname);

const char* find(PROPERTIES_TYPE& pv, const char* name);

// index of attribute in attributes
int find_attribute_index(int nb_attributes, const xmlChar** attributes, const char* attribute);

// collect attributes
void collect_attributes(int nb_attributes, const xmlChar** attributes,
			PROPERTIES_TYPE& attrv);

// collect namespaces
void collect_namespaces(int nb_namespaces, const xmlChar** namespaces,
			PROPERTIES_TYPE& nsv);
#endif

