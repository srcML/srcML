/**
 * @file src_input_filesystem.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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

/*
  src_input_filesystem assigns directories to parse request
*/

#include <src_input_libarchive.hpp>
#include <src_input_filesystem.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <list>
#include <vector>

void src_input_filesystem(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          const std::string& input) {

    // list of directories with the input as the first one
    std::list<boost::filesystem::path> dirs(1, boost::filesystem::path(input));

    while (!dirs.empty()) {

        // prepare a list of sorted files for the back() directory
        std::vector<boost::filesystem::path> files(boost::filesystem::directory_iterator(dirs.back()),
                                                   boost::filesystem::directory_iterator());
        sort(files.begin(), files.end());
        dirs.pop_back();

        // process the files from the top directory
        BOOST_FOREACH(boost::filesystem::path& file, files) {

            // regular files are passed to the handler
            if (is_regular_file(file))
                src_input_libarchive(queue, srcml_arch, srcml_request, file.string());

            // directories are put at the back
            else if (is_directory(file))
                dirs.push_back(file);

              // TODO: Are we ignoring other types? symlinks? Should state so here.
        }
    }
}
