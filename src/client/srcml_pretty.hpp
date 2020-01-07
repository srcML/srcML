/**
 * @file srcml_pretty.hpp
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
 */

#ifndef SRCML_PRETTY_HPP
#define SRCML_PRETTY_HPP

#include <string>
#include <vector>
#include <srcml.h>
#include <boost/optional.hpp>
#include "srcml_cli.hpp"

struct pretty_template_t {
    boost::optional<std::string> header;
    std::vector<std::string> header_args;
    boost::optional<std::string> body;
    std::vector<std::string> body_args;
    boost::optional<std::string> footer;
    std::vector<std::string> footer_args;
    boost::optional<size_t> error_location;
};

/*
PRETTY Args
- %C: total number of units (only for footer)
- %F: file attribute on the archive
- %f: file name attribute on the unit
- %h: hash attribute on the unit
- %i: index of the unit within the archive
- %l: unit language
- %N: print xmlns namespace %N = full namespace, %N:u = namespace uri, %N:p = namespace prefix
- %S: source encoding attribute on the archive
- %s: source encoding attribute on the unit
- %t: timestamp on unit
- %U: directory attribute on the archive
- %V: version attribute on the archive
- %v: version attribute on the unit
- %X: XML encoding on the archive
*/

const std::string valid_header_args = "FNSUVX";
const std::string valid_body_args = "FfhilNSstUVvX";
const std::string valid_footer_args = "CFNSUVX";

int srcml_pretty(srcml_archive* srcml_arch, const std::string& pretty_input, const srcml_request_t& srcml_request);

#endif
