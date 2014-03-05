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

void src_input_filesystem(ParseQueue& queue, srcml_archive* srcml_arch, const std::string& input, const boost::optional<std::string>& lang) {
    boost::filesystem::path localPath(input);
    std::list<boost::filesystem::path> dirs;
    dirs.push_front(localPath);

    while (!dirs.empty()) {
        std::vector<boost::filesystem::path> files;
        copy(boost::filesystem::directory_iterator(dirs.front()), boost::filesystem::directory_iterator(), back_inserter(files));
        sort(files.begin(), files.end());
        dirs.pop_front();

        std::list<boost::filesystem::path>::iterator dir_iter=dirs.begin();

        for (std::vector<boost::filesystem::path>::const_iterator it (files.begin()); it != files.end(); ++it) {
            if (is_directory(boost::filesystem::path(*it))) {
                dirs.insert(dir_iter,*it);
                std::list<boost::filesystem::path>::iterator iter;
                continue;
            }

            if (is_regular_file(*it)) {
                if (srcml_archive_check_extension(srcml_arch, it->string().c_str()))
                    src_input_libarchive(queue, srcml_arch, it->string(), 0, 0, lang);
            }
        }
    }
}
