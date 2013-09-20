/*
  test_srcml_archive_check_extension.cpp

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

  Test cases for srcml_archive_check_extension
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  /* 
     srcml_check_extension
   */

  srcml_archive * archive = srcml_create_archive();
  srcml_archive_register_file_extension(archive, "cpp", "C++");

  {
    assert(srcml_archive_check_extension(archive, "a.cpp") == std::string("C++"));
  }

  {
    assert(srcml_archive_check_extension(archive, "a.cpp.gz") == std::string("C++"));
  }

  {
    assert(srcml_archive_check_extension(archive, "a.foo") == 0);
  }

  {
    assert(srcml_archive_check_extension(archive, 0) == 0);
  }

  {
    assert(srcml_archive_check_extension(0, "a.cpp") == 0);
  }

  srcml_free_archive(archive);

  return 0;

}
