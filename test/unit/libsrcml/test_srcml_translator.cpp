/**
 * @file test_srcml_translator.cpp

 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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

    /*

      srcml_translator

    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = 0;

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, "", "", "", 0, 0, namespace_prefix, namespace_uri, 4);

        translator.close();
        xmlBufferFree(buffer);

    }

    /*

      srcml_translator

    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = 0;

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, "", "", "", 0, 0, namespace_prefix, namespace_uri, 4);


        translator.close();
        xmlBufferFree(buffer);

    }

    /*

      translate

    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = 0;

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        UTF8CharBuffer * input = new UTF8CharBuffer("a;", 2, "UTF-8", 0);

        translator.translate(input);
        dassert((char *)buffer->content, std::string("<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>\n"));

        translator.close();
        xmlBufferFree(buffer);

    }

    /*

      add_unit

    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);

    }

    /*
        add_unit (namespace/attribute add)
    */


    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_CPP;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_CPP;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_LITERAL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:lit=\"http://www.sdml.info/srcML/literal\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_LITERAL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:lit=\"http://www.sdml.info/srcML/literal\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_OPERATOR;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:op=\"http://www.sdml.info/srcML/operator\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_OPERATOR;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:op=\"http://www.sdml.info/srcML/operator\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_MODIFIER;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:type=\"http://www.sdml.info/srcML/modifier\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_MODIFIER;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:type=\"http://www.sdml.info/srcML/modifier\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_POSITION;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:pos=\"http://www.sdml.info/srcML/position\" language=\"C++\" pos:tabs=\"4\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_POSITION;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit language=\"C++\" pos:tabs=\"4\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:pos=\"http://www.sdml.info/srcML/position\" pos:tabs=\"4\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_register_namespace(archive, "foo", "bar");
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, archive->prefixes, archive->namespaces, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:foo=\"bar\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_register_namespace(archive, "foo", "bar");
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, archive->prefixes, archive->namespaces, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:foo=\"bar\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_CPP | OPTION_LITERAL | OPTION_OPERATOR | OPTION_MODIFIER | OPTION_POSITION;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_register_namespace(archive, "foo", "bar");
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, archive->prefixes, archive->namespaces, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" xmlns:lit=\"http://www.sdml.info/srcML/literal\" xmlns:op=\"http://www.sdml.info/srcML/operator\" xmlns:type=\"http://www.sdml.info/srcML/modifier\" xmlns:pos=\"http://www.sdml.info/srcML/position\" xmlns:foo=\"bar\" language=\"C++\" pos:tabs=\"4\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL | OPTION_CPP | OPTION_LITERAL | OPTION_OPERATOR | OPTION_MODIFIER | OPTION_POSITION;
        srcml_archive * archive = srcml_create_archive();
        srcml_archive_register_namespace(archive, "foo", "bar");
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, archive->prefixes, archive->namespaces, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" pos:tabs=\"4\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:lit=\"http://www.sdml.info/srcML/literal\" xmlns:op=\"http://www.sdml.info/srcML/operator\" xmlns:type=\"http://www.sdml.info/srcML/modifier\" xmlns:pos=\"http://www.sdml.info/srcML/position\" xmlns:foo=\"bar\" pos:tabs=\"4\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);

    }

    /*

      add_unit hash
    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit xmlns=\"http://www.sdml.info/srcML/src\" hash=\"\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" hash=\"0123456789abcdef\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit xmlns=\"http://www.sdml.info/srcML/src\" hash=\"foobar\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" hash=\"0123456789abcdef\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit hash=\"\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" hash=\"0123456789abcdef\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit hash=\"foobar\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit xmlns=\"http://www.sdml.info/srcML/src\" hash=\"0123456789abcdef\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n" + s + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s_before = "<unit hash=\"\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";
        std::string s = "<unit hash=\"0123456789abcdef\" language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

        translator.add_unit(unit, s_before.c_str());
        translator.add_unit(unit, s_before.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit";

        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;
        srcml_archive * archive = srcml_create_archive();
        srcml_unit * unit = srcml_create_unit(archive);
        srcml_unit_set_hash(unit, "0123456789abcdef");

        srcml_translator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
        std::string s = "<unit";

        translator.add_unit(unit, s.c_str());
        translator.add_unit(unit, s.c_str());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n" + "</unit>\n");

        xmlBufferFree(buffer);

    }

    /*
        add_raw_len
    */

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "<unit/>";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), srcml_unit.size());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + srcml_unit + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "<unit/>";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), 1);
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "<\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "<unit/>";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), 0);
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "<unit/>";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), srcml_unit.size());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "<unit xmlns=\"http://www.sdml.info/srcML/src\">" + srcml_unit + "</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "<unit/>";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), 1);
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "<unit xmlns=\"http://www.sdml.info/srcML/src\"><</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "<unit/>";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), 0);
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "<unit xmlns=\"http://www.sdml.info/srcML/src\"/>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "srcML";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), srcml_unit.size());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + srcml_unit + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "srcML";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), 1);
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "s\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "srcML";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), 0);
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "srcML";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), srcml_unit.size());
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "<unit xmlns=\"http://www.sdml.info/srcML/src\">" + srcml_unit + "</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "srcML";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), 1);
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "<unit xmlns=\"http://www.sdml.info/srcML/src\">s</unit>\n");

        xmlBufferFree(buffer);

    }

    {

        xmlBufferPtr buffer = xmlBufferCreate();
        xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
        OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;

        std::string decl = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
        std::string srcml_unit = "srcML";

        srcml_translator translator(Language::LANGUAGE_CXX, "UTF-8", "UTF-8",
                                   output_buffer, op, 0, 0, 0, 0, 0, namespace_prefix, namespace_uri, 4);

        translator.add_raw_len(srcml_unit.c_str(), 0);
        translator.close();
        std::string result = (const char *)buffer->content;
        dassert(result, decl + "<unit xmlns=\"http://www.sdml.info/srcML/src\"/>\n");

        xmlBufferFree(buffer);

    }

    unlink("a.cpp");

    return 0;
}
