/*

  Test cases for read open for archives
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
#include <unistd.h>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>\n\n</unit>";

  std::ofstream srcml_file("project.xml");
  srcml_file << srcml;
  srcml_file.close();

  /* 
     srcml_read_open_filename
  */

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_filename(archive, "project.xml");

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_read_open_filename(archive, "foobar.xml") == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_read_open_filename(archive, 0) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
  assert(srcml_read_open_filename(0, "project.xml") == SRCML_STATUS_ERROR);
  }

  /* 
     srcml_read_open_memory
  */

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_memory(archive, srcml.c_str(), srcml.size());

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_read_open_memory(archive, 0, 1) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_read_open_memory(archive, srcml.c_str(), 0) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
    assert(srcml_read_open_memory(0, srcml.c_str(), srcml.size()) == SRCML_STATUS_ERROR);
  }

  return 0;

}
