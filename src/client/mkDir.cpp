/**
 * @file mkDir.cpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <mkDir.hpp>

mkDir::mkDir() {

    arch = archive_write_disk_new();
    entry = archive_entry_new();
    archive_entry_set_filetype(entry, AE_IFDIR);
    archive_entry_set_perm(entry, 0744);
}

void mkDir::mkdir(const std::string& path) {

    if (last != path) {
        archive_entry_set_pathname(entry, path.c_str());
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
