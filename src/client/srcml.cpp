// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml.cpp
 *
 * @copyright Copyright (C) 2014-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <srcml.h>
#include <srcml_cli.hpp>
#include <srcml_options.hpp>
#include <create_srcml.hpp>
#include <compress_srcml.hpp>
#include <create_src.hpp>
#include <srcml_display_metadata.hpp>
#include <srcml_execute.hpp>
#include <Timer.hpp>
#include <PerfCounters.hpp>
#include <SRCMLStatus.hpp>
#include <curl/curl.h>
#include <iostream>
#include <csignal>
#include <cmath>
#include <cstdio>
#include <TraceLog.hpp>
#include <stdin_libarchive.hpp>
#include <src_input_clipboard.hpp>
#include <input_archive.hpp>
#include <string_view>

#ifndef _MSC_VER
#include <sys/uio.h>
#include <unistd.h>
#endif

using namespace ::std::literals::string_view_literals;

namespace {
    // decide if a step is needed
    bool request_create_srcml      (const srcml_request_t&);
    bool request_display_metadata  (const srcml_request_t&);
    bool request_output_compression(const srcml_request_t&);
    bool request_create_src        (const srcml_request_t&);

    // format for debugging information
    std::string with_commas(std::uint64_t);
    std::string milliseconds(double);
}

const char* SRCML_CLIENT_VERSION_STRING = "1.2.0";
const int SRCML_CLIENT_VERSION_NUMBER = 10200;

