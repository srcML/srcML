// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file unarchive_srcml.cpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <unarchive_srcml.hpp>
#include <curl/curl.h>
#include <archive.h>
#include <input_curl.hpp>
#include <SRCMLStatus.hpp>
#include <libarchive_utilities.hpp>

void unarchive_srcml(const srcml_request_t& /* srcml_request */,
    const srcml_input_t& input_sources,
    const srcml_output_dest& destination) {

    std::unique_ptr<archive> libarchive_srcml;

    if (!input_sources[0].parchive) {

        libarchive_srcml.reset(archive_read_new());

        archive_read_support_format_ar(libarchive_srcml.get());
        archive_read_support_format_cpio(libarchive_srcml.get());
        archive_read_support_format_gnutar(libarchive_srcml.get());
        archive_read_support_format_iso9660(libarchive_srcml.get());
        archive_read_support_format_tar(libarchive_srcml.get());
        archive_read_support_format_xar(libarchive_srcml.get());
        archive_read_support_format_zip(libarchive_srcml.get());
        archive_read_support_format_raw(libarchive_srcml.get());
        archive_read_support_format_empty(libarchive_srcml.get());

        // just a bunch of bytes
        archive_read_support_format_raw(libarchive_srcml.get());

        // File Formats
        archive_read_support_format_7zip(libarchive_srcml.get());
        archive_read_support_format_cab(libarchive_srcml.get());
        archive_read_support_format_lha(libarchive_srcml.get());
        archive_read_support_format_rar(libarchive_srcml.get());

        // Compressions
        archive_read_support_filter_all(libarchive_srcml.get());

        int status = ARCHIVE_OK;
        const int buffer_size = 16384;

        if (contains<FILE*>(input_sources[0])) {

            status = archive_read_open_FILE(libarchive_srcml.get(), input_sources[0]);
        } else if (contains<int>(input_sources[0])) {

            status = archive_read_open_fd(libarchive_srcml.get(), input_sources[0], buffer_size);
        } else {

            status = archive_read_open_filename(libarchive_srcml.get(), input_sources[0].resource.data(), buffer_size);
        }
        if (status != ARCHIVE_OK) {
            SRCMLstatus(ERROR_MSG, std::to_string(status));
            exit(1);
        }
        archive_entry *entry;
        status = archive_read_next_header(libarchive_srcml.get(), &entry);
    } else {

        libarchive_srcml.reset(input_sources[0].parchive);

        // write the first block read in previous to this
        if (write(*destination.fd, input_sources[0].buffer.data(), (unsigned int) input_sources[0].buffer.size()) == -1) {
            SRCMLstatus(ERROR_MSG, "Unable to write to internal pipeline");
            exit(1);
        }
    }

    // copy from the libarchive unarchiveed data into the destination file descriptor
    // for the next stage in the pipeline
    const char* buffer;
    size_t size;
    int64_t offset;
    while (archive_read_data_block(libarchive_srcml.get(), (const void**) &buffer, &size, &offset) == ARCHIVE_OK) {

        if (write(*destination.fd, buffer, (unsigned int) size) == -1) {
            SRCMLstatus(ERROR_MSG, "Unable to write to internal pipeline");
            exit(1);
        }
   }

    // important to close, since this is how the file descriptor reader get an EOF
    close(*destination.fd);
}
