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
