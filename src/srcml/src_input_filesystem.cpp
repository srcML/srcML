/**
 * @file src_input_filesystem.cpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
  src_input_filesystem assigns directories to parse request
*/

#include <src_input_libarchive.hpp>
#include <src_input_filesystem.hpp>
#include <boost/filesystem.hpp>
#include <list>
#include <vector>

void src_input_filesystem(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const std::string& input,
                          const boost::optional<std::string>& language)
{
    // setup the list of directories with the input as the first one
    std::list<boost::filesystem::path> dirs(1, boost::filesystem::path(input));

    while (!dirs.empty()) {

        // prepare a list of sorted files for the back() directory
        std::vector<boost::filesystem::path> files(boost::filesystem::directory_iterator(dirs.back()),
                                                   boost::filesystem::directory_iterator());
        sort(files.begin(), files.end());
        dirs.pop_back();

        // process the files from the top direcotry
        for (std::vector<boost::filesystem::path>::const_iterator it (files.begin()); it != files.end(); ++it) {

            // regular files are passed to the handler
            if (is_regular_file(*it))
                src_input_libarchive(queue, srcml_arch, it->string(), 0, 0, 0, language);

            // directories are put at the back()??? or back???
            else if (is_directory(*it))
                dirs.push_back(*it);
        }
    }
}
