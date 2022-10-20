// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_input_srcml.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 */

#ifndef SRCML_INPUT_SRCML_HPP
#define SRCML_INPUT_SRCML_HPP

#include <srcml_utilities.hpp>
#include <optional>
#include <srcml_cli.hpp>

class ParseQueue;
class srcml_input_src;
struct srcml_archive;

int srcml_input_srcml(ParseQueue& queue,
                       srcml_archive* srcml_output_archive,
                       const srcml_request_t& srcml_request,
                       const srcml_input_src& srcml_input_source,
                       const std::optional<size_t> & revision);

#endif
