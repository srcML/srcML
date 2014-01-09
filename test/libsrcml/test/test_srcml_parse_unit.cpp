/*
  test_srcml_parse_unit.cpp

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

  Test cases for srcml_parse_unit
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

#include "dassert.hpp"

int main(int argc, char * argv[]) {

  const std::string src = "a;\n";
  const std::string srcml = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";
  const std::string srcml_full = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";
  std::ofstream src_file("project.c");
  src_file << src;
  src_file.close();

  /*
    srcml_parse_unit_filename
   */
  
  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_parse_unit_filename(unit, "project.c");
    dassert(*unit->unit, srcml);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C++");
    srcml_unit_set_filename(unit, "project");
    srcml_unit_set_directory(unit, "test");
    srcml_unit_set_version(unit , "1");
    srcml_parse_unit_filename(unit, "project.c");
    dassert(*unit->unit, srcml_full);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    dassert(srcml_parse_unit_filename(unit, "project.cpp"), SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
    dassert(srcml_parse_unit_filename(unit, "project.c"), SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    dassert(srcml_parse_unit_filename(unit, 0), SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    dassert(srcml_parse_unit_filename(0, "project.c"), SRCML_STATUS_ERROR);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  /*
    srcml_parse_unit_memory
   */

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    srcml_parse_unit_memory(unit, src.c_str(), src.size());
    dassert(*unit->unit, srcml);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C++");
    srcml_unit_set_filename(unit, "project");
    srcml_unit_set_directory(unit, "test");
    srcml_unit_set_version(unit , "1");
    srcml_parse_unit_memory(unit, src.c_str(), src.size());
    dassert(*unit->unit, srcml_full);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    dassert(srcml_parse_unit_memory(unit, src.c_str(), src.size()), SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    dassert(srcml_parse_unit_memory(unit, 0, src.size()), SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    dassert(srcml_parse_unit_memory(unit, src.c_str(), 0), SRCML_STATUS_ERROR); 

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    dassert(srcml_parse_unit_memory(0, src.c_str(), src.size()), SRCML_STATUS_ERROR);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  /*
    srcml_parse_unit_FILE
   */

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    FILE * file = fopen("project.c", "r");
    srcml_parse_unit_FILE(unit, file);
    dassert(*unit->unit, srcml);
    fclose(file);

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C++");
    srcml_unit_set_filename(unit, "project");
    srcml_unit_set_directory(unit, "test");
    srcml_unit_set_version(unit , "1");
    FILE * file = fopen("project.c", "r");
    srcml_parse_unit_FILE(unit, file);
    dassert(*unit->unit, srcml_full);
    fclose(file);

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    FILE * file = fopen("project.c", "r");
    dassert(srcml_parse_unit_FILE(unit, file), SRCML_STATUS_ERROR);
    fclose(file);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    dassert(srcml_parse_unit_FILE(unit, 0), SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    FILE * file = fopen("project.c", "r");
    dassert(srcml_parse_unit_FILE(0, file), SRCML_STATUS_ERROR);
    fclose(file);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  /*
    srcml_parse_unit_fd
   */

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    int fd = open("project.c", O_RDONLY);
    srcml_parse_unit_fd(unit, fd);
    dassert(*unit->unit, srcml);
    close(fd);

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C++");
    srcml_unit_set_filename(unit, "project");
    srcml_unit_set_directory(unit, "test");
    srcml_unit_set_version(unit , "1");
    int fd = open("project.c", O_RDONLY);
    srcml_parse_unit_fd(unit, fd);
    dassert(*unit->unit, srcml_full);
    close(fd);

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    int fd = open("project.c", O_RDONLY);
    dassert(srcml_parse_unit_fd(unit, fd), SRCML_STATUS_ERROR);
    close(fd);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    dassert(srcml_parse_unit_fd(unit, -1), SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    int fd = open("project.c", O_RDONLY);
    dassert(srcml_parse_unit_fd(0, fd), SRCML_STATUS_ERROR);
    close(fd);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  unlink("project.c");
  unlink("project.xml");

  srcml_cleanup_globals();

  return 0;
}
