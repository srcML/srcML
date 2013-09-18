/*
  test_srcml_global.cpp

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

  /* 
     srcml_check_extension
   */

  {
    srcml_register_file_extension("cpp", "C++");
    assert(srcml_check_extension("a.cpp") == std::string("C++"));
  }

  {
    assert(srcml_check_extension("a.cpp.gz") == std::string("C++"));
  }

  {
    assert(srcml_check_extension("a.foo") == 0);
  }

  {
    assert(srcml_check_extension(0) == 0);
  }

  /* 
     srcml_check_format
   */

  {
    assert(srcml_check_format("a.tar") != 0);
  }

  {
    assert(srcml_check_format("a.tar.bz2") != 0);
  }

  {
    assert(srcml_check_format("a.tar.gz") != 0);
  }

  {
    assert(srcml_check_format("a.tar.xz") != 0);
  }

  {
    assert(srcml_check_format("a.xz") != 0);
  }

  {
    assert(srcml_check_format("a.foo") == 0);
  }

  {
    assert(srcml_check_format(0) == 0);
  }

  /* 
     srcml_check_encoding
   */

  {
    assert(srcml_check_encoding("UTF-8") != 0);
  }

  {
    assert(srcml_check_encoding("foo") == 0);
  }

  {
    assert(srcml_check_encoding(0) == 0);
  }

  return 0;

}
