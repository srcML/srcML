/*!
 * @file KeywordLexer.g
 *
 * @copyright Copyright (C) 2004-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

header "pre_include_hpp" {
    #include <cstring>
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
    #pragma GCC diagnostic ignored "-Wunknown-warning-option"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
}

header {
    #include <string>
    #include "Language.hpp"
    #include "UTF8CharBuffer.hpp"
    #include "antlr/TokenStreamSelector.hpp"
    #include "CommentTextLexer.hpp"
    #include "srcMLToken.hpp"
    #include <srcml_types.hpp>
    #include <srcml_macros.hpp>
    #include <srcml.h>
    #undef CONST
    #undef VOID
    #undef DELETE
    #undef INTERFACE
    #undef OUT
    #undef IN
    #undef THIS
}

header "post_include_cpp" {

void KeywordLexer::changetotextlexer(int typeend, const std::string delim) {

    selector->push("text"); 
    ((CommentTextLexer* ) (selector->getStream("text")))->init(typeend, onpreprocline, atstring, delim, isline, line_number, options);
}

}

options {
    language = "Cpp";
    namespaceAntlr = "antlr";
    namespaceStd = "std";
}

class KeywordLexer extends OperatorLexer;

options {
    classHeaderSuffix="public Language";
    k = 1;
    testLiterals = false;
    noConstructors = true;
    defaultErrorHandler = false;
    importVocab = OperatorLexer;
//    codeGenBitsetTestThreshold = 20; 
}

tokens {

    // special identifier
    MAIN;

    // statements
    BREAK;
    CONTINUE;

    WHILE;
    DO;
    FOR;    

    IF;
    ELSE;

    SWITCH;
    CASE;
    DEFAULT;

    ENUM;

    // C Family
    TYPEDEF;
    GOTO;
    ASM;
    VISUAL_CXX_ASM;
    SIZEOF;
    EXTERN;
    AUTO;

    // C
    REGISTER;
    RESTRICT;
    ATOMIC;
    COMPLEX;
    GENERIC_SELECTION;
    IMAGINARY;
    NORETURN;
    STATIC_ASSERT;

    // Combined C/C++
    CRESTRICT;
    CXX_TRY;
    CXX_CATCH;
    CXX_CLASS;

    // C++
    CONSTEXPR;
    NOEXCEPT;
    THREAD_LOCAL;
    NULLPTR;
    DECLTYPE;
    ALIGNAS;
    TYPENAME;
    ALIGNOF;
    TYPEID;
    CONST_CAST;
    DYNAMIC_CAST;
    REINTERPRET_CAST;
    STATIC_CAST;

    // aggregate types
    UNION;
    STRUCT;

    // types
    VOID;

    // functions
    RETURN;

    // cpp
    INCLUDE;
    DEFINE;
    ELIF;
    ENDIF;
    ERRORPREC;
    WARNING;
    IFDEF;
    IFNDEF;
    LINE;
    PRAGMA;
    UNDEF;

    INLINE;

    // macro
    MACRO_TYPE_NAME;
    MACRO_CASE;
    MACRO_LABEL;
    MACRO_SPECIFIER;

    // exception handling
    TRY;
    CATCH;
    THROW;
    THROWS;

    // class
    CLASS;
    PUBLIC;
    PRIVATE;
    PROTECTED;
    VIRTUAL;
    FRIEND;
    OPERATOR;
    EXPLICIT;

    // Qt
    FOREVER;
    SIGNAL;
    EMIT;

    // namespaces
    NAMESPACE;
    USING;

    // templates
    TEMPLATE;

    NEW;
    DELETE;

    // specifiers
    STATIC;
    CONST;
    MUTABLE;
    VOLATILE;
    TRANSIENT;

    // Java tokens
    IMPORT;
    PACKAGE;
    FINALLY;
    EXTENDS;
    IMPLEMENTS;
    INTERFACE;
    FINAL;
    ABSTRACT;
    SUPER;
    SYNCHRONIZED;
    NATIVE;
    STRICTFP;
    NULLLITERAL;
    ASSERT;

    // C# tokens
    FOREACH;
    REF;
    OUT;
    IN;
    LOCK;
    IS;
    INTERNAL;
    SEALED;
    OVERRIDE;
    IMPLICIT;
    STACKALLOC;
    AS;
    DELEGATE;
    FIXED;
    CHECKED;
    UNCHECKED;
    REGION;
    ENDREGION;
    UNSAFE;
    READONLY;
    GET;
    SET;
    ADD;
    REMOVE;
    YIELD;
    PARTIAL;
    AWAIT;
    EVENT;
    ASYNC;
    THIS;
    PARAMS;
    TYPEOF;
    ALIAS;

    // linq
    FROM;
    WHERE;
    SELECT;
    LET;
    ORDERBY;
    ASCENDING;
    DESCENDING;
    GROUP;
    BY;
    JOIN;
    ON;
    EQUALS;
    INTO;

    // Objective-C
    ATINTERFACE;
    ATIMPLEMENTATION;
    ATEND;
    ATPROTOCOL;
    ATREQUIRED;
    ATOPTIONAL;
    PROPERTY;
    SYNTHESIZE;
    DYNAMIC;
    ENCODE;
    SELECTOR;
    ATCLASS;

    // Apple
    BLOCK;
    WEAK;
    STRONG;
    AUTORELEASEPOOL;
    COMPATIBILITY_ALIAS;
    NIL;

    // OpenMp
    OMP_OMP;

    ASSIGNMENT; // +=, -=, etc.
}

{
public:
    OPTION_TYPE & options;
    bool onpreprocline;
    bool startline;
    bool atstring;
    bool rawstring;
    std::string delimiter;
    bool isline;
    long line_number;
    int lastpos;
    int prev;
    int currentmode;

// map from text of literal to token number, adjusted to language
struct keyword { char const * const text; int token; int language; };

void changetotextlexer(int typeend, const std::string delimiter = "");

KeywordLexer(UTF8CharBuffer* pinput, int language, OPTION_TYPE & options,
             std::vector<std::string> user_macro_list)
    : antlr::CharScanner(pinput,true), Language(language), options(options), onpreprocline(false), startline(true),
    atstring(false), rawstring(false), delimiter(""), isline(false), line_number(-1), lastpos(0), prev(0)
{
    if (isoption(options, SRCML_OPTION_LINE))
       setLine(getLine() + (1 << 16));
    setTokenObjectFactory(srcMLToken::factory);

    for (std::vector<std::string>::size_type i = 0; i < user_macro_list.size(); i += 2) {
        if (user_macro_list[i + 1] == "src:macro")
            literals[user_macro_list[i].c_str()] = MACRO_NAME;
        else if (user_macro_list[i + 1] == "src:name")
            literals[user_macro_list[i].c_str()] = MACRO_TYPE_NAME;
        else if (user_macro_list[i + 1] == "src:type")
            literals[user_macro_list[i].c_str()] = MACRO_TYPE_NAME;
        else if (user_macro_list[i + 1] == "src:case")
            literals[user_macro_list[i].c_str()] = MACRO_CASE;
        else if (user_macro_list[i + 1] == "src:label")
            literals[user_macro_list[i].c_str()] = MACRO_LABEL;
        else if (user_macro_list[i + 1] == "src:specifier")
            literals[user_macro_list[i].c_str()] = MACRO_SPECIFIER;
    }

    constexpr const keyword keyword_map[] = {
        // common keywords
        { "if"           , IF            , LANGUAGE_ALL }, 
        { "else"         , ELSE          , LANGUAGE_ALL }, 

        { "while"        , WHILE         , LANGUAGE_ALL }, 
        { "for"          , FOR           , LANGUAGE_ALL }, 
        { "do"           , DO            , LANGUAGE_ALL }, 

        { "break"        , BREAK         , LANGUAGE_ALL }, 
        { "continue"     , CONTINUE      , LANGUAGE_ALL }, 

        { "switch"       , SWITCH        , LANGUAGE_ALL }, 
        { "case"         , CASE          , LANGUAGE_ALL }, 
        { "default"      , DEFAULT       , LANGUAGE_ALL }, 

        { "return"       , RETURN        , LANGUAGE_ALL }, 

        { "enum"         , ENUM          , LANGUAGE_ALL }, 

        { "static"       , STATIC        , LANGUAGE_ALL }, 
        { "const"        , CONST         , LANGUAGE_ALL }, 
 
        // operators and special characters
        { ")"            , RPAREN        , LANGUAGE_ALL }, 
        { ";"            , TERMINATE     , LANGUAGE_ALL }, 
        { "("            , LPAREN        , LANGUAGE_ALL }, 
        { "~"            , DESTOP        , LANGUAGE_ALL }, 
        { ":"            , COLON         , LANGUAGE_ALL }, 
        { "}"            , RCURLY        , LANGUAGE_ALL }, 
        { ","            , COMMA         , LANGUAGE_ALL }, 
        { "]"            , RBRACKET      , LANGUAGE_ALL }, 
        { "{"            , LCURLY        , LANGUAGE_ALL }, 
        { "["            , LBRACKET      , LANGUAGE_ALL }, 

        { "<"            , TEMPOPS       , LANGUAGE_ALL }, 
        { ">"            , TEMPOPE       , LANGUAGE_ALL },
        { "&"            , REFOPS        , LANGUAGE_ALL }, 
        { "="            , EQUAL         , LANGUAGE_ALL }, 

        { "."            , PERIOD        , LANGUAGE_ALL }, 
        { "*"            , MULTOPS       , LANGUAGE_ALL }, 
        { "*="           , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "%="           , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "/="           , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "/"            , OPERATORS     , LANGUAGE_ALL }, 
        { "^"            , BLOCKOP       , LANGUAGE_ALL }, 
        { "^="           , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "|="           , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "||="          , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "+="           , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "-="           , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "->"           , TRETURN       , LANGUAGE_ALL }, 
        { "->*"          , MPDEREF       , LANGUAGE_ALL }, 
        { "?"            , QMARK         , LANGUAGE_ALL }, 
        { ".."           , DOTDOT        , LANGUAGE_ALL }, 
        { "..."          , DOTDOTDOT     , LANGUAGE_ALL }, 
        { "&="           , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "&&="          , ASSIGNMENT    , LANGUAGE_ALL }, 
        { ">>="          , ASSIGNMENT    , LANGUAGE_ALL }, 
        { "<<="          , ASSIGNMENT    , LANGUAGE_ALL }, 
        { ".*"           , DOTDEREF      , LANGUAGE_C_FAMILY }, 
        { "-"            , MSPEC         , LANGUAGE_JAVA }, 
        { "+"            , CSPEC         , LANGUAGE_JAVA }, 
        { "<<<"          , CUDA          , LANGUAGE_C | LANGUAGE_CXX }, 
        { "=>"           , LAMBDA        , LANGUAGE_CSHARP }, 
        { "@"            , ATSIGN        , LANGUAGE_ALL }, 
        { "u\""          , STRING_START  , LANGUAGE_ALL }, 
        { "u8\""         , STRING_START  , LANGUAGE_ALL }, 
        { "U\""          , STRING_START  , LANGUAGE_ALL }, 
        { "L\""          , STRING_START  , LANGUAGE_ALL }, 
        { "R\""          , STRING_START  , LANGUAGE_CXX }, 
        { "LR\""         , STRING_START  , LANGUAGE_CXX }, 
        { "uR\""         , STRING_START  , LANGUAGE_CXX }, 
        { "UR\""         , STRING_START  , LANGUAGE_CXX }, 
        { "u8R\""        , STRING_START  , LANGUAGE_CXX }, 

        // C and C++ specific keywords
        { "main"         , MAIN           , LANGUAGE_C_FAMILY }, 

        { "typedef"      , TYPEDEF        , LANGUAGE_C_FAMILY }, 

        { "include"      , INCLUDE        , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "define"       , DEFINE         , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "elif"         , ELIF           , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "endif"        , ENDIF          , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "error"        , ERRORPREC      , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "warning"      , WARNING        , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "ifdef"        , IFDEF          , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "ifndef"       , IFNDEF         , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "line"         , LINE           , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "pragma"       , PRAGMA         , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 
        { "undef"        , UNDEF          , LANGUAGE_C_FAMILY | LANGUAGE_JAVA }, 

        { "union"        , UNION          , LANGUAGE_C | LANGUAGE_CXX }, 
        { "struct"       , STRUCT         , LANGUAGE_C_FAMILY }, 
        { "void"         , VOID           , LANGUAGE_ALL }, 

        { "inline"       , INLINE         , LANGUAGE_C_FAMILY }, 
        { "extern"       , EXTERN         , LANGUAGE_C_FAMILY }, 

        { "asm"          , ASM            , LANGUAGE_C_FAMILY },
        { "__asm__"      , ASM            , LANGUAGE_C_FAMILY },
        { "__asm"        , VISUAL_CXX_ASM , LANGUAGE_C_FAMILY }, 

        { "goto"         , GOTO           , LANGUAGE_ALL }, 
        { "sizeof"       , SIZEOF         , LANGUAGE_C_FAMILY }, 

        { "register"     , REGISTER       , LANGUAGE_C | LANGUAGE_CXX }, 
        { "mutable"      , MUTABLE        , LANGUAGE_CXX }, 
        { "volatile"     , VOLATILE       , LANGUAGE_ALL }, 
        { "__volatile__" , VOLATILE       , LANGUAGE_C_FAMILY  },
        { "auto"         , AUTO           , LANGUAGE_CXX | LANGUAGE_C },

        // C keywords
        { "restrict"       , RESTRICT          , LANGUAGE_C },
        { "_Alignas"       , ALIGNAS           , LANGUAGE_C },
        { "_Alignof"       , ALIGNOF           , LANGUAGE_C },
        { "_Atomic"        , ATOMIC            , LANGUAGE_C },
        { "_Complex"       , COMPLEX           , LANGUAGE_C },
        { "_Generic"       , GENERIC_SELECTION , LANGUAGE_C },
        { "_Imaginary"     , IMAGINARY         , LANGUAGE_C },
        { "_Noreturn"      , NORETURN          , LANGUAGE_C },
        { "_Static_assert" , STATIC_ASSERT     , LANGUAGE_C },
        { "_Thread_local"  , THREAD_LOCAL      , LANGUAGE_C },

        // exception handling
        { "try"          , TRY           , LANGUAGE_OO }, 
        { "catch"        , CATCH         , LANGUAGE_OO }, 
        { "throw"        , THROW         , LANGUAGE_OO }, 

        // class
        { "class"        , CLASS         , LANGUAGE_OO }, 
        { "public"       , PUBLIC        , LANGUAGE_OO }, 
        { "private"      , PRIVATE       , LANGUAGE_OO }, 
        { "protected"    , PROTECTED     , LANGUAGE_OO }, 

        { "new"          , NEW           , LANGUAGE_OO }, 

        // Qt
        { "signals"      , SIGNAL        , LANGUAGE_CXX }, 
        { "foreach"      , FOREACH       , LANGUAGE_CXX }, 
        { "forever"      , FOREVER       , LANGUAGE_CXX },
        { "emit"         , EMIT          , LANGUAGE_CXX },  

        // C++ specific keywords
        { "virtual"      , VIRTUAL       , LANGUAGE_CXX_FAMILY }, 
        { "friend"       , FRIEND        , LANGUAGE_CXX }, 
        { "operator"     , OPERATOR      , LANGUAGE_CXX_FAMILY }, 
        { "explicit"     , EXPLICIT      , LANGUAGE_CXX_FAMILY }, 
        
        // namespaces
        { "namespace"    , NAMESPACE     , LANGUAGE_CXX_FAMILY }, 
        { "using"        , USING         , LANGUAGE_CXX_FAMILY }, 
        
        // templates
        { "template"     , TEMPLATE      , LANGUAGE_CXX }, 
        
        { "delete"       , DELETE        , LANGUAGE_CXX }, 
        
        // special C++ operators
        { "::"           , DCOLON        , LANGUAGE_CXX_FAMILY }, 
        { "&&"           , RVALUEREF     , LANGUAGE_CXX_FAMILY }, 

        // special C++ constant values
        { "false"        , LITERAL_FALSE         , LANGUAGE_OO }, 
        { "true"         , LITERAL_TRUE          , LANGUAGE_OO }, 

        // C++ specifiers
        { "final"         , FINAL          , LANGUAGE_CXX },
        { "override"      , OVERRIDE       , LANGUAGE_CXX },
 
        // C++ specific keywords
        { "constexpr"        , CONSTEXPR        , LANGUAGE_CXX }, 
        { "noexcept"         , NOEXCEPT         , LANGUAGE_CXX }, 
        { "thread_local"     , THREAD_LOCAL     , LANGUAGE_CXX }, 
        { "nullptr"          , NULLPTR          , LANGUAGE_CXX }, 
        { "decltype"         , DECLTYPE         , LANGUAGE_CXX }, 
        { "alignas"          , ALIGNAS          , LANGUAGE_CXX }, 
        { "typename"         , TYPENAME         , LANGUAGE_CXX }, 
        { "alignof"          , ALIGNOF          , LANGUAGE_CXX }, 
        { "typeid"           , TYPEID           , LANGUAGE_CXX },
        { "const_cast"       , CONST_CAST       , LANGUAGE_CXX }, 
        { "dynamic_cast"     , DYNAMIC_CAST     , LANGUAGE_CXX },
        { "reinterpret_cast" , REINTERPRET_CAST , LANGUAGE_CXX },
        { "static_cast"      , STATIC_CAST      , LANGUAGE_CXX },
        { "static_assert"    , STATIC_ASSERT    , LANGUAGE_CXX },

        // C++ alternative operators
        { "and"           , OPERATORS        , LANGUAGE_CXX }, 
        { "and_eq"        , OPERATORS        , LANGUAGE_CXX }, 
        { "bitand"        , OPERATORS        , LANGUAGE_CXX }, 
        { "bitor"         , OPERATORS        , LANGUAGE_CXX }, 
        { "compl"         , OPERATORS        , LANGUAGE_CXX }, 
        { "not"           , OPERATORS        , LANGUAGE_CXX }, 
        { "not_eq"        , OPERATORS        , LANGUAGE_CXX }, 
        { "or"            , OPERATORS        , LANGUAGE_CXX }, 
        { "or_eq"         , OPERATORS        , LANGUAGE_CXX }, 
        { "xor"           , OPERATORS        , LANGUAGE_CXX }, 
        { "xor_eq"        , OPERATORS        , LANGUAGE_CXX }, 

        // Java specific keywords
        { "throws"        , THROWS        , LANGUAGE_JAVA }, 
        { "finally"       , FINALLY       , LANGUAGE_JAVA }, 
        { "interface"     , INTERFACE     , LANGUAGE_JAVA }, 
        { "extends"       , EXTENDS       , LANGUAGE_JAVA }, 
        { "implements"    , IMPLEMENTS    , LANGUAGE_JAVA }, 
        { "super"         , SUPER         , LANGUAGE_JAVA }, 
        { "import"        , IMPORT        , LANGUAGE_JAVA }, 
        { "package"       , PACKAGE       , LANGUAGE_JAVA }, 
        { "final"         , FINAL         , LANGUAGE_JAVA }, 
        { "abstract"      , ABSTRACT      , LANGUAGE_JAVA },
        { "synchronized"  , SYNCHRONIZED  , LANGUAGE_JAVA },
        { "native"        , NATIVE        , LANGUAGE_JAVA },
        { "strictfp"      , STRICTFP      , LANGUAGE_JAVA },
        { "transient"     , TRANSIENT     , LANGUAGE_JAVA }, 
        { "|"             , BAR           , LANGUAGE_JAVA }, 
        { "@"             , ATSIGN        , LANGUAGE_JAVA }, 
        { "null"          , NULLLITERAL   , LANGUAGE_JAVA }, 
        { "instanceof"    , OPERATORS     , LANGUAGE_JAVA }, 
        { "assert"        , ASSERT        , LANGUAGE_JAVA }, 


        // C# specific keywords
        { "foreach"       , FOREACH       , LANGUAGE_CSHARP }, 
        { "ref"           , REF           , LANGUAGE_CSHARP }, 
        { "out"           , OUT           , LANGUAGE_CSHARP }, 
        { "in"            , IN            , LANGUAGE_CSHARP }, 
        { "lock"          , LOCK          , LANGUAGE_CSHARP }, 
        { "is"            , IS            , LANGUAGE_CSHARP }, 
        { "internal"      , INTERNAL      , LANGUAGE_CSHARP }, 
        { "sealed"        , SEALED        , LANGUAGE_CSHARP }, 
        { "override"      , OVERRIDE      , LANGUAGE_CSHARP }, 
        { "explicit"      , EXPLICIT      , LANGUAGE_CSHARP }, 
        { "implicit"      , IMPLICIT      , LANGUAGE_CSHARP }, 
        { "stackalloc"    , STACKALLOC    , LANGUAGE_CSHARP }, 
        { "as"            , AS            , LANGUAGE_CSHARP }, 
        { "interface"     , INTERFACE     , LANGUAGE_CSHARP }, 
        { "delegate"      , DELEGATE      , LANGUAGE_CSHARP }, 
        { "fixed"         , FIXED         , LANGUAGE_CSHARP }, 
        { "checked"       , CHECKED       , LANGUAGE_CSHARP }, 
        { "unchecked"     , UNCHECKED     , LANGUAGE_CSHARP }, 
        { "finally"       , FINALLY       , LANGUAGE_CSHARP }, 
        { "region"        , REGION        , LANGUAGE_CSHARP }, 
        { "endregion"     , ENDREGION     , LANGUAGE_CSHARP }, 
        { "unsafe"        , UNSAFE        , LANGUAGE_CSHARP }, 
        { "readonly"      , READONLY      , LANGUAGE_CSHARP }, 
        { "partial"       , PARTIAL       , LANGUAGE_CSHARP }, 
        { "get"           , GET           , LANGUAGE_CSHARP }, 
        { "set"           , SET           , LANGUAGE_CSHARP }, 
        { "add"           , ADD           , LANGUAGE_CSHARP }, 
        { "remove"        , REMOVE        , LANGUAGE_CSHARP }, 
        { "await"         , AWAIT         , LANGUAGE_CSHARP }, 
        { "abstract"      , ABSTRACT      , LANGUAGE_CSHARP }, 
        { "event"         , EVENT         , LANGUAGE_CSHARP }, 
        { "async"         , ASYNC         , LANGUAGE_CSHARP }, 
        { "this"          , THIS          , LANGUAGE_CSHARP }, 
        { "yield"         , YIELD         , LANGUAGE_CSHARP }, 
        { "params"        , PARAMS        , LANGUAGE_CSHARP },
        { "null"          , NULLLITERAL   , LANGUAGE_CSHARP },
        { "typeof"        , TYPEOF        , LANGUAGE_CSHARP },
        { "alias"         , ALIAS         , LANGUAGE_CSHARP },

        // C# linq
        { "from"          , FROM          , LANGUAGE_CSHARP }, 
        { "where"         , WHERE         , LANGUAGE_CSHARP }, 
        { "select"        , SELECT        , LANGUAGE_CSHARP }, 
        { "let"           , LET           , LANGUAGE_CSHARP }, 
        { "orderby"       , ORDERBY       , LANGUAGE_CSHARP }, 
        { "ascending"     , ASCENDING     , LANGUAGE_CSHARP }, 
        { "descending"    , DESCENDING    , LANGUAGE_CSHARP }, 
        { "group"         , GROUP         , LANGUAGE_CSHARP }, 
        { "by"            , BY            , LANGUAGE_CSHARP }, 
        { "join"          , JOIN          , LANGUAGE_CSHARP }, 
        { "on"            , ON            , LANGUAGE_CSHARP }, 
        { "equals"        , EQUALS        , LANGUAGE_CSHARP }, 
        { "into"          , INTO          , LANGUAGE_CSHARP },

        // Objective-C
        { "@interface"           , ATINTERFACE         , LANGUAGE_OBJECTIVE_C }, 
        { "@implementation"      , ATIMPLEMENTATION    , LANGUAGE_OBJECTIVE_C }, 
        { "@protocol"            , ATPROTOCOL          , LANGUAGE_OBJECTIVE_C },
        { "@end"                 , ATEND               , LANGUAGE_OBJECTIVE_C },
        { "@private"             , PRIVATE             , LANGUAGE_OBJECTIVE_C },
        { "@protected"           , PROTECTED           , LANGUAGE_OBJECTIVE_C },
        { "@public"              , PUBLIC              , LANGUAGE_OBJECTIVE_C },
        { "@required"            , ATREQUIRED          , LANGUAGE_OBJECTIVE_C },
        { "@optional"            , ATOPTIONAL          , LANGUAGE_OBJECTIVE_C },
        { "import"               , IMPORT              , LANGUAGE_OBJECTIVE_C },
        { "@try"                 , TRY                 , LANGUAGE_OBJECTIVE_C },
        { "@catch"               , CATCH               , LANGUAGE_OBJECTIVE_C },
        { "@finally"             , FINALLY             , LANGUAGE_OBJECTIVE_C },
        { "@throw"               , THROW               , LANGUAGE_OBJECTIVE_C },
        { "@synchronized"        , SYNCHRONIZED        , LANGUAGE_OBJECTIVE_C },
        { "@property"            , PROPERTY            , LANGUAGE_OBJECTIVE_C },
        { "@synthesize"          , SYNTHESIZE          , LANGUAGE_OBJECTIVE_C },
        { "@dynamic"             , DYNAMIC             , LANGUAGE_OBJECTIVE_C },
        { "in"                   , IN                  , LANGUAGE_OBJECTIVE_C },
        { "@YES"                 , LITERAL_TRUE        , LANGUAGE_OBJECTIVE_C },
        { "@NO"                  , LITERAL_FALSE       , LANGUAGE_OBJECTIVE_C },
        { "YES"                  , LITERAL_TRUE        , LANGUAGE_OBJECTIVE_C },
        { "NO"                   , LITERAL_FALSE       , LANGUAGE_OBJECTIVE_C },
        { "@true"                , LITERAL_TRUE        , LANGUAGE_OBJECTIVE_C },
        { "@false"               , LITERAL_FALSE       , LANGUAGE_OBJECTIVE_C },
        { "@encode"              , ENCODE              , LANGUAGE_OBJECTIVE_C },
        { "@selector"            , SELECTOR            , LANGUAGE_OBJECTIVE_C },
        { "@autoreleasepool"     , AUTORELEASEPOOL     , LANGUAGE_OBJECTIVE_C },
        { "@compatibility_alias" , COMPATIBILITY_ALIAS , LANGUAGE_OBJECTIVE_C },
        { "@class"               , ATCLASS             , LANGUAGE_OBJECTIVE_C },
        { "nil"                  , NIL                 , LANGUAGE_OBJECTIVE_C },
        { "@("                   , LPAREN              , LANGUAGE_OBJECTIVE_C },
        { "@["                   , ATLBRACKET          , LANGUAGE_OBJECTIVE_C },
        { "@{"                   , LCURLY              , LANGUAGE_OBJECTIVE_C },

        // Apple
        { "__block"         , BLOCK            , LANGUAGE_CXX | LANGUAGE_C | LANGUAGE_OBJECTIVE_C },
        { "__weak"          , WEAK             , LANGUAGE_CXX | LANGUAGE_C | LANGUAGE_OBJECTIVE_C },
        { "__strong"        , STRONG           , LANGUAGE_CXX | LANGUAGE_C | LANGUAGE_OBJECTIVE_C },


        // Combined C/C++ Mode  at end so overrides defaults
        { "restrict"     , CRESTRICT         , LANGUAGE_CXX },
        { "try"          , CXX_TRY           , LANGUAGE_CXX },
        { "catch"        , CXX_CATCH         , LANGUAGE_CXX },
        { "class"        , CXX_CLASS         , LANGUAGE_CXX },

        // OpenMp
        { "omp"          , OMP_OMP           , LANGUAGE_C_FAMILY }, 

   };

    // fill up the literals for the language that we are parsing
    for (unsigned int i = 0; i < (sizeof(keyword_map) / sizeof(keyword_map[0])); ++i)
        if (inLanguage(keyword_map[i].language))
            literals[keyword_map[i].text] = keyword_map[i].token;
}

private:
    antlr::TokenStreamSelector* selector;
public:
    void setSelector(antlr::TokenStreamSelector* selector_) {
        selector = selector_;
    }
}

protected
SPECIAL_CHARS :
    '\3'..'\377'
;
