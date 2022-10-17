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

// Internal srcml command options
const int SRCML_COMMAND_LONGINFO                  = 1<<0;
const int SRCML_COMMAND_INFO                      = 1<<1;

const int SRCML_COMMAND_CPP_TEXT_IF0              = 1<<2;
const int SRCML_COMMAND_CPP_MARKUP_ELSE           = 1<<3;
const int SRCML_COMMAND_QUIET                     = 1<<4;
const int SRCML_COMMAND_VERBOSE                   = 1<<5;
const int SRCML_COMMAND_VERSION                   = 1<<6;

const int SRCML_COMMAND_XML                       = 1<<7;
const int SRCML_COMMAND_SRC                       = 1<<8;
const int SRCML_COMMAND_LIST                      = 1<<9;
const int SRCML_COMMAND_UNITS                     = 1<<10;

const int SRCML_COMMAND_TO_DIRECTORY              = 1<<11;
const int SRCML_COMMAND_TIMESTAMP                 = 1<<12;

const int SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE    = 1<<13;
const int SRCML_COMMAND_DISPLAY_SRCML_URL         = 1<<14;
const int SRCML_COMMAND_DISPLAY_SRCML_FILENAME    = 1<<15;
const int SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION = 1<<16;
const int SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP   = 1<<17;
const int SRCML_COMMAND_DISPLAY_SRCML_HASH        = 1<<18;
const int SRCML_COMMAND_DISPLAY_SRCML_ENCODING    = 1<<19;

const int SRCML_COMMAND_NO_COLOR                  = 1<<20;

const int SRCML_COMMAND_UPDATE                    = 1<<21;

const int SRCML_COMMAND_NOARCHIVE                 = 1<<22;

const int SRCML_DEBUG_MODE                        = 1<<23;

const int SRCML_TIMING_MODE                       = 1<<24;

const int SRCML_ARCHIVE                           = 1<<25;

const int SRCML_HASH                              = 1<<26;

const int SRCML_COMMAND_XML_RAW                   = 1<<27;
const int SRCML_COMMAND_XML_FRAGMENT              = 1<<28;

const int SRCML_COMMAND_PARSER_TEST               = 1<<29;

const int SRCML_COMMAND_CAT_XML                   = 1<<30;

// commands that are simple queries on srcml
const int SRCML_COMMAND_INSRCML =
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

    int command = 0;
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
