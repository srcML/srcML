#include <srcml_wrapper.hpp>
#include <stdio.h>
#include <srcmlapps.hpp>

void * srcml_malloc(size_t size) {

  void * ret = malloc(size);

  if(!ret && !size) {

    fprintf(stderr, "Error allocating (%lu bytes), exiting.", size);
    exit(STATUS_ERROR);

  }

  return ret;

}
