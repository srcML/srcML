/**
 * @file decompress_srcml.cpp
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

#include <decompress_srcml.hpp>
#include <curl/curl.h>
#include <archive.h>

namespace {
    struct curl {
        CURL* handle;
        CURLM* multi_handle;
        CURLMsg* msg;
        int msgs_left;
        int still_running;
        size_t data_len;
        char* data_buffer;
        std::string source;
    };

    size_t curl_cb(void* buffer, size_t len, size_t nmemb, void* data);

    int     archive_curl_open(archive *, void *client_data);
    ssize_t archive_curl_read(archive *, void *client_data, const void **buff);
    int     archive_curl_close(archive *, void *client_data);

    bool curl_supported(const std::string& input_protocol) {
        const char* const* curl_types = curl_version_info(CURLVERSION_NOW)->protocols;
        for (int i = 0; curl_types[i] != NULL; ++i) {
            if (strcmp(curl_types[i], input_protocol.c_str()) == 0)
                return true;
        }
        return false;
    }
}

void decompress_srcml(const srcml_request_t& /* srcml_request */,
    const srcml_input_t& input_sources,
    const srcml_output_dest& destination) {

    archive* libarchive_srcml = archive_read_new();

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
#endif    // setup decompressions

    int status = ARCHIVE_OK;
    curl curling;

    if (contains<int>(input_sources[0])) {
        status = archive_read_open_fd(libarchive_srcml, input_sources[0], 16384);
    } else if (curl_supported(input_sources[0].protocol)) {

        curling.source = input_sources[0].filename;
        status = archive_read_open(libarchive_srcml, &curling, archive_curl_open, archive_curl_read, archive_curl_close);

    } else {
        status = archive_read_open_filename(libarchive_srcml, input_sources[0].resource.c_str(), 16384);
    }
    if (status != ARCHIVE_OK) {
        std::cerr << status;
        exit(1);
    }
    archive_entry *entry;
    status = archive_read_next_header(libarchive_srcml, &entry);

    // copy from the libarchive decompressed data into the destination file descriptor
    // for the next stage in the pipeline
    archive_read_data_into_fd(libarchive_srcml, *destination.fd);

    // important to close, since this is how the file descriptor reader get an EOF
    close(*destination.fd);

    archive_read_close(libarchive_srcml);
    archive_read_finish(libarchive_srcml);
}

namespace {

    size_t curl_cb(void* buffer, size_t len, size_t nmemb, void* data) {

        curl* curldata = (curl*) data;

        curldata->data_buffer = (char*)buffer;
        curldata->data_len = len * nmemb;

        return curldata->data_len;
    }

    int archive_curl_open(archive*, void* client_data) {

        curl* curldata = (curl*) client_data;

        curl_global_init(CURL_GLOBAL_DEFAULT);

        curldata->handle = curl_easy_init();

        curl_easy_setopt(curldata->handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curldata->handle, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curldata->handle, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
        curl_easy_setopt(curldata->handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curldata->handle, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curldata->handle, CURLOPT_WRITEFUNCTION, curl_cb);
        curl_easy_setopt(curldata->handle, CURLOPT_WRITEDATA, curldata);
        curl_easy_setopt(curldata->handle, CURLOPT_URL, curldata->source.c_str());

        curldata->multi_handle = curl_multi_init();
        curl_multi_add_handle(curldata->multi_handle, curldata->handle);
        curl_multi_perform(curldata->multi_handle, &curldata->still_running);

        // TODO: SOMETHING HERE TO MAKE SURE THE FILE IS ACTUALLY PRESENT
        return ARCHIVE_OK;
    }

    ssize_t archive_curl_read(archive*, void* client_data, const void** buff) {

        curl* curldata = (curl*) client_data;

        curldata->data_len = 0;
        while (curldata->data_len == 0 && curldata->still_running) {
            curl_multi_perform(curldata->multi_handle, &curldata->still_running);
        }

        *buff = curldata->data_buffer;

        return curldata->data_len;
    }

    int archive_curl_close(archive*, void* client_data) {

        curl* curldata = (curl*) client_data;
        curl_multi_cleanup(curldata->multi_handle);
        curl_easy_cleanup(curldata->handle);

        return 0;
    }
};
