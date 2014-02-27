/*
  src_language.cpp

  Copyright (C) 2014 SDML (www.srcML.org)

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
  src_language.cpp functions for determining the language of a given input file
*/

#include <src_language.hpp>
#include <boost/filesystem.hpp>

std::string src_language(const std::string& inputFile) {
    boost::filesystem::path localPath(inputFile);
    std::string language;

    if (is_directory(localPath))
        language = "dir";

    if (is_regular_file(localPath)) {
        if (localPath.extension().string().compare(".xml") == 0) {
            language = "xml";
        }
        else {
            const char* lang = srcml_check_extension(inputFile.c_str());
            language = lang ? lang : "";
        }
    }
    return language;
}
