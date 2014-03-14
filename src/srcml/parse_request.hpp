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
    ParseRequest() : buffer(0), srcml_arch(0), position(0), status(0) {}

    void swap(ParseRequest& other) {

        std::swap(filename, other.filename);
        std::swap(directory, other.directory);
        std::swap(version, other.version);
        std::swap(buffer, other.buffer);
        std::swap(srcml_arch, other.srcml_arch);
        std::swap(language, other.language);
        std::swap(position, other.position);
        std::swap(disk_filename, other.disk_filename);
        std::swap(status, other.status);
    }

    // empty ParseRequests indicate termination
    bool empty() const {
        return filename && buffer.empty() && language.empty();
    }

    // Fields required by thread to process a unit
    boost::optional<std::string> filename;
    boost::optional<std::string> directory;
    boost::optional<std::string> version;
    std::vector<char> buffer;
    srcml_archive * srcml_arch;
    std::string language;
    int position;
    std::string disk_filename;
    int status;
};

#endif
