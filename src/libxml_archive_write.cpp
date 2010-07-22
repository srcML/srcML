#include "libxml_archive_write.h"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <fnmatch.h>
#include <archive.h>
#include <archive_entry.h>
#include <string>
#include <time.h>

static const char* output_format = 0;

void archiveWriteOutputFormat(const char* format) {

  output_format = format;
}

static const int NUMARCHIVES = 4;
static const char * ARCHIVE_FILTER_EXTENSIONS[] = {"tar", "zip", "tgz", "cpio", "gz", "bz2", 0};

static struct archive *wa;
static struct archive_entry *wentry;
static std::string root_filename;
static std::string filename;

static std::string data;

static bool isstdout = false;

/* A table that maps extensions to functions. */
static struct { const char *extension; int (*setter)(struct archive *); } extensions[] =
  {
    { "gz",archive_write_set_compression_gzip },
    { "bz2",archive_write_set_compression_bzip2 },
    { "cpio",archive_write_set_format_cpio },
    { "tar",archive_write_set_format_pax_restricted },
#if ARCHIVE_VERSION_STAMP >= 2008000
    { "zip",archive_write_set_format_zip },
#endif
    { 0,0 }
  };

int archive_write_set_by_name(struct archive *wa, const char *extension)
{
  int i;

  for (i = 0; extensions[i].extension != NULL; i++) {
    if (strcmp(extension, extensions[i].extension) == 0)
      return ((extensions[i].setter)(wa));
  }

  return (ARCHIVE_FATAL);
}


// check if archive matches the protocol on the URI
int archiveWriteMatch_src2srcml(const char * URI) {
  
  if (URI == NULL)
      return 0;

  return
    (fnmatch("*.tar", URI, 0) == 0) ||
    (fnmatch("*.tar.gz", URI, 0) == 0) ||
    (fnmatch("*.tar.bz2", URI, 0) == 0) ||

    (fnmatch("*.cpio", URI, 0) == 0) ||
    (fnmatch("*.cpio.gz", URI, 0) == 0) ||
    (fnmatch("*.cpio.bz2", URI, 0) == 0) ||

    (fnmatch("*.zip", URI, 0) == 0);
}

// check if archive matches the protocol on the URI
int archiveWriteMatch_srcml2src(const char * URI) {

  return 1;

  if (URI == NULL)
      return 0;

  // don't handle standard output (write_disk handles that)
  if (strcmp(URI, "-") == 0)
    return  0;

  // since we are only registered when needed, then anything should match
  if (!root_filename.empty())
     return 1;

  return 0;
}

// setup archive for this URI
void* archiveWriteRootOpen(const char * URI) {

  // save the root URI
  root_filename = URI;

  isstdout = root_filename == "-" && isatty(STDOUT_FILENO);

  return 0;
}

/*
  Setup the format and compression for archive wa
  based on the path.
*/
void setupArchive(const char* path, struct archive *wa) {

}


// setup archive for this URI
void* archiveWriteOpen(const char * URI) {

  if (!wa) {

    if (!isstdout) {
      wa = archive_write_new();

      // setup the desired compression
      // TODO:  Extract into method, and make more general
      std::string s = ".";
      s += output_format ? output_format : root_filename.c_str();

      int length = s.length();
      const char* extname = s.c_str();

      // set formats
      archive_write_set_format_ustar(wa);
      for(int i = length - 1 ; i >= 0; --i)
      {
	int start;
	for(start = i; start >= 0 && extname[start] != '.'; --start);

	std::string extension = "";
	for(int pos = start + 1; pos < i + 1; ++pos)
	  extension += extname[pos];
	archive_write_set_by_name(wa, extension.c_str());

	i = start;
      }

      /*      if (!fnmatch("*.gz", extname, 0))
	archive_write_set_compression_gzip(wa);
      else if (!fnmatch("*.bz2", extname, 0))
	archive_write_set_compression_bzip2(wa);

      // setup the desired format
      // TODO:  Extract into method, and make more general
#if ARCHIVE_VERSION_STAMP >= 2008000
      if (!fnmatch("*.zip", extname, 0) || !fnmatch("*.zip.*", extname, 0))
	archive_write_set_format_zip(wa);
#else
      if (false)
      	;
#endif
      else if (fnmatch("*.cpio", extname, 0) == 0 || fnmatch("*.cpio.*", extname, 0) == 0)
	archive_write_set_format_cpio(wa);
      else
	archive_write_set_format_ustar(wa);
      */

      archive_write_open_filename(wa, root_filename.c_str());
    } else {
       wa = archive_write_disk_new();
    }
  }

  filename = URI;

  data.clear();

  return wa;
}

// read from the URI
int archiveWrite(void * context, const char * buffer, int len) {

  data.append(buffer, len);

  return len;
}

// close the open file
int archiveWriteClose(void * context) {

  if (!wentry) {
    wentry = archive_entry_new();
    archive_entry_set_filetype(wentry, AE_IFREG);
    archive_entry_set_perm(wentry, 0644);
    time_t now = time(NULL);
    //    archive_entry_set_birthtime(wentry, now, 0);
    archive_entry_set_atime(wentry, now, 0);
    archive_entry_set_ctime(wentry, now, 0);
    archive_entry_set_mtime(wentry, now, 0);
  }

  archive_entry_set_pathname(wentry, filename.c_str());
  archive_entry_set_size(wentry, data.size());
  archive_write_header(wa, wentry);
  archive_write_data(wa, data.c_str(), data.size());
  //  archive_entry_free(wentry);
  //  wentry = 0;
  //  archive_entry_clear(wentry);

  return 1;
}

int archiveWriteRootClose(void * context) {

  if (wa) {
    archive_entry_free(wentry);
    archive_write_close(wa);
    archive_write_finish(wa);
  }

  wa = 0;
  root_filename.clear();
  filename.clear();

  return 1;
}
