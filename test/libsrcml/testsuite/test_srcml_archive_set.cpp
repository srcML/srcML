// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_archive_set.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_archive_set_*.
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

        size_t pos = srcml_archive_get_namespace_size(archive) - 1;
        dassert(srcml_archive_get_namespace_prefix(archive, pos), std::string("foo"));
        dassert(srcml_archive_get_namespace_uri(archive, pos) , std::string("bar"));

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        srcml_archive_register_namespace(archive, "foo", "http://www.srcML.org/srcML/src");

        size_t pos = srcml_archive_get_namespace_size(archive) - 1;
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
      srcml_archive_add_attribute
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");

        dassert(srcml_archive_add_attribute(archive, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_attribute_size(archive), 1);
        dassert(srcml_archive_get_attribute_prefix(archive, 0), std::string("metric"));
        dassert(srcml_archive_get_attribute_name(archive, 0) , std::string("complexity"));
        dassert(srcml_archive_get_attribute_value(archive, 0) , std::string("high"));

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");

        dassert(srcml_archive_add_attribute(archive, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_attribute_size(archive), 1);

        char* s = 0;
        size_t size = 0;
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:metric="https://www.srcml.org/test/metric" revision=")" SRCML_VERSION_STRING R"(" metric:complexity="high"/>
)";
        dassert(std::string(s, size), srcml);

        srcml_memory_free(s);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");

        dassert(srcml_archive_add_attribute(archive, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_attribute_size(archive), 1);

        char* s = 0;
        size_t size = 0;
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "", 0);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:metric="https://www.srcml.org/test/metric" revision=")" SRCML_VERSION_STRING R"(" metric:complexity="high">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>

</unit>
)";
        dassert(std::string(s, size), srcml);

        srcml_memory_free(s);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");

        dassert(srcml_archive_add_attribute(archive, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_attribute_size(archive), 1);
        dassert(srcml_archive_add_attribute(archive, "metric", "loc", "5"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_attribute_size(archive), 2);

        dassert(srcml_archive_get_attribute_prefix(archive, 0), std::string("metric"));
        dassert(srcml_archive_get_attribute_name(archive, 0) , std::string("complexity"));
        dassert(srcml_archive_get_attribute_value(archive, 0) , std::string("high"));

        dassert(srcml_archive_get_attribute_prefix(archive, 1), std::string("metric"));
        dassert(srcml_archive_get_attribute_name(archive, 1) , std::string("loc"));
        dassert(srcml_archive_get_attribute_value(archive, 1) , std::string("5"));

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");

        dassert(srcml_archive_add_attribute(archive, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_attribute_size(archive), 1);
        dassert(srcml_archive_add_attribute(archive, "metric", "loc", "5"), SRCML_STATUS_OK);
        dassert(srcml_archive_get_attribute_size(archive), 2);

        char* s = 0;
        size_t size = 0;
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:metric="https://www.srcml.org/test/metric" revision=")" SRCML_VERSION_STRING R"(" metric:complexity="high" metric:loc="5"/>
)";
        dassert(std::string(s, size), srcml);

        srcml_memory_free(s);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_add_attribute(archive, 0, "complexity", "high"), SRCML_STATUS_INVALID_ARGUMENT);
        dassert(srcml_archive_get_attribute_size(archive), 0);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_add_attribute(archive, "metric", 0, "high"), SRCML_STATUS_INVALID_ARGUMENT);
        dassert(srcml_archive_get_attribute_size(archive), 0);

        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_add_attribute(archive, "metric", "complexity", 0), SRCML_STATUS_INVALID_ARGUMENT);
        dassert(srcml_archive_get_attribute_size(archive), 0);

        srcml_archive_free(archive);
    }

    {
        dassert(srcml_archive_add_attribute(0, "metric", "complexity", "high"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        srcml_archive* archive = srcml_archive_create();

        dassert(srcml_archive_add_attribute(archive, "metric", "complexity", "high"), SRCML_STATUS_UNASSIGNED_PREFIX);
        dassert(srcml_archive_get_attribute_size(archive), 0);

        srcml_archive_free(archive);
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
