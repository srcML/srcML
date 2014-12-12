/**
 * @file src_input_filelist.cpp
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

#include <src_input_filelist.hpp>
#include <src_input_libarchive.hpp>
#include <src_input_file.hpp>
#include <src_input_filesystem.hpp>
#include <boost/foreach.hpp>
#include <create_srcml.hpp>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

void src_input_filelist(ParseQueue& queue,
                        srcml_archive* srcml_arch,
                        const srcml_request_t& srcml_request,
                        const std::string& input) {

    std::ifstream filelist(input.c_str());
    std::string line;
    while (getline(filelist, line)) {

		boost::algorithm::trim(line);

		if (line[0] == '#')
			continue;

        // TODO: Trim leading and trailing whitespace
        if (line[0] == '\n')
            continue;

        srcml_handler_dispatch(queue, srcml_arch, srcml_request, line);
    }
}
