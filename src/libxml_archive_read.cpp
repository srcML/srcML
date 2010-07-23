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

static const int NUMARCHIVES = 5;
static const char* ARCHIVE_FILTER_EXTENSIONS[] = {"tar", "zip", "tgz", "cpio", "shar", "gz", "bz2", 0};

static struct archive* a = 0;
static int status = 0;
static struct archive_entry* ae = 0;

static std::string root_filename;
static bool isdir = false;

// check if file has an archive extension
bool isArchiveRead() {

  return a && status == ARCHIVE_OK
#if ARCHIVE_VERSION_STAMP >= 2008000
    && (archive_format(a) != ARCHIVE_FORMAT_RAW
#else
    && (archive_format(a) != ARCHIVE_FORMAT_EMPTY
#endif
  );
}

// format (e.g., tar, cpio) of the current file
const char* archiveReadFormat() {
  return !a ? 0 : archive_format_name(a);
}

// compression (e.g., gz, bzip2) of the current file
const char* archiveReadCompression() {
  return !a ? 0 : archive_compression_name(a);
}

 bool archiveIsDir() {

   return isdir;
 }
// check if archive matches the protocol on the URI
int archiveReadMatch(const char* URI) {
  
  if (URI == NULL)
      return 0;

#if ARCHIVE_VERSION_STAMP >= 2008000
  // put all input through libarchive for automatic detection of the format
  return 1;

#else
  // allow libxml to handle non-archive files encrypted with gz
  int extpos = strlen(URI) - 3;
  if (fnmatch(URI, "*.gz", 0) == 0 &&
      fnmatch(URI, "*.tar.*", 0) != 0 &&
      fnmatch(URI, "*.cpio.*", 0) != 0 &&
      fnmatch(URI, "*.zip.*", 0) != 0)
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

  return status;
}

const char* archiveReadFilename(const char* URI) {

  if (!a)
    archiveReadOpen(URI);

  if (archiveReadStatus() != ARCHIVE_OK && archiveReadStatus() != ARCHIVE_EOF)
    return 0;

  return isArchiveRead() ? archive_entry_pathname(ae) : 0;
}


static void* mcontext;
 static bool ishttp = true;

static int archive_read_open_http_callback(struct archive* a,
					   void* _client_data) {

  mcontext = ishttp ? xmlNanoHTTPOpen(root_filename.c_str(), 0) : xmlNanoFTPOpen(root_filename.c_str());

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

  static char data[512];
  *_buffer = data;
  int len = 510;
  int size = ishttp ? xmlNanoHTTPRead(mcontext, data, len) : xmlNanoFTPRead(mcontext, data, len);

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

  // just in case archiveOpenRoot() was not called
  if (!a) {
    status = 0;
    a = archive_read_new();
    archive_read_support_compression_all(a);
    //    archive_read_support_compression_bzip2(a);
    //    archive_read_support_compression_gzip(a);

#if ARCHIVE_VERSION_STAMP >= 2008000
  archive_read_support_format_raw(a);
#endif
    archive_read_support_format_all(a);
    //    archive_read_support_format_tar(a);
    //    archive_read_support_format_zip(a);
    //    archive_read_support_format_cpio(a);

    //    int r = archive_read_open_filename(a, URI, 4000);
    ishttp = xmlIOHTTPMatch(URI);
    if (ishttp || xmlIOFTPMatch(URI)) {
      root_filename = URI;
      status = archive_read_open(a, 0, archive_read_open_http_callback, archive_read_http_callback,
			      archive_read_close_http_callback);
    } else {
      status = archive_read_open_filename(a, strcmp(URI, "-") == 0 ? 0 : URI, 4000);
    }
    if (status != ARCHIVE_OK) {
      archive_read_finish(a);
      a = 0;
      ae = 0;
      return 0;
    }

    status = archive_read_next_header(a, &ae);
    if (status != ARCHIVE_OK)
      return 0;

    mcontext = a;
    isdir = archive_entry_filetype(ae) == AE_IFDIR;
  }

  return a;
}

// close the open file
int archiveReadClose() {
  archiveReadClose(mcontext);
}

// close the open file
int archiveReadClose(void* context) {

  if (context == NULL)
    return -1;

  if (status != ARCHIVE_OK)
    return 0;

  // read the next header.  If there isn't one, then really finish
  status = archive_read_next_header(a, &ae);
  if (status != ARCHIVE_OK) {

    archive_read_finish(a);
    a = 0;
    return 0;
  }

  //  if (archive_entry_filetype(ae) == AE_IFDIR)
  //    archiveReadClose(context);

  return 0;
}

// read from the URI
int archiveRead(void* context, char* buffer, int len) {

  if (status != ARCHIVE_OK)
    return 0;

  size_t size = archive_read_data(a, buffer, len);
  if (size < 0)
    return 0;

  if (size == 0)
    return 0;

  return size;
}
