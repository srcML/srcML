/*
  src_input_validator.cpp

  Copyright (C) 2004-2014  SDML (www.srcML.org)

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
  src_input_validator is used to check input sources to determine if they are accessible.
*/

#include <src_input_validator.hpp>
#include <src_prefix.hpp>
#include <boost/filesystem.hpp>

// TODO: This should be const std::string&. You should not change the string in validation
// A src_validate should NOT change the string. Construct a new one and return it
bool src_validate(std::string& input) {
  if (input.compare("/dev/stdin") == 0 || input.compare("-") == 0) {
    input = "-";
    src_prefix_add_uri(input, "stdin://");
    return true;
  }
    
  // Check for local file
  if (input.find("http:") == std::string::npos){
    boost::filesystem::path localFile (input);
    
    // File is unavailable
    if (!exists(localFile))
      return false;

    src_prefix_add_uri(input, "file://");
  }

  return true;
}
