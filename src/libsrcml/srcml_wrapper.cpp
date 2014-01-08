#include <srcml_wrapper.hpp>
#include <stdio.h>
#include <srcmlapps.hpp>
#include <string.h>

void * srcml_malloc(size_t size) {

  void * ret = malloc(size);

  if(!ret && !size) {

    fprintf(stderr, "Error allocating (%lu bytes), exiting.", size);
    exit(STATUS_ERROR);

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
