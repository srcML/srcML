#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include  <fcntl.h>

int main(int argc, char** argv) {
  char buff[8192];
  int bytes_read;
  struct archive *a = archive_read_disk_new();
  archive_read_disk_set_standard_lookup(a);

  struct archive_entry* ae;
  struct stat s;
  
  int fd = open("../test/cli/archive/b.cpp", O_RDONLY);
  if (fd < 0)
    return 1;

  ae = archive_entry_new();
  archive_entry_copy_sourcepath(ae, "../test/cli/archive");
  // archive_entry_set_pathname(ae, "t");
  archive_read_disk_entry_from_file(a, ae, fd, &s);
  while ((bytes_read = read(fd, buff, sizeof(buff))) > 0)
    fprintf(stdout, "%d : %s", bytes_read, buff);

  archive_read_finish(a);
  archive_entry_free(ae);

				    //  archive_read_finish(a);
}
