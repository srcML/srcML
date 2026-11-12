// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file FastKeywordLexer.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * KeywordLexer with a fast path for the most common tokens in C and C++, names and
 * whitespace, that produces the same tokens with the same changes to the lexer state
 * as the rules of the grammar, without matching a character at a time.
 */

#ifndef INCLUDED_FAST_KEYWORD_LEXER_HPP
#define INCLUDED_FAST_KEYWORD_LEXER_HPP

#include <KeywordLexer.hpp>
#include <array>
#ifdef SRCML_LEXER_SHADOW
#include <cstdio>
#endif

class FastKeywordLexer : public KeywordLexer {
public:

    using KeywordLexer::KeywordLexer;

    antlr::RefToken nextToken() override {

        // nextToken() is not called while the lexer is guessing, but the fast path always keeps the text and column,
        // which consume() does not do while guessing
        if ((getLanguage() == LANGUAGE_C || getLanguage() == LANGUAGE_CXX) && inputState->guessing == 0) {

            const int c = LA(1);
            if (c >= 0 && c <= 0xFF && charClass()[c] == NAME_START)
                return fastName();
            if (c >= '0' && c <= '9')
                return fastInteger();
            if ((c == ' ' || c == '\t') && !isoption(options, SRCML_PARSER_OPTION_EXPAND_TABS))
                return fastWhitespace();
            switch (c) {
            case ';': return fastPunctuation(c, TERMINATE);
            case ',': return fastPunctuation(c, COMMA);
            case '(': return fastPunctuation(c, LPAREN);
            case ')': return fastPunctuation(c, RPAREN);
            case '{': return fastPunctuation(c, LCURLY);
            case '}': return fastPunctuation(c, RCURLY);
            }
        }

        return KeywordLexer::nextToken();
    }

private:

    // Single-character operators whose token types are fixed in C and C++.
    antlr::RefToken fastPunctuation(int c, int type) {
#ifdef SRCML_LEXER_SHADOW
        const std::string expectText(1, static_cast<char>(c));
        const int line = getLine();
        const int column = getColumn();
        const int three = lookaheadMinusTwo, two = lookaheadMinusOne;
        auto token = KeywordLexer::nextToken();
        shadowCompare(token, type, expectText, line, column, column + 1, three, two, c, c, false);
        if (lastpos != column + 1 || prev != c)
            fprintf(stderr, "LEXER-SHADOW operator state difference at %d:%d lastpos %d/%d prev %d/%d\n",
                    line, column, lastpos, column + 1, prev, c);
        return token;
#else
        resetText();
        text += static_cast<char>(c);
        lookaheadMinusThree = lookaheadMinusTwo;
        lookaheadMinusTwo = lookaheadMinusOne;
        lookaheadMinusOne = c;
        lastnonspacetoken = c;
        inputState->getInput().consume();
        ++inputState->column;
        startline = false;
        lastpos = getColumn();
        prev = c;
        _returnToken = makeToken(type);
        _returnToken->setText(text);
        return _returnToken;
#endif
    }

    enum { OTHER = 0, NAME_START = 1, NAME_CHAR = 2 };

    // characters of names, as in the NAME rule for C and C++
    static const std::array<unsigned char, 256>& charClass() {
        static const auto table = [] {
            std::array<unsigned char, 256> table{};
            for (int c = 'a'; c <= 'z'; ++c)
                table[c] = NAME_START;
            for (int c = 'A'; c <= 'Z'; ++c)
                table[c] = NAME_START;
            for (int c = 0x80; c <= 0xFF; ++c)
                table[c] = NAME_START;
            table['_'] = NAME_START;
            table['$'] = NAME_START;
            for (int c = '0'; c <= '9'; ++c)
                table[c] = NAME_CHAR;
            return table;
        }();
        return table;
    }

    static bool isNameChar(int c) {
        return c >= 0 && c <= 0xFF && charClass()[c] != OTHER;
    }

    // a name, unless it is a prefix of a string or character literal, which the grammar handles
    antlr::RefToken fastName() {

        antlr::InputBuffer& input = inputState->getInput();
        unsigned int n = 1;
        for (;;) {
            const unsigned int available = input.entries();
            const int* chars = input.data();
            while (n < available && isNameChar(chars[n]))
                ++n;
            if (n < available)
                break;
            // Refill can reallocate the queue, so reacquire its pointer above.
            input.LA(n + 1);
        }
        const int* chars = input.data();
        const int after = chars[n];
        if (after == '"' || after == '\'')
            return KeywordLexer::nextToken();

#ifdef SRCML_LEXER_SHADOW
        // the expected token and state, compared to the grammar
        std::string expectText;
        for (unsigned int i = 1; i <= n; ++i)
            expectText += static_cast<char>(LA(i));
        const int expectLine = getLine();
        const int expectStartColumn = getColumn();
        int three = lookaheadMinusThree, two = lookaheadMinusTwo, one = lookaheadMinusOne;
        for (char ch : expectText) {
            three = two;
            two = one;
            one = static_cast<unsigned char>(ch);
        }
        const int expectType = testLiteralsTable(expectText, NAME);
        auto token = KeywordLexer::nextToken();
        shadowCompare(token, expectType, expectText, expectLine, expectStartColumn, expectStartColumn + (int) n,
                      three, two, one, one, false);
        return token;
#else
        resetText();

        // as the NAME rule: not at the start of a line, and each character is a non-whitespace character
        startline = false;
        text.append(chars, n);
        if (n >= 3) {
            lookaheadMinusThree = chars[n - 3];
            lookaheadMinusTwo = chars[n - 2];
        } else if (n == 2) {
            lookaheadMinusThree = lookaheadMinusOne;
            lookaheadMinusTwo = chars[0];
        } else {
            lookaheadMinusThree = lookaheadMinusTwo;
            lookaheadMinusTwo = lookaheadMinusOne;
        }
        lookaheadMinusOne = chars[n - 1];
        input.consume(n);
        lastnonspacetoken = lookaheadMinusOne;
        inputState->column += (int) n;

        _returnToken = makeToken(testLiteralsTable(NAME));
        _returnToken->setText(text);
        return _returnToken;
#endif
    }

