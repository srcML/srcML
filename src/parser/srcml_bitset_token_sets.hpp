// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_bitset_token_sets.hpp
 *
 * @copyright Copyright (C) 2014-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML translator.
 */

#ifndef SRCML_BITSET_TOKEN_SETS_HPP
#define SRCML_BITSET_TOKEN_SETS_HPP

template <typename T>
constexpr unsigned long convert(const T /* base */) {
    return 0;
}

template<typename T, typename... Args>
constexpr unsigned long convert(const int pos, T value, Args... args) {
    return ((value < 32 * (pos + 1)) && (value >= 32 * pos) ? (1 << (value - 32 * pos)) : 0UL) | convert(pos, args...);
}

#define token_set(CLASS, MEMBER, ...) const unsigned long MEMBER ## _foo[] = { convert(0, __VA_ARGS__), convert(1, __VA_ARGS__), convert(2, __VA_ARGS__), convert(3, __VA_ARGS__), convert(4, __VA_ARGS__), convert(5, __VA_ARGS__), convert(6, __VA_ARGS__), convert(7, __VA_ARGS__), \
convert(8, __VA_ARGS__), convert(9, __VA_ARGS__), convert(10, __VA_ARGS__), convert(11, __VA_ARGS__), convert(12, __VA_ARGS__), convert(13, __VA_ARGS__), convert(14, __VA_ARGS__), convert(15, __VA_ARGS__) }; const antlr::BitSet CLASS::MEMBER(MEMBER ## _foo, 16);

token_set(srcMLParser, keyword_name_token_set,
    srcMLParser::LPAREN, srcMLParser::RCURLY, srcMLParser::EQUAL, srcMLParser::TEMPOPS, srcMLParser::TEMPOPE, srcMLParser::DESTOP,
    srcMLParser::OPERATORS, srcMLParser::PERIOD, srcMLParser::DOTDEREF, srcMLParser::TRETURN, srcMLParser::MPDEREF, srcMLParser::RPAREN,
    srcMLParser::LBRACKET, srcMLParser::RBRACKET, srcMLParser::TERMINATE, srcMLParser::COLON, srcMLParser::COMMA, srcMLParser::MULTOPS,
    srcMLParser::QMARK, srcMLParser::BAR, srcMLParser::REFOPS, srcMLParser::RVALUEREF
)

token_set(srcMLParser, keyword_token_set,
    srcMLParser::DO, srcMLParser::RETURN, srcMLParser::DEFAULT, srcMLParser::UNSAFE, srcMLParser::WHILE, srcMLParser::CASE,
    srcMLParser::SWITCH, srcMLParser::CATCH, srcMLParser::IMPORT, srcMLParser::ASM, srcMLParser::TYPEDEF, srcMLParser::CHECKED,
    srcMLParser::GOTO, srcMLParser::FOR, srcMLParser::PACKAGE, srcMLParser::LOCK, srcMLParser::NAMESPACE, srcMLParser::YIELD,
    srcMLParser::ELSE, srcMLParser::TRY, srcMLParser::FINALLY, srcMLParser::FOREACH, srcMLParser::FIXED, srcMLParser::IF,
    srcMLParser::UNCHECKED, srcMLParser::BREAK, srcMLParser::CONTINUE, srcMLParser::TEMPLATE, srcMLParser::USING,
    srcMLParser::THROW, srcMLParser::ASSERT, srcMLParser::MACRO_CASE, srcMLParser::FOREVER, srcMLParser::STATIC_ASSERT,
    srcMLParser::CXX_CATCH, srcMLParser::CXX_TRY, srcMLParser::PY_2_EXEC, srcMLParser::PY_2_PRINT, srcMLParser::PY_ALIAS,
    srcMLParser::PY_ASYNC, srcMLParser::PY_CASE, srcMLParser::PY_FUNCTION, srcMLParser::PY_DELETE, srcMLParser::PY_ELIF,
    srcMLParser::PY_EXCEPT, srcMLParser::PY_FROM, srcMLParser::PY_GLOBAL, srcMLParser::PY_IMPORT, srcMLParser::PY_IN,
    srcMLParser::PY_LAMBDA, srcMLParser::PY_MATCH, srcMLParser::PY_NONLOCAL, srcMLParser::PY_PASS, srcMLParser::PY_RAISE,
    srcMLParser::PY_TYPE, srcMLParser::PY_WITH, srcMLParser::PY_YIELD
)

token_set(srcMLParser, macro_call_token_set,
    srcMLParser::ELIF, srcMLParser::GROUP, srcMLParser::JOIN, srcMLParser::REGION, srcMLParser::LINE, srcMLParser::FINAL,
    srcMLParser::SELECT, srcMLParser::SET, srcMLParser::GET, srcMLParser::ASCENDING, srcMLParser::OVERRIDE, srcMLParser::BY,
    srcMLParser::DEFINE, srcMLParser::ORDERBY, srcMLParser::UNDEF, srcMLParser::CHECKED, srcMLParser::INTO, srcMLParser::EQUALS,
    srcMLParser::YIELD, srcMLParser::ADD, srcMLParser::DESCENDING, srcMLParser::PRAGMA, srcMLParser::ENDIF, srcMLParser::ASYNC,
    srcMLParser::INCLUDE, srcMLParser::WHERE, srcMLParser::NAME, srcMLParser::ON, srcMLParser::FROM, srcMLParser::ERRORPREC,
    srcMLParser::ENDREGION, srcMLParser::THIS, srcMLParser::SIGNAL, srcMLParser::REMOVE, srcMLParser::LET, srcMLParser::IFDEF,
    srcMLParser::IFNDEF, srcMLParser::SUPER, srcMLParser::UNCHECKED, srcMLParser::VOID, srcMLParser::CRESTRICT, srcMLParser::ASM,
    srcMLParser::MUTABLE, srcMLParser::CXX_CATCH, srcMLParser::CXX_TRY, srcMLParser::CXX_CLASS
)

token_set(srcMLParser, argument_token_set, srcMLParser::C_ATTRIBUTE,
    srcMLParser::VOLATILE, srcMLParser::REGISTER, srcMLParser::LBRACKET, srcMLParser::PROTECTED, srcMLParser::LINE, srcMLParser::BY, srcMLParser::DEFINE,
    srcMLParser::CHECKED, srcMLParser::ENUM, srcMLParser::ENDIF, srcMLParser::WHERE, srcMLParser::ON, srcMLParser::PARTIAL,
    srcMLParser::ENDREGION, srcMLParser::THIS, srcMLParser::REGION, srcMLParser::THREAD_LOCAL, srcMLParser::TRANSIENT, srcMLParser::MAIN,
    srcMLParser::GROUP, srcMLParser::SYNCHRONIZED, srcMLParser::UNSAFE, srcMLParser::STATIC, srcMLParser::MUTABLE, srcMLParser::DCOLON,
    srcMLParser::FINAL, srcMLParser::SELECT, srcMLParser::GET, srcMLParser::EXPLICIT, srcMLParser::READONLY, srcMLParser::LET,
    srcMLParser::ORDERBY, srcMLParser::RVALUEREF, srcMLParser::UNDEF, srcMLParser::UNION, srcMLParser::EQUALS, srcMLParser::VIRTUAL,
    srcMLParser::OPERATOR, srcMLParser::INCLUDE, srcMLParser::ERRORPREC, srcMLParser::DOTDOTDOT, srcMLParser::REMOVE, srcMLParser::PUBLIC,
    srcMLParser::DELEGATE, srcMLParser::IFNDEF, srcMLParser::UNCHECKED, srcMLParser::LPAREN, srcMLParser::DEFAULT, srcMLParser::DESTOP,
    srcMLParser::EXTERN, srcMLParser::NEW, srcMLParser::REF, srcMLParser::NATIVE, srcMLParser::SET, srcMLParser::ASCENDING,
    srcMLParser::STRICTFP, srcMLParser::FRIEND, srcMLParser::OUT, srcMLParser::ADD, srcMLParser::DESCENDING, srcMLParser::ASYNC,
    srcMLParser::JOIN, srcMLParser::NAME, srcMLParser::QMARK, srcMLParser::INTERNAL, srcMLParser::PARAMS, srcMLParser::INLINE,
    srcMLParser::EVENT, srcMLParser::ELIF, srcMLParser::CONST, srcMLParser::ABSTRACT, srcMLParser::REFOPS, srcMLParser::MULTOPS,
    srcMLParser::PRIVATE, srcMLParser::OVERRIDE, srcMLParser::ATSIGN, srcMLParser::CLASS, srcMLParser::INTO, srcMLParser::YIELD,
    srcMLParser::TEMPOPS, srcMLParser::PRAGMA, srcMLParser::IN, srcMLParser::IMPLICIT, srcMLParser::FROM, srcMLParser::STRUCT,
    srcMLParser::SIGNAL, srcMLParser::CONSTEXPR, srcMLParser::SEALED, srcMLParser::IFDEF, srcMLParser::SUPER, srcMLParser::RESTRICT,
    srcMLParser::ALIGNAS, srcMLParser::VOID, srcMLParser::DECLTYPE, srcMLParser::TYPENAME, srcMLParser::MACRO_TYPE_NAME,
    srcMLParser::MACRO_SPECIFIER, srcMLParser::TEMPLATE, srcMLParser::CRESTRICT, srcMLParser::COMPLEX, srcMLParser::ATOMIC,
    srcMLParser::NORETURN, srcMLParser::IMAGINARY, srcMLParser::GENERIC_SELECTION, srcMLParser::ASM, srcMLParser::CXX_TRY, srcMLParser::CXX_CATCH,
    srcMLParser::CXX_CLASS, srcMLParser::ATREQUIRED, srcMLParser::ATOPTIONAL, srcMLParser::BLOCK, srcMLParser::WEAK, srcMLParser::STRONG
)

token_set(srcMLParser, enum_preprocessing_token_set,
    srcMLParser::EXTERN, srcMLParser::RESTRICT, srcMLParser::CONSTEXPR, srcMLParser::THREAD_LOCAL, srcMLParser::ALIGNAS,
    srcMLParser::INLINE, srcMLParser::MACRO_SPECIFIER, srcMLParser::PUBLIC, srcMLParser::PRIVATE, srcMLParser::PROTECTED,
    srcMLParser::VIRTUAL, srcMLParser::FRIEND, srcMLParser::EXPLICIT, srcMLParser::NEW, srcMLParser::STATIC, srcMLParser::CONST,
    srcMLParser::MUTABLE, srcMLParser::VOLATILE, srcMLParser::REGISTER, srcMLParser::TRANSIENT, srcMLParser::FINAL, srcMLParser::ABSTRACT, srcMLParser::SYNCHRONIZED,
    srcMLParser::NATIVE, srcMLParser::STRICTFP, srcMLParser::REF, srcMLParser::OUT, srcMLParser::IN, srcMLParser::INTERNAL, srcMLParser::SEALED,
    srcMLParser::OVERRIDE, srcMLParser::IMPLICIT, srcMLParser::DELEGATE, srcMLParser::UNSAFE, srcMLParser::READONLY, srcMLParser::PARTIAL,
    srcMLParser::EVENT, srcMLParser::ASYNC, srcMLParser::PARAMS, srcMLParser::CRESTRICT, srcMLParser::COMPLEX, srcMLParser::NORETURN,
    srcMLParser::IMAGINARY, srcMLParser::ENUM, srcMLParser::BLOCK, srcMLParser::WEAK, srcMLParser::STRONG
)

token_set(srcMLParser, literal_tokens_set,
    srcMLParser::CHAR_START, srcMLParser::COMPLEX_NUMBER, srcMLParser::STRING_START, srcMLParser::CONSTANTS, srcMLParser::LITERAL_FALSE, srcMLParser::LITERAL_TRUE, srcMLParser::NULLPTR,
    srcMLParser::NULLLITERAL, srcMLParser::NIL, srcMLParser::LITERAL_NONE, srcMLParser::LITERAL_ELLIPSIS, srcMLParser::DQUOTE_DOCSTRING_START, srcMLParser::SQUOTE_DOCSTRING_START
)

token_set(srcMLParser, modifier_tokens_set,
    srcMLParser::MULTOPS, srcMLParser::REFOPS, srcMLParser::RVALUEREF, srcMLParser::QMARK, srcMLParser::BLOCKOP
)

token_set(srcMLParser, skip_tokens_set,
    srcMLParser::WS, srcMLParser::CONTROL_CHAR, srcMLParser::EOL_BACKSLASH, srcMLParser::BLOCK_COMMENT_START, srcMLParser::BLOCK_COMMENT_END, srcMLParser::LINE_COMMENT_END, srcMLParser::COMMENT_TEXT,
    srcMLParser::LINE_COMMENT_START, srcMLParser::JAVADOC_COMMENT_START, srcMLParser::DOXYGEN_COMMENT_START, srcMLParser::LINE_DOXYGEN_COMMENT_START, srcMLParser::EOL, srcMLParser::WHOLE_COMMENT,
    srcMLParser::HASHTAG_COMMENT_START, srcMLParser::HASHTAG_COMMENT_END, srcMLParser::HASHBANG_COMMENT_START, srcMLParser::HASHBANG_COMMENT_END, srcMLParser::WS_EOL
)

token_set(srcMLParser, class_tokens_set,
    srcMLParser::CLASS, srcMLParser::CXX_CLASS, srcMLParser::STRUCT, srcMLParser::UNION, srcMLParser::ENUM
)

token_set(srcMLParser, decl_specifier_tokens_set,

    // function decl and other
    srcMLParser::VIRTUAL, srcMLParser::STATIC, srcMLParser::INLINE, srcMLParser::EXTERN, srcMLParser::TEMPLATE, srcMLParser::CONSTEXPR, srcMLParser::NORETURN, srcMLParser::PUBLIC, srcMLParser::PRIVATE, srcMLParser::PROTECTED,
    srcMLParser::DEFAULT, srcMLParser::SYNCHRONIZED, srcMLParser::STRICTFP, srcMLParser::NATIVE,
    srcMLParser::OVERRIDE, srcMLParser::NEW, srcMLParser::DELEGATE, srcMLParser::ASYNC, srcMLParser::ABSTRACT, srcMLParser::PARTIAL, srcMLParser::SEALED, srcMLParser::IMPLICIT, srcMLParser::EXPLICIT,
    srcMLParser::FINAL,

    // decl_stmt
    srcMLParser::TRANSIENT, srcMLParser::INTERNAL

)

token_set(srcMLParser, identifier_list_tokens_set,

    srcMLParser::NAME, srcMLParser::INCLUDE, srcMLParser::DEFINE, srcMLParser::ELIF, srcMLParser::ENDIF, srcMLParser::ERRORPREC, srcMLParser::IFDEF, srcMLParser::IFNDEF, srcMLParser::LINE,
    srcMLParser::PRAGMA, srcMLParser::UNDEF, srcMLParser::WARNING, srcMLParser::SUPER, srcMLParser::REGION, srcMLParser::ENDREGION, srcMLParser::GET, srcMLParser::SET, srcMLParser::ADD,
    srcMLParser::REMOVE, srcMLParser::ASYNC, srcMLParser::YIELD, srcMLParser::FINAL, srcMLParser::OVERRIDE, srcMLParser::VOID, srcMLParser::ASM,

    // C# linq
    srcMLParser::FROM, srcMLParser::WHERE, srcMLParser::SELECT, srcMLParser::LET, srcMLParser::ORDERBY, srcMLParser::ASCENDING, srcMLParser::DESCENDING, srcMLParser::GROUP, srcMLParser::BY,
    srcMLParser::JOIN, srcMLParser::ON, srcMLParser::EQUALS, srcMLParser::INTO, srcMLParser::THIS, srcMLParser::ALIAS,

    // Objective-C
    srcMLParser::IMPORT, srcMLParser::ATPROTOCOL,

    // C
    srcMLParser::CRESTRICT, srcMLParser::MUTABLE, srcMLParser::CXX_TRY, srcMLParser::CXX_CATCH,

    // Java
    srcMLParser::RECORD,

    // Qt
    srcMLParser::EMIT, srcMLParser::FOREACH, srcMLParser::SIGNAL, srcMLParser::FOREVER,

    // Python
    srcMLParser::PY_2_EXEC, srcMLParser::PY_2_PRINT, srcMLParser::PY_ASYNC, srcMLParser::PY_CASE, srcMLParser::PY_MATCH, srcMLParser::PY_TYPE
)

token_set(srcMLParser, whitespace_token_set,
    srcMLParser::WS,
    srcMLParser::WS_EOL,
    srcMLParser::CONTROL_CHAR,
    srcMLParser::EOL_BACKSLASH,
    srcMLParser::WHOLE_COMMENT,
    srcMLParser::BLOCK_COMMENT_START,
    srcMLParser::BLOCK_COMMENT_END,
    srcMLParser::JAVADOC_COMMENT_END,
    srcMLParser::DOXYGEN_COMMENT_END,
    srcMLParser::LINE_COMMENT_START,
    srcMLParser::LINE_COMMENT_END,
    srcMLParser::LINE_DOXYGEN_COMMENT_START,
    srcMLParser::LINE_DOXYGEN_COMMENT_END,
    srcMLParser::COMMENT_TEXT,
    srcMLParser::JAVADOC_COMMENT_START,
    srcMLParser::DOXYGEN_COMMENT_START,
    srcMLParser::HASHBANG_COMMENT_START,
    srcMLParser::HASHBANG_COMMENT_END,
    srcMLParser::HASHTAG_COMMENT_START,
    srcMLParser::HASHTAG_COMMENT_END
)

token_set(srcMLParser, duplex_keyword_set,
    // Python
    srcMLParser::PY_EXCEPT,
    srcMLParser::PY_YIELD,

    // JavaScript
    srcMLParser::JS_YIELD
)

token_set(srcMLParser, keyword_name_token_set_py,
    srcMLParser::BAR, srcMLParser::DESTOP, srcMLParser::DOTDEREF, srcMLParser::ELSE, srcMLParser::EQUAL,
    srcMLParser::EXPONENTIATION, srcMLParser::IF, srcMLParser::LBRACKET, srcMLParser::LITERAL_ELLIPSIS,
    srcMLParser::LPAREN, srcMLParser::MULTOPS, srcMLParser::OPERATORS, srcMLParser::PERIOD, srcMLParser::PY_AND,
    srcMLParser::PY_ARROW, srcMLParser::PY_ATSIGN, srcMLParser::PY_AWAIT, srcMLParser::PY_COLON, srcMLParser::PY_IN,
    srcMLParser::PY_IS, srcMLParser::PY_LCURLY, srcMLParser::PY_NOT, srcMLParser::PY_OR, srcMLParser::PY_RCURLY,
    srcMLParser::QMARK, srcMLParser::RBRACKET, srcMLParser::REFOPS, srcMLParser::RVALUEREF, srcMLParser::TEMPOPE,
    srcMLParser::TEMPOPS
)

token_set(srcMLParser, expect_blocks_py_token_set,
    // Python keywords
    srcMLParser::CLASS, srcMLParser::ELSE, srcMLParser::FINALLY, srcMLParser::FOR, srcMLParser::IF,
    srcMLParser::PY_CASE, srcMLParser::PY_ELIF, srcMLParser::PY_EXCEPT, srcMLParser::PY_FUNCTION,
    srcMLParser::PY_MATCH, srcMLParser::PY_WITH, srcMLParser::TRY, srcMLParser::WHILE,

    // Python attributes/specifiers
    srcMLParser::PY_ASYNC, srcMLParser::PY_ATSIGN
)

token_set(srcMLParser, left_bracket_py_token_set,
    srcMLParser::LPAREN, srcMLParser::LBRACKET, srcMLParser::PY_LCURLY
)

token_set(srcMLParser, right_bracket_py_token_set,
    srcMLParser::RPAREN, srcMLParser::RBRACKET, srcMLParser::PY_RCURLY
)

token_set(srcMLParser, comment_py_token_set,
    // Python comments
    srcMLParser::HASHTAG_COMMENT_START, srcMLParser::HASHTAG_COMMENT_END,
    srcMLParser::HASHBANG_COMMENT_START, srcMLParser::HASHBANG_COMMENT_END
)

token_set(srcMLParser, multiline_literals_py_token_set,
    // Python string literals that can span multiple lines
    srcMLParser::STRING_START, srcMLParser::STRING_END,
    srcMLParser::CHAR_START, srcMLParser::CHAR_END,
    srcMLParser::DQUOTE_DOCSTRING_END, srcMLParser::DQUOTE_DOXYGEN_END,
    srcMLParser::SQUOTE_DOCSTRING_END, srcMLParser::SQUOTE_DOXYGEN_END
)

#endif
