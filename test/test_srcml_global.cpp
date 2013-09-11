/*

  Test cases for srcml_archive_set_*.
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
     srcml_get_encoding
   */

  {
    global_archive.encoding = 0;
    assert(srcml_get_encoding() == 0);
  }

  {
    global_archive.encoding = new std::string("foo");
    assert(srcml_get_encoding() == std::string("foo"));
    delete global_archive.encoding;
  }

  /* 
     srcml_get_language
   */

  {
    global_archive.language = 0;
    assert(srcml_get_language() == 0);
  }

  {
    global_archive.language  = new std::string("foo");
    assert(srcml_get_language() == std::string("foo"));
    delete global_archive.encoding;
  }

  /* 
     srcml_get_filename
   */

  {
    global_archive.filename = 0;
    assert(srcml_get_filename() == 0);
  }

  {
    global_archive.filename  = new std::string("foo");
    assert(srcml_get_filename() == std::string("foo"));
    delete global_archive.encoding;
  }

  /* 
     srcml_get_directory
   */

  {
    global_archive.directory = 0;
    assert(srcml_get_directory() == 0);
  }

  {
    global_archive.directory  = new std::string("foo");
    assert(srcml_get_directory() == std::string("foo"));
    delete global_archive.encoding;
  }

  /* 
     srcml_get_version
   */

  {
    global_archive.version = 0;
    assert(srcml_get_version() == 0);
  }

  {
    global_archive.version  = new std::string("foo");
    assert(srcml_get_version() == std::string("foo"));
    delete global_archive.encoding;
  }

  /* 
     srcml_get_encoding
   */

  {
    srcml_set_encoding(0);
    assert(*global_archive.encoding == 0);
  }

  {
    srcml_set_encoding("foo");
    assert(*global_archive.encoding == "foo");
  }

  /* 
     srcml_get_language
   */

  {
    srcml_set_language(0);
    assert(*global_archive.language == 0);
  }

  {
    srcml_set_language("foo");
    assert(*global_archive.language == "foo");
  }

  /* 
     srcml_get_filename
   */

  {
    srcml_set_filename(0);
    assert(*global_archive.filename == 0);
  }

  {
    srcml_set_filename("foo");
    assert(*global_archive.filename == "foo");
  }

  /* 
     srcml_get_directory
   */

  {
    srcml_set_directory(0);
    assert(*global_archive.directory == 0);
  }

  {
    srcml_set_directory("foo");
    assert(*global_archive.directory == "foo");
  }

  /* 
     srcml_get_version
   */

  {
    srcml_set_version(0);
    assert(*global_archive.version == 0);
  }

  {
    srcml_set_version("foo");
    assert(*global_archive.version == "foo");
  }


  return 0;

}
