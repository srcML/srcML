// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file NewlineTerminateJavaScript.cpp
 *
 * @copyright Copyright (C) 2025 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Injects artificial TERMINATE tokens to the token stream
 */

#include <NewlineTerminateJavaScript.hpp>

// insert TERMINATE tokens to JavaScript code without semicolon delimiters
antlr::RefToken NewlineTerminateJavaScript::nextToken() {
    // determine the first non-skip token in the file before doing anything else
    if (firstToken) {
        // add a dummy entry to the bracket token buffer to ensure it is never empty
        bracketTokenTracker.emplace_front("*");

        antlr::RefToken token = input.nextToken();

        // find the next non-skip token
        while (srcMLParser::skip_tokens_set.member(token->getType())) {
            buffer.emplace_back(token);
            token = input.nextToken();
        }

        // in a code snippet beginning with '(', '[', or '{'
        if (
            token->getType() == srcMLParser::LPAREN
            || token->getType() == srcMLParser::LBRACKET
            || token->getType() == srcMLParser::LCURLY
        )
            bracketTokenTracker.emplace_back(token->getText());

        firstToken = false;

        // denote the current token as the upcoming last token, if it is not a skip token
        if (!srcMLParser::skip_tokens_set.member(token->getType()))
            lastToken = token;
    }

    // process pairs of tokens: the previously-used token and the next non-skip token
    if (buffer.empty()) {
        antlr::RefToken token = lastToken;
        auto nextNonSkipToken = input.nextToken();
        bool containsEOL = false;

        // find the next non-skip token, if applicable
        while (srcMLParser::skip_tokens_set.member(nextNonSkipToken->getType())) {
            // record if the temporary skip token buffer will contain an EOL
            if (nextNonSkipToken->getType() == srcMLParser::EOL)
                containsEOL = true;

            tempSkipBuffer.emplace_back(nextNonSkipToken);
            nextNonSkipToken = input.nextToken();
        }

        // in a code snippet beginning with '(', '[', or '{'
        if (
            token->getType() == srcMLParser::LPAREN
            || token->getType() == srcMLParser::LBRACKET
            || token->getType() == srcMLParser::LCURLY
        ) {
            bracketTokenTracker.emplace_front(token->getText());
        }
        // exiting a code snippet that began with '(', '[', or '{'
        else if (
            bracketTokenTracker.front() != "*"
            && (
                token->getType() == srcMLParser::RPAREN
                || token->getType() == srcMLParser::RBRACKET
                || token->getType() == srcMLParser::RCURLY
            )
        ) {
            bracketTokenTracker.pop_front();
        }

        // do not insert a terminate in code enclosed in parentheses or square brackets
        if (bracketTokenTracker.front() == "(" || bracketTokenTracker.front() == "[")
            insertTerminate = false;
        else
            insertTerminate = true;

        buffer.emplace_back(token);

        // insert a TERMINATE token if applicable
        if (insertTerminate && isTerminateCase(token, nextNonSkipToken, containsEOL))
            insertTerminateToken(token->getLine());

        // empty the temporary skip token buffer, if applicable
        while (!tempSkipBuffer.empty()) {
            buffer.emplace_back(tempSkipBuffer.front());
            tempSkipBuffer.pop_front();
        }

        lastToken = nextNonSkipToken;

        // ensure the EOF token is not missed at the end of a file
        if (nextNonSkipToken->getType() == 1 /* EOF */)
            buffer.emplace_back(nextNonSkipToken);

        // ensure name + postfix operator detection is turned off after potential usage
        if (wasPostfixName)
            wasPostfixName = false;

        // check if NAME and "++" (or "--") were directly next to each other
        if (
            !containsEOL
            && token->getType() == srcMLParser::NAME
            && nextNonSkipToken->getType() == srcMLParser::OPERATORS
            && (
                nextNonSkipToken->getText() == "++"
                || nextNonSkipToken->getText() == "--"
            )
        )
            wasPostfixName = true;
    }

    // next token
    auto token = buffer.front();
    buffer.pop_front();
    return token;
}

/**
 * Checks if the given situation requires a manual TERMINATE insertion.
 *
 * `token` is the current token.
 * `nextNonSkipToken` is the next token after `token` that is not WS, comments, etc.
 * `containsEOL` is true if there is an EOL between `token` and `nextNonSkipToken`, otherwise it is false.
 */
