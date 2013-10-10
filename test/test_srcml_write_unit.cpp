/*
  test_srcml_write_unit.cpp

  Copyright (C) 2013  SDML (www.srcML.org)

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
  const std::string srcml_b = "<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";

  const std::string srcml_a_archive = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

  const std::string srcml_b_archive = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" dir=\"test\" filename=\"project\" version=\"1\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";
  
  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

  const std::string srcml_ns = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" dir=\"test\" filename=\"project\" version=\"1\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

  /* 
     srcml_write_unit
   */

  {
    char * s;
    int size;
    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_memory(archive, &s, &size);
    srcml_unit * unit = srcml_create_unit(archive);
    unit->unit = new std::string(srcml_a);
    srcml_write_unit(archive, unit);
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
 
    assert(s == srcml_a_archive);
    free(s);
  }

  {
    char * s;
    int size;
    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_language(archive, "C++");
    srcml_archive_set_filename(archive, "project");
    srcml_archive_set_directory(archive, "test");
    srcml_archive_set_version(archive, "1");
    srcml_archive_register_namespace(archive, "s", "http://www.sdml.info/srcML/src");
    srcml_write_open_memory(archive, &s, &size);
    srcml_unit * unit = srcml_create_unit(archive);
    unit->unit = new std::string(srcml_b);
    srcml_write_unit(archive, unit);
    srcml_free_unit(unit);
    unit = srcml_create_unit(archive);
    unit->unit = new std::string(srcml_b);
    srcml_write_unit(archive, unit);
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);

    assert(s == srcml_ns);
    free(s);
  }

  {
    char * s;
    int size;
    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_memory(archive, &s, &size);
    srcml_unit * unit = srcml_create_unit(archive);
    unit->unit = new std::string(srcml_a);
    srcml_write_unit(archive, unit);
    srcml_free_unit(unit);
    unit = srcml_create_unit(archive);
    unit->unit = new std::string(srcml_a);
    srcml_write_unit(archive, unit);
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
 
    assert(s == srcml);
    free(s);
  }

  {
    char * s;
    int size;
    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_language(archive, "C++");
    srcml_archive_set_filename(archive, "project");
    srcml_archive_set_directory(archive, "test");
    srcml_archive_set_version(archive, "1");
    srcml_archive_register_namespace(archive, "s", "http://www.sdml.info/srcML/src");
    srcml_write_open_memory(archive, &s, &size);
    srcml_unit * unit = srcml_create_unit(archive);
    unit->unit = new std::string(srcml_b);
    srcml_write_unit(archive, unit);
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);

    assert(s == srcml_b_archive);
    free(s);
  }

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
    assert(srcml_write_unit(archive, unit) == SRCML_STATUS_ERROR);
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {
    char * s;
    int size;
    srcml_archive * archive = srcml_create_archive();
    srcml_archive_set_language(archive, "C++");
    srcml_archive_set_filename(archive, "project");
    srcml_archive_set_directory(archive, "test");
    srcml_archive_set_version(archive, "1");
    srcml_archive_register_namespace(archive, "s", "http://www.sdml.info/srcML/src");
    srcml_write_open_memory(archive, &s, &size);
    assert(srcml_write_unit(archive, 0) == SRCML_STATUS_ERROR);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
    assert(srcml_write_unit(0, unit) == SRCML_STATUS_ERROR);
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  return 0;

}
