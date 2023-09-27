// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_unit_set.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 *
 * Test cases for srcml_unit_set_*.
 */

#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    /*
      srcml_unit_set_src_encoding
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, 0);

        dassert(srcml_unit_get_src_encoding(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        dassert(srcml_unit_get_src_encoding(unit), std::string("ISO-8859-1"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_set_src_encoding(0, "ISO-8859-1"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_language
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, 0);

        dassert(srcml_unit_get_language(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");

        dassert(srcml_unit_get_language(unit), std::string("C++"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_set_language(0, "C++"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_filename
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, 0);

        dassert(srcml_unit_get_filename(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "main.cpp");

        dassert(srcml_unit_get_filename(unit), std::string("main.cpp"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_set_filename(0, "main.cpp"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_version
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_version(unit, 0);

        dassert(srcml_unit_get_version(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_version(unit, "0.9.5");

        dassert(srcml_unit_get_version(unit), std::string("0.9.5"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit1 = srcml_unit_create(archive);

        dassert(srcml_unit_set_version(unit1, "v1"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_version(unit1), std::string("v1"));

        srcml_unit* unit2 = srcml_unit_create(archive);

        dassert(srcml_unit_set_version(unit2, "v2"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_version(unit2), std::string("v2"));

        srcml_archive_set_version(archive, "archiveVersion");

        dassert(srcml_unit_get_version(unit1), std::string("v1"));
        dassert(srcml_unit_get_version(unit2), std::string("v2"));

        srcml_unit_free(unit1);
        srcml_unit_free(unit2);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_set_version(0, "v1"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_timestamp
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_timestamp(unit, 0);

        dassert(srcml_unit_get_timestamp(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_timestamp(unit, "Wed Jul  3 16:38:14 EDT 2019");

        dassert(srcml_unit_get_timestamp(unit), std::string("Wed Jul  3 16:38:14 EDT 2019"));

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_set_timestamp(0, "Wed Jul  3 16:38:14 EDT 2019"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_set_eol
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_AUTO), SRCML_STATUS_OK);
        dassert(srcml_unit_get_eol(unit), SOURCE_OUTPUT_EOL_AUTO);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_LF), SRCML_STATUS_OK);
        dassert(srcml_unit_get_eol(unit), SOURCE_OUTPUT_EOL_LF);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CR), SRCML_STATUS_OK);
        dassert(srcml_unit_get_eol(unit), SOURCE_OUTPUT_EOL_CR);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CRLF), SRCML_STATUS_OK);
        dassert(srcml_unit_get_eol(unit), SOURCE_OUTPUT_EOL_CRLF);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CRLF + 1), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_set_eol(0, SOURCE_OUTPUT_EOL_AUTO), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        std::string text = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++">
<expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

        srcml_archive* archive = srcml_archive_create();
        srcml_archive_read_open_memory(archive, text.c_str(), text.size());
        srcml_unit* unit = srcml_archive_read_unit(archive);

        char* buf;
        size_t size;

        srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_LF);
        srcml_unit_unparse_memory(unit, &buf, &size);
        dassert(buf, std::string("\na;\n"));
        free(buf);

        srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CRLF);
        srcml_unit_unparse_memory(unit, &buf, &size);
        dassert(buf, std::string("\r\na;\r\n"));
        free(buf);

        srcml_unit_set_eol(unit, SOURCE_OUTPUT_EOL_CR);
        srcml_unit_unparse_memory(unit, &buf, &size);
        dassert(buf, std::string("\ra;\r"));
        free(buf);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    /*
      srcml_unit_register_namespace
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        srcml_unit_register_namespace(unit, "foo", "bar");

        size_t pos = srcml_unit_get_namespace_size(unit) - 1;
        dassert(srcml_unit_get_namespace_prefix(unit, pos), std::string("foo"));
        dassert(srcml_unit_get_namespace_uri(unit, pos) , std::string("bar"));

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        srcml_unit_register_namespace(unit, "foo", "http://www.srcML.org/srcML/src");

        size_t pos = srcml_unit_get_namespace_size(unit) - 1;
        dassert(srcml_unit_get_namespace_prefix(unit, pos), std::string("foo"));
        dassert(srcml_unit_get_namespace_uri(unit, pos) , std::string("http://www.srcML.org/srcML/src"));

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_register_namespace(unit, 0, "bar"), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_register_namespace(unit, "foo", 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_register_namespace(0, "foo", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
      srcml_unit_add_attribute
    */

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_add_attribute(unit, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_attribute_size(unit), 1);
        dassert(srcml_unit_get_attribute_prefix(unit, 0), std::string("metric"));
        dassert(srcml_unit_get_attribute_name(unit, 0) , std::string("complexity"));
        dassert(srcml_unit_get_attribute_value(unit, 0) , std::string("high"));

        srcml_archive_close(archive);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");
        char* s = 0;
        size_t size = 0;
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_add_attribute(unit, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_attribute_size(unit), 1);

        srcml_unit_set_language(unit, "C++");

        srcml_unit_parse_memory(unit, "1", 0);

        srcml_archive_write_unit(archive, unit);

        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:metric="https://www.srcml.org/test/metric" revision=")" SRCML_VERSION_STRING R"(" language="C++" metric:complexity="high"/>
)";
        dassert(std::string(s, size), srcml);

        srcml_memory_free(s);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_add_attribute(unit, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_attribute_size(unit), 1);

        char* s = 0;
        size_t size = 0;
        srcml_archive_write_open_memory(archive, &s, &size);
        // srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "", 0);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:metric="https://www.srcml.org/test/metric" revision=")" SRCML_VERSION_STRING R"(">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709" metric:complexity="high"/>

</unit>
)";
        dassert(std::string(s, size), srcml);

        srcml_memory_free(s);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_add_attribute(unit, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_attribute_size(unit), 1);
        dassert(srcml_unit_add_attribute(unit, "metric", "loc", "5"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_attribute_size(unit), 2);

        dassert(srcml_unit_get_attribute_prefix(unit, 0), std::string("metric"));
        dassert(srcml_unit_get_attribute_name(unit, 0) , std::string("complexity"));
        dassert(srcml_unit_get_attribute_value(unit, 0) , std::string("high"));

        dassert(srcml_unit_get_attribute_prefix(unit, 1), std::string("metric"));
        dassert(srcml_unit_get_attribute_name(unit, 1) , std::string("loc"));
        dassert(srcml_unit_get_attribute_value(unit, 1) , std::string("5"));

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        char* s = 0;
        size_t size = 0;
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_archive_register_namespace(archive, "metric", "https://www.srcml.org/test/metric");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);

        dassert(srcml_unit_add_attribute(unit, "metric", "complexity", "high"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_attribute_size(unit), 1);
        dassert(srcml_unit_add_attribute(unit, "metric", "loc", "5"), SRCML_STATUS_OK);
        dassert(srcml_unit_get_attribute_size(unit), 2);

        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "", 0);

        srcml_archive_write_unit(archive, unit);

        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:metric="https://www.srcml.org/test/metric" revision=")" SRCML_VERSION_STRING R"(" language="C++" metric:complexity="high" metric:loc="5"/>
)";
        dassert(std::string(s, size), srcml);

        srcml_memory_free(s);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_add_attribute(unit, 0, "complexity", "high"), SRCML_STATUS_INVALID_ARGUMENT);
        dassert(srcml_unit_get_attribute_size(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_add_attribute(unit, "metric", 0, "high"), SRCML_STATUS_INVALID_ARGUMENT);
        dassert(srcml_unit_get_attribute_size(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_add_attribute(unit, "metric", "complexity", 0), SRCML_STATUS_INVALID_ARGUMENT);
        dassert(srcml_unit_get_attribute_size(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        dassert(srcml_unit_add_attribute(0, "metric", "complexity", "high"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_unit_add_attribute(unit, "metric", "complexity", "high"), SRCML_STATUS_UNASSIGNED_PREFIX);
        dassert(srcml_unit_get_attribute_size(unit), 0);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    return 0;
}
