// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_display_info.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRCML_DISPLAY_METADATA_HPP
#define SRCML_DISPLAY_METADATA_HPP

#include <string>
#include <vector>
#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>

void srcml_display_metadata(const srcml_request_t& srcml_request, const srcml_input_t& src_input, const srcml_output_dest&);

#endif
