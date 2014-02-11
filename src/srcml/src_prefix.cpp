/*
  src_prefix.hpp

  Copyright (C) 2014  SDML (www.srcML.org)

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
  src_prefix.hpp functions for adding/splitting assigned prefixes from input sources
*/

#include <src_prefix.hpp>

const char* PROTOCOL_SEPARATOR = "://";

void src_prefix_add_uri(std::string& input_file, const std::string& protocol) {
  input_file = input_file.insert(0, protocol);
}

void src_prefix_split_uri(const std::string& input_file, std::string& protocol, std::string& resource){
  // Extract function split_uri(input_file, protocol, resource)
  size_t prefixPos = input_file.find(PROTOCOL_SEPARATOR);

  if (prefixPos == std::string::npos) {
    protocol = "";
    resource = "";
    return;
  }
  protocol = input_file.substr(0, prefixPos);
  resource = input_file.substr(prefixPos + strlen(PROTOCOL_SEPARATOR));
}
