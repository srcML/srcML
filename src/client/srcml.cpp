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
#include <Timer.hpp>
#include <SRCMLStatus.hpp>
#include <curl/curl.h>
#include <boost/version.hpp>
#include <archive.h>
#include <iostream>
#include <csignal>
#include <cmath>
#include <TraceLog.hpp>

#ifndef _MSC_BUILD
#include <unistd.h>
#endif

// decide if a step is needed
namespace {
    bool request_create_srcml      (const srcml_request_t&);
    bool request_transform_srcml   (const srcml_request_t&);
    bool request_display_metadata  (const srcml_request_t&);
    bool request_output_compression(const srcml_request_t&);
    bool request_create_src        (const srcml_request_t&);
}

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

namespace {
    void set_state_stdin(srcml_request_t& srcml_request);
    void set_state_isxml(srcml_request_t& srcml_request);
}

int main(int argc, char * argv[]) {

    Timer runtime = Timer();

    // parse the command line
    auto srcml_request = parseCLI(argc, argv);

    // global access to options
    SRCMLOptions::set(srcml_request.command);

    // version
    if (option(SRCML_COMMAND_VERSION)) {
        std::cout << "libsrcml " << srcml_version_string() << '\n'
                  << "srcml " << srcml_version_string() << '\n'
                  << archive_version_string() << '\n';
        return 0;
    }

    // debug info
    if (srcml_request.command & SRCML_DEBUG_MODE) {
        SRCMLstatus(DEBUG_MSG) << "Library Versions: " << '\n'
                               << "libsrcml " << srcml_version_string() << '\n'
                               << "srcml " << srcml_version_string() << '\n'
                               <<  archive_version_string() << '\n'
                               << "libcurl " << curl_version_info(CURLVERSION_NOW)->version << '\n'
                               << "libboost " << BOOST_LIB_VERSION << '\n';
    }

    // for a single file request, copy the unit number to that input source
    if (srcml_request.input_sources.size() == 1 && srcml_request.unit != 0)
        srcml_request.input_sources[0].unit = srcml_request.unit;

    // determine if stdin is srcML or src
    if (srcml_request.stdindex)
        set_state_stdin(srcml_request);
 
    /*
        Setup the internal pipeline of possible steps:
        * creating srcml from src files and input srcml files
        * transforming srcml to srcml
        * displaying metadata about a srcml file
        * creating src from srcml
        * compressing output (compressing input performed on an individual input source)
    */
    // steps in the internal pipeline
    processing_steps_t pipeline;

    // step src->srcml
    if (request_create_srcml(srcml_request)) {

        pipeline.push_back(create_srcml);
    }

    // step srcml->srcml
    if (request_transform_srcml(srcml_request)) {

        pipeline.push_back(transform_srcml);
    }

    // step srcml->metadata
    if (request_display_metadata(srcml_request)) {

        pipeline.push_back(srcml_display_metadata);
    }

    // step srcml->src
    if (request_create_src(srcml_request)) {

        pipeline.push_back(create_src);
    }

    // step (srcml|src)->compressed
    if (request_output_compression(srcml_request)) {

#if ARCHIVE_VERSION_NUMBER > 3001002
        pipeline.push_back(compress_srcml);
#else
        SRCMLstatus(ERROR_MSG, "srcml: Unsupported output compression");
        exit(1);
#endif
    }

    // should always have something to do
    if (pipeline.empty()) {
        SRCMLstatus(ERROR_MSG, "srcml: Internal error, cannot decide what processing needed");
        exit(1);
    }

    // execute the pipeline
    srcml_execute(srcml_request, pipeline, srcml_request.input_sources, srcml_request.output_filename);

    srcml_cleanup_globals();

    // debugging information
    SRCMLstatus(DEBUG_MSG, "CPU Time: %l ms", runtime.cpu_time_elapsed());
    auto realtime = runtime.real_world_elapsed();
    SRCMLstatus(DEBUG_MSG, "Real Time: %l ms", realtime);
    SRCMLstatus(DEBUG_MSG, "LOC: %l", TraceLog::totalLOC());
    SRCMLstatus(DEBUG_MSG, "KLOC/s: %l", realtime > 0 ? std::round(TraceLog::totalLOC() / realtime) : 0);

    // error status is 0 unless a critical, error, or warning
    return SRCMLStatus::errors() ? 1 : 0;
}

namespace {

    /*
        Create srcML

        * One of the input sources is source code
        * Destination is srcML
    */
    bool request_create_srcml(const srcml_request_t& request) {

        return std::find_if(request.input_sources.begin(), request.input_sources.end(), is_src) != request.input_sources.end() ||
        (request.output_filename.state == SRCML && request.input_sources[0].unit == 0 && !option(SRCML_COMMAND_XML) && request.transformations.empty());
    }

    /*
        Transform srcml

        * Transformations requested
    */
    bool request_transform_srcml(const srcml_request_t& request) {

        return !request.transformations.empty();
    }

    /*
        Extract out of srcML

        * ?
        * 
    */
    bool request_display_metadata(const srcml_request_t& request) {

        return (option(SRCML_COMMAND_INSRCML) || request.xmlns_prefix_query || request.pretty_format);
    }

    /*
        Output is compressed

        * Format of output includes compression
    */
    bool request_output_compression(const srcml_request_t& request) {

        return request.output_filename.compressions.size() >= 1;
    }

    /*
        Creating source code

        * Specific option for source output
        * The destination is not a srcML file and we are not creating srcML, asking for metadata, or performing a transformation
    */
    bool request_create_src(const srcml_request_t& request) {

        return (option(SRCML_COMMAND_SRC) || (request.output_filename.state != SRCML &&
            !request_create_srcml(request) &&
            !request_display_metadata(request) &&
            !request_transform_srcml(request))) ||
            (request.input_sources.size() == 1 && request.input_sources[0].unit >= 0 && option(SRCML_COMMAND_XML));
        ;
    }

    void set_state_stdin(srcml_request_t& request) {

        set_state_isxml(request);
    }

    void set_state_isxml(srcml_request_t& request) {

        // stdin input source
        auto& rstdin = request.input_sources[*request.stdindex];

        // stdin accessed as FILE*
        rstdin.fileptr = fdopen(STDIN_FILENO, "r");
        if (!rstdin.fileptr) {
            SRCMLstatus(ERROR_MSG, "srcml: Unable to open stdin");
            exit(1);
        }
        rstdin.fd = boost::none;

        // setup a 5 second timeout for stdin from the terminal
        if (isatty(0)) {
//          #ifndef _MSC_BUILD
//          alarm(5);
//          signal(SIGALRM, timeout);
//          #endif
            timeout(0);
        }

        // determine if the input is srcML or src
        rstdin.state = isxml(*(rstdin.fileptr)) ? SRCML : SRC;
    }

}
