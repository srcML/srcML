/*
  test_Options.cpp

  Unit tests for Options.

 */

#include <Options.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>

int main(int argc, char * argv[]) {

  /*
    isoption
   */ 

  {
    assert(isoption(1 | 2, 1));
  }

  {
    assert(!isoption(1 | 2, 4));
  }

  return 0;
}
