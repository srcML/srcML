/**
 * @file test_srcml_archive_create.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*

  Test cases for srcml_archive_create.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

int main() {

    {

        srcml_archive * archive = srcml_archive_create();
        dassert(archive->type, SRCML_ARCHIVE_INVALID);
        dassert(archive->encoding, 0);
        dassert(archive->src_encoding, 0);
        dassert(archive->language, 0);
        dassert(archive->url, 0);
        dassert(archive->version, 0);
        dassert(archive->attributes.size(), 0);

        dassert(archive->options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL 
                                   | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_HASH));

        dassert(archive->tabstop, 8);
        dassert(archive->prefixes.size(), 1);
        dassert(archive->namespaces.size(), 1);
        dassert(!archive->registered_languages.size(), 0);

        dassert(archive->prefixes.at(0), SRCML_SRC_NS_PREFIX_DEFAULT);
        dassert(archive->namespaces.at(0), SRCML_SRC_NS_URI);

        dassert(archive->user_macro_list.size(), 0);

        dassert(archive->translator, 0);
        dassert(archive->reader, 0);
        dassert(archive->input, 0);

        dassert(archive->transformations.size(), 0);

        srcml_archive_free(archive);
    }

    return 0;
}
