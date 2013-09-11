/*

  Test cases for global functions
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

extern srcml_archive global_archive;

int main(int argc, char * argv[]) {

  /* 
     srcml_check_language
   */

  {
    assert(srcml_check_language("C") == 1);
  }

  {
    assert(srcml_check_language("C++") == 2);
  }

  {
    assert(srcml_check_language("Java") == 8);
  }

  {
    assert(srcml_check_language("C#") == 32);
  }

  /* 
     srcml_language_list
   */

  {
    assert(srcml_language_list()[0] == std::string("C"));
  }

  {
    assert(srcml_language_list()[1] == std::string("C++"));
  }

  {
    assert(srcml_language_list()[2] == std::string("C#"));
  }

  {
    assert(srcml_language_list()[3] == std::string("Java"));
  }

  {
    assert(srcml_language_list()[4] == 0);
  }

  return 0;

}
