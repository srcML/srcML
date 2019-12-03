/**
 * @file srcml_input_srcml.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
 *
 */

#ifndef SRCML_INPUT_SRCML_HPP
#define SRCML_INPUT_SRCML_HPP

#include <srcml_utilities.hpp>
#include <memory>
#include <boost/optional.hpp>
#include <srcml_cli.hpp>

class ParseQueue;
class srcml_input_src;
struct srcml_archive;

int srcml_input_srcml(ParseQueue& queue,
                       srcml_archive* srcml_output_archive,
                       const srcml_request_t& srcml_request,
                       const srcml_input_src& srcml_input_source,
                       const boost::optional<size_t> & revision);

#endif
