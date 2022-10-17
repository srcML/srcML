// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file create_src.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef CREATE_SRC_HPP
#define CREATE_SRC_HPP

#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>

// create srcml from the current request
void create_src(const srcml_request_t& srcml_request,
                const srcml_input_t& input_sources,
                const srcml_output_dest& output);

#endif

