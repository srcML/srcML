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

    //assert(archive->options == 0);

    int tabstop;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    std::vector<pair> registered_languages;

    assert(archive->translator == 0);
    assert(archive->reader == 0);
    assert(archive->input == 0);

    assert(archive->transformations.size() = 0);
  }

  return 0;
}
