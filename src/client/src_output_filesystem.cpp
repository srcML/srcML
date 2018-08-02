/**
 * @file src_output_filesystem.cpp
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

#include <src_output_filesystem.hpp>
#include <srcml.h>
#include <iostream>
#include <archive.h>
#include <archive_entry.h>

static __LA_MODE_T curperm() {

    // permissions of current directory
    auto darchive = archive_read_disk_new();
    archive_read_disk_set_behavior(darchive, ARCHIVE_READDISK_NO_ACL | ARCHIVE_READDISK_NO_XATTR | ARCHIVE_READDISK_NO_FFLAGS);
    archive_read_disk_open(darchive, ".");
    archive_entry* dentry = nullptr;
    archive_read_next_header(darchive, &dentry);
    auto perm = archive_entry_perm(dentry);
//    archive_read_free(darchive);
//    archive_entry_free(dentry);

    return perm;
}

static void ourmkdir(const std::string& path, __LA_MODE_T perm) {

    // assume path is directory name (don't split yet)
    std::string curdirname = path;

    // create new directory
    auto a = archive_write_disk_new();
    auto entry = archive_entry_new();
    archive_entry_set_pathname(entry, curdirname.c_str());
    archive_entry_set_filetype(entry, AE_IFDIR);
    archive_entry_set_perm(entry, perm);
    archive_write_header(a, entry);
    archive_write_finish_entry(a);
    archive_write_free(a);
    archive_entry_free(entry);
}

void src_output_filesystem(srcml_archive* srcml_arch, const std::string& output_dir, TraceLog& log) {

    // construct the relative directory
    std::string prefix;
    if (output_dir != "." && output_dir != "./")
        prefix = output_dir;

    auto perm = curperm();

    int count = 0;
    while (srcml_unit* unit = srcml_archive_read_unit(srcml_arch)) {

        const char* cfilename = srcml_unit_get_filename(unit);
        // @todo What do we do with no filename?
        if (!cfilename)
            continue;

        std::string filename = cfilename;

        // separate pathname from filename
        std::string path = filename.rfind('/') != std::string::npos ? filename.substr(0, filename.rfind('/')) : "";

        // construct the relative directory
        std::string fullpath = prefix;
        if (!path.empty()) {
            fullpath += '/';
            fullpath += path;
        }

        ourmkdir(fullpath, perm);

        std::string fullfilename = prefix;
        fullfilename += "/";
        fullfilename += filename;

        // unparse directory to filename
        log << ++count << fullfilename;

        srcml_unit_unparse_filename(unit, fullfilename.c_str());

        srcml_unit_free(unit);
    }
}
