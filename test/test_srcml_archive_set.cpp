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

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_encoding(archive, 0);
    assert(archive->encoding == 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_encoding(archive, "foo");
    assert(*archive->encoding == "foo");
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_language(archive, 0);
    assert(archive->language == 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_language(archive, "foo");
    assert(*archive->language == "foo");
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_filename(archive, 0);
    assert(archive->filename == 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_filename(archive, "foo");
    assert(*archive->filename == "foo");
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_directory(archive, 0);
    assert(archive->directory == 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_directory(archive, "foo");
    assert(*archive->directory == "foo");
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_version(archive, 0);
    assert(archive->version == 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_version(archive, "foo");
    assert(*archive->version == "foo");
    srcml_free_archive(archive);
  }


  return 0;
}
