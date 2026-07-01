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
            currentToken = token;
    }

    // process pairs of tokens: the previously-used token and the next non-skip token
    if (buffer.empty()) {
        antlr::RefToken token = currentToken;
        auto nextNonSkipToken = input.nextToken();
        bool containsEOL = false;
        bool containsTerminate = false;

        // special case: do not generate an extra TERMINATE if the current token is TERMINATE
        if (token->getType() == srcMLParser::TERMINATE)
            containsTerminate = true;

        // find the next non-skip token, if applicable
        while (srcMLParser::skip_tokens_set.member(nextNonSkipToken->getType())) {
            // record if the temporary skip token buffer will contain an EOL
            if (nextNonSkipToken->getType() == srcMLParser::EOL)
                containsEOL = true;

            tempSkipBuffer.emplace_back(nextNonSkipToken);
            nextNonSkipToken = input.nextToken();
        }

        // special case: do not generate an extra TERMINATE if the next token is TERMINATE
        if (nextNonSkipToken->getType() == srcMLParser::TERMINATE)
            containsTerminate = true;

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

        // do not insert a terminate between "<" and a TypeScript word operator used in generic argument lists
        if (
            token->getType() == srcMLParser::TEMPOPS
            && (
                nextNonSkipToken->getType() == srcMLParser::JS_TYPEOF
                || nextNonSkipToken->getType() == srcMLParser::TS_INFER
                || nextNonSkipToken->getType() == srcMLParser::TS_IS
                || nextNonSkipToken->getType() == srcMLParser::TS_KEYOF
                || nextNonSkipToken->getType() == srcMLParser::TS_SATISFIES
            )
        ) {
            insertTerminate = false;
        }

        buffer.emplace_back(token);

        // insert a TERMINATE token if applicable
        if (insertTerminate && !containsTerminate && isTerminateCase(token, nextNonSkipToken, containsEOL))
            insertTerminateToken(token->getLine());

        // stop "= ..." check if there was an EOL between the token and the next non-skip token
        if (inEqualRHS && containsEOL)
            inEqualRHS = false;

        // start checking the right-hand side of an "=" to see if an inserted terminate is required
        if (!containsEOL && token->getType() == srcMLParser::EQUAL)
            inEqualRHS = true;

        // empty the temporary skip token buffer, if applicable
        while (!tempSkipBuffer.empty()) {
            buffer.emplace_back(tempSkipBuffer.front());
            tempSkipBuffer.pop_front();
        }

        prevNonWhitespaceToken = currentToken;
        currentToken = nextNonSkipToken;

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

            // found "= ... \n {}", so there should be a terminate between the end of equal RHS and the "{"
            || (
                containsEOL
                && inEqualRHS
                && nextNonSkipToken->getType() == srcMLParser::LCURLY
            )

            // token is JS_DEBUGGER (always insert a terminate after a JS_DEBUGGER token)
            || (token->getType() == srcMLParser::JS_DEBUGGER)

            // an EOL separates the token and the next non-skip token, which are any combination of:
            // - names
            // - literals (numbers, booleans, strings, etc.) except backtick literals
            // Note: never place a TERMINATE between the start and end of a string or char
            || (
                containsEOL
                && srcMLParser::insert_terminate_js_token_set.member(token->getType())
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
                && (
                    (
                        nextNonSkipToken->getType() != srcMLParser::JS_CONST
                        && srcMLParser::name_differentiator_js_token_set.member(nextNonSkipToken->getType())
                    )
                    || (
                        nextNonSkipToken->getType() == srcMLParser::JS_CONST
                        && token->getType() != srcMLParser::TEMPOPS
                    )
                )
            )

            // an EOL separates RPAREN/RBRACKET and any non-skip token if top-level
            || (
                containsEOL
                && (bracketTokenTracker.front() == "*" || bracketTokenTracker.front() == "{")
                && (token->getType() == srcMLParser::RPAREN || token->getType() == srcMLParser::RBRACKET)
                && !srcMLParser::general_operator_tokens_set.member(nextNonSkipToken->getType())
                && nextNonSkipToken->getType() != srcMLParser::COLON
                && nextNonSkipToken->getType() != srcMLParser::QMARK
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

            // an EOL separates an RCURLY "}" and an LCURLY "{"
            || (
                containsEOL
                && token->getType() == srcMLParser::RCURLY
                && nextNonSkipToken->getType() == srcMLParser::LCURLY
            )

            // an EOL separates an "!" and a NAME if the previous token was ")"
            || (
                containsEOL
                && prevNonWhitespaceToken->getType() == srcMLParser::RPAREN
                && token->getType() == srcMLParser::OPERATORS
                && token->getText() == "!"
                && nextNonSkipToken->getType() == srcMLParser::NAME
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
