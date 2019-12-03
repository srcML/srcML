/**
 * @file test_srcml_archive_create.cpp
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

int main(int, char* argv[]) {

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_language(archive), 0);
        dassert(srcml_archive_get_xml_encoding(archive), 0);
        dassert(srcml_archive_get_src_encoding(archive), 0);
        dassert(srcml_archive_get_url(archive), 0);
        dassert(srcml_archive_get_version(archive), 0);
        dassert(srcml_archive_get_tabstop(archive), 8);
        dassert(srcml_archive_get_namespace_size(archive), 1);
        dassert(srcml_archive_get_namespace_uri(archive, 0), std::string("http://www.srcML.org/srcML/src"));
        srcml_archive_free(archive);
    }

    return 0;
}
