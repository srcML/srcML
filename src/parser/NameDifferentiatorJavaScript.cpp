// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file NameDifferentiatorJavaScript.cpp
 *
 * @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Changes certain JavaScript/TypeScript keywords to names if their inclusion is unexpected.
 */

#include <NameDifferentiatorJavaScript.hpp>

// Converts certain statement, operator, specifier, etc. tokens to names
antlr::RefToken NameDifferentiatorJavaScript::nextToken() {
    // Ensure that the bracket buffer is never empty by adding a dummy entry
    if (bracketBuffer.empty()) {
        bracketBuffer.emplace_front("*");
    }

    // Place all input tokens in a buffer to help check for keywords that should be names
    if (buffer.empty()) {
        auto token = input.nextToken();
        checkBracketToken(token);  // Detect if currently in/out of `()`, `{}`, or `[]`

        // Check if the current JavaScript/TypeScript keyword is really a keyword, or if it is used as a name
        if (srcMLParser::name_differentiator_js_token_set.member(token->getType()))
            lookAheadDifferentiator(token);
        // Manually set the previous token as the current token
        else
            prevToken = token;

        // Insert read token
        buffer.emplace_front(token);
    }

    // Remove the front token from the buffer and return it
    auto token = buffer.front();

    if (!srcMLParser::skip_tokens_set.member(token->getType()))
        prevNonWhitespaceToken = token;

    buffer.pop_front();
    return token;
}

/**
 * Looks ahead at least 1 token to determine if `token` should be a name or not.
 *
 * If `token` is followed by whitespace, then it will look ahead until it finds
 * the next non-whitespace token. If the new `nextToken` needs to be processed,
 * then this method will handle that.
 */
void NameDifferentiatorJavaScript::lookAheadDifferentiator(antlr::RefToken token) {
    auto nextToken = input.nextToken();
    checkBracketToken(nextToken);  // Detect if currently in/out of `()`, `{}`, or `[]`
    buffer.emplace_back(nextToken);

    /*
        CASE 1: `token` should be the name of a variable in a declaration (if it is not already)
                and `token` should be the name of a TypeScript type in an argument list
    */
    if (
        (
            srcMLParser::decl_start_js_token_set.member(prevNonWhitespaceToken->getType())
            && token->getType() != srcMLParser::JS_GET
            && token->getType() != srcMLParser::JS_SET
            && token->getType() != srcMLParser::NAME
            && token->getType() != srcMLParser::TS_PRIVATE
            && token->getType() != srcMLParser::TS_PROTECTED
            && token->getType() != srcMLParser::TS_PUBLIC
            && token->getType() != srcMLParser::TS_READONLY
        )
        || (
            bracketBuffer.front() == "*"
            && (
                prevNonWhitespaceToken->getType() == srcMLParser::TEMPOPS
                || prevNonWhitespaceToken->getType() == srcMLParser::COMMA
            )
            && srcMLParser::name_differentiator_js_token_set.member(token->getType())
            && (
                nextToken->getType() == srcMLParser::TEMPOPE
                || nextToken->getType() == srcMLParser::COMMA
            )
        )
    ) {
        token->setType(srcMLParser::NAME);
        prevToken = nextToken;

        if (!srcMLParser::skip_tokens_set.member(token->getType()))
            prevNonWhitespaceToken = token;

        return;
    }

    /*
        CASE 2: Do not change `token` if it is followed by `{` or another NAME
    */
    if (nextToken->getType() == srcMLParser::LCURLY || nextToken->getType() == srcMLParser::NAME) {
        prevToken = nextToken;

        if (!srcMLParser::skip_tokens_set.member(token->getType()))
            prevNonWhitespaceToken = token;

        return;
    }

    /*
        CASE 3: `token` is followed by whitespace, comments, EOL, etc.
    */
    if (srcMLParser::whitespace_token_set.member(nextToken->getType()) || nextToken->getType() == srcMLParser::EOL) {
        auto newPrevToken = prevToken;

        // Find the next non-whitespace token
        while (srcMLParser::whitespace_token_set.member(nextToken->getType()) || nextToken->getType() == srcMLParser::EOL) {
            newPrevToken = nextToken;
            nextToken = input.nextToken();
            buffer.emplace_back(nextToken);
        }

        if (isNameToken(token, nextToken))
            token->setType(srcMLParser::NAME);

        // Update here to ensure the bracket count is correct after invoking name-checking logic
        checkBracketToken(nextToken);  // Detect if currently in/out of `()`, `{}`, or `[]`

        prevToken = newPrevToken;

        if (!srcMLParser::skip_tokens_set.member(token->getType()))
            prevNonWhitespaceToken = token;

        // The new `nextToken` may need to be processed (e.g., "as default" in name lists)
        if (srcMLParser::name_differentiator_js_token_set.member(nextToken->getType()))
            lookAheadDifferentiator(nextToken);

        return;
    }

    /*
        CASE 4: `token` is a keyword that is used in a list, so it should be a NAME instead
    */
    if (
        srcMLParser::name_differentiator_subset_js_token_set.member(token->getType())
        && (
            nextToken->getType() == srcMLParser::COMMA
            || nextToken->getType() == srcMLParser::OPERATORS
            || nextToken->getType() == srcMLParser::RPAREN
            || nextToken->getType() == srcMLParser::RCURLY
            || nextToken->getType() == srcMLParser::RBRACKET
        )
    ) {
        token->setType(srcMLParser::NAME);
        prevToken = nextToken;

        if (!srcMLParser::skip_tokens_set.member(token->getType()))
            prevNonWhitespaceToken = token;

        return;
    }

    /*
        CASE 5: Change `token` to a NAME if one of the following conditions is met
    */
    if (
        isNameToken(token, nextToken)
        || (
            nextToken->getType() == srcMLParser::LPAREN
            && (
                token->getType() != srcMLParser::JS_CATCH
                && token->getType() != srcMLParser::JS_CONSTRUCTOR
                && token->getType() != srcMLParser::IF
                && token->getType() != srcMLParser::FOR
                && token->getType() != srcMLParser::JS_FUNCTION
                && token->getType() != srcMLParser::JS_GET
                && token->getType() != srcMLParser::JS_SET
                && token->getType() != srcMLParser::JS_WITH
                && token->getType() != srcMLParser::NEW
                && token->getType() != srcMLParser::WHILE
            )
        )
    ) {
        token->setType(srcMLParser::NAME);
        prevToken = nextToken;

        if (!srcMLParser::skip_tokens_set.member(token->getType()))
            prevNonWhitespaceToken = token;

        return;
    }
}

