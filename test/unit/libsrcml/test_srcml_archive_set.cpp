/**
 * @file test_srcml_archive_set.cpp
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

  Test cases for srcml_archive_set_*.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

int main() {

    /*
      srcml_archive_set_src_encoding
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_src_encoding(archive, 0);
        dassert(archive->src_encoding, 0);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_src_encoding(archive, "foo");
        dassert(*archive->src_encoding, "foo");
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_src_encoding(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_xml_encoding
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_xml_encoding(archive, 0);
        dassert(archive->encoding, 0);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_xml_encoding(archive, "foo");
        dassert(*archive->encoding, "foo");
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_xml_encoding(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_language
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_language(archive, 0);
        dassert(archive->language, 0);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_language(archive, "foo");
        dassert(*archive->language, "foo");
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_language(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_url
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_url(archive, 0);
        dassert(archive->url, 0);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_url(archive, "foo");
        dassert(*archive->url, "foo");
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_url(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_version
    */

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_version(archive, 0);
        dassert(archive->version, 0);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_version(archive, "foo");
        dassert(*archive->version, "foo");
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_version(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*

      srcml_archive_set_attributes
      {

      srcml_archive * archive = srcml_archive_create();
      const char * attr[3][2] = { "foo", "bar", "", "", 0, 0 };

      srcml_archive_set_attributes(archive, attr);

      dassert(archive->attributes.size(), 4);
      dassert(archive->attributes.at(0), "foo");
      dassert(archive->attributes.at(1), "bar");
      dassert(archive->attributes.at(2), "");
      dassert(archive->attributes.at(3), "");
      srcml_archive_free(archive);
      }

      {

      srcml_archive * archive = srcml_archive_create();
      const char * attr[3][2] = { "foo", "bar", "", 0, 0, 0 };

      srcml_archive_set_attributes(archive, attr);

      dassert(archive->attributes.size(), 0);
      srcml_archive_free(archive);
      }
    */

    /*
      srcml_archive_set_options
    */

    {

        srcml_archive * archive = srcml_archive_create();
        archive->options = 0;

        srcml_archive_set_options(archive, 1 | 2 | 4);

        dassert(archive->options, (1 | 2 | 4));
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->options = 0;

        srcml_archive_set_options(archive, 1 | 2 | 4);
        srcml_archive_set_options(archive, 1);

        dassert(archive->options, 1);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_options(0, 1 | 2 | 4), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_enable_option
    */

    {

        srcml_archive * archive = srcml_archive_create();
        archive->options = 0;

        srcml_archive_enable_option(archive, 1);

        dassert(archive->options, 1);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->options = 0;

        srcml_archive_set_options(archive, 1 | 2);
        srcml_archive_enable_option(archive, 4);

        dassert(archive->options, (1 | 2 | 4));
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->options = 0;

        srcml_archive_set_options(archive, 1);
        srcml_archive_enable_option(archive, 2 | 4);

        dassert(archive->options, (1 | 2 | 4));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_enable_option(0, 1), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_disable_option
    */

    {

        srcml_archive * archive = srcml_archive_create();
        archive->options = 0;

        srcml_archive_disable_option(archive, 0);

        dassert(archive->options, 0);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->options = 0;

        srcml_archive_set_options(archive, 1 | 2 | 4);
        srcml_archive_disable_option(archive, 0);

        dassert(archive->options, (1 | 2 | 4));
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->options = 0;

        srcml_archive_set_options(archive, 1 | 2 | 4);
        srcml_archive_disable_option(archive, 2);

        dassert(archive->options, (1 | 4));
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();
        archive->options = 0;

        srcml_archive_set_options(archive, 1 | 2 | 4);
        srcml_archive_disable_option(archive, 1 | 2);

        dassert(archive->options, 4);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_disable_option(0, 1), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_tabstop
    */

    {

        srcml_archive * archive = srcml_archive_create();

        srcml_archive_set_tabstop(archive, 4);

        dassert(archive->tabstop, 4);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_tabstop(0, 4), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_register_file_extension
    */

    {

        srcml_archive * archive = srcml_archive_create();

        srcml_archive_register_file_extension(archive, "foo", "C++");

        dassert(get_extension(archive->registered_languages.last()), "foo");
        dassert(get_language(archive->registered_languages.last()), 2);
        srcml_archive_free(archive);
    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_register_file_extension(archive, "foo", "C+"), SRCML_STATUS_INVALID_INPUT);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_register_file_extension(archive, "foo", 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_register_file_extension(archive, 0, "C++"), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);

    }

    {
        dassert(srcml_archive_register_file_extension(0, "foo", "C++"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_register_namespace
    */

    {

        srcml_archive * archive = srcml_archive_create();

        srcml_archive_register_namespace(archive, "foo", "bar");

        dassert(archive->prefixes.back(), "foo");
        dassert(archive->namespaces.back(), "bar");
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        srcml_archive_register_namespace(archive, "foo", "http://www.srcML.org/srcML/src");

        dassert(archive->prefixes.at(0), "foo");
        dassert(archive->namespaces.at(0) , "http://www.srcML.org/srcML/src");
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_register_namespace(archive, 0, "bar"), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_register_namespace(archive, "foo", 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);

    }

    {
        dassert(srcml_archive_register_namespace(0, "foo", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_register_macro
    */

    {

        srcml_archive * archive = srcml_archive_create();

        srcml_archive_register_macro(archive, "foo", "bar");

        dassert(archive->user_macro_list.at(archive->user_macro_list.size() - 2), "foo");
        dassert(archive->user_macro_list.back(), "bar");
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        srcml_archive_register_macro(archive, "foo", "");
        srcml_archive_register_macro(archive, "foo", "bar");

        dassert(archive->user_macro_list.at(0), "foo");
        dassert(archive->user_macro_list.at(1) , "bar");
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_register_macro(archive, 0, "bar"), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);

    }

    {

        srcml_archive * archive = srcml_archive_create();

        dassert(srcml_archive_register_macro(archive, "foo", 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);

    }

    {
        dassert(srcml_archive_register_macro(0, "foo", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    return 0;
}
