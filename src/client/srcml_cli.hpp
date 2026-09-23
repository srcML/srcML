// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_cli.hpp
 *
 * @copyright Copyright (C) 2014-2024 srcML, LLC. (www.srcML.org)
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
const unsigned long long SRCML_COMMAND_LONGINFO                  = 1ull << 0ull;
const unsigned long long SRCML_COMMAND_INFO                      = 1ull << 1ull;

const unsigned long long SRCML_COMMAND_CPP_TEXT_IF0              = 1ull << 2ull;
const unsigned long long SRCML_COMMAND_CPP_MARKUP_ELSE           = 1ull << 3ull;
const unsigned long long SRCML_COMMAND_QUIET                     = 1ull << 4ull;
const unsigned long long SRCML_COMMAND_VERBOSE                   = 1ull << 5ull;
const unsigned long long SRCML_COMMAND_VERSION                   = 1ull << 6ull;

const unsigned long long SRCML_COMMAND_XML                       = 1ull << 7ull;
const unsigned long long SRCML_COMMAND_SRC                       = 1ull << 8ull;
const unsigned long long SRCML_COMMAND_LIST                      = 1ull << 9ull;
const unsigned long long SRCML_COMMAND_UNITS                     = 1ull << 10ull;

const unsigned long long SRCML_COMMAND_TO_DIRECTORY              = 1ull << 11ull;
const unsigned long long SRCML_COMMAND_TIMESTAMP                 = 1ull << 12ull;

const unsigned long long SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE    = 1ull << 13ull;
const unsigned long long SRCML_COMMAND_DISPLAY_SRCML_URL         = 1ull << 14ull;
const unsigned long long SRCML_COMMAND_DISPLAY_SRCML_FILENAME    = 1ull << 15ull;
const unsigned long long SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION = 1ull << 16ull;
const unsigned long long SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP   = 1ull << 17ull;
const unsigned long long SRCML_COMMAND_DISPLAY_SRCML_HASH        = 1ull << 18ull;
const unsigned long long SRCML_COMMAND_DISPLAY_SRCML_ENCODING    = 1ull << 19ull;

const unsigned long long SRCML_COMMAND_NO_COLOR                  = 1ull << 20ull;

const unsigned long long SRCML_COMMAND_UPDATE                    = 1ull << 21ull;

const unsigned long long SRCML_COMMAND_NOARCHIVE                 = 1ull << 22ull;

const unsigned long long SRCML_DEBUG_MODE                        = 1ull << 23ull;

const unsigned long long SRCML_TIMING_MODE                       = 1ull << 24ull;

const unsigned long long SRCML_ARCHIVE                           = 1ull << 25ull;

const unsigned long long SRCML_HASH                              = 1ull << 26ull;

const unsigned long long SRCML_COMMAND_XML_RAW                   = 1ull << 27ull;
const unsigned long long SRCML_COMMAND_XML_FRAGMENT              = 1ull << 28ull;

const unsigned long long SRCML_COMMAND_PARSER_TEST               = 1ull << 29ull;

const unsigned long long SRCML_COMMAND_CAT_XML                   = 1ull << 30ull;

const unsigned long long SRCML_COMMAND_NULL                      = 1ull << 31ull;

const unsigned long long SRCML_COMMAND_SRCQL_WARNING_OFF         = 1ull << 32ull;

const unsigned long long SRCML_COMMAND_HEADER                    = 1ull << 33ull;

// commands that are simple queries on srcml
const unsigned long long SRCML_COMMAND_INSRCML =
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

    unsigned long long command = 0ull;

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

    // pre-input
    char buf[4] = { 0 };
    size_t bufsize = 0;
};

// parse the CLI options into a srcml client request
srcml_request_t parseCLI11(int argc, char* argv[]);

#endif
