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


    std::string data(vbuffer.begin(), vbuffer.end());
    std::istringstream iss(data);
    std::string sline;
    while (std::getline(iss, sline)) {
        // trim from both ends
        const std::string WHITESPACE = " \n\r\t\f\v";
        auto start = sline.find_first_not_of(WHITESPACE);
        sline = (start == std::string::npos) ? "" : sline.substr(start);
        auto end = sline.find_last_not_of(WHITESPACE);
        sline = (end == std::string::npos) ? "" : sline.substr(0, end + 1);

        // skip empty or comment lines
        if (sline.empty() || sline[0] == '#')
            continue;

        srcml_input_src input(sline);

        // verify that the file entry is not the same as the file list
        std::error_code ec;
        if (std::filesystem::equivalent(input.resource, input_file, ec)) {
            SRCMLstatus(WARNING_MSG, "srcml: WARNING Filelist entry duplicate of filelist: " + std::string(input_file));
            continue;
        }

        // process this file
        auto fileStatus = srcml_handler_dispatch(queue, srcml_arch, srcml_request, input, destination);
        if (fileStatus == -1) {
            // Ensure failure inside the list propagates a non-zero exit
            return -1;
        }
    }

    return 1;
}
