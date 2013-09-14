/*

  Test cases for srcml_read_unit
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
  const std::string srcml_a = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";

  const std::string srcml_b = "<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";

  const std::string srcml_b_single = "<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";

  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";
  const std::string srcml_full = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";
  const std::string srcml_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n";

  /*
    srcml_read_unit
   */

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml.c_str(), srcml.size());
    srcml_unit * unit = srcml_read_unit(archive);
    assert(*unit->unit == srcml_a);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_full.c_str(), srcml_full.size());
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unit_get_language(unit) == std::string("C++"));
    assert(srcml_unit_get_filename(unit) == std::string("project"));
    assert(srcml_unit_get_directory(unit) == std::string("test"));
    assert(srcml_unit_get_version(unit) == std::string("1"));
    assert(*unit->unit == srcml_b);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_single.c_str(), srcml_single.size());
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unit_get_language(unit) == std::string("C++"));
    assert(srcml_unit_get_filename(unit) == std::string("project"));
    assert(srcml_unit_get_directory(unit) == std::string("test"));
    assert(srcml_unit_get_version(unit) == std::string("1"));
    assert(*unit->unit == srcml_b_single);

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    assert(srcml_read_unit(archive) == 0);
    srcml_free_archive(archive);
  }

  {
    assert(srcml_read_unit(0) == 0);
  }

  return 0;
}
