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
            if ((c == ' ' || c == '\t') && !isoption(options, SRCML_PARSER_OPTION_EXPAND_TABS))
                return fastWhitespace();
        }

        return KeywordLexer::nextToken();
    }

private:

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

        unsigned int n = 1;
        while (isNameChar(LA(n + 1)))
            ++n;
        const int after = LA(n + 1);
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
        antlr::InputBuffer& input = inputState->getInput();
        for (unsigned int i = 1; i <= n; ++i) {
            const int ch = input.LA(1);
            lookaheadMinusThree = lookaheadMinusTwo;
            lookaheadMinusTwo = lookaheadMinusOne;
            lookaheadMinusOne = ch;
            text += static_cast<char>(ch);
            input.consume();
        }
        lastnonspacetoken = lookaheadMinusOne;
        inputState->column += (int) n;

        _returnToken = makeToken(testLiteralsTable(NAME));
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

        // as the WS rule: a tab is the last non-space character
        for (int ch = input.LA(1); ch == ' ' || ch == '\t'; ch = input.LA(1)) {
            text += static_cast<char>(ch);
            if (ch == '\t') {
                lastnonspacetoken = '\t';
                tab();
            } else {
                ++inputState->column;
            }
            input.consume();
        }

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
