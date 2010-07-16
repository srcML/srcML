#include "libxml_archive_write_disk.h"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <fnmatch.h>
#include <archive.h>
#include <archive_entry.h>

static const int NUMARCHIVES = 4;
static const char * ARCHIVE_FILTER_EXTENSIONS[] = {"tar", "zip", "tgz", "cpio", "gz", "bz2", 0};

static char* pdata = 0;
static int size = 0;
static int pos = 0;
static struct archive *wa;
static struct archive_entry *wentry;
static char root_filename[512] = { 0 };
static char filename[512] = { 0 };

// check if archive matches the protocol on the URI
int archiveWriteDiskMatch(const char * URI) {

  //  fprintf(stderr, "MATCH: %s %s\n", URI, root_filename);

  if (URI == NULL)
      return 0;

  return 1;

  if (strcmp(URI, "-") == 0)
    return  0;

  if (root_filename[0])
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
void* archiveWriteDiskRootOpen(const char * URI) {
  //  fprintf(stderr, "ARCHIVE_WRITE_ROOT_OPEN: %s\n", URI);
  strcpy(root_filename, URI);
}

// setup archive for this URI
void* archiveWriteDiskOpen(const char * URI) {

  //  fprintf(stderr, "ARCHIVE_WRITE_OPEN\n");

  if (!wa) {
    wa = archive_write_disk_new();
  }

  wentry = archive_entry_new();
  archive_entry_set_pathname(wentry, URI);
  archive_entry_set_filetype(wentry, AE_IFREG);
  archive_entry_set_perm(wentry, 0644);
  archive_write_header(wa, wentry);

  pos = 0;
  strcpy(filename, URI);

  //fprintf(stderr, "FILE: %s\n", URI);

  return wa;
}

// read from the URI
int archiveWriteDisk(void * context, const char * buffer, int len) {

  //  fprintf(stderr, "ARCHIVE_WRITE_WRITE: %d\n", len);

  archive_write_data(wa, buffer, len);

  return len;
}

// close the open file
int archiveWriteDiskClose(void * context) {

  //  fprintf(stderr, "ARCHIVE_WRITE_CLOSE: %d\n", pos);
  archive_entry_free(wentry);
  wentry = 0;

  return 1;
}

int archiveWriteDiskRootClose(void * context) {

  //  fprintf(stderr, "ARCHIVE_WRITE_ROOT_CLOSE\n");

  if (wa) {
    archive_write_close(wa);
    archive_write_finish(wa);
  }

  wa = 0;
  strcpy(root_filename, "");
  strcpy(filename, "");

  return 1;
}
