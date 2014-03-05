/**
 * @file parse_request.hpp
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
  parseRequest contains a collection of information and raw data to be
  queued up for libsrcml processing
*/

#ifndef PARSE_REQUEST_HPP
#define PARSE_REQUEST_HPP

#include <algorithm>
#include <archive.h>
#include <srcml.h>

#include <string>
#include <vector>

#include <boost/optional.hpp>

struct ParseRequest {
    ParseRequest() : buffer(0), srcml_arch(0), position(0) {}

    void swap(ParseRequest& other) {

        filename.swap(other.filename);
        directory.swap(other.directory);
        version.swap(other.version);
        buffer.swap(other.buffer);
        std::swap(srcml_arch, other.srcml_arch);
        lang.swap(other.lang);
        std::swap(position, other.position);
    }

    // empty ParseRequests indicate termination
    bool empty() const {
        return filename && buffer.empty() && lang.empty();
    }

    // Fields required by thread to process a unit
    boost::optional<std::string> filename;
    boost::optional<std::string> directory;
    boost::optional<std::string> version;
    std::vector<char> buffer;
    srcml_archive * srcml_arch;
    std::string lang;
    int position;
};

#endif
