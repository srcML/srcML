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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/thread.hpp>
#pragma GCC diagnostic pop
#include <boost/foreach.hpp>
#include <boost/range.hpp>

void srcml_execute(const srcml_request_t& srcml_request,
                   std::list<command>& commands,
                   const srcml_input_t& input_sources,
                   const srcml_output_dest& destination) {

    boost::thread_group command_processing_threads;

    // create a thread for each command, creating pipes between adjoining commands
    int fds[2];
    BOOST_FOREACH(command curcommand, commands) {

        // special handling for first and last command_processing_threads
        bool first = curcommand == commands.front();
        bool last  = curcommand == commands.back();

        // pipe between each command
        int prevoutfd = fds[0];
        if (!first)
            pipe(fds);

        /* run this command in the sequence */
        command_processing_threads.create_thread(
            boost::bind(
                curcommand,
                srcml_request,
                /* first command uses input_source, rest input from previous output pipe */
                first ? input_sources : srcml_input_t(1, srcml_input_src("stdin://-", prevoutfd)),
                /* last command uses destination, rest output to pipe */                
                last  ? destination   : srcml_output_dest("-", fds[1])
            )
        );

        command_processing_threads.join_all();
    }
}
