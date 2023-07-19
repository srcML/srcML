// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file stdin_libarchive.cpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <stdin_libarchive.hpp>
#include <archive.h>
#include <SRCMLStatus.hpp>
#include <libarchive_utilities.hpp>
#include <string_view>

using namespace ::std::literals::string_view_literals;

namespace {
    bool issrcML(std::string_view buffer) {

        // short input must be source
        if (buffer.size() < "<unit"sv.size())
            return false;

        // trim leading whitespace
        buffer.remove_prefix(buffer.find_first_not_of(" \n\t"));

        // all XML constructs start with an angle bracket
        if (buffer[0] != '<')
            return false;

        // starts with an XML declaration
        if (buffer[1] == '?')
            return true;

        // look for start unit
        return buffer.substr(0, "<src:unit"sv.size()) == "<src:unit"sv ||
               buffer.substr(0, "<unit"sv.size()) == "<unit"sv;
    }
};

void open_stdin_libarchive(srcml_input_src& input_source) {

    std::unique_ptr<archive> stdinArchive(archive_read_new());

    archive_read_support_format_ar(stdinArchive.get());
    archive_read_support_format_cpio(stdinArchive.get());
    archive_read_support_format_gnutar(stdinArchive.get());
    archive_read_support_format_iso9660(stdinArchive.get());
    archive_read_support_format_tar(stdinArchive.get());
    archive_read_support_format_xar(stdinArchive.get());
    archive_read_support_format_zip(stdinArchive.get());
    archive_read_support_format_raw(stdinArchive.get());
    archive_read_support_format_empty(stdinArchive.get());

    // just a bunch of bytes
    archive_read_support_format_raw(stdinArchive.get());

    // File Formats
    archive_read_support_format_7zip(stdinArchive.get());
    archive_read_support_format_cab(stdinArchive.get());
    archive_read_support_format_lha(stdinArchive.get());
    archive_read_support_format_rar(stdinArchive.get());

    // Compressions
    archive_read_support_filter_all(stdinArchive.get());

    // block size at 16K
    const int buffer_size = 16384;

    int status = ARCHIVE_OK;
    if (contains<FILE*>(input_source)) {

        status = archive_read_open_FILE(stdinArchive.get(), input_source);
    } else if (contains<int>(input_source)) {

#if WIN32
        // In Windows, the archive_read_open_fd() does not seem to work. The input is read as an empty archive,
        // or cut short.
        // So for Windwos, convert to a FILE*. Note sure when to close the FILE*
        FILE* f = fdopen(input_source, "r");
        status = archive_read_open_FILE(stdinArchive.get(), f);
#else
        status = archive_read_open_fd(stdinArchive.get(), input_source, buffer_size);
#endif
    } else {

        status = archive_read_open_filename(stdinArchive.get(), input_source.resource.data(), buffer_size);
    }
    if (status != ARCHIVE_OK) {
        SRCMLstatus(ERROR_MSG, std::to_string(status));
        exit(1);
    }

    // read the first header
    archive_entry* firstEntryPtr;
    status = archive_read_next_header(stdinArchive.get(), &firstEntryPtr);
    std::unique_ptr<archive_entry> firstEntry(firstEntryPtr);

    // read the first block
    // offset is ignored because we are not trying to recreate a sparse input file
    const char* buffer;
    size_t size;
#if ARCHIVE_VERSION_NUMBER < 3000000
    off_t offset;
#else
    int64_t offset;
#endif
    status = archive_read_data_block(stdinArchive.get(), (const void**) &buffer, &size, &offset);

    // empty input produces wrong size
    if (archive_format(stdinArchive.get()) == ARCHIVE_FORMAT_EMPTY) {
        size = 0;
    }

    std::string_view firstBlock(buffer, size);

    // store part already read in
    input_source.preReadLibarchive = true;
    input_source.parchive = stdinArchive.release();
    input_source.pentry = firstEntry.release();
    input_source.buffer = std::move(firstBlock);
    input_source.issrcML = issrcML(input_source.buffer);
}
