/*
  test_srcml_unparse_unit.cpp

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

  Test cases for srcml_unparse_unit
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

  const std::string src = "a;\n";
  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\" filename=\"project.c\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

  std::ofstream srcml_file("project.xml");
  srcml_file << srcml;
  srcml_file.close();

  /*
    srcml_unparse_unit_filename
   */
  
  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    srcml_unparse_unit_filename(unit, "project.c");
    std::ifstream src_file("project.c");
    std::string aunit;
    src_file >> aunit;
    aunit += "\n";
    assert(aunit == src);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    assert(srcml_unparse_unit_filename(unit, "project.c") == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unparse_unit_filename(unit, 0) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    assert(srcml_unparse_unit_filename(0, "project.c") == SRCML_STATUS_ERROR);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  /*
    srcml_unparse_unit_memory
   */

  {

    char * s;
    int size;
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    srcml_unparse_unit_memory(unit, &s, &size);
    assert(s == src);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
    free(s);
  }

  {
    char * s;
    int size;
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    assert(srcml_unparse_unit_memory(unit, &s, &size) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {
    int size;
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unparse_unit_memory(unit, 0, &size) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  // TODO put in null size test

  {
    char * s;
    int size;
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unparse_unit_memory(0, &s, &size) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  /*
    srcml_unparse_unit_FILE
   */

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    FILE * file = fopen("project.c", "w");
    srcml_unparse_unit_FILE(unit, file);
    fclose(file);
    std::ifstream src_file("project.c");
    std::string aunit;
    src_file >> aunit;
    aunit += "\n";
    assert(aunit == src);

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    FILE * file = fopen("project.c", "w");
    assert(srcml_unparse_unit_FILE(unit, file) == SRCML_STATUS_ERROR);
    fclose(file);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unparse_unit_FILE(unit, 0) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    FILE * file = fopen("project.c", "w");
    assert(srcml_unparse_unit_FILE(0, file) == SRCML_STATUS_ERROR);
    fclose(file);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  /*
    srcml_unparse_unit_fd
   */

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    int fd = open("project.c", O_WRONLY);
    srcml_unparse_unit_fd(unit, fd);
    close(fd);
    std::ifstream src_file("project.c");
    std::string aunit;
    src_file >> aunit;
    aunit += "\n";
    assert(aunit == src);


    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
    int fd = open("project.c", O_WRONLY);
    assert(srcml_unparse_unit_fd(unit, fd) == SRCML_STATUS_ERROR);
    close(fd);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unparse_unit_fd(unit, -1) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    int fd = open("project.c", O_WRONLY);
    assert(srcml_unparse_unit_fd(0, fd) == SRCML_STATUS_ERROR);
    close(fd);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  unlink("project.c");
  unlink("project.xml");

  return 0;
}
