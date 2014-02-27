/*
  srcml_list_unit_files.cpp

  @copyright Copyright (C) 2014  SDML (www.srcML.org)

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
  srcml_list_unit_files.cpp function for listing all unit files contained in a srcml archive
*/

#include <srcml_list_unit_files.hpp>
#include <srcml.h>
#include <iostream>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <src_prefix.hpp>

void srcml_list_unit_files(const std::vector<std::string>& pos_args) {
    BOOST_FOREACH(const std::string& input_file, pos_args) {

        boost::filesystem::path file(src_prefix_resource(input_file));
        if(file.extension().compare(".xml") == 0)
            srcml_list_unit_files(file.string());
    }
}

// display all files in srcml archive
void srcml_list_unit_files(const std::string& srcml_input) {

    srcml_archive* srcml_arch = srcml_create_archive();
    srcml_read_open_filename(srcml_arch, srcml_input.c_str());

    int numUnits = 0;
    while (srcml_unit* unit = srcml_read_unit_header(srcml_arch)) {

        ++numUnits;

        std::cout << std::setw(5) << numUnits << ' ' << srcml_unit_get_filename(unit) << '\n';

        srcml_free_unit(unit);
    }

    srcml_close_archive(srcml_arch);
    srcml_free_archive(srcml_arch);
}
