#include "libxml_archive_io.h"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <fnmatch.h>
#include <archive.h>
#include <archive_entry.h>

static struct archive* a = 0;
static int status = 0;
static struct archive_entry* ae;

static const int NUMARCHIVES = 4;
static const char * ARCHIVE_FILTER_EXTENSIONS[] = {"tar", "zip", "tgz", "cpio", "gz", "bz2", 0};

// check if file has an archive extension
bool isArchiveRead(const char * path)
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
bool isArchiveRead() {

  return a && status == ARCHIVE_OK && (archive_format(a) != ARCHIVE_FORMAT_RAW);
}

// format (e.g., tar, cpio) of the current file
const char* archiveReadFormat() {
  return !a ? 0 : archive_format_name(a);
}

// compression (e.g., gz, bzip2) of the current file
const char* archiveReadCompression() {
  return !a ? 0 : archive_compression_name(a);
}

// check if archive matches the protocol on the URI
int archiveReadMatch(const char * URI) {

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
int archiveReadStatus() {

  return status;
}

const char* archiveReadFilename(const char* URI) {

  if (!a)
    archiveReadOpen(URI);

  return isArchiveRead() ? archive_entry_pathname(ae) : 0;
}

// setup archive for this URI
void* archiveReadOpen(const char * URI) {

  //  fprintf(stderr, "ARCHIVE_OPEN\n");

  if (!archiveReadMatch(URI))
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
int archiveReadClose(void * context) {

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
int archiveRead(void * context, char * buffer, int len) {

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

static char* pdata = 0;
static int size = 0;
static int pos = 0;
static struct archive *wa;
static struct archive_entry *wentry;
static char root_filename[512];
static char filename[512];

// check if archive matches the protocol on the URI
int archiveWriteMatch(const char * URI) {

  // fprintf(stderr, "MATCH: %s\n", URI);
  
  if(URI == NULL)
      return 0;

  if (root_filename[0] != '\0')
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

// setup archive for this URI
void* archiveWriteRootOpen(const char * URI) {
  //  fprintf(stderr, "ARCHIVE_WRITE_ROOT_OPEN: %s\n", URI);
  strcpy(root_filename, URI);
}

// setup archive for this URI
void* archiveWriteOpen(const char * URI) {

  //  fprintf(stderr, "ARCHIVE_WRITE_OPEN\n");

  if (!wa) {
    wa = archive_write_new();

    // setup the desired compression
    // TODO:  Extract into method, and make more general
    if (!fnmatch("*.gz", root_filename, 0))
      archive_write_set_compression_gzip(wa);
    else if (!fnmatch("*.bz2", root_filename, 0))
      archive_write_set_compression_bzip2(wa);
    else
      archive_write_set_compression_none(wa);

    // setup the desired format
    // TODO:  Extract into method, and make more general
    if (!fnmatch("*.zip", root_filename, 0) || !fnmatch("*.zip.*", root_filename, 0))
      archive_write_set_format_zip(wa);
      else if (!fnmatch("*.cpio", root_filename, 0) || !fnmatch("*.cpio.*", root_filename, 0))
      archive_write_set_format_cpio(wa);
    else
      archive_write_set_format_pax_restricted(wa); // Correct for (gnu) tar?

    //    fprintf(stderr, "ROOT: %s %s %s\n", root_filename, archive_compression_name(wa),
    //	    archive_format_name(wa));

    archive_write_open_filename(wa, root_filename);
  }
  pos = 0;
  strcpy(filename, URI);

  //  fprintf(stderr, "FILE: %s\n", URI);

  return wa;
}

// read from the URI
int archiveWrite(void * context, const char * buffer, int len) {

  //  fprintf(stderr, "ARCHIVE_WRITE_WRITE: %d\n", len);

  // make sure we have room
  if (pos + len >= size) {
    size = (pos + len) * 2;
    pdata = (char*) realloc(pdata, size);
  }

  memcpy(pdata + pos, buffer, len);
  pos += len;

  return len;
}

// close the open file
int archiveWriteClose(void * context) {

  //  fprintf(stderr, "ARCHIVE_WRITE_CLOSE: %d\n", pos);

  wentry = archive_entry_new();
  archive_entry_set_pathname(wentry, filename);
  archive_entry_set_size(wentry, pos);
  archive_entry_set_filetype(wentry, AE_IFREG);
  archive_entry_set_perm(wentry, 0644);
  archive_write_header(wa, wentry);
  archive_write_data(wa, pdata, pos);
  archive_entry_free(wentry);
  wentry = 0;

  return 1;
}

int archiveWriteRootClose(void * context) {

  //  fprintf(stderr, "ARCHIVE_WRITE_ROOT_CLOSE\n");

  if (wa) {
    archive_write_close(wa);
    archive_write_finish(wa);
  }
  if (pdata)
    free(pdata);

  wa = 0;
  strcpy(root_filename, "");
  strcpy(filename, "");

  return 1;
}
