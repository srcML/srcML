/*
  srcml_wrapper.hpp

  srcML wrapper functions.

  Michael J. Decker
  mjd52@zips.uakron.edu
*/

#include <sys/types.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <SAX2ExtractUnitsSrc.hpp>

#ifndef INCLUDED_SRCML_WRAPPER_HPP
#define INCLUDED_SRCML_WRAPPER_HPP

void * srcml_malloc(size_t size);
void * srcml_malloc_sax(size_t size, xmlParserCtxtPtr ctxt);

char * srcml_strdup(const char * s1);
char * srcml_strdup_sax(const char * s1, xmlParserCtxtPtr ctxt);

#endif
