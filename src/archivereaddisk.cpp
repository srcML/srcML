#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>

int main(int argc, char** argv) {

  struct archive *a = archive_read_disk_new();
  archive_read_disk_set_standard_lookup(a);

  struct archive_entry* ae;
  struct stat s;

  ae = archive_entry_new();
  archive_entry_set_pathname(ae, "t");
  archive_read_disk_entry_from_file(a, ae, -1, &s);
  
				    //  archive_read_finish(a);
}
