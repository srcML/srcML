/*

  Test cases for read open for archives
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  /* 
     srcml_read_open_filename
   */

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_filename(archive, "a.cpp.xml");

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_read_open_filename(archive, 0) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
  assert(srcml_read_open_filename(0, "a.cpp.xml") == SRCML_STATUS_ERROR);
  }

  return 0;

}
