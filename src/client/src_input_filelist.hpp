// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_filelist.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRC_INPUT_FILELIST_HPP
#define SRC_INPUT_FILELIST_HPP

#include <ParseQueue.hpp>
#include <srcml_cli.hpp>
#include <srcml.h>
#include <string>
#include <optional>
#include <srcml_input_src.hpp>

int src_input_filelist(ParseQueue& queue,
                        srcml_archive* srcml_arch,
                        const srcml_request_t& srcml_request,
                        std::string_view input_filename,
                        const srcml_output_dest& destination);

#endif
