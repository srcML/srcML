/**
 * @file src_prefix.hpp
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

#ifndef SRC_PREFIX_HPP
#define SRC_PREFIX_HPP

#include <string>

std::string src_prefix_add_uri(const std::string& protocol, const std::string& resource);

std::string src_prefix_add_uri(const std::string& input_file);

void src_prefix_split_uri(const std::string& input_file, std::string& protocol, std::string& resource);

std::string src_prefix_protocol(const std::string& input_file);

std::string src_prefix_resource(const std::string& input_file);

#endif
