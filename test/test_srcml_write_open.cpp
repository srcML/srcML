/*
  test_srcml_write_open.cpp

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

  Test cases for write open for archives
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

  /* 
     srcml_write_open_filename
  */

  {

  srcml_archive * archive = srcml_create_archive();
  srcml_write_open_filename(archive, "project.xml");

  assert(archive->type == SRCML_ARCHIVE_WRITE);
  assert(archive->translator != 0);
  assert(srcml_archive_get_options(archive) == SRCML_OPTION_ARCHIVE);

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_write_open_filename(archive, 0) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
  assert(srcml_write_open_filename(0, "project.xml") == SRCML_STATUS_ERROR);
  }

  /* 
     srcml_write_open_memory
  */

  {

  char * s;
  int size;
  srcml_archive * archive = srcml_create_archive();
  srcml_write_open_memory(archive, &s, &size);

  assert(archive->type == SRCML_ARCHIVE_WRITE);
  assert(archive->translator != 0);
  assert(srcml_archive_get_options(archive) == SRCML_OPTION_ARCHIVE);

  srcml_close_archive(archive);
  srcml_free_archive(archive);

  }

  {

  int size;
  srcml_archive * archive = srcml_create_archive();
  assert(srcml_write_open_memory(archive, 0, &size) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {

  char * s;
  srcml_archive * archive = srcml_create_archive();
  assert(srcml_write_open_memory(archive, &s, 0) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
    char * s;
    int size;
    assert(srcml_write_open_memory(0, &s, &size) == SRCML_STATUS_ERROR);
  }

  /* 
     srcml_write_open_FILE
  */

  {
  FILE * file = fopen("project.xml", "w");

  srcml_archive * archive = srcml_create_archive();
  srcml_write_open_FILE(archive, file);

  assert(archive->type == SRCML_ARCHIVE_WRITE);
  assert(archive->translator != 0);
  assert(srcml_archive_get_options(archive) == SRCML_OPTION_ARCHIVE);

  srcml_close_archive(archive);
  srcml_free_archive(archive);
  fclose(file);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_write_open_FILE(archive, 0) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
  FILE * file = fopen("project_ns.xml", "r");
  assert(srcml_write_open_FILE(0, file) == SRCML_STATUS_ERROR);
  fclose(file);
  }

  /* 
     srcml_write_open_fd
  */

  {
  int fd = open("project.xml", O_WRONLY);

  srcml_archive * archive = srcml_create_archive();
  srcml_write_open_fd(archive, fd);

  assert(archive->type == SRCML_ARCHIVE_WRITE);
  assert(archive->translator != 0);
  assert(srcml_archive_get_options(archive) == SRCML_OPTION_ARCHIVE);

  srcml_close_archive(archive);
  srcml_free_archive(archive);
  close(fd);

  }

  {

  srcml_archive * archive = srcml_create_archive();
  assert(srcml_write_open_fd(archive, -1) == SRCML_STATUS_ERROR);

  srcml_free_archive(archive);

  }

  {
  int fd = open("project_ns.xml", O_WRONLY);
  assert(srcml_write_open_fd(0, fd) == SRCML_STATUS_ERROR);
  close(fd);
  }

  unlink("project.xml");
  unlink("project_ns.xml");

  srcml_cleanup_globals();

  return 0;

}
