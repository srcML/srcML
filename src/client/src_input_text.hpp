// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_text.hpp
 *
 * @copyright Copyright (C) 2015-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRC_INPUT_TEXT_HPP
#define SRC_INPUT_TEXT_HPP

#include <srcml.h>
#include <srcml_cli.hpp>
#include <string>
#include <ParseQueue.hpp>

int src_input_text(ParseQueue& queue,
                    srcml_archive* srcml_arch,
                    const srcml_request_t& srcml_request,
                    const srcml_input_src& input);

#endif
