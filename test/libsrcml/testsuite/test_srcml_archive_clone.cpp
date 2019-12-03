/**
 * @file test_srcml_sax2_reader.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*

  Test cases for srcml_archive_create.
*/

#include <srcml.h>

#include <dassert.hpp>

int main(int argc, char* argv[]) {

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_src_encoding(archive, "e");
        srcml_archive_set_url(archive, "u");
        srcml_archive_set_version(archive, "v");
        srcml_archive_set_options(archive, 1 | 2);
        srcml_archive_set_tabstop(archive, 4);
        srcml_append_transform_xpath(archive, "//src:unit");
        srcml_archive_register_file_extension(archive, "foo", "bar");
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_register_namespace(archive, "foo", "bar");

        srcml_archive* new_archive = srcml_archive_clone(archive);
        dassert(srcml_archive_get_src_encoding(new_archive), std::string("e"));
        dassert(srcml_archive_get_url(new_archive), std::string("u"));
        dassert(srcml_archive_get_version(new_archive), std::string("v"));
        dassert(srcml_archive_get_options(new_archive), (1 | 2));
        dassert(srcml_archive_get_tabstop(new_archive), 4);
        dassert(srcml_archive_get_namespace_prefix(new_archive, 0), std::string("s"));
        dassert(srcml_archive_get_namespace_uri(new_archive, 0), std::string("http://www.srcML.org/srcML/src"));
        dassert(srcml_archive_get_namespace_size(new_archive), srcml_archive_get_namespace_size(archive));
        dassert(srcml_archive_get_namespace_prefix(new_archive, srcml_archive_get_namespace_size(new_archive) - 1),
            std::string("foo"));
        dassert(srcml_archive_get_namespace_uri(new_archive, srcml_archive_get_namespace_size(new_archive) - 1),
            std::string("bar"));

        srcml_archive_free(archive);
        srcml_archive_free(new_archive);
    }

    {
        dassert(srcml_archive_clone(0), 0);
    }

    return 0;
}
