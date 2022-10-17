// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_libarchive.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * Source input from local files, stdin, and source archives for srcml parsing queue
 */

#ifndef SRC_INPUT_LIBARCHIVE_HPP
#define SRC_INPUT_LIBARCHIVE_HPP

#include <srcml.h>
#include <srcml_cli.hpp>
#include <string>
#include <ParseQueue.hpp>
#include <srcml_input_src.hpp>
#include <src_archive.hpp>

archive* libarchive_input_file(const srcml_input_src& input_file);

int src_input_libarchive(ParseQueue& queue,
                          srcml_archive* srcml_arch,
                          const srcml_request_t& srcml_request,
                          const srcml_input_src& input);

#endif
