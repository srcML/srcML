/**
 * @file test_srcml_archive_write_unit.cpp
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

  Test cases for srcml_archive_check_extension
*/

#include <srcml.h>

#include <dassert.hpp>

#include <string.h>

int main(int, char* argv[]) {

    const std::string srcml_a = R"(<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";
    const std::string srcml_b = R"(<s:unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="b.cpp"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>)";

    const std::string utf8_srcml_no_xmldecl = R"(<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" url="test" filename="project" version="1"><comment type="block">/* ✓ */</comment>
</unit>)";

    const std::string latin_srcml_no_xmldecl = R"(<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="project" version="1"><comment type="block">/* Ã¾Ã¿ */</comment>
</unit>)";

    const std::string srcml_old_uri_a = R"(<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string utf8_srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" url="test" version="1">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="project" version="1"><comment type="block">/* ✓ */</comment>
</unit>

</unit>
)";

    const std::string utf8_srcml_latin = R"(<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" url="test" version="1">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="project" version="1"><comment type="block">/* &#10003; */</comment>
</unit>

</unit>
)";

    const std::string latin_srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" url="test" version="1">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="project" version="1"><comment type="block">/* Ã¾Ã¿ */</comment>
</unit>

</unit>
)";

    const std::string latin_srcml_latin = R"(<?xml version="1.0" encoding="ISO-8859-1" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" url="test" version="1">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="project" version="1"><comment type="block">/* þÿ */</comment>
</unit>

</unit>
)";

    const std::string srcml_a_single_no_xmldecl = R"(<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_a_single = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>
)";

    const std::string srcml_a_archive = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
)";

    const std::string srcml_old_uri_a_single_no_xmldecl = R"(<unit xmlns="http://www.sdml.info/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_first_old_uri_a_single_no_xmldecl = R"(<unit xmlns="http://www.sdml.info/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_second_old_uri_a_single_no_xmldecl = R"(<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>)";

    const std::string srcml_b_single_no_xmldecl = R"(<s:unit xmlns:s="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" url="test" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>)";

    const std::string srcml_b_single = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="project" version="1"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>
)";

    const std::string srcml_b_archive = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" url="test" version="1">

<s:unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="b.cpp"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>

</s:unit>
)";

    const std::string srcml = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(">

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

<unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="a.cpp"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>
</unit>

</unit>
)";

    const std::string srcml_ns = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<s:unit xmlns:s="http://www.srcML.org/srcML/src" revision=")" SRCML_VERSION_STRING R"(" url="test" version="1">

<s:unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="b.cpp"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>

<s:unit revision=")" SRCML_VERSION_STRING R"(" language="C++" filename="b.cpp"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>
</s:unit>

</s:unit>
)";

    /*
      srcml_archive_write_unit
    */
    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_single);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_archive);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_disable_hash(archive);
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "b.cpp");
        srcml_unit_parse_memory(unit, "b;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);

        unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "b.cpp");
        srcml_unit_parse_memory(unit, "b;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_ns);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_language(unit, "C++");

        srcml_unit_parse_memory(unit, "a;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_version(unit, "1");
        srcml_unit_parse_memory(unit, "b;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_b_single);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_version(unit, "1");
        srcml_unit_parse_memory(unit, "b;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_b_single);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_disable_hash(archive);

        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "b.cpp");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "b;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_b_archive);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_disable_hash(archive);
        srcml_archive_set_xml_encoding(archive, "UTF-8");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_version(unit, "1");
        srcml_unit_set_src_encoding(unit, "UTF-8");
        srcml_archive_set_src_encoding(archive, "UTF-8");
        const char* code = "/* ✓ */\n";
        srcml_unit_parse_memory(unit, code, strlen(code));

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), utf8_srcml);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_disable_hash(archive);
        srcml_archive_set_xml_encoding(archive, "ISO-8859-1");
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_set_version(unit, "1");
        srcml_unit_set_src_encoding(unit, "UTF-8");
        const char* code = "/* ✓ */\n";
        srcml_unit_parse_memory(unit, code, strlen(code));

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), utf8_srcml_latin);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_disable_hash(archive);
        srcml_archive_set_src_encoding(archive, "ISO-8859-1");
        srcml_archive_disable_hash(archive);
        srcml_archive_set_xml_encoding(archive, "UTF-8");
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_version(unit, "1");
        const char* code = "/* þÿ */\n";
        srcml_unit_parse_memory(unit, code, strlen(code));

        dassert(srcml_unit_get_srcml_outer(unit), latin_srcml_no_xmldecl);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), latin_srcml);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_disable_hash(archive);
        srcml_archive_set_xml_encoding(archive, "ISO-8859-1");
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_version(unit, "1");
        const char* code = "/* þÿ */\n";
        srcml_unit_parse_memory(unit, code, strlen(code));

        dassert(srcml_unit_get_srcml_outer(unit), latin_srcml_no_xmldecl);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), latin_srcml_latin);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_archive* iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a_archive.c_str(), srcml_a_archive.size());
        srcml_unit* unit = srcml_archive_read_unit(iarchive);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_archive);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_language(unit, "C++");

        dassert(srcml_unit_parse_memory(unit, "a;\n", 3), SRCML_STATUS_OK);
        dassert(srcml_unit_get_srcml(unit), srcml_second_old_uri_a_single_no_xmldecl);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_single);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;\n", 3);
        dassert(srcml_unit_get_srcml(unit), srcml_second_old_uri_a_single_no_xmldecl);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_single);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_enable_solitary_unit(archive);
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;\n", 3);
        dassert(srcml_unit_get_srcml(unit), srcml_second_old_uri_a_single_no_xmldecl);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_single);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_disable_hash(archive);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;\n", 3);
        srcml_unit_set_filename(unit, "a.cpp");

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);

        unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "a;\n", 3);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_OK);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml);

        free(s);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_UNINITIALIZED_UNIT);

        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        free(s);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);
        srcml_unit_set_language(unit, "C++");
        srcml_unit_parse_memory(unit, "", 0);

        dassert(srcml_unit_get_srcml_outer(unit), std::string(R"(<unit revision="1.0.0" language="C++" hash="da39a3ee5e6b4b0d3255bfef95601890afd80709"/>)"));
        dassert(srcml_archive_write_unit(archive, unit), SRCML_STATUS_INVALID_IO_OPERATION);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        char* s = 0;
        size_t size;
        srcml_archive* archive = srcml_archive_create();
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);

        dassert(srcml_archive_write_unit(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_archive_close(archive);
        srcml_archive_free(archive);

        free(s);
    }

    {
        srcml_archive* archive = srcml_archive_create();
        srcml_unit* unit = srcml_unit_create(archive);

        dassert(srcml_archive_write_unit(0, unit), SRCML_STATUS_INVALID_ARGUMENT);

        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    srcml_cleanup_globals();

    return 0;
}
