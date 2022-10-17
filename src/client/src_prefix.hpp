// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_prefix.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRC_PREFIX_HPP
#define SRC_PREFIX_HPP

#include <string>
#include <tuple>

std::string src_prefix_add_uri(const std::string& protocol, const std::string& resource);

std::string src_prefix_add_uri(const std::string& input_file);

std::tuple<std::string, std::string> src_prefix_split_uri(const std::string& input_file);

std::string src_prefix_protocol(const std::string& input_file);

std::string src_prefix_resource(const std::string& input_file);

#endif
