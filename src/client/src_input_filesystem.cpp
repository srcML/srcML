/**
 * @file src_input_filesystem.cpp
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

/*
  src_input_filesystem assigns directories to parse request
*/

#include <src_input_libarchive.hpp>
#include <src_input_filesystem.hpp>

#include <list>
#include <deque>
#include <vector>
#include <archive.h>
#include <archive_entry.h>

#if ARCHIVE_VERSION_NUMBER < 3000000
#include <boost/filesystem.hpp>
#endif

int src_input_filesystem(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          const std::string& input) {

#if ARCHIVE_VERSION_NUMBER < 3000000

    // list of directories with the input as the first one
    std::list<boost::filesystem::path> dirs(1, boost::filesystem::path(input));

    while (!dirs.empty()) {

        // prepare a list of sorted files for the back() directory
        std::vector<boost::filesystem::path> files(boost::filesystem::directory_iterator(dirs.back()),
                                                   boost::filesystem::directory_iterator());
        sort(files.begin(), files.end());
        dirs.pop_back();

        // process the files from the top directory
        for (const auto& file : files) {
            
            // TODO: Are we ignoring other types? symlinks? Should state so here.
            // Skip ALL symlinks (files or directories)
            if (is_symlink(file)) {
                continue;
            }

            // regular files are passed to the handler
            if (is_regular_file(file)) {

                //src_input_libarchive(queue, srcml_arch, srcml_request, file.string());
                srcml_input_src input_file(file.string());

                // If a directory contains archives skip them
                if (!(input_file.archives.empty())) {
                    input_file.skip = true;
                }

                src_input_libarchive(queue, srcml_arch, srcml_request, input_file);

            // directories are put at the back
            } else if (is_directory(file)) {

                dirs.push_back(file);
            }
        }
    }

#else

    // get a list of files (including directories) from the current directory
    std::vector<std::string> files;

    // start at the root of the tree
    auto darchive = archive_read_disk_new();
    archive_read_disk_set_behavior(darchive, ARCHIVE_READDISK_NO_ACL | ARCHIVE_READDISK_NO_XATTR | ARCHIVE_READDISK_NO_FFLAGS);
    archive_read_disk_open(darchive, input.c_str());

    archive_entry* entry = nullptr;
    bool first = true;
    int status = ARCHIVE_OK;
    while ((status = archive_read_next_header(darchive, &entry)) == ARCHIVE_OK) {

        archive_read_disk_descend(darchive);
        if (first) {
            first = false;
            continue;
        }

        if (archive_entry_filetype(entry) == AE_IFLNK)
            continue;

        files.push_back(archive_entry_pathname(entry));
    }
    archive_read_close(darchive);

    std::sort(files.begin(), files.end());

    for (auto& filename : files) {

        srcml_input_src input_file(filename);

        // If a directory contains archives skip them
        if (!(input_file.archives.empty())) {
            input_file.skip = true;
        }

        src_input_libarchive(queue, srcml_arch, srcml_request, input_file);
    }

#endif

    return 1;
}
