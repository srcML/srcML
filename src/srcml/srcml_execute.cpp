/**
 * @file srcml_execute.cpp
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

#include <srcml_execute.hpp>
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/thread.hpp>
#pragma GCC diagnostic warning "-Wshorten-64-to-32"

void srcml_execute(const srcml_request_t& srcml_request,
                   std::list<command>& commands,
                   const srcml_input_t& input_sources,
                   const srcml_output_dest& destination) {

    // execute all but the last command in the sequence
    boost::thread_group command_processing_threads;

    int prevpipe = 0;
    while (commands.size() > 1) {

        // create a pipe for output
        int fds[2];
        pipe(fds);

        // run the front command in the sequence with possible input from previous pipe, and output to a new pipe
        command_processing_threads.create_thread( boost::bind(commands.front(),
            srcml_request,
            prevpipe ? input_sources : srcml_input_t(1, srcml_input_src("stdin://-", prevpipe)),
            srcml_output_dest("-", fds[1])));

        // will become input on next command
        prevpipe = fds[0];

        commands.pop_front();
    }

    // execute the last command in the sequence
    commands.front()(srcml_request,
                     !prevpipe ? input_sources : srcml_input_t(1, srcml_input_src("stdin://-", prevpipe)),
                     destination);

    // for normal processing, should not be needed. Basically for safety with error handling
    command_processing_threads.join_all();
}



