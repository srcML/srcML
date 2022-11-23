// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_filesystem.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRC_INPUT_FILESYSTEM_HPP
#define SRC_INPUT_FILESYSTEM_HPP

#include <srcml.h>
#include <srcml_cli.hpp>
#include <ParseQueue.hpp>

int src_input_filesystem(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          std::string_view input_filename);

#endif
