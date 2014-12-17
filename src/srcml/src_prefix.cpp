/**
 * @file src_prefix.cpp
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

#include <src_prefix.hpp>

#include <string.h>
#include <iostream>

const char* PROTOCOL_SEPARATOR = "://";

std::string src_prefix_add_uri(const std::string& protocol, const std::string& resource) {
    return protocol + PROTOCOL_SEPARATOR + resource;
}

std::string src_prefix_add_uri(const std::string& input_file) {

    // Only add a prefix if the input doesn't have one already (IE. http://)
    size_t prefixPos = input_file.find(PROTOCOL_SEPARATOR);
    if (prefixPos == std::string::npos) {

        if (input_file.compare("/dev/stdin") == 0 || input_file.compare("-") == 0)
            return src_prefix_add_uri("stdin", "-");

        // Check for local file
        if (input_file.find("http:") == std::string::npos || input_file.find("https:") == std::string::npos) {
            return src_prefix_add_uri("file", input_file);
        }
    }

    return input_file;
}

void src_prefix_split_uri(const std::string& input_file, std::string& protocol, std::string& resource){
    // Extract function split_uri(input_file, protocol, resource)
    size_t prefixPos = input_file.find(PROTOCOL_SEPARATOR);

    if (prefixPos == std::string::npos) {
        protocol = "";
        resource = input_file;
        return;
    }
    protocol = input_file.substr(0, prefixPos);
    resource = input_file.substr(prefixPos + strlen(PROTOCOL_SEPARATOR));
}

std::string src_prefix_resource(const std::string& input_file) {

    size_t prefixPos = input_file.find(PROTOCOL_SEPARATOR);

    if (prefixPos == std::string::npos)
        return input_file;

    return input_file.substr(prefixPos + strlen(PROTOCOL_SEPARATOR));
}

std::string src_prefix_protocol(const std::string& input_file) {

    size_t prefixPos = input_file.find(PROTOCOL_SEPARATOR);

    if (prefixPos == std::string::npos)
        return "";

    return input_file.substr(0, prefixPos);
}
