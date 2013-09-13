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
  const std::string srcml_full = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";
  const std::string srcml_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n";
  std::ofstream srcml_file("project.xml");
  srcml_file << srcml;
  srcml_file.close();

  std::ofstream srcml_full_file("project_full.xml");
  srcml_full_file << srcml_full;
  srcml_full_file.close();

  std::ofstream srcml_single_file("project_single.xml");
  srcml_single_file << srcml_single;
  srcml_single_file.close();

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
    srcml_read_open_filename(archive, "project_single.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unit_get_language(unit) == std::string("C++"));
    assert(srcml_unit_get_filename(unit) == std::string("project"));
    assert(srcml_unit_get_directory(unit) == std::string("test"));
    assert(srcml_unit_get_version(unit) == std::string("1"));
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
    srcml_read_open_filename(archive, "project_full.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unit_get_language(unit) == std::string("C++"));
    assert(srcml_unit_get_filename(unit) == std::string("project"));
    assert(srcml_unit_get_directory(unit) == std::string("test"));
    assert(srcml_unit_get_version(unit) == std::string("1"));
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
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project_full.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unit_get_language(unit) == std::string("C++"));
    assert(srcml_unit_get_filename(unit) == std::string("project"));
    assert(srcml_unit_get_directory(unit) == std::string("test"));
    assert(srcml_unit_get_version(unit) == std::string("1"));
    srcml_unparse_unit_memory(unit, &s);
    assert(s == src);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {

    char * s;
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    srcml_unparse_unit_memory(unit, &s);
    assert(s == src);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {
    char * s;
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_create_unit(archive);
    assert(srcml_unparse_unit_memory(unit, &s) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unparse_unit_memory(unit, 0) == SRCML_STATUS_ERROR);
   
    srcml_free_unit(unit);
    srcml_close_archive(archive);
    srcml_free_archive(archive);
  }

  {
    char * s;
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_filename(archive, "project.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unparse_unit_memory(0, &s) == SRCML_STATUS_ERROR);
   
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
    srcml_read_open_filename(archive, "project_full.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unit_get_language(unit) == std::string("C++"));
    assert(srcml_unit_get_filename(unit) == std::string("project"));
    assert(srcml_unit_get_directory(unit) == std::string("test"));
    assert(srcml_unit_get_version(unit) == std::string("1"));
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
    srcml_read_open_filename(archive, "project_full.xml");
    srcml_unit * unit = srcml_read_unit(archive);
    assert(srcml_unit_get_language(unit) == std::string("C++"));
    assert(srcml_unit_get_filename(unit) == std::string("project"));
    assert(srcml_unit_get_directory(unit) == std::string("test"));
    assert(srcml_unit_get_version(unit) == std::string("1"));
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
  unlink("project_full.xml");
  unlink("project_single.xml");

  return 0;
}