    // Unsuffixed integer digits; leave more complex literals and #line state to the grammar.
    antlr::RefToken fastInteger() {
        if (onpreprocline && isline)
            return KeywordLexer::nextToken();

        antlr::InputBuffer& input = inputState->getInput();
        unsigned int n = 1;
        for (;;) {
            const unsigned int available = input.entries();
            const int* chars = input.data();
            while (n < available && chars[n] >= '0' && chars[n] <= '9')
                ++n;
            if (n < available)
                break;
            input.LA(n + 1);
        }
        const int* chars = input.data();
        const int after = chars[n];
        if (isNameChar(after) || after == '.' || after == '\'' || after == '"')
            return KeywordLexer::nextToken();

#ifdef SRCML_LEXER_SHADOW
        std::string expectText;
        expectText.reserve(n);
        for (unsigned int i = 0; i < n; ++i)
            expectText += static_cast<char>(chars[i]);
        const int line = getLine(), column = getColumn();
        const int three = lookaheadMinusThree, two = lookaheadMinusTwo, one = lookaheadMinusOne;
        const int last = chars[n - 1];
        auto token = KeywordLexer::nextToken();
        shadowCompare(token, CONSTANTS, expectText, line, column, column + static_cast<int>(n),
                      three, two, one, last, false);
        return token;
#else
        resetText();
        text.append(chars, n);
        lastnonspacetoken = chars[n - 1];
        input.consume(n);
        inputState->column += static_cast<int>(n);
        startline = false;
        _returnToken = makeToken(CONSTANTS);
        _returnToken->setText(text);
        return _returnToken;
#endif
    }

    // a run of spaces and tabs, without tab expansion
    antlr::RefToken fastWhitespace() {

#ifdef SRCML_LEXER_SHADOW
        std::string expectText;
        int column = getColumn();
        int last = lastnonspacetoken;
        for (unsigned int i = 1; LA(i) == ' ' || LA(i) == '\t'; ++i) {
            expectText += static_cast<char>(LA(i));
            if (LA(i) == '\t') {
                column = (((column - 1) / getTabSize()) + 1) * getTabSize() + 1;
                last = '\t';
            } else {
                ++column;
            }
        }
        const int expectLine = getLine();
        const int expectStartColumn = getColumn();
        const int three = lookaheadMinusThree, two = lookaheadMinusTwo, one = lookaheadMinusOne;
        const bool expectStartline = startline;
        auto token = KeywordLexer::nextToken();
        shadowCompare(token, WS, expectText, expectLine, expectStartColumn, column,
                      three, two, one, last, expectStartline);
        return token;
#else
        resetText();

        antlr::InputBuffer& input = inputState->getInput();

        unsigned int n = 0;
        int column = getColumn();
        bool hasTab = false;
        for (;;) {
            const unsigned int available = input.entries();
            const int* chars = input.data();
            while (n < available && (chars[n] == ' ' || chars[n] == '\t')) {
                if (chars[n] == '\t') {
                    column = (((column - 1) / getTabSize()) + 1) * getTabSize() + 1;
                    hasTab = true;
                } else {
                    ++column;
                }
                ++n;
            }
            if (n < available)
                break;
            // Refill can reallocate the queue, so reacquire its pointer above.
            input.LA(n + 1);
        }
        text.append(input.data(), n);
        input.consume(n);
        inputState->column = column;
        // As the WS rule: spaces preserve the last non-space character, but tabs replace it.
        if (hasTab)
            lastnonspacetoken = '\t';

        _returnToken = makeToken(WS);
        _returnToken->setText(text);
        return _returnToken;
#endif
    }

#ifdef SRCML_LEXER_SHADOW
    void shadowCompare(const antlr::RefToken& token, int type, const std::string& expectText, int line, int startColumn,
                       int endColumn, int three, int two, int one, int last, bool expectStartline) {
        static long tokens = 0, differences = 0;
        static struct Report { ~Report() { fprintf(stderr, "LEXER-SHADOW tokens %ld differences %ld\n", tokens, differences); } } report;
        ++tokens;
        const bool same = token->getType() == type && token->getText() == expectText && token->getLine() == line
            && token->getColumn() == startColumn && getColumn() == endColumn && lookaheadMinusThree == three
            && lookaheadMinusTwo == two && lookaheadMinusOne == one && lastnonspacetoken == last
            && startline == expectStartline;
        if (!same && ++differences <= 20)
            fprintf(stderr, "LEXER-SHADOW difference at %d:%d type %d/%d text '%s'/'%s' end %d/%d history %d,%d,%d/%d,%d,%d last %d/%d start %d/%d\n",
                line, startColumn, token->getType(), type, token->getText().data(), expectText.data(), getColumn(), endColumn,
                lookaheadMinusThree, lookaheadMinusTwo, lookaheadMinusOne, three, two, one, lastnonspacetoken, last, startline, expectStartline);
    }
#endif
};

#endif
