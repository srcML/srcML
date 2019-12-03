/**
 * @file src_archive.hpp
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

#ifndef SRC_ARCHIVE_HPP
#define SRC_ARCHIVE_HPP

#include <string>
struct archive;

int archive_write_set_format_by_extension(struct archive*, const char* extension);
int archive_write_set_compression_by_extension(struct archive*, const char* extension);

bool is_archive(const std::string& input_file_extension);
bool is_compressed(const std::string& input_file_extension);

std::string language_to_std_extension(const char* extension);

#endif