bool NewlineTerminateJavaScript::isTerminateCase(antlr::RefToken token, antlr::RefToken nextNonSkipToken, bool containsEOL) {

    return (
        (
            // token is not LCURLY and the next non-skip token is RCURLY
            // (the parser will ignore these TERMINATEs for specific scenarios such as name lists)
            (token->getType() != srcMLParser::LCURLY && nextNonSkipToken->getType() == srcMLParser::RCURLY)

            // the next non-skip token is the end of the file
            || (nextNonSkipToken->getType() == 1 /* EOF */)

            // token is JS_DEBUGGER (always insert a terminate after a JS_DEBUGGER token) 
            || (token->getType() == srcMLParser::JS_DEBUGGER)

            // both token and the next non-skip token are any combination of the following:
            // - names
            // - literals (numbers, booleans, strings, etc.) except backtick literals
            // Note: never place a TERMINATE between the start and end of a string or char
            || (
                srcMLParser::insert_terminate_js_token_set.member(token->getType())
                && srcMLParser::insert_terminate_js_token_set.member(nextNonSkipToken->getType())
                && (
                    !(token->getType() == srcMLParser::STRING_START && nextNonSkipToken->getType() == srcMLParser::STRING_END)
                    && !(token->getType() == srcMLParser::CHAR_START && nextNonSkipToken->getType() == srcMLParser::CHAR_END)
                )
            )

            // an EOL separates (almost) any combination of the following tokens:
            // - names
            // - left parentheses (for lambdas; must be "nextNonSkipToken", not "token")
            // - right parentheses/brackets/braces (must be "token", not "nextNonSkipToken")
            // - literals (numbers, booleans, strings, etc.), including backtick literals
            // - the bitwise NOT operator ("~")
            // Note: never place a TERMINATE between the start and end of a string, char, or backtick literal
            || (
                containsEOL
                && (
                    srcMLParser::insert_terminate_eol_js_token_set.member(token->getType())
                    || token->getType() == srcMLParser::RPAREN
                    || token->getType() == srcMLParser::RBRACKET
                    || token->getType() == srcMLParser::RCURLY
                )
                && (
                    srcMLParser::insert_terminate_eol_js_token_set.member(nextNonSkipToken->getType())
                    || nextNonSkipToken->getType() == srcMLParser::LPAREN
                )
                && (
                    !(token->getType() == srcMLParser::STRING_START && nextNonSkipToken->getType() == srcMLParser::STRING_END)
                    && !(token->getType() == srcMLParser::CHAR_START && nextNonSkipToken->getType() == srcMLParser::CHAR_END)
                    && !(token->getType() == srcMLParser::BACKTICK_START && nextNonSkipToken->getType() == srcMLParser::BACKTICK_END)
                )
            )

            // an EOL separates "++" or "--" (with a NAME before it) and any other non-skip token
            || (
                wasPostfixName
                && containsEOL
                && (
                    token->getType() == srcMLParser::OPERATORS
                    && (
                        token->getText() == "++"
                        || token->getText() == "--"
                    )
                )
            )

            // an EOL separates token and "++" (or "--")
            || (
                containsEOL
                && (
                    nextNonSkipToken->getType() == srcMLParser::OPERATORS
                    && (
                        nextNonSkipToken->getText() == "++"
                        || nextNonSkipToken->getText() == "--"
                    )
                )
            )

            // an EOL separates token and "!"
            || (
                containsEOL
                && nextNonSkipToken->getType() == srcMLParser::OPERATORS
                && nextNonSkipToken->getText() == "!"
            )

            // an EOL separates BREAK/CONTINUE and NAME
            || (
                containsEOL
                && (token->getType() == srcMLParser::BREAK || token->getType() == srcMLParser::CONTINUE)
                && nextNonSkipToken->getType() == srcMLParser::NAME
            )

            // a non-NAME token separates BREAK/CONTINUE
            || (
                (token->getType() == srcMLParser::BREAK || token->getType() == srcMLParser::CONTINUE)
                && nextNonSkipToken->getType() != srcMLParser::NAME
            )

            // an EOL separates RETURN/THROW/YIELD and any non-skip token
            || (
                containsEOL
                && (
                    token->getType() == srcMLParser::RETURN
                    || token->getType() == srcMLParser::THROW
                    || token->getType() == srcMLParser::JS_YIELD
                )
            )

            // an EOL separates RPAREN and JS_ARROW
            || (
                containsEOL
                && token->getType() == srcMLParser::RPAREN
                && nextNonSkipToken->getType() == srcMLParser::JS_ARROW
            )

            // an EOL separates JS_ASYNC and LPAREN/JS_FUNCTION
            || (
                containsEOL
                && token->getType() == srcMLParser::JS_ASYNC
                && (
                    nextNonSkipToken->getType() == srcMLParser::LPAREN
                    || nextNonSkipToken->getType() == srcMLParser::JS_FUNCTION
                )
            )

            // an EOL separates a token (in certain cases) and a JavaScript/TypeScript keyword
            || (
                containsEOL
                && token->getType() != srcMLParser::LCURLY
                && token->getType() != srcMLParser::COMMA
                && (
                    !srcMLParser::name_differentiator_js_token_set.member(token->getType())
                    || token->getType() == srcMLParser::LITERAL_TRUE
                    || token->getType() == srcMLParser::LITERAL_FALSE
                    || token->getType() == srcMLParser::JS_NULL
                    || token->getType() == srcMLParser::JS_UNDEFINED
                    || token->getType() == srcMLParser::JS_REGEX
                    || token->getType() == srcMLParser::JS_VOID
                )
                && srcMLParser::name_differentiator_js_token_set.member(nextNonSkipToken->getType())
            )

            // an EOL separates RPAREN/RBRACKET and any non-skip token if top-level
            || (
                containsEOL
                && (bracketTokenTracker.front() == "*" || bracketTokenTracker.front() == "{")
                && (token->getType() == srcMLParser::RPAREN || token->getType() == srcMLParser::RBRACKET)
            )

            // an EOL separates a JSX literal (e.g., "<div/>") and any non-skip token
            || (containsEOL && token->getType() == srcMLParser::JS_JSX_LITERAL)

            // an EOL separates a name and a constraint in TypeScript declaration statements
            || (
                containsEOL
                && bracketTokenTracker.front() == "{"
                && token->getType() == srcMLParser::NAME
                && nextNonSkipToken->getType() == srcMLParser::LBRACKET
            )
        )
    );
}

/**
 * Creates a new TERMINATE token and adds it to the buffer.
 *
 * `line` is the TERMINATE token's intended line number.
 */
void NewlineTerminateJavaScript::insertTerminateToken(int line) {
    auto terminateToken = srcMLToken::factory();
    terminateToken->setType(srcMLParser::TERMINATE);
    terminateToken->setColumn(1);
    terminateToken->setLine(line);

    buffer.emplace_back(terminateToken);
}
