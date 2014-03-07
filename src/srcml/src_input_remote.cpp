/**
 * @file src_input_remote.cpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
  src_input_remote assigns remote files and archives to the srcml parsing queue
*/

#include <src_input_remote.hpp>
#include <vector>
#include <curl/curl.h>
#include <archive.h>
#include <archive_entry.h>

#ifdef __MACH__
#include <sys/time.h>
#define CLOCK_REALTIME 0
//clock_gettime is not implemented on OSX
int clock_gettime(int /*clk_id*/, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
#endif

#define CURL_BUFFER_SIZE 16000

void src_input_remote(ParseQueue& queue, srcml_archive* srcml_arch, const std::string& remote_uri, const boost::optional<std::string>& language, const boost::optional<std::string>& option_filename, const boost::optional<std::string>& option_directory, boost::optional<FILE*> fstdin) {    
    return;
}