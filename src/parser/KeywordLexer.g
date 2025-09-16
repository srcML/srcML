// SPDX-License-Identifier: GPL-3.0-only
/*!
 * @file KeywordLexer.g
 *
 * @copyright Copyright (C) 2004-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML translator.
 */

header "pre_include_hpp" {
}

header "pre_include_cpp" {
#if defined(__GNUC__)
#endif
#ifdef __clang__
#endif
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#ifdef _MSC_VER
    #pragma warning(disable : 4365)  // 'argument': conversion from 'int' to 'unsigned int', signed/unsigned mismatch
    #pragma warning(disable : 4101)  // 'pe' unreferenced local variable
    #pragma warning(disable : 4456)  // declaration of 'theRetToken' hides previous local declaration
    #pragma warning(disable : 4242) // 'argument': conversion from 'int' to 'char'
#endif
}

header {
    #include <string>
    #include <string_view>
    #include <unordered_map>
    #include <Language.hpp>
    #include <UTF8CharBuffer.hpp>
    #include <antlr/TokenStreamSelector.hpp>
    #include <CommentTextLexer.hpp>
    #include <srcMLToken.hpp>
    #undef CONST
    #undef VOID
    #undef DELETE
    #undef INTERFACE
    #undef OUT
    #undef IN
    #undef THIS

    using namespace ::std::literals::string_view_literals;
}

