/**
 * @file test_srcml_sax2_reader.cpp
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

  Test cases for srcml_sax2_reader
*/

// includes

#include <srcml_macros.hpp>

#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>

#if defined(__GNUC__) && !defined(__MINGW32__)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>

#include <srcml.h>
#include <srcml_sax2_reader.hpp>
#include <srcmlns.hpp>

#include <unit_tests.hpp>

#include <boost/optional.hpp>

int main() {

    const std::string srcml_a = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" url=\"test\" filename=\"a.cpp\" version=\"1\" timestamp=\"today\" hash=\"abc\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";
    const std::string srcml_b = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>";

    const std::string srcml_old_uri_a = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" url=\"test\" filename=\"a.cpp\" version=\"1\" timestamp=\"today\" hash=\"abc\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";
    const std::string srcml_old_uri_b = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>";

    const std::string srcml_ns_a = "<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";
    const std::string srcml_ns_b = "<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";

    const std::string srcml_single_a = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" timestamp=\"today\" hash=\"abc\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";

    const std::string srcml_empty_single_as_unit = "<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" foo=\"bar\"/>";

    const std::string srcml_empty_nested_a = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"/>";

    const std::string srcml_empty_nested_b = "<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"/>";

    const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:pos=\"http://www.srcML.org/srcML/position\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\" pos:tabs=\"4\" foo=\"bar\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" url=\"test\" filename=\"a.cpp\" version=\"1\" timestamp=\"today\" hash=\"abc\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

    const std::string srcml_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" timestamp=\"today\" hash=\"abc\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n";

    const std::string srcml_ns = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.srcML.org/srcML/src\" xmlns:pos=\"http://www.srcML.org/srcML/position\" xmlns:foo=\"bar\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\" pos:tabs=\"4\" foo=\"bar\">\n\n<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n<s:unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

    const std::string srcml_empty_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" foo=\"bar\"/>";

    const std::string srcml_empty_nested = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.srcML.org/srcML/src\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" foo=\"bar\">\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"/>\n\n<unit xmlns:cpp=\"http://www.srcML.org/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"/>\n\n</unit>";

    const std::string srcml_old_uri = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:pos=\"http://www.sdml.info/srcML/position\" language=\"C++\" url=\"test\" filename=\"project\" version=\"1\" pos:tabs=\"4\" foo=\"bar\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" url=\"test\" filename=\"a.cpp\" version=\"1\" timestamp=\"today\" hash=\"abc\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

    std::ofstream srcml_file("project.xml");
    srcml_file << srcml;
    srcml_file.close();

    std::ofstream srcml_file_single("project_single.xml");
    srcml_file_single << srcml_single;
    srcml_file_single.close();

    std::ofstream srcml_file_ns("project_ns.xml");
    srcml_file_ns << srcml_ns;
    srcml_file_ns.close();

    std::ofstream srcml_file_empty_single("project_empty_single.xml");
    srcml_file_empty_single << srcml_empty_single;
    srcml_file_empty_single.close();

    std::ofstream srcml_file_empty_nested("project_empty_nested.xml");
    srcml_file_empty_nested << srcml_empty_nested;
    srcml_file_empty_nested.close();

    std::ofstream srcml_file_old_uri("project_old_uri.xml");
    srcml_file_old_uri << srcml_old_uri;
    srcml_file_old_uri.close();

    /*
      srcml_sax2_reader(const char * filename)
    */

    {
        try {
            srcml_sax2_reader reader("project.xml");

        } catch(...) {
            assert(false);
        }

    }

    /*
      {
      try {
      srcml_sax2_reader reader("foo.xml");
      assert(false);
      } catch(...) {}

      }
    */

    {
        try {
            srcml_sax2_reader reader((const char *)NULL);
            assert(false);
        } catch(...) {}

    }

    /*
      srcml_sax2_reader(xmlParserInputBufferPtr input)
    */

    {
        xmlParserInputBufferPtr input = xmlParserInputBufferCreateFilename("project.xml", xmlParseCharEncoding(0));
        try {
            srcml_sax2_reader reader(input);
        } catch(...) {
            assert(false);
        }
        xmlFreeParserInputBuffer(input);
    }

    {
        try {
            srcml_sax2_reader reader((xmlParserInputBufferPtr)NULL);
            assert(false);
        } catch(...) {}

    }

    /*
      read_root_unit_attributes
    */

    {
        srcml_sax2_reader reader("project.xml");
        boost::optional<std::string> encoding, language, directory, version;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "pos");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/position");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_ARCHIVE
            | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION));
        dassert(tabstop, 4);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "pos");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/position");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_ARCHIVE
            | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION));
        dassert(tabstop, 4);
    }

    {
        srcml_sax2_reader reader("project_ns.xml");
        boost::optional<std::string> encoding, language, directory, version;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "ISO-8859-1");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 3);
        dassert(prefixes.at(0), "s");
        dassert(prefixes.at(1), "pos");
        dassert(prefixes.at(2), "foo");
        dassert(namespaces.size(), 3);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/position");
        dassert(namespaces.at(2), "bar");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_ARCHIVE
            | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION));
        dassert(tabstop, 4);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(*encoding, "ISO-8859-1");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(prefixes.size(), 3);
        dassert(prefixes.at(0), "s");
        dassert(prefixes.at(1), "pos");
        dassert(prefixes.at(2), "foo");
        dassert(namespaces.size(), 3);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/position");
        dassert(namespaces.at(2), "bar");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_ARCHIVE
            | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION));
        dassert(tabstop, 4);
    }

    {
        srcml_sax2_reader reader("project_single.xml");
        boost::optional<std::string> encoding, language, directory, version;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "cpp");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/cpp");
        dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY));
        dassert(tabstop, 4);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "cpp");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/cpp");
        dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY));
        dassert(tabstop, 4);
    }

    {
        srcml_sax2_reader reader("project_empty_single.xml");
        boost::optional<std::string> encoding, language, directory, version;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "cpp");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/cpp");
        dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY));
        dassert(tabstop, 4);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "cpp");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/cpp");
        dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY));
        dassert(tabstop, 4);
    }

    {
        srcml_sax2_reader reader("project_empty_nested.xml");
        boost::optional<std::string> encoding, language, directory, version;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 1);
        dassert(prefixes.at(0), "");
        dassert(namespaces.size(), 1);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY));
        dassert(tabstop, 4);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 1);
        dassert(prefixes.at(0), "");
        dassert(namespaces.size(), 1);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY));
        dassert(tabstop, 4);
    }

    {
        srcml_sax2_reader reader("project_old_uri.xml");
        boost::optional<std::string> encoding, language, directory, version;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "pos");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/position");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_ARCHIVE
            | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION));
        dassert(tabstop, 4);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "pos");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/position");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_ARCHIVE
            | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION));
        dassert(tabstop, 4);
    }

    {
        srcml_sax2_reader reader("project.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;

        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
    }

    {
        srcml_sax2_reader reader("project_ns.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;

        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
    }

    {
        srcml_sax2_reader reader("project_single.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;

        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
    }

    {
        srcml_sax2_reader reader("project_empty_single.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;

        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
    }

    {
        srcml_sax2_reader reader("project_empty_nested.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;

        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(reader.read_root_unit_attributes(encoding, language, directory,
                                                 version, attributes, prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
    }

    /*
      read_unit_attributes
    */

    {
        srcml_sax2_reader reader("project.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "a.cpp");
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(*timestamp, "today");
        dassert(*hash, "abc");
        dassert(attributes.size(), 0);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "b.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
    }

    {
        srcml_sax2_reader reader("project_ns.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "a.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(attributes.size(), 0);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "b.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0)
        dassert(hash, 0)
        dassert(attributes.size(), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
    }

    {
        srcml_sax2_reader reader("project_single.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "project");
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(*timestamp, "today");
        dassert(*hash, "abc");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
    }

    {
        srcml_sax2_reader reader("project_empty_single.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "project");
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
    }

    {
        srcml_sax2_reader reader("project_empty_nested.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "a.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 0);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "b.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
    }

    {
        srcml_sax2_reader reader("project_old_uri.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "a.cpp");
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(*timestamp, "today");
        dassert(*hash, "abc");
        dassert(attributes.size(), 0);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "b.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
    }

    /*
      read_srcml
    */

    {
        srcml_sax2_reader reader("project.xml");
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_a);
        reader.read_srcml(unit);
        dassert(*unit, srcml_b);
        reader.read_srcml(unit);
        dassert(unit, 0);
        reader.read_srcml(unit);
        dassert(unit, 0);
    }

    {
        srcml_sax2_reader reader("project_ns.xml");
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_ns_a);
        reader.read_srcml(unit);
        dassert(*unit, srcml_ns_b);
        reader.read_srcml(unit);
        dassert(unit, 0);
        reader.read_srcml(unit);
        dassert(unit, 0);
    }

    {
        srcml_sax2_reader reader("project_single.xml");
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_single_a);
        reader.read_srcml(unit);
        dassert(unit, 0);
        reader.read_srcml(unit);
        dassert(unit, 0);
    }

    {
        srcml_sax2_reader reader("project_empty_single.xml");
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_empty_single_as_unit);
        reader.read_srcml(unit);
        dassert(unit, 0);
        reader.read_srcml(unit);
        dassert(unit, 0);
    }

    {
        srcml_sax2_reader reader("project_empty_nested.xml");
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_empty_nested_a);
        reader.read_srcml(unit);
        dassert(*unit, srcml_empty_nested_b);
        reader.read_srcml(unit);
        dassert(unit, 0);
        reader.read_srcml(unit);
        dassert(unit, 0);
    }

    {
        srcml_sax2_reader reader("project_old_uri.xml");
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_old_uri_a);
        reader.read_srcml(unit);
        dassert(*unit, srcml_old_uri_b);
        reader.read_srcml(unit);
        dassert(unit, 0);
        reader.read_srcml(unit);
        dassert(unit, 0);
    }

    /*
      combined
    */

    {
        srcml_sax2_reader reader("project.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "pos");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/position");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_ARCHIVE
            | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION));
        dassert(tabstop, 4);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "a.cpp");
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(*timestamp, "today");
        dassert(*hash, "abc");
        dassert(attributes.size(), 0);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_a);
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "b.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 0);
        reader.read_srcml(unit);
        dassert(*unit, srcml_b);
        reader.read_srcml(unit);
        dassert(unit, 0);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_srcml(unit), 0);
    }

    {
        srcml_sax2_reader reader("project_ns.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "ISO-8859-1");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 2);
        dassert(prefixes.size(), 3);
        dassert(prefixes.at(0), "s");
        dassert(prefixes.at(1), "pos");
        dassert(prefixes.at(2), "foo");
        dassert(namespaces.size(), 3);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/position");
        dassert(namespaces.at(2), "bar");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_ARCHIVE
            | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION));
        dassert(tabstop, 4);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "a.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 0);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_ns_a);
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "b.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 0);
        reader.read_srcml(unit);
        dassert(*unit, srcml_ns_b);
        reader.read_srcml(unit);
        dassert(unit, 0);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_srcml(unit), 0);
    }

    {
        srcml_sax2_reader reader("project_single.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "cpp");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/cpp");
        dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY));
        dassert(tabstop, 4);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "project");
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(*timestamp, "today");
        dassert(*hash, "abc");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_single_a);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_srcml(unit), 0);
    }

    {
        srcml_sax2_reader reader("project_empty_single.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "cpp");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/cpp");
        dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY));
        dassert(tabstop, 4);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "project");
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_empty_single_as_unit);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_srcml(unit), 0);
    }

    {
        srcml_sax2_reader reader("project_empty_nested.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 1);
        dassert(prefixes.at(0), "");
        dassert(namespaces.size(), 1);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY));
        dassert(tabstop, 4);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "a.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 0);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_empty_nested_a);
        reader.read_srcml(unit);
        dassert(*unit, srcml_empty_nested_b);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_srcml(unit), 0);
    }

    {
        srcml_sax2_reader reader("project_old_uri.xml");
        boost::optional<std::string> encoding, language, filename, directory, version, timestamp, hash;
        std::vector<std::string> attributes;
        std::vector<std::string> prefixes;
        std::vector<std::string> namespaces;
        boost::optional<std::pair<std::string, std::string> > processing_instruction;
        OPTION_TYPE options = 0;
        size_t tabstop = 0;
        std::vector<std::string> user_macro_list;
        reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                         prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list);
        dassert(*encoding, "UTF-8");
        dassert(language, boost::optional<std::string>());
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(attributes.size(), 2);
        dassert(attributes.at(0), "foo");
        dassert(attributes.at(1), "bar");
        dassert(prefixes.size(), 2);
        dassert(prefixes.at(0), "");
        dassert(prefixes.at(1), "pos");
        dassert(namespaces.size(), 2);
        dassert(namespaces.at(0), "http://www.srcML.org/srcML/src");
        dassert(namespaces.at(1), "http://www.srcML.org/srcML/position");
        dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_ARCHIVE
            | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY | SRCML_OPTION_POSITION));
        dassert(tabstop, 4);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "a.cpp");
        dassert(*directory, "test");
        dassert(*version, "1");
        dassert(*timestamp, "today");
        dassert(*hash, "abc");
        dassert(attributes.size(), 0);
        language = boost::optional<std::string>(), filename = boost::optional<std::string>(), directory = boost::optional<std::string>(),
            version = boost::optional<std::string>(), timestamp = boost::optional<std::string>(), hash = boost::optional<std::string>(), attributes = std::vector<std::string>();
        boost::optional<std::string> unit;
        reader.read_srcml(unit);
        dassert(*unit, srcml_old_uri_a);
        reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes);
        dassert(*language, "C++");
        dassert(*filename, "b.cpp");
        dassert(directory, 0);
        dassert(version, 0);
        dassert(timestamp, 0);
        dassert(hash, 0);
        dassert(attributes.size(), 0);
        reader.read_srcml(unit);
        dassert(*unit, srcml_old_uri_b);
        reader.read_srcml(unit);
        dassert(unit, 0);
        dassert(reader.read_root_unit_attributes(encoding, language, directory, version, attributes,
                                                 prefixes, namespaces, processing_instruction, options, tabstop, user_macro_list), 0);
        dassert(reader.read_unit_attributes(language, filename, directory, version, timestamp, hash, attributes), 0);
        dassert(reader.read_srcml(unit), 0);
    }

    UNLINK("project.xml");
    UNLINK("project_single.xml");
    UNLINK("project_ns.xml");
    UNLINK("project_empty_single.xml");
    UNLINK("project_empty_nested.xml");
    UNLINK("project_old_uri.xml");

    srcml_cleanup_globals();

    return 0;

}
