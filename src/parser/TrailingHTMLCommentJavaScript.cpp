// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TrailingHTMLCommentJavaScript.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Records the last non-skip token (e.g., not whitespace, comments, etc.)
 */

#include <TrailingHTMLCommentJavaScript.hpp>

// "-->" is only a comment start if it is the first token on a line
// for each line, lookahead until finding a non-whitespace token
// if the tokens are not "--" immediately followed by ">", it is not a comment
antlr::RefToken TrailingHTMLCommentJavaScript::nextToken() {
    // record the first token so the correct next token is returned
    if (buffer.empty()) {
        token = input.nextToken();
        buffer.push_front(token);
    }
    else {
        token = buffer.front();
    }

    // reset the whitespace check each time there is a new line
    if (token->getType() == srcMLParser::EOL) {
        foundNonWhitespace = false;
    }

    // check if the token is "--"
    if (
        !foundNonWhitespace &&
        token->getType() == srcMLParser::OPERATORS && 
        token->getText() == "--"
    ) {
        auto next = input.nextToken();
        buffer.push_back(next);
        // check if the immediate next token is >
        if (next->getType() == srcMLParser::TEMPOPE) {
            // if we find "-->" at the start of the line, it's a line comment
            token->setType(srcMLParser::TRAILING_HTML_COMMENT_START);
        }
    }
    // did not look ahead. token does not need to be on the buffer
    buffer.pop_front();


    // if the token was not "--", check if it is not whitespace
    if (!srcMLParser::skip_tokens_set.member(token->getType())) {
        foundNonWhitespace = true;
    }

    return token;
}
