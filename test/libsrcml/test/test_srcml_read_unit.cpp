/*
  test_srcml_read_unit.cpp

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

  Test cases for srcml_read_unit
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>
// #include <unistd.h>
#include <fcntl.h>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include "dassert.hpp"

int main() {
  const std::string srcml_a = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";

  const std::string srcml_b = "<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";

  const std::string srcml_b_single = "<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>";

  const std::string srcml_b_two = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>";

  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";
  const std::string srcml_full = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";
  const std::string srcml_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n";

  const std::string srcml_two = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";
  /*
    srcml_read_unit
   */

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml.c_str(), srcml.size());
    srcml_unit * unit = srcml_read_unit(archive);
    dassert(*unit->unit, srcml_a);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_full.c_str(), srcml_full.size());
    srcml_unit * unit = srcml_read_unit(archive);
    dassert(srcml_unit_get_language(unit), std::string("C++"));
    dassert(srcml_unit_get_filename(unit), std::string("project"));
    dassert(srcml_unit_get_directory(unit), std::string("test"));
    dassert(srcml_unit_get_version(unit), std::string("1"));
    dassert(*unit->unit, srcml_b);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_single.c_str(), srcml_single.size());
    srcml_unit * unit = srcml_read_unit(archive);
    dassert(srcml_unit_get_language(unit), std::string("C++"));
    dassert(srcml_unit_get_filename(unit), std::string("project"));
    dassert(srcml_unit_get_directory(unit), std::string("test"));
    dassert(srcml_unit_get_version(unit), std::string("1"));
    dassert(*unit->unit, srcml_b_single);

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    dassert(srcml_read_unit(archive), 0);
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_read_unit(0), 0);
  }

  /*
    srcml_skip_unit
   */

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml.c_str(), srcml.size());
    dassert(srcml_skip_unit(archive), 1);
    dassert(srcml_skip_unit(archive), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_full.c_str(), srcml_full.size());
    dassert(srcml_skip_unit(archive), 1);
    dassert(srcml_skip_unit(archive), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_single.c_str(), srcml_single.size());
    dassert(srcml_skip_unit(archive), 1);
    dassert(srcml_skip_unit(archive), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_two.c_str(), srcml_two.size());
    dassert(srcml_skip_unit(archive), 1);
    dassert(srcml_skip_unit(archive), 1);
    dassert(srcml_skip_unit(archive), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    dassert(srcml_skip_unit(archive), 0);
   
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_skip_unit(0), 0);
  }

  /*
    srcml_read_unit_position
   */

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml.c_str(), srcml.size());
    srcml_unit * unit = srcml_read_unit_position(archive, 1);
    dassert(*unit->unit, srcml_a);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_full.c_str(), srcml_full.size());
    srcml_unit * unit = srcml_read_unit_position(archive, 1);
    dassert(srcml_unit_get_language(unit), std::string("C++"));
    dassert(srcml_unit_get_filename(unit), std::string("project"));
    dassert(srcml_unit_get_directory(unit), std::string("test"));
    dassert(srcml_unit_get_version(unit), std::string("1"));
    dassert(*unit->unit, srcml_b);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_single.c_str(), srcml_single.size());
    srcml_unit * unit = srcml_read_unit_position(archive, 1);
    dassert(srcml_unit_get_language(unit), std::string("C++"));
    dassert(srcml_unit_get_filename(unit), std::string("project"));
    dassert(srcml_unit_get_directory(unit), std::string("test"));
    dassert(srcml_unit_get_version(unit), std::string("1"));
    dassert(*unit->unit, srcml_b_single);

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml.c_str(), srcml.size());
    dassert(srcml_read_unit_position(archive, 2), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_full.c_str(), srcml_full.size());
    dassert(srcml_read_unit_position(archive, 2), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_single.c_str(), srcml_single.size());
    dassert(srcml_read_unit_position(archive, 2), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml.c_str(), srcml.size());
    dassert(srcml_read_unit_position(archive, 3), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_full.c_str(), srcml_full.size());
    dassert(srcml_read_unit_position(archive, 3), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_single.c_str(), srcml_single.size());
    dassert(srcml_read_unit_position(archive, 3), 0);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_two.c_str(), srcml_two.size());
    srcml_unit * unit = srcml_read_unit_position(archive, 1);
    dassert(*unit->unit, srcml_a);
    srcml_free_unit(unit);
    unit = srcml_read_unit_position(archive, 1);
    dassert(*unit->unit, srcml_b_two);
    srcml_free_unit(unit);
    dassert(srcml_read_unit_position(archive, 1), 0);

    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_two.c_str(), srcml_two.size());
    srcml_unit * unit = srcml_read_unit_position(archive, 2);
    dassert(*unit->unit, srcml_b_two);
    srcml_free_unit(unit);
    dassert(srcml_read_unit_position(archive, 1), 0);

    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_two.c_str(), srcml_two.size());
    dassert(srcml_read_unit_position(archive, 3), 0);

    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml_two.c_str(), srcml_two.size());
    dassert(srcml_read_unit_position(archive, 4), 0);

    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    dassert(srcml_read_unit_position(archive, 1), 0);

    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_read_unit_position(0, 1), 0);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml.c_str(), srcml.size());
    dassert(srcml_read_unit_position(archive, 0), 0);

    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, srcml.c_str(), srcml.size());
    dassert(srcml_read_unit_position(archive, -1), 0);

    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  srcml_cleanup_globals();

  return 0;
}
