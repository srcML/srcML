/*
  test_srcMLSAX2Reader.cpp

  Copyright (C) 2013-2014  SDML (www.srcML.org)

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*

  Test cases for srcMLSAX2Reader
*/

// includes
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>

#include <srcml.h>
#include <srcMLSAX2Reader.hpp>
#include <srcmlns.hpp>
#include "dassert.hpp"

int main() {

  const std::string srcml_a = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"a.cpp\" version=\"1\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";
  const std::string srcml_b = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>";

  const std::string srcml_ns_a = "<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";
  const std::string srcml_ns_b = "<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";

  const std::string srcml_single_a = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";

  const std::string srcml_empty_single_as_unit = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" foo=\"bar\"/>";

  const std::string srcml_empty_nested_a = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"/>";

  const std::string srcml_empty_nested_b = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"/>";

  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:pos=\"http://www.sdml.info/srcML/position\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" pos:tabs=\"4\" foo=\"bar\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"a.cpp\" version=\"1\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

  const std::string srcml_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n";

  const std::string srcml_ns = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" xmlns:pos=\"http://www.sdml.info/srcML/position\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" pos:tabs=\"4\" foo=\"bar\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

  const std::string srcml_empty_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" foo=\"bar\"/>";

  const std::string srcml_empty_nested = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" foo=\"bar\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"/>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"/>\n\n</unit>";

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

  /*
    srcMLSAX2Reader(const char * filename)
  */

  {
    try {
      srcMLSAX2Reader reader("project.xml");

    } catch(...) {
      assert(false);
    }

  }

  {
    try {
      srcMLSAX2Reader reader("foo.xml");
      assert(false);
    } catch(...) {}

  }

  {
    try {
      srcMLSAX2Reader reader((const char *)NULL);
      assert(false);
    } catch(...) {}

  }

  /*
    srcMLSAX2Reader(xmlParserInputBufferPtr input)
  */

  {
    xmlParserInputBufferPtr input = xmlParserInputBufferCreateFilename("project.xml", xmlParseCharEncoding(0));
    try {
      srcMLSAX2Reader reader(input);
    } catch(...) {
      assert(false);
    }
    xmlFreeParserInputBuffer(input);
  }

  {
    try {
      srcMLSAX2Reader reader((xmlParserInputBufferPtr)NULL);
      assert(false);
    } catch(...) {}

  }

  /*
    readRootUnitAttributes
  */

  {
    srcMLSAX2Reader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "");
    dassert(prefixes.at(1), "pos");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/position");
    dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_POSITION));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0, attributes.clear(), prefixes.clear(), namespaces.clear();
    options = 0, tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "");
    dassert(prefixes.at(1), "pos");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/position");
    dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_POSITION));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLSAX2Reader reader("project_ns.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "s");
    dassert(prefixes.at(1), "pos");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/position");
    dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_POSITION));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0, attributes.clear(), prefixes.clear(), namespaces.clear();
    options = 0, tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "s");
    dassert(prefixes.at(1), "pos");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/position");
    dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_POSITION));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLSAX2Reader reader("project_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "");
    dassert(prefixes.at(1), "cpp");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/cpp");
    dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
		      | SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0, attributes.clear(), prefixes.clear(), namespaces.clear();
    options = 0, tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "");
    dassert(prefixes.at(1), "cpp");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/cpp");
    dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
		      | SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLSAX2Reader reader("project_empty_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "");
    dassert(prefixes.at(1), "cpp");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/cpp");
    dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
		      | SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0, attributes.clear(), prefixes.clear(), namespaces.clear();
    options = 0, tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "");
    dassert(prefixes.at(1), "cpp");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/cpp");
    dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
		      | SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLSAX2Reader reader("project_empty_nested.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 1);
    dassert(prefixes.at(0), "");
    dassert(namespaces.size(), 1);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL) );
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0, attributes.clear(), prefixes.clear(), namespaces.clear();
    options = 0, tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 1);
    dassert(prefixes.at(0), "");
    dassert(namespaces.size(), 1);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLSAX2Reader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;

    reader.readUnitAttributes(language, filename, directory, version);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
  }

  {
    srcMLSAX2Reader reader("project_ns.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;

    reader.readUnitAttributes(language, filename, directory, version);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
  }

  {
    srcMLSAX2Reader reader("project_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;

    reader.readUnitAttributes(language, filename, directory, version);
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
  }

  {
    srcMLSAX2Reader reader("project_empty_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;

    reader.readUnitAttributes(language, filename, directory, version);
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
  }

  {
    srcMLSAX2Reader reader("project_empty_nested.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;

    reader.readUnitAttributes(language, filename, directory, version);
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
  }

  /*
    readUnitAttributes
  */

  {
    srcMLSAX2Reader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "a.cpp");
    dassert(*directory, "test");
    dassert(*version, "1");
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "b.cpp");
    dassert(directory, 0);
    dassert(version, 0);
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
  }

  {
    srcMLSAX2Reader reader("project_ns.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "a.cpp");
    dassert(directory, 0);
    dassert(version, 0);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "b.cpp");
    dassert(directory, 0);
    dassert(version, 0);
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
  }

  {
    srcMLSAX2Reader reader("project_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
  }

  {
    srcMLSAX2Reader reader("project_empty_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
  }

  {
    srcMLSAX2Reader reader("project_empty_nested.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "a.cpp");
    dassert(directory, 0);
    dassert(version, 0);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "b.cpp");
    dassert(directory, 0);
    dassert(version, 0);
    delete language, delete filename, delete directory, delete version;
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
  }

  /*
    readsrcML
  */

  {
    srcMLSAX2Reader reader("project.xml");
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_a);
    delete unit;
    unit = reader.readsrcML();
    dassert(*unit, srcml_b);
    delete unit;
    unit = reader.readsrcML();
    dassert(unit, 0);
    unit = reader.readsrcML();
    dassert(unit, 0);
  }

  {
    srcMLSAX2Reader reader("project_ns.xml");
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_ns_a);
    delete unit;
    unit = reader.readsrcML();
    dassert(*unit, srcml_ns_b);
    delete unit;
    unit = reader.readsrcML();
    dassert(unit, 0);
    unit = reader.readsrcML();
    dassert(unit, 0);
  }

  {
    srcMLSAX2Reader reader("project_single.xml");
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_single_a);
    delete unit;
    unit = reader.readsrcML();
    dassert(unit, 0);
    unit = reader.readsrcML();
    dassert(unit, 0);
  }

  {
    srcMLSAX2Reader reader("project_empty_single.xml");
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_empty_single_as_unit);
    delete unit;
    unit = reader.readsrcML();
    dassert(unit, 0);
    unit = reader.readsrcML();
    dassert(unit, 0);
  }

  {
    srcMLSAX2Reader reader("project_empty_nested.xml");
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_empty_nested_a);
    delete unit;
    unit = reader.readsrcML();
    dassert(*unit, srcml_empty_nested_b);
    delete unit;
    unit = reader.readsrcML();
    dassert(unit, 0);
    unit = reader.readsrcML();
    dassert(unit, 0);
  }

  /*
    combined
  */

  {
    srcMLSAX2Reader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "");
    dassert(prefixes.at(1), "pos");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/position");
    dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_POSITION));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "a.cpp");
    dassert(*directory, "test");
    dassert(*version, "1");
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_a);
    delete unit;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "b.cpp");
    dassert(directory, 0);
    dassert(version, 0);
    delete language, delete filename, delete directory, delete version;
    unit = reader.readsrcML();
    dassert(*unit, srcml_b);
    delete unit;
    unit = reader.readsrcML();
    dassert(unit, 0);
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readsrcML(), 0);
  }

  {
    srcMLSAX2Reader reader("project_ns.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "s");
    dassert(prefixes.at(1), "pos");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/position");
    dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_POSITION));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "a.cpp");
    dassert(directory, 0);
    dassert(version, 0);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_ns_a);
    delete unit;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "b.cpp");
    dassert(directory, 0);
    dassert(version, 0);
    delete language, delete filename, delete directory, delete version;
    unit = reader.readsrcML();
    dassert(*unit, srcml_ns_b);
    delete unit;
    unit = reader.readsrcML();
    dassert(unit, 0);
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readsrcML(), 0);
  }

  {
    srcMLSAX2Reader reader("project_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "");
    dassert(prefixes.at(1), "cpp");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/cpp");
    dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
		      | SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_single_a);
    delete unit;
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readsrcML(), 0);
  }

  {
    srcMLSAX2Reader reader("project_empty_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 2);
    dassert(prefixes.at(0), "");
    dassert(prefixes.at(1), "cpp");
    dassert(namespaces.size(), 2);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(namespaces.at(1), "http://www.sdml.info/srcML/cpp");
    dassert(options, (SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL
		      | SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_empty_single_as_unit);
    delete unit;
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readsrcML(), 0);
  }

  {
    srcMLSAX2Reader reader("project_empty_nested.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop);
    dassert(*language, "C++");
    dassert(*filename, "project");
    dassert(*directory, "test");
    dassert(*version, "1");
    dassert(attributes.size(), 2);
    dassert(attributes.at(0), "foo");
    dassert(attributes.at(1), "bar");
    dassert(prefixes.size(), 1);
    dassert(prefixes.at(0), "");
    dassert(namespaces.size(), 1);
    dassert(namespaces.at(0), "http://www.sdml.info/srcML/src");
    dassert(options, (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL));
    dassert(tabstop, 4);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(language, filename, directory, version);
    dassert(*language, "C++");
    dassert(*filename, "a.cpp");
    dassert(directory, 0);
    dassert(version, 0);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    std::string * unit = reader.readsrcML();
    dassert(*unit, srcml_empty_nested_a);
    delete unit;
    unit = reader.readsrcML();
    dassert(*unit, srcml_empty_nested_b);
    delete unit;
    dassert(reader.readRootUnitAttributes(language, filename, directory, version, attributes, prefixes, namespaces, options, tabstop), 0);
    dassert(reader.readUnitAttributes(language, filename, directory, version), 0);
    dassert(reader.readsrcML(), 0);
  }


  unlink("project.xml");
  unlink("project_single.xml");
  unlink("project_ns.xml");
  unlink("project_empty_single.xml");
  unlink("project_empty_nested.xml");

  srcml_cleanup_globals();

  return 0;

}
