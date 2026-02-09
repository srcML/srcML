// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TokenLookbackJavaScript.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Records the last non-skip token (e.g., not whitespace, comments, etc.)
 */

#include <TokenLookbackJavaScript.hpp>

int TokenLookbackJavaScript::tokenType = 0;

/**
 * Save the current token if it is not a skip token
 */
antlr::RefToken TokenLookbackJavaScript::nextToken() {
    antlr::RefToken token = input.nextToken();

    if (!srcMLParser::skip_tokens_set.member(token->getType())) {
        tokenType = token->getType();
    }

    return token;
}

/**
 * Returns the type of the last non-skip token.
 */
int TokenLookbackJavaScript::lastTokenType() {
    return tokenType;
}
