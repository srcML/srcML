#include "SAX2Utilities.h"
#include <cstring>

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

// collect attributes
void collect_attributes(int nb_attributes, const xmlChar** attributes,
			std::map<std::string, std::string>& attrv) {

    // collect attributes
    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {

      const char* name = qname((const char*) attributes[index + 1], (const char*) attributes[index]);

      std::string value((const char*) attributes[index + 3], (const char*)  attributes[index + 4]);

      attrv[name] = value;
    }
}
