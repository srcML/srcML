/**
 * @file curl_input_file.cpp
 *
 * @copyright Copyright (C) 2015 srcML, LLC. (www.srcML.org)
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

#include <curl_input_file.hpp>
#include <archive.h>
#include <Timer.hpp>
#include <string>

bool curl_supported(const std::string& input_protocol) {
    const char* const* curl_types = curl_version_info(CURLVERSION_NOW)->protocols;
    for (int i = 0; curl_types[i] != NULL; ++i) {
        if (std::string(curl_types[i]) == input_protocol)
            return true;
    }
    return false;
}

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

    // Quick check to see if the remote location exists or is available
    CURL* ping = curl_easy_duphandle(curldata->handle);
    curl_easy_setopt(ping, CURLOPT_NOBODY, 1L);
    //curl_easy_setopt(ping, CURLOPT_HEADER, 1L);
    curl_easy_perform(ping);

    long http_code = 0;
    double data_size = 0;
    curl_easy_getinfo (ping, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_getinfo (ping, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &data_size);

    curl_easy_cleanup(ping);
    if (http_code != 200)
    {
        return -1;
    }

    // The resource is there, so lets go get it!
    curldata->multi_handle = curl_multi_init();
    curl_multi_add_handle(curldata->multi_handle, curldata->handle);
    curl_multi_perform(curldata->multi_handle, &curldata->still_running);
    curldata->stopwatch = Timer(10); // 10 second timeout

    return ARCHIVE_OK;
}

__LA_SSIZE_T archive_curl_read(archive*, void* client_data, const void** buff) {

    curl* curldata = (curl*) client_data;

    curldata->data_len = 0;
    curldata->stopwatch.start();
    while (curldata->data_len == 0 && curldata->still_running) {
        curl_multi_perform(curldata->multi_handle, &curldata->still_running);

           // Connection lost mid-transfer time to give up
        if (curldata->stopwatch.is_expired()) {
            curldata->data_len = 0;
            curldata->data_buffer = 0;
            break;
        }
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