header "post_include_cpp" {

void KeywordLexer::changetotextlexer(int typeend, std::string delim) {

    selector->push("text"); 
    ((CommentTextLexer* ) (selector->getStream("text")))->init(typeend, onpreprocline, atstring, delim, isline, line_number, options);
}

int KeywordLexer::testLiteralsTable(int ttype) const {

    const auto p = srcMLLiterals.find(text);
    if (p != srcMLLiterals.end())
        return p->second;
    return ttype;
}

int KeywordLexer::testLiteralsTable(const std::string& txt, int ttype) const {

    const auto p = srcMLLiterals.find(txt);
    if (p != srcMLLiterals.end())
        return p->second;
    return ttype;
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
    C_ATTRIBUTE;

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
    RECORD;

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

    // Python
    EXPONENTIATION;
    LITERAL_ELLIPSIS;
    LITERAL_NONE;
    PY_2_EXEC;
    PY_2_PRINT;
    PY_ALIAS;
    PY_AND;
    PY_ARROW;
    PY_ASYNC;
    PY_ATSIGN;
    PY_AWAIT;
    PY_CASE;
    PY_COLON;
    PY_DELETE;
    PY_ELIF;
    PY_EXCEPT;
    PY_FROM;
    PY_FUNCTION;
    PY_GLOBAL;
    PY_IMPORT;
    PY_IN;
    PY_IS;
    PY_LAMBDA;
    PY_LCURLY;
    PY_MATCH;
    PY_NONLOCAL;
    PY_NOT;
    PY_OR;
    PY_PASS;
    PY_RAISE;
    PY_RCURLY;
    PY_TYPE;
    PY_WITH;
    PY_YIELD;

    // CMake
    CMAKE_ENDFOREACH;
    CMAKE_ELSEIF;
    CMAKE_ENDWHILE;
    CMAKE_FOREACH;
    CMAKE_OPTIONS;
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
struct keyword { std::string_view text; int token; int language; };

void changetotextlexer(int typeend, std::string delimiter = "");

virtual int testLiteralsTable(int ttype) const;
virtual int testLiteralsTable(const std::string& txt, int ttype) const;

KeywordLexer(UTF8CharBuffer* pinput, int language, OPTION_TYPE & options,
             std::vector<std::string> user_macro_list)
    : antlr::CharScanner(pinput,true), Language(language), options(options), onpreprocline(false), startline(true),
    atstring(false), rawstring(false), delimiter(""), isline(false), line_number(-1), lastpos(0), prev(0)
{
    if (isoption(options, SRCML_OPTION_LINE))
       setLine(getLine() + (1 << 16));
    setTokenObjectFactory(srcMLToken::factory);

    for (std::vector<std::string>::size_type i = 0; i < user_macro_list.size(); i += 2) {
        if (user_macro_list[i + 1] == "src:macro"sv)
            literals[user_macro_list[i].data()] = MACRO_NAME;
        else if (user_macro_list[i + 1] == "src:name"sv)
            literals[user_macro_list[i].data()] = MACRO_TYPE_NAME;
        else if (user_macro_list[i + 1] == "src:type"sv)
            literals[user_macro_list[i].data()] = MACRO_TYPE_NAME;
        else if (user_macro_list[i + 1] == "src:case"sv)
            literals[user_macro_list[i].data()] = MACRO_CASE;
        else if (user_macro_list[i + 1] == "src:label"sv)
            literals[user_macro_list[i].data()] = MACRO_LABEL;
        else if (user_macro_list[i + 1] == "src:specifier"sv)
            literals[user_macro_list[i].data()] = MACRO_SPECIFIER;
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
        { "__attribute__", C_ATTRIBUTE    , LANGUAGE_CXX | LANGUAGE_C },

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
        { "record"        , RECORD        , LANGUAGE_JAVA },


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


        // Combined C/C++ Mode at end so overrides defaults
        { "restrict"     , CRESTRICT         , LANGUAGE_CXX },
        { "try"          , CXX_TRY           , LANGUAGE_CXX },
        { "catch"        , CXX_CATCH         , LANGUAGE_CXX },
        { "class"        , CXX_CLASS         , LANGUAGE_CXX },

        // OpenMp
        { "omp"          , OMP_OMP           , LANGUAGE_C_FAMILY },

        // Python special characters or operators
        { "}"            , PY_RCURLY         , LANGUAGE_PYTHON },
        { "{"            , PY_LCURLY         , LANGUAGE_PYTHON },
        { ":"            , PY_COLON          , LANGUAGE_PYTHON },
        { "@"            , PY_ATSIGN         , LANGUAGE_PYTHON },
        { "**"           , EXPONENTIATION    , LANGUAGE_PYTHON },
        { "..."          , LITERAL_ELLIPSIS  , LANGUAGE_PYTHON },
        { "->"           , PY_ARROW          , LANGUAGE_PYTHON },
        { "and"          , PY_AND            , LANGUAGE_PYTHON },
        { "await"        , PY_AWAIT          , LANGUAGE_PYTHON },
        { "in"           , PY_IN             , LANGUAGE_PYTHON },
        { "is"           , PY_IS             , LANGUAGE_PYTHON },
        { "not"          , PY_NOT            , LANGUAGE_PYTHON },
        { "or"           , PY_OR             , LANGUAGE_PYTHON },

        // Existing language keywords that are names in Python
        { "__asm"        , NAME              , LANGUAGE_PYTHON },
        { "__volatile__" , NAME              , LANGUAGE_PYTHON },
        { "catch"        , NAME              , LANGUAGE_PYTHON },
        { "const"        , NAME              , LANGUAGE_PYTHON },
        { "default"      , NAME              , LANGUAGE_PYTHON },
        { "do"           , NAME              , LANGUAGE_PYTHON },
        { "enum"         , NAME              , LANGUAGE_PYTHON },
        { "extern"       , NAME              , LANGUAGE_PYTHON },
        { "explicit"     , NAME              , LANGUAGE_PYTHON },
        { "false"        , NAME              , LANGUAGE_PYTHON },
        { "goto"         , NAME              , LANGUAGE_PYTHON },
        { "inline"       , NAME              , LANGUAGE_PYTHON },
        { "main"         , NAME              , LANGUAGE_PYTHON },
        { "namespace"    , NAME              , LANGUAGE_PYTHON },
        { "new"          , NAME              , LANGUAGE_PYTHON },
        { "omp"          , NAME              , LANGUAGE_PYTHON },
        { "operator"     , NAME              , LANGUAGE_PYTHON },
        { "private"      , NAME              , LANGUAGE_PYTHON },
        { "protected"    , NAME              , LANGUAGE_PYTHON },
        { "public"       , NAME              , LANGUAGE_PYTHON },
        { "sizeof"       , NAME              , LANGUAGE_PYTHON },
        { "static"       , NAME              , LANGUAGE_PYTHON },
        { "struct"       , NAME              , LANGUAGE_PYTHON },
        { "switch"       , NAME              , LANGUAGE_PYTHON },
        { "throw"        , NAME              , LANGUAGE_PYTHON },
        { "true"         , NAME              , LANGUAGE_PYTHON },
        { "typedef"      , NAME              , LANGUAGE_PYTHON },
        { "using"        , NAME              , LANGUAGE_PYTHON },
        { "virtual"      , NAME              , LANGUAGE_PYTHON },
        { "volatile"     , NAME              , LANGUAGE_PYTHON },

        // Python
        { "as"           , PY_ALIAS          , LANGUAGE_PYTHON },
        { "assert"       , ASSERT            , LANGUAGE_PYTHON },
        { "async"        , PY_ASYNC          , LANGUAGE_PYTHON },
        { "case"         , PY_CASE           , LANGUAGE_PYTHON },
        { "def"          , PY_FUNCTION       , LANGUAGE_PYTHON },
        { "del"          , PY_DELETE         , LANGUAGE_PYTHON },
        { "elif"         , PY_ELIF           , LANGUAGE_PYTHON },
        { "except"       , PY_EXCEPT         , LANGUAGE_PYTHON },
        { "exec"         , PY_2_EXEC         , LANGUAGE_PYTHON },
        { "False"        , LITERAL_FALSE     , LANGUAGE_PYTHON },
        { "finally"      , FINALLY           , LANGUAGE_PYTHON },
        { "from"         , PY_FROM           , LANGUAGE_PYTHON },
        { "global"       , PY_GLOBAL         , LANGUAGE_PYTHON },
        { "import"       , PY_IMPORT         , LANGUAGE_PYTHON },
        { "lambda"       , PY_LAMBDA         , LANGUAGE_PYTHON },
        { "match"        , PY_MATCH          , LANGUAGE_PYTHON },
        { "None"         , LITERAL_NONE      , LANGUAGE_PYTHON },
        { "nonlocal"     , PY_NONLOCAL       , LANGUAGE_PYTHON },
        { "pass"         , PY_PASS           , LANGUAGE_PYTHON },
        { "print"        , PY_2_PRINT        , LANGUAGE_PYTHON },
        { "raise"        , PY_RAISE          , LANGUAGE_PYTHON },
        { "True"         , LITERAL_TRUE      , LANGUAGE_PYTHON },
        { "type"         , PY_TYPE           , LANGUAGE_PYTHON },
        { "with"         , PY_WITH           , LANGUAGE_PYTHON },
        { "yield"        , PY_YIELD          , LANGUAGE_PYTHON },

        // CMake options
        { "ABSOLUTE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ABSOLUTE_PATH",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ACTION",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "AFTER",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "AFTER_TIMEOUT",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ALIAS",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ALIAS_GLOBAL",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ALL",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ALLOW_SYSTEM_INCLUDES",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ALLOW_SYSTEM_LIBS",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ALL_BUILD",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ALL_COMPONENTS",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ALPHABET",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ANDROID_MK",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ANGLE",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ANSI",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ANY",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "API_VERSION",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "APPEND",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "APPENDIX",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "APPEND_STRING",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ARCHIVE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ARCHIVE_CREATE",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ARCHIVE_EXTRACT",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ARCHIVE_OUTPUT_DIRECTORY",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ARGC",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ARGN",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ARGS",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ARGV",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ARRAY",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ASCENDING",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ASCII",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ASM_MARMASM",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ASM_MASM",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ASM_NASM",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "AT",                                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ATTACHED_FILES",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ATTACHED_FILES_ON_FAIL",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "AUTHOR_WARNING",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "AUTO",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "AUTOMOC",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "AUTOUIC",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "AVAILABLE_PHYSICAL_MEMORY",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "AVAILABLE_VIRTUAL_MEMORY",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BASE_DIR",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BASE_DIRECTORY",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BASE_DIRS",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BEFORE",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BEST_EFFORT",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BIN",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BINARY_DIR",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BOOL",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BOOLEAN",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BOTH",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BRIEF_DOCS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BUILD",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BUILD_DIR",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BUILD_ID",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BUILD_INTERFACE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BUILD_SHARED_LIBS",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BUILD_TESTING",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BUILD_TYPE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BUNDLE",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BUNDLE_EXECUTABLE",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BYPASS_PROVIDER",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "BYPRODUCTS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CACHE",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CACHED_VARIABLE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CALL",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CANCEL_CALL",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CAPTURE_CMAKE_ERROR",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CASE",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CATEGORY",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CDASH_UPLOAD",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CDASH_UPLOAD_TYPE",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CFLAGS",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CHECK_FAIL",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CHECK_PASS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CHECK_START",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CHMOD",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CHMOD_RECURSE",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CLEAR",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKEFILES",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_ADD_CUSTOM_COMMAND_DEPENDS_EXPLICIT_ONLY",     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_APPBUNDLE_PATH",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_BINARY_DIR",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_BUILD_PARALLEL_LEVEL",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_BUILD_TYPE",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_COMMAND",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CROSSCOMPILING",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CROSSCOMPILING_EMULATOR",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CUDA_COMPILE_FEATURES",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CUDA_EXTENSIONS",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CUDA_RUNTIME_LIBRARY",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CUDA_STANDARD",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CUDA_STANDARD_REQUIRED",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CURRENT_BINARY_DIR",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CURRENT_FUNCTION",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CURRENT_FUNCTION_LIST_DIR",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CURRENT_FUNCTION_LIST_FILE",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CURRENT_FUNCTION_LIST_LINE",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CURRENT_LIST_FILE",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CURRENT_SOURCE_DIR",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CXX_COMPILER",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CXX_COMPILE_FEATURES",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CXX_EXTENSIONS",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CXX_STANDARD",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_CXX_STANDARD_REQUIRED",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_C_COMPILE_FEATURES",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_C_EXTENSIONS",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_C_STANDARD",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_C_STANDARD_REQUIRED",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_ENABLE_EXPORTS",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_ERROR_DEPRECATED",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_EXECUTE_PROCESS_COMMAND_ECHO",                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_EXE_LINKER_FLAGS",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_EXPERIMENTAL_EXPORT_PACKAGE_DEPENDENCIES",     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_EXPERIMENTAL_EXPORT_PACKAGE_INFO",             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_EXPORT_NO_PACKAGE_REGISTRY",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_EXPORT_PACKAGE_REGISTRY",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_APPBUNDLE",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_FRAMEWORK",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_LIBRARY_CUSTOM_LIB_SUFFIX",               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_PACKAGE_NAME",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_PACKAGE_NO_PACKAGE_REGISTRY",             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY",      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_PACKAGE_PREFER_CONFIG",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_PACKAGE_REDIRECTS_DIR",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_PACKAGE_RESOLVE_SYMLINKS",                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_PACKAGE_SORT_DIRECTION",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_PACKAGE_SORT_ORDER",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_PACKAGE_TARGETS_GLOBAL",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_ROOT_PATH",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_ROOT_PATH_BOTH",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_ROOT_PATH_MODE_INCLUDE",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_ROOT_PATH_MODE_LIBRARY",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_ROOT_PATH_MODE_PACKAGE",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_ROOT_PATH_MODE_PROGRAM",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_USE_CMAKE_ENVIRONMENT_PATH",              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_USE_CMAKE_PATH",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_USE_CMAKE_SYSTEM_PATH",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_USE_INSTALL_PREFIX",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_USE_PACKAGE_REGISTRY",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_USE_PACKAGE_ROOT_PATH",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH",             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FIND_USE_SYSTEM_PACKAGE_REGISTRY",             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FLAGS",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_FRAMEWORK_PATH",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_OS_RELEASE_FALLBACK_CONTENT",              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_OS_RELEASE_FALLBACK_RESULT",               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_OS_RELEASE_FALLBACK_RESULT_ID",            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_OS_RELEASE_FALLBACK_RESULT_NAME",          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_OS_RELEASE_FALLBACK_RESULT_PRETTY_NAME",   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_OS_RELEASE_FALLBACK_RESULT_VERSION",       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_OS_RELEASE_FALLBACK_RESULT_VERSION_ID",    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_OS_RELEASE_FALLBACK_SCRIPTS",              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_RUNTIME_DEPENDENCIES_COMMAND",             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_RUNTIME_DEPENDENCIES_PLATFORM",            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_GET_RUNTIME_DEPENDENCIES_TOOL",                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_HOST_SYSTEM_NAME",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_HOST_SYSTEM_PROCESSOR",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_IGNORE_PATH",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_IGNORE_PREFIX_PATH",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INCLUDE_DIRECTORIES_BEFORE",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INCLUDE_PATH",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_BINDIR",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_DATADIR",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_DATAROOTDIR",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_DEFAULT_COMPONENT_NAME",               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS",        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_DOCDIR",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_INCLUDEDIR",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_INFODIR",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_LIBDIR",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_LIBEXECDIR",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_LOCALEDIR",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_LOCALSTATEDIR",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_MANDIR",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_MESSAGE",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_MODE",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_PREFIX",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_RUNSTATEDIR",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_SBINDIR",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_SHARESTATEDIR",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_INSTALL_SYSCONFDIR",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_LIBRARY_ARCHITECTURE",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_LIBRARY_PATH",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_LINK_DIRECTORIES_BEFORE",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_LINK_LIBRARIES_STRATEGY",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_LINK_SEARCH_END_STATIC",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_LINK_SEARCH_START_STATIC",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_MESSAGE_CONTEXT",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_MESSAGE_CONTEXT_SHOW",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_MESSAGE_INDENT",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_MESSAGE_LOG_LEVEL",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_MINIMUM_REQUIRED_VERSION",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_MODULE_PATH",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_MSVC_DEBUG_INFORMATION_FORMAT",                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_MSVC_RUNTIME_LIBRARY",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_NETRC",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_NETRC_FILE",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_OBJCXX_EXTENSIONS",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_OBJCXX_STANDARD",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_OBJCXX_STANDARD_REQUIRED",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_OBJC_EXTENSIONS",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_OBJC_STANDARD",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_OBJC_STANDARD_REQUIRED",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_OBJDUMP",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_OSX_ARCHITECTURES",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_ALLOW_SYS_INCLUDES",                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_ALLOW_SYS_LIBS",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_CFLAGS",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_COMPILE_OPTIONS",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_CONFLICTS",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_DESCRIPTION",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_DISABLE_UNINSTALLED",               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_INCLUDES",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_LIBDIRS",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_LIBNAMES",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_LIBS",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_LINK_OPTIONS",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_NAME",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_PC_LIB_DIRS",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_PC_PATH",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_PKGCONF_INCLUDES",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_PKGCONF_LIB_DIRS",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_PROVIDES",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_REQUIRES",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_SYSROOT_DIR",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_SYS_INCLUDE_DIRS",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_SYS_LIB_DIRS",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_TOP_BUILD_DIR",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PKG_CONFIG_VERSION",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_POLICY_DEFAULT_CMP",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_POSITION_INDEPENDENT_CODE",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PREFIX_PATH",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PROGRAM_PATH",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PROJECT_DESCRIPTION",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PROJECT_HOMEPAGE_URL",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PROJECT_INCLUDE",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PROJECT_INCLUDE_BEFORE",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PROJECT_NAME",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PROJECT_TOP_LEVEL_INCLUDES",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_PROJECT_VERSION",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SIZEOF_VOID_P",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_STAGING_PREFIX",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSROOT",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSTEM",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSTEM_APPBUNDLE_PATH",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSTEM_FRAMEWORK_PATH",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSTEM_IGNORE_PATH",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSTEM_IGNORE_PREFIX_PATH",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSTEM_INCLUDE_PATH",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSTEM_LIBRARY_PATH",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSTEM_PREFIX_PATH",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_SYSTEM_PROGRAM_PATH",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TESTDRIVER_AFTER_TESTMAIN",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TESTDRIVER_BEFORE_TESTMAIN",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TLS_CAINFO",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TLS_VERIFY",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TLS_VERSION",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TOOLCHAIN_FILE",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TRY_COMPILE_CONFIGURATION",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TRY_COMPILE_NO_PLATFORM_VARIABLES",            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TRY_COMPILE_PLATFORM_VARIABLES",               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_TRY_COMPILE_TARGET_TYPE",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_WARN_DEPRECATED",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CMAKE_WATCOM_RUNTIME_LIBRARY",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CODE",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CODEGEN",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CODEMODEL",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMMAND_ECHO",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMMAND_ERROR_IS_FATAL",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMMAND_EXPAND_LISTS",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMMAND_NAME",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMMENT",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPARE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPAT_VERSION",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPILE_DEFINITIONS",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPILE_FEATURES",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPILE_LANGUAGE",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPILE_OPTIONS",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPILE_OUTPUT_VARIABLE",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPONENT",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPONENTS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPRESSION",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COMPRESSION_LEVEL",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONCAT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONDITION",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONFIG",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONFIGS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONFIGURATION",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONFIGURATIONS",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONFIGURE",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONFIGURE_DEPENDS",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONFIGURE_FILE",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONFIGURE_LOG",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONFLICTING_DEPENDENCIES_PREFIX",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONTENT",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CONVERT",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COPY",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COPYONLY",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COPY_FILE",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COPY_FILE_ERROR",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "COPY_ON_ERROR",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CORRECT",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CPATH",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CPLUS_INCLUDE_PATH",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CREATE_LINK",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CRLF",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CROSSCOMPILING_EMULATOR",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_BINARY_DIRECTORY",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_BUILD_COMMAND",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_BUILD_CONFIGURATION",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_BUILD_FLAGS",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_BUILD_TARGET",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_CHECKOUT_COMMAND",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_CUSTOM_MAXIMUM_FAILED_TEST_OUTPUT_SIZE",       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_CUSTOM_MAXIMUM_PASSED_TEST_OUTPUT_SIZE",       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_CUSTOM_TEST_OUTPUT_TRUNCATION",                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_CVS_CHECKOUT",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_EXTRA_SUBMIT_FILES",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_NOTES_FILES",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_SOURCE_DIRECTORY",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_SUBMIT_URL",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CTEST_TEST_LOAD",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CUDA",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CUDA_EXTENSIONS",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CUDA_RESOLVE_DEVICE_SYMBOLS",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CUDA_SEPARABLE_COMPILATION",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CUDA_STANDARD",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CUDA_STANDARD_REQUIRED",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_EXTENSIONS",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_MODULE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_MODULES",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_MODULES_BMI",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_MODULES_DIRECTORY",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_MODULE_DIRS",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_MODULE_SET",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_MODULE_SETS",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_STANDARD",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CXX_STANDARD_REQUIRED",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "CYGWIN",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "C_EXTENSIONS",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "C_INCLUDE_PATH",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "C_STANDARD",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "C_STANDARD_REQUIRED",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DATA",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DATAROOT",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DBAR",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DBUILD_TYPE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DCMAKE_PROJECT_TOP_LEVEL_INCLUDES",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DCOMPILE_DEFINITIONS",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DCOMPONENT",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEBUG",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEBUG_CONFIGURATIONS",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEC",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DECIMAL",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEFAULT_CONFIGURATIONS",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEFAULT_TARGETS",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEFECT_COUNT",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEFER",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEFINITION",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEPENDS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEPENDS_EXPLICIT_ONLY",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEPFILE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEPRECATION",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DESCENDING",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DESCRIPTION",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DESTDIR",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DESTINATION",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DEVICE_LINK",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DFOO",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DIRECTORIES",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DIRECTORY",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DIRECTORY_PERMISSIONS",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISABLE_PRECOMPILE_HEADERS",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISABLE_UNINSTALLED",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRIB_INFO",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRIB_PRETTY_NAME",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_BUG_REPORT_URL",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_HOME_URL",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_ID",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_ID_LIKE",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_NAME",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_PRETTY_NAME",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_PRIVACY_POLICY_URL",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_SUPPORT_URL",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_UBUNTU_CODENAME",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_VERSION",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_VERSION_CODENAME",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DISTRO_VERSION_ID",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DLINK_LIBRARIES",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DOBJ",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DOC",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DOWNLOAD",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "DVAR",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ECHO_ERROR_VARIABLE",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ECHO_OUTPUT_VARIABLE",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ENABLED",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ENABLE_EXPORTS",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ENCODING",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ENV",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ENV_MODE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ERROR_FILE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ERROR_QUIET",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ERROR_STRIP_TRAILING_WHITESPACE",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ERROR_VARIABLE",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ESCAPE_QUOTES",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EVAL",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXACT",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXCLUDE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXCLUDE_FIXTURE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXCLUDE_FIXTURE_CLEANUP",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXCLUDE_FIXTURE_SETUP",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXCLUDE_FROM_ALL",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXCLUDE_FROM_FILE",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXCLUDE_LABEL",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXECUTABLE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXECUTABLES",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXIT",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXPAND_TILDE",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXPECTED_HASH",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXPORT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXPORT_ANDROID_MK",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXPORT_FIND_PACKAGE_NAME",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXPORT_LINK_INTERFACE_LIBRARIES",                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXPORT_NAME",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXPORT_PACKAGE_DEPENDENCIES",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXPR",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXT",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXTENSION",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXTENSION_DEF",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXTRACT",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXTRA_ARGS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "EXTRA_INCLUDE",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FAILED_TO_RUN",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FAIL_REGULAR_EXPRESSION",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FAST",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FATAL_ERROR",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FDO",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FETCHCONTENT_MAKEAVAILABLE_SERIAL",                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FETCHCONTENT_TRY_FIND_PACKAGE_MODE",                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FI",                                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILE",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILENAME",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILENAME_DEF",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILEPATH",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILES",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILES_MATCHING",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILE_BASENAME",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILE_PERMISSIONS",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILE_SET",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FILTER",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FIND",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FIND_PACKAGE",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FIND_PACKAGE_ARGS",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FIND_PACKAGE_VERSION_FORMAT",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FIND_XXX",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FIND_XXX_REGISTRY_VIEW",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FLAGS",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FLTK",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FOLLOW_SYMLINKS",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FOLLOW_SYMLINK_CHAIN",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FOO_ENABLE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FOO_STRING",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FOR",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FORCE",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FORMAT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FQDN",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FRAMEWORK",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FULL_DOCS",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "FUNCTION",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GENERATE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GENERATED",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GENEX_STRIP",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_CALL",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_CALL_IDS",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_EXTENSION",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_FILENAME",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_MESSAGE_LOG_LEVEL",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_PARENT_PATH",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_RELATIVE_PART",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_ROOT_DIRECTORY",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_ROOT_NAME",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_ROOT_PATH",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_RUNTIME_DEPENDENCIES",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GET_STEM",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GIT_REPOSITORY",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GIT_SUBMODULES",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GIT_TAG",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GLOB",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GLOBAL",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GLOB_RECURSE",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GROUP",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GROUP_EXECUTE",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GROUP_READ",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GROUP_WRITE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GUARD",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "GUID",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HASH",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_EXTENSION",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_FILENAME",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_FPU",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_MMX",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_MMX_PLUS",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_PARENT_PATH",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_RELATIVE_PART",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_ROOT_DIRECTORY",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_ROOT_NAME",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_ROOT_PATH",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_SERIAL_NUMBER",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_SSE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_SSE_FP",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_SSE_MMX",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HAS_STEM",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HEADERS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HEADER_DIRS",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HEADER_FILE_ONLY",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HEADER_SET",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HEADER_SETS",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HEX",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HEXADECIMAL",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HINTS",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKCC",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKCR",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKCU",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKEY_CLASSES_ROOT",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKEY_CURRENT_CONFIG",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKEY_CURRENT_USER",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKEY_LOCAL_MACHINE",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKEY_USERS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKLM",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HKU",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HOME",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HOMEPAGE_URL",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HOST",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HOSTNAME",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HOST_LINK",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HTTP",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "HTTPHEADER",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ID",                                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ID_VAR",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IGNORED",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IMPLICIT_DEPENDS",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IMPORTED",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IMPORTED_IMPLIB",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IMPORTED_LOCATION",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IMPORTED_NO_SONAME",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IMPORTED_OBJECTS",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IMPORTED_RUNTIME_ARTIFACTS",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IMPORTED_SONAME",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INACTIVITY_TIMEOUT",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INCLUDE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INCLUDES",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INCLUDE_DIRECTORIES",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INCLUDE_FROM_FILE",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INCLUDE_INTERNALS",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INCLUDE_LABEL",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INFO",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INHERITED",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INITIALIZE_FROM_VARIABLE",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INPUT",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INPUT_FILE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INPUT_MAY_BE_RECENT",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INSENSITIVE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INSERT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INSTALL",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INSTALL_INTERFACE",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INSTALL_PARALLEL",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INSTALL_PREFIX",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_COMPILE_DEFINITIONS",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_COMPILE_FEATURES",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_COMPILE_OPTIONS",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_CXX_MODULE_SETS",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_HEADER_SETS",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_INCLUDE_DIRECTORIES",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_LINK_DIRECTORIES",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_LINK_LIBRARIES",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_LINK_OPTIONS",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_PRECOMPILE_HEADERS",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_SOURCES",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERFACE_SYSTEM_INCLUDE_DIRECTORIES",               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "INTERNAL",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ISPC",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IS_PREFIX",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "IS_RELATIVE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ITEMS",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "JOB_POOL",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "JOB_POOLS",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "JOB_SERVER_AWARE",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "JOIN",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "JSON",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LABELS",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LANG",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LANGUAGES",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LAST",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LAST_EXT",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LAST_ONLY",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LC_ID_DYLIB",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LENGTH",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LENGTH_MAXIMUM",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LENGTH_MINIMUM",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIB",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIBEXEC",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIBRARIES",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIBRARY",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIBRARY_OUTPUT_DIRECTORY",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIBRARY_PATH",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIBS",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIMIT",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIMIT_COUNT",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIMIT_INPUT",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIMIT_OUTPUT",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINKER",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINKER_LANGUAGE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_DIRECTORIES",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_FLAGS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_INTERFACE_LIBRARIES",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_INTERFACE_LIBRARIES_DEBUG",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_INTERFACE_MULTIPLICITY",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_LIBRARIES",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_LIBRARIES_STRATEGY",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_OPTIONS",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_PRIVATE",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LINK_PUBLIC",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIST",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LISTS",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIST_DIRECTORIES",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LIST_ONLY",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LOCALE",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LOCALSTATE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LOCATION",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LOCK",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LOG",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LOG_DESCRIPTION",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "LOWER_CASE_FILE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MACOSX_BUNDLE",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MACROS",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MAIN_DEPENDENCY",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MAKE_C_IDENTIFIER",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MAKE_DIRECTORY",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MAN",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MANUALLY_ADDED_DEPENDENCIES",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MATCH",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MATCHALL",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MEMBER",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MESSAGE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MESSAGE_NEVER",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MMX",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MODIFIED_ACCESS",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MODULE",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MODULES",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MSDN",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MSVC",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MSYS",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MSYSTEM",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MSYSTEM_PREFIX",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MTIME",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MULTI",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MYCOMP_PROVIDER_INSTALL_DIR",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MYLIBRARY",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MY_CHECK_RESULT",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MY_NEW_PROP",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "MY_SCRIPT",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NAME",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NAMELINK_COMPONENT",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NAMELINK_ONLY",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NAMELINK_SKIP",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NAMES",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NAMESPACE",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NAMES_PER_DIR",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NAME_WE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NAME_WLE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NATIVE_COMMAND",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NATIVE_PATH",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NATURAL",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NETRC",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NETRC_FILE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NEVER",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NEWLINE_CONSUME",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NEWLINE_STYLE",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NEW_PROCESS",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NNNN",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NONE",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NORMAL",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NORMALIZE",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NORMAL_PATH",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NOTE",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NOTEQUAL",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NOTICE",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NOT_EQUAL",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_CACHE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_CMAKE_BUILDS_PATH",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_CMAKE_ENVIRONMENT_PATH",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_CMAKE_FIND_ROOT_PATH",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_CMAKE_INSTALL_PREFIX",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_CMAKE_PACKAGE_REGISTRY",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_CMAKE_PATH",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_CMAKE_SYSTEM_PACKAGE_REGISTRY",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_CMAKE_SYSTEM_PATH",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_DEFAULT_PATH",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_HEX_CONVERSION",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_LOG",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_MODULE",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_PACKAGE_ROOT_PATH",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_POLICY_SCOPE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_REPLACE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_SOURCE_PERMISSIONS",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NO_SYSTEM_ENVIRONMENT_PATH",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NULL",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NUMBER",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NUMBER_ERRORS",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NUMBER_OF_LOGICAL_CORES",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NUMBER_OF_PHYSICAL_CORES",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "NUMBER_WARNINGS",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJC",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJCXX",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJCXX_EXTENSIONS",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJCXX_STANDARD",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJCXX_STANDARD_REQUIRED",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJC_EXTENSIONS",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJC_INCLUDE_PATH",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJC_STANDARD",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJC_STANDARD_REQUIRED",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJECT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJECTS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OBJECT_DEPENDS",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OEM",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OFFSET",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ONLY",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ONLY_CMAKE_FIND_ROOT_PATH",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ONLY_IF_DIFFERENT",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OPTIMIZE_DEPENDENCIES",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OPTIONAL",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OPTIONAL_COMPONENTS",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OPTIONS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ORDER",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ORIGIN",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OS_NAME",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OS_PLATFORM",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OS_RELEASE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OS_VERSION",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUTS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUT_DIRECTORY",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUT_FILE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUT_FORMAT",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUT_JUNIT",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUT_NAME",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUT_QUIET",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUT_STRIP_TRAILING_WHITESPACE",                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OUTPUT_VARIABLE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OVERRIDE_FIND_PACKAGE",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OWNER_EXECUTE",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OWNER_READ",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "OWNER_WRITE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGENAME",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_DEPENDENCY",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_NAME",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_COMPLETE",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_COUNT",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MAJOR",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MAX",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MAX_COUNT",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MAX_MAJOR",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MAX_MINOR",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MAX_PATCH",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MAX_TWEAK",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MIN",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MINOR",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MIN_COUNT",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MIN_MAJOR",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MIN_MINOR",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MIN_PATCH",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_MIN_TWEAK",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_PATCH",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_RANGE",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_RANGE_MAX",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_RANGE_MIN",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_FIND_VERSION_TWEAK",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_INFO",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_VERSION",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_VERSION_COMPATIBLE",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_VERSION_EXACT",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PACKAGE_VERSION_UNSUITABLE",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PARALLEL_LEVEL",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PARENT_PATH",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PARENT_SCOPE",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PARSE_ARGV",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PARTS",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PASS_REGULAR_EXPRESSION",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PATH",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PATHS",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PATH_SUFFIXES",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PATH_TO_MESA",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PATTERN",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PATTERNS",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PC_LIBDIR",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PC_PATH",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PC_SYSROOT_DIR",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PERMISSIONS",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PERMISSIVE",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKGCONF",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_ALLOW_SYSTEM_CFLAGS",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_ALLOW_SYSTEM_LIBS",                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_DISABLE_UNINSTALLED",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_LIBDIR",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_PATH",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_SYSROOT_DIR",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_SYSROOT_PATH",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_SYSTEM_INCLUDE_PATH",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_SYSTEM_LIBRARY_PATH",                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PKG_CONFIG_TOP_BUILD_DIR",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PLATFORM",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POLICIES",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POLICY",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POP",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POP_BACK",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POP_FRONT",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POSITION_INDEPENDENT_CODE",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POSIX",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POST_BUILD",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POST_EXCLUDE_FILES",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POST_EXCLUDE_REGEXES",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POST_INCLUDE_FILES",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POST_INCLUDE_REGEXES",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "POST_INSTALL_SCRIPT",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRECOMPILE_HEADERS",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRECOMPILE_HEADERS_REUSE_FROM",                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PREFIX",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PREORDER",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PREPEND",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRETTY_NAME",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRE_BUILD",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRE_EXCLUDE_REGEXES",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRE_INCLUDE_REGEXES",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRE_INSTALL_SCRIPT",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRE_LINK",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRIVATE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PRIVATE_HEADER",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROCESS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROCESSOR_DESCRIPTION",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROCESSOR_NAME",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROCESSOR_SERIAL_NUMBER",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROGRAM",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROGRAMS",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROGRAM_ARGS",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_BINARY_DIR",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_DESCRIPTION",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_HOMEPAGE_URL",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_IS_TOP_LEVEL",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_NAME",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_SOURCE_DIR",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_VERSION",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_VERSION_MAJOR",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_VERSION_MINOR",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_VERSION_PATCH",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROJECT_VERSION_TWEAK",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROPAGATE",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROPAGATE_TOP_LEVEL_INCLUDES_TO_TRY_COMPILE",        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROPERTIES",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PROPERTY",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PUBLIC",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PUBLIC_HEADER",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "PUSH",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "QUERY",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "QUIET",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RANDOM",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RANDOM_SEED",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RANGE",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RANGE_END",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RANGE_START",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "READ",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "READ_ACCESS",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "READ_SYMLINK",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "READ_WITH_PREFIX",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REALPATH",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REAL_PATH",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REGEX",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REGISTRY_VIEW",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REGULAR_EXPRESSION",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REG_DWORD",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REG_EXPAND_SZ",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REG_MULTI_SZ",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REG_QWORD",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REG_SZ",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RELATIVE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RELATIVE_PART",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RELATIVE_PATH",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RELEASE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REMOVE",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REMOVED_ACCESS",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REMOVE_AT",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REMOVE_DUPLICATES",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REMOVE_EXTENSION",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REMOVE_FILENAME",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REMOVE_ITEM",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REMOVE_RECURSE",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RENAME",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REPEAT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REPLACE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REPLACE_EXTENSION",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REPLACE_FILENAME",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REQUIRED",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REQUIRES",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RESOLVED_DEPENDENCIES_VAR",                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RESOURCE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RESOURCE_SPEC_FILE",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RESULT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RESULTS_VARIABLE",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RESULTVAR",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RESULT_VARIABLE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RETRY_COUNT",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RETRY_DELAY",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RETURN_VALUE",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REUSE_FROM",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "REVERSE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ROOT_DIRECTORY",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ROOT_NAME",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ROOT_PATH",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RPATH",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUNPATH",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUNSTATE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUNTIME",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUNTIME_DEPENDENCIES",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUNTIME_DEPENDENCY_SET",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUNTIME_DIRECTORY",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUNTIME_OUTPUT_DIRECTORY",                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUN_OUTPUT_STDERR_VARIABLE",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUN_OUTPUT_STDOUT_VARIABLE",                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "RUN_OUTPUT_VARIABLE",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SBIN",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SCHEDULE_RANDOM",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SCOPE_FOR",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SCRIPT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SELECTOR",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SEND_ERROR",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SENSITIVE",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SEPARATE_ARGS",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SEPARATOR",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SET",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SETGID",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SETUID",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SETUP",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SET_DEPENDENCY_PROVIDER",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SHARED",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SHAREDSTATE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SHELL",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SHOW_PROGRESS",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SIZE",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SKIP_PRECOMPILE_HEADERS",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SKIP_REGULAR_EXPRESSION",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOFTWARE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SONAME",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SORT",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOURCE",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOURCES",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOURCES_TYPE",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOURCE_DATE_EPOCH",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOURCE_DIR",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOURCE_FROM_CONTENT",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOURCE_FROM_FILE",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOURCE_FROM_VAR",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SOVERSION",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "START",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STATIC",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STATIC_LIBRARY",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STATIC_LIBRARY_OPTIONS",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STATUS",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STDERR",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STDOUT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STEM",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STOP_ON_FAILURE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STOP_TIME",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STRICT",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STRICTNESS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STRIDE",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STRING",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STRINGS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "STRIP",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SUBKEYS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SUBLIST",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SUBMIT_URL",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SUBSTRING",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SUPPORTED_METHODS",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SYMBOLIC",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SYSCONF",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SYSTEM",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SYSTEM_INCLUDE_DIRS",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "SYSTEM_LIBRARY_DIRS",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TARGETS",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TARGET_DIRECTORY",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TARGET_FILE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TARGET_LINKER_FILE",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TARGET_OBJECTS",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TARGET_PDB_FILE",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TARGET_PROPERTY",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TARGET_RUNTIME_DLLS",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TARGET_SONAME_FILE",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TEST_LAUNCHER",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TEST_LOAD",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TEST_VARIABLE",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TIMEOUT",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TIMESTAMP",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TLS_CAINFO",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TLS_VERIFY",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TLS_VERSION",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TOLOWER",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TOOLCHAINS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TOP_BUILD_DIR",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TOTAL_PHYSICAL_MEMORY",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TOTAL_VIRTUAL_MEMORY",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TOUCH",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TOUCH_NOCREATE",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TOUPPER",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TO_CMAKE_PATH",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TO_CMAKE_PATH_LIST",                                 CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TO_NATIVE_PATH",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TO_NATIVE_PATH_LIST",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TRACE",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TRACK",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TRANSFORM",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TRANSFORM_APPEND",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TRANSFORM_GENEX_STRIP",                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TRANSFORM_REPLACE",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TRANSFORM_STRIP",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TRANSFORM_TOLOWER",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TREE",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "TYPE",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UNIX",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UNIX_COMMAND",                                       CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UNKNOWN",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UNKNOWN_MODIFIED_ACCESS",                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UNKNOWN_READ_ACCESS",                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UNRESOLVED_DEPENDENCIES_VAR",                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UNSET",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UNTIL_FAIL",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UNTIL_PASS",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UPLOAD",                                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UPPER",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "USERPROFILE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "USERPWD",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "USES_TERMINAL",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "USE_SOURCE_PERMISSIONS",                             CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UTC",                                                CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "UUID",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VALIDATOR",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VALUE",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VALUE_NAMES",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VARIABLE",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VARIABLES",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VERBATIM",                                           CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VERBOSE",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VERSION",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VERSION_SCHEMA",                                     CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "VIEW",                                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WARNING",                                            CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WILL_FAIL",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WINDOWS_COMMAND",                                    CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WINDOWS_REGISTRY",                                   CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WORKING_DIRECTORY",                                  CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WORLD_EXECUTE",                                      CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WORLD_READ",                                         CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WORLD_WRITE",                                        CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WRITE",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "WRONG",                                              CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "XCFRAMEWORK_LOCATION",                               CMAKE_OPTIONS, LANGUAGE_CMAKE },
        { "ZIP_LISTS",                                          CMAKE_OPTIONS, LANGUAGE_CMAKE },

        // CMake special characters or operators
        { "AND",                    OPERATORS, LANGUAGE_CMAKE },
        { "COMMAND",                OPERATORS, LANGUAGE_CMAKE },
        { "DEFINED",                OPERATORS, LANGUAGE_CMAKE },
        { "DEFINED CACHE",          OPERATORS, LANGUAGE_CMAKE },
        { "DEFINED ENV",            OPERATORS, LANGUAGE_CMAKE },
        { "EQUAL",                  OPERATORS, LANGUAGE_CMAKE },
        { "EXISTS",                 OPERATORS, LANGUAGE_CMAKE },
        { "GREATER",                OPERATORS, LANGUAGE_CMAKE },
        { "GREATER_EQUAL",          OPERATORS, LANGUAGE_CMAKE },
        { "IN_LIST",                OPERATORS, LANGUAGE_CMAKE },
        { "IS_ABSOLUTE",            OPERATORS, LANGUAGE_CMAKE },
        { "IS_DIRECTORY",           OPERATORS, LANGUAGE_CMAKE },
        { "IS_EXECUTABLE",          OPERATORS, LANGUAGE_CMAKE },
        { "IS_NEWER_THAN",          OPERATORS, LANGUAGE_CMAKE },
        { "IS_READABLE",            OPERATORS, LANGUAGE_CMAKE },
        { "IS_SYMLINK",             OPERATORS, LANGUAGE_CMAKE },
        { "IS_WRITABLE",            OPERATORS, LANGUAGE_CMAKE },
        { "LESS",                   OPERATORS, LANGUAGE_CMAKE },
        { "LESS_EQUAL",             OPERATORS, LANGUAGE_CMAKE },
        { "MATCHES",                OPERATORS, LANGUAGE_CMAKE },
        { "NOT",                    OPERATORS, LANGUAGE_CMAKE },
        { "OR",                     OPERATORS, LANGUAGE_CMAKE },
        { "PATH_EQUAL",             OPERATORS, LANGUAGE_CMAKE },
        { "STREQUAL",               OPERATORS, LANGUAGE_CMAKE },
        { "STRGREATER",             OPERATORS, LANGUAGE_CMAKE },
        { "STRGREATER_EQUAL",       OPERATORS, LANGUAGE_CMAKE },
        { "STRLESS",                OPERATORS, LANGUAGE_CMAKE },
        { "STRLESS_EQUAL",          OPERATORS, LANGUAGE_CMAKE },
        { "TARGET",                 OPERATORS, LANGUAGE_CMAKE },
        { "TEST",                   OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_EQUAL",          OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_GREATER",        OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_GREATER_EQUAL",  OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_LESS",           OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_LESS_EQUAL",     OPERATORS, LANGUAGE_CMAKE },
        { "XOR",                    OPERATORS, LANGUAGE_CMAKE },

        // CMake
        { "endforeach"   , CMAKE_ENDFOREACH  , LANGUAGE_CMAKE },
        { "elseif"       , CMAKE_ELSEIF      , LANGUAGE_CMAKE },
        { "endwhile"     , CMAKE_ENDWHILE    , LANGUAGE_CMAKE },
        { "foreach"      , CMAKE_FOREACH     , LANGUAGE_CMAKE },
   };

    // fill up the literals for the language that we are parsing
    for (unsigned int i = 0; i < (sizeof(keyword_map) / sizeof(keyword_map[0])); ++i)
        if (inLanguage(keyword_map[i].language)) {
            srcMLLiterals[keyword_map[i].text] = keyword_map[i].token;
        }
}

private:
    antlr::TokenStreamSelector* selector;
    std::unordered_map<std::string_view, int> srcMLLiterals;
public:
    void setSelector(antlr::TokenStreamSelector* selector_) {
        selector = selector_;
    }
}

protected
SPECIAL_CHARS :
    '\3'..'\377'
;
