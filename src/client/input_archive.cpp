// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file input_archive.hpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <srcml_pipe.hpp>
#include <input_archive.hpp>
#include <unarchive_srcml.hpp>

int input_archive(const srcml_input_src& input) {

    srcml_input_src uninput = input;

    if (!input.archives.empty() || !input.compressions.empty()) {

        srcml_pipe(uninput, unarchive_srcml);
    }

    return *uninput.fd;
}
