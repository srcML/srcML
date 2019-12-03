/**
 * @file src_prefix.hpp
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
