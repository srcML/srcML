// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file libarchive_utilities.hpp
 *
 * @copyright Copyright (C) 2018-2019 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_LIBARCHIVE_UTILITIES_HPP
#define INCLUDED_LIBARCHIVE_UTILITIES_HPP

#include <archive.h>
#include <archive_entry.h>
#include <memory>

// std::unique_ptr deleter functions for libarchive
// usage: std::unique<archive> p(archive_create_new());
// Call p.get() for original pointer
// Will deallocate automatically at end of std::unique_ptr lifetime
namespace std {
    template<>
    struct default_delete<archive> {
        void operator()(archive* ar) {
            archive_write_close(ar);
            archive_write_free(ar);
        }
    };

    template<>
    struct default_delete<archive_entry> {
        void operator()(archive_entry* e) {
            archive_entry_free(e);
        }
    };
}

#endif
