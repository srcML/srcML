/*
  archivewritedisk.cpp

  Copyright (C) 2010  SDML (www.sdml.info)                                                                                                                                                

  This file is part of the srcML translator.                                                                                                                                                   

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

#include <archive.h>
#include <archive_entry.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cstring>

int main(int argc, const char **argv)
{
  struct archive *a;
  struct archive_entry *entry;
  struct stat st;
  char buff[8192];
  int len;
  int fd;

  a = archive_write_disk_new();
  //  archive_write_set_format_pax_restricted(a);
  //  archive_write_set_compression_gzip(a);
  entry = archive_entry_new();
  archive_entry_set_pathname(entry, argv[1]);
  archive_entry_set_filetype(entry, AE_IFREG);
  archive_entry_set_perm(entry, 0644);
  archive_write_header(a, entry);

  strcpy(buff, "Hello!");
  archive_write_data(a, buff, 7);
  archive_entry_free(entry);

  archive_write_close(a);
  archive_write_finish(a); // Note 5

  return 0;
}
