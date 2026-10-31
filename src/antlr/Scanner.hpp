// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Scanner.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Minimal port of the ANTLR 2 Java runtime (CharScanner, LLkParser) used by the
 * hand-translated lexers and parsers that read grammar files, vocabulary files,
 * and actions.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_SCANNER_HPP
#define ANTLR2CPP_SCANNER_HPP

#include "BitSet.hpp"
#include "JavaHashtable.hpp"
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <functional>

namespace antlrcpp {

namespace TokenType {
    constexpr int MIN_USER_TYPE = 4;
    constexpr int NULL_TREE_LOOKAHEAD = 3;
    constexpr int INVALID_TYPE = 0;
    constexpr int EOF_TYPE = 1;
    constexpr int SKIP = -1;
}

struct Token {
    int type = TokenType::INVALID_TYPE;
    std::string text;
    int line = 0;
    int column = 0;

    Token() = default;
    Token(int t, std::string s) : type(t), text(std::move(s)) {}
};

using TokenRef = std::shared_ptr<Token>;

inline TokenRef makeTokenRef(int type, const std::string& text, int line = 0, int column = 0) {
    auto t = std::make_shared<Token>(type, text);
    t->line = line;
    t->column = column;
    return t;
}

// FileLineFormatter
inline std::string formatLocation(const OStr& fileName, int line, int column) {
    std::string buf;
    if (fileName)
        buf += *fileName + ":";
    if (line != -1) {
        if (!fileName)
            buf += "line ";
        buf += std::to_string(line);
        if (column != -1)
            buf += ":" + std::to_string(column);
        buf += ":";
    }
    buf += " ";
    return buf;
}

// thrown for syntax errors in grammar files and actions
struct RecognitionException {
    std::string message;
    std::string filename;
    int line = -1;
    int column = -1;

    std::string toString() const { return formatLocation(filename, line, column) + message; }
};

// lexer error seen by a parser
struct TokenStreamException {
    RecognitionException error;
};

// Port of antlr.CharScanner with an in-memory input buffer
class CharScanner {
public:
    static constexpr int EOF_CHAR = 0xFFFF;

    CharScanner(const std::string& input) : input(input) {}
    virtual ~CharScanner() = default;

    int LA(int i) const {
        std::size_t pos = position + static_cast<std::size_t>(i) - 1;
        if (pos >= input.size())
            return EOF_CHAR;
        return static_cast<unsigned char>(input[pos]);
    }

    void consume() {
        if (guessing == 0) {
            int c = LA(1);
            if (saveConsumedInput && c != EOF_CHAR)
                text += static_cast<char>(c);
            if (c == '\t')
                tab();
            else
                ++column;
        }
        if (position < input.size())
            ++position;
    }

    void tab() {
        int c = column;
        column = (((c - 1) / tabsize) + 1) * tabsize + 1;
    }

    void newline() {
        ++line;
        column = 1;
    }

    void match(int c) {
        if (LA(1) != c)
            throwMismatch("expecting " + charName(c) + ", found " + charName(LA(1)));
        consume();
    }

    void match(const BitSet& b) {
        if (!b.member(LA(1))) {
            std::string chars;
            for (int e : b.toArray())
                chars += charName(e);
            throwMismatch("expecting one of (" + chars + "), found " + charName(LA(1)));
        }
        consume();
    }

    void match(const std::string& s) {
        for (char c : s)
            match(static_cast<unsigned char>(c));
    }

    void matchNot(int c) {
        if (LA(1) == c)
            throwMismatch("expecting anything but '" + charName(c) + "'; got it anyway");
        consume();
    }

    void matchRange(int c1, int c2) {
        if (LA(1) < c1 || LA(1) > c2)
            throwMismatch("expecting token in range: " + charName(c1) + ".." + charName(c2) + ", found " + charName(LA(1)));
        consume();
    }

    void resetText() {
        text.clear();
        tokenStartColumn = column;
        tokenStartLine = line;
    }

    void setText(const std::string& s) {
        resetText();
        text += s;
    }

    TokenRef makeToken(int t) const {
        auto tok = std::make_shared<Token>();
        tok->type = t;
        tok->column = tokenStartColumn;
        tok->line = tokenStartLine;
        return tok;
    }

    int testLiteralsTable(int ttype) const {
        return testLiteralsTable(text, ttype);
    }

    int testLiteralsTable(const std::string& s, int ttype) const {
        auto it = literals.get(s);
        return it ? *it : ttype;
    }

    // helper for the end of each translated lexer rule
    void finishToken(bool createToken, TokenRef& token, int ttype, std::size_t begin) {
        if (createToken && !token && ttype != TokenType::SKIP) {
            token = makeToken(ttype);
            token->text = text.substr(begin);
        }
        returnToken = token;
    }

    // NoViableAltForCharException
    [[noreturn]] void throwNoViable() const {
        int c = LA(1);
        std::string msg = "unexpected char: ";
        if (c >= ' ' && c <= '~')
            msg += std::string("'") + static_cast<char>(c) + "'";
        else
            msg += "0x" + toUpperAscii(toRadix(c, 16));
        throw RecognitionException{ msg, filename, line, column };
    }

    // MismatchedCharException
    [[noreturn]] void throwMismatch(const std::string& msg) const {
        throw RecognitionException{ msg, filename, line, column };
    }

    static std::string charName(int c) {
        switch (c) {
        case EOF_CHAR: return "'<EOF>'";
        case '\n': return "'\\n'";
        case '\r': return "'\\r'";
        case '\t': return "'\\t'";
        default: return std::string("'") + static_cast<char>(c) + "'";
        }
    }

    std::string input;
    std::size_t position = 0;
    std::string text;
    int line = 1;
    int column = 1;
    int tokenStartLine = 1;
    int tokenStartColumn = 1;
    int tabsize = 8;
    int guessing = 0;
    bool saveConsumedInput = true;
    std::string filename;
    JavaHashtable<int> literals;
    TokenRef returnToken;
};

// Port of the parts of antlr.LLkParser used by the translated parsers
class TokenStreamParser {
public:
    explicit TokenStreamParser(std::function<TokenRef()> next) : nextToken(std::move(next)) {}
    virtual ~TokenStreamParser() = default;

    // lexer errors are not caught by parser rules
    TokenRef LT(int i) {
        while (static_cast<int>(buffer.size()) < i) {
            try {
                buffer.push_back(nextToken());
            } catch (const RecognitionException& e) {
                throw TokenStreamException{ e };
            }
        }
        return buffer[static_cast<std::size_t>(i - 1)];
    }

    int LA(int i) { return LT(i)->type; }

    void consume() {
        LT(1);
        buffer.pop_front();
    }

    void match(int t) {
        if (LA(1) != t)
            throwMismatch(t);
        consume();
    }

    // NoViableAltException
    [[noreturn]] void throwNoViable() {
        auto t = LT(1);
        std::string msg = "unexpected token: " + t->text;
        throw RecognitionException{ msg, filename, t->line, t->column };
    }

    // MismatchedTokenException
    [[noreturn]] void throwMismatch(int expecting) {
        auto t = LT(1);
        std::string expected = expecting >= 0 && expecting < static_cast<int>(tokenNames.size())
                                   ? tokenNames[static_cast<std::size_t>(expecting)]
                                   : "<" + std::to_string(expecting) + ">";
        throw RecognitionException{ "expecting " + expected + ", found '" + t->text + "'", filename, t->line, t->column };
    }

    std::string filename;
    std::vector<std::string> tokenNames;

private:
    std::function<TokenRef()> nextToken;
    std::deque<TokenRef> buffer;
};

} // namespace antlrcpp

#endif
