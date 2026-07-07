// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file NameDifferentiatorJavaScript.hpp
 *
 * @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Changes certain JavaScript/TypeScript keywords to names if their inclusion is unexpected.
 */

#ifndef INCLUDED_NAMEDIFFERENTIATORJAVASCRIPT_HPP
#define INCLUDED_NAMEDIFFERENTIATORJAVASCRIPT_HPP

#include <antlr/TokenStream.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>
#include <deque>
#include <algorithm>

class NameDifferentiatorJavaScript : public antlr::TokenStream {

public:

    NameDifferentiatorJavaScript(antlr::TokenStream& input) : input(input) {}

    antlr::RefToken nextToken();

    void lookAheadDifferentiator(antlr::RefToken token);

    bool isNameToken(antlr::RefToken token, antlr::RefToken nextToken) const;

    void checkBracketToken(antlr::RefToken token);

private:
    antlr::TokenStream& input;
    std::deque<antlr::RefToken> buffer;
    std::deque<std::string> bracketBuffer;

    antlr::RefToken prevToken = srcMLToken::factory();
    antlr::RefToken prevNonWhitespaceToken = srcMLToken::factory();

    int numBrackets = 0;  // encompasses (), {}, and []
};

#endif
