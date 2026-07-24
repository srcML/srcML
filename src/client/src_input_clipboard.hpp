// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_clipboard.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRC_INPUT_CLIPBOARD_HPP
#define SRC_INPUT_CLIPBOARD_HPP

#include <srcml_input_src.hpp>

// read the clipboard into the input source as in-memory input,
// determining whether the content is source code or srcML
void open_clipboard(srcml_input_src& input_source);

#endif
