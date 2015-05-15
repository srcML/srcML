/**
 * @file test_srcml_archive_get.cpp
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

  Test cases for srcml_archive_get_*.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

#include <boost/optional.hpp>

int main() {

    /*
      srcml_archive_get_src_encoding
    */

    {

        srcml_archive * archive = srcml_archive_create();
        archive->src_encoding = boost::optional<std::string>();
        dassert(srcml_archive_get_src_encoding(archive), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_src_encoding(0), 0);
    }


    {

        srcml_archive * archive = srcml_archive_create();
        archive->src_encoding = "foo";
        dassert(srcml_archive_get_src_encoding(archive), std::string("foo"));
        srcml_archive_free(archive);
    }


    /*
      srcml_archive_get_xml_encoding
    */

    {

        srcml_archive * archive = srcml_archive_create();
        archive->encoding = boost::optional<std::string>();
        dassert(srcml_archive_get_xml_encoding(archive), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_xml_encoding(0), 0);
    }


    {

        srcml_archive * archive = srcml_archive_create();
        archive->encoding = "foo";
        dassert(srcml_archive_get_xml_encoding(archive), std::string("foo"));
        srcml_archive_free(archive);
    }

    /*
      srcml_archive_get_revision
    */

    {

        srcml_archive * archive = srcml_archive_create();
        dassert(srcml_archive_get_revision(archive), std::string(SRCML_VERSION_STRING));
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->revision = boost::optional<std::string>();
        dassert(srcml_archive_get_revision(archive), 0);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_revision(0), 0);
    }


    {

        srcml_archive * archive = srcml_archive_create();
        archive->revision = "foo";
        dassert(srcml_archive_get_revision(archive), std::string("foo"));
        srcml_archive_free(archive);
    }

    /*
      srcml_archive_get_language
    */

    {

        srcml_archive * archive = srcml_archive_create();
        archive->language = boost::optional<std::string>();
        dassert(srcml_archive_get_language(archive), 0);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->language = "foo";
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

        srcml_archive * archive = srcml_archive_create();
        archive->url = boost::optional<std::string>();
        dassert(srcml_archive_get_url(archive), 0);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->url = "foo";
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

        srcml_archive * archive = srcml_archive_create();
        archive->version = boost::optional<std::string>();
        dassert(srcml_archive_get_version(archive), 0);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->version = "foo";
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

        srcml_archive * archive = srcml_archive_create();

        archive->options = 1;

        dassert(srcml_archive_get_options(archive), 1);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();

        archive->options = 1 | 2;

        dassert(srcml_archive_get_options(archive), (1 | 2));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_get_options(0), 0);
    }

    /*
      srcml_archive_get_tabstop
    */

    {

        srcml_archive * archive = srcml_archive_create();

        archive->tabstop = 4;

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

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_namespace_size(archive), 1);

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

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

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_namespace_prefix(archive, 0), std::string(""));

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_namespace_prefix(archive, -1), 0);

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

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

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_prefix_from_uri(archive, "http://www.srcML.org/srcML/src"), std::string(""));

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_prefix_from_uri(archive, "bar3"), 0);

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

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

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_namespace_uri(archive, 0), std::string("http://www.srcML.org/srcML/src"));

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_namespace_uri(archive, -1), 0);

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

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

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_uri_from_prefix(archive, ""), std::string("http://www.srcML.org/srcML/src"));

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_uri_from_prefix(archive, "foo3"), 0);

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_uri_from_prefix(archive, 0), 0);

        srcml_archive_free(archive);

    }

    {
        dassert(srcml_archive_get_uri_from_prefix(0, ""), 0);
    }

    /*
      srcml_get_macro_list_size
    */

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_macro_list_size(archive), 0);

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        srcml_archive_register_macro(archive, "foo1", "bar1");
        srcml_archive_register_macro(archive, "foo2", "bar2");
        dassert(srcml_archive_get_macro_list_size(archive), 2);

        srcml_archive_free(archive);

    }

    {
        dassert(srcml_archive_get_macro_list_size(0), 0);
    }

    /*
      srcml_archive_get_macro_token
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_register_macro(archive, "foo1", "bar1");
        srcml_archive_register_macro(archive, "foo2", "bar2");

        dassert(srcml_archive_get_macro_token(archive, 1), std::string("foo2"));

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_macro_token(archive, -1), 0);

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_macro_token(archive, 7), 0);

        srcml_archive_free(archive);

    }

    {
        dassert(srcml_archive_get_macro_token(0, 0), 0);
    }

    /*
      srcml_archive_get_macro_token_type
    */
    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_register_macro(archive, "foo1", "bar1");
        srcml_archive_register_macro(archive, "foo2", "bar2");

        dassert(srcml_archive_get_macro_token_type(archive, "foo2"), std::string("bar2"));

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_macro_token_type(archive, "foo3"), 0);

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_macro_token_type(archive, 0), 0);

        srcml_archive_free(archive);

    }

    {
        dassert(srcml_archive_get_macro_token_type(0, "foo2"), 0);
    }

    /*
      srcml_archive_get_macro_type
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_register_macro(archive, "foo1", "bar1");
        srcml_archive_register_macro(archive, "foo2", "bar2");

        dassert(srcml_archive_get_macro_type(archive, 1), std::string("bar2"));

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_macro_type(archive, -1), 0);

        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_get_macro_type(archive, 7), 0);

        srcml_archive_free(archive);

    }

    {
        dassert(srcml_archive_get_macro_type(0, 0), 0);
    }

    return 0;
}
