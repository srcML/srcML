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

#include <NewlineTerminateJavaScript.hpp>

// Inserts TERMINATE tokens at EOL for JavaScript
antlr::RefToken NewlineTerminateJavaScript::nextToken() {

    // place all input tokens in the buffer so we can insert a TERMINATE
    if (buffer.empty()) {
        auto token = input.nextToken();

        // buffer any non-EOL whitespace or line continuation backslashes
        // since these must be placed after the inserted terminate
        std::deque<antlr::RefToken> wsBuffer;
        while (token->getType() == srcMLParser::WS || token->getType() == srcMLParser::EOL_BACKSLASH) {
            wsBuffer.emplace_back(token);
            token = input.nextToken();
        }

        // update the open parentheses count (includes parentheses and square brackets)
        if (token->getType() == srcMLParser::LPAREN || token->getType() == srcMLParser::LBRACKET)
            ++parenthesesCount;
        else if (parenthesesCount > 0 && (token->getType() == srcMLParser::RPAREN || token->getType() == srcMLParser::RBRACKET))
            --parenthesesCount;

        // For a newline, insert a TERMINATE in certain cases
        if (((token->getType() == srcMLParser::EOL ||
              token->getType() == srcMLParser::WS_EOL ||
              token->getType() == srcMLParser::HASHTAG_COMMENT_START ||
              token->getType() == srcMLParser::HASHBANG_COMMENT_START) &&

            // not in parentheses
            parenthesesCount == 0 &&

            // not an empty line
            !firstCharacter &&

            // not an existing TERMINATE
            lastToken->getType() != srcMLParser::TERMINATE &&

            // do not place a TERMINATE right after a block begins
            lastToken->getType() != srcMLParser::LCURLY &&

            // not in the middle of an expression with a previous operator
            // A non-postfix operator at the end means the expression is not complete
            // JavaScript has two postfix operators ("++" and "--")
            (lastNonWhitespaceToken->getType() != srcMLParser::OPERATORS || (
                lastNonWhitespaceToken->getType() == srcMLParser::OPERATORS &&
                (
                    lastNonWhitespaceToken->getText() == "++" ||
                    lastNonWhitespaceToken->getText() == "--"
                )
            )) &&
            lastNonWhitespaceToken->getType() != srcMLParser::TEMPOPE &&
            lastNonWhitespaceToken->getType() != srcMLParser::TEMPOPS) ||

            // At EOF with no previous EOL
            (token->getType() == 1 /* EOF */ && lastToken->getType() != srcMLParser::EOL)) {

            // create new terminate token
            auto terminateToken = srcMLToken::factory();
            terminateToken->setType(srcMLParser::TERMINATE);
            terminateToken->setColumn(1);
            terminateToken->setLine(token->getLine());

            // reset the parentheses count
            parenthesesCount = 0;

            // insert terminal token
            buffer.emplace_back(terminateToken);
        }

        if (token->getType() == srcMLParser::EOL) {
            firstCharacter = true;
            isEmptyLine = true;
        } else if (token->getType() != srcMLParser::WS) {
            firstCharacter = false;
            isEmptyLine = false;
        }

        lastToken = token;

        // insert skipped whitespace
        while (!wsBuffer.empty()) {
            buffer.emplace_back(wsBuffer.front());
            wsBuffer.pop_front();
        }

        // insert read token
        buffer.emplace_back(token);
    }

    // next token
    auto token = buffer.front();
    buffer.pop_front();
    return token;
}
