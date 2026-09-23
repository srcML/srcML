// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TokenLookbackJavaScript.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Records the last non-skip token (e.g., not whitespace, comments, etc.)
 */

#ifndef TOKENLOOKBACKJAVASCRIPT_HPP
#define TOKENLOOKBACKJAVASCRIPT_HPP

#include <antlr/TokenStream.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>

class TokenLookbackJavaScript : public antlr::TokenStream {

public:

    TokenLookbackJavaScript(antlr::TokenStream& input) : input(input) {}

    virtual antlr::RefToken nextToken();

    static int lastTokenType();

    static int lastTokenTypeTwo();

private:
    antlr::TokenStream& input;
    static int lookaheadMinusOne;
    static int lookaheadMinusTwo;
};

#endif
