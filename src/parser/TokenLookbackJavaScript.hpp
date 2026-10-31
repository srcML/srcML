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

#include <ANTLRRuntime.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>

class TokenLookbackJavaScript : public antlr::TokenStream {

public:

    TokenLookbackJavaScript(antlr::TokenStream& input) : input(input) {}

    virtual antlr::RefToken nextToken();

    int lastTokenType() const;

    int lastTokenTypeTwo() const;

private:
    antlr::TokenStream& input;

    // types of the last two non-skip tokens of this parse
    int lookaheadMinusOne = 0;
    int lookaheadMinusTwo = 0;
};

#endif
