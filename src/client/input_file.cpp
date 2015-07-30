/**
 * @file input_file.hpp
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

#include <input_file.hpp>
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

void input_file(srcml_input_src& input) {

    if (!input.compressions.empty() &&
        (input.compressions.size() > 1 || input.compressions.front() != ".gz")) {

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
                decompress_srcml,
                srcml_request_t(),
                srcml_input_t(1, input),
                srcml_output_dest("-", fds[1])
            )
        );

        // the thread will write to fds[1], and the following input can read
        // from fds[0]
        input.fd = fds[0];
    }
}
