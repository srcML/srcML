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
    srcml_archive->type = SRCML_ARCHIVE_READ;
    archive->filename = new std::string ("f");
    archive->encoding = new std::string ("e");
    archive->language = new std::string ("l");
    archive->directory = new std::string ("d");
    archive->version = new std::string ("v");
    archive->attributes.push_back("a");
    archive->attributes.push_back("a");
    archive->options = 1 | 2;
    archive->tabstop = 4;
    archive->prefixes.at(1) = "c";
    archive->prefixes.push_back("foo");
    archive->namespaces.push_back("bar");
    srcml_archive_register_file_extension(archive, "foo", "bar");
    archive->translator = 1;
    archive->reader = 1;
    archive->input = 1;


    assert(archive->type == SRCML_ARCHIVE_INVALID);
    assert(*archive->filename == "f");
    assert(*archive->encoding == "e");
    assert(archive->xml_encoding == 0);
    assert(*archive->language == "l");
    assert(*archive->directory == "d");
    assert(*archive->version == "v");
    assert(archive->attributes.size() == 2);
    assert(archive->attributes.at(0) == "a");
    assert(archive->attributes.at(1) == "a");

    assert(archive->options == (1 | 2));

    assert(archive->tabstop == 4);
    assert(archive->prefixes.size() == 8);
    assert(archive->namespaces.size() == 8);
    assert(archive->registered_languages.size() == archive->registered_languages.size());

    assert(archive->prefixes.at(0) == SRCML_SRC_NS_PREFIX_DEFAULT);
    assert(archive->namespaces.at(0) == SRCML_SRC_NS_URI);
    assert(archive->prefixes.at(1) == "c");
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
    assert(archive->prefixes.at(7) == "foo");
    assert(archive->namespaces.at(7) == "bar");

    assert(archive->translator == 0);
    assert(archive->reader == 0);
    assert(archive->input == 0);

    assert(archive->transformations.size() == 0);

    archive->translator = 0;
    archive->reader = 0;
    archive->input = 0;
    srcml_free_archive(archive);
  }

  return 0;
}
