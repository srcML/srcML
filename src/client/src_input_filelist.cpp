// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_filelist.cpp
 *
 * @copyright Copyright (C) 2014-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <src_input_filelist.hpp>
#include <src_input_libarchive.hpp>
#include <src_input_file.hpp>
#include <src_input_filesystem.hpp>
#include <create_srcml.hpp>
#include <libarchive_utilities.hpp>
#include <iostream>
#include <archive.h>
#include <archive_entry.h>
#include <SRCMLStatus.hpp>
#include <filesystem>
#include <sstream>

int src_input_filelist(ParseQueue& queue,
                        srcml_archive* srcml_arch,
                        const srcml_request_t& srcml_request,
                        std::string_view input_file,
                        const srcml_output_dest& destination) {

    std::unique_ptr<archive> arch(libarchive_input_file(srcml_input_src(input_file)));
    if (!arch) {
        SRCMLstatus(ERROR_MSG, "srcml: Unable to open file " + std::string(input_file));
        return -1;
    }

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
        SRCMLstatus(ERROR_MSG, "srcml: Invalid filelist " + std::string(input_file));
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
        vbuffer.reserve(static_cast<std::size_t>(archive_entry_size(entry)));

    // read the file into a buffer
    const char* buffer;
    size_t size;
    int64_t offset;
    while (status == ARCHIVE_OK && archive_read_data_block(arch.get(), (const void**) &buffer, &size, &offset) == ARCHIVE_OK) {
           vbuffer.insert(vbuffer.end(), buffer, buffer + size);
    }


    // Process the buffer line by line using string_view to avoid extra copies
    std::string_view data(vbuffer.data(), vbuffer.size());
    size_t start_pos = 0;
    size_t end_pos;

    while (start_pos < data.size()) {
        // Find the next newline character
        end_pos = data.find_first_of("\n\r", start_pos);
        std::string_view sline;
        
        if (end_pos != std::string_view::npos) {
            sline = data.substr(start_pos, end_pos - start_pos);
            // Advance start_pos past the newline
            start_pos = end_pos + 1;
            // Handle CRLF sequence
            if (start_pos < data.size() && data[end_pos] == '\r' && data[start_pos] == '\n') {
                start_pos++;
            }
        } else {
            // Last line without a trailing newline
            sline = data.substr(start_pos);
            start_pos = data.size();
        }

        // Trim whitespace from both ends
        const std::string_view WHITESPACE = " \n\r\t\f\v";
        auto first = sline.find_first_not_of(WHITESPACE);
        if (first == std::string_view::npos) continue; // Skip empty/whitespace-only lines
        
        auto last = sline.find_last_not_of(WHITESPACE);
        sline = sline.substr(first, last - first + 1);

        // Skip comment lines
        if (sline[0] == '#')
            continue;

        srcml_input_src input(sline);

        // Verify that the file entry is not the same as the file list itself
        std::error_code ec;
        if (std::filesystem::equivalent(input.resource, input_file, ec)) {
            SRCMLstatus(WARNING_MSG, "srcml: WARNING Filelist entry duplicate of filelist: " + std::string(input_file));
            continue;
        }

        // Dispatch the file for processing
        auto fileStatus = srcml_handler_dispatch(queue, srcml_arch, srcml_request, input, destination);
        if (fileStatus == -1) {
            // Failure inside the list propagates a non-zero exit status
            return -1;
        }
    }

    return 1;
}
