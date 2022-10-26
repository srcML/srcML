// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_prefix.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <src_prefix.hpp>

#include <iostream>

std::string_view PROTOCOL_SEPARATOR = "://";

std::string src_prefix_add_uri(std::string_view protocol, std::string_view resource) {

    std::string s(protocol);
    s += PROTOCOL_SEPARATOR;
    s += resource;

    return s;
}

std::string src_prefix_add_uri(std::string_view input_file) {

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

    return std::string(input_file);
}

std::tuple<std::string, std::string> src_prefix_split_uri(std::string_view input_file){
    // Extract function split_uri(input_file, protocol, resource)
    size_t prefixPos = input_file.find(PROTOCOL_SEPARATOR);

    if (prefixPos == std::string::npos) {
        return std::tuple<std::string, std::string>("", input_file);
    }

    return std::tuple<std::string, std::string>(input_file.substr(0, prefixPos), input_file.substr(prefixPos + PROTOCOL_SEPARATOR.size()));
}

std::string_view src_prefix_resource(std::string_view input_file) {

    size_t prefixPos = input_file.find(PROTOCOL_SEPARATOR);

    if (prefixPos == std::string::npos)
        return input_file;

    return input_file.substr(prefixPos + PROTOCOL_SEPARATOR.size());
}

std::string_view src_prefix_protocol(std::string_view input_file) {

    size_t prefixPos = input_file.find(PROTOCOL_SEPARATOR);

    if (prefixPos == std::string::npos)
        return "";

    return input_file.substr(0, prefixPos);
}
