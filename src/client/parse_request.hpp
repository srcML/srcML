/**
 * @file parse_request.hpp
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

#ifndef PARSE_REQUEST_HPP
#define PARSE_REQUEST_HPP

#include <srcml.h>

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <timer.hpp>

struct ParseRequest {
    ParseRequest(int size = 0) : buffer(size), srcml_arch(0), unit(0), position(0), status(0), loc(0) {}

    // Fields required by thread to process a unit
    std::string language;
    boost::optional<std::string> filename;
    boost::optional<std::string> url;
    boost::optional<std::string> version;
    std::vector<char> buffer;
    srcml_archive* srcml_arch;
    srcml_unit* unit;
    boost::optional<std::string> disk_filename;
    boost::optional<std::string> disk_dir;
    int position;
    int status;
    long loc;
    long runtime;
    boost::optional<std::string> time_stamp;
};

#endif
