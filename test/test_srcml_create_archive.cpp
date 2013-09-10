/*

  Test cases for srcml_create_archive.
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

    assert(archive->prefixes.at(0) == SRCML_SRC_NS_PREFIX_DEFAULT);
    assert(archive->namespaces.at(0) == SRCML_SRC_NS_URI);
    assert(archive->prefixes.at(1) == SRCML_CPP_NS_PREFIX_DEFAULT);
    assert(archive->namespaces.at(1) == SRCML_CPP_NS_URI);
    assert(archive->prefixes.at(2) == SRCML_ERR_NS_PREFIX_DEFAULT);
    assert(archive->namespaces.at(2) == SRCML_ERR_NS_URI);
    assert(archive->prefixes.at(3) == SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT);
    assert(archive->namespaces.at(3) == SRCML_EXT_LITERAL_NS_URI);
    assert(archive->prefixes.at(4) == SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT);
    assert(archive->namespaces.at(4) == SRCML_EXT_OPERATOR_NS_URI);
    assert(archive->prefixes.at(5) == SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT);
    assert(archive->namespaces.at(5) == SRCML_EXT_MODIFIER_NS_URI);
    assert(archive->prefixes.at(6) == SRCML_EXT_POSITION_NS_PREFIX_DEFAULT);
    assert(archive->namespaces.at(6) == SRCML_EXT_POSITION_NS_URI);

    assert(archive->translator == 0);
    assert(archive->reader == 0);
    assert(archive->input == 0);

    assert(archive->transformations.size() == 0);

    srcml_free_archive(archive);
  }

  return 0;
}
