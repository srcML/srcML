/*
  test_srcml_list.cpp

  Copyright (C) 2013  SDML (www.sdml.info)

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*

  Test cases for srcml_list
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
#include <unistd.h>

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

  unlink("project_one.xml");
  unlink("project_two.xml");

  return 0;

}
