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
    (fnmatch("*.zip", URI, 0) == 0) ||
    (fnmatch("*.tgz", URI, 0) == 0) ||
    (fnmatch("*.cpio", URI, 0) == 0) ||
    (fnmatch("*.gz", URI, 0) == 0) ||
    (fnmatch("*.bz2", URI, 0) == 0));
}

// setup archive for this URI
void* archiveOpen(const char * URI) {

  if (!archiveMatch(URI))
    return NULL;

  if (!a) {
    fprintf(stderr, "REALOPEN\n");
    a = archive_read_new();
    archive_read_support_compression_all(a);
    archive_read_support_format_all(a);

    int r = archive_read_open_filename(a, URI, 4000);
    fprintf(stderr, "OPEN: %d\n", r);
    if (r != ARCHIVE_OK)
      return 0;

    struct archive_entry* ae;
    r = archive_read_next_header(a, &ae);
    if (r != ARCHIVE_OK)
      return 0;
  }

  return a;
}

// close the open file
int archiveClose(void * context) {

    if (context == NULL)
      return -1;

    struct archive_entry* ae;
    int r = archive_read_next_header(a, &ae);
    if (r == ARCHIVE_OK)
      return 0;

    archive_read_finish(a);  
    fprintf(stderr, "REALCLOSE\n");

    return 0;
}

// read from the URI
int archiveRead(void * context, char * buffer, int len) {

  size_t size = archive_read_data(a, buffer, len);
  if (size < 0)
    return 0;

  if (size == 0)
    return 0;

  fprintf(stderr, "SIZE: %d\n", size);

  return size;
}
