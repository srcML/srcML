// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file create_srcml.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef CREATE_SRCML_HPP
#define CREATE_SRCML_HPP

#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>
#include <ParseQueue.hpp>

// create srcml from the current request
void create_srcml(const srcml_request_t& srcml_request,
                  const srcml_input_t& input_sources,
                  const srcml_output_dest& destination);

// call appropriate handler based on input source
int srcml_handler_dispatch(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          const srcml_input_src& input,
                          const srcml_output_dest& destination);

#endif

