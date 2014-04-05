/**
 * @file archivecomp.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <archivecomp.hpp>
#include <archive.h>
#include <string.h>
#include <errno.h>
#include <string>

// Extension that map to archive types
static struct { const char *name; int (*setter)(struct archive *); } format_calls[] = {
#if ARCHIVE_VERSION_NUMBER >= 3000000
    { ".7z", 0 },
#endif

    { ".ar", 0 },

#if ARCHIVE_VERSION_NUMBER >= 3000000
    { ".cab", 0 },
#endif

    { ".cpio", 0 },
    { ".iso", 0 },

#if ARCHIVE_VERSION_NUMBER >= 3000000
    { ".lha", 0 },
    { ".lzh", 0 },
#endif

    { ".mtree", 0 },
    { ".pax", 0 },

#if ARCHIVE_VERSION_NUMBER >= 3000000
    { ".rar", 0 },
#endif

    { ".shar", 0 },
    { ".tar", archive_write_set_format_pax_restricted },
    { ".taz", 0 },  // (archive w/ compression)
    { ".tb2", 0 },  // (archive w/ compression)
    { ".tbz", 0 },  // (archive w/ compression)
    { ".tbz2", 0 }, // (archive w/ compression)
    { ".tgz", 0 },  // (archive w/ compression)
    { ".tlz", 0 },  // (archive w/ compression)
    { ".txz", 0 },  // (archive w/ compression)
    { ".xar", 0 },
    { ".zip", archive_write_set_format_zip },  // (archive w/ compression)
    { NULL, NULL }
};

// Extension that map to compression types
static struct { const char *name; int (*setter)(struct archive *); } compression_calls[] = {
    { ".bz"  , 0 },
    { ".bz2" , archive_write_set_compression_bzip2 },
    { ".gz"  , archive_write_set_compression_gzip },
    { ".lz"  , 0 },
    { ".lzma", archive_write_set_compression_lzma },
    { ".xz " , archive_write_set_compression_xz },
    { ".z"   , archive_write_set_compression_compress },
    { NULL, NULL }
};

int archive_write_set_format_by_extension(struct archive* ar, const char* extension) {

    for (int i = 0; format_calls[i].name != NULL; ++i)
        if (strcmp(extension, format_calls[i].name) == 0) {
            if (!format_calls[i].setter)
                break;
            return ((format_calls[i].setter)(ar));
        }

    archive_set_error(ar, EINVAL, "No such format for this extension '%s'", extension);
    return ARCHIVE_FATAL;
}

int archive_write_set_compression_by_extension(struct archive* ar, const char* extension) {

    for (int i = 0; compression_calls[i].name != NULL; ++i)
        if (strcmp(extension, compression_calls[i].name) == 0) {
            if (!compression_calls[i].setter)
                break;
            return ((compression_calls[i].setter)(ar));
        }

    archive_set_error(ar, EINVAL, "No such compression for this extension '%s'", extension);
    return ARCHIVE_FATAL;
}


bool is_archive(const std::string& extension) {

    for (int i = 0; format_calls[i].name != NULL; ++i)
        if (strcmp(extension.c_str(), format_calls[i].name) == 0)
            return true;

    return false;
}

bool is_compressed(const std::string& extension) {

    for (int i = 0; compression_calls[i].name != NULL; ++i)
        if (strcmp(extension.c_str(), compression_calls[i].name) == 0)
            return true;

    return false;
}
