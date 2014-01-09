/*
  test_srcml_list.cpp

  Copyright (C) 2013  SDML (www.srcML.org)

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
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

#include "dassert.hpp"

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
    dassert(!list, 0);
    dassert(list[0], std::string("a.cpp"));
    dassert(list[1], 0);
    for(int i = 0; list[i]; ++i)
      free((void *)list[i]);
    free((void *)list);

  }

  {

    const char ** list = srcml_list("project_two.xml");
    dassert(!list, 0);
    dassert(list[0], std::string("a.cpp"));
    dassert(list[1], std::string("b.cpp"));
    dassert(list[2], std::string("c.cpp"));
    dassert(list[3], 0);
    for(int i = 0; list[i]; ++i)
      free((void *)list[i]);
    free((void *)list);
  }

  {

    const char ** list = srcml_list(0);
    dassert(list, 0);

  }

  unlink("project_one.xml");
  unlink("project_two.xml");

  srcml_cleanup_globals();

  return 0;

}
