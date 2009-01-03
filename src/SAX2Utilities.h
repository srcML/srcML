#ifndef INCLUDED_SAX2UTILITIES_H
#define INCLUDED_SAX2UTILITIES_H

#include <string>
#include <map>
#include <libxml/parser.h>

// expanded prefix for xmlns declaration
const char* xmlnsprefix(const char* prefix);

const char* qname(const char* prefix, const char* localname);

// collect attributes
void collect_attributes(int nb_attributes, const xmlChar** attributes,
			std::map<std::string, std::string>& attrv);

#endif

