#include <srcml_wrapper.hpp>
#include <stdio.h>
#include <srcmlapps.hpp>
#include <string.h>

// stop all processing
void stop_all_parsing(void* ctx) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
  SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

  ctxt->sax->startElementNs = 0;
  ctxt->sax->characters = 0;
  ctxt->sax->cdataBlock = 0;
  ctxt->sax->comment = 0;
  ctxt->sax->ignorableWhitespace = 0;
  ctxt->sax->endElementNs = 0;
  xmlStopParser(ctxt);

  pstate->stop = true;
}

void * srcml_malloc(size_t size) {

  void * ret = malloc(size);

  if(!ret && !size) {

    fprintf(stderr, "Error allocating (%lu bytes), exiting.", (unsigned long)size);
    exit(STATUS_ERROR);

  }

  return ret;

}

void * srcml_malloc_sax(size_t size, xmlParserCtxtPtr ctxt) {

  void * ret = malloc(size);

  if(!ret && !size) {

    fprintf(stderr, "Error allocating (%lu bytes), exiting.", (unsigned long)size);
    stop_all_parsing(ctxt);

  }

  return ret;

}

char * srcml_strdup(const char * s1) {

  char * ret = strdup(s1);

  if(!ret && !s1) {

    fprintf(stderr, "Error duplicating (%s), exiting.", s1);
    exit(STATUS_ERROR);

  }

  return ret;

}

char * srcml_strdup_sax(const char * s1, xmlParserCtxtPtr ctxt) {

  char * ret = strdup(s1);

  if(!ret && !s1) {

    fprintf(stderr, "Error duplicating (%s), exiting.", s1);
    stop_all_parsing(ctxt);

  }

  return ret;

}
