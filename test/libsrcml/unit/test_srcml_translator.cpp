/**
 * @file test_srcml_translator.cpp

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
  test_srcml_translator.cpp

  Unit tests for srcml_translator.

*/
#include <stdio.h>
#include <string.h>
#ifndef _MSC_BUILD
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fstream>
#include <srcml_translator.hpp>
#include <UTF8CharBuffer.hpp>
#include <Language.hpp>
#include <srcmlns.hpp>
#include <cassert>
#include <dassert.hpp>
#include <srcml.h>

#include <srcml_types.hpp>

int main() {

    std::ofstream file("a.cpp");
    file << "a;";
    file.close();

    std::vector<std::string> namespace_prefix;
    namespace_prefix.push_back(SRCML_SRC_NS_PREFIX_DEFAULT);
    namespace_prefix.push_back(SRCML_CPP_NS_PREFIX_DEFAULT);
    namespace_prefix.push_back(SRCML_ERR_NS_PREFIX_DEFAULT);
    namespace_prefix.push_back(SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT);
    namespace_prefix.push_back(SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT);
    namespace_prefix.push_back(SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT);
    namespace_prefix.push_back(SRCML_EXT_POSITION_NS_PREFIX_DEFAULT);

    std::vector<std::string> namespace_uri;
    namespace_uri.push_back(SRCML_SRC_NS_URI);
    namespace_uri.push_back(SRCML_CPP_NS_URI);
    namespace_uri.push_back(SRCML_ERR_NS_URI);
    namespace_uri.push_back(SRCML_EXT_LITERAL_NS_URI);
    namespace_uri.push_back(SRCML_EXT_OPERATOR_NS_URI);
    namespace_uri.push_back(SRCML_EXT_MODIFIER_NS_URI);
    namespace_uri.push_back(SRCML_EXT_POSITION_NS_URI);

    std::vector<std::string> attributes;
    attributes.push_back("foo");
    attributes.push_back("bar");

    boost::optional<std::pair<std::string, std::string> > processing_instruction;

    /*

      srcml_translator

    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = 0;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, "", "", "", "", attributes, 0, 0, 0);

        translator.close();
        xmlBufferFree(buffer);

    }

    /*

      srcml_translator

    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_TERNARY;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, "", "", "", "", attributes, 0, 0, 0);


        translator.close();
        xmlBufferFree(buffer);

    }

    /*

      translate

    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_TERNARY;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, "archive", 0, 0, 0, attributes, 0, 0, 0);

        UTF8CharBuffer * input = new UTF8CharBuffer("a;", 2, "UTF-8", 0);

        translator.translate(input);
        translator.close();

        dassert((char *)buffer->content, std::string("<unit revision=\"archive\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>\n"));

        xmlBufferFree(buffer);

    }

    /*

      add_unit

    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" foo=\"bar\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    /*
        add_unit (namespace/attribute add)
    */


    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_CPP;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_CPP;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" foo=\"bar\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_OPTIONAL_MARKUP | SRCML_OPTION_LITERAL;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:lit=\"http://www.srcML.org/srcML/literal\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_OPTIONAL_MARKUP | SRCML_OPTION_LITERAL;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:lit=\"http://www.srcML.org/srcML/literal\" foo=\"bar\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_OPTIONAL_MARKUP | SRCML_OPTION_OPERATOR;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:op=\"http://www.srcML.org/srcML/operator\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_OPTIONAL_MARKUP | SRCML_OPTION_OPERATOR;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:op=\"http://www.srcML.org/srcML/operator\" foo=\"bar\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_OPTIONAL_MARKUP | SRCML_OPTION_MODIFIER;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:type=\"http://www.srcML.org/srcML/modifier\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_OPTIONAL_MARKUP | SRCML_OPTION_MODIFIER;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:type=\"http://www.srcML.org/srcML/modifier\" foo=\"bar\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);
                srcml_archive_free(archive);
        srcml_unit_free(unit);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:pos=\"http://www.srcML.org/srcML/position\" language=\"C++\" pos:tabs=\"4\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" pos:tabs=\"4\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:pos=\"http://www.srcML.org/srcML/position\" pos:tabs=\"4\" foo=\"bar\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "foo", "bar");
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, archive->prefixes, archive->namespaces, archive->processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, archive->attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "foo", "bar");
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, archive->prefixes, archive->namespaces, archive->processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, archive->attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:foo=\"bar\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_CPP | SRCML_OPTION_OPTIONAL_MARKUP | SRCML_OPTION_LITERAL | SRCML_OPTION_OPERATOR | SRCML_OPTION_MODIFIER | SRCML_OPTION_POSITION;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "foo", "bar");
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, archive->prefixes, archive->namespaces, archive->processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, archive->attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" xmlns:lit=\"http://www.srcML.org/srcML/literal\" xmlns:op=\"http://www.srcML.org/srcML/operator\" xmlns:type=\"http://www.srcML.org/srcML/modifier\" xmlns:pos=\"http://www.srcML.org/srcML/position\" xmlns:foo=\"bar\" language=\"C++\" pos:tabs=\"4\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_CPP | SRCML_OPTION_OPTIONAL_MARKUP | SRCML_OPTION_LITERAL | SRCML_OPTION_OPERATOR | SRCML_OPTION_MODIFIER | SRCML_OPTION_POSITION;
        srcml_archive * archive = srcml_archive_create();
        srcml_archive_register_namespace(archive, "foo", "bar");
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, archive->prefixes, archive->namespaces, archive->processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, archive->attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" pos:tabs=\"4\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:lit=\"http://www.srcML.org/srcML/literal\" xmlns:op=\"http://www.srcML.org/srcML/operator\" xmlns:type=\"http://www.srcML.org/srcML/modifier\" xmlns:pos=\"http://www.srcML.org/srcML/position\" xmlns:foo=\"bar\" pos:tabs=\"4\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    /*

      add_unit hash
    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit xmlns=\"http://www.srcML.org/srcML/src\" hash=\"\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\" hash=\"0123456789abcdef\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit xmlns=\"http://www.srcML.org/srcML/src\" hash=\"foobar\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\" hash=\"0123456789abcdef\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit hash=\"\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\" hash=\"0123456789abcdef\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit hash=\"foobar\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\" hash=\"0123456789abcdef\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit hash=\"\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" hash=\"0123456789abcdef\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" foo=\"bar\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit";

        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        unit->attributes = attributes;
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" foo=\"bar\">\n\n" + "</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    /*

      add_unit revision
    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        archive->revision = "archive";
        srcml_unit * unit = srcml_unit_create(archive);
        unit->revision = "unit";

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, archive->revision->c_str(), 0, 0, 0, unit->attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"archive\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY;
        srcml_archive * archive = srcml_archive_create();
        archive->revision = "archive";
        srcml_unit * unit = srcml_unit_create(archive);
        unit->revision = "unit";

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, archive->revision->c_str(), 0, 0, 0, unit->attributes, 0, 0, 0);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"unit\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\" revision=\"archive\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    /*

      add_unit source encoding
    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_STORE_ENCODING;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");

        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, unit->attributes, 0, 0, "ISO-8859-1");

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\" src-encoding=\"ISO-8859-1\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_TERNARY | SRCML_OPTION_STORE_ENCODING;
        srcml_archive * archive = srcml_archive_create();
        srcml_unit * unit = srcml_unit_create(archive);
        srcml_unit_set_src_encoding(unit, "ISO-8859-1");
 
        srcml_translator translator(output_buffer, "ISO-8859-1", op, namespace_prefix, namespace_uri, processing_instruction, 4, Language::LANGUAGE_CXX, 0, 0, 0, 0, unit->attributes, 0, 0, "ISO-8859-1");

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" revision=\"" SRCML_VERSION_STRING "\" language=\"C++\" src-encoding=\"ISO-8859-1\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.srcML.org/srcML/src\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        srcml_archive_free(archive);
        srcml_unit_free(unit);
        xmlBufferFree(buffer);

    }

    unlink("a.cpp");

    return 0;
}
