/**
 * @file src_archive.cpp
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

#include <src_archive.hpp>
#include <archive.h>
#include <string.h>
#include <errno.h>
#include <string>
#include <algorithm>
#include <boost/bind.hpp>

// Extension that map to archive types
struct archive_calls_t { const char *name; int (*setter)(struct archive *); };

static archive_calls_t format_calls[] = {
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
    { ".zip", archive_write_set_format_zip }  // (archive w/ compression)
};

// Extension that map to compression types
static archive_calls_t compression_calls[] = {
    { ".bz"  , 0 },
    { ".bz2" , archive_write_set_compression_bzip2 },
    { ".gz"  , archive_write_set_compression_gzip },
    { ".lz"  , 0 },
    { ".lzma", archive_write_set_compression_lzma },
    { ".xz"  , archive_write_set_compression_xz },
    { ".z"   , archive_write_set_compression_compress }
};

bool compare(const archive_calls_t& call, const char* extension) {

    return strcmp(extension, call.name) == 0;
}

int archive_write_set_format_by_extension(struct archive* ar, const char* extension) {

    archive_calls_t* end = format_calls + sizeof(format_calls) / sizeof(format_calls[0]);
    archive_calls_t* p = std::find_if(format_calls, end, boost::bind(compare, _1, extension));
    if (p != end && p->setter)
        return (p->setter)(ar);

    archive_set_error(ar, EINVAL, "No such format for this extension '%s'", extension);
    return ARCHIVE_FATAL;
}

int archive_write_set_compression_by_extension(struct archive* ar, const char* extension) {

    archive_calls_t* end = compression_calls + sizeof(compression_calls) / sizeof(compression_calls[0]);
    archive_calls_t* p = std::find_if(compression_calls, end, boost::bind(compare, _1, extension));
    if (p != end && p->setter)
        return (p->setter)(ar);

    archive_set_error(ar, EINVAL, "No such compression for this extension '%s'", extension);
    return ARCHIVE_FATAL;
}


bool is_archive(const std::string& extension) {

    archive_calls_t* end = format_calls + sizeof(format_calls) / sizeof(format_calls[0]);

    return std::find_if(format_calls, end, boost::bind(compare, _1, extension.c_str())) != end;
}

bool is_compressed(const std::string& extension) {

    archive_calls_t* end = compression_calls + sizeof(compression_calls) / sizeof(compression_calls[0]);

    return std::find_if(compression_calls, end, boost::bind(compare, _1, extension.c_str())) != end;
}
