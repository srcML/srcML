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

#if defined(_MSC_BUILD) || defined(__MINGW32__)
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#endif

static int outfd = 0;

size_t write_data_because_libcurl_is_stupid(void *buffer, size_t size, size_t nmemb, void * /* userp */) {


    write(outfd, buffer, nmemb);

    return size;
}

// Note: Stupid name, but can be changed later
void curl_it_all(const srcml_request_t& /* srcml_request */,
    const srcml_input_t& input_sources,
    const srcml_output_dest& destination) {

    // input comes from URL (I think)
    std::string url = input_sources[0].resource;

    // output is a file descriptor
    outfd = *destination.fd;

    // setup curl to use url with a write function write_data_because_libcurl_is_stupid()


    // just to see if it works
    const char buffer[] = "a;";
    write_data_because_libcurl_is_stupid((void*) buffer, sizeof(buffer), sizeof(char), 0);
    close(outfd);


    // make sure to close out libcurl read here

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
