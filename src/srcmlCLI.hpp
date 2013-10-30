/*
  srcmlCLI.hpp

  Copyright (C) 2004-2013  SDML (www.srcML.org)

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
  srcmlCLI handles parsing for CLI options for srcml
*/

#ifndef SRCMLCLI_HPP
#define SRCMLCLI_HPP

#include <string>
#include <vector>

struct srcml_request_t {
  int command;
  int markup_options;
  std::string filename;
  std::string output;
  std::string src_encoding;
  std::string encoding;
  std::string files_from;
  std::string language;
  std::vector<std::string> register_ext;
  int tabs;
  std::string directory;
  std::string src_versions;
  std::string prefix;
  std::string xmlns_uri;
  std::vector<std::string> xmlns_prefix;
  std::string relaxng;
  std::string xpath;
  std::vector<std::string> xpathparam;
  std::string xslt;
  int unit;
  std::vector<std::string> positional_args;
};

class srcmlCLI {
  public :
    static srcml_request_t parseCLI (int argc, const char* argv);
}