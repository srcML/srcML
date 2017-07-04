/**
 * @file curl_input_file.hpp
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

#ifdef _MSC_BUILD
#define ssize_t __int64
#endif

#include <curl/curl.h>
#include <archive.h>
#include <timer.hpp>
#include <string>

 struct curl {
    CURL* handle;
    CURLM* multi_handle;
    CURLMsg* msg;
    int msgs_left;
    int still_running;
    size_t data_len;
    char* data_buffer;
    std::string source;
    Timer stopwatch;
};

size_t curl_cb(void* buffer, size_t len, size_t nmemb, void* data);

int     archive_curl_open(archive *, void *client_data);
__LA_SSIZE_T archive_curl_read(archive *, void *client_data, const void **buff);
int     archive_curl_close(archive *, void *client_data);

bool curl_supported(const std::string& input_protocol);
