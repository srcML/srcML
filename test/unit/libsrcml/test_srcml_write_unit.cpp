/**
 * @file test_srcml_write_unit.cpp
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

  Test cases for srcml_archive_check_extension
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

int main() {

    const std::string srcml_a = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";
    const std::string srcml_b = "<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";

    const std::string utf8_srcml_no_xmldecl = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \u2713 */</comment>\n</unit>";

    const std::string latin_srcml_no_xmldecl = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \u00fe\u00ff */</comment>\n</unit>";

    const std::string srcml_old_uri_a = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";

    const std::string utf8_srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\" url=\"test\" version=\"1\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \u2713 */</comment>\n</unit>\n\n</unit>\n";

    const std::string utf8_srcml_latin = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\" url=\"test\" version=\"1\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* &#10003; */</comment>\n</unit>\n\n</unit>\n";

    const std::string latin_srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\" url=\"test\" version=\"1\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \u00fe\u00ff */</comment>\n</unit>\n\n</unit>\n";

    const std::string latin_srcml_latin = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\" url=\"test\" version=\"1\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><comment type=\"block\">/* \xfe\xff */</comment>\n</unit>\n\n</unit>\n";

    const std::string srcml_macro_no_xmldecl = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><macro><name>MACRO1</name></macro><empty_stmt>;</empty_stmt>\n<macro><name>MACRO2</name></macro><empty_stmt>;</empty_stmt>\n</unit>";

    const std::string srcml_macro_single_no_xmldecl = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><macro-list token=\"MACRO1\" type=\"src:macro\"/><macro-list token=\"MACRO2\" type=\"src:macro\"/><macro><name>MACRO1</name></macro><empty_stmt>;</empty_stmt>\n<macro><name>MACRO2</name></macro><empty_stmt>;</empty_stmt>\n</unit>";

    const std::string srcml_a_single_no_xmldecl = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";

    const std::string srcml_a_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n";

    const std::string srcml_a_archive = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

    const std::string srcml_old_uri_a_single_no_xmldecl = "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";

    const std::string srcml_first_old_uri_a_single_no_xmldecl = "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";

    const std::string srcml_second_old_uri_a_single_no_xmldecl = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";

    const std::string srcml_b_single_no_xmldecl = "<s:unit xmlns:s=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";

    const std::string srcml_b_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n";

    const std::string srcml_b_archive = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\" url=\"test\" version=\"1\">\n\n<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

    const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

    const std::string srcml_ns = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\" url=\"test\" version=\"1\">\n\n<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

    const std::string srcml_macro = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"" SRCML_VERSION_STRING "\" url=\"test\" version=\"1\"><macro-list token=\"MACRO1\" type=\"src:macro\"/><macro-list token=\"MACRO2\" type=\"src:macro\"/>\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" filename=\"a.cpp\"><macro><name>MACRO1</name></macro><empty_stmt>;</empty_stmt>\n<macro><name>MACRO2</name></macro><empty_stmt>;</empty_stmt>\n</unit>\n\n</unit>\n";

    const std::string srcml_macro_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\"><macro-list token=\"MACRO1\" type=\"src:macro\"/><macro-list token=\"MACRO2\" type=\"src:macro\"/><macro><name>MACRO1</name></macro><empty_stmt>;</empty_stmt>\n<macro><name>MACRO2</name></macro><empty_stmt>;</empty_stmt>\n</unit>\n";

    /*
      srcml_write_unit
    */

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_a_single_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_single);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_a;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_archive);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "b.cpp");
        unit->unit = srcml_b;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "b.cpp");
        unit->unit = srcml_b;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_ns);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_a;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_a;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_url(unit, "test");
        srcml_unit_set_version(unit, "1");
        unit->unit = srcml_b_single_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_b_single);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_url(unit, "test");
        srcml_unit_set_version(unit, "1");
        unit->unit = srcml_b_single_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_b_single);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "b.cpp");
        unit->unit = srcml_b;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_b_archive);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_xml_encoding(archive, "UTF-8");
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_url(unit, "test");
        srcml_unit_set_version(unit, "1");
        unit->unit = utf8_srcml_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), utf8_srcml);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_xml_encoding(archive, "ISO-8859-1");
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_url(unit, "test");
        srcml_unit_set_version(unit, "1");
        unit->unit = utf8_srcml_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), utf8_srcml_latin);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_xml_encoding(archive, "UTF-8");
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_url(unit, "test");
        srcml_unit_set_version(unit, "1");
        unit->unit = latin_srcml_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), latin_srcml);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_xml_encoding(archive, "ISO-8859-1");
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_url(unit, "test");
        srcml_unit_set_version(unit, "1");
        unit->unit = latin_srcml_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), latin_srcml_latin);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_register_macro(archive, "MACRO1", "src:macro");
        srcml_archive_register_macro(archive, "MACRO2", "src:macro");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_macro_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_macro);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_register_macro(archive, "MACRO1", "src:macro");
        srcml_archive_register_macro(archive, "MACRO2", "src:macro");
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "project");
        srcml_unit_set_url(unit, "test");
        srcml_unit_set_version(unit, "1");
        unit->unit = srcml_macro_single_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_macro_single);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_archive * iarchive = srcml_archive_create();
        srcml_archive_read_open_memory(iarchive, srcml_a_archive.c_str(), srcml_a_archive.size());
        srcml_unit * unit = srcml_read_unit_header(iarchive);
        dassert(unit->unit, 0)
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(iarchive);
        srcml_archive_free(iarchive);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_archive);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_old_uri_a_single_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_single);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_first_old_uri_a_single_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_single);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_second_old_uri_a_single_no_xmldecl;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml_a_single);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_old_uri_a;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        unit = srcml_unit_create(archive);
        srcml_unit_set_filename(unit, "a.cpp");
        unit->unit = srcml_old_uri_a;
        srcml_write_unit(archive, unit);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);

        dassert(std::string(s, size), srcml);
        free(s);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_write_open_memory(archive, &s, &size);
        srcml_unit * unit = srcml_unit_create(archive);
        dassert(srcml_write_unit(archive, unit), SRCML_STATUS_UNINITIALIZED_UNIT);
        srcml_unit_free(unit);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->unit = "<unit/>";
        dassert(srcml_write_unit(archive, unit), SRCML_STATUS_INVALID_IO_OPERATION);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    {
        char * s = 0;
        size_t size;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_set_language(archive, "C++");
        srcml_archive_set_url(archive, "test");
        srcml_archive_set_version(archive, "1");
        srcml_archive_register_namespace(archive, "s", "http://www.srcML.org/srcML/src");
        srcml_archive_write_open_memory(archive, &s, &size);
        dassert(srcml_write_unit(archive, 0), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_archive_close(archive);
        srcml_archive_free(archive);
        free(s);
    }

    {
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        dassert(srcml_write_unit(0, unit), SRCML_STATUS_INVALID_ARGUMENT);
        srcml_unit_free(unit);
        srcml_archive_free(archive);
    }

    srcml_cleanup_globals();

    return 0;

}
