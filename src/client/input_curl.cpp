/**
 * @file input_curl.hpp
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

#include <pipe.hpp>
#include <input_curl.hpp>
#include <curl/curl.h>
#include <SRCMLStatus.hpp>
#include <mutex>
#include <condition_variable>

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
        int outfd;
        size_t currentsize;
        std::string buffer;
    };

    const size_t CURL_MAX_ERROR_SIZE = 100;

    std::mutex c;

    bool curl_errors = false;
}


/*
    Write callback for curl. libcurl internals use fwrite() as default, so replacing it
    with our own callback does not entail an additional copy
*/
size_t our_curl_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {

    curl_write_info* data = (curl_write_info*) userdata;

    goCurl(true);

    return write(data->outfd, ptr, size * nmemb);
}

// downloads URL into file descriptor
void curl_download_url(const srcml_request_t& /* srcml_request */,
    const srcml_input_t& input_sources,
    const srcml_output_dest& destination) {

    // input comes from URL
    std::string url = input_sources[0].filename;

    curl_write_info write_info;
    write_info.outfd = *destination.fd; // output is a file descriptor
    write_info.currentsize = 0;

    CURL *curl_handle;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_handle = curl_easy_init();

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
    //curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);

    // start the download
    CURLcode response;
    response = curl_easy_perform(curl_handle);

    // check for download errors
    long http_code = 0;
    curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
    if(response != CURLE_OK || http_code != 200) {
        SRCMLstatus(WARNING_MSG, "srcml: Unable to access URL " + url);
        setCurlErrors();
        goCurl(false);

    } else {

        goCurl(true);
        
        // ok, no errors, but may have cached data in the buffer, especially for small files
        if (!write_info.buffer.empty()) {
            write(write_info.outfd, write_info.buffer.c_str(), write_info.buffer.size());
        }
    }

    // close the output file descriptor we were writing the download to
    close(write_info.outfd);

    // cleanup out download
    curl_easy_cleanup(curl_handle);

    // all done with libcurl
    curl_global_cleanup();
}

int input_curl(srcml_input_src& input) {

    input_pipe(input, curl_download_url);

    // wait to see if curl is able to download the url at all
    return waitCurl();
}

void setCurlErrors() {
    std::unique_lock<std::mutex> l(c);
    curl_errors = true;
}

void clearCurlErrors() {
    std::unique_lock<std::mutex> l(c);
    curl_errors = false;
}

bool getCurlErrors() {
    std::unique_lock<std::mutex> l(c);
    return curl_errors;
}
