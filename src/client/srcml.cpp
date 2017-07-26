/**
 * @file srcml.cpp
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

#include <srcml.h>
#include <srcml_cli.hpp>
#include <srcml_options.hpp>
#include <create_srcml.hpp>
#include <compress_srcml.hpp>
#include <create_src.hpp>
#include <transform_srcml.hpp>
#include <srcml_display_metadata.hpp>
#include <srcml_execute.hpp>
#include <isxml.hpp>
#include <timer.hpp>
#include <srcml_logger.hpp>
#include <curl/curl.h>
#include <boost/version.hpp>
#include <archive.h>
#include <iostream>
#include <unistd.h>
#include <csignal>

// logic for each stage to decide if needed
namespace {
    bool request_create_srcml          (const srcml_request_t&, const srcml_input_t&, const srcml_output_dest&);
    bool request_transform_srcml       (const srcml_request_t&, const srcml_input_t&, const srcml_output_dest&);
    bool request_display_metadata      (const srcml_request_t&, const srcml_input_t&, const srcml_output_dest&);
    bool request_additional_compression(const srcml_request_t&, const srcml_input_t&, const srcml_output_dest&);
    bool request_create_src            (const srcml_request_t&, const srcml_input_t&, const srcml_output_dest&);
};

// global request
srcml_request_t global_srcml_request;

// stdin timeout message
void timeout(int) {

    fprintf(stderr, R"(srcml typically accepts input from standard input from a pipe, not a terminal.
Typical usage includes:

    # convert from a source file to srcML
    srcml main.cpp -o main.cpp.xml

    # convert from text to srcML
    srcml --text="int i = 1;" --language C++

    # pipe in source code
    echo "int i = 1;" | srcml --language C++

    # convert from srcML back to source code 
    srcml main.cpp.xml -o main.cpp

Consider using the --text option for direct entry of text.

See `srcml --help` for more information.
)");
    exit(1);
}

int main(int argc, char * argv[]) {
    Timer runtime = Timer();
    runtime.start();

    // parse the command line
    auto srcml_request = parseCLI(argc, argv);
    global_srcml_request = srcml_request;

    // global access to options
    SRCMLOptions::set(srcml_request.command);

    // version
    if (srcml_request.command & SRCML_COMMAND_VERSION) {
        std::cout << "libsrcml " << srcml_version_string() << "\n";
        std::cout << "srcml " << srcml_version_string() << "\n";
        std::cout << archive_version_string() << "\n";
        return 0;
    }

    if (srcml_request.command & SRCML_DEBUG_MODE)
        SRCMLlog(DEBUG_MSG,
                         std::string("Library Versions:\n") +
                         "libsrcml " + srcml_version_string() + "\n" +
                         "srcml " + srcml_version_string() + "\n" +
                         std::string(archive_version_string()) + "\n" +
                         "libcurl " + std::string(curl_version_info(CURLVERSION_NOW)->version) + "\n" +
                         "libboost " + BOOST_LIB_VERSION + "\n");

    if (srcml_request.input_sources.size() == 1 && srcml_request.unit != 0)
        srcml_request.input_sources[0].unit = srcml_request.unit;

    // standard input handled as FILE* to determine if srcML or src
    if (srcml_request.stdindex) {

        auto* pstdin = &srcml_request.input_sources[*srcml_request.stdindex];

        // stdin accessed as FILE*
        pstdin->fileptr = fdopen(STDIN_FILENO, "r");
        if (!pstdin->fileptr) {
            SRCMLlog(CRITICAL_MSG, "srcml: Unable to open stdin");
            exit(1);
        }
        pstdin->fd = boost::none;

        // setup a 5 second timeout for stdin from the terminal
        if (isatty(0)) {
            alarm(5);
            signal(SIGALRM, timeout);
        }

        // determine if the input is srcML or src
        pstdin->state = isxml(*(pstdin->fileptr)) ? SRCML : SRC;
    }

    // steps in the internal pipeline
    processing_steps_t pipeline;

    // step src->srcml
    if (request_create_srcml(srcml_request, srcml_request.input_sources, srcml_request.output_filename)) {

        pipeline.push_back(create_srcml);
    }

    // step srcml->srcml
    if (request_transform_srcml(srcml_request, srcml_request.input_sources, srcml_request.output_filename)) {

        pipeline.push_back(transform_srcml);
    }

    // step srcml->metadata
    if (request_display_metadata(srcml_request, srcml_request.input_sources, srcml_request.output_filename)) {

        pipeline.push_back(srcml_display_metadata);
    }

    // step srcml->src
    if (request_create_src(srcml_request, srcml_request.input_sources, srcml_request.output_filename)) {

        pipeline.push_back(create_src);
    }

    // step (srcml|src)->compressed
    if (request_additional_compression(srcml_request, srcml_request.input_sources, srcml_request.output_filename)) {

#if ARCHIVE_VERSION_NUMBER > 3001002
        pipeline.push_back(compress_srcml);
#else
        SRCMLlog(CRITICAL_MSG, "srcml: Unsupported output compression");
        exit(1);
#endif
    }

    // should always have something to do
    if (pipeline.empty()) {
        SRCMLlog(CRITICAL_MSG, "srcml: Internal error, cannot decide what processing needed");
        exit(1);
    }

    // execute the pipeline
    srcml_execute(srcml_request, pipeline, srcml_request.input_sources, srcml_request.output_filename);

    srcml_cleanup_globals();

    SRCMLlog(DEBUG_MSG, "CPU Time: " + std::to_string(runtime.cpu_time_elapsed()) + "ms");
    SRCMLlog(DEBUG_MSG, "Real Time: " + std::to_string(runtime.real_world_elapsed()) + "ms");

    // error status is 0 unless a critical, error, or warning
    return SRCMLLogger::errors() ? 1 : 0;
}

namespace {

    bool request_create_srcml(const srcml_request_t& srcml_request,
      const srcml_input_t& input_sources,
      const srcml_output_dest& destination) {

        return std::find_if(input_sources.begin(), input_sources.end(), is_src) != input_sources.end() ||
        (input_sources.size() > 1 && destination.state == SRCML) ||
        (input_sources.size() == 1 && input_sources[0].unit >= 0 && (srcml_request.command & SRCML_COMMAND_XML));
    }

    bool request_transform_srcml(const srcml_request_t& srcml_request,
                             const srcml_input_t& /* input_sources */,
                             const srcml_output_dest& /* destination */) {

        return !srcml_request.transformations.empty();
    }

    bool request_display_metadata(const srcml_request_t& srcml_request,
                              const srcml_input_t& /* input_sources */,
                              const srcml_output_dest& /* destination */) {

        return (srcml_request.command & SRCML_COMMAND_INSRCML || srcml_request.xmlns_prefix_query || srcml_request.pretty_format);
    }

    bool request_additional_compression(const srcml_request_t& /* srcml_request */,
                                        const srcml_input_t& /* input_sources */,
        const srcml_output_dest& destination) {

        return (destination.compressions.size() >= 1) /* ||
            (destination.compressions.size() == 1 && destination.compressions.front() != ".gz")*/;
    }

    bool request_create_src(const srcml_request_t& srcml_request,
        const srcml_input_t& input_sources,
        const srcml_output_dest& destination) {

        return (srcml_request.command & SRCML_COMMAND_SRC) || (!request_create_srcml(srcml_request, input_sources, destination) &&
            destination.state != SRCML &&
            !request_display_metadata(srcml_request, input_sources, destination) &&
            !request_transform_srcml(srcml_request, input_sources, destination));
    }
};
