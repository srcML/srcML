/*

  Test cases for write open for archives
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
