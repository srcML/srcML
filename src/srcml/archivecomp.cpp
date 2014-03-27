/**
 * @file archivecomp.cpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <archivecomp.hpp>

// Extension that map to archive types
static const char* archive_extensions[] = {
#if ARCHIVE_VERSION_NUMBER >= 3000000
    ".7z",
#endif

    ".ar",

#if ARCHIVE_VERSION_NUMBER >= 3000000
    ".cab",
#endif

    ".cpio",
    ".iso",

#if ARCHIVE_VERSION_NUMBER >= 3000000
    ".lha",
    ".lzh",
#endif

    ".mtree",
    ".pax",

#if ARCHIVE_VERSION_NUMBER >= 3000000
    ".rar",
#endif

    ".shar",
    ".tar",
    ".taz",  // (archive w/ compression)
    ".tb2",  // (archive w/ compression)
    ".tbz",  // (archive w/ compression)
    ".tbz2", // (archive w/ compression)
    ".tgz",  // (archive w/ compression)
    ".tlz",  // (archive w/ compression)
    ".txz",  // (archive w/ compression)
    ".xar",
    ".zip",  // (archive w/ compression)
};

// Extension that map to compression types
static const char* compression_extensions[] = {
    ".bz"
    ".bz2",
    ".gz",
    ".lz",
    ".lzma",
    ".xz",
    ".z",
};

bool is_archive(const std::string& input_file_extension) {

    const char** end = archive_extensions + sizeof(archive_extensions) / sizeof(archive_extensions[0]);

    return std::find(archive_extensions, end, input_file_extension) != end;
}

bool is_compressed(const std::string& input_file_extension) {

    const char** end = compression_extensions + sizeof(compression_extensions) / sizeof(compression_extensions[0]);

    return std::find(compression_extensions, end, input_file_extension) != end;
}
