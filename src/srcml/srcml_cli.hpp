/*
  srcml_cli.hpp

  @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
  srcml_cli handles parsing for CLI options for srcml
*/

#ifndef SRCML_CLI_HPP
#define SRCML_CLI_HPP

#include <srcml.h>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <iostream>

// Internal srcml command options
#define SRCML_COMMAND_LONGINFO          1<<0
#define SRCML_COMMAND_INFO              1<<1
#define SRCML_COMMAND_INFO_FILENAME     1<<2

#define SRCML_COMMAND_CPP_TEXT_IF0      1<<4
#define SRCML_COMMAND_CPP_MARKUP_ELSE   1<<5
#define SRCML_COMMAND_QUIET             1<<6
#define SRCML_COMMAND_VERBOSE           1<<7
#define SRCML_COMMAND_VERSION           1<<8

#define SRCML_COMMAND_EXPRESSION        1<<10
#define SRCML_COMMAND_INTERACTIVE       1<<11
#define SRCML_COMMAND_XML               1<<12
#define SRCML_COMMAND_LIST              1<<13
#define SRCML_COMMAND_UNITS             1<<14
#define SRCML_COMMAND_INFO_DIRECTORY    1<<15
#define SRCML_COMMAND_INFO_ENCODING     1<<16
#define SRCML_COMMAND_INFO_LANGUAGE     1<<17
#define SRCML_COMMAND_INFO_SRC_VERSION  1<<18
#define SRCML_COMMAND_TO_DIRECTORY      1<<19

// request for srcml client processing
struct srcml_request_t {
    std::vector<std::string> positional_args;
    int command;
    int markup_options;

    // unit attributes
    std::string att_language;
    boost::optional<std::string> att_filename;
    boost::optional<std::string> att_directory;
    boost::optional<std::string> att_encoding;
    boost::optional<std::string> att_src_versions;

    std::string src_encoding;

    std::string output_filename;

    std::string files_from;
    std::vector<std::string> language_ext;
    int tabs;

    // xml namespaces
    std::string prefix;
    std::string xmlns_uri;
    std::vector<std::string> xmlns_prefix;

    // srcml transformation
    std::string xpath;
    std::vector<std::string> xpathparam;
    std::string xslt;
    std::string relaxng;

    int unit;
    int max_threads;
    bool help_set;
    bool src_versions_set;
};

// parse the CLI options into a srcml client request
srcml_request_t parseCLI(int argc, char* argv[]);

#endif
