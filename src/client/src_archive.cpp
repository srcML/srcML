/**
 * @file src_archive.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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
#include <cerrno>
#include <unordered_map>

namespace {

    // map from file extension to libarchive write format calls
    std::unordered_map<std::string, int (*)(struct archive *)> format_callsmap = {
#if ARCHIVE_VERSION_NUMBER >= 3000000
        { ".7z", archive_write_set_format_7zip },
#endif
        { ".ar", archive_write_set_format_ar_bsd },
        { ".cpio", archive_write_set_format_cpio },
#if ARCHIVE_VERSION_NUMBER >= 3000000
        { ".iso", archive_write_set_format_iso9660 },
#endif
        { ".mtree", archive_write_set_format_mtree },
        { ".pax", archive_write_set_format_pax },
        { ".shar", archive_write_set_format_shar },
        { ".tar",  archive_write_set_format_pax_restricted },
        { ".taz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".tb2",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".tbz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".tbz2", archive_write_set_format_pax_restricted }, // (archive w/ compression)
        { ".tgz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".tlz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".txz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".xar",  archive_write_set_format_pax_restricted },
        { ".zip",  archive_write_set_format_zip },  // (archive w/ compression)
    };

    // map from file extension to libarchive write compression calls
    std::unordered_map<std::string, int (*)(struct archive *)> compression_callsmap = {

#if ARCHIVE_VERSION_NUMBER >= 3000000
        { ".bz2" , archive_write_add_filter_bzip2 },
        { ".gz"  , archive_write_add_filter_gzip },
        { ".lz"  , archive_write_add_filter_lzma },
        { ".lzma", archive_write_add_filter_lzma },
        { ".taz",  archive_write_add_filter_compress },  // (archive w/ compression)
        { ".tb2",  archive_write_add_filter_bzip2 },  // (archive w/ compression)
        { ".tbz2", archive_write_add_filter_bzip2 }, // (archive w/ compression)
        { ".tgz",  archive_write_add_filter_gzip },  // (archive w/ compression)
        { ".tlz",  archive_write_add_filter_lzma },  // (archive w/ compression)
        { ".txz",  archive_write_add_filter_xz },  // (archive w/ compression)
        { ".xar",  archive_write_add_filter_xz },
        { ".xz"  , archive_write_add_filter_xz },
        { ".z"   , archive_write_add_filter_compress },
#else
        { ".bz2" , archive_write_set_compression_bzip2 },
        { ".gz"  , archive_write_set_compression_gzip },
        { ".lz"  , archive_write_set_compression_lzma },
        { ".lzma", archive_write_set_compression_lzma },
        { ".taz",  archive_write_set_compression_compress },  // (archive w/ compression)
        { ".tb2",  archive_write_set_compression_bzip2 },  // (archive w/ compression)
        { ".tbz2", archive_write_set_compression_bzip2 }, // (archive w/ compression)
        { ".tgz",  archive_write_set_compression_gzip },  // (archive w/ compression)
        { ".tlz",  archive_write_set_compression_lzma },  // (archive w/ compression)
        { ".txz",  archive_write_set_compression_xz },  // (archive w/ compression)
        { ".xar",  archive_write_set_compression_xz },
        { ".xz"  , archive_write_set_compression_xz },
        { ".z"   , archive_write_set_compression_compress },
#endif
    };

    // map from language to file extension
    std::unordered_map<std::string, const char *> lang2ext = {
        { "C", ".c" },
        { "C++", ".cpp" },
        { "C#", ".cs" },
        { "Java", ".java" },
    };

}

int archive_write_set_format_by_extension(struct archive* ar, const char* extension) {

    auto it = format_callsmap.find(extension);
    if (it != format_callsmap.end())
        return (it->second)(ar);

    archive_set_error(ar, EINVAL, "No such format for this extension '%s'", extension);
    return ARCHIVE_FATAL;
}

int archive_write_set_compression_by_extension(struct archive* ar, const char* extension) {

    auto it = compression_callsmap.find(extension);
    if (it != compression_callsmap.end())
        return (it->second)(ar);

    archive_set_error(ar, EINVAL, "No such compression for this extension '%s'", extension);
    return ARCHIVE_FATAL;
}

std::string language_to_std_extension(const char* extension) {

    auto it = lang2ext.find(extension);
    if (it != lang2ext.end())
        return it->second;

    return "";
}

bool is_archive(const std::string& extension) {

    return format_callsmap.find(extension) != format_callsmap.end();
}

bool is_compressed(const std::string& extension) {

    return compression_callsmap.find(extension) != compression_callsmap.end();
}
