/*

  Test cases for srcMLReader
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:pos=\"http://www.sdml.info/srcML/position\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" pos:tabs=\"4\" foo=\"bar\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"a.cpp\" version=\"1\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

  const std::string srcml_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" tabs=\"4\" foo=\"bar\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n";

  const std::string srcml_ns = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" xmlns:pos=\"http://www.sdml.info/srcML/position\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\" pos:tabs=\"4\" foo=\"bar\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

  std::ofstream srcml_file("project.xml");
  srcml_file << srcml;
  srcml_file.close();

  std::ofstream srcml_file_single("project_single.xml");
  srcml_file_single << srcml_single;
  srcml_file_single.close();

  std::ofstream srcml_file_ns("project_ns.xml");
  srcml_file_ns << srcml_ns;
  srcml_file_ns.close();

  /* 
     srcMLReader(const char * filename)
  */

  {
    try {
      srcMLReader reader("project.xml");

    } catch(...) {
      assert(false);
    }

  }

  {
    try {
      srcMLReader reader("foo.xml");
      assert(false);
    } catch(...) {}

  }

  {
    try {
      srcMLReader reader((const char *)NULL);
      assert(false);
    } catch(...) {}

  }

  /* 
     srcMLReader(xmlParserInputBufferPtr input)
  */

  {
    try {
      xmlParserInputBufferPtr input = xmlParserInputBufferCreateFilename("project.xml", xmlParseCharEncoding(0));
      srcMLReader reader(input);
    } catch(...) {      
      assert(false);
    }

  }

  {
    try {
      srcMLReader reader((xmlParserInputBufferPtr)NULL);
      assert(false);
    } catch(...) {}

  }

  /* 
     readRootUnitAttributes
  */


  {
    srcMLReader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(&language, &filename, &directory, &version, attributes, prefixes, namespaces, options, tabstop);
    assert(*language == "C++");
    assert(*filename == "project");
    assert(*directory == "test");
    assert(*version == "1");
    assert(attributes.size() == 2);
    assert(attributes.at(0) == "foo");
    assert(attributes.at(1) == "bar");
    assert(prefixes.size() == 2);
    assert(prefixes.at(0) == "");
    assert(prefixes.at(1) == "pos");
    assert(namespaces.size() == 2);
    assert(namespaces.at(0) == "http://www.sdml.info/srcML/src");
    assert(namespaces.at(1) == "http://www.sdml.info/srcML/position");
    assert(options == SRCML_OPTION_POSITION);
    assert(tabstop == 4);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLReader reader("project_ns.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(&language, &filename, &directory, &version, attributes, prefixes, namespaces, options, tabstop);
    assert(*language == "C++");
    assert(*filename == "project");
    assert(*directory == "test");
    assert(*version == "1");
    assert(attributes.size() == 2);
    assert(attributes.at(0) == "foo");
    assert(attributes.at(1) == "bar");
    assert(prefixes.size() == 2);
    assert(prefixes.at(0) == "s");
    assert(prefixes.at(1) == "pos");
    assert(namespaces.size() == 2);
    assert(namespaces.at(0) == "http://www.sdml.info/srcML/src");
    assert(namespaces.at(1) == "http://www.sdml.info/srcML/position");
    assert(options == SRCML_OPTION_POSITION);
    assert(tabstop == 4);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLReader reader("project_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    reader.readRootUnitAttributes(&language, &filename, &directory, &version, attributes, prefixes, namespaces, options, tabstop);
    assert(*language == "C++");
    assert(*filename == "project");
    assert(*directory == "test");
    assert(*version == "1");
    assert(attributes.size() == 2);
    assert(attributes.at(0) == "foo");
    assert(attributes.at(1) == "bar");
    assert(prefixes.size() == 2);
    assert(prefixes.at(0) == "");
    assert(prefixes.at(1) == "cpp");
    assert(namespaces.size() == 2);
    assert(namespaces.at(0) == "http://www.sdml.info/srcML/src");
    assert(namespaces.at(1) == "http://www.sdml.info/srcML/cpp");
    assert(options == SRCML_OPTION_CPP);
    assert(tabstop == 4);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLReader reader("project_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    
    reader.readUnitAttributes(&language, &filename, &directory, &version);
    delete language, delete filename, delete directory, delete version;
    assert(reader.readRootUnitAttributes(&language, &filename, &directory, &version, attributes, prefixes, namespaces, options, tabstop) == 0);
  }

  {
    srcMLReader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    assert(reader.readRootUnitAttributes(0, &filename, &directory, &version, attributes, prefixes, namespaces, options, tabstop) == 0);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLReader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    assert(reader.readRootUnitAttributes(0, &filename, &directory, &version, attributes, prefixes, namespaces, options, tabstop) == 0);
    reader.readRootUnitAttributes(&language, &filename, &directory, &version, attributes, prefixes, namespaces, options, tabstop);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLReader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    assert(reader.readRootUnitAttributes(&language, 0, &directory, &version, attributes, prefixes, namespaces, options, tabstop) == 0);
    reader.readRootUnitAttributes(&language, &filename, &directory, &version, attributes, prefixes, namespaces, options, tabstop);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLReader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    assert(reader.readRootUnitAttributes(&language, &filename, 0, &version, attributes, prefixes, namespaces, options, tabstop) == 0);
    delete language, delete filename, delete directory, delete version;
  }

  {
    srcMLReader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    std::vector<std::string> attributes;
    std::vector<std::string> prefixes;
    std::vector<std::string> namespaces;
    OPTION_TYPE options = 0;
    int tabstop = 0;
    assert(reader.readRootUnitAttributes(&language, &filename, &directory, 0, attributes, prefixes, namespaces, options, tabstop) == 0);

    delete language, delete filename, delete directory, delete version;
  }

  /* 
     readUnitAttributes
  */


  {
    srcMLReader reader("project.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    reader.readUnitAttributes(&language, &filename, &directory, &version);
    assert(*language == "C++");
    assert(*filename == "a.cpp");
    assert(*directory == "test");
    assert(*version == "1");
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(&language, &filename, &directory, &version);
    assert(*language == "C++");
    assert(*filename == "b.cpp");
    assert(directory == 0);
    assert(version == 0);
    delete language, delete filename, delete directory, delete version;
    assert(reader.readUnitAttributes(&language, &filename, &directory, &version) == 0);
  }

  {
    srcMLReader reader("project_ns.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    reader.readUnitAttributes(&language, &filename, &directory, &version);
    assert(*language == "C++");
    assert(*filename == "a.cpp");
    assert(directory == 0);
    assert(version == 0);
    delete language, delete filename, delete directory, delete version;
    language = 0, filename = 0, directory = 0, version = 0;
    reader.readUnitAttributes(&language, &filename, &directory, &version);
    assert(*language == "C++");
    assert(*filename == "b.cpp");
    assert(directory == 0);
    assert(version == 0);
    delete language, delete filename, delete directory, delete version;
    assert(reader.readUnitAttributes(&language, &filename, &directory, &version) == 0);
  }

  {
    srcMLReader reader("project_single.xml");
    std::string * language = 0, * filename = 0, * directory = 0, * version = 0;
    reader.readUnitAttributes(&language, &filename, &directory, &version);
    assert(*language == "C++");
    assert(*filename == "project");
    assert(*directory == "test");
    assert(*version == "1");
    delete language, delete filename, delete directory, delete version;
    assert(reader.readUnitAttributes(&language, &filename, &directory, &version) == 0);
  }

  unlink("project.xml");
  unlink("project_single.xml");
  unlink("project_ns.xml");

  return 0;

}
