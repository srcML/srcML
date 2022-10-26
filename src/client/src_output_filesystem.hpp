// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_output_filesystem.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRC_OUTPUT_FILESYSTEM_HPP
#define SRC_OUTPUT_FILESYSTEM_HPP

#include <srcml.h>
#include <string>
#include <TraceLog.hpp>

void src_output_filesystem(srcml_archive* srcml_arch, std::string_view output_dir, TraceLog& log);

#endif
