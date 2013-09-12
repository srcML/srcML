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

  srcml_archive * archive = srcml_create_archive();

  /* 
     srcml_set_language
   */

  {

    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, 0);
    assert(unit->language == 0);
    srcml_free_unit(unit);
  }

  {

    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "foo");
    assert(*unit->language == "foo");
    srcml_free_unit(unit);
  }

  {
    assert(srcml_unit_set_language(0, "foo") == SRCML_STATUS_ERROR);
  }

  /* 
     srcml_set_filename
   */

  {

    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_filename(unit, 0);
    assert(unit->filename == 0);
    srcml_free_unit(unit);
  }

  {

    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_filename(unit, "foo");
    assert(*unit->filename == "foo");
    srcml_free_unit(unit);
  }

  {
    assert(srcml_unit_set_filename(0, "foo") == SRCML_STATUS_ERROR);
  }

  /* 
     srcml_set_directory
   */

  {

    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_directory(unit, 0);
    assert(unit->directory == 0);
    srcml_free_unit(unit);
  }

  {

    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_directory(unit, "foo");
    assert(*unit->directory == "foo");
    srcml_free_unit(unit);
  }

  {
    assert(srcml_unit_set_directory(0, "foo") == SRCML_STATUS_ERROR);
  }

  /* 
     srcml_set_version
   */

  {

    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_version(unit, 0);
    assert(unit->version == 0);
    srcml_free_unit(unit);
  }

  {

    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_version(unit, "foo");
    assert(*unit->version == "foo");
    srcml_free_unit(unit);
  }

  {
    assert(srcml_unit_set_version(0, "foo") == SRCML_STATUS_ERROR);
  }

  srcml_free_archive(archive);

  return 0;
}
