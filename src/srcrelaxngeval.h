/*
  srcrelaxngeval.h

  
*/

#ifndef INCLUDED_SRCRELAXNGEVAL_H
#define INCLUDED_SRCRELAXNGEVAL_H

#include <libxml/xmlreader.h>

int srcrelaxngeval(const char* xpath, xmlTextReaderPtr reader, const char* ofilename);

#endif
