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
#include <srcml_input_srcml.hpp>

#include <list>
#include <deque>
#include <vector>
#include <archive.h>
#include <archive_entry.h>

int src_input_filesystem(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          const std::string& input) {

    // get a list of files (including directories) from the current directory
    std::vector<std::string> files;

    // start at the root of the tree
    auto darchive = archive_read_disk_new();
#if ARCHIVE_VERSION_NUMBER >= 3002003
    archive_read_disk_set_behavior(darchive, ARCHIVE_READDISK_NO_ACL | ARCHIVE_READDISK_NO_XATTR | ARCHIVE_READDISK_NO_FFLAGS);
#elif ARCHIVE_VERSION_NUMBER >= 3002000
    archive_read_disk_set_behavior(darchive, ARCHIVE_READDISK_NO_XATTR);
#endif
    archive_read_disk_open(darchive, input.c_str());
    archive_entry* entry = nullptr;
    bool first = true;
    int status = ARCHIVE_OK;
    while ((status = archive_read_next_header(darchive, &entry)) == ARCHIVE_OK) {

        std::string filename = archive_entry_pathname(entry);

        // do not descend into . directories
        if (filename[filename.find_last_of("/") + 1] == '.') {
            continue;
        }

        archive_read_disk_descend(darchive);
        if (first) {
            first = false;
            continue;
        }

        if (archive_entry_filetype(entry) != AE_IFREG)
            continue;

        if (srcml_request.command & SRCML_COMMAND_PARSER_TEST) {
            if (filename.substr(filename.find_last_of(".") + 1) != "xml")
                continue;
        }

        files.push_back(archive_entry_pathname(entry));
    }
    archive_read_close(darchive);

    std::sort(files.begin(), files.end());

    for (auto& filename : files) {

        srcml_input_src input_file(filename);

        // If a directory contains archives skip them
        if (!(srcml_request.command & SRCML_COMMAND_PARSER_TEST) && !(input_file.archives.empty())) {
            input_file.skip = true;
        }

        if (srcml_request.command & SRCML_COMMAND_PARSER_TEST) {
            srcml_input_srcml(queue, srcml_arch, srcml_request, input_file, srcml_request.revision);
        } else {
            src_input_libarchive(queue, srcml_arch, srcml_request, input_file);
        }
    }

    return 1;
}
