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
  archive_write_set_format_pax_restricted(a);
  archive_write_set_compression_gzip(a);
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
