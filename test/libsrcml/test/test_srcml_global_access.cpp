/*
  test_srcml_global_access.cpp

  Copyright (C) 2013-2014  SDML (www.srcML.org)

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

  Test cases for srcml_global get and set

*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include "dassert.hpp"

extern srcml_archive global_archive;

int main() {

    /*
      srcml_set_src_encoding
    */

    {
        srcml_set_src_encoding(0);
        dassert(global_archive.src_encoding, 0);
    }

    {
        srcml_set_src_encoding("foo");
        dassert(*global_archive.src_encoding, "foo");
    }

    /*
      srcml_set_encoding
    */

    {
        srcml_set_encoding(0);
        dassert(global_archive.encoding, 0);
    }

    {
        srcml_set_encoding("foo");
        dassert(*global_archive.encoding, "foo");
    }

    /*
      srcml_set_language
    */

    {
        srcml_set_language(0);
        dassert(global_archive.language, 0);
    }

    {
        srcml_set_language("foo");
        dassert(*global_archive.language, "foo");
    }

    /*
      srcml_set_filename
    */

    {
        srcml_set_filename(0);
        dassert(global_archive.filename, 0);
    }

    {
        srcml_set_filename("foo");
        dassert(*global_archive.filename, "foo");
    }

    /*
      srcml_set_directory
    */

    {
        srcml_set_directory(0);
        dassert(global_archive.directory, 0);
    }

    {
        srcml_set_directory("foo");
        dassert(*global_archive.directory, "foo");
    }

    /*
      srcml_set_version
    */

    {
        srcml_set_version(0);
        dassert(global_archive.version, 0);
    }

    {
        srcml_set_version("foo");
        dassert(*global_archive.version, "foo");
    }

    /*
      srcml_set_options
    */

    {
        srcml_set_options(1 | 2 | 3);
        dassert(global_archive.options, (1 | 2 | 3));
    }

    {
        srcml_set_options(1 | 2 | 3);
        srcml_set_options(1);
        dassert(global_archive.options, 1);
    }

    /*
      srcml_enable_option
    */

    {
        srcml_set_options(0);
        srcml_enable_option(1);
        dassert(global_archive.options, 1);
    }

    {
        srcml_set_options(1 | 2);
        srcml_enable_option(4);
        dassert(global_archive.options, (1 | 2 | 4));
    }

    {
        srcml_set_options(1);
        srcml_enable_option(2 | 4);
        dassert(global_archive.options, (1 | 2 | 4));
    }

    /*
      srcml_disable_option
    */

    {
        srcml_set_options(0);
        srcml_disable_option(0);
        dassert(global_archive.options, 0);
    }

    {
        srcml_set_options(1 | 2 | 4);
        srcml_disable_option(0);
        dassert(global_archive.options, (1 | 2 | 4));
    }

    {
        srcml_set_options(1 | 2 | 4);
        srcml_disable_option(2);
        dassert(global_archive.options, (1 | 4));
    }

    {
        srcml_set_options(1 | 2 | 4);
        srcml_disable_option(1 | 2);
        dassert(global_archive.options, 4);
    }

    /*
      srcml_set_tabstop
    */

    {
        srcml_set_tabstop(4);
        dassert(global_archive.tabstop, 4);
    }

    /*
      srcml_register_file_extension
    */

    {
        srcml_register_file_extension("foo", "C++");
        dassert(global_archive.registered_languages.back().s, "foo");
        dassert(global_archive.registered_languages.back().n, 2);
    }

    {
        dassert(srcml_register_file_extension("foo", "C+"), SRCML_STATUS_ERROR);
    }

    {
        dassert(srcml_register_file_extension("foo", 0), SRCML_STATUS_ERROR);
    }

    {
        dassert(srcml_register_file_extension(0, "C++"), SRCML_STATUS_ERROR);
    }

    /*
      srcml_register_namespace
    */

    {
        srcml_register_namespace("foo", "bar");
        dassert(global_archive.prefixes.back(), "foo");
        dassert(global_archive.namespaces.back(), "bar");
    }

    {
        srcml_register_namespace("foo2", "bar");
        dassert(global_archive.prefixes.at(0), "foo2");
        dassert(global_archive.namespaces.at(0), "bar");
    }

    {
        dassert(srcml_register_namespace(0, "bar"), SRCML_STATUS_ERROR);
    }

    {
        dassert(srcml_register_namespace("foo", 0), SRCML_STATUS_ERROR);
    }

    /*
      srcml_register_macro
    */

    {
        srcml_register_macro("foo", "bar");

        dassert(global_archive.user_macro_list.at(global_archive.user_macro_list.size() - 2), "foo");
        dassert(global_archive.user_macro_list.back(), "bar");
    }

    {
        srcml_register_macro("foo", "");
        srcml_register_macro("foo", "bar");

        dassert(global_archive.user_macro_list.at(0), "foo");
        dassert(global_archive.user_macro_list.at(1) , "bar");
    }

    {
        dassert(srcml_register_macro(0, "bar"), SRCML_STATUS_ERROR);
    }

    {
        dassert(srcml_register_macro("foo", 0), SRCML_STATUS_ERROR);
    }

    /*
      srcml_get_src_encoding
    */

    {
        global_archive.src_encoding = boost::optional<std::string>();
        dassert(srcml_get_src_encoding(), 0);
    }

    {
        global_archive.src_encoding = "foo";
        dassert(srcml_get_src_encoding(), std::string("foo"));
        global_archive.src_encoding = boost::optional<std::string>();
    }

    /*
      srcml_get_encoding
    */

    {
        global_archive.encoding = boost::optional<std::string>();
        dassert(srcml_get_encoding(), 0);
    }

    {
        global_archive.encoding = "foo";
        dassert(srcml_get_encoding(), std::string("foo"));
        global_archive.encoding = boost::optional<std::string>();
    }

    /*
      srcml_get_language
    */

    {
        global_archive.language = boost::optional<std::string>();
        dassert(srcml_get_language(), 0);
    }

    {
        global_archive.language = "foo";
        dassert(srcml_get_language(), std::string("foo"));
        global_archive.language = boost::optional<std::string>();
    }

    /*
      srcml_get_filename
    */

    {
        global_archive.filename = boost::optional<std::string>();
        dassert(srcml_get_filename(), 0);
    }

    {
        global_archive.filename = "foo";
        dassert(srcml_get_filename(), std::string("foo"));
        global_archive.filename = boost::optional<std::string>();
    }

    /*
      srcml_get_directory
    */

    {
        global_archive.directory = boost::optional<std::string>();
        dassert(srcml_get_directory(), 0);
    }

    {
        global_archive.directory = "foo";
        dassert(srcml_get_directory(), std::string("foo"));
        global_archive.directory = boost::optional<std::string>();
    }

    /*
      srcml_get_version
    */

    {
        global_archive.version = boost::optional<std::string>();
        dassert(srcml_get_version(), 0);
    }

    {
        global_archive.version = "foo";
        dassert(srcml_get_version(), std::string("foo"));
        global_archive.version = boost::optional<std::string>();
    }

    /*
      srcml_get_options
    */

    {
        global_archive.options = 1;
        dassert(srcml_get_options(), 1);
    }

    {
        global_archive.options = 1 | 2;
        dassert(srcml_get_options(), (1 | 2));
    }

    /*
      srcml_get_tabstop
    */

    {
        global_archive.tabstop = 4;
        dassert(srcml_get_tabstop(), 4);
    }

    /*
      srcml_get_namespace_size
    */

    {
        dassert(srcml_get_namespace_size(), 1);
    }

    {
        srcml_register_namespace("foo2", "bar2");
        srcml_register_namespace("foo3", "bar3");
        dassert(srcml_get_namespace_size(), 3);
    }

    /*
      srcml_get_prefix
    */

    {
        dassert(srcml_get_prefix(1), std::string("foo2"));
    }

    {
        dassert(srcml_get_prefix(-1), 0);
    }

    {
        dassert(srcml_get_prefix(3), 0);
    }

    /*
      srcml_get_prefix_uri
    */

    {
        dassert(srcml_get_prefix_uri("bar2"), std::string("foo2"));
    }

    {
        dassert(srcml_get_prefix_uri("bar4"), 0);
    }

    {
        dassert(srcml_get_prefix_uri(0), 0);
    }

    /*
      srcml_get_namespace
    */

    {
        dassert(srcml_get_namespace(1), std::string("bar2"));
    }

    {
        dassert(srcml_get_namespace(-1), 0);
    }

    {
        dassert(srcml_get_namespace(3), 0);
    }

    /*
      srcml_get_namespace_prefix
    */

    {
        dassert(srcml_get_namespace_prefix("foo2"), std::string("bar"));
    }

    {
        dassert(srcml_get_namespace_prefix("foo4"), 0);
    }

    {
        dassert(srcml_get_namespace_prefix(0), 0);
    }

    /*
      srcml_get_macro_list_size
    */

    {
        dassert(srcml_get_macro_list_size(), 1);
    }

    {
        srcml_register_macro("foo2", "bar2");
        srcml_register_macro("foo3", "bar3");
        dassert(srcml_get_macro_list_size(), 3);
    }

    /*
      srcml_get_macro_token
    */

    {
        dassert(srcml_get_macro_token(1), std::string("foo2"));
    }

    {
        dassert(srcml_get_macro_token(-1), 0);
    }

    {
        dassert(srcml_get_macro_token(3), 0);
    }

    /*
      srcml_get_macro_token_type
    */

    {
        dassert(srcml_get_macro_token_type("foo2"), std::string("bar2"));
    }

    {
        dassert(srcml_get_macro_token_type("foo4"), 0);
    }

    {
        dassert(srcml_get_macro_token_type(0), 0);
    }

    /*
      srcml_get_macro_type
    */

    {
        dassert(srcml_get_macro_type(1), std::string("bar2"));
    }

    {
        dassert(srcml_get_macro_type(-1), 0);
    }

    {
        dassert(srcml_get_macro_type(3), 0);
    }


    return 0;

}
