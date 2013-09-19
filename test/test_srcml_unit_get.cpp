/*
  test_srcml_unit_get.cpp

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

  Test cases for srcml_unit_get_*.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  srcml_archive * archive = srcml_create_archive();

  /* 
     srcml_unit_get_language
   */

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->language = 0;
    assert(srcml_unit_get_language(unit) == 0);
    srcml_free_unit(unit);
  }

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->language = new std::string("foo");
    assert(srcml_unit_get_language(unit) == std::string("foo"));
    srcml_free_unit(unit);
  }

  {
    assert(srcml_unit_get_language(0) == 0);
  }

  /* 
     srcml_unit_get_filename
   */

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->filename = 0;
    assert(srcml_unit_get_filename(unit) == 0);
    srcml_free_unit(unit);
  }

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->filename = new std::string("foo");
    assert(srcml_unit_get_filename(unit) == std::string("foo"));
    srcml_free_unit(unit);
  }

  {
    assert(srcml_unit_get_filename(0) == 0);
  }

  /* 
     srcml_unit_get_directory
   */

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->directory = 0;
    assert(srcml_unit_get_directory(unit) == 0);
    srcml_free_unit(unit);
  }

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->directory = new std::string("foo");
    assert(srcml_unit_get_directory(unit) == std::string("foo"));
    srcml_free_unit(unit);
  }

  {
    assert(srcml_unit_get_directory(0) == 0);
  }

  /* 
     srcml_unit_get_version
   */

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->version = 0;
    assert(srcml_unit_get_version(unit) == 0);
    srcml_free_unit(unit);
  }

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->version = new std::string("foo");
    assert(srcml_unit_get_version(unit) == std::string("foo"));
    srcml_free_unit(unit);
  }

  {
    assert(srcml_unit_get_version(0) == 0);
  }

  /* 
     srcml_unit_get_xml
   */

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->unit = 0;
    assert(srcml_unit_get_xml(unit) == 0);
    srcml_free_unit(unit);
  }

  {

    srcml_unit * unit = srcml_create_unit(archive);
    unit->unit = new std::string("<unit/>");
    assert(srcml_unit_get_xml(unit) == std::string("<unit/>"));
    srcml_free_unit(unit);
  }

  {
    assert(srcml_unit_get_xml(0) == 0);
  }

  srcml_free_archive(archive);

  return 0;
}
