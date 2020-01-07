/**
 * @file input_curl.hpp
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

#include <srcml_pipe.hpp>
#include <input_curl.hpp>
#include <curl/curl.h>
#include <SRCMLStatus.hpp>
#include <mutex>
#include <condition_variable>
#include <atomic>

bool curl_supported(const std::string& input_protocol) {
    const char* const* curl_types = curl_version_info(CURLVERSION_NOW)->protocols;
    for (int i = 0; curl_types[i] != nullptr; ++i) {
        if (std::string(curl_types[i]) == input_protocol)
            return true;
    }
    return false;
}

namespace {

    bool go = false;
    std::condition_variable cv;
    std::mutex d;

    int waitCurl() {
        std::unique_lock<std::mutex> l(d);
        cv.wait(l);
        return go;
    }

    void goCurl(bool flag) {
        std::unique_lock<std::mutex> l(d);
        go = flag;
        cv.notify_one();
    }

    struct curl_write_info {
        int outfd = 0;
        CURL* curlhandle = nullptr;
    };

    std::atomic<bool> curl_errors(false);
}

// coordinate curl error response
// only call before goCurl()
void setCurlErrors(bool flag) {
    curl_errors = flag;
}

bool getCurlErrors() {
    return curl_errors;
}

/*
    Write callback for curl. libcurl internals use fwrite() as default, so replacing it
    with our own callback does not entail an additional copy
*/
size_t our_curl_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {

    curl_write_info* data = (curl_write_info*) userdata;

    // have to check for download errors before we write anything into the pipe
    // you may expect that if there were errors, you could check for that before data
    // is written. But that is not the case
    long http_code = 0;
    curl_easy_getinfo (data->curlhandle, CURLINFO_RESPONSE_CODE, &http_code);

    // return immediately with an error so that curl_easy_perform() returns
    // and curl_easy_getinfo() can get data to process the error
    if (http_code != 200)
        return 0;

    goCurl(true);

    ssize_t result = write(data->outfd, ptr, size * nmemb);

    return result == -1 ? 0 : (size_t) result;
}

// downloads URL into file descriptor
int input_curl(srcml_input_src& input) {

    srcml_pipe(input, [](const srcml_request_t& /* srcml_request */, const srcml_input_t& input_sources, const srcml_output_dest& destination) {

        // input comes from URL
        std::string url = input_sources[0].filename;

        curl_global_init(CURL_GLOBAL_DEFAULT);

        CURL* curl_handle{ 0 };
        curl_handle = curl_easy_init();

        curl_write_info write_info;
        write_info.outfd = *destination.fd; // output is a file descriptor
        write_info.curlhandle = curl_handle;

        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);

        // setup to use a write function
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &write_info);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, our_curl_write_callback);

        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME, 5L);
        curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 1L);

        // start the download
        CURLcode response = curl_easy_perform(curl_handle);

        // check for download errors
        if (response != CURLE_OK) {
            SRCMLstatus(WARNING_MSG, "srcml: Unable to access URL " + url);
            setCurlErrors(true);
            goCurl(false);

        } else {

            setCurlErrors(false);
            goCurl(true);
        }

        // close the output file descriptor we were writing the download to
        close(write_info.outfd);

        // cleanup out download
        curl_easy_cleanup(curl_handle);

        // all done with libcurl
        curl_global_cleanup();
    });

    // wait to see if curl is able to download the url at all
    return waitCurl();
}
