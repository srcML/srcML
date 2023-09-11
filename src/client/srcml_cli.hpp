// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_cli.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRCML_CLI_HPP
#define SRCML_CLI_HPP

// #include <srcml.h>
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <utility>
#include <map>
#include <srcml_input_src.hpp>
#include <string_view>
#include <memory>
#include <libarchive_utilities.hpp>

// Internal srcml command options
const uint_fast64_t SRCML_COMMAND_LONGINFO                  = 1ULL << 0;
const uint_fast64_t SRCML_COMMAND_INFO                      = 1ULL << 1;

const uint_fast64_t SRCML_COMMAND_CPP_TEXT_IF0              = 1ULL << 2;
const uint_fast64_t SRCML_COMMAND_CPP_MARKUP_ELSE           = 1ULL << 3;
const uint_fast64_t SRCML_COMMAND_QUIET                     = 1ULL << 4;
const uint_fast64_t SRCML_COMMAND_VERBOSE                   = 1ULL << 5;
const uint_fast64_t SRCML_COMMAND_VERSION                   = 1ULL << 6;

const uint_fast64_t SRCML_COMMAND_XML                       = 1ULL << 7;
const uint_fast64_t SRCML_COMMAND_SRC                       = 1ULL << 8;
const uint_fast64_t SRCML_COMMAND_LIST                      = 1ULL << 9;
const uint_fast64_t SRCML_COMMAND_UNITS                     = 1ULL << 10;

const uint_fast64_t SRCML_COMMAND_TO_DIRECTORY              = 1ULL << 11;
const uint_fast64_t SRCML_COMMAND_TIMESTAMP                 = 1ULL << 12;

const uint_fast64_t SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE    = 1ULL << 13;
const uint_fast64_t SRCML_COMMAND_DISPLAY_SRCML_URL         = 1ULL << 14;
const uint_fast64_t SRCML_COMMAND_DISPLAY_SRCML_FILENAME    = 1ULL << 15;
const uint_fast64_t SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION = 1ULL << 16;
const uint_fast64_t SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP   = 1ULL << 17;
const uint_fast64_t SRCML_COMMAND_DISPLAY_SRCML_HASH        = 1ULL << 18;
const uint_fast64_t SRCML_COMMAND_DISPLAY_SRCML_ENCODING    = 1ULL << 19;

const uint_fast64_t SRCML_COMMAND_NO_COLOR                  = 1ULL << 20;

const uint_fast64_t SRCML_COMMAND_UPDATE                    = 1ULL << 21;

const uint_fast64_t SRCML_COMMAND_NOARCHIVE                 = 1ULL << 22;

const uint_fast64_t SRCML_DEBUG_MODE                        = 1ULL << 23;

const uint_fast64_t SRCML_TIMING_MODE                       = 1ULL << 24;

const uint_fast64_t SRCML_ARCHIVE                           = 1ULL << 25;

const uint_fast64_t SRCML_HASH                              = 1ULL << 26;

const uint_fast64_t SRCML_COMMAND_XML_RAW                   = 1ULL << 27;
const uint_fast64_t SRCML_COMMAND_XML_FRAGMENT              = 1ULL << 28;

const uint_fast64_t SRCML_COMMAND_PARSER_TEST               = 1ULL << 29;

const uint_fast64_t SRCML_COMMAND_CAT_XML                   = 1ULL << 30;

const uint_fast64_t SRCML_COMMAND_NULL                      = 1ULL << 31;

const uint_fast64_t SRCML_COMMAND_HEADER                    = 1ULL << 32;

// commands that are simple queries on srcml
const uint_fast64_t SRCML_COMMAND_INSRCML =
    SRCML_COMMAND_LONGINFO |
    SRCML_COMMAND_INFO    |
    SRCML_COMMAND_VERSION |
    SRCML_COMMAND_LIST |
    SRCML_COMMAND_UNITS |
    SRCML_COMMAND_UNITS |
    SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE |
    SRCML_COMMAND_DISPLAY_SRCML_URL |
    SRCML_COMMAND_DISPLAY_SRCML_FILENAME |
    SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION |
    SRCML_COMMAND_DISPLAY_SRCML_ENCODING |
    SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP |
    SRCML_COMMAND_DISPLAY_SRCML_HASH;

// Error Codes
const int CLI_STATUS_OK = 0;
const int CLI_STATUS_ERROR = 1;
const int CLI_STATUS_INTERNAL_ERROR = 2;

struct attribute {
    std::optional<std::string> prefix;
    std::optional<std::string> name;
    std::optional<std::string> value;
};

struct element {
    std::optional<std::string> prefix;
    std::optional<std::string> name;
};

// request for srcml client processing
struct srcml_request_t {
    srcml_input_t input_sources;

    std::optional<std::size_t> stdindex;

    uint_fast64_t command = 0;
    std::optional<int> markup_options;

    // unit attributes
    std::optional<std::string> att_language;
    std::optional<std::string> att_filename;
    std::optional<std::string> att_url;
    std::optional<std::string> att_xml_encoding;
    std::optional<std::string> att_version;

    std::optional<std::string> src_encoding;

    std::optional<size_t> eol;

    std::optional<std::string> external;

    srcml_output_dest output_filename;

    //filelist:// prefix
    std::vector<std::string> files_from;
    std::vector<std::string> language_ext;
    size_t tabs;

    // xml namespaces
    std::optional<std::string> xmlns_prefix_query;

    // xml processing attributes
    std::optional<std::string> xml_processing;

    std::map<std::string,std::string> xmlns_namespaces;

    // Use for checking for overwriting standard namespaces ("", cpp)
    std::map<std::string,std::string> xmlns_namespace_uris;

    // srcml transformation
    std::vector<std::string> transformations;
    std::vector< std::pair< std::optional<element>, std::optional<attribute> > > xpath_query_support;

    int unit = 0;
    int max_threads;

    std::optional<std::string> pretty_format;

    std::optional<size_t> revision;

    // pre-input
    char buf[4] = { 0 };
    size_t bufsize = 0;
};

// parse the CLI options into a srcml client request
srcml_request_t parseCLI11(int argc, char* argv[]);

#endif
