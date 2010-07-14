#include "libxml_archive_io.h"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <fnmatch.h>
#include <archive.h>
#include <archive_entry.h>

static struct archive* a = 0;
static bool root = false;
static int status = 0;
static struct archive_entry* ae;

static const char * ARCHIVE_EXTENSIONS[] = {"tar", "zip", "tgz", "cpio", "gz", "bz2", 0};

char s[500];

// check if archive matches the protocol on the URI
int archiveMatch(const char * URI) {

  /*
  return (URI != NULL) && (
    (fnmatch("*.tar", URI, 0) == 0) ||
    (fnmatch("*.zip", URI, 0) == 0) ||
    (fnmatch("*.tgz", URI, 0) == 0) ||
    (fnmatch("*.cpio", URI, 0) == 0) ||
    (fnmatch("*.gz", URI, 0) == 0) ||
    (fnmatch("*.bz2", URI, 0) == 0));
  */

  if(URI == NULL)
      return 0;

  for(const char ** pos = ARCHIVE_EXTENSIONS; *pos != 0; ++pos )
    {
      char pattern[10] = { 0 } ;
      strcpy(pattern, "*.");
      strcat(pattern, *pos);
      if(int match = fnmatch(pattern, URI, 0) == 0)
	return match;
     }

  return 0;
}

// setup archive root for this URI
bool archiveGood() {

  return status == ARCHIVE_OK;
}

const char* archiveFilename() {

  return s;
}

// setup archive for this URI
void* archiveOpenRoot(const char * URI) {

  //  fprintf(stderr, "ARCHIVE_OPEN_ROOT\n");

  root = true;

  if (!archiveMatch(URI))
    return NULL;

  a = archive_read_new();
  archive_read_support_compression_all(a);

#if ARCHIVE_VERSION_STAMP > 2008000
  archive_read_support_format_raw(a);
#endif
  archive_read_support_format_all(a);

  int r = archive_read_open_filename(a, URI, 4000);
  if (r != ARCHIVE_OK)
    return 0;

  status = archive_read_next_header(a, &ae);
  if (status != ARCHIVE_OK)
    return 0;

  strcpy(s, archive_entry_pathname(ae));

  return a;
}

// setup archive for this URI
void* archiveOpen(const char * URI) {

  //  fprintf(stderr, "ARCHIVE_OPEN\n");

  if (!archiveMatch(URI))
    return NULL;
  /*
  // just in case archiveOpenRoot() was not called
  if (!a) {
    a = archive_read_new();
    archive_read_support_compression_all(a);
    archive_read_support_format_raw(a);
    archive_read_support_format_all(a);

    int r = archive_read_open_filename(a, URI, 4000);
    if (r != ARCHIVE_OK)
      return 0;

    status = archive_read_next_header(a, &ae);
    if (status != ARCHIVE_OK)
      return 0;
  }
  */
  return a;
}

// close the open file
int archiveClose(void * context) {

  //  fprintf(stderr, "ARCHIVE_CLOSE\n");

  if (context == NULL)
    return -1;

  if (root) {
    status = archive_read_next_header(a, &ae);
    if (status != ARCHIVE_OK)
      return -1;
    strcpy(s, archive_entry_pathname(ae));
  } else {
    archive_read_finish(a);  
  }

  return 0;
}

// close the open file
int archiveCloseRoot(void * context) {

  //  fprintf(stderr, "ARCHIVE_CLOSE_ROOT\n");

  root = false;

  if (context == NULL)
    return -1;

  archive_read_finish(a);  
  a = 0;

  return 0;
}

// read from the URI
int archiveRead(void * context, char * buffer, int len) {

  //  fprintf(stderr, "ARCHIVE_READ\n");

  size_t size = archive_read_data(a, buffer, len);
  if (size < 0)
    return 0;

  if (size == 0)
    return 0;

  //  fprintf(stderr, "SIZE: %d\n", size);

  return size;
}
