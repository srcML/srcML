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

// inserts TERMINATE tokens at EOL for JavaScript
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

        // record the line the lambda block starts on, if applicable
        if (wasLambdaArrow) {
            if (token->getType() == srcMLParser::LCURLY)
                lambdaBlockStartLine = token->getLine();

            wasLambdaArrow = false;
        }

        // found an arrow ("=>")
        if (token->getType() == srcMLParser::JS_ARROW)
            wasLambdaArrow = true;

        //
        // TERMINATE-checking logic that requires knowing the next token
        //
        if (
            token->getType() == srcMLParser::EOL
            && (
                lastToken->getType() == srcMLParser::NAME
                || srcMLParser::identifier_list_tokens_set.member(lastToken->getType())
            )
        ) {
            auto nextToken = input.nextToken();

            // for "NAME \n TOKEN", insert a TERMINATE if TOKEN is not an operator or colon
            if (
                (
                    lastToken->getType() == srcMLParser::NAME
                    || srcMLParser::identifier_list_tokens_set.member(lastToken->getType())
                )
                && !(
                    (
                        nextToken->getType() == srcMLParser::OPERATORS
                        && nextToken->getText() != "++"
                        && nextToken->getText() != "--"
                    )
                    || nextToken->getType() == srcMLParser::TEMPOPE
                    || nextToken->getType() == srcMLParser::TEMPOPS
                    || nextToken->getType() == srcMLParser::COLON
                )
            ) {
                insertTerminateToken(token->getLine());
            }

            buffer.emplace_back(token);
            buffer.emplace_back(nextToken);
            lastToken = nextToken;
            wasNameNewlineToken = true;
        }
        //
        // TERMINATE-checking logic that only requires prior token information
        //
        else if (
            (
                (
                    token->getType() == srcMLParser::EOL ||
                    token->getType() == srcMLParser::WS_EOL ||
                    token->getType() == srcMLParser::LINE_COMMENT_START ||
                    token->getType() == srcMLParser::HASHBANG_COMMENT_START
                )

                // not in parentheses
                && parenthesesCount == 0

                // not an empty line
                && !firstCharacter

                // not an existing TERMINATE
                && lastToken->getType() != srcMLParser::TERMINATE

                // do not place a TERMINATE after a comment; if applicable, it should go before
                && lastToken->getType() != srcMLParser::LINE_COMMENT_END
                && lastToken->getType() != srcMLParser::HASHBANG_COMMENT_END
                && lastToken->getType() != srcMLParser::BLOCK_COMMENT_END

                // do not place a TERMINATE right after a block begins
                && lastToken->getType() != srcMLParser::LCURLY

                // do not place a TERMINATE in the middle of a comma-separated structure
                && lastToken->getType() != srcMLParser::COMMA

                // not in the middle of an expression with a previous operator
                // a non-postfix operator at the end means the expression is not complete
                && (lastToken->getType() != srcMLParser::OPERATORS
                    || (
                        lastToken->getType() == srcMLParser::OPERATORS
                        && (
                            lastToken->getText() == "++"
                            || lastToken->getText() == "--"
                        )
                    )
                )
                && lastToken->getType() != srcMLParser::TEMPOPE
                && lastToken->getType() != srcMLParser::TEMPOPS
            )

            // special case: statement in a single-line lambda block ends before RCURLY
            || (lambdaBlockStartLine != -1 && token->getType() == srcMLParser::RCURLY && token->getLine() == lambdaBlockStartLine)

            // special case: always insert a TERMINATE after RCURLY; the parser will ignore them if needed
            || (token->getType() != srcMLParser::TERMINATE && token->getType() != srcMLParser::RPAREN && lastToken->getType() == srcMLParser::RCURLY)

            // at EOF with no previous EOL
            || (token->getType() == 1 /* EOF */ && lastToken->getType() != srcMLParser::EOL)
        ) {
            insertTerminateToken(token->getLine());
        }

        if (token->getType() == srcMLParser::EOL) {
            firstCharacter = true;
            isEmptyLine = true;
        } else if (token->getType() != srcMLParser::WS) {
            firstCharacter = false;
            isEmptyLine = false;
        }

        // record current token for next iteration, unless it was already done previously
        if (!wasNameNewlineToken)
            lastToken = token;

        // insert skipped whitespace
        while (!wsBuffer.empty()) {
            buffer.emplace_back(wsBuffer.front());
            wsBuffer.pop_front();
        }

        // insert read token, unless it was already done previously
        if (!wasNameNewlineToken)
            buffer.emplace_back(token);
        else
            wasNameNewlineToken = false;
    }

    // next token
    auto token = buffer.front();
    buffer.pop_front();
    return token;
}

/**
 * Creates a new TERMINATE token and adds it to the buffer.
 *
 * The `line` parameter is the TERMINATE token's intended line.
 */
void NewlineTerminateJavaScript::insertTerminateToken(int line) {
    // create new terminate token
    auto terminateToken = srcMLToken::factory();
    terminateToken->setType(srcMLParser::TERMINATE);
    terminateToken->setColumn(1);
    terminateToken->setLine(line);

    // reset the parentheses count
    parenthesesCount = 0;

    // reset the lambda block starting line number
    lambdaBlockStartLine = -1;

    // insert terminal token
    buffer.emplace_back(terminateToken);
}
