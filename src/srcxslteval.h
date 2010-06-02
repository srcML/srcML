/*
  xpathproc

  A simple XPath expression evaluator.

  Usage:  xpathproc xpath-expression xml-file
*/

#ifndef INCLUDED_SRCXSLTEVAL_H
#define INCLUDED_SRCXSLTEVAL_H

#include <libxml/xmlreader.h>

//#include "SAX2UnitDOM.h"

int srcxslteval(const char* context, const char* xpath, xmlTextReaderPtr reader, const char* ofilename, const char* params[], int paramcount, int options);

#endif
