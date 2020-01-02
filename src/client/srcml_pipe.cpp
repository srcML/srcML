/**
 * @file pipe.cpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
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

#include <thread>

#include <SRCMLStatus.hpp>

#if defined(_MSC_BUILD) || defined(__MINGW32__)
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#endif

static std::list<std::thread*> lthreads;

void srcml_pipe(srcml_input_src& input, srcml_pipe_process process, srcml_request_t srcml_request) {

    // setup the pipes
    int fds[2] = { -1, -1 };
#if !defined(_MSC_BUILD) && !defined(__MINGW32__)
    if (pipe(fds) == -1) {
        perror("srcml");
        return;
    }
#else
    HANDLE read_pipe;
    HANDLE write_pipe;
    CreatePipe(&read_pipe,&write_pipe, nullptr, 0);

    fds[1] = _open_osfhandle((intptr_t)write_pipe, 0);
    fds[0] = _open_osfhandle((intptr_t)read_pipe, _O_RDONLY);
#endif

    // create a single thread to prefix decompression
    lthreads.push_back(new std::thread(
            process,
            srcml_request,
            srcml_input_t(1, input),
            srcml_output_dest("-", fds[1])
    ));

    // the thread will write to fds[1], and the following input can read
    // from fds[0]
    input.fd = fds[0];
}

void srcml_pipe_clean() {

    for(auto thread : lthreads) {
        thread->join();
        delete thread;
    }
}

