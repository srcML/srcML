// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_filesystem.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

/*
  src_input_filesystem assigns directories to parse request
*/

#include <src_input_libarchive.hpp>
#include <src_input_filesystem.hpp>
#include <srcml_input_srcml.hpp>

#include <string_view>
#include <list>
#include <deque>
#include <vector>
#include <archive.h>
#include <archive_entry.h>

#include <stdio.h>
#ifdef _MSC_VER
    #include <direct.h>
#else
    #include <unistd.h>
#endif

using namespace ::std::literals::string_view_literals;

int src_input_filesystem(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          std::string_view raw_input) {

    // with immediate directory "." lookup the current working directory
    std::string_view input = raw_input;
    if (input == "."sv) {
        char* cwd(getcwd(nullptr, 0));
        input = cwd;
        free(cwd);
    }

    // get a list of files (including directories) from the current directory
    std::vector<std::string> files;

    // start at the root of the tree
    auto darchive = archive_read_disk_new();
#if ARCHIVE_VERSION_NUMBER >= 3002003
    archive_read_disk_set_behavior(darchive, ARCHIVE_READDISK_NO_ACL | ARCHIVE_READDISK_NO_XATTR | ARCHIVE_READDISK_NO_FFLAGS);
#elif ARCHIVE_VERSION_NUMBER >= 3002000
    archive_read_disk_set_behavior(darchive, ARCHIVE_READDISK_NO_XATTR);
#endif
    archive_read_disk_open(darchive, input.data());

    /* Null entry with archive_read_next_header() causes a segfault on ARCHIVE_VERSION_NUMBER < 300200
       Creating an entry and using archive_read_next_header2() works */
    archive_entry* entry = archive_entry_new();
    bool first = true;
    int status = ARCHIVE_OK;
    while ((status = archive_read_next_header2(darchive, entry)) == ARCHIVE_OK) {

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
    archive_entry_free(entry);
    archive_read_free(darchive);

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
            input_file.prefix = filename.substr(0, filename.find_last_of('/'));
            src_input_libarchive(queue, srcml_arch, srcml_request, input_file);
        }
    }

    return 1;
}
