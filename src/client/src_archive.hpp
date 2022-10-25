// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_archive.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRC_ARCHIVE_HPP
#define SRC_ARCHIVE_HPP

#include <string>
#include <string_view>
struct archive;

int archive_write_set_format_by_extension(struct archive*, std::string_view extension);
int archive_write_set_compression_by_extension(struct archive*, std::string_view extension);

bool is_archive(std::string_view input_file_extension);
bool is_compressed(std::string_view input_file_extension);

std::string_view language_to_std_extension(std::string_view extension);

#endif
