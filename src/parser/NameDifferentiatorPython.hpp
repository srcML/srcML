// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file NameDifferentiatorPython.hpp
 *
 * @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Changes certain statement keywords to names in Python.
 * This includes `case`, `match`, and `type`.
 */

#ifndef INCLUDED_NAMEDIFFERENTIATORPYTHON_HPP
#define INCLUDED_NAMEDIFFERENTIATORPYTHON_HPP

#include <ANTLRRuntime.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>
#include <deque>
#include <algorithm>

class NameDifferentiatorPython : public antlr::TokenStream {

public:

    NameDifferentiatorPython(antlr::TokenStream& input) : input(input) {}

    antlr::RefToken nextToken();

    void lookAheadTwoDifferentiator(antlr::RefToken token);

    void variableLookAheadDifferentiator(antlr::RefToken token);

    void checkBracketToken(antlr::RefToken token);

    void setBlockStartToken(int token);

    int getBlockStartToken() const;

private:
    antlr::TokenStream& input;
    std::deque<antlr::RefToken> buffer;

    bool isName = true;

    int numBrackets = 0;  // encompasses (), {}, and []
    int numCurrentBrackets = 0;  // encompasses (), {}, and []
    int blockStartToken = -1;
};

#endif
