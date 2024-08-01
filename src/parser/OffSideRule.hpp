// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file OffSideRule.hpp
 *
 * @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Injects INDENT and DEDENT tokens to the token stream
 */

#ifndef INCLUDED_OFFSIDERULE_HPP
#define INCLUDED_OFFSIDERULE_HPP

#include <antlr/TokenStream.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>
#include <deque>

class OffSideRule : public antlr::TokenStream {

public:

    OffSideRule(antlr::TokenStream& input) : input(input) {}

    antlr::RefToken nextToken();

private:
    antlr::TokenStream& input;
    std::deque<antlr::RefToken> buffer;

    int prevColStart = 1;
    int currentColStart = 1;
    int numIndents = 0;

    bool skippedColSet = false;
    bool debugInfo = false;
};

#endif
