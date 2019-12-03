/**
 * @file test_srcml_global_access.cpp
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

  Test cases for srcml_global get and set

*/

#include <srcml.h>

#include <dassert.hpp>

int main(int argc, char* argv[]) {

    /*
      srcml_set_src_encoding
    */

    {
        srcml_set_src_encoding(0);
        dassert(srcml_get_src_encoding(), 0);
    }

    {
        srcml_set_src_encoding("foo");
        dassert(srcml_get_src_encoding(), std::string("foo"));
    }

    /*
      srcml_set_xml_encoding
    */

    {
        srcml_set_xml_encoding(0);
        dassert(srcml_get_xml_encoding(), 0);
    }

    {
        srcml_set_xml_encoding("foo");
        dassert(srcml_get_xml_encoding(), std::string("foo"));
    }

    /*
      srcml_set_language
    */

    {
        srcml_set_language(0);
        dassert(srcml_get_language(), 0);
    }

    {
        srcml_set_language("foo");
        dassert(srcml_get_language(), std::string("foo"));
    }

    /*
      srcml_set_filename
    */

    {
        srcml_set_filename(0);
        dassert(srcml_get_filename(), 0);
    }

    {
        srcml_set_filename("foo");
        dassert(srcml_get_filename(), std::string("foo"));
    }

    /*
      srcml_set_url
    */

    {
        srcml_set_url(0);
        dassert(srcml_get_url(), 0);
    }

    {
        srcml_set_url("foo");
        dassert(srcml_get_url(), std::string("foo"));
    }

    /*
      srcml_set_version
    */

    {
        srcml_set_version(0);
        dassert(srcml_get_version(), 0);
    }

    {
        srcml_set_version("foo");
        dassert(srcml_get_version(), std::string("foo"));
    }

    /*
      srcml_set_timestamp
    */

    {
        srcml_set_timestamp(0);
        dassert(srcml_get_timestamp(), 0);
    }

    {
        srcml_set_timestamp("foo");
        dassert(srcml_get_timestamp(), std::string("foo"));
    }

    /*
      srcml_set_options
    */

    {
        srcml_set_options(1 | 2 | 3);
        dassert(srcml_get_options(), (1 | 2 | 3));
    }

    {
        srcml_set_options(1 | 2 | 3);
        srcml_set_options(1);
        dassert(srcml_get_options(), 1);
    }

    /*
      srcml_enable_option
    */

    {
        srcml_set_options(0);
        srcml_enable_option(1);
        dassert(srcml_get_options(), 1);
    }

    {
        srcml_set_options(1 | 2);
        srcml_enable_option(4);
        dassert(srcml_get_options(), (1 | 2 | 4));
    }

    {
        srcml_set_options(1);
        srcml_enable_option(2 | 4);
        dassert(srcml_get_options(), (1 | 2 | 4));
    }

    /*
      srcml_disable_option
    */

    {
        srcml_set_options(0);
        srcml_disable_option(0);
        dassert(srcml_get_options(), 0);
    }

    {
        srcml_set_options(1 | 2 | 4);
        srcml_disable_option(0);
        dassert(srcml_get_options(), (1 | 2 | 4));
    }

    {
        srcml_set_options(1 | 2 | 4);
        srcml_disable_option(2);
        dassert(srcml_get_options(), (1 | 4));
    }

    {
        srcml_set_options(1 | 2 | 4);
        srcml_disable_option(1 | 2);
        dassert(srcml_get_options(), 4);
    }

    /*
      srcml_set_tabstop
    */

    {
        srcml_set_tabstop(2);
        dassert(srcml_get_tabstop(), 2);
    }

    /*
      srcml_register_file_extension
    */

    {
        dassert(srcml_register_file_extension("foo", "C+"), SRCML_STATUS_INVALID_INPUT);
    }

    {
        dassert(srcml_register_file_extension("foo", 0), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        dassert(srcml_register_file_extension(0, "C++"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_register_namespace
    */

    {
        srcml_register_namespace("foo", "bar");
        dassert(srcml_get_namespace_prefix(srcml_get_namespace_size() - 1), std::string("foo"));
        dassert(srcml_get_namespace_uri(srcml_get_namespace_size() - 1), std::string("bar"));
    }

    {
        srcml_register_namespace("foo2", "bar");
        dassert(srcml_get_namespace_prefix(srcml_get_namespace_size() - 1), std::string("foo2"));
        dassert(srcml_get_namespace_uri(srcml_get_namespace_size() - 1), std::string("bar"));
    }

    {
        dassert(srcml_register_namespace(0, "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        dassert(srcml_register_namespace("foo", 0), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_set_eol
    */

    {
        srcml_set_eol(SOURCE_OUTPUT_EOL_CRLF);
        dassert(srcml_get_eol(), SOURCE_OUTPUT_EOL_CRLF);
    }

    {
        srcml_set_eol(SOURCE_OUTPUT_EOL_CRLF);
        srcml_set_eol(SOURCE_OUTPUT_EOL_AUTO);
        dassert(srcml_get_eol(), SOURCE_OUTPUT_EOL_AUTO);
    }

    {
        dassert(srcml_set_eol(SOURCE_OUTPUT_EOL_CRLF + 1), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_set_srcdiff_revision
    */

    {
        srcml_set_srcdiff_revision(SRCDIFF_REVISION_ORIGINAL);
        dassert(srcml_get_srcdiff_revision(), SRCDIFF_REVISION_ORIGINAL);
    }

    {
        srcml_set_srcdiff_revision(SRCDIFF_REVISION_ORIGINAL);
        srcml_set_srcdiff_revision(SRCDIFF_REVISION_MODIFIED);
        dassert(srcml_get_srcdiff_revision(), SRCDIFF_REVISION_MODIFIED);
    }

    {
        dassert(srcml_set_srcdiff_revision(SRCDIFF_REVISION_INVALID), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_get_src_encoding
    */

    {
        srcml_set_src_encoding(0);
        dassert(srcml_get_src_encoding(), 0);
    }

    {
        srcml_set_src_encoding("foo");
        dassert(srcml_get_src_encoding(), std::string("foo"));
    }

    /*
      srcml_get_xml_encoding
    */

    {
        srcml_set_xml_encoding(0);
        dassert(srcml_get_xml_encoding(), 0);
    }

    {
        srcml_set_xml_encoding("foo");
        dassert(srcml_get_xml_encoding(), std::string("foo"));
    }

    /*
      srcml_get_revision
    */

    {
        dassert(srcml_get_revision(), std::string(SRCML_VERSION_STRING));
    }

    /*
      srcml_get_language
    */

    {
        srcml_set_language(0);
        dassert(srcml_get_language(), 0);
    }

    {
        srcml_set_language("foo");
        dassert(srcml_get_language(), std::string("foo"));
    }

    /*
      srcml_get_filename
    */

    {
        srcml_set_filename(0);
        dassert(srcml_get_filename(), 0);
    }

    {
        srcml_set_filename("foo");
        dassert(srcml_get_filename(), std::string("foo"));
    }

    /*
      srcml_get_url
    */

    {
        srcml_set_url(0);
        dassert(srcml_get_url(), 0);
    }

    {
        srcml_set_url("foo");
        dassert(srcml_get_url(), std::string("foo"));
    }

    /*
      srcml_get_version
    */

    {
        srcml_set_version(0);
        dassert(srcml_get_version(), 0);
    }

    {
        srcml_set_version("foo");
        dassert(srcml_get_version(), std::string("foo"));
    }

    /*
      srcml_get_timestamp
    */

    {
        srcml_set_timestamp(0);
        dassert(srcml_get_timestamp(), 0);
    }

    {
        srcml_set_timestamp("foo");
        dassert(srcml_get_timestamp(), std::string("foo"));
    }

    /*
      srcml_get_hash
    */

    {
        dassert(srcml_get_hash(), 0);
    }

    /*
      srcml_get_options
    */

    {
        srcml_set_options(1);
        dassert(srcml_get_options(), 1);
    }

    {
        srcml_set_options(1 | 2);
        dassert(srcml_get_options(), (1 | 2));
    }

    /*
      srcml_get_tabstop
    */

    {
        srcml_set_tabstop(4);
        dassert(srcml_get_tabstop(), 4);
    }

    /*
      srcml_get_namespace_size
    */

    {
        dassert(srcml_get_namespace_size(), 2);
    }

    {
        srcml_register_namespace("foo2", "bar2");
        srcml_register_namespace("foo3", "bar3");
        dassert(srcml_get_namespace_size(), 4);
    }

    /*
      srcml_get_namespace_prefix
    */

    {
        dassert(srcml_get_namespace_prefix(1), std::string("foo2"));
    }

    {
        dassert(srcml_get_namespace_prefix(-1), 0);
    }

    {
        dassert(srcml_get_namespace_prefix(5), 0);
    }

    /*
      srcml_get_prefix_from_uri
    */

    {
        dassert(srcml_get_prefix_from_uri("bar2"), std::string("foo2"));
    }

    {
        dassert(srcml_get_prefix_from_uri("bar4"), 0);
    }

    {
        dassert(srcml_get_prefix_from_uri(0), 0);
    }

    /*
      srcml_get_namespace_uri
    */

    {
        dassert(srcml_get_namespace_uri(1), std::string("bar"));
    }

    {
        dassert(srcml_get_namespace_uri(-1), 0);
    }

    {
        dassert(srcml_get_namespace_uri(4), 0);
    }

    /*
      srcml_get_uri_from_prefix
    */

    {
        dassert(srcml_get_uri_from_prefix("foo2"), std::string("bar2"));
    }

    {
        dassert(srcml_get_uri_from_prefix("foo4"), 0);
    }

    {
        dassert(srcml_get_uri_from_prefix(0), 0);
    }

    /*
      srcml_get_srcdiff_revision
    */

    {
        srcml_set_srcdiff_revision(0);
        dassert(srcml_get_srcdiff_revision(), 0);
    }

    {
        srcml_set_srcdiff_revision(SRCDIFF_REVISION_ORIGINAL);
        dassert(srcml_get_srcdiff_revision(), SRCDIFF_REVISION_ORIGINAL);
    }

    return 0;
}
