/*
  test_srcml_read_open.cpp

  Copyright (C) 2013  SDML (www.sdml.info)

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*

  Test cases for read open for archives
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

  const std::string srcml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" dir=\"test\" filename=\"project\" version=\"1\">\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n\n<unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><expr_stmt><expr><name>b</name></expr>;</expr_stmt>\n</unit>\n\n</unit>\n";

  const std::string srcml_single = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<unit xmlns=\"http://www.sdml.info/srcML/src\" xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" dir=\"test\" filename=\"project\" version=\"1\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt>\n</unit>\n";

  const std::string srcml_ns = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<s:unit xmlns:s=\"http://www.sdml.info/srcML/src\">\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"a.cpp\"><s:expr_stmt><s:expr><s:name>a</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n<s:unit xmlns:cpp=\"http://www.sdml.info/srcML/cpp\" language=\"C++\" filename=\"b.cpp\"><s:expr_stmt><s:expr><s:name>b</s:name></s:expr>;</s:expr_stmt>\n</s:unit>\n\n</s:unit>\n";

  std::ofstream srcml_file("project.xml");
  srcml_file << srcml;
  srcml_file.close();

  std::ofstream srcml_file_single("project_single.xml");
  srcml_file_single << srcml_single;
  srcml_file_single.close();

  std::ofstream srcml_file_ns("project_ns.xml");
  srcml_file_ns << srcml_ns;
  srcml_file_ns.close();

  /* 
     srcml_read_open_filename
  */

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_filename(archive, "project.xml");

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(srcml_archive_get_filename(archive) == std::string("project"));
  assert(srcml_archive_get_directory(archive) == std::string("test"));
  assert(srcml_archive_get_version(archive) == std::string("1"));
  assert(srcml_archive_get_options(archive) == 0);

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_filename(archive, "project_single.xml");

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(srcml_archive_get_language(archive) == std::string("C++"));
  assert(srcml_archive_get_filename(archive) == std::string("project"));
  assert(srcml_archive_get_directory(archive) == std::string("test"));
  assert(srcml_archive_get_version(archive) == std::string("1"));
  assert(srcml_archive_get_options(archive) == SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO);

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_filename(archive, "project_ns.xml");

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(archive->prefixes.at(0) == "s");
  assert(srcml_archive_get_options(archive) == 0);

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

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(srcml_archive_get_filename(archive) == std::string("project"));
  assert(srcml_archive_get_directory(archive) == std::string("test"));
  assert(srcml_archive_get_version(archive) == std::string("1"));
  assert(srcml_archive_get_options(archive) == 0);

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_memory(archive, srcml_single.c_str(), srcml_single.size());

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(srcml_archive_get_language(archive) == std::string("C++"));
  assert(srcml_archive_get_filename(archive) == std::string("project"));
  assert(srcml_archive_get_directory(archive) == std::string("test"));
  assert(srcml_archive_get_version(archive) == std::string("1"));
  assert(srcml_archive_get_options(archive) == SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO);

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_memory(archive, srcml_ns.c_str(), srcml_ns.size());

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(archive->prefixes.at(0) == "s");
  assert(srcml_archive_get_options(archive) == 0);

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

  /* 
     srcml_read_open_FILE
  */

  {
  FILE * file = fopen("project.xml", "r");

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_FILE(archive, file);

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(srcml_archive_get_filename(archive) == std::string("project"));
  assert(srcml_archive_get_directory(archive) == std::string("test"));
  assert(srcml_archive_get_version(archive) == std::string("1"));
  assert(srcml_archive_get_options(archive) == 0);

  srcml_close_archive(archive);
  srcml_free_archive(archive);
  fclose(file);

  }

  {
  FILE * file = fopen("project_single.xml", "r");

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_FILE(archive, file);

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(srcml_archive_get_language(archive) == std::string("C++"));
  assert(srcml_archive_get_filename(archive) == std::string("project"));
  assert(srcml_archive_get_directory(archive) == std::string("test"));
  assert(srcml_archive_get_version(archive) == std::string("1"));
  assert(srcml_archive_get_options(archive) == SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO);

  srcml_close_archive(archive);
  srcml_free_archive(archive);
  fclose(file);

  }

  {

  FILE * file = fopen("project_ns.xml", "r");

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_FILE(archive, file);

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(archive->prefixes.at(0) == "s");
  assert(srcml_archive_get_options(archive) == 0);

  srcml_close_archive(archive);
  srcml_free_archive(archive);
  fclose(file);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_read_open_FILE(archive, 0) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
  FILE * file = fopen("project_ns.xml", "r");
  assert(srcml_read_open_FILE(0, file) == SRCML_STATUS_ERROR);
  fclose(file);
  }

  /* 
     srcml_read_open_fd
  */

  {
  int fd = open("project.xml", O_RDONLY);

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_fd(archive, fd);

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(srcml_archive_get_filename(archive) == std::string("project"));
  assert(srcml_archive_get_directory(archive) == std::string("test"));
  assert(srcml_archive_get_version(archive) == std::string("1"));
  assert(srcml_archive_get_options(archive) == 0);

  srcml_close_archive(archive);
  srcml_free_archive(archive);
  close(fd);

  }

  {
  int fd = open("project_single.xml", O_RDONLY);

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_fd(archive, fd);

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(srcml_archive_get_language(archive) == std::string("C++"));
  assert(srcml_archive_get_filename(archive) == std::string("project"));
  assert(srcml_archive_get_directory(archive) == std::string("test"));
  assert(srcml_archive_get_version(archive) == std::string("1"));
  assert(srcml_archive_get_options(archive) == SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO);

  srcml_close_archive(archive);
  srcml_free_archive(archive);
  close(fd);

  }

  {

  int fd = open("project_ns.xml", O_RDONLY);

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_fd(archive, fd);

  assert(archive->type == SRCML_ARCHIVE_READ);
  assert(archive->reader != 0);
  assert(archive->prefixes.at(0) == "s");
  assert(srcml_archive_get_options(archive) == 0);

  srcml_close_archive(archive);
  srcml_free_archive(archive);
  close(fd);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_read_open_fd(archive, -1) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
  int fd = open("project_ns.xml", O_RDONLY);
  assert(srcml_read_open_fd(0, fd) == SRCML_STATUS_ERROR);
  close(fd);
  }

  unlink("project.xml");
  unlink("project_single.xml");
  unlink("project_ns.xml");

  return 0;

}
