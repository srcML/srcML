// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Tokdef.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of ANTLRTokdefLexer and ANTLRTokdefParser (tokdef.g).
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "Tokdef.hpp"
#include "Grammar.hpp"
#include "Scanner.hpp"

namespace antlrcpp {

namespace {

enum TokdefType {
    TD_ID = 4,
    TD_STRING = 5,
    TD_ASSIGN = 6,
    TD_LPAREN = 7,
    TD_RPAREN = 8,
    TD_INT = 9,
};

class TokdefLexer : public CharScanner {
public:
    using CharScanner::CharScanner;

    TokenRef nextToken() {
        for (;;) {
            resetText();
            int c = LA(1);
            if (c == EOF_CHAR)
                return makeToken(TokenType::EOF_TYPE);
            int ttype = TokenType::SKIP;
            if (c == ' ' || c == '\t') {
                consume();
            } else if (c == '\r') {
                consume();
                if (LA(1) == '\n')
                    consume();
                newline();
            } else if (c == '\n') {
                consume();
                newline();
            } else if (c == '/' && LA(2) == '/') {
                match("//");
                while (LA(1) != '\n' && LA(1) != '\r' && LA(1) != EOF_CHAR)
                    consume();
                if (LA(1) == '\n') {
                    consume();
                } else {
                    match('\r');
                    if (LA(1) == '\n')
                        consume();
                }
                newline();
            } else if (c == '/' && LA(2) == '*') {
                match("/*");
                while (!(LA(1) == '*' && LA(2) == '/')) {
                    if (LA(1) == EOF_CHAR)
                        throwNoViable();
                    if (LA(1) == '\n')
                        newline();
                    consume();
                }
                match("*/");
            } else if (c == '(') {
                consume();
                ttype = TD_LPAREN;
            } else if (c == ')') {
                consume();
                ttype = TD_RPAREN;
            } else if (c == '=') {
                consume();
                ttype = TD_ASSIGN;
            } else if (c == '"') {
                consume();
                while (LA(1) != '"') {
                    if (LA(1) == EOF_CHAR)
                        throwNoViable();
                    if (LA(1) == '\\')
                        consume();
                    consume();
                }
                consume();
                ttype = TD_STRING;
            } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                consume();
                while ((LA(1) >= 'a' && LA(1) <= 'z') || (LA(1) >= 'A' && LA(1) <= 'Z') || LA(1) == '_' ||
                       (LA(1) >= '0' && LA(1) <= '9'))
                    consume();
                ttype = TD_ID;
            } else if (c >= '0' && c <= '9') {
                while (LA(1) >= '0' && LA(1) <= '9')
                    consume();
                ttype = TD_INT;
            } else {
                throwNoViable();
            }
            if (ttype != TokenType::SKIP) {
                auto t = makeToken(ttype);
                t->text = text;
                return t;
            }
        }
    }
};

} // namespace

void readTokdef(const std::string& input, const std::string& filename, TokenManager* tm, Tool*) {
    TokdefLexer lexer(input);
    lexer.filename = filename;
    TokenStreamParser p([&lexer]() { return lexer.nextToken(); });
    p.filename = filename;
    p.tokenNames = { "<0>", "EOF", "<2>", "NULL_TREE_LOOKAHEAD", "ID", "STRING", "ASSIGN", "LPAREN", "RPAREN", "INT" };

    // file : name:ID (line)* ;
    p.match(TD_ID);
    while (p.LA(1) == TD_ID || p.LA(1) == TD_STRING) {
        TokenRef t;
        TokenRef s;
        TokenRef para;
        if (p.LA(1) == TD_STRING) {
            s = p.LT(1);
            p.match(TD_STRING);
        } else if (p.LA(1) == TD_ID && p.LA(2) == TD_ASSIGN && p.LA(3) == TD_STRING) {
            t = p.LT(1);
            p.match(TD_ID);
            p.match(TD_ASSIGN);
            s = p.LT(1);
            p.match(TD_STRING);
        } else if (p.LA(1) == TD_ID && p.LA(2) == TD_LPAREN) {
            t = p.LT(1);
            p.match(TD_ID);
            p.match(TD_LPAREN);
            para = p.LT(1);
            p.match(TD_STRING);
            p.match(TD_RPAREN);
        } else if (p.LA(1) == TD_ID && p.LA(2) == TD_ASSIGN && p.LA(3) == TD_INT) {
            t = p.LT(1);
            p.match(TD_ID);
        } else {
            p.throwNoViable();
        }
        p.match(TD_ASSIGN);
        TokenRef i = p.LT(1);
        p.match(TD_INT);
        int value = std::stoi(i->text);
        if (s) {
            tm->define(s->text, value);
            if (t) {
                auto sl = static_cast<StringLiteralSymbol*>(tm->getTokenSymbol(s->text));
                sl->label = t->text;
                tm->mapToTokenSymbol(t->text, sl);
            }
        } else if (t) {
            tm->define(t->text, value);
            if (para) {
                TokenSymbol* ts = tm->getTokenSymbol(t->text);
                ts->paraphrase = para->text;
            }
        }
    }
    p.match(TokenType::EOF_TYPE);
}

} // namespace antlrcpp
