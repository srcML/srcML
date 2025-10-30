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

    void insertTerminateToken(int token);

private:
    antlr::TokenStream& input;
    std::deque<antlr::RefToken> buffer;
    antlr::RefToken lastToken = srcMLToken::factory();

    bool isEmptyLine = true;
    bool firstCharacter = true;
    bool wasNameNewlineToken = false;
    bool wasLambdaArrow = false;

    int parenthesesCount = 0;
    int lambdaBlockStartLine = -1;
};

#endif
