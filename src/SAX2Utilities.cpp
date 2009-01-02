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

