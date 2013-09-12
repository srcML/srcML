/*

  Test cases for srcml_archive_get_*.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  /* 
     srcml_archive_get_encoding
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->encoding = 0;
    assert(srcml_archive_get_encoding(archive) == 0);
    srcml_free_archive(archive);
  }

  {
    assert(srcml_archive_get_encoding(0) == 0);
  }


  {

    srcml_archive * archive = srcml_create_archive();
    archive->encoding = new std::string("foo");
    assert(srcml_archive_get_encoding(archive) == std::string("foo"));
    srcml_free_archive(archive);
  }

  /* 
     srcml_archive_get_language
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->language = 0;
    assert(srcml_archive_get_language(archive) == 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    archive->language = new std::string("foo");
    assert(srcml_archive_get_language(archive) == std::string("foo"));
    srcml_free_archive(archive);
  }

  {
    assert(srcml_archive_get_language(0) == 0);
  }

  /* 
     srcml_archive_get_filename
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->filename = 0;
    assert(srcml_archive_get_filename(archive) == 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    archive->filename = new std::string("foo");
    assert(srcml_archive_get_filename(archive) == std::string("foo"));
    srcml_free_archive(archive);
  }

  {
    assert(srcml_archive_get_filename(0) == 0);
  }

  /* 
     srcml_archive_get_directory
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->directory = 0;
    assert(srcml_archive_get_directory(archive) == 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    archive->directory = new std::string("foo");
    assert(srcml_archive_get_directory(archive) == std::string("foo"));
    srcml_free_archive(archive);
  }

  {
    assert(srcml_archive_get_directory(0) == 0);
  }

  /* 
     srcml_archive_get_version
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->version = 0;
    assert(srcml_archive_get_version(archive) == 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    archive->version = new std::string("foo");
    assert(srcml_archive_get_version(archive) == std::string("foo"));
    srcml_free_archive(archive);
  }

  {
    assert(srcml_archive_get_version(0) == 0);
  }

  /*
    srcml_archive_get_option
   */

  {

    srcml_archive * archive = srcml_create_archive();

    archive->options = 1;

    assert(srcml_archive_get_options(archive) == 1);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();

    archive->options = 1 | 2;

    assert(srcml_archive_get_options(archive) == (1 | 2));
    srcml_free_archive(archive);
  }

  {
    assert(srcml_archive_get_options(0) == 0);
  }

  /*
    srcml_archive_get_tabstop
  */

  {

    srcml_archive * archive = srcml_create_archive();

    archive->tabstop = 4;

    assert(srcml_archive_get_tabstop(archive) == 4);
    srcml_free_archive(archive);
  }

  {
    assert(srcml_archive_get_tabstop(0) == 0);
  }

  return 0;
}
