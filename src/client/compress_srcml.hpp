// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file compress_srcml.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef COMPRESS_SRCML_HPP
#define COMPRESS_SRCML_HPP

#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>

#if ARCHIVE_VERSION_NUMBER >= 3002000
// compress srcml from the current request
void compress_srcml(const srcml_request_t& srcml_request,
                const srcml_input_t& input_sources,
                const srcml_output_dest& output);
#endif

#endif
