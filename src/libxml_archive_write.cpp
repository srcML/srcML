/*
  libxml_archive_write.cpp

  Copyright (C) 2010-2011  SDML (www.sdml.info)

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

#include "libxml_archive_write.hpp"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <fnmatch.h>
#include <archive.h>
#include <archive_entry.h>
#include <string>
#include <time.h>

#ifdef __MINGW32__
#include <io.h>
#endif

class UnsupportedFormat {};

static const char* output_format = 0;

void archiveWriteOutputFormat(const char* format) {

  output_format = format;
}

static const int NUMARCHIVES = 4;
//static const char * ARCHIVE_FILTER_EXTENSIONS[] = {"tar", "zip", "tgz", "cpio", "gz", "bz2", 0};

static struct archive *wa;
static struct archive_entry *wentry;
static std::string root_filename;
static std::string filename;

static std::string data;

static bool isstdout = false;

/* A table that maps compressions to functions. */
static struct { const char *compression; int (*setter)(struct archive *); } compressions[] =
  {
    { "gz",  archive_write_set_compression_gzip },
    { "bz2", archive_write_set_compression_bzip2 },
    { "tgz", archive_write_set_compression_gzip },
    { 0,0 }
  };

int archive_write_set_compression_by_name(struct archive *wa, const char *compression)
{
  for (int i = 0; compressions[i].compression != NULL; ++i) {
    if (strcmp(compression, compressions[i].compression) == 0)
      return ((compressions[i].setter)(wa));
  }

  return (ARCHIVE_FATAL);
}

/* A table that maps formats to functions. */
static struct { const char *format; int (*setter)(struct archive *); } formats[] =
  {
    { "cpio", archive_write_set_format_cpio },
    { "tar",  archive_write_set_format_pax_restricted },
    { "tgz",  archive_write_set_format_pax_restricted },
#if ARCHIVE_VERSION_STAMP >= 2008000
    { "zip",  archive_write_set_format_zip },
#endif
    { 0,0 }
  };

int archive_write_set_format_by_name(struct archive *wa, const char *format)
{
  for (int i = 0; formats[i].format != NULL; ++i) {
    if (strcmp(format, formats[i].format) == 0)
      return ((formats[i].setter)(wa));
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
int setupArchive(struct archive* wa, const char* path) {

      // required outermost extension
      const char* outer = strrchr(path, '.');
      if (!outer || outer[0] == '\0')
	return 0;
      ++outer;

      // try to set the format based on the outermost extension
      int setarchive = archive_write_set_format_by_name(wa, outer);

      // find the innermost extension which is not required
      std::string ext2(path, outer - 1);
      const char* inner = strrchr(ext2.c_str(), '.');
      if (!inner || inner[0] == '\0')
	inner = 0;
      else
        ++inner;

      // if we still don't have a format, try the innermost extension if it exists
      if (setarchive == ARCHIVE_FATAL && inner)
	setarchive = archive_write_set_format_by_name(wa, inner);

      // if we couldn't set the format based on the last two extensions, then get out
      if (setarchive == ARCHIVE_FATAL)
        return 0;

      // try to set the compression based on the last extension (may fail)
      archive_write_set_compression_by_name(wa, outer);

      return 1;
}


// setup archive for this URI
void* archiveWriteOpen(const char * URI) {

  if (!wa) {

    wentry =  0;
    if (!isstdout) {
      wa = archive_write_new();
      if (!setupArchive(wa, output_format ? std::string(".").append(output_format).c_str() : root_filename.c_str())) {
        fprintf(stderr, "Invalid or unsupported format/compression\n");
        throw UnsupportedFormat();
        return 0;
      }
      // open by direct filename, or stdout ("" instead of "-")
      archive_write_open_filename(wa, root_filename == "-" ? "" : root_filename.c_str());

    } else {
      wa = archive_write_disk_new();
    }

    //fprintf(stderr, "Format: %s\n", archive_format_name(wa));
    //fprintf(stderr, "Compression: %s\n", archive_compression_name(wa));
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
