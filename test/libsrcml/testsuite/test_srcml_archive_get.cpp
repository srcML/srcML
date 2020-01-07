/**
 * @file test_srcml_archive_get.cpp
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

  Test cases for srcml_archive_get_*.
*/

#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    /*
      srcml_archive_get_src_encoding
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_src_encoding(archive, 0);
        dassert(srcml_archive_get_src_encoding(archive), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_src_encoding(0), 0);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_src_encoding(archive, "foo");
        dassert(srcml_archive_get_src_encoding(archive), std::string("foo"));
        srcml_archive_free(archive);
    }

    /*
      srcml_archive_get_xml_encoding
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_src_encoding(archive, 0);
        dassert(srcml_archive_get_xml_encoding(archive), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_xml_encoding(0), 0);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_src_encoding(archive, "foo");
        dassert(srcml_archive_get_src_encoding(archive), std::string("foo"));
        srcml_archive_free(archive);
    }

    /*
      srcml_archive_get_revision
    */
    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_revision(archive), std::string(SRCML_VERSION_STRING));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_revision(0), 0);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_version(archive, "foo");
        dassert(srcml_archive_get_version(archive), std::string("foo"));
        srcml_archive_free(archive);
    }

    /*
      srcml_archive_get_language
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_language(archive), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_language(archive, "foo");
        dassert(srcml_archive_get_language(archive), std::string("foo"));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_language(0), 0);
    }

    /*
      srcml_archive_get_url
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_url(archive), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_url(archive, "foo");
        dassert(srcml_archive_get_url(archive), std::string("foo"));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_url(0), 0);
    }

    /*
      srcml_archive_get_version
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_version(archive), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_version(archive, "foo");
        dassert(srcml_archive_get_version(archive), std::string("foo"));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_version(0), 0);
    }

    /*
      srcml_archive_get_option
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_options(archive, SRCML_OPTION_CPP);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_CPP);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_options(archive, SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_options(0), 0);
    }

    /*
      srcml_archive_get_tabstop
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_tabstop(archive, 4);
        dassert(srcml_archive_get_tabstop(archive), 4);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_tabstop(0), 0);
    }

    /*
      srcml_get_namespace_size
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_namespace_size(archive), 1);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "foo1", "bar1");
        srcml_archive_register_namespace(archive, "foo2", "bar2");
        dassert(srcml_archive_get_namespace_size(archive), 3);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_namespace_size(0), 0);
    }

    /*
      srcml_archive_get_namespace_prefix
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_namespace_prefix(archive, 0), std::string(""));
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_namespace_prefix(archive, -1), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_namespace_prefix(archive, 2), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_namespace_prefix(0, 0), 0);
    }

    /*
      srcml_archive_get_prefix_from_uri
    */
    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_prefix_from_uri(archive, "http://www.srcML.org/srcML/src"), std::string(""));
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_prefix_from_uri(archive, "bar3"), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_prefix_from_uri(archive, 0), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_prefix_from_uri(0, "http://www.srcML.org/srcML/cpp"), 0);
    }

    /*
      srcml_archive_get_namespace_uri
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_namespace_uri(archive, 0), std::string("http://www.srcML.org/srcML/src"));
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_namespace_uri(archive, -1), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_namespace_uri(archive, 2), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_namespace_uri(0, 0), 0);
    }

    /*
      srcml_archive_get_uri_from_prefix
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_uri_from_prefix(archive, ""), std::string("http://www.srcML.org/srcML/src"));
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_uri_from_prefix(archive, "foo3"), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_uri_from_prefix(archive, 0), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_uri_from_prefix(0, ""), 0);
    }

    /*
      srcml_archive_get_srcdiff_revision
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_srcdiff_revision(archive, SRCDIFF_REVISION_ORIGINAL);
        dassert(srcml_archive_get_srcdiff_revision(archive), SRCDIFF_REVISION_ORIGINAL);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_get_srcdiff_revision(archive), SRCDIFF_REVISION_INVALID);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_srcdiff_revision(0), SRCDIFF_REVISION_INVALID);
    }

    return 0;
}
