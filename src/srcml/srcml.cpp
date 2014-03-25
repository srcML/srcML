/**
 * @file srcml.cpp
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

#include <srcml.h>
#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>
#include <srcml_display_metadata.hpp>
#include <srcml_list_unit_files.hpp>
#include <src_input_validator.hpp>
#include <src_language.hpp>
#include <srcml_options.hpp>
#include <create_srcml.hpp>
#include <process_srcml.hpp>
#include <create_src.hpp>
#include <isxml.hpp>
#include <peek4char.hpp>
#include <src_prefix.hpp>
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/thread.hpp>
#pragma GCC diagnostic warning "-Wshorten-64-to-32"

#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <errno.h>

#ifdef WIN32
#include <io.h>
#define  STDIN_FILENO   0       /* standard input file descriptor */
#define STDOUT_FILENO   1       /* standard output file descriptor */
#define STDERR_FILENO   2       /* standard error file descriptor */
#endif

// commands that are simple queries on srcml
const int SRCML_COMMAND_INSRCML =
    SRCML_COMMAND_LONGINFO |
    SRCML_COMMAND_INFO    |
    SRCML_COMMAND_INFO_FILENAME |
    SRCML_COMMAND_VERSION |
    SRCML_COMMAND_LIST |
    SRCML_COMMAND_UNITS |
    SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE |
    SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY |
    SRCML_COMMAND_DISPLAY_SRCML_FILENAME |
    SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION |
    SRCML_COMMAND_DISPLAY_SRCML_ENCODING;

int main(int argc, char * argv[]) {

    // parse the command line
    srcml_request_t srcml_request = parseCLI(argc, argv);

    // global options
    SRCMLOptions::set(srcml_request.command);

    // version
    if (srcml_request.command & SRCML_COMMAND_VERSION) {
        std::cout <<  srcml_version_string() << "\n";
        std::cout << "srcml Version Number " << srcml_version_number() << "\n";
        std::cout << "libarchive Version " << ARCHIVE_VERSION_NUMBER << "\n";
        return 0;
    }

    // convert the list of input filenames to actual input sources
    srcml_input_t input_sources(srcml_request.input.begin(), srcml_request.input.end());

    // input source that is stdin, if it exists
    srcml_input_src* pstdin = srcml_request.stdindex ? &input_sources[*srcml_request.stdindex] : 0;

    // standard input handled as FILE* to be able to peek
    if (pstdin) {

        // FILE* becomes part of stdin input source
        pstdin->fileptr = fdopen(STDIN_FILENO, "r");

        // peek at the first 4 bytes
        unsigned char data[4];
        ssize_t size = 0;
        peek4char(*(pstdin->fileptr), data, &size);

        // from the first up-to 4 bytes determine if is srcML or not
        pstdin->state = isxml(data, size) ? SRCML : SRC;
    }

    // output destination setup just like an input source
    // TODO: Make output_filename not boost::optional
    srcml_output_dest destination(*srcml_request.output_filename);

    // Determine what processing needs to occur based on the inputs, outputs, and commands

    // srcml->src, based on the destination
    bool createsrc = destination.state == SRC;

    // metadata(srcml)
    bool insrcml = srcml_request.command & SRCML_COMMAND_INSRCML;

    // A single src input file implies src->srcml
    // Note: src->srcml->src implies a temporary srcml file
    bool src_input = false;
    BOOST_FOREACH(const srcml_input_src& input, input_sources) {
        if (input.state == SRC) {
            src_input = true;
            break;
        }
    }

    // create srcml when there is a src input, or the command is not to create src
    bool createsrcml = src_input ? true : !createsrc;

    // adjust if explicitly told differently via command line options
    // TODO: may warn/error on some inconsistencies here
    if (!createsrc && srcml_request.command & SRCML_COMMAND_SRC) {
        createsrc = true;
    } else if (!createsrcml && srcml_request.command & SRCML_COMMAND_SRCML) {
        createsrcml = true;
    } else if (!createsrcml && !srcml_request.files_from.empty()) {
        createsrcml = true;
    }

    // language is required when creating srcml and standard input is used for source
    if (createsrcml && pstdin && (pstdin->state == SRC) && !srcml_request.att_language) {
            std::cerr << "Using stdin requires a declared language\n";
            exit(1);
    }

    // src->srcml (or src->srcml->src)
    boost::thread_group create_srcml_thread;
    srcml_input_t pipe_input_sources;
    if (createsrcml && !createsrc) {

        create_srcml(input_sources, srcml_request, destination);

    } if (createsrcml && createsrc) {

        // setup a pipe for src->srcml can write to fds[1], and srcml->src can read from fds[0]
        int fds[2];
        pipe(fds);

        // set the output destination
        destination = fds[1];

        // start src->srcml writing to the pipe
        // No need to join or wait as it will write to destination pipe
        create_srcml_thread.create_thread( boost::bind(create_srcml, input_sources, srcml_request, destination) );

        // No need to join or wait as the thread will write to destination pipe, then return

        // the srcml->src stage must now read from internal input sources
        pipe_input_sources.push_back(srcml_input_src("stdin://-", fds[0]));
    }

    if (insrcml) {
        srcml_display_metadata(srcml_request);
    }

    // srcml->src
    if (createsrc) {

        // creating src from external input sources, or from internal pipe
        create_src(pipe_input_sources.empty()? input_sources : pipe_input_sources, srcml_request, destination);
    }

    srcml_cleanup_globals();

    return 0;
}
