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

#include <dassert.hpp>

int main(int, char* argv[]) {

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_language(archive), 0);
        dassert(srcml_archive_get_xml_encoding(archive), 0);
        dassert(srcml_archive_get_src_encoding(archive), 0);
        dassert(srcml_archive_get_url(archive), 0);
        dassert(srcml_archive_get_version(archive), 0);
        dassert(srcml_archive_get_tabstop(archive), 8);
        // @todo Does 1 namespace make sense by default?
        dassert(srcml_archive_get_namespace_size(archive), 1);
//        fprintf(stderr, "DEBUG:  %s %s %d srcml_archive_get_namespace_uri(archive, 1): %d\n", __FILE__,  __FUNCTION__, __LINE__,  (int) srcml_archive_get_namespace_uri(archive, 1));

//        dassert(srcml_archive_get_namespace_uri(archive, 1), std::string("http://www.srcML.org/srcML/src"));
//        dassert(archive->attributes.size(), 0);

//        dassert(archive->options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL 
//                                   | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_HASH));

//        dassert(archive->tabstop, 8);
//        dassert(archive->namespaces.size(), 1);
//        dassert(!archive->registered_languages.size(), 0);

//        dassert(archive->namespaces[0].uri, SRCML_SRC_NS_URI);
//        dassert(archive->namespaces[0].prefix, SRCML_SRC_NS_DEFAULT_PREFIX);
////        dassert(archive->namespaces[0].flags, NS_REQUIRED | NS_ROOT);

//        dassert(archive->user_macro_list.size(), 0);

//        dassert(archive->transformations.size(), 0);
//        assert(archive->context.empty());
//        assert(!archive->revision_number.is_initialized());

 //       srcml_archive_free(archive);
    }

    return 0;
}
