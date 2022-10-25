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
#include <string_view>
#include <tuple>

std::string src_prefix_add_uri(std::string_view protocol, std::string_view resource);

std::string src_prefix_add_uri(std::string_view input_file);

std::tuple<std::string, std::string> src_prefix_split_uri(std::string_view input_file);

std::string_view src_prefix_protocol(std::string_view input_file);

std::string_view src_prefix_resource(std::string_view input_file);

#endif
