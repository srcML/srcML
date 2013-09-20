/*
  test_srcml_create_archive.cpp

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
