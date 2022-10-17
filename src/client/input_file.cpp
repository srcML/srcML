// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file input_file.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <srcml_pipe.hpp>
#include <input_file.hpp>
#include <decompress_srcml.hpp>

void input_file(srcml_input_src& input) {

    if (!input.compressions.empty()) {

        srcml_pipe(input, decompress_srcml);
    }
}
