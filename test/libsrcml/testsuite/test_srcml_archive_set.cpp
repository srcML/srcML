/**
 * @file test_srcml_archive_set.cpp
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

  Test cases for srcml_archive_set_*.
*/

#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    /*
      srcml_archive_set_src_encoding
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_src_encoding(archive, 0), SRCML_STATUS_OK);
        dassert(srcml_archive_get_src_encoding(archive), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_src_encoding(archive, "ISO-8859-1"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_src_encoding(archive), std::string("ISO-8859-1"));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_src_encoding(0, "ISO-8859-1"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_xml_encoding
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_xml_encoding(archive, 0), SRCML_STATUS_OK);
        dassert(srcml_archive_get_xml_encoding(archive), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_xml_encoding(archive, "ISO-8859-1"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_xml_encoding(archive), std::string("ISO-8859-1"));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_xml_encoding(0, "ISO-8859-1"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_language
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_language(archive, 0), SRCML_STATUS_OK);
        dassert(srcml_archive_get_language(archive), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_language(archive, "Java"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_language(archive), std::string("Java"));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_language(0, "Java"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_url
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_url(archive, 0), SRCML_STATUS_OK);
        dassert(srcml_archive_get_url(archive), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_url(archive, "https://srcML.org"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_url(archive), std::string("https://srcML.org"));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_url(0, "https://srcML.org"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_version
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_version(archive, 0), SRCML_STATUS_OK);
        dassert(srcml_archive_get_version(archive), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_version(archive, "foo"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_version(archive), std::string("foo"));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_version(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_options
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_options(archive, SRCML_OPTION_CPP), SRCML_STATUS_OK);
        srcml_archive_get_options(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_set_options(archive, SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL), SRCML_STATUS_OK);
        srcml_archive_get_options(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_options(0, SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_enable_option
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_enable_option(archive, SRCML_OPTION_CPP), SRCML_STATUS_OK);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_options(archive, SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL);
        dassert(srcml_archive_enable_option(archive, SRCML_OPTION_STORE_ENCODING), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL | SRCML_OPTION_STORE_ENCODING));
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_enable_option(0, 1), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_disable_option
    */

    {
        srcml_archive* archive = srcml_archive_create();
        dassert(srcml_archive_disable_option(archive, 0), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), 0);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_options(archive, SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL | SRCML_OPTION_STORE_ENCODING);
        srcml_archive_disable_option(archive, 0);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL | SRCML_OPTION_STORE_ENCODING));
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_options(archive, SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL | SRCML_OPTION_STORE_ENCODING);
        dassert(srcml_archive_disable_option(archive, SRCML_OPTION_CPP), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), (SRCML_OPTION_NO_XML_DECL | SRCML_OPTION_STORE_ENCODING));
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_options(archive, SRCML_OPTION_CPP | SRCML_OPTION_NO_XML_DECL | SRCML_OPTION_STORE_ENCODING);
        dassert(srcml_archive_disable_option(archive, SRCML_OPTION_NO_XML_DECL | SRCML_OPTION_STORE_ENCODING), SRCML_STATUS_OK);
        dassert(srcml_archive_get_options(archive), SRCML_OPTION_CPP);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_disable_option(0, 1), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_tabstop
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_set_tabstop(archive, 4), SRCML_STATUS_OK);
        dassert(srcml_archive_get_tabstop(archive), 4);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_tabstop(0, 4), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_register_file_extension
    */

    {
        srcml_archive* archive = srcml_archive_create();

        srcml_archive_register_file_extension(archive, "foo", "C++");
        dassert(srcml_archive_check_extension(archive, "main.foo"), std::string("C++"));
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_register_file_extension(archive, "foo", "C+"), SRCML_STATUS_INVALID_INPUT);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_register_file_extension(archive, "foo", 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

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
        srcml_archive* archive = srcml_archive_create();

        srcml_archive_register_namespace(archive, "foo", "bar");

        int pos = srcml_archive_get_namespace_size(archive) - 1;
        dassert(srcml_archive_get_namespace_prefix(archive, pos), std::string("foo"));
        dassert(srcml_archive_get_namespace_uri(archive, pos) , std::string("bar"));

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        srcml_archive_register_namespace(archive, "foo", "http://www.srcML.org/srcML/src");

        int pos = srcml_archive_get_namespace_size(archive) - 1;
        dassert(srcml_archive_get_namespace_prefix(archive, pos), std::string("foo"));
        dassert(srcml_archive_get_namespace_uri(archive, pos) , std::string("http://www.srcML.org/srcML/src"));

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_register_namespace(archive, 0, "bar"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_register_namespace(archive, "foo", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_register_namespace(0, "foo", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_archive_set_srcdiff_revision
    */

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_set_srcdiff_revision(archive, SRCDIFF_REVISION_ORIGINAL), SRCML_STATUS_OK);
        dassert(srcml_archive_set_srcdiff_revision(0, SRCDIFF_REVISION_INVALID), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_set_srcdiff_revision(archive, SRCDIFF_REVISION_ORIGINAL), SRCML_STATUS_OK);
        dassert(srcml_archive_set_srcdiff_revision(archive, SRCDIFF_REVISION_ORIGINAL), SRCML_STATUS_OK);

        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_set_srcdiff_revision(0, SRCDIFF_REVISION_ORIGINAL), SRCML_STATUS_INVALID_ARGUMENT);
    }

    return 0;
}
