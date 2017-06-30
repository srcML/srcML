/**
 * @file unarchive_srcml.cpp
 *
 * @copyright Copyright (C) 2017 srcML, LLC. (www.srcML.org)
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

#include <unarchive_srcml.hpp>
#include <curl/curl.h>
#include <archive.h>
#include <input_curl.hpp>
#include <srcml_logger.hpp>

namespace {

    bool curl_supported(const std::string& input_protocol) {
        const char* const* curl_types = curl_version_info(CURLVERSION_NOW)->protocols;
        for (int i = 0; curl_types[i] != NULL; ++i) {
            if (strcmp(curl_types[i], input_protocol.c_str()) == 0)
                return true;
        }
        return false;
    }
}

void unarchive_srcml(const srcml_request_t& /* srcml_request */,
    const srcml_input_t& input_sources,
    const srcml_output_dest& destination) {

    archive* libarchive_srcml = archive_read_new();

    archive_read_support_format_ar(libarchive_srcml);
    archive_read_support_format_cpio(libarchive_srcml);
    archive_read_support_format_gnutar(libarchive_srcml);
    archive_read_support_format_iso9660(libarchive_srcml);
    archive_read_support_format_tar(libarchive_srcml);
    archive_read_support_format_xar(libarchive_srcml);
    archive_read_support_format_zip(libarchive_srcml);
    archive_read_support_format_raw(libarchive_srcml);
    archive_read_support_format_empty(libarchive_srcml);

    // just a bunch of bytes
    archive_read_support_format_raw(libarchive_srcml);

    /* Check libarchive version enable version specific features/syntax */
#if ARCHIVE_VERSION_NUMBER < 3000000
    // V2 Only Settings
    // Compressions
    archive_read_support_compression_all(libarchive_srcml);

#else
    // V3 Only Settings
    // File Formats
    archive_read_support_format_7zip(libarchive_srcml);
    archive_read_support_format_cab(libarchive_srcml);
    archive_read_support_format_lha(libarchive_srcml);
    archive_read_support_format_rar(libarchive_srcml);

    // Compressions
    archive_read_support_filter_all(libarchive_srcml);
#endif    // setup unarchiveions

    int status = ARCHIVE_OK;
    const int buffer_size = 16384;

    if (contains<int>(input_sources[0])) {
        status = archive_read_open_fd(libarchive_srcml, input_sources[0], buffer_size);
    } else if (curl_supported(input_sources[0].protocol)) {

        // input must go through libcurl pipe
        srcml_input_src uninput = input_sources[0];
        input_curl(uninput);
        status = archive_read_open_fd(libarchive_srcml, uninput, buffer_size);

    } else {
        status = archive_read_open_filename(libarchive_srcml, input_sources[0].resource.c_str(), buffer_size);
    }
    if (status != ARCHIVE_OK) {
        SRCMLLogger::log(SRCMLLogger::CRITICAL_MSG, std::to_string(status));
        exit(1);
    }
    archive_entry *entry;
    status = archive_read_next_header(libarchive_srcml, &entry);

    // copy from the libarchive unarchiveed data into the destination file descriptor
    // for the next stage in the pipeline
    archive_read_data_into_fd(libarchive_srcml, *destination.fd);

    // important to close, since this is how the file descriptor reader get an EOF
    close(*destination.fd);

    archive_read_close(libarchive_srcml);
#if ARCHIVE_VERSION_NUMBER >= 3000000
    archive_read_free(libarchive_srcml);
#else
    archive_read_finish(libarchive_srcml);
#endif
}
