/*

  Test cases for srcml_archive_set_*.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  /* 
     srcml_get_encoding
   */

  {
    srcml_set_encoding(0);
    assert(srcml_get_encoding() == 0);
  }

  {
    srcml_set_encoding("foo");
    assert(srcml_get_encoding() == std::string("foo"));
  }

  return 0;

}
