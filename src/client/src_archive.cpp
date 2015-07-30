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
#include <string.h>
#include <errno.h>
#include <string>
#include <algorithm>

// Extension that map to archive types
struct archive_calls_t { char const * const name; int (*setter)(struct archive *); };

#ifdef __clang__
#if !__has_feature(cxx_constexpr)
#define constexpr const
#endif
#else
#define constexpr const
#endif

// TEMP_NOTE: To turn this feature on, enable "-std=c++11" by setting line 160
// to: set(CMAKE_CXX_FLAGS "-fPIC -O3 -std=c++11 ${CLANG_WARNINGS}")
// in srcML/CMake/config.cmake
#ifdef __clang__
#if __has_feature(cxx_constexpr)

constexpr bool isequal(char const* s1, char const* s2) {
  return *s1 && *s2 ? *s1 == *s2 && isequal(s1 + 1, s2 + 1) : !*s1 && !*s2;
}

constexpr bool islessthanorequal(char const* s1, char const* s2) {
  return *s1 && *s2 ? (*s1 != *s2 ? *s1 < *s2 : islessthanorequal(s1 + 1, s2 + 1)) : !*s1;
}
static_assert(islessthanorequal("a", "a"), "islessthanorequal failure");
static_assert(islessthanorequal("a", "b"), "islessthanorequal failure");
static_assert(!islessthanorequal("b", "a"), "islessthanorequal failure");
static_assert(islessthanorequal("aa", "aa"), "islessthanorequal failure");
static_assert(islessthanorequal("aa", "ab"), "islessthanorequal failure");
static_assert(!islessthanorequal("ab", "aa"), "islessthanorequal failure");

constexpr bool isordered(const archive_calls_t* p) {
    return !(p->name) || !((p + 1)->name) ? true : islessthanorequal(p->name, (p+1)->name) && isordered(p + 1);
}

#endif
#endif

// map from file extension to libarchive write format calls
// Note: Must be ordered
static constexpr archive_calls_t format_calls[] = {
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
    { NULL, NULL }
};
#ifdef __clang__
#if __has_feature(cxx_constexpr)
static_assert(isordered(format_calls), "format_calls is not ordered");
#endif
#endif

// map from file extension to libarchive write compression calls
// Note: Must be ordered
static constexpr archive_calls_t compression_calls[] = {
    { ".bz"  , 0 },
    
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

    { NULL, NULL }
};
#ifdef __clang__
#if __has_feature(cxx_constexpr)
static_assert(isordered(compression_calls), "compression_calls is not ordered");
#endif
#endif

bool compare(const archive_calls_t& call, const char* extension) {

    return strcmp(extension, call.name) == 0;
}

int archive_write_set_format_by_extension(struct archive* ar, const char* extension) {

    const archive_calls_t* end = format_calls + sizeof(format_calls) / sizeof(format_calls[0]) - 1;
    archive_calls_t searchvalue = { extension, 0 };

    const archive_calls_t* extcall = std::lower_bound(format_calls, end, searchvalue);
    if (extcall != end && extcall->setter)
        return (extcall->setter)(ar);

    archive_set_error(ar, EINVAL, "No such format for this extension '%s'", extension);
    return ARCHIVE_FATAL;
}

int archive_write_set_compression_by_extension(struct archive* ar, const char* extension) {

    const archive_calls_t* end = compression_calls + sizeof(compression_calls) / sizeof(compression_calls[0]) - 1;
    archive_calls_t searchvalue = { extension, 0 };

    const archive_calls_t* extcall = std::lower_bound(compression_calls, end, searchvalue);
    if (extcall != end && extcall->setter)
        return (extcall->setter)(ar);

    archive_set_error(ar, EINVAL, "No such compression for this extension '%s'", extension);
    return ARCHIVE_FATAL;
}

bool operator<(const archive_calls_t& call, const archive_calls_t& call2) {

    return strcmp(call.name, call2.name) < 0;
}

bool is_archive(const std::string& extension) {

    const archive_calls_t* end = format_calls + sizeof(format_calls) / sizeof(format_calls[0]) - 1;
    archive_calls_t searchvalue = { extension.c_str(), 0 };

    return std::binary_search(format_calls, end, searchvalue);
}

bool is_compressed(const std::string& extension) {

    const archive_calls_t* end = compression_calls + sizeof(compression_calls) / sizeof(compression_calls[0]) - 1;
    archive_calls_t searchvalue = { extension.c_str(), 0 };

    return std::binary_search(compression_calls, end, searchvalue);
}
