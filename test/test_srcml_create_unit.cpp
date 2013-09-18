/*
  test_srcml_create_unit.cpp

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

  Test cases for srcml_create_unit.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
   
    assert(unit->archive == archive);
    assert(unit->language == 0);
    assert(unit->filename == 0);
    assert(unit->directory == 0);
    assert(unit->version == 0);
    assert(unit->unit == 0);

    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    assert(srcml_create_unit(0) == 0);
   
  }

  return 0;
}
