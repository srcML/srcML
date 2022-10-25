// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file decompress_srcml.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <decompress_srcml.hpp>
#include <curl/curl.h>
#include <archive.h>
#include <input_curl.hpp>
#include <SRCMLStatus.hpp>
#include <libarchive_utilities.hpp>
#include <string_view>

using namespace ::std::literals::string_view_literals;

namespace {

    bool curl_supported_file(std::string_view input_protocol) {
        const char* const* curl_types = curl_version_info(CURLVERSION_NOW)->protocols;
        for (int i = 0; curl_types[i] != nullptr; ++i) {
            std::string_view curl(curl_types[i]);
            if (curl != "file"sv && curl == input_protocol)
                return true;
        }
        return false;
    }
}

void decompress_srcml(const srcml_request_t& /* srcml_request */,
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

    } else if (curl_supported_file(input_sources[0].protocol)) {

        // input must go through libcurl pipe
        srcml_input_src uninput = input_sources[0];
        if (!input_curl(uninput))
            exit(1);

#if WIN32
        // In Windows, the archive_read_open_fd() does not seem to work. The input is read as an empty archive,
        // or cut short. 
        // So for Windwos, convert to a FILE*. Note sure when to close the FILE*
        FILE* f = fdopen(*(uninput.fd), "r");
        status = archive_read_open_FILE(libarchive_srcml.get(), f);
#else
        status = archive_read_open_fd(libarchive_srcml.get(), *(uninput.fd), buffer_size);
#endif

    } else {

        status = archive_read_open_filename(libarchive_srcml.get(), input_sources[0].resource.data(), buffer_size);
    }
    if (status != ARCHIVE_OK) {
        SRCMLstatus(ERROR_MSG, std::to_string(status));
        exit(1);
    }

    archive_entry *entry;
    status = archive_read_next_header(libarchive_srcml.get(), &entry);

    // copy from the libarchive decompressed data into the destination file descriptor
    // for the next stage in the pipeline
    archive_read_data_into_fd(libarchive_srcml.get(), *destination.fd);

    // important to close, since this is how the file descriptor reader get an EOF
    close(*destination.fd);
}
