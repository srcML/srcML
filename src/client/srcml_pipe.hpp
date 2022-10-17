// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_pipe.hpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRCML_PIPE_HPP
#define SRCML_PIPE_HPP

#include <srcml_input_src.hpp>
#include <srcml_cli.hpp>

// has to be a typedef for older gcc compilers
typedef void (*srcml_pipe_process)(const srcml_request_t&, const srcml_input_t&, const srcml_output_dest&);

void srcml_pipe(srcml_input_src& input, srcml_pipe_process process, srcml_request_t srcml_request = srcml_request_t());

void srcml_pipe_clean();

#endif
