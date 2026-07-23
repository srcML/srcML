// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file issrcml.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef ISSRCML_HPP
#define ISSRCML_HPP

#include <string_view>

// heuristically determine whether a buffer holds srcML (vs. source code)
bool issrcML(std::string_view buffer);

#endif
