// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file unarchive_srcml.hpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef UNARCHIVE_SRCML_HPP
#define UNARCHIVE_SRCML_HPP

#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>

// unarchive srcml from the current request
void unarchive_srcml(const srcml_request_t& srcml_request,
                     const srcml_input_t& input_sources,
                     const srcml_output_dest& output);

#endif
