/**
 * @file srcml_cli.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef SRCML_CLI_HPP
#define SRCML_CLI_HPP

#include <srcml.h>
#include <string>
#include <vector>
#include <boost/optional.hpp>
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
    boost::optional<std::string> prefix;
    boost::optional<std::string> name;
    boost::optional<std::string> value;
};

struct element {
    boost::optional<std::string> prefix;
    boost::optional<std::string> name;
};

// request for srcml client processing
struct srcml_request_t {
    srcml_input_t input_sources;

    boost::optional<int> stdindex;

    int command = 0;
    boost::optional<int> markup_options;

    // unit attributes
    boost::optional<std::string> att_language;
    boost::optional<std::string> att_filename;
    boost::optional<std::string> att_url;
    boost::optional<std::string> att_xml_encoding;
    boost::optional<std::string> att_version;

    boost::optional<std::string> src_encoding;

    boost::optional<int> eol;

    boost::optional<std::string> external;

    srcml_output_dest output_filename;

    //filelist:// prefix
    std::vector<std::string> files_from;
    std::vector<std::string> language_ext;
    int tabs;

    // xml namespaces
    boost::optional<std::string> xmlns_prefix_query;

    // xml processing attributes
    boost::optional<std::string> xml_processing;

    std::map<std::string,std::string> xmlns_namespaces;

    // Use for checking for overwriting standard namespaces ("", cpp)
    std::map<std::string,std::string> xmlns_namespace_uris;

    // srcml transformation
    std::vector<std::string> transformations;
    std::vector< std::pair< boost::optional<element>, boost::optional<attribute> > > xpath_query_support;

    int unit = 0;
    int max_threads;

    boost::optional<std::string> pretty_format;

    boost::optional<size_t> revision;

    // pre-input
    char buf[4] = { 0 };
    size_t bufsize = 0;
};

// parse the CLI options into a srcml client request
srcml_request_t parseCLI11(int argc, char* argv[]);

#endif
