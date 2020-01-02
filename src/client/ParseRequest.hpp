/**
 * @file ParseRequest.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef PARSE_REQUEST_HPP
#define PARSE_REQUEST_HPP

#include <srcml.h>
#include <string>
#include <vector>
#include <srcml_utilities.hpp>
#include <memory>
#include <boost/optional.hpp>

struct ParseRequest {
    ParseRequest(int size = 0) : buffer(size) {}

    // Fields required by thread to process a unit
    std::string language;
    boost::optional<std::string> filename;
    boost::optional<std::string> url;
    boost::optional<std::string> version;
    std::vector<char> buffer;
    srcml_archive* srcml_arch = nullptr;
    std::unique_ptr<srcml_unit> unit;
    boost::optional<std::string> disk_filename;
    boost::optional<std::string> disk_dir;
    std::string parsertest_filename;
    int position = 0;
    int status = 0;
    double runtime = 0;
    boost::optional<std::string> time_stamp;
    boost::optional<std::string> errormsg;
    bool needsparsing = true;
    srcml_transform_result* results = nullptr;
    std::shared_ptr<srcml_archive> input_archive;
};

#endif
