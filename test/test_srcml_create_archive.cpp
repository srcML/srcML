/*

  Test cases for srcml_create_archive.
 */
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>

int main(int argc, char * argv[]) {

  {

    srcml_archive * archive = srcml_create_archive();
    assert(archive->type == SRCML_ARCHIVE_INVALID);
    assert(archive->filename == 0);
    assert(archive->encoding == 0);
    assert(archive->xml_encoding == 0);
    assert(archive->language == 0);
    assert(archive->directory == 0);
    assert(archive->version == 0);
    assert(archive->attributes.size() == 0);

    assert(archive->options == 0);

    assert(archive->tabstop == 0);
    assert(archive->prefixes.size() == 7);
    assert(archive->namespaces.size() == 7);
    assert(archive->registered_languages.size() != 0);

    assert(archive->translator == 0);
    assert(archive->reader == 0);
    assert(archive->input == 0);

    assert(archive->transformations.size() == 0);
  }

  return 0;
}
