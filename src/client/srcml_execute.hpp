// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_consume.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRCML_EXECUTE_HPP
#define SRCML_EXECUTE_HPP

#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>
#include <list>

// has to be a typedef for older gcc compilers
typedef void (*process_srcml)(const srcml_request_t& srcml_request, const srcml_input_t& input_sources, const srcml_output_dest& destination);

typedef std::list<process_srcml> processing_steps_t;

void srcml_execute(const srcml_request_t& srcml_request,
                   processing_steps_t& processing_steps,
                   const srcml_input_t& input_sources,
                   const srcml_output_dest& output);

#endif
