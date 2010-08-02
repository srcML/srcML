#include "libxml_archive_read.h"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <fnmatch.h>
#include <archive.h>
#include <archive_entry.h>
#include <string>
#include <libxml/nanohttp.h>
#include <libxml/nanoftp.h>
#include <vector>

static const int NUMARCHIVES = 5;
static const char* ARCHIVE_FILTER_EXTENSIONS[] = {"tar", "zip", "tgz", "cpio", "shar", "gz", "bz2", 0};

struct archiveData {
  struct archive* a;
  struct archive_entry* ae;
  int status;
  std::string root_filename;
  bool first;
};

static struct archiveData scontext = { 0, 0, 0, "", true };

bool archiveIsDir() {

  return scontext.ae && archive_entry_filetype(scontext.ae) == AE_IFDIR;
}

bool isArchiveFirst() {

  return scontext.first;
}

// check if file has an archive extension
bool isArchiveRead() {

  return scontext.a && scontext.status == ARCHIVE_OK
#if ARCHIVE_VERSION_STAMP >= 2008000
    && (archive_format(scontext.a) != ARCHIVE_FORMAT_RAW
#else
    && (archive_format(scontext.a) != ARCHIVE_FORMAT_EMPTY
#endif
  );
}

// format (e.g., tar, cpio) of the current file
const char* archiveReadFormat() {
  return !scontext.a ? 0 : archive_format_name(scontext.a);
}

// compression (e.g., gz, bzip2) of the current file
const char* archiveReadCompression() {
  return !scontext.a ? 0 : archive_compression_name(scontext.a);
}

// check if archive matches the protocol on the URI
int archiveReadMatch(const char* URI) {
  
  if (URI == NULL)
      return 0;

  if (fnmatch("*.xsl", URI, 0) == 0)
    return 0;

#if ARCHIVE_VERSION_STAMP >= 2008000
  // put all input through libarchive for automatic detection of the format
  return 1;

#else
  // allow libxml to handle non-archive files encrypted with gz
  int extpos = strlen(URI) - 3;
  if (fnmatch("*.gz", URI, 0) == 0 &&
      fnmatch("*.tar.*", URI, 0) != 0 &&
      fnmatch("*.cpio.*", URI, 0) != 0 &&
      fnmatch("*.zip.*", URI, 0) != 0)
    return 0;

  if ((URI[0] == '-' && URI[1] == '\0') || (strcmp(URI, "/dev/stdin") == 0))
    return 1;

  for(const char** pos = ARCHIVE_FILTER_EXTENSIONS;*pos != 0; ++pos )
    {
      char pattern[10] = { 0 } ;
      strcpy(pattern, "*.");
      strcat(pattern, *pos);
      if(int match = fnmatch(pattern, URI, 0) == 0)
	return match;
     }

  return 0;

#endif
}

// setup archive root for this URI
int archiveReadStatus() {

  return scontext.status;
}

const char* archiveReadFilename() {

  if (!scontext.ae || (archiveReadStatus() != ARCHIVE_OK && archiveReadStatus() != ARCHIVE_EOF))
    return 0;

  return isArchiveRead() ? archive_entry_pathname(scontext.ae) : 0;
}


static void* mcontext;
 static bool ishttp = true;

static int archive_read_open_http_callback(struct archive* a,
					   void* _client_data) {

  mcontext = ishttp ? xmlNanoHTTPOpen(scontext.root_filename.c_str(), 0) : xmlNanoFTPOpen(scontext.root_filename.c_str());

  return 0;
}

static 
#if ARCHIVE_VERSION_STAMP >= 2008000
__LA_SSIZE_T
#else
ssize_t
#endif
archive_read_http_callback(struct archive* a,
					   void* _client_data, const void** _buffer) {

  static const int len = 4096;
  static std::vector<char> data(len);
  *_buffer = &data[0];
  int size = ishttp ? xmlNanoHTTPRead(mcontext, &data[0], len) : xmlNanoFTPRead(mcontext, &data[0], len);

  return size;
}

static int archive_read_close_http_callback(struct archive* a,
					   void* _client_data) {

  if (ishttp)
    xmlNanoHTTPClose(mcontext);
  else
    xmlNanoFTPClose(mcontext);
  return 1;
}

// setup archive for this URI
void* archiveReadOpen(const char* URI) {

  if (!archiveReadMatch(URI))
    return NULL;

  scontext.first = false;

  // just in case archiveOpenRoot() was not called
  if (!scontext.a) {

    scontext.first = true;
    scontext.status = 0;
    scontext.a = archive_read_new();
    archive_read_support_compression_all(scontext.a);
    //    archive_read_support_compression_bzip2(scontext.a);
    //    archive_read_support_compression_gzip(scontext.a);

#if ARCHIVE_VERSION_STAMP >= 2008000
  archive_read_support_format_raw(scontext.a);
#endif
    archive_read_support_format_all(scontext.a);
    //    archive_read_support_format_tar(scontext.a);
    //    archive_read_support_format_zip(scontext.a);
    //    archive_read_support_format_cpio(scontext.a);

    //    int r = archive_read_open_filename(a, URI, 4000);
    ishttp = xmlIOHTTPMatch(URI);
    if (ishttp || xmlIOFTPMatch(URI)) {
      scontext.root_filename = URI;
      scontext.status = archive_read_open(scontext.a, 0, archive_read_open_http_callback, archive_read_http_callback,
			      archive_read_close_http_callback);
    } else {
      scontext.status = archive_read_open_filename(scontext.a, strcmp(URI, "-") == 0 ? 0 : URI, 4000);
    }
    if (scontext.status != ARCHIVE_OK) {
      archive_read_finish(scontext.a);
      scontext.a = 0;
      scontext.ae = 0;
      return 0;
    }

    scontext.status = archive_read_next_header(scontext.a, &scontext.ae);
    if (scontext.status != ARCHIVE_OK)
      return 0;

    //    while (archive_entry_filetype(scontext.ae) == AE_IFDIR)
    //      archiveReadClose(mcontext);
  }

  return scontext.a;
}

// close the open file
int archiveReadClose() {

  return archiveReadClose(mcontext);
}



// close the open file
int archiveReadClose(void* context) {

  //  if (context == NULL)
  //    return -1;

  if (scontext.status != ARCHIVE_OK)
    return 0;

  // read the next header.  If there isn't one, then really finish
  scontext.status = archive_read_next_header(scontext.a, &scontext.ae);
  if (scontext.status != ARCHIVE_OK) {

    archive_read_finish(scontext.a);
    scontext.a = 0;
    return 0;
  }

  return 0;
}

// read from the URI
int archiveRead(void* context, char* buffer, int len) {

  if (scontext.status != ARCHIVE_OK)
    return 0;

  size_t size = archive_read_data(scontext.a, buffer, len);
  if (size < 0)
    return 0;

  if (size == 0)
    return 0;

  return size;
}
