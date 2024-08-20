// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file test_srcml_write.cpp
 *
 * @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
 *
 * Test cases for srcml_write_*
 */

#include <srcml.h>

#include <string>
#include <dassert.hpp>

int main(int, char* argv[]) {

    std::string xml_decl =R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
)";
    std::string empty_unit = R"(<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp" version="1" timestamp="today"/>
)";
    std::string empty_inner_unit = R"(<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp" version="1" timestamp="today"/>
)";
    std::string start_root_unit_tag = R"(<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" url="url">

)";
    std::string end_root_unit_tag = R"(
</unit>
)";

    std::string start_unit = R"(<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++">)";
    std::string end_unit = R"(</unit>
)";

    /*
      srcml_write_start_unit
    */

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_set_url(archive, "url");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_version(unit, "1");
        srcml_unit_set_timestamp(unit, "today");
        srcml_write_start_unit(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + empty_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_url(archive, "url");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_version(unit, "1");
        srcml_unit_set_timestamp(unit, "today");
        srcml_write_start_unit(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_root_unit_tag + empty_inner_unit + end_root_unit_tag);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_url(archive, "url");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_version(unit, "1");
        srcml_unit_set_timestamp(unit, "today");
        srcml_write_start_unit(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_write_start_unit(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_root_unit_tag + empty_inner_unit + "\n" + empty_inner_unit + end_root_unit_tag);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_archive_enable_solitary_unit(archive);
        dassert(srcml_write_start_unit(0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    /*
      srcml_write_end_unit
    */

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_write_end_unit(unit), SRCML_STATUS_INVALID_INPUT);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        dassert(srcml_write_end_unit(0), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
        srcml_write_start_element
    */

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_write_start_unit(unit);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "foo", "element", 0);
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<foo:element/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "foo", "element", "bar");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string start_unit = R"(<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="bar" revision=")" SRCML_VERSION_STRING R"(" language="C++">)";

        dassert(std::string(s, size), xml_decl + start_unit + R"(<foo:element/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "foo", "element", "bar");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string start_unit = R"(<unit xmlns="http://www.srcML.org/srcML/src" xmlns:foo="bar" revision=")" SRCML_VERSION_STRING R"(" language="C++">)";

        dassert(std::string(s, size), xml_decl + start_unit + R"(<foo:element/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_end_element(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element/><element/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_end_element(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element/><element/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_end_element(unit);
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element><element/></element>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element><element/></element>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        dassert(srcml_write_start_element(unit, 0, 0, 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_write_start_element(unit, 0, "element", 0), SRCML_STATUS_INVALID_INPUT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        dassert(srcml_write_start_element(0, "foo", "element", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
        srcml_write_end_element
    */

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_write_end_element(unit), SRCML_STATUS_INVALID_INPUT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        dassert(srcml_write_end_element(0), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
        srcml_write_namespace
    */

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_namespace(unit, "foo", "bar");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element xmlns:foo="bar"/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_namespace(unit, 0, "bar");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element xmlns="bar"/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        dassert(srcml_write_namespace(unit, "foo", 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_write_namespace(unit, "foo", "bar"), SRCML_STATUS_INVALID_INPUT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        dassert(srcml_write_namespace(0, "foo", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
        srcml_write_attribute
    */

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_attribute(unit, 0, "foo", 0, "bar");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element foo="bar"/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_attribute(unit, "f", "foo", 0, "bar");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element f:foo="bar"/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_attribute(unit, 0, "foo", "b", "bar");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element foo="bar" xmlns="b"/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_attribute(unit, "f", "foo", "b", "bar");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element f:foo="bar" xmlns:f="b"/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_attribute(unit, 0, "foo", 0, "bar");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element foo="bar"/>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        dassert(srcml_write_attribute(unit, "f", 0, "b", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_write_attribute(unit, 0, "foo", 0, "bar"), SRCML_STATUS_INVALID_INPUT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        dassert(srcml_write_attribute(0, "f", "foo", "b", "bar"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    /*
        srcml_write_string
    */

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_string(unit, "foo");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element>foo</element>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_start_element(unit, 0, "element", 0);
        srcml_write_string(unit, "foo");
        srcml_write_end_element(unit);
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), xml_decl + start_unit + R"(<element><element>foo</element></element>)" + end_unit);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        dassert(srcml_write_string(unit, "foo"), SRCML_STATUS_INVALID_INPUT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, 0, "element", 0);
        dassert(srcml_write_string(unit, 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        dassert(srcml_write_string(0, "foo"), SRCML_STATUS_INVALID_ARGUMENT);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "cpp", "element", "http://www.srcML.org/srcML/cpp");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="1.0.0" language="C++"><cpp:element/></unit>
)";
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "cpp", "element", "http://www.srcML.org/srcML/cpp");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="1.0.0" language="C++"><cpp:element/></unit>

</unit>
)";
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "omp", "element", "http://www.srcML.org/srcML/omp");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:omp="http://www.srcML.org/srcML/omp" revision="1.0.0" language="C++"><omp:element/></unit>
)";
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "omp", "element", "http://www.srcML.org/srcML/omp");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision="1.0.0">

<unit xmlns:omp="http://www.srcML.org/srcML/omp" revision="1.0.0" language="C++"><omp:element/></unit>

</unit>
)";
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "omp", "element", "http://www.srcML.org/srcML/omp");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:omp="http://www.srcML.org/srcML/omp" revision="1.0.0" language="C++"><omp:element/></unit>
)";
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "src", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "", "element", "http://newlement.net");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<src:unit xmlns:src="http://www.srcML.org/srcML/src" revision="1.0.0">

<src:unit xmlns="http://newlement.net" revision="1.0.0" language="C++"><element/></src:unit>

</src:unit>
)";
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_archive_register_namespace(archive, "sa", "http://www.srcML.org/sa1");
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "sa1", "element", "http://www.srcML.org/sa1");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" xmlns:sa1="http://www.srcML.org/sa1" revision="1.0.0" language="C++"><sa1:element/></unit>
)";
        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "src", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_write_start_unit(unit);
        srcml_write_start_element(unit, "", "element", "http://newlement.net");
        srcml_write_end_element(unit);
        srcml_write_end_unit(unit);
        srcml_archive_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<src:unit xmlns:src="http://www.srcML.org/srcML/src" revision="1.0.0">

<src:unit xmlns="http://newlement.net" revision="1.0.0" language="C++"><element/></src:unit>

</src:unit>
)";
        dassert(std::string(s, size), srcml);
        free(s);
    }

    srcml_cleanup_globals();

    return 0;
}
