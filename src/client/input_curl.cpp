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

#include <input_curl.hpp>
#include <decompress_srcml.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/thread.hpp>
#pragma GCC diagnostic pop

#include <curl/curl.h>

#if defined(_MSC_BUILD) || defined(__MINGW32__)
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#endif

static int outfd = 0;

// Note: Stupid name, but can be changed later
void curl_it_all(const srcml_request_t& /* srcml_request */,
    const srcml_input_t& input_sources,
    const srcml_output_dest& destination) {

    // input comes from URL (I think)
    std::string url = input_sources[0].filename;

    // output is a file descriptor
    outfd = *destination.fd;

    // libcurl needs a FILE* to write to
    FILE* outfile = fdopen(outfd, "w");

    // setup curl to use url with a write function write_data_because_libcurl_is_stupid()
    CURL *curl_handle;
    CURLcode response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);

    // output will be to FILE* outfile (wrapper for file descriptor outfd) instead of STDOUT
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, outfile);

    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /*
        I really don't like pinging first.

        However, if the resource is not available, the error pages is written into the pipe,
        and libarchive gets it instead of source.

        Better solution: synchronize threads so that libarchive does not read from the input file descriptor
        until we have read some data and know it is good.
    */
        // Quick check to see if the remote location exists or is available
    CURL* ping = curl_easy_duphandle(curl_handle);
    curl_easy_setopt(ping, CURLOPT_NOBODY, 1L);
        //curl_easy_setopt(ping, CURLOPT_HEADER, 1L);
    curl_easy_perform(ping);

    long http_code = 0;
    //double data_size = 0;
    curl_easy_getinfo (ping, CURLINFO_RESPONSE_CODE, &http_code);
    //curl_easy_getinfo (ping, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &data_size);

    curl_easy_cleanup(ping);
    if (http_code != 200)
    {
        // bad, can't open the remote resource.
        // what to do here...
        std::cerr << "Resource: " << url << " unavailable - " << http_code << "\n";
        goto end;
    }
    // The resource is there, so lets go get it!
    response = curl_easy_perform(curl_handle);

    /* check for errors */
    if(response != CURLE_OK) {
        // this is also bad...
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(response) << std::endl;
    }

end:
    fclose(outfile);

    // make sure to close out libcurl read here
    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();
}

void input_curl(srcml_input_src& input) {

    	// setup the pipes
	    int fds[2] = { -1, -1 };
#if !defined(_MSC_BUILD) && !defined(__MINGW32__)
        pipe(fds);
#else
        HANDLE read_pipe;
        HANDLE write_pipe;
        CreatePipe(&read_pipe,&write_pipe, NULL, 0);

        fds[1] = _open_osfhandle((intptr_t)write_pipe, 0);
        fds[0] = _open_osfhandle((intptr_t)read_pipe, _O_RDONLY);
#endif

   	    // create a single thread to prefix decompression
        boost::thread input_thread(
            boost::bind(
                curl_it_all,
                srcml_request_t(),
                srcml_input_t(1, input),
                srcml_output_dest("-", fds[1])
            )
        );

        // the thread will write to fds[1], and the following input can read
        // from fds[0]
        input.fd = fds[0];
}
