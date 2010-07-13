#include "libxml_archive_io.h"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <fnmatch.h>
#include <archive.h>
#include <archive_entry.h>

static struct archive* a = 0;

// check if archive matches the protocol on the URI
int archiveMatch(const char * URI) {

  return (URI != NULL) && (
    (fnmatch("*.tar", URI, 0) == 0) ||
    (fnmatch("*.bz2", URI, 0) == 0));
}

// setup archive for this URI
void* archiveOpen(const char * URI) {

  if (!archiveMatch(URI))
    return NULL;

  if (!a) {
    a = archive_read_new();
    archive_read_support_compression_all(a);
    archive_read_support_format_raw(a);

    int r = archive_read_open_filename(a, URI, 4000);
    if (r != ARCHIVE_OK)
      return 0;

    struct archive_entry* ae;
    r = archive_read_next_header(a, &ae);
    if (r != ARCHIVE_OK)
      return 0;
    fprintf(stderr, "HERE4 name: %s\n", archive_entry_pathname(ae));
    fprintf(stderr, "HERE4 size: %d\n", (int)archive_entry_size(ae));
  }

  return a;
}

// close the open file
int archiveClose(void * context) {

    if (context == NULL)
      return -1;

    // prime for next read
    struct archive_entry* ae;
    int r = archive_read_next_header(a, &ae);
    if (r != ARCHIVE_OK) {
      archive_read_finish(a);  
      return 0;
    }

    return 0;
}

// read from the URI
int archiveRead(void * context, char * buffer, int len) {

  size_t size = archive_read_data(a, buffer, len);
  fprintf(stderr, "HERE4 data: %d\n", size);
  if (size < 0)
    return 0;

  if (size == 0)
    return 0;

  return size;
}
