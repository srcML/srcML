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

int main(int argc, char * argv[]) {

  const std::string src = "a;\n";
  const std::string srcml = "<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>";
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
    assert(*unit->unit == srcml);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    assert(srcml_parse_unit_filename(unit, "project.cpp") == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
    assert(srcml_parse_unit_filename(unit, "project.c") == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    assert(srcml_parse_unit_filename(unit, 0) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    assert(srcml_parse_unit_filename(0, "project.c") == SRCML_STATUS_ERROR);
   
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
    assert(*unit->unit == srcml);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    assert(srcml_parse_unit_memory(unit, src.c_str(), src.size()) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    assert(srcml_parse_unit_memory(unit, 0, src.size()) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    assert(srcml_parse_unit_memory(unit, src.c_str(), 0) == SRCML_STATUS_ERROR); 

    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    assert(srcml_parse_unit_memory(0, src.c_str(), src.size()) == SRCML_STATUS_ERROR);
   
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
    assert(*unit->unit == srcml);
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
    assert(srcml_parse_unit_FILE(unit, file) == SRCML_STATUS_ERROR);
    fclose(file);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    assert(srcml_parse_unit_FILE(unit, 0) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    FILE * file = fopen("project.c", "r");
    assert(srcml_parse_unit_FILE(0, file) == SRCML_STATUS_ERROR);
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
    assert(*unit->unit == srcml);
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
    assert(srcml_parse_unit_fd(unit, fd) == SRCML_STATUS_ERROR);
    close(fd);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    srcml_unit_set_language(unit, "C");
    assert(srcml_parse_unit_fd(unit, -1) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_write_open_filename(archive, "project.xml");
    int fd = open("project.c", O_RDONLY);
    assert(srcml_parse_unit_fd(0, fd) == SRCML_STATUS_ERROR);
    close(fd);
   
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  unlink("project.c");
  unlink("project.xml");

  return 0;
}
