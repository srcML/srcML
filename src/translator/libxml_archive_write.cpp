/*
  libxml_archive_write.cpp

  Copyright (C) 2010-2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "libxml_archive_write.hpp"
#include <stdio.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <archive.h>
#include <archive_entry.h>
#include <string>
#include <time.h>
#include <algorithm>

#if defined(__MINGW32__) || defined(LIBSRCML_COMPILER_IS_MSVC)
#include <io.h>
#endif

struct archive_write_t {

    struct archive *wa;
    struct archive_entry *wentry;
    std::string root_filename;
    std::string filename;
    std::string data;
    bool isstdout;
    const char* output_format;
};

class UnsupportedFormat {};

static const char* output_format = 0;

void archiveWriteOutputFormat(const char* format) {

    output_format = format;
}

archive_write_t archive_data = { 0, 0, "", "", "", false, 0 };

/* A table that maps compressions to functions. */
static struct { const char *compression; int (*setter)(struct archive *); } compressions[] =
{
#if ARCHIVE_VERSION_NUMBER <= 3000200
    { "gz",  archive_write_set_compression_gzip },
    { "bz2", archive_write_set_compression_bzip2 },
    { "tgz", archive_write_set_compression_gzip },
#else
    { "gz",  archive_write_add_filter_gzip },
    { "bz2", archive_write_add_filter_bzip2 },
    { "tgz", archive_write_add_filter_gzip },
#endif
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
#if ARCHIVE_VERSION_NUMBER >= 2008000
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

    // reversed copy of the path
    std::string path(URI);
    std::reverse(path.begin(), path.end());

    // see if the extension is for a source archive
    const char* tails[] = { ".tar", ".tar.gz", ".tar.bz2", ".cpio", ".cpio.gz", ".cpio.bz2", ".zip"};

    for (unsigned int i = 0; i < sizeof(tails) / sizeof(tails[0]); ++i)
        if (strcmp(URI + strlen(URI) - strlen(tails[i]), tails[i]) == 0)
            return 1;

    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// check if archive matches the protocol on the URI
int archiveWriteMatch_srcml2src(const char * URI) {

    return 1;
}

#pragma GCC diagnostic pop

// setup archive for this URI
void* archiveWriteRootOpen(const char * URI) {

    // save the root URI
    archive_data.root_filename = URI;

    archive_data.isstdout = archive_data.root_filename == "-" && isatty(
    #ifdef LIBSRCML_COMPILER_IS_MSVC
        _fileno(stdout)
    #else
        STDOUT_FILENO
    #endif
    );

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

    if (!archive_data.wa) {

        archive_data.wentry =  0;
        if (!archive_data.isstdout) {
            archive_data.wa = archive_write_new();
            if (!setupArchive(archive_data.wa, output_format ? std::string(".").append(output_format).c_str() : archive_data.root_filename.c_str())) {
                fprintf(stderr, "Invalid or unsupported format/compression\n");
                throw UnsupportedFormat();
                return 0;
            }
            // open by direct filename, or stdout ("" instead of "-")
            archive_write_open_filename(archive_data.wa, archive_data.root_filename == "-" ? "" : archive_data.root_filename.c_str());

        } else {
            archive_data.wa = archive_write_disk_new();
        }

        //fprintf(stderr, "Format: %s\n", archive_format_name(wa));
        //fprintf(stderr, "Compression: %s\n", archive_compression_name(wa));
    }

    archive_data.filename = URI;

    archive_data.data.clear();

    return archive_data.wa;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// read from the URI
int archiveWrite(void * context, const char * buffer, int len) {

    archive_data.data.append(buffer, len);

    return len;
}


// close the open file
int archiveWriteClose(void * context) {

    if (!archive_data.wentry) {
        archive_data.wentry = archive_entry_new();
        archive_entry_set_filetype(archive_data.wentry, AE_IFREG);
        archive_entry_set_perm(archive_data.wentry, 0644);
        time_t now = time(NULL);
        //    archive_entry_set_birthtime(wentry, now, 0);
        archive_entry_set_atime(archive_data.wentry, now, 0);
        archive_entry_set_ctime(archive_data.wentry, now, 0);
        archive_entry_set_mtime(archive_data.wentry, now, 0);
    }

    archive_entry_set_pathname(archive_data.wentry, archive_data.filename.c_str());
    archive_entry_set_size(archive_data.wentry, archive_data.data.size());
    archive_write_header(archive_data.wa, archive_data.wentry);
    archive_write_data(archive_data.wa, archive_data.data.c_str(), archive_data.data.size());

    return 1;
}

int archiveWriteRootClose(void * context) {

    if (archive_data.wa) {
        archive_entry_free(archive_data.wentry);
        archive_write_close(archive_data.wa);
#if ARCHIVE_VERSION_NUMBER <= 3000200
        archive_write_finish(archive_data.wa);
#else
        archive_write_free(archive_data.wa);
#endif
    }

    archive_data.wa = 0;
    archive_data.root_filename.clear();
    archive_data.filename.clear();

    return 1;
}

#pragma GCC diagnostic pop
