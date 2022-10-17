// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file unit_utilities.hpp
 *
 * @copyright Copyright (C) 2018-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef INCLUDED_UNIT_UTILITIES_HPP
#define INCLUDED_UNIT_UTILITIES_HPP

#include <srcml_types.hpp>
#include <libxml/parser.h>

// Update unit attributes with xml parsed attributes
void unit_update_attributes(srcml_unit* unit, int num_attributes, const xmlChar** attributes);

// Extract source code from srcml
std::string extract_src(const std::string& srcml, std::optional<int> revision = std::nullopt);
std::string extract_revision(const char* srcml, int size, int revision, bool text_only = false);
std::string attribute_revision(const std::string& attribute, int revision);

#endif
