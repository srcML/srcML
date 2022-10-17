// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_execute.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <srcml_execute.hpp>

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#endif

#include <thread>
#include <list>
#include <srcml_pipe.hpp>

void srcml_execute(const srcml_request_t& srcml_request,
                   processing_steps_t& pipeline,
                   const srcml_input_t& input_sources,
                   const srcml_output_dest& destination) {

    // create a thread for each step, creating pipes between adjoining steps
    std::list<std::thread> pipethreads;
    int fds[2] = { -1, -1 };
    for (const auto& command : pipeline) {

        // special handling for first and last steps
        bool first = command == pipeline.front();
        bool last  = command == pipeline.back();

        // pipe between each step
        int prevoutfd = fds[0];
        fds[0] = fds[1] = -1;
        if (pipeline.size() > 1 && !last) {
#if !defined(_MSC_VER) && !defined(__MINGW32__)
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
        }

        /* run this step in the sequence */
        pipethreads.push_back(std::thread(
            command,
            srcml_request,
            /* first process_srcml uses input_source, rest input from previous output pipe */
            first ? input_sources : srcml_input_t(1, srcml_input_src("stdin://-", prevoutfd)),
            /* last process_srcml uses destination, rest output to pipe */
            last  ? destination   : srcml_output_dest("-", fds[1])
        ));
    }

    // wait for all the input pipes to finish
    srcml_pipe_clean();

    // wait on all threads
    std::for_each(pipethreads.begin(), pipethreads.end(), [](std::thread& t) { t.join(); });
}
