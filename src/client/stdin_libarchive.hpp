// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file stdin_libarchive.hpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef STDIN_LIBARCHIVE_HPP
#define STDIN_LIBARCHIVE_HPP

#include <srcml_input_src.hpp>

// unarchive srcml from the current request
void open_stdin_libarchive(srcml_input_src& input_source);

#endif
