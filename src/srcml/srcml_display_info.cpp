/*
  srcml_display_info.cpp

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
  srcml_display_info.cpp display info about a given srcml input file.
*/

#include <srcml_display_info.hpp>
#include <src_prefix.hpp>
#include <srcml.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

// TODO: Need to show encoding
// TODO: Need to not show language for archive
void srcml_display_info(const std::vector<std::string>& pos_args) {
  BOOST_FOREACH(const std::string& input_file, pos_args) {
    std::string resource;
    std::string protocol;
    src_prefix_split_uri(input_file, protocol, resource);
    boost::filesystem::path file (resource);
    if(file.extension().compare(".xml") == 0)
      srcml_display_info(resource);
  }
}

void srcml_display_info(const std::string& srcml_input) {

  int numUnits = 0;
  srcml_archive* srcml_arch = srcml_create_archive();
  srcml_read_open_filename(srcml_arch, srcml_input.c_str());
 
  while (true) {
    srcml_unit* unit = srcml_read_unit(srcml_arch);
    if (unit == 0)
      break;

    ++numUnits;

    /* Query options of srcml unit */
    const char* language = srcml_unit_get_language(unit);
    if (language)
      std::cout << "Language: " << language << "\n";          
    
    if (srcml_unit_get_filename(unit))
      std::cout << "Filename: " << srcml_unit_get_filename(unit) << "\n";

    if (srcml_unit_get_directory(unit))
      std::cout << "Directory: " << srcml_unit_get_directory(unit) << "\n";
    
    if (srcml_unit_get_version(unit))
      std::cout << "Version: " << srcml_unit_get_version(unit) << "\n";

    srcml_free_unit(unit);
  }

  std::cout << "units=" << numUnits << "\n";

  srcml_free_archive(srcml_arch);
}