int main(int argc, char* argv[]) {

    Timer runtime = Timer();

    // parse the command line
    auto srcml_request = parseCLI11(argc, argv);

    // hardware counters for debugging information
    PerfCounters counters(srcml_request.command & (SRCML_DEBUG_MODE | SRCML_TIMING_MODE));

    // global access to options
    SRCMLOptions::set(srcml_request.command);

    // version
    if (option(SRCML_COMMAND_VERSION)) {

        // output the client and libsrcml versions
        std::cout << "srcml: " << SRCML_CLIENT_VERSION_STRING << '\n'
                  << "libsrcml: " << srcml_libsrcml_version_string() << '\n'
                  << "srcql: " << "1.0.0" << '\n';

        std::cout << "markup:" << '\n';

        // output Python support first
        for (size_t i = 0; i < srcml_get_language_list_size(); ++i) {
            std::string_view language = srcml_get_language_list(i);
            if (language != "Python"sv)
                continue;

            std::cout << "  " << language << ": " << srcml_markup_version_string(srcml_get_language_list(i)) << '\n';
            break;
        }

        // output other languages support
        for (size_t i = 0; i < srcml_get_language_list_size(); ++i) {
            std::string_view language = srcml_get_language_list(i);
            if (language == "Python"sv)
                continue;

            std::cout << "  " << language << ": " << srcml_markup_version_string(srcml_get_language_list(i)) << '\n';
        }

        return 0;
    }

    // debug info
    if (srcml_request.command & SRCML_DEBUG_MODE) {
        SRCMLstatus(DEBUG_MSG) << "Library Versions: " << '\n'
                               << "srcml markup " << srcml_markup_version_string("C++") << '\n'
                               << "srcml client " << SRCML_CLIENT_VERSION_STRING << '\n'
                               << "libsrcml " << srcml_libsrcml_version_string() << '\n'
                               <<  archive_version_string() << '\n'
                               << "libcurl " << curl_version_info(CURLVERSION_NOW)->version << '\n';
    }

    // for a single file request, copy the unit number to that input source
    if (srcml_request.input_sources.size() == 1 && srcml_request.unit != 0)
        srcml_request.input_sources[0].unit = srcml_request.unit;

    // stdin from a terminal is not allowed
    if (srcml_request.stdindex && isatty(0)) {

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

    // determine if stdin is srcML or src
    if (srcml_request.stdindex) {

        // pre-read stdin to determine if source code or srcML
        auto& stdInput = srcml_request.input_sources[*srcml_request.stdindex];
        open_stdin_libarchive(stdInput);

        // determine source or srcML input based on content
        stdInput.state = stdInput.issrcML ? SRCML : SRC;

        // for srcML stdin, convert from libarchive input (should just be compression)
        if (stdInput.issrcML) {
            stdInput.fd = input_archive(stdInput);
        }
    }

    // read any clipboard input, determining if it is source code or srcML
    for (auto& input : srcml_request.input_sources) {
        if (input.protocol != "clipboard"sv)
            continue;

        open_clipboard(input);
        input.state = input.issrcML ? SRCML : SRC;
    }

    // a line and column suffix is a position in a source file, and they start at one
    for (const auto& input : srcml_request.input_sources) {

        if (input.line == srcml_input_src::INVALID_POSITION || input.end_line == srcml_input_src::INVALID_POSITION) {
            SRCMLstatus(ERROR_MSG, "srcml: Invalid line number 0 in %s", src_prefix_resource(input.filename));
            exit(1);
        }

        if (input.column == srcml_input_src::INVALID_POSITION || input.end_column == srcml_input_src::INVALID_POSITION) {
            SRCMLstatus(ERROR_MSG, "srcml: Invalid column number 0 in %s", src_prefix_resource(input.filename));
            exit(1);
        }

        if (input.line == 0)
            continue;

        // the end of a range is at, or after, the start
        if (input.end_line != srcml_input_src::END_OF_FILE &&
            (input.end_line < input.line ||
                (input.end_line == input.line && input.end_column != 0 && input.end_column < std::max(input.column, 1)))) {
            SRCMLstatus(ERROR_MSG, "srcml: Invalid range with the end before the start in %s", src_prefix_resource(input.filename));
            exit(1);
        }

        if (input.state == SRCML) {
            SRCMLstatus(ERROR_MSG, "srcml: Line suffix not allowed on the srcML input %s", src_prefix_resource(input.filename));
            exit(1);
        }

        if (input.isdirectory) {
            SRCMLstatus(ERROR_MSG, "srcml: Line suffix not allowed on the directory %s", src_prefix_resource(input.filename));
            exit(1);
        }

        // an archive holds more than one file, so a position in it is ambiguous, while a compression is a single file
        if (!input.archives.empty()) {
            SRCMLstatus(ERROR_MSG, "srcml: Line suffix not allowed on the archive %s", src_prefix_resource(input.filename));
            exit(1);
        }
    }

    /*
        Setup the internal pipeline of possible steps:
        * creating srcml from src files and input srcml files, and transforming srcml
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

#if ARCHIVE_VERSION_NUMBER >= 3002000
        pipeline.push_back(compress_srcml);
#else
    SRCMLstatus(ERROR_MSG, "srcml: Unable to compress output with the libraries on this platform");
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
    if (srcml_request.command & SRCML_DEBUG_MODE || srcml_request.command & SRCML_TIMING_MODE) {
        auto realtime = runtime.real_world_elapsed();
        SRCMLstatus(DEBUG_MSG) << "CPU Time: " << milliseconds(runtime.cpu_time_elapsed()) << '\n'
                               << "Real Time: " << milliseconds(realtime) << '\n';
        std::uint64_t instructions = 0;
        std::uint64_t cycles = 0;
        if (counters.read(instructions, cycles)) {
            SRCMLstatus(DEBUG_MSG) << "Instructions: " << PerfCounters::scaled(instructions) << '\n'
                                   << "Cycles: " << PerfCounters::scaled(cycles) << '\n';
            if (cycles > 0) {
                SRCMLstatus(DEBUG_MSG) << "IPC: " << std::round(100.0 * instructions / cycles) / 100 << '\n';
            }
        }
        if (TraceLog::totalLOC() > 0) {
            SRCMLstatus(DEBUG_MSG) << "LOC: " << with_commas(TraceLog::totalLOC()) << '\n'
                                   << "KLOC/s: " << with_commas(realtime > 0 ? (std::uint64_t) std::round((double) TraceLog::totalLOC() / realtime) : 0) << '\n';
        }

        SRCMLstatus(DEBUG_MSG) << "Status: " << (SRCMLStatus::errors() ? 1 : 0) << '\n';
    }

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

        if (!request.transformations.empty()) {
            enable(SRCML_COMMAND_XML);
        }

        if (option(SRCML_COMMAND_PARSER_TEST))
            return true;

        if (option(SRCML_COMMAND_CAT_XML))
            return true;

        return std::find_if(request.input_sources.begin(), request.input_sources.end(), [](const srcml_input_src& input) { return input.state == SRC; }) != request.input_sources.end() ||
        (request.output_filename.state == SRCML || option(SRCML_COMMAND_XML)) ||
        !request.transformations.empty();
    }

    /*
        Extract from srcML
        * Request any sort of metadata
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
            !request_display_metadata(request)));
    }

    /*
        Integer with commas between groups of three digits, e.g., 27,154,393
    */
    std::string with_commas(std::uint64_t value) {

        std::string digits = std::to_string(value);
        std::string result;
        for (std::size_t i = 0; i < digits.size(); ++i) {
            if (i > 0 && (digits.size() - i) % 3 == 0)
                result += ',';
            result += digits[i];
        }

        return result;
    }

    /*
        Time with 4 significant digits, in milliseconds under a second, otherwise in seconds, e.g., 28.01ms, 152.8s
    */
    std::string milliseconds(double ms) {

        // seconds when the value would round to 1000.0ms or more
        double value = ms;
        const char* unit = "ms";
        if (value >= 999.95) {
            value /= 1000;
            unit = "s";
        }
        int decimals = value < 10 ? 3 : value < 100 ? 2 : 1;

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.*f%s", decimals, value, unit);
        return buffer;
    }
}