/**
 * Returns true if `token` should be changed to a NAME token, and false otherwise.
 */
bool NameDifferentiatorJavaScript::isNameToken(antlr::RefToken token, antlr::RefToken nextToken) const {
    return (
        (token->getType() != srcMLParser::JS_DEFAULT && nextToken->getType() == srcMLParser::COLON)
        || (token->getType() == srcMLParser::JS_DEFAULT
            && (
                nextToken->getType() == srcMLParser::JS_AS
                || nextToken->getType() == srcMLParser::COMMA
                || nextToken->getType() == srcMLParser::RPAREN
                || nextToken->getType() == srcMLParser::RCURLY
                || nextToken->getType() == srcMLParser::RBRACKET
            )
        )
        || (
            token->getType() == srcMLParser::TS_TYPE
            && (
                nextToken->getType() == srcMLParser::OPERATORS
                || nextToken->getType() == srcMLParser::COLON
                || nextToken->getType() == srcMLParser::EQUAL
                || nextToken->getType() == srcMLParser::PERIOD
                || nextToken->getType() == srcMLParser::QMARK
                || nextToken->getType() == srcMLParser::JS_ARROW
                || nextToken->getType() == srcMLParser::TERMINATE
                || nextToken->getType() == srcMLParser::EOF_
                || (
                    (bracketBuffer.front() == "(" || bracketBuffer.front() == "[")
                    && (
                        nextToken->getType() == srcMLParser::COMMA
                        || nextToken->getType() == srcMLParser::RPAREN
                        || nextToken->getType() == srcMLParser::RBRACKET
                    )
                )
            )
        )
        || (
            srcMLParser::name_differentiator_subset_js_token_set.member(token->getType())
            && (
                (
                    prevNonWhitespaceToken->getLine() == token->getLine()
                    && prevNonWhitespaceToken->getType() == srcMLParser::EQUAL
                )
                || (
                    (prevNonWhitespaceToken->getLine() == token->getLine())
                    && !srcMLParser::declaration_specifiers_ts_token_set.member(token->getType())
                    && !srcMLParser::function_declaration_specifiers_ts_token_set.member(token->getType())
                    && token->getType() != srcMLParser::JS_LET
                    && token->getType() != srcMLParser::JS_VAR
                    && token->getType() != srcMLParser::JS_CONST
                    && token->getType() != srcMLParser::JS_STATIC
                    && prevNonWhitespaceToken->getType() == srcMLParser::LPAREN
                )
                || (
                    (prevNonWhitespaceToken->getLine() == token->getLine() || bracketBuffer.front() == "(")
                    && prevNonWhitespaceToken->getType() == srcMLParser::OPERATORS
                )
                || prevNonWhitespaceToken->getType() == srcMLParser::JS_AS
            )
        )
        || (
            prevNonWhitespaceToken->getType() == srcMLParser::JS_AWAIT
            && srcMLParser::name_differentiator_js_token_set.member(token->getType())
            && nextToken->getType() == srcMLParser::LPAREN
        )
        || (
            (token->getType() == srcMLParser::CLASS || token->getType() == srcMLParser::JS_FUNCTION)
            && prevNonWhitespaceToken->getType() == srcMLParser::JS_AS
        )
        || (prevToken->getType() == srcMLParser::PERIOD || nextToken->getType() == srcMLParser::PERIOD)
        || (prevToken->getType() == srcMLParser::QMARK_PERIOD || nextToken->getType() == srcMLParser::QMARK_PERIOD)
        || (token->getType() == srcMLParser::JS_VOID && nextToken->getType() == srcMLParser::JS_ARROW)
        || (token->getType() == srcMLParser::JS_ASYNC && nextToken->getType() == srcMLParser::JS_RANGE_OF)
        || (
            token->getType() == srcMLParser::JS_ASYNC
            && (nextToken->getType() == srcMLParser::QMARK || nextToken->getType() == srcMLParser::JS_RANGE_IN)
        )
        || (
            prevNonWhitespaceToken->getType() == srcMLParser::CLASS
            && (
                token->getType() != srcMLParser::NAME
                && token->getType() != srcMLParser::LCURLY
            )
        )
        || (
            bracketBuffer.front() == "*"
            && (
                prevNonWhitespaceToken->getType() == srcMLParser::TEMPOPS
                || prevNonWhitespaceToken->getType() == srcMLParser::COMMA
            )
            && srcMLParser::name_differentiator_js_token_set.member(token->getType())
            && (
                nextToken->getType() == srcMLParser::TEMPOPE
                || nextToken->getType() == srcMLParser::COMMA
            )
        )
        || (srcMLParser::decl_start_js_token_set.member(token->getType()) && nextToken->getType() == srcMLParser::LPAREN)
        || (
            token->getType() == srcMLParser::NEW
            && (
                prevNonWhitespaceToken->getType() == srcMLParser::LCURLY
                || prevNonWhitespaceToken->getType() == srcMLParser::RCURLY
                || prevNonWhitespaceToken->getType() == srcMLParser::TERMINATE
            )
        )
        || (
            srcMLParser::decl_start_js_token_set.member(prevNonWhitespaceToken->getType())
            && (
                token->getType() == srcMLParser::JS_GET
                || token->getType() == srcMLParser::JS_SET
                || token->getType() == srcMLParser::TS_PRIVATE
                || token->getType() == srcMLParser::TS_PROTECTED
                || token->getType() == srcMLParser::TS_PUBLIC
            )
            && (
                nextToken->getType() == srcMLParser::EQUAL
                || nextToken->getType() == srcMLParser::TERMINATE
                || nextToken->getType() == srcMLParser::EOF_
                || nextToken->getLine() > token->getLine()
            )
        )
    );
}

/**
 * Detects opening and closing brackets (e.g., `()`, `{}`, and `[]`).
 * 
 * Operates under the assumption the code contains balanced brackets.
 */
void NameDifferentiatorJavaScript::checkBracketToken(antlr::RefToken token) {
    switch (token->getType()) {
        case srcMLParser::LPAREN:
        case srcMLParser::LCURLY:
        case srcMLParser::LBRACKET:
            ++numBrackets;
            bracketBuffer.emplace_front(token->getText());
            break;

        case srcMLParser::RPAREN:
        case srcMLParser::RCURLY:
        case srcMLParser::RBRACKET:
            if (numBrackets > 0) {
                --numBrackets;
            }

            if (bracketBuffer.front() != "*") {
                bracketBuffer.pop_front();
            }

            break;
    }
}
