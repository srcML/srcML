// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file input_curl.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef input_curl_HPP
#define input_curl_HPP

#include <srcml_input_src.hpp>
#include <string_view>

bool curl_supported(std::string_view input_protocol);

// adjust input for libcurl and libarchive decompressions
int input_curl(srcml_input_src& input);

bool getCurlErrors();

#endif
