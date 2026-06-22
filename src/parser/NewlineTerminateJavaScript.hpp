// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file NewlineTerminateJavaScript.hpp
 *
 * @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Injects artificial TERMINATE tokens to the token stream
 */

#ifndef INCLUDED_NEWLINETERMINATEJAVASCRIPT_HPP
#define INCLUDED_NEWLINETERMINATEJAVASCRIPT_HPP

#include <antlr/TokenStream.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>
#include <deque>

class NewlineTerminateJavaScript : public antlr::TokenStream {

public:

    NewlineTerminateJavaScript(antlr::TokenStream& input) : input(input) {}

    antlr::RefToken nextToken();

    bool isTerminateCase(antlr::RefToken token, antlr::RefToken nextToken, bool containsEOL);

    void insertTerminateToken(int token);

private:
    antlr::TokenStream& input;
    std::deque<antlr::RefToken> buffer;
    std::deque<antlr::RefToken> tempSkipBuffer;
    std::deque<std::string> bracketTokenTracker;  // tracks '(', '[', and '{'
    antlr::RefToken currentToken = srcMLToken::factory();
    antlr::RefToken prevNonWhitespaceToken = srcMLToken::factory();

    bool firstToken = true;
    bool wasPostfixName = false;
    bool insertTerminate = false;
};

#endif
