// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_write.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRCML_WRITE_HPP
#define SRCML_WRITE_HPP

#include <memory>
#include <srcml_input_src.hpp>

struct ParseRequest;
class TraceLog;

void srcml_write_request(std::shared_ptr<ParseRequest> prequest, TraceLog&, const srcml_output_dest& destination);

#endif
