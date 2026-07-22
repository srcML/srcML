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
        if (
            srcMLParser::name_differentiator_js_token_set.member(token->getType())
            || token->getType() == srcMLParser::JS_VOID
            || token->getType() == srcMLParser::TS_ENUM
        )
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
        CASE 1: Do not change `token` if it is followed by `{` or another NAME
    */
    if (nextToken->getType() == srcMLParser::LCURLY || nextToken->getType() == srcMLParser::NAME) {
        prevToken = nextToken;

        if (!srcMLParser::skip_tokens_set.member(token->getType()))
            prevNonWhitespaceToken = token;

        return;
    }

    /*
        CASE 2: `token` should be the name of a variable in a declaration (if it is not already)
                and `token` should be the name of a TypeScript type in an argument list
    */
    if (
        (
            srcMLParser::decl_start_js_token_set.member(prevNonWhitespaceToken->getType())
            && token->getType() != srcMLParser::JS_ASYNC
            && token->getType() != srcMLParser::JS_GET
            && token->getType() != srcMLParser::JS_SET
            && token->getType() != srcMLParser::NAME
            && token->getType() != srcMLParser::TS_ACCESSOR
            && token->getType() != srcMLParser::TS_ENUM
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

        if (isNameToken(token, nextToken, true))
            token->setType(srcMLParser::NAME);

        // Update here to ensure the bracket count is correct after invoking name-checking logic
        checkBracketToken(nextToken);  // Detect if currently in/out of `()`, `{}`, or `[]`

        prevToken = newPrevToken;

        if (!srcMLParser::skip_tokens_set.member(token->getType()))
            prevNonWhitespaceToken = token;

        // The new `nextToken` may need to be processed (e.g., "as default" in name lists)
        if (
            srcMLParser::name_differentiator_js_token_set.member(nextToken->getType())
            || nextToken->getType() == srcMLParser::JS_VOID
            || nextToken->getType() == srcMLParser::TS_ENUM
        )
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
            || nextToken->getType() == srcMLParser::RPAREN
            || nextToken->getType() == srcMLParser::RCURLY
            || nextToken->getType() == srcMLParser::RBRACKET
            || (nextToken->getType() == srcMLParser::OPERATORS && token->getType() != srcMLParser::RETURN)
        )
    ) {
        token->setType(srcMLParser::NAME);
        prevToken = nextToken;

        if (!srcMLParser::skip_tokens_set.member(token->getType()))
            prevNonWhitespaceToken = token;

        return;
    }

    /*
        CASE 5: Change `token` to a NAME if a condition is met in `isNameToken()`
    */
    if (isNameToken(token, nextToken, false)) {
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
bool NameDifferentiatorJavaScript::isNameToken(antlr::RefToken token, antlr::RefToken nextToken, bool hadWhitespace) const {
    // special case: the current token is "enum" or "void" and the next token is a name
    // can trigger if "enum" or "void" were the nextToken entry in Case #3 and sent recursively
    if (
        (token->getType() == srcMLParser::JS_VOID || token->getType() == srcMLParser::TS_ENUM)
        && nextToken->getType() == srcMLParser::NAME
    ) {
        return false;
    }

    return (
        // the current token is a subset of all keywords and it is directly next to a "(" (no whitespace, newlines, etc.)
        (
            !hadWhitespace
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
                && token->getType() != srcMLParser::SWITCH
                && token->getType() != srcMLParser::WHILE
            )
            && nextToken->getType() == srcMLParser::LPAREN
        )

        // the current token is a subset of all keywords and the next token is a ";"
        || (
            srcMLParser::name_differentiator_subset_js_token_set.member(token->getType())
            && token->getType() != srcMLParser::BREAK
            && token->getType() != srcMLParser::CONTINUE
            && token->getType() != srcMLParser::RETURN
            && nextToken->getType() == srcMLParser::TERMINATE
        )

        // the current token is a keyword in the table and the next token is an "="
        || (
            srcMLParser::table_keywords_js_token_set.member(token->getType())
            && nextToken->getType() == srcMLParser::EQUAL
        )

        // the current token is a subset of all keywords, surrounded by "{}" or "[]"
        || (
            srcMLParser::name_differentiator_js_token_set.member(token->getType())
            && (
                (
                    prevNonWhitespaceToken->getType() == srcMLParser::LCURLY
                    && token->getType() != srcMLParser::BREAK
                    && token->getType() != srcMLParser::CONTINUE
                    && token->getType() != srcMLParser::RETURN
                    && nextToken->getType() == srcMLParser::RCURLY
                )
                || (
                    prevNonWhitespaceToken->getType() == srcMLParser::LBRACKET
                    && nextToken->getType() == srcMLParser::RBRACKET
                )
            )
        )

        // the previous token was ":", the current token is a subset of all table keywords,
        // and the next token is either "," or "}"
        || (
            (
                srcMLParser::table_keywords_js_token_set.member(token->getType())
                && token->getType() != srcMLParser::BREAK
                && token->getType() != srcMLParser::CONTINUE
                && token->getType() != srcMLParser::RETURN
            )
            && (
                prevNonWhitespaceToken->getType() == srcMLParser::COLON
                && (
                    nextToken->getType() == srcMLParser::COMMA
                    || nextToken->getType() == srcMLParser::RCURLY
                )
            )
        )

        // the previous token is a declaration specifier, current token is a keyword, and the next token is "=" or ";"
        || (
            srcMLParser::declaration_specifiers_ts_token_set.member(prevNonWhitespaceToken->getType())
            && (
                srcMLParser::name_differentiator_js_token_set.member(token->getType())
                || token->getType() == srcMLParser::TS_ENUM
            )
            && (
                nextToken->getType() == srcMLParser::EQUAL
                || nextToken->getType() == srcMLParser::TERMINATE
            )
        )

        // the current token is a declaration specifier and the next token is "as"
        || (
            (
                srcMLParser::declaration_specifiers_ts_token_set.member(token->getType())
                || srcMLParser::function_declaration_specifiers_ts_token_set.member(token->getType())
            )
            && nextToken->getType() == srcMLParser::JS_AS
        )

        // the current token is NOT "default" and the next token is ":"
        || (token->getType() != srcMLParser::JS_DEFAULT && nextToken->getType() == srcMLParser::COLON)

        // the current token is a subset of all keywords and the next token is "?"
        || (
            (
                srcMLParser::name_differentiator_subset_js_token_set.member(token->getType())
                || token->getType() == srcMLParser::TS_READONLY
            )
            && nextToken->getType() == srcMLParser::QMARK
        )

        // the current token is "readonly" and one of the following is true:
        // - the previous token was "=" and the next token is NOT "["
        // - the previous token was "{" or "," and the next token is ",", ":", or "}"
        || (
            token->getType() == srcMLParser::TS_READONLY
            && (
                (prevNonWhitespaceToken->getType() == srcMLParser::EQUAL && nextToken->getType() != srcMLParser::LBRACKET)
                || (
                    (prevNonWhitespaceToken->getType() == srcMLParser::LCURLY || prevNonWhitespaceToken->getType() == srcMLParser::COMMA)
                    && (
                        nextToken->getType() == srcMLParser::COMMA
                        || nextToken->getType() == srcMLParser::COLON
                        || nextToken->getType() == srcMLParser::RCURLY
                    )
                )
            )
        )

        // the current token is "default" and one of the following is true:
        // - the next token is "as", ")", "}", or "]"
        // - the previous token was a decl specifier (e.g., "public")
        || (
            token->getType() == srcMLParser::JS_DEFAULT
            && (
                nextToken->getType() == srcMLParser::JS_AS
                || nextToken->getType() == srcMLParser::COMMA
                || nextToken->getType() == srcMLParser::RPAREN
                || nextToken->getType() == srcMLParser::RCURLY
                || nextToken->getType() == srcMLParser::RBRACKET
                || srcMLParser::declaration_specifiers_ts_token_set.member(prevNonWhitespaceToken->getType())
            )
        )

        // the current token is "type" and one of the following is true:
        // - the next token is an operator, ":", ".", "?", "=>", a terminate (";"), or end-of-file
        // - the previous token was a ":"
        // - currently in a "{}" pair and either the next token is "}", is "{ type ,", or is ", type ,"
        // - currently in a "()" or "[]" pair and the next token is ",", ")", or "]"
        || (
            token->getType() == srcMLParser::TS_TYPE
            && (
                nextToken->getType() == srcMLParser::OPERATORS
                || nextToken->getType() == srcMLParser::COLON
                || nextToken->getType() == srcMLParser::PERIOD
                || nextToken->getType() == srcMLParser::QMARK
                || nextToken->getType() == srcMLParser::JS_ARROW
                || nextToken->getType() == srcMLParser::TERMINATE
                || nextToken->getType() == srcMLParser::EOF_
                || nextToken->getType() == srcMLParser::JS_AS
                || nextToken->getType() == srcMLParser::JS_RANGE_IN
                || nextToken->getType() == srcMLParser::TS_KEYOF
                || prevNonWhitespaceToken->getType() == srcMLParser::BREAK
                || prevNonWhitespaceToken->getType() == srcMLParser::COLON
                || prevNonWhitespaceToken->getType() == srcMLParser::RETURN
                || (
                    bracketBuffer.front() == "{"
                    && (
                        nextToken->getType() == srcMLParser::RCURLY
                        || (prevNonWhitespaceToken->getType() == srcMLParser::LCURLY && nextToken->getType() == srcMLParser::COMMA)
                        || (prevNonWhitespaceToken->getType() == srcMLParser::COMMA && nextToken->getType() == srcMLParser::COMMA)
                    )
                )
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

        // the current token is "get" or "set" and 
        // - the previous token expects to be followed by a name
        // - the current token is a property in an object
        // - the next token is a multops
        || (
            (token->getType() == srcMLParser::JS_GET || token->getType() == srcMLParser::JS_SET)
            && (
                prevNonWhitespaceToken->getType() == srcMLParser::CLASS
                || prevNonWhitespaceToken->getType() == srcMLParser::JS_ARROW
                || prevNonWhitespaceToken->getType() == srcMLParser::TS_ENUM
                || prevNonWhitespaceToken->getType() == srcMLParser::JS_FUNCTION
                || prevNonWhitespaceToken->getType() == srcMLParser::TS_INTERFACE
                || prevNonWhitespaceToken->getType() == srcMLParser::TS_MODULE
                || prevNonWhitespaceToken->getType() == srcMLParser::TS_NAMESPACE
                || (prevNonWhitespaceToken->getType() == srcMLParser::LCURLY && nextToken->getType() == srcMLParser::COMMA)
                || (prevNonWhitespaceToken->getType() == srcMLParser::COMMA && nextToken->getType() == srcMLParser::COMMA)
                || (prevNonWhitespaceToken->getType() == srcMLParser::COMMA && nextToken->getType() == srcMLParser::RCURLY)
                || nextToken->getType() == srcMLParser::MULTOPS
            )
        )

        // the current token is "from" and the next token is not a string literal
        || (
            token->getType() == srcMLParser::JS_FROM 
            && (
                nextToken->getType() != srcMLParser::STRING_START
                && nextToken->getType() != srcMLParser::CHAR_START
            )
        ) 

        // the current token is a subset of all keywords and one of the following is true:
        // - the previous token was on the same line as the current token and the previous token was "="
        // - the current token is NOT a decl specifier, function specifier, "let", "var", "const", nor "static"
        //   and the previous token was "("
        // - the previous token was an operator and either [1.] the previous token was on the same line as the
        //   current token or [2.] currently in a "()" pair
        // - the previous token was "as"
        || (
            srcMLParser::name_differentiator_subset_js_token_set.member(token->getType())
            && (
                (
                    prevNonWhitespaceToken->getLine() == token->getLine()
                    && prevNonWhitespaceToken->getType() == srcMLParser::EQUAL
                )
                || (
                    !srcMLParser::declaration_specifiers_ts_token_set.member(token->getType())
                    && !srcMLParser::function_declaration_specifiers_ts_token_set.member(token->getType())
                    && token->getType() != srcMLParser::JS_LET
                    && token->getType() != srcMLParser::JS_VAR
                    && token->getType() != srcMLParser::JS_CONST
                    && token->getType() != srcMLParser::JS_STATIC
                    && prevNonWhitespaceToken->getType() == srcMLParser::LPAREN
                )
                || (
                    (prevNonWhitespaceToken->getLine() == token->getLine() || bracketBuffer.front() == "(")
                    && (
                        prevNonWhitespaceToken->getType() == srcMLParser::OPERATORS
                        && (
                            nextToken->getType() == srcMLParser::NAME
                            || !srcMLParser::literal_tokens_set.member(nextToken->getType())
                        )
                    )
                )
                || prevNonWhitespaceToken->getType() == srcMLParser::JS_AS
            )
        )

        // the current token is a declaration specifier (or table keyword) and one of the following is true:
        // - the previous token was "," or a keyword that comes before an expression
        // - the next token is ",", "]", "}", ")", ">", or ";"
        || (
            (
                srcMLParser::function_declaration_specifiers_ts_token_set.member(token->getType())
                || srcMLParser::declaration_specifiers_ts_token_set.member(token->getType())
                || srcMLParser::table_keywords_js_token_set.member(token->getType())
            )
            && (
                (
                    prevNonWhitespaceToken->getType() == srcMLParser::COMMA
                    || srcMLParser::keyword_expression_pair_js_token_set.member(prevNonWhitespaceToken->getType())
                )
                && (
                    nextToken->getType() == srcMLParser::COMMA
                    || nextToken->getType() == srcMLParser::RBRACKET
                    || nextToken->getType() == srcMLParser::RCURLY
                    || nextToken->getType() == srcMLParser::RPAREN
                    || nextToken->getType() == srcMLParser::TEMPOPE
                    || nextToken->getType() == srcMLParser::TERMINATE
                )
            )
        )

        // the previous token was "await", the current token is a keyword, and the next token is "("
        || (
            prevNonWhitespaceToken->getType() == srcMLParser::JS_AWAIT
            && srcMLParser::name_differentiator_js_token_set.member(token->getType())
            && nextToken->getType() == srcMLParser::LPAREN
        )

        // the previous token was "as" and one of the following is true:
        // - the current token is "class"
        // - the current token is "function"
        || (
            (token->getType() == srcMLParser::CLASS || token->getType() == srcMLParser::JS_FUNCTION)
            && prevNonWhitespaceToken->getType() == srcMLParser::JS_AS
        )

        // the current token is "extends" or "implements" and one of the following is true:
        // - the next token is "?" or "!"
        // - the previous token was "{" or "," and the next token is ",", ":", or "}"
        || (
            (token->getType() == srcMLParser::JS_EXTENDS || token->getType() == srcMLParser::TS_IMPLEMENTS)
            && (
                (
                    nextToken->getType() == srcMLParser::QMARK
                    || (nextToken->getType() == srcMLParser::OPERATORS && nextToken->getText() == "!")
                )
                || (
                    (prevNonWhitespaceToken->getType() == srcMLParser::LCURLY || prevNonWhitespaceToken->getType() == srcMLParser::COMMA)
                    && (
                        nextToken->getType() == srcMLParser::COMMA
                        || nextToken->getType() == srcMLParser::COLON
                        || nextToken->getType() == srcMLParser::RCURLY
                    )
                )
            )
        )

        // the previous token was "extends" or "implements", the current token is a decl. specifier, and the next token is "{"
        || (
            (
                prevNonWhitespaceToken->getType() == srcMLParser::JS_EXTENDS
                || prevNonWhitespaceToken->getType() == srcMLParser::TS_IMPLEMENTS
            )
            && (
                srcMLParser::declaration_specifiers_ts_token_set.member(token->getType())
                || srcMLParser::function_declaration_specifiers_ts_token_set.member(token->getType())
            )
            && nextToken->getType() == srcMLParser::LCURLY
        )

        // the current token is a decl. specifier and the next token is "}"
        || (
            (
                srcMLParser::declaration_specifiers_ts_token_set.member(token->getType())
                || srcMLParser::function_declaration_specifiers_ts_token_set.member(token->getType())
            )
            && nextToken->getType() == srcMLParser::RCURLY
        )

        // the previous token was "typeof", the current token is "module", and the next token is an operator
        || (
            prevNonWhitespaceToken->getType() == srcMLParser::JS_TYPEOF
            && token->getType() == srcMLParser::TS_MODULE
            && srcMLParser::general_operator_tokens_set.member(nextToken->getType())
        )

        // the previous token was NOT a ":" and the current token is "asserts"
        || (prevNonWhitespaceToken->getType() != srcMLParser::COLON && token->getType() == srcMLParser::TS_ASSERTS)

        // the current token is "void" and the next token is "=>"
        || (token->getType() == srcMLParser::JS_VOID && nextToken->getType() == srcMLParser::JS_ARROW)

        // the token directly before was a "." or the token directly after is also "."
        || (prevToken->getType() == srcMLParser::PERIOD || nextToken->getType() == srcMLParser::PERIOD)

        // the token directly before was a "?." or the token directly after is also "?."
        || (prevToken->getType() == srcMLParser::QMARK_PERIOD || nextToken->getType() == srcMLParser::QMARK_PERIOD)

        // the current token is "try" and the next token is anything but "{"
        || (token->getType() == srcMLParser::TRY && nextToken->getType() != srcMLParser::LCURLY)

        // the current token is a subset of all keywords and the next token is "=>"
        || (srcMLParser::name_differentiator_subset_js_token_set.member(token->getType()) && nextToken->getType() == srcMLParser::JS_ARROW)

        // the current token is "catch" and the previous nonwhitespace token is "."
        || (token->getType() == srcMLParser::JS_CATCH && prevNonWhitespaceToken->getType() == srcMLParser::PERIOD)

        // the current token is a subset of all keywords in the table and the previous nonwhitespace token is a function keyword
        || (
            (
                token->getType() == srcMLParser::FOR
                || token->getType() == srcMLParser::NEW
                || token->getType() == srcMLParser::SWITCH
                || token->getType() == srcMLParser::IF
                || token->getType() == srcMLParser::WHILE
                || token->getType() == srcMLParser::JS_CATCH
                || token->getType() == srcMLParser::JS_FUNCTION
                || token->getType() == srcMLParser::JS_WITH
                || token->getType() == srcMLParser::TS_ENUM
            )
            && (
                prevNonWhitespaceToken->getType() == srcMLParser::JS_FUNCTION
                || prevNonWhitespaceToken->getType() == srcMLParser::JS_GET
                || prevNonWhitespaceToken->getType() == srcMLParser::JS_SET
            )
        )

        // the current token is "accessor" one of the following is true:
        // - the next token is NOT a keyword
        // - the previous nonwhitespace is "accessor"
        || (
            token->getType() == srcMLParser::TS_ACCESSOR
            && (
                (
                    !srcMLParser::name_differentiator_js_token_set.member(nextToken->getType())
                    && nextToken->getType() != srcMLParser::TS_ENUM
                )
                || prevNonWhitespaceToken->getType() == srcMLParser::TS_ACCESSOR
            )
        )

        // the current token is "async" and one of the following is true:
        // - the next token is ":", ",", "=>", "in", "of", "?", ")", ";", or a subset of all operators
        // - the previous token was "async"
        // - the previous token was "=>" and the next token is NOT "async", "function", "(", nor a name
        // - the previous token was a subset of all keywords and the next token is NOT "async", "function",
        //   "(", "*", nor a name
        || (
            token->getType() == srcMLParser::JS_ASYNC
            && (
                nextToken->getType() == srcMLParser::COLON
                || nextToken->getType() == srcMLParser::COMMA
                || nextToken->getType() == srcMLParser::JS_ARROW
                || nextToken->getType() == srcMLParser::JS_RANGE_IN
                || nextToken->getType() == srcMLParser::JS_RANGE_OF
                || nextToken->getType() == srcMLParser::QMARK
                || nextToken->getType() == srcMLParser::RPAREN
                || nextToken->getType() == srcMLParser::TERMINATE
                || (
                    srcMLParser::general_operator_tokens_set.member(nextToken->getType())
                    && nextToken->getType() != srcMLParser::MULTOPS
                    && nextToken->getType() != srcMLParser::TEMPOPS
                )
                || prevNonWhitespaceToken->getType() == srcMLParser::JS_ASYNC
                || (
                    prevNonWhitespaceToken->getType() == srcMLParser::JS_ARROW
                    && nextToken->getType() != srcMLParser::JS_ASYNC
                    && nextToken->getType() != srcMLParser::JS_FUNCTION
                    && nextToken->getType() != srcMLParser::LPAREN
                    && nextToken->getType() != srcMLParser::NAME
                )
                || (
                    prevNonWhitespaceToken->getType() == srcMLParser::COLON
                    && (
                        nextToken->getType() == srcMLParser::COMMA
                        || nextToken->getType() == srcMLParser::RBRACKET
                        || nextToken->getType() == srcMLParser::RCURLY
                        || nextToken->getType() == srcMLParser::RPAREN
                        || nextToken->getType() == srcMLParser::TEMPOPE
                    )
                )
                || (
                    srcMLParser::name_differentiator_subset_js_token_set.member(prevNonWhitespaceToken->getType())
                    && nextToken->getType() != srcMLParser::JS_ASYNC
                    && nextToken->getType() != srcMLParser::JS_FUNCTION
                    && nextToken->getType() != srcMLParser::LPAREN
                    && nextToken->getType() != srcMLParser::MULTOPS
                    && nextToken->getType() != srcMLParser::NAME
                )
            )
        )

        // the current token is "yield" and the next token is ":", ",", or "=>"
        || (
            token->getType() == srcMLParser::JS_YIELD
            && (
                (prevNonWhitespaceToken->getType() == srcMLParser::LPAREN && nextToken->getType() == srcMLParser::RPAREN)
                || nextToken->getType() == srcMLParser::COLON
                || nextToken->getType() == srcMLParser::COMMA
                || nextToken->getType() == srcMLParser::JS_ARROW
            )
        )

        // the current token is "await" and one of the following is true:
        // - the next token is ":", ",", "=>", or ")"
        // - the previous token was "=>"
        // - the previous token was "(" and the next token is ")"
        // - the previous token was a subset of all keywords (excluding "for")
        || (
            token->getType() == srcMLParser::JS_AWAIT
            && (
                nextToken->getType() == srcMLParser::COLON
                || nextToken->getType() == srcMLParser::COMMA
                || nextToken->getType() == srcMLParser::JS_ARROW
                || nextToken->getType() == srcMLParser::RPAREN
                || prevNonWhitespaceToken->getType() == srcMLParser::JS_ARROW
                || (prevNonWhitespaceToken->getType() == srcMLParser::LPAREN && nextToken->getType() == srcMLParser::RPAREN)
                || (
                    srcMLParser::name_differentiator_subset_js_token_set.member(prevNonWhitespaceToken->getType())
                    && prevNonWhitespaceToken->getType() != srcMLParser::FOR
                )
            )
        )

        // the previous token was "class" and ALL of the following are true:
        // - the current token is NOT a name, "{", "extends", nor "implements"
        || (
            prevNonWhitespaceToken->getType() == srcMLParser::CLASS
            && (
                token->getType() != srcMLParser::NAME
                && token->getType() != srcMLParser::LCURLY
                && token->getType() != srcMLParser::JS_EXTENDS
                && token->getType() != srcMLParser::TS_IMPLEMENTS
            )
        )

        // the code is NOT enclosed in any kind of bracket and ALL of the following are true:
        // - the previous token was "<" or ","
        // - the current token is a keyword
        // - the next token is ">" or ","
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

        // the current token is "constructor" and it follows:
        // - the previous token is "{" or "," and the next token is NOT "("
        // - extends or implements (used in a super list), get or set (name of an accessor), new, or
        //   MULTOPS "*" (generator function name)
        // - the previous token was "," or a keyword that comes before an expression
        // - the next token is ",", "]", "}", ")", ">", or ";"
        || (
            token->getType() == srcMLParser::JS_CONSTRUCTOR 
            && (
                (
                    (
                        prevNonWhitespaceToken->getType() == srcMLParser::LCURLY
                        || prevNonWhitespaceToken->getType() == srcMLParser::COMMA
                    )
                    && nextToken->getType() != srcMLParser::LPAREN
                )
                || (
                    prevNonWhitespaceToken->getType() == srcMLParser::MULTOPS
                    || prevNonWhitespaceToken->getType() == srcMLParser::JS_EXTENDS
                    || prevNonWhitespaceToken->getType() == srcMLParser::JS_FUNCTION
                    || prevNonWhitespaceToken->getType() == srcMLParser::JS_GET
                    || prevNonWhitespaceToken->getType() == srcMLParser::JS_SET
                    || prevNonWhitespaceToken->getType() == srcMLParser::NEW
                    || prevNonWhitespaceToken->getType() == srcMLParser::TS_IMPLEMENTS
                )
                || (
                    (
                        prevNonWhitespaceToken->getType() == srcMLParser::COMMA
                        || srcMLParser::keyword_expression_pair_js_token_set.member(prevNonWhitespaceToken->getType())
                    )
                    && (
                        nextToken->getType() == srcMLParser::COMMA
                        || nextToken->getType() == srcMLParser::RBRACKET
                        || nextToken->getType() == srcMLParser::RCURLY
                        || nextToken->getType() == srcMLParser::RPAREN
                        || nextToken->getType() == srcMLParser::TEMPOPE
                        || nextToken->getType() == srcMLParser::TERMINATE
                    )
                )
            )
        )

        // the current token starts a decl (e.g., "var") and the next token is "("
        || (srcMLParser::decl_start_js_token_set.member(token->getType()) && nextToken->getType() == srcMLParser::LPAREN)

        // the current token is "new" and one of the following is true:
        // - the previous token was "{"
        // - the previous token was "}"
        // - the previous token was a terminate (";")
        || (
            token->getType() == srcMLParser::NEW
            && (
                prevNonWhitespaceToken->getType() == srcMLParser::LCURLY
                || prevNonWhitespaceToken->getType() == srcMLParser::RCURLY
                || prevNonWhitespaceToken->getType() == srcMLParser::TERMINATE
            )
        )

        // the current token starts a decl (e.g., "var") and both conditions are met:
        // [condition 1]:
        // - the current token is "get", "set", "private", "protected", or "public"
        // - the current token is NOT "async"
        // - the current token is "async" and the next token is NOT "*" nor a name
        // [condition 2]:
        // - the next token is "=", a terminate (";"), the end-of-file, or the next
        //   token is on a line AFTER the current token
        || (
            srcMLParser::decl_start_js_token_set.member(prevNonWhitespaceToken->getType())
            && (
                token->getType() == srcMLParser::JS_GET
                || token->getType() == srcMLParser::JS_SET
                || token->getType() == srcMLParser::TS_PRIVATE
                || token->getType() == srcMLParser::TS_PROTECTED
                || token->getType() == srcMLParser::TS_PUBLIC
                || (
                    token->getType() != srcMLParser::JS_ASYNC
                    || (
                        token->getType() == srcMLParser::JS_ASYNC
                        && (
                            nextToken->getType() != srcMLParser::MULTOPS
                            || nextToken->getType() != srcMLParser::NAME
                        )
                    )
                )
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
