/**
 * @file pipe.hpp
 *
 * @copyright Copyright (C) 2017 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef PIPE_HPP
#define PIPE_HPP

#include <srcml_input_src.hpp>
#include <srcml_cli.hpp>

// has to be a typedef for older gcc compilers
typedef void (*pipe_process)(const srcml_request_t&, const srcml_input_t&, const srcml_output_dest&);

void input_pipe(srcml_input_src& input, pipe_process process, srcml_request_t srcml_request = srcml_request_t());

#endif
