// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_output_libarchive.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRC_OUTPUT_LIBARCHIVE_HPP
#define SRC_OUTPUT_LIBARCHIVE_HPP

#include <archive.h>
#include <srcml.h>

void src_output_libarchive(srcml_archive* srcml_arch, archive* ar);

#endif
