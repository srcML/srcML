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
struct archive;

int archive_write_set_format_by_extension(struct archive*, const char* extension);
int archive_write_set_compression_by_extension(struct archive*, const char* extension);

bool is_archive(const std::string& input_file_extension);
bool is_compressed(const std::string& input_file_extension);

std::string language_to_std_extension(const char* extension);

#endif
