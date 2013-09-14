/*

  Test cases for srcml_list
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  /* 
     srcml_list
   */

  const char * srcml_one = "<unit filename=\"a.cpp\"></unit>";
  const char * srcml_one = "<unit><unit filename=\"a.cpp\"></unit><unit filename=\"b.cpp\"></unit><unit filename=\"c.cpp\"></unit></unit>";

  std::ofstream out_one("project_one.xml");
  out_one << srcml_one;
  out_one.close();
  std::ofstream out_two("project_two.xml");
  out_two << srcml_two;
  out_two.close();

  {

    srcml_list("project_one.xml");

  }

  return 0;

}
