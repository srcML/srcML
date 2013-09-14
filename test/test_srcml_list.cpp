/*

  Test cases for srcml_list
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  /* 
     srcml_list
   */

  const char * srcml_one = "<unit filename=\"a.cpp\"></unit>";
  const char * srcml_two = "<unit><unit filename=\"a.cpp\"></unit><unit filename=\"b.cpp\"></unit><unit filename=\"c.cpp\"></unit></unit>";

  std::ofstream out_one("project_one.xml");
  out_one << srcml_one;
  out_one.close();
  std::ofstream out_two("project_two.xml");
  out_two << srcml_two;
  out_two.close();

  {

    const char ** list = srcml_list("project_one.xml");
    assert(list != 0);
    assert(list[0] == std::string("a.cpp"));
    assert(list[1] == 0);

  }

  {

    const char ** list = srcml_list("project_two.xml");
    assert(list != 0);
    assert(list[0] == std::string("a.cpp"));
    assert(list[1] == std::string("b.cpp"));
    assert(list[2] == std::string("c.cpp"));
    assert(list[3] == 0);

  }

  {

    const char ** list = srcml_list(0);
    assert(list == 0);

  }

  return 0;

}
