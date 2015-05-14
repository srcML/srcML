/**
 * @file srcml_bitset_token_sets.hpp
 * 
 * @copyright Copyright (C) 2014  srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML translator.
 *
 * The srcML translator is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML translator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML translator; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
#ifndef SRCML_BITSET_TOKEN_SETS_HPP
#define SRCML_BITSET_TOKEN_SETS_HPP

#include <bitset_bucket_sorter.hpp>

create_token_set(keyword_name_tokens, srcMLParser::LPAREN, srcMLParser::RCURLY, srcMLParser::EQUAL, srcMLParser::TEMPOPS, srcMLParser::TEMPOPE, srcMLParser::DESTOP,
                                  srcMLParser::OPERATORS, srcMLParser::PERIOD, srcMLParser::DOTDEREF, srcMLParser::TRETURN, srcMLParser::MPDEREF, srcMLParser::RPAREN,
                                  srcMLParser::LBRACKET, srcMLParser::RBRACKET, srcMLParser::TERMINATE, srcMLParser::COLON, srcMLParser::COMMA, srcMLParser::MULTOPS,
                                  srcMLParser::QMARK, srcMLParser::BAR, srcMLParser::REFOPS, srcMLParser::RVALUEREF);


const antlr::BitSet srcMLParser::keyword_name_token_set(bitset_buckets<keyword_name_tokens>::data, bitset_buckets<keyword_name_tokens>::num_token_longs);

create_token_set(keyword_tokens, srcMLParser::DO, srcMLParser::RETURN, srcMLParser::DEFAULT, srcMLParser::UNSAFE, srcMLParser::WHILE, srcMLParser::CASE,
                                            srcMLParser::SWITCH, srcMLParser::CATCH, srcMLParser::IMPORT, srcMLParser::ASM, srcMLParser::TYPEDEF, srcMLParser::CHECKED,
                                            srcMLParser::GOTO, srcMLParser::FOR, srcMLParser::PACKAGE, srcMLParser::LOCK, srcMLParser::NAMESPACE, srcMLParser::YIELD,
                                            srcMLParser::ELSE, srcMLParser::TRY, srcMLParser::FINALLY, srcMLParser::FOREACH, srcMLParser::FIXED, srcMLParser::IF,
                                            srcMLParser::UNCHECKED, srcMLParser::BREAK, srcMLParser::CONTINUE, srcMLParser::TEMPLATE, srcMLParser::USING,
                                            srcMLParser::THROW, srcMLParser::ASSERT, srcMLParser::MACRO_CASE, srcMLParser::FOREVER, srcMLParser::STATIC_ASSERT,
                                            srcMLParser::CXX_CATCH, srcMLParser::CXX_TRY);

const antlr::BitSet srcMLParser::keyword_token_set(bitset_buckets<keyword_tokens>::data, bitset_buckets<keyword_tokens>::num_token_longs);

create_token_set(macro_call_tokens, srcMLParser::ELIF, srcMLParser::GROUP, srcMLParser::JOIN, srcMLParser::REGION, srcMLParser::LINE, srcMLParser::FINAL,
                                            srcMLParser::SELECT, srcMLParser::SET, srcMLParser::GET, srcMLParser::ASCENDING, srcMLParser::OVERRIDE, srcMLParser::BY,
                                            srcMLParser::DEFINE, srcMLParser::ORDERBY, srcMLParser::UNDEF, srcMLParser::CHECKED, srcMLParser::INTO, srcMLParser::EQUALS,
                                            srcMLParser::YIELD, srcMLParser::ADD, srcMLParser::DESCENDING, srcMLParser::PRAGMA, srcMLParser::ENDIF, srcMLParser::ASYNC,
                                            srcMLParser::INCLUDE, srcMLParser::WHERE, srcMLParser::NAME, srcMLParser::ON, srcMLParser::FROM, srcMLParser::ERRORPREC,
                                            srcMLParser::ENDREGION, srcMLParser::THIS, srcMLParser::SIGNAL, srcMLParser::REMOVE, srcMLParser::LET, srcMLParser::IFDEF,
                                            srcMLParser::IFNDEF, srcMLParser::SUPER, srcMLParser::UNCHECKED, srcMLParser::VOID, srcMLParser::CRESTRICT, srcMLParser::ASM,
                                            srcMLParser::MUTABLE, srcMLParser::CXX_CATCH, srcMLParser::CXX_TRY, srcMLParser::CXX_CLASS);

const antlr::BitSet srcMLParser::macro_call_token_set(bitset_buckets<macro_call_tokens>::data, bitset_buckets<macro_call_tokens>::num_token_longs);
#ifdef SRCML_BOOST_MPL_LARGE
create_token_set(argument_tokens, srcMLParser::VOLATILE, srcMLParser::REGISTER, srcMLParser::LBRACKET, srcMLParser::PROTECTED, srcMLParser::LINE, srcMLParser::BY, srcMLParser::DEFINE,
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
                                            srcMLParser::CXX_CLASS, srcMLParser::ATREQUIRED, srcMLParser::ATOPTIONAL, srcMLParser::BLOCK, srcMLParser::WEAK, srcMLParser::STRONG);

const antlr::BitSet srcMLParser::argument_token_set(bitset_buckets<argument_tokens>::data, bitset_buckets<argument_tokens>::num_token_longs);
#else
create_token_set(argument_tokens_one, srcMLParser::VOLATILE, srcMLParser::LBRACKET, srcMLParser::PROTECTED, srcMLParser::LINE, srcMLParser::BY, srcMLParser::DEFINE,
                                            srcMLParser::CHECKED, srcMLParser::ENUM, srcMLParser::ENDIF, srcMLParser::WHERE, srcMLParser::ON, srcMLParser::PARTIAL,
                                            srcMLParser::ENDREGION, srcMLParser::THIS, srcMLParser::REGION, srcMLParser::THREAD_LOCAL, srcMLParser::TRANSIENT, srcMLParser::MAIN,
                                            srcMLParser::GROUP, srcMLParser::SYNCHRONIZED, srcMLParser::UNSAFE, srcMLParser::STATIC, srcMLParser::MUTABLE, srcMLParser::DCOLON,
                                            srcMLParser::FINAL, srcMLParser::SELECT, srcMLParser::GET, srcMLParser::EXPLICIT, srcMLParser::READONLY, srcMLParser::LET,
                                            srcMLParser::ORDERBY, srcMLParser::RVALUEREF, srcMLParser::UNDEF, srcMLParser::UNION, srcMLParser::EQUALS, srcMLParser::VIRTUAL,
                                            srcMLParser::OPERATOR, srcMLParser::INCLUDE, srcMLParser::ERRORPREC, srcMLParser::DOTDOTDOT, srcMLParser::REMOVE, srcMLParser::PUBLIC,
                                            srcMLParser::DELEGATE, srcMLParser::IFNDEF, srcMLParser::UNCHECKED, srcMLParser::LPAREN, srcMLParser::DEFAULT, srcMLParser::DESTOP,
                                            srcMLParser::EXTERN, srcMLParser::NEW);

const antlr::BitSet srcMLParser::argument_token_set_one(bitset_buckets<argument_tokens_one>::data, bitset_buckets<argument_tokens_one>::num_token_longs);

create_token_set(argument_tokens_two, srcMLParser::REF, srcMLParser::NATIVE, srcMLParser::SET, srcMLParser::ASCENDING, srcMLParser::STRICTFP, srcMLParser::FRIEND,
                                            srcMLParser::OUT, srcMLParser::ADD, srcMLParser::DESCENDING, srcMLParser::ASYNC, srcMLParser::JOIN, srcMLParser::NAME,
                                            srcMLParser::QMARK, srcMLParser::INTERNAL, srcMLParser::PARAMS, srcMLParser::INLINE, srcMLParser::EVENT, srcMLParser::ELIF,
                                            srcMLParser::CONST, srcMLParser::ABSTRACT, srcMLParser::REFOPS, srcMLParser::MULTOPS, srcMLParser::PRIVATE, srcMLParser::OVERRIDE,
                                            srcMLParser::ATSIGN, srcMLParser::CLASS, srcMLParser::INTO, srcMLParser::YIELD, srcMLParser::TEMPOPS, srcMLParser::PRAGMA, 
                                            srcMLParser::IN, srcMLParser::IMPLICIT, srcMLParser::FROM, srcMLParser::STRUCT, srcMLParser::SIGNAL, srcMLParser::CONSTEXPR,
                                            srcMLParser::SEALED, srcMLParser::IFDEF, srcMLParser::SUPER, srcMLParser::RESTRICT, srcMLParser::ALIGNAS, srcMLParser::VOID,
                                            srcMLParser::DECLTYPE, srcMLParser::TYPENAME, srcMLParser::MACRO_TYPE_NAME, srcMLParser::MACRO_SPECIFIER, srcMLParser::TEMPLATE, srcMLParser::CRESTRICT,
                                            srcMLParser::COMPLEX, srcMLParser::ATOMIC);

const antlr::BitSet srcMLParser::argument_token_set_two(bitset_buckets<argument_tokens_two>::data, bitset_buckets<argument_tokens_two>::num_token_longs);

create_token_set(argument_tokens_three, srcMLParser::NORETURN, srcMLParser::IMAGINARY, srcMLParser::GENERIC_SELECTION, srcMLParser::ASM,
                                            srcMLParser::CXX_TRY, srcMLParser::CXX_CATCH, srcMLParser::CXX_CLASS, srcMLParser::ATREQUIRED, srcMLParser::ATOPTIONAL,
                                            srcMLParser::BLOCK, srcMLParser::WEAK, srcMLParser::STRONG, srcMLParser::REGISTER);

const antlr::BitSet srcMLParser::argument_token_set_three(bitset_buckets<argument_tokens_three>::data, bitset_buckets<argument_tokens_three>::num_token_longs);
#endif

create_token_set(enum_preprocessing_tokens, srcMLParser::EXTERN, srcMLParser::RESTRICT, srcMLParser::CONSTEXPR, srcMLParser::THREAD_LOCAL, srcMLParser::ALIGNAS,
                                            srcMLParser::INLINE, srcMLParser::MACRO_SPECIFIER, srcMLParser::PUBLIC, srcMLParser::PRIVATE, srcMLParser::PROTECTED,
                                            srcMLParser::VIRTUAL, srcMLParser::FRIEND, srcMLParser::EXPLICIT, srcMLParser::NEW, srcMLParser::STATIC, srcMLParser::CONST,
                                            srcMLParser::MUTABLE, srcMLParser::VOLATILE, srcMLParser::REGISTER, srcMLParser::TRANSIENT, srcMLParser::FINAL, srcMLParser::ABSTRACT, srcMLParser::SYNCHRONIZED,
                                            srcMLParser::NATIVE, srcMLParser::STRICTFP, srcMLParser::REF, srcMLParser::OUT, srcMLParser::IN, srcMLParser::INTERNAL, srcMLParser::SEALED,
                                            srcMLParser::OVERRIDE, srcMLParser::IMPLICIT, srcMLParser::DELEGATE, srcMLParser::UNSAFE, srcMLParser::READONLY, srcMLParser::PARTIAL,
                                            srcMLParser::EVENT, srcMLParser::ASYNC, srcMLParser::PARAMS, srcMLParser::CRESTRICT, srcMLParser::COMPLEX, srcMLParser::NORETURN,
                                            srcMLParser::IMAGINARY, srcMLParser::ENUM, srcMLParser::BLOCK, srcMLParser::WEAK, srcMLParser::STRONG);

const antlr::BitSet srcMLParser::enum_preprocessing_token_set(bitset_buckets<enum_preprocessing_tokens>::data, bitset_buckets<enum_preprocessing_tokens>::num_token_longs);

create_token_set(literal_tokens, srcMLParser::CHAR_START, srcMLParser::COMPLEX_NUMBER, srcMLParser::STRING_START, srcMLParser::CONSTANTS, srcMLParser::FALSE, srcMLParser::TRUE, srcMLParser::NULLPTR,
                                 srcMLParser::NULLLITERAL, srcMLParser::NIL);

const antlr::BitSet srcMLParser::literal_tokens_set(bitset_buckets<literal_tokens>::data, bitset_buckets<literal_tokens>::num_token_longs);

create_token_set(modifier_tokens, srcMLParser::MULTOPS, srcMLParser::REFOPS, srcMLParser::RVALUEREF, srcMLParser::QMARK, srcMLParser::BLOCKOP);

const antlr::BitSet srcMLParser::modifier_tokens_set(bitset_buckets<modifier_tokens>::data, bitset_buckets<modifier_tokens>::num_token_longs);


create_token_set(skip_tokens, srcMLParser::WS, srcMLParser::CONTROL_CHAR, srcMLParser::EOL_BACKSLASH, srcMLParser::COMMENT_START, srcMLParser::COMMENT_END, srcMLParser::LINECOMMENT_END, srcMLParser::COMMENT_TEXT, 
                srcMLParser::LINECOMMENT_START, srcMLParser::JAVADOC_COMMENT_START, srcMLParser::DOXYGEN_COMMENT_START, srcMLParser::LINE_DOXYGEN_COMMENT_START, srcMLParser::EOL);

const antlr::BitSet srcMLParser::skip_tokens_set(bitset_buckets<skip_tokens>::data, bitset_buckets<skip_tokens>::num_token_longs);

create_token_set(class_tokens, srcMLParser::CLASS, srcMLParser::CXX_CLASS, srcMLParser::STRUCT, srcMLParser::UNION, srcMLParser::ENUM);

const antlr::BitSet srcMLParser::class_tokens_set(bitset_buckets<class_tokens>::data, bitset_buckets<class_tokens>::num_token_longs);

create_token_set(decl_specifier_tokens, 

    // function decl and other
    srcMLParser::VIRTUAL, srcMLParser::STATIC, srcMLParser::INLINE, srcMLParser::EXTERN, srcMLParser::TEMPLATE, srcMLParser::CONSTEXPR, srcMLParser::NORETURN, srcMLParser::PUBLIC, srcMLParser::PRIVATE, srcMLParser::PROTECTED,
    srcMLParser::DEFAULT, srcMLParser::SYNCHRONIZED, srcMLParser::STRICTFP, srcMLParser::NATIVE,
    srcMLParser::OVERRIDE, srcMLParser::NEW, srcMLParser::DELEGATE, srcMLParser::ASYNC, srcMLParser::ABSTRACT, srcMLParser::PARTIAL, srcMLParser::SEALED, srcMLParser::IMPLICIT, srcMLParser::EXPLICIT,

    // decl_stmt
    srcMLParser::TRANSIENT, srcMLParser::INTERNAL
    );

const antlr::BitSet srcMLParser::decl_specifier_tokens_set(bitset_buckets<decl_specifier_tokens>::data, bitset_buckets<decl_specifier_tokens>::num_token_longs);

#endif
