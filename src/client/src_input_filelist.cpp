/**
 * @file src_input_filelist.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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

#include <src_input_filelist.hpp>
#include <src_input_libarchive.hpp>
#include <src_input_file.hpp>
#include <src_input_filesystem.hpp>
#include <create_srcml.hpp>
#include <libarchive_utilities.hpp>
#include <iostream>
#include <cstring>
#include <archive.h>
#include <archive_entry.h>
#include <SRCMLStatus.hpp>

int src_input_filelist(ParseQueue& queue,
                        srcml_archive* srcml_arch,
                        const srcml_request_t& srcml_request,
                        const std::string& input_file,
                        const srcml_output_dest& destination) {

    std::unique_ptr<archive> arch(libarchive_input_file(input_file));
    if (!arch)
        return -1;

    archive_entry *entry = 0;
    int status = archive_read_next_header(arch.get(), &entry);

    if (status == ARCHIVE_EOF) {
        return 1;
    }

    // filelist cannot be a source archive, must only be compressed
    if (archive_format(arch.get()) != ARCHIVE_FORMAT_RAW && archive_format(arch.get()) != ARCHIVE_FORMAT_EMPTY) {
        SRCMLstatus(INFO_MSG, "srcml: filelist requires a non-archived file format");
        return -1;
    }

    if (status != ARCHIVE_OK) {
        SRCMLstatus(ERROR_MSG, "srcml: Invalid filelist " + input_file);
        return -1;
    }

    // ARE THE LAST TWO NECESSARY?
    // skip any directories
    if (archive_entry_filetype(entry) == AE_IFDIR) {
        SRCMLstatus(INFO_MSG, "srcml: filelist requires a non-directory file format");
        return -1;
    }

    // if we know the size, create the right sized data_buffer
    std::vector<char> vbuffer;
    if (archive_entry_size_is_set(entry))
        vbuffer.reserve(archive_entry_size(entry));

    // read the file into a buffer
    const char* buffer;
    size_t size;
    int64_t offset;
    while (status == ARCHIVE_OK && archive_read_data_block(arch.get(), (const void**) &buffer, &size, &offset) == ARCHIVE_OK) {
           vbuffer.insert(vbuffer.end(), buffer, buffer + size);
    }

    char* line = &vbuffer[0];
    while (line < &vbuffer[vbuffer.size() - 1]) {

        // find the line
        char* startline = line;
        while (*line != '\n' && line != &vbuffer[vbuffer.size() - 1])
            ++line;
        ++line;

        std::string sline(startline, line - startline);

        // trim from both ends
        const std::string WHITESPACE = " \n\r\t\f\v";
        auto start = sline.find_first_not_of(WHITESPACE);
        sline = (start == std::string::npos) ? "" : sline.substr(start);
        auto end = sline.find_last_not_of(WHITESPACE);
        sline = (end == std::string::npos) ? "" : sline.substr(0, end + 1);

        // skip empty lines
        if (sline[0] == 0)
            continue;

        // skip comment lines
        if (sline[0] == '#')
            continue;

        // process this file
        srcml_input_src input(sline);
        int status = srcml_handler_dispatch(queue, srcml_arch, srcml_request, input, destination);
        if (status == -1)
            return -1;
    }

    return 1;
}
