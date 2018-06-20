/**
 * @file input_stdin.hpp
 *
 * @copyright Copyright (C) 2018 srcML, LLC. (www.srcML.org)
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

#include <input_stdin.hpp>
#include <isxml.hpp>
#include <pipe.hpp>
#include <sys/types.h>
#include <sys/uio.h>

/*
    Does stdin contain xml or source
*/
void input_stdin(srcml_request_t& request) {

    // stdin input source
    auto& rstdin = request.input_sources[*request.stdindex];

    // determine if input is srcML or not
    request.bufsize = read(0, request.buf, 4);
    rstdin.state = isxml((unsigned char*) request.buf, (int) request.bufsize) ? SRCML : SRC;

    // copy rest of stdin into pipe
    srcml_pipe(rstdin, [](const srcml_request_t& srcml_request, const srcml_input_t& input_sources, const srcml_output_dest& destination) {

        // write the prerequest
        write(*destination.fd, srcml_request.buf, srcml_request.bufsize);

        // copy the rest of the input source
        char buf[512];
        ssize_t size = 0;
        do {
            size = read(*input_sources[0].fd, buf, 512);
            if (size > 0)
                write(*destination.fd, buf, size);
        } while (size > 0);

        close(*destination.fd);
    }, request);
}
