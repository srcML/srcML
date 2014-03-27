/**
 * @file archivecomp.hpp
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
 *
 * Source input from local files, stdin, and source archives for srcml parsing queue
 */

#ifndef ARCHIVECOMP_HPP
#define ARCHIVECOMP_HPP

#include <string>

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
    0
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
    0
};

bool is_archive(const std::string& input_file_extension);
bool is_compressed(const std::string& input_file_extension);

#endif
