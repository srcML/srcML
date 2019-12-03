/**
 * @file unarchive_srcml.cpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
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

#include <unarchive_srcml.hpp>
#include <curl/curl.h>
#include <archive.h>
#include <input_curl.hpp>
#include <SRCMLStatus.hpp>
#include <cstring>
#include <libarchive_utilities.hpp>
#include <memory>

void unarchive_srcml(const srcml_request_t& /* srcml_request */,
    const srcml_input_t& input_sources,
    const srcml_output_dest& destination) {

    std::unique_ptr<archive> libarchive_srcml(archive_read_new());

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

    if (contains<int>(input_sources[0])) {

        status = archive_read_open_fd(libarchive_srcml.get(), input_sources[0], buffer_size);
    } else {
        status = archive_read_open_filename(libarchive_srcml.get(), input_sources[0].resource.c_str(), buffer_size);
    }
    if (status != ARCHIVE_OK) {
        SRCMLstatus(ERROR_MSG, std::to_string(status));
        exit(1);
    }
    archive_entry *entry;
    status = archive_read_next_header(libarchive_srcml.get(), &entry);

    // copy from the libarchive unarchiveed data into the destination file descriptor
    // for the next stage in the pipeline
    archive_read_data_into_fd(libarchive_srcml.get(), *destination.fd);

    // important to close, since this is how the file descriptor reader get an EOF
    close(*destination.fd);
}
