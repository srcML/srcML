// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file mkDir.cpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <mkDir.hpp>

mkDir::mkDir() {

    arch = archive_write_disk_new();
    entry = archive_entry_new();
    archive_entry_set_filetype(entry, AE_IFDIR);
    archive_entry_set_perm(entry, 0744);
}

void mkDir::mkdir(std::string_view path) {

    if (last != path) {
        archive_entry_set_pathname(entry, path.data());
        archive_write_header(arch, entry);
        archive_write_finish_entry(arch);
    }
    last = path;
}

mkDir::~mkDir() {
    archive_entry_free(entry);
    archive_write_close(arch);
    archive_write_free(arch);
}
