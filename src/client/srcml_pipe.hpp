/**
 * @file srcml_pipe.hpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef SRCML_PIPE_HPP
#define SRCML_PIPE_HPP

#include <srcml_input_src.hpp>
#include <srcml_cli.hpp>

// has to be a typedef for older gcc compilers
typedef void (*srcml_pipe_process)(const srcml_request_t&, const srcml_input_t&, const srcml_output_dest&);

void srcml_pipe(srcml_input_src& input, srcml_pipe_process process, srcml_request_t srcml_request = srcml_request_t());

void srcml_pipe_clean();

#endif
