// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file DocstringPython.hpp
 *
 * @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Alters string literals that should be docstrings in the token stream
 */

#ifndef INCLUDED_DOCSTRINGPYTHON_HPP
#define INCLUDED_DOCSTRINGPYTHON_HPP

#include <ANTLRRuntime.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>
#include <deque>
#include <algorithm>

class DocstringPython : public antlr::TokenStream {

public:

    DocstringPython(antlr::TokenStream& input) : input(input) {}

    antlr::RefToken nextToken();

    void countWSNewlineTokens(antlr::RefToken token);

    void setBlockStartToken(int token);

    int getBlockStartToken() const;

private:
    antlr::TokenStream& input;
    std::deque<antlr::RefToken> buffer;

    bool isFunctionOrClass = false;
    bool isBlockStart = false;

    int numBrackets = 0;  // encompasses (), {}, and []
    int blockStartToken = -1;
    int lineNumber = 1;
};

#endif
