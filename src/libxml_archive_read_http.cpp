#include "libxml_archive_read_http.h"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <fnmatch.h>
#include <archive.h>
#include <archive_entry.h>
#include <string>

static struct archive* a = 0;
static int status = 0;
static struct archive_entry* ae;

static const int NUMARCHIVES = 4;
static const char * ARCHIVE_FILTER_EXTENSIONS[] = {"tar", "zip", "tgz", "cpio", "gz", "bz2", 0};

// check if file has an archive extension
bool isArchiveReadHttp(const char * path)
{
  for(int i = 0; i < NUMARCHIVES; ++i)
  {
    char pattern[10] = { 0 };
    strcpy(pattern, "*.");
    strcat(pattern, ARCHIVE_FILTER_EXTENSIONS[i]);
    if(fnmatch(pattern, path, 0) == 0)
      return true;
    strcat(pattern, ".*");
    if(fnmatch(pattern, path, 0) == 0)
      return true;
  }

  return false;
}

// check if file has an archive extension
bool isArchiveReadHttp() {

  return a && status == ARCHIVE_OK && (archive_format(a) != ARCHIVE_FORMAT_RAW);
}

// format (e.g., tar, cpio) of the current file
const char* archiveReadHttpFormat() {
  return !a ? 0 : archive_format_name(a);
}

// compression (e.g., gz, bzip2) of the current file
const char* archiveReadHttpCompression() {
  return !a ? 0 : archive_compression_name(a);
}

// check if archive matches the protocol on the URI
int archiveReadHttpMatch(const char * URI) {

  //  fprintf(stderr, "MATCH: %s\n", URI);
  
  if(URI == NULL)
      return 0;

  if ((URI[0] == '-' && URI[1] == '\0') || (strcmp(URI, "/dev/stdin") == 0))
    return 1;

  for(const char ** pos = ARCHIVE_FILTER_EXTENSIONS; *pos != 0; ++pos )
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
int archiveReadHttpStatus() {

  return status;
}

const char* archiveReadHttpFilename(const char* URI) {

  if (!a)
    archiveReadHttpOpen(URI);

  return isArchiveReadHttp() ? archive_entry_pathname(ae) : 0;
}

// setup archive for this URI
void* archiveReadHttpOpen(const char * URI) {

  //  fprintf(stderr, "ARCHIVE_OPEN\n");

  if (!archiveReadHttpMatch(URI))
    return NULL;

  // just in case archiveOpenRoot() was not called
  if (!a) {
    //    fprintf(stderr, "REALLY OPEN\n");
    a = archive_read_new();
    //    archive_read_support_compression_all(a);
    archive_read_support_compression_bzip2(a);
    archive_read_support_compression_gzip(a);

#if ARCHIVE_VERSION_STAMP >= 2008000
  archive_read_support_format_raw(a);
#endif
  //    archive_read_support_format_all(a);
    archive_read_support_format_tar(a);
    archive_read_support_format_zip(a);
    archive_read_support_format_cpio(a);

    //    int r = archive_read_open_filename(a, URI, 4000);
    int r = archive_read_open_filename(a, strcmp(URI, "-") == 0 ? 0 : URI, 4000);
    if (r != ARCHIVE_OK)
      return 0;

    status = archive_read_next_header(a, &ae);
    if (status != ARCHIVE_OK)
      return 0;

    //fprintf(stderr, "FORMAT: %s\n", archive_format_name(a));
  }

  return a;
}

// close the open file
int archiveReadHttpClose(void * context) {

  //  fprintf(stderr, "ARCHIVE_CLOSE\n");

  if (context == NULL)
    return -1;

  if (status != ARCHIVE_OK)
    return 0;

  // read the next header.  If there isn't one, then really finish
  status = archive_read_next_header(a, &ae);
  if (status != ARCHIVE_OK) {
    //    fprintf(stderr, "REALLY CLOSE\n");
    archive_read_finish(a);
    a = 0;
    return 0;
  }

  return 0;
}

// read from the URI
int archiveReadHttp(void * context, char * buffer, int len) {

  //  fprintf(stderr, "ARCHIVE_READ\n");

  if (status != ARCHIVE_OK)
    return 0;

  size_t size = archive_read_data(a, buffer, len);
  if (size < 0)
    return 0;

  if (size == 0)
    return 0;

  //  fprintf(stderr, "SIZE: %d\n", size);

  return size;
}
