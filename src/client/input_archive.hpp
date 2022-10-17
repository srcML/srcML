// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file input_archive.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef INPUT_ARCHIVE_HPP
#define INPUT_ARCHIVE_HPP

#include <srcml_input_src.hpp>

// adjust input for libcurl and libarchive decompressions
int input_archive(const srcml_input_src& input);

#endif
