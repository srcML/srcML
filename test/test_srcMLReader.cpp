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

  const std::string srcml_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n";

  const std::string srcml_ns = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

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
  }


  unlink("project.xml");
  unlink("project_single.xml");
  unlink("project_ns.xml");

  return 0;

}
