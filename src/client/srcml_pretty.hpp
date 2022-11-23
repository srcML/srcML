// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_pretty.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRCML_PRETTY_HPP
#define SRCML_PRETTY_HPP

#include <string>
#include <vector>
#include <srcml.h>
#include <optional>
#include <srcml_cli.hpp>

struct pretty_template_t {
    std::optional<std::string> header;
    std::vector<std::string> header_args;
    std::optional<std::string> body;
    std::vector<std::string> body_args;
    std::optional<std::string> footer;
    std::vector<std::string> footer_args;
    std::optional<size_t> error_location;
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

int srcml_pretty(srcml_archive* srcml_arch, std::string_view pretty_input, const srcml_request_t& srcml_request);

#endif
