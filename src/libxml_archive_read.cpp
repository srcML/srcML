/*
  libxml_archive_read.cpp

  Copyright (C) 2010  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "libxml_archive_read.hpp"
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
  void* libxmlcontext;
};

static archiveData* gpcontext = 0;

bool archiveIsDir(void* context) {

  archiveData* pcontext = (archiveData*) context;

  return pcontext->ae && archive_entry_filetype(pcontext->ae) == AE_IFDIR;
}

bool isArchiveFirst(void* context) {

  archiveData* pcontext = (archiveData*) context;

  return pcontext && pcontext->first;
}

bool isAnythingOpen(void* context) {

  return gpcontext == context;
}

// check if file has an archive extension
bool isArchiveRead(void* context) {

  archiveData* pcontext = (archiveData*) context;

  bool status = pcontext && pcontext->a && pcontext->status == ARCHIVE_OK
#if ARCHIVE_VERSION_STAMP >= 2008000
    && (archive_format(pcontext->a) != ARCHIVE_FORMAT_RAW
#else
    && (archive_format(pcontext->a) != ARCHIVE_FORMAT_EMPTY
#endif
  );

        return status;
}

// format (e.g., tar, cpio) of the current file
const char* archiveReadFormat(void* context) {
  archiveData* pcontext = (archiveData*) context;

  return !pcontext->a ? 0 : archive_format_name(pcontext->a);
}

// compression (e.g., gz, bzip2) of the current file
const char* archiveReadCompression(void* context) {
  archiveData* pcontext = (archiveData*) context;

  return !pcontext->a ? 0 : archive_compression_name(pcontext->a);
}


// match the extension
int archiveReadMatchExtension(const char* URI) {

  // allow libxml to handle non-archive files encrypted with gz
  if (fnmatch("*.gz", URI, 0) == 0 &&
      fnmatch("*.tar.*", URI, 0) != 0 &&
      fnmatch("*.cpio.*", URI, 0) != 0 &&
      fnmatch("*.zip.*", URI, 0) != 0)
    return 0;

  if ((URI[0] == '-' && URI[1] == '\0') || (strcmp(URI, "/dev/stdin") == 0))
    return 0;

  for(const char** pos = ARCHIVE_FILTER_EXTENSIONS;*pos != 0; ++pos )
    {
      char pattern[10] = { 0 } ;
      strcpy(pattern, "*.");
      strcat(pattern, *pos);
      if(int match = fnmatch(pattern, URI, 0) == 0)
	return match;
     }

  return 0;
}

// check if archive matches the protocol on the URI
int archiveReadMatch(const char* URI) {

  if (URI == NULL)
      return 0;

  // when the xslt option is used, the input cannot come through libarchive/libxml
  // so fix here so that it is not used
  if (fnmatch("*.xsl", URI, 0) == 0)
    return 0;

#if ARCHIVE_VERSION_STAMP >= 2008000
  // put all input through libarchive for automatic detection of the format
  return 1;

#else
  // match archive extensions
  return archiveReadMatchExtension(URI);
#endif
}

// setup archive root for this URI
int archiveReadStatus(void* context) {

  archiveData* pcontext = (archiveData*) context;

  return pcontext->status;
}

const char* archiveReadFilename(void* context) {

  archiveData* pcontext = (archiveData*) context;

  if (!gpcontext)
    return 0;

  if (!pcontext->ae || (archiveReadStatus(context) != ARCHIVE_OK && archiveReadStatus(context) != ARCHIVE_EOF))
    return 0;

  return isArchiveRead(context) ? archive_entry_pathname(pcontext->ae) : 0;
}


 static bool ishttp = true;

static int archive_read_open_http_callback(struct archive* a,
					   void* _client_data) {

  archiveData* pcontext = (archiveData*) _client_data;

  pcontext->libxmlcontext = (archiveData*) (ishttp ? xmlNanoHTTPOpen(pcontext->root_filename.c_str(), 0) : xmlNanoFTPOpen(pcontext->root_filename.c_str()));

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

  archiveData* pcontext = (archiveData*) _client_data;

  static const int len = 4096;
  static std::vector<char> data(len);
  *_buffer = &data[0];
  int size = ishttp ? xmlNanoHTTPRead(pcontext->libxmlcontext, &data[0], len) : xmlNanoFTPRead(pcontext->libxmlcontext, &data[0], len);

  return size;
}

static int archive_read_close_http_callback(struct archive* a,
					   void* _client_data) {

  archiveData* pcontext = (archiveData*) _client_data;

  if (ishttp)
   xmlNanoHTTPClose(pcontext->libxmlcontext);
  else
    xmlNanoFTPClose(pcontext->libxmlcontext);
  return 1;
}

// setup archive for this URI
void* archiveReadOpen(const char* URI) {

  if (!archiveReadMatch(URI))
    return NULL;

  if (!gpcontext) {

    gpcontext = new archiveData;
    gpcontext->first = true;
    gpcontext->status = 0;
    gpcontext->a = archive_read_new();
    archive_read_support_compression_all(gpcontext->a);
    //    archive_read_support_compression_bzip2(gpcontext->a);
    //    archive_read_support_compression_gzip(gpcontext->a);

#if ARCHIVE_VERSION_STAMP >= 2008000
  archive_read_support_format_raw(gpcontext->a);
#endif
    archive_read_support_format_all(gpcontext->a);
    //    archive_read_support_format_tar(gpcontext->a);
    //    archive_read_support_format_zip(gpcontext->a);
    //    archive_read_support_format_cpio(gpcontext->a);

    //    int r = archive_read_open_filename(a, URI, 4000);
    ishttp = xmlIOHTTPMatch(URI);
    if (ishttp || xmlIOFTPMatch(URI)) {
      gpcontext->root_filename = URI;
      gpcontext->status = archive_read_open(gpcontext->a, gpcontext, archive_read_open_http_callback, archive_read_http_callback,
			      archive_read_close_http_callback);
    } else {
      gpcontext->status = archive_read_open_filename(gpcontext->a, strcmp(URI, "-") == 0 ? 0 : URI, 4000);
    }
    if (gpcontext->status != ARCHIVE_OK) {
      archive_read_finish(gpcontext->a);
      delete gpcontext;
      gpcontext = 0;
      return 0;
    }

    gpcontext->status = archive_read_next_header(gpcontext->a, &gpcontext->ae);
    if (gpcontext->status == ARCHIVE_EOF) {

      return gpcontext;
    } else if (gpcontext->status != ARCHIVE_OK) {

      archive_read_finish(gpcontext->a);
      gpcontext->a = 0;
      return 0;
    }
  }

  return gpcontext;
}

// close the open file
int archiveReadClose(void* context) {

  archiveData* pcontext = (archiveData*) context;

  //  if (context == NULL)
  //    return -1;

  if (pcontext->status != ARCHIVE_OK) {

    delete pcontext;
    gpcontext = 0;
    return 0;
  }

  // read the next header.  If there isn't one, then really finish
  pcontext->status = archive_read_next_header(pcontext->a, &pcontext->ae);
  pcontext->first = false;
  if (pcontext->status != ARCHIVE_OK) {

    archive_read_finish(pcontext->a);
    delete pcontext;
    gpcontext = 0;
    return 0;
  }

  return 0;
}

// read from the URI
int archiveRead(void* context, char* buffer, int len) {

  archiveData* pcontext = (archiveData*) context;

  if (pcontext->status != ARCHIVE_OK)
    return 0;

  size_t size = archive_read_data(pcontext->a, buffer, len);
  if (size < 0)
    return 0;

  if (size == 0)
    return 0;

  return size;
}
