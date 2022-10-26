// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_archive.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <src_archive.hpp>
#include <archive.h>
#include <cerrno>
#include <unordered_map>
#include <string_view>

namespace {

    // map from file extension to libarchive write format calls
    const std::unordered_map<std::string_view, int (*)(struct archive *)> format_callsmap = {
        { ".7z", archive_write_set_format_7zip },
        { ".ar", archive_write_set_format_ar_bsd },
        { ".cpio", archive_write_set_format_cpio },
        { ".iso", archive_write_set_format_iso9660 },
        { ".mtree", archive_write_set_format_mtree },
        { ".pax", archive_write_set_format_pax },
        { ".shar", archive_write_set_format_shar },
        { ".tar",  archive_write_set_format_pax_restricted },
        { ".taz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".tb2",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".tbz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".tbz2", archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".tgz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".tlz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".txz",  archive_write_set_format_pax_restricted },  // (archive w/ compression)
        { ".xar",  archive_write_set_format_pax_restricted },
        { ".zip",  archive_write_set_format_zip },  // (archive w/ compression)
    };

    // map from file extension to libarchive write compression calls
    const std::unordered_map<std::string_view, int (*)(struct archive *)> compression_callsmap = {

        { ".bz2" , archive_write_add_filter_bzip2 },
        { ".gz"  , archive_write_add_filter_gzip },
        { ".lz"  , archive_write_add_filter_lzma },
        { ".lzma", archive_write_add_filter_lzma },
        { ".taz",  archive_write_add_filter_compress }, // (archive w/ compression)
        { ".tb2",  archive_write_add_filter_bzip2 },    // (archive w/ compression)
        { ".tbz2", archive_write_add_filter_bzip2 },    // (archive w/ compression)
        { ".tgz",  archive_write_add_filter_gzip },     // (archive w/ compression)
        { ".tlz",  archive_write_add_filter_lzma },     // (archive w/ compression)
        { ".txz",  archive_write_add_filter_xz },       // (archive w/ compression)
        { ".xar",  archive_write_add_filter_xz },
        { ".xz"  , archive_write_add_filter_xz },
        { ".z"   , archive_write_add_filter_compress },
    };

    // map from language to file extension
    const std::unordered_map<std::string_view, std::string_view> lang2ext = {
        { "C", ".c" },
        { "C++", ".cpp" },
        { "C#", ".cs" },
        { "Java", ".java" },
    };

}

int archive_write_set_format_by_extension(struct archive* ar, std::string_view extension) {

    auto it = format_callsmap.find(extension);
    if (it != format_callsmap.end())
        return (it->second)(ar);

    archive_set_error(ar, EINVAL, "No such format for this extension '%s'", extension.data());
    return ARCHIVE_FATAL;
}

int archive_write_set_compression_by_extension(struct archive* ar, std::string_view extension) {

    auto it = compression_callsmap.find(extension);
    if (it != compression_callsmap.end())
        return (it->second)(ar);

    archive_set_error(ar, EINVAL, "No such compression for this extension '%s'", extension.data());
    return ARCHIVE_FATAL;
}

std::string_view language_to_std_extension(std::string_view extension) {

    auto it = lang2ext.find(extension);
    if (it != lang2ext.end())
        return it->second;

    return "";
}

bool is_archive(std::string_view extension) {

    return format_callsmap.find(extension) != format_callsmap.end();
}

bool is_compressed(std::string_view extension) {

    return compression_callsmap.find(extension) != compression_callsmap.end();
}
