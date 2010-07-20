#include "libxml_archive_write.h"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <fnmatch.h>
#include <archive.h>
#include <archive_entry.h>
#include <string>

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

// check if archive matches the protocol on the URI
int archiveWriteMatch_src2srcml(const char * URI) {
  
  //  fprintf(stderr, "SRC2SRCML MATCH: %s %s\n", URI, root_filename.c_str());
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

  // fprintf(stderr, "MATCH: %s %s\n", URI, root_filename.c_str());

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
  // fprintf(stderr, "ARCHIVE_WRITE_ROOT_OPEN: %s\n", URI);

  // save the root URI
  root_filename = URI;

  isstdout = root_filename == "-";

  return 0;
}

// setup archive for this URI
void* archiveWriteOpen(const char * URI) {

  // fprintf(stderr, "ARCHIVE_WRITE_OPEN: %s\n", URI);

  if (!wa) {

    if (!isstdout) {
      wa = archive_write_new();

      // setup the desired compression
      // TODO:  Extract into method, and make more general
      const char* extname = output_format ? output_format : root_filename.c_str();
      if (!fnmatch("*.gz", extname, 0))
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
      else if (!fnmatch("*.cpio", extname, 0) || !fnmatch("*.cpio.*", extname, 0))
	archive_write_set_format_cpio(wa);
      else
	archive_write_set_format_ustar(wa);

      //    fprintf(stderr, "ROOT: %s %s %s\n", root_filename.c_str(), archive_compression_name(wa),
      //    archive_format_name(wa));

      archive_write_open_filename(wa, root_filename.c_str());
    } else {
       wa = archive_write_disk_new();
    }
  }

  filename = URI;

  data.clear();

  // fprintf(stderr, "FILE: %s\n", URI);

  return wa;
}

// read from the URI
int archiveWrite(void * context, const char * buffer, int len) {

  // fprintf(stderr, "ARCHIVE_WRITE_WRITE: %d\n", len);

  data.append(buffer, len);

  return len;
}

// close the open file
int archiveWriteClose(void * context) {

  // fprintf(stderr, "ARCHIVE_WRITE_CLOSE: %d\n", filename.size());

  wentry = archive_entry_new();
  archive_entry_set_pathname(wentry, filename.c_str());
  archive_entry_set_size(wentry, data.size());
  archive_entry_set_filetype(wentry, AE_IFREG);
  archive_entry_set_perm(wentry, 0644);
  archive_entry_set_atime(wentry, 2, 20);
  //  archive_entry_set_birthtime(wentry, 3, 30);
  archive_entry_set_ctime(wentry, 4, 40);
  archive_entry_set_mtime(wentry, 5, 50);
  archive_write_header(wa, wentry);
  archive_write_data(wa, data.c_str(), data.size());
  archive_entry_free(wentry);
  wentry = 0;

  return 1;
}

int archiveWriteRootClose(void * context) {

  // fprintf(stderr, "ARCHIVE_WRITE_ROOT_CLOSE\n");

  if (wa) {
    // fprintf(stderr, "FINISHING\n");
    archive_write_close(wa);
    archive_write_finish(wa);
  }

  wa = 0;
  root_filename.clear();
  filename.clear();

  return 1;
}
