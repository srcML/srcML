// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ANTLRTokenTypes.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Token types of the ANTLR 2.7.7 grammar-file lexer and parser (ANTLRTokenTypes).
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_ANTLRTOKENTYPES_HPP
#define ANTLR2CPP_ANTLRTOKENTYPES_HPP

namespace antlrcpp {

enum ANTLRTokenType {
    ANTLR_EOF = 1,
    ANTLR_LITERAL_tokens = 4,
    ANTLR_LITERAL_header = 5,
    ANTLR_STRING_LITERAL = 6,
    ANTLR_ACTION = 7,
    ANTLR_DOC_COMMENT = 8,
    ANTLR_LITERAL_lexclass = 9,
    ANTLR_LITERAL_class = 10,
    ANTLR_LITERAL_extends = 11,
    ANTLR_LITERAL_Lexer = 12,
    ANTLR_LITERAL_TreeParser = 13,
    ANTLR_OPTIONS = 14,
    ANTLR_ASSIGN = 15,
    ANTLR_SEMI = 16,
    ANTLR_RCURLY = 17,
    ANTLR_LITERAL_charVocabulary = 18,
    ANTLR_CHAR_LITERAL = 19,
    ANTLR_INT = 20,
    ANTLR_OR = 21,
    ANTLR_RANGE = 22,
    ANTLR_TOKENS = 23,
    ANTLR_TOKEN_REF = 24,
    ANTLR_OPEN_ELEMENT_OPTION = 25,
    ANTLR_CLOSE_ELEMENT_OPTION = 26,
    ANTLR_LPAREN = 27,
    ANTLR_RPAREN = 28,
    ANTLR_LITERAL_Parser = 29,
    ANTLR_LITERAL_protected = 30,
    ANTLR_LITERAL_public = 31,
    ANTLR_LITERAL_private = 32,
    ANTLR_BANG = 33,
    ANTLR_ARG_ACTION = 34,
    ANTLR_LITERAL_returns = 35,
    ANTLR_COLON = 36,
    ANTLR_LITERAL_throws = 37,
    ANTLR_COMMA = 38,
    ANTLR_LITERAL_exception = 39,
    ANTLR_LITERAL_catch = 40,
    ANTLR_RULE_REF = 41,
    ANTLR_NOT_OP = 42,
    ANTLR_SEMPRED = 43,
    ANTLR_TREE_BEGIN = 44,
    ANTLR_QUESTION = 45,
    ANTLR_STAR = 46,
    ANTLR_PLUS = 47,
    ANTLR_IMPLIES = 48,
    ANTLR_CARET = 49,
    ANTLR_WILDCARD = 50,
    ANTLR_LITERAL_options = 51,
    ANTLR_WS = 52,
    ANTLR_COMMENT = 53,
    ANTLR_SL_COMMENT = 54,
    ANTLR_ML_COMMENT = 55,
};

} // namespace antlrcpp

#endif
