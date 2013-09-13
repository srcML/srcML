/*

  Test cases for srcml_archive_check_extension
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  const std::string srcml_a = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";
  const std::string srcml_b = "<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><name>b</name></s:expr>;</s:expr_stmt>\n</s:unit>";

  const std::string srcml_a_archive = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

  const std::string srcml_b_archive = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns=\"http://www.sdml.info/srcML/src\" dir=\"test\" filename=\"project\" version=\"1\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";
  
  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

  const std::string srcml_ns = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" dir=\"test\" filename=\"project\" version=\"1\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

  /* 
     srcml_write_unit
   */

  {
    char * s;
    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_memory(archive, &s);
    srcml_unit * unit = srcml_create_unit(archive);
    unit->unit = new std::string(srcml_a);
    srcml_write_unit(archive, unit);
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
 
    assert(s == srcml_a_archive);
  }

  return 0;

}
