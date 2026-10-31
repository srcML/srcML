// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file NewlineTerminatePython.hpp
 *
 * @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Injects INDENT and DEDENT tokens to the token stream
 */

#ifndef INCLUDED_NEWLINETERMINATEPYTHON_HPP
#define INCLUDED_NEWLINETERMINATEPYTHON_HPP

#include <ANTLRRuntime.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>
#include <deque>

class NewlineTerminatePython : public antlr::TokenStream {

public:

    NewlineTerminatePython(antlr::TokenStream& input) : input(input) {}

    antlr::RefToken nextToken();

private:
    antlr::TokenStream& input;
    std::deque<antlr::RefToken> buffer;
    antlr::RefToken lastToken = srcMLToken::factory();
    bool isEmptyLine = true;
    int parenthesesCount = 0;
    bool firstCharacter = true;
    antlr::RefToken lastNonWhitespaceToken = srcMLToken::factory();
};

#endif
