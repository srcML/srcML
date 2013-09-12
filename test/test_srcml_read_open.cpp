/*

  Test cases for read open for archives
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  /* 
     srcml_read_open_filename
  */
  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n
</unit>\n\n</unit>";

  std::ofstream("projectxml");

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_filename(archive, "project.xml");

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_read_open_filename(archive, 0) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
  assert(srcml_read_open_filename(0, "a.cpp.xml") == SRCML_STATUS_ERROR);
  }

  return 0;

}
