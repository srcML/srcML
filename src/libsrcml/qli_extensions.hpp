#ifndef QLI_EXTENSION_FUNCTIONS_HPP
#define QLI_EXTENSION_FUNCTIONS_HPP

#include <libxml/parser.h>
#include <libxml/xpathInternals.h>

void add_element(xmlXPathParserContext*, int);

void clear_elements(xmlXPathParserContext*, int);

void is_valid_element(xmlXPathParserContext*, int);

#endif
