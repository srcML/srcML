/**
 * @file srcml_cli.hpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

#ifndef SRCML_CLI_HPP
#define SRCML_CLI_HPP

#include <srcml.h>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <iostream>
#include <utility>
#include <map>

// Internal srcml command options
const int SRCML_COMMAND_LONGINFO                  = 1<<0;
const int SRCML_COMMAND_INFO                      = 1<<1;

const int SRCML_COMMAND_CPP_TEXT_IF0              = 1<<2;
const int SRCML_COMMAND_CPP_MARKUP_ELSE           = 1<<3;
const int SRCML_COMMAND_QUIET                     = 1<<4;
const int SRCML_COMMAND_VERBOSE                   = 1<<5;
const int SRCML_COMMAND_VERSION                   = 1<<6;

const int SRCML_COMMAND_EXPRESSION                = 1<<7;
const int SRCML_COMMAND_INTERACTIVE               = 1<<8;
const int SRCML_COMMAND_XML                       = 1<<9;
const int SRCML_COMMAND_LIST                      = 1<<10;
const int SRCML_COMMAND_UNITS                     = 1<<11;

const int SRCML_COMMAND_TO_DIRECTORY              = 1<<12;
const int SRCML_COMMAND_TIMESTAMP                 = 1<<13;
const int SRCML_COMMAND_HASH                      = 1<<14;

const int SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE    = 1<<15;
const int SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY   = 1<<16;
const int SRCML_COMMAND_DISPLAY_SRCML_FILENAME    = 1<<17;
const int SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION = 1<<18;
const int SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP   = 1<<19;
const int SRCML_COMMAND_DISPLAY_SRCML_HASH        = 1<<20;
const int SRCML_COMMAND_DISPLAY_SRCML_ENCODING    = 1<<21;

const int SRCML_COMMAND_OUTPUT_ORDERED            = 1<<22;
const int SRCML_COMMAND_UPDATE                    = 1<<23;

const int SRCML_COMMAND_NOARCHIVE                 = 1<<24; 

// commands that are simple queries on srcml
const int SRCML_COMMAND_INSRCML =
    SRCML_COMMAND_LONGINFO |
    SRCML_COMMAND_INFO    |
    SRCML_COMMAND_VERSION |
    SRCML_COMMAND_LIST |
    SRCML_COMMAND_UNITS |
    SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE |
    SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY |
    SRCML_COMMAND_DISPLAY_SRCML_FILENAME |
    SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION |
    SRCML_COMMAND_DISPLAY_SRCML_ENCODING |
    SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP | 
    SRCML_COMMAND_DISPLAY_SRCML_HASH;

// request for srcml client processing
struct srcml_request_t {

    std::vector<std::string> input;
    boost::optional<int> stdindex;

    int command;
    boost::optional<int> markup_options;

    // unit attributes
    boost::optional<std::string> att_language;
    boost::optional<std::string> att_filename;
    boost::optional<std::string> att_directory;
    boost::optional<std::string> att_xml_encoding;
    boost::optional<std::string> att_version;

    boost::optional<std::string> src_encoding;

    boost::optional<std::string> output_filename;

    //filelist:// prefix
    std::vector<std::string> files_from;
    std::vector<std::string> language_ext;
    int tabs;

    // xml namespaces
    boost::optional<std::string> xmlns_prefix_query;

    // xml processing attributes
    boost::optional<std::string> xml_processing;    

    std::vector<std::string> xmlns_prefix;
    std::map<std::string,std::string> xmlns_namespaces;

    // srcml transformation
    std::vector<std::string> transformations;
    std::vector< std::pair< boost::optional<std::string>, boost::optional<std::string> > > xpath_query_support;

    int unit;
    int max_threads;
};

// parse the CLI options into a srcml client request
srcml_request_t parseCLI(int argc, char* argv[]);

#endif
