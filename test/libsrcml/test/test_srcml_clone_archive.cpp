/*
  test_srcml_clone.cpp

  Copyright (C) 2013  SDML (www.srcML.org)

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

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
    archive->type = SRCML_ARCHIVE_READ;
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
    archive->translator = (srcMLTranslator *)1;
    archive->reader = (srcMLReader *)1;
    archive->input = (xmlParserInputBufferPtr)1;
    transform trans = {SRCML_XPATH, "//src:unit"};
    archive->transformations.push_back(trans);

    srcml_archive * new_archive = srcml_clone_archive(archive);

    assert(new_archive->type == SRCML_ARCHIVE_INVALID);
    assert(*new_archive->filename == "f");
    assert(*new_archive->encoding == "e");
    assert(new_archive->xml_encoding == 0);
    assert(*new_archive->language == "l");
    assert(*new_archive->directory == "d");
    assert(*new_archive->version == "v");
    assert(new_archive->attributes.size() == 2);
    assert(new_archive->attributes.at(0) == "a");
    assert(new_archive->attributes.at(1) == "a");

    assert(new_archive->options == (1 | 2));

    assert(new_archive->tabstop == 4);
    assert(new_archive->prefixes.size() == 8);
    assert(new_archive->namespaces.size() == 8);
    assert(new_archive->registered_languages.size() == archive->registered_languages.size());

    assert(new_archive->prefixes.at(0) == SRCML_SRC_NS_PREFIX_DEFAULT);
    assert(new_archive->namespaces.at(0) == SRCML_SRC_NS_URI);
    assert(new_archive->prefixes.at(1) == "c");
    assert(new_archive->namespaces.at(1) == SRCML_CPP_NS_URI);
    assert(new_archive->prefixes.at(2) == SRCML_ERR_NS_PREFIX_DEFAULT);
    assert(new_archive->namespaces.at(2) == SRCML_ERR_NS_URI);
    assert(new_archive->prefixes.at(3) == SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT);
    assert(new_archive->namespaces.at(3) == SRCML_EXT_LITERAL_NS_URI);
    assert(new_archive->prefixes.at(4) == SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT);
    assert(new_archive->namespaces.at(4) == SRCML_EXT_OPERATOR_NS_URI);
    assert(new_archive->prefixes.at(5) == SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT);
    assert(new_archive->namespaces.at(5) == SRCML_EXT_MODIFIER_NS_URI);
    assert(new_archive->prefixes.at(6) == SRCML_EXT_POSITION_NS_PREFIX_DEFAULT);
    assert(new_archive->namespaces.at(6) == SRCML_EXT_POSITION_NS_URI);
    assert(new_archive->prefixes.at(7) == "foo");
    assert(new_archive->namespaces.at(7) == "bar");

    assert(new_archive->translator == 0);
    assert(new_archive->reader == 0);
    assert(new_archive->input == 0);
    assert(new_archive->close_input == 0);

    assert(new_archive->transformations.size() == 0);

    archive->translator = 0;
    archive->reader = 0;
    archive->input = 0;
    srcml_free_archive(archive);
    srcml_free_archive(new_archive);
  }

  {

    assert(srcml_clone_archive(0) == 0);

  }

  return 0;
}
