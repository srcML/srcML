/*
  srcpatheval.h

  XPath expression evaluator on compound srcML documents
*/

#ifndef INCLUDED_SRCPATHEVAL_H
#define INCLUDED_SRCPATHEVAL_H

#include <libxml/xmlreader.h>

int srcpatheval(const char* context_element, const char* xpath, xmlTextReaderPtr reader, const char* ofilename);

#endif
