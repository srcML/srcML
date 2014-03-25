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
#include <srcml_display_info.hpp>
#include <srcml_list_unit_files.hpp>
#include <src_input_validator.hpp>
#include <src_language.hpp>
#include <srcml_options.hpp>
#include <create_srcml.hpp>
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

    // We would prefer to just deal with the inputs one-by-one, however we need
    // to know whether they are all srcml, or a mixture first, so lets do it here
    // our own input sources as we determine things about them
    srcml_input_t input_sources(srcml_request.input.begin(), srcml_request.input.end());

    // find the location of standard input
    srcml_input_src* pstdin = srcml_request.stdindex ? &input_sources[*srcml_request.stdindex] : 0;

    // standard input handled as FILE* to be able to peek
    if (pstdin) {

        // Note: If stdin only, then have to read from this FILE*, then make sure to use it below
        pstdin->fileptr = fdopen(STDIN_FILENO, "r");

        // read the first 4 bytes as separate characters to get around byte ordering
        unsigned char data[4];
        ssize_t size = 0;
        peek4char(*(pstdin->fileptr), data, &size);

        // pass the first 4 bytes and the size actually read in
        pstdin->state = isxml(data, size) ? SRCML : SRC;
    }

    // do the same sort of processing for the output destination
    srcml_output_dest destination = srcml_request.output_filename ? *srcml_request.output_filename : "stdout:///-";

    // Determine what processing needs to occur

    // src->srcml
    bool createsrcml = false;

    // srcml->src
    bool createsrc = false;

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
    if (src_input) {
        // at least one src file, so we have to create srcml
        // may also have to go back to src
        createsrcml = true;
        createsrc = destination.state == SRC;
    } else {
        // all inputs are srcml, so result depends on destination
        // if destination is srcml, no src creation
        // if destination is src (or indeterminate), create src
        createsrc = destination.state == SRC;
        createsrcml = !createsrc;
    }

    // adjust if explicitly told differently via commands
    // TODO: may warn/error on some inconsistencies here
    if (!createsrc && srcml_request.command & SRCML_COMMAND_SRC) {
        createsrc = true;
    } else if (!createsrcml && srcml_request.command & SRCML_COMMAND_SRCML) {
        createsrcml = true;
    } else if (!createsrcml && !srcml_request.files_from.empty()) {
        createsrcml = true;
    }

    // when creating srcml, and standard input is used for source, we the user must have specified a language
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
        pipe_input_sources.resize(1);
        pipe_input_sources[0] = "stdin://-"; // TOCHECK: What is the proper prefix for this protocol?
        pipe_input_sources[0] = fds[0];
    }

    if (insrcml) {
        // create the output srcml archive
        srcml_archive* srcml_arch = srcml_create_archive();
        // Assuming one srcml input
        if (srcml_read_open_filename(srcml_arch, (src_prefix_resource(srcml_request.input[0]).c_str())) != SRCML_STATUS_OK) {
            std::cerr << "srcML file " << src_prefix_resource(srcml_request.input[0]) << " could not be opened.\n";
            return 1; // Error on opening the the srcml
        }

        // srcml->src language
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE){
            const char* archive_info = srcml_archive_get_language(srcml_arch);
            if (archive_info)
                std::cout << "Language: " << archive_info << "\n";
        }
        // srcml->src directory
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY){
            const char* archive_info = srcml_archive_get_directory(srcml_arch);
            if (archive_info)
                std::cout << "Directory: " << archive_info << "\n";
        }
        // srcml->src filename
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_FILENAME){
            const char* archive_info = srcml_archive_get_filename(srcml_arch);
            if (archive_info)
                std::cout << "Filename: " << archive_info << "\n";
        }
        // srcml->src src version
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION){
            const char* archive_info = srcml_archive_get_version(srcml_arch);
            if (archive_info)
                std::cout << "Version: " << archive_info << "\n";
        }
        // srcml->src encoding
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_ENCODING){
            const char* archive_info = srcml_archive_get_src_encoding(srcml_arch);
            if (archive_info)
                std::cout << "Source Encoding: " << archive_info << "\n";
        }
        // srcml long info
        if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
            srcml_display_info(srcml_request.input);
        }
        // srcml info
        if (srcml_request.command & SRCML_COMMAND_INFO) {
            srcml_display_info(srcml_request.input);
        }
        // list filenames in srcml archive
        if (srcml_request.command & SRCML_COMMAND_LIST) {
            srcml_list_unit_files(srcml_request.input);

        }

        srcml_free_archive(srcml_arch);
    }

    // srcml->src
    if (createsrc) {

        // creating src from external input sources, or from internal pipe
        create_src(pipe_input_sources.empty()? input_sources : pipe_input_sources, srcml_request, destination);
    }

    srcml_cleanup_globals();

    return 0;
}
