// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file issrcml.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <issrcml.hpp>

using namespace ::std::literals::string_view_literals;

// heuristically determine whether a buffer holds srcML (vs. source code)
bool issrcML(std::string_view buffer) {

    // short input must be source
    if (buffer.size() < "<unit"sv.size())
        return false;

    // trim leading whitespace
    buffer.remove_prefix(buffer.find_first_not_of(" \n\t"));

    // all XML constructs start with an angle bracket
    if (buffer[0] != '<')
        return false;

    // starts with an XML declaration
    if (buffer[1] == '?')
        return true;

    // look for start unit
    return buffer.substr(0, "<src:unit"sv.size()) == "<src:unit"sv ||
           buffer.substr(0, "<unit"sv.size()) == "<unit"sv;
}
