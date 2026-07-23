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
    #include <algorithm>
    #include <string>
    #include <string_view>
    #include <unordered_map>
    #include <Language.hpp>
    #include <UTF8CharBuffer.hpp>
    #include <antlr/TokenStreamSelector.hpp>
    #include <CommentTextLexer.hpp>
    #include <srcMLToken.hpp>
    #include <TokenLookbackJavaScript.hpp>
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

    // JavaScript
    JS_ARROW;
    JS_AS;
    JS_ASYNC;
    JS_AWAIT;
    JS_CATCH;
    JS_CONST;
    JS_CONSTRUCTOR;
    JS_DEBUGGER;
    JS_DEFAULT;
    JS_DELETE;
    JS_EACH;
    JS_ELSE;
    JS_EXPORT;
    JS_EXTENDS;
    JS_FROM;
    JS_FUNCTION;
    JS_GET;
    JS_IMPORT;
    JS_INSTANCEOF;
    JS_JSX_LITERAL;
    JS_LET;
    JS_NULL;
    JS_RANGE_IN;
    JS_RANGE_OF;
    JS_REGEX;
    JS_SET;
    JS_STATIC;
    JS_TYPEOF;
    JS_UNDEFINED;
    JS_USING;
    JS_VAR;
    JS_VOID;
    JS_WITH;
    JS_YIELD;
    QMARK_PERIOD;

    // TypeScript (appears as a namespace prefix)
    TS_ABSTRACT;
    TS_ACCESSOR;
    TS_ASSERTS;
    TS_ATSIGN;
    TS_DATSIGN;
    TS_DECLARE;
    TS_ENUM;
    TS_IMPLEMENTS;
    TS_INFER;
    TS_INTERFACE;
    TS_IS;
    TS_KEYOF;
    TS_MODULE;
    TS_NAMESPACE;
    TS_OVERRIDE;
    TS_PRIVATE;
    TS_PROTECTED;
    TS_PUBLIC;
    TS_READONLY;
    TS_SATISFIES;
    TS_TYPE;

    // CMake
    CMAKE_BLOCK;
    CMAKE_COMPILER_FLAG;
    CMAKE_ENDBLOCK;
    CMAKE_ENDFOREACH;
    CMAKE_ENDFUNCTION;
    CMAKE_ENDIF;
    CMAKE_ELSEIF;
    CMAKE_ENDMACRO;
    CMAKE_ENDWHILE;
    CMAKE_FOREACH;
    CMAKE_FUNCTION;
    CMAKE_IN;
    CMAKE_ITEMS;
    CMAKE_LISTS;
    CMAKE_MACRO;
    CMAKE_OPERATORS;
    CMAKE_OPTIONS;
    CMAKE_PROPAGATE;
    CMAKE_RANGE;
    CMAKE_RCURLY;
    CMAKE_SCOPE_FOR;
    CMAKE_ZIP_LISTS;
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
    int lastnonspacetoken;
    bool in_cmake_string = false;
    bool in_cmake_bracket = false;

    virtual void consume() noexcept(false) {
        if (LA(1) != ' ')
            lastnonspacetoken = LA(1);

        antlr::CharScanner::consume();
    }

    int getLastToken() const { return lastnonspacetoken; }

    // if LA(1) is the current character, then this was the value of LA(1) three non-whitespace characters ago
    // Note: excludes newline characters, tabs, and whitespace
    int lookaheadMinusThree;

    // if LA(1) is the current character, then this was the value of LA(1) two non-whitespace characters ago
    // Note: excludes newline characters, tabs, and whitespace
    int lookaheadMinusTwo;

    // if LA(1) is the current character, then this was the value of LA(1) one non-whitespace characters ago
    // Note: excludes newline characters, tabs, and whitespace
    int lookaheadMinusOne;

    // adjust the prior non-whitespace character values
    void updateNonWhitespaceCharacters() {
        if (LA(1) != '\n' && LA(1) != '\t' && LA(1) != ' ') {
            lookaheadMinusThree = lookaheadMinusTwo;
            lookaheadMinusTwo = lookaheadMinusOne;
            lookaheadMinusOne = LA(1);
        }
    }

    bool isEnvExprCMake() {
        size_t index = 1;
        if (LA(index++) == 'E' && LA(index++) == 'N' && LA(index++) == 'V' && LA(index) == '{') {
            return true;
        }
        else {
            return false;
        }
    }

    bool isCacheExprCMake() {
        size_t index = 1;
        if (LA(index++) == 'C' && LA(index++) == 'A' && LA(index++) == 'C' && LA(index++) == 'H' && LA(index++) == 'E' && LA(index) == '{') {
            return true;
        }
        else {
            return false;
        }
    }

    // determine if "<" starts a JSX literal in JavaScript by looking ahead
    bool isJSXLiteral() {
        size_t index = 1;
        size_t angle_bracket_count = 0;

        while (LA(index) != -1 /* EOF */) {
            // ignore JavaScript code found in blocks (e.g., "{...}")
            if (LA(index) == '{') {
                size_t curly_count = 0;

                while (LA(index) != -1 /* EOF */) {
                    if (LA(index) == '{')
                        ++curly_count;

                    if (LA(index) == '}' && curly_count > 0) {
                        --curly_count;

                        if (curly_count == 0)
                            break;
                    }

                    ++index;
                }
            }

            // process line comments (e.g., "// ...") or hashbang comments (e.g., "#! ...")
            if (
                (LA(index) == '/' && LA(index + 1) == '/')
                || (LA(index) == '#' && LA(index + 1) == '!')
            ) {
                while (LA(index) != -1 /* EOF */) {
                    if (LA(index) == '\n')
                        break;
                    else
                        ++index;
                }
            }

            // process block comments (e.g., "/* ... */")
            if (LA(index) == '/' && LA(index + 1) == '*') {
                while (LA(index) != -1 /* EOF */) {
                    if ((LA(index) == '*' && LA(index + 1) == '/'))
                        break;
                    else
                        ++index;
                }
            }

            // process HTML comments separately (e.g., "<!-- ... -->")
            if (LA(index) == '<' && LA(index + 1) == '!' && LA(index + 2) == '-' && LA(index + 3) == '-') {
                while (LA(index) != -1 /* EOF */) {
                    // found end of HTML comment (e.g., "-->")
                    if (LA(index) == '-' && LA(index + 1) == '-' && LA(index + 2) == '>') {
                        index += 3;  // "consume" the end of the comment
                        break;
                    }

                    ++index;
                }
            }
            // potential start of an opening/closing JSX tag (e.g., "<h1>" or "</h1>")
            else if (LA(index) == '<') {
                ++angle_bracket_count;
                ++index;

                // found a closing JSX tag (e.g., "</h1>")
                if (LA(index) == '/' && LA(index + 1) != '*' && angle_bracket_count == 1)
                    return true;
            }

            // found a self-closing JSX tag (e.g., "<h1/>")
            if (LA(index) == '/' && LA(index + 1) == '>' && angle_bracket_count == 1)
                return true;

            // found a comma in a start tag, so it is not a JSX literal
            if (angle_bracket_count > 0 && LA(index) == ',')
                return false;

            // found the ">" to end the current opening/closing JSX tag
            if (LA(index) == '>' && angle_bracket_count > 0)
                --angle_bracket_count;

            // stop searching at TERMINATE or EOF
            if (LA(index) == ';')
                break;

            ++index;
        }
        return false;
    }

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
        { "do"           , DO            , LANGUAGE_ALL & ~LANGUAGE_PYTHON },

        { "break"        , BREAK         , LANGUAGE_ALL },
        { "continue"     , CONTINUE      , LANGUAGE_ALL },

        { "switch"       , SWITCH        , LANGUAGE_ALL & ~LANGUAGE_PYTHON },
        { "case"         , CASE          , LANGUAGE_ALL },
        { "default"      , DEFAULT       , LANGUAGE_ALL & ~LANGUAGE_PYTHON },

        { "return"       , RETURN        , LANGUAGE_ALL },

        { "enum"         , ENUM          , LANGUAGE_ALL & ~LANGUAGE_PYTHON },

        { "static"       , STATIC        , LANGUAGE_ALL & ~LANGUAGE_PYTHON },
        { "const"        , CONST         , LANGUAGE_ALL & ~LANGUAGE_PYTHON },
 
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

        { "goto"         , GOTO           , LANGUAGE_ALL & ~LANGUAGE_PYTHON & ~LANGUAGE_JAVASCRIPT },
        { "sizeof"       , SIZEOF         , LANGUAGE_C_FAMILY },

        { "register"     , REGISTER       , LANGUAGE_C | LANGUAGE_CXX },
        { "mutable"      , MUTABLE        , LANGUAGE_CXX },
        { "volatile"     , VOLATILE       , LANGUAGE_ALL & ~LANGUAGE_PYTHON },
        { "__volatile__" , VOLATILE       , LANGUAGE_C_FAMILY },
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
        { "catch"        , CATCH         , LANGUAGE_OO & ~LANGUAGE_PYTHON },
        { "throw"        , THROW         , LANGUAGE_OO & ~LANGUAGE_PYTHON },

        // class
        { "class"        , CLASS         , LANGUAGE_OO },
        { "public"       , PUBLIC        , LANGUAGE_OO & ~LANGUAGE_PYTHON },
        { "private"      , PRIVATE       , LANGUAGE_OO & ~LANGUAGE_PYTHON },
        { "protected"    , PROTECTED     , LANGUAGE_OO & ~LANGUAGE_PYTHON },

        { "new"          , NEW           , LANGUAGE_OO & ~LANGUAGE_PYTHON },

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
        { "false"        , LITERAL_FALSE         , LANGUAGE_OO & ~LANGUAGE_PYTHON },
        { "true"         , LITERAL_TRUE          , LANGUAGE_OO & ~LANGUAGE_PYTHON },

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

        // JavaScript
        { "=>"           , JS_ARROW          , LANGUAGE_JAVASCRIPT },
        { "?."           , QMARK_PERIOD      , LANGUAGE_JAVASCRIPT },
        { "as"           , JS_AS             , LANGUAGE_JAVASCRIPT },
        { "async"        , JS_ASYNC          , LANGUAGE_JAVASCRIPT },
        { "await"        , JS_AWAIT          , LANGUAGE_JAVASCRIPT },
        { "catch"        , JS_CATCH          , LANGUAGE_JAVASCRIPT },
        { "const"        , JS_CONST          , LANGUAGE_JAVASCRIPT },
        { "constructor"  , JS_CONSTRUCTOR    , LANGUAGE_JAVASCRIPT },
        { "debugger"     , JS_DEBUGGER       , LANGUAGE_JAVASCRIPT },
        { "default"      , JS_DEFAULT        , LANGUAGE_JAVASCRIPT },
        { "delete"       , JS_DELETE         , LANGUAGE_JAVASCRIPT },
        { "each"         , JS_EACH           , LANGUAGE_JAVASCRIPT },
        { "else"         , JS_ELSE           , LANGUAGE_JAVASCRIPT },
        { "export"       , JS_EXPORT         , LANGUAGE_JAVASCRIPT },
        { "extends"      , JS_EXTENDS        , LANGUAGE_JAVASCRIPT },
        { "finally"      , FINALLY           , LANGUAGE_JAVASCRIPT },
        { "from"         , JS_FROM           , LANGUAGE_JAVASCRIPT },
        { "function"     , JS_FUNCTION       , LANGUAGE_JAVASCRIPT },
        { "get"          , JS_GET            , LANGUAGE_JAVASCRIPT },
        { "import"       , JS_IMPORT         , LANGUAGE_JAVASCRIPT },
        { "in"           , JS_RANGE_IN       , LANGUAGE_JAVASCRIPT },
        { "instanceof"   , JS_INSTANCEOF     , LANGUAGE_JAVASCRIPT },
        { "let"          , JS_LET            , LANGUAGE_JAVASCRIPT },
        { "null"         , JS_NULL           , LANGUAGE_JAVASCRIPT },
        { "of"           , JS_RANGE_OF       , LANGUAGE_JAVASCRIPT },
        { "set"          , JS_SET            , LANGUAGE_JAVASCRIPT },
        { "static"       , JS_STATIC         , LANGUAGE_JAVASCRIPT },
        { "typeof"       , JS_TYPEOF         , LANGUAGE_JAVASCRIPT },
        { "undefined"    , JS_UNDEFINED      , LANGUAGE_JAVASCRIPT },
        { "using"        , JS_USING          , LANGUAGE_JAVASCRIPT },
        { "var"          , JS_VAR            , LANGUAGE_JAVASCRIPT },
        { "void"         , JS_VOID           , LANGUAGE_JAVASCRIPT },
        { "with"         , JS_WITH           , LANGUAGE_JAVASCRIPT },
        { "yield"        , JS_YIELD          , LANGUAGE_JAVASCRIPT },

        // TypeScript special characters or operators
        { "@"            , TS_ATSIGN         , LANGUAGE_JAVASCRIPT },
        { "is"           , TS_IS             , LANGUAGE_JAVASCRIPT },
        { "keyof"        , TS_KEYOF          , LANGUAGE_JAVASCRIPT },

        // TypeScript (appears as a namespace prefix)
        { "abstract"     , TS_ABSTRACT       , LANGUAGE_JAVASCRIPT },
        { "accessor"     , TS_ACCESSOR       , LANGUAGE_JAVASCRIPT },
        { "asserts"      , TS_ASSERTS        , LANGUAGE_JAVASCRIPT },
        { "declare"      , TS_DECLARE        , LANGUAGE_JAVASCRIPT },
        { "enum"         , TS_ENUM           , LANGUAGE_JAVASCRIPT },
        { "implements"   , TS_IMPLEMENTS     , LANGUAGE_JAVASCRIPT },
        { "infer"        , TS_INFER          , LANGUAGE_JAVASCRIPT },
        { "interface"    , TS_INTERFACE      , LANGUAGE_JAVASCRIPT },
        { "module"       , TS_MODULE         , LANGUAGE_JAVASCRIPT },
        { "namespace"    , TS_NAMESPACE      , LANGUAGE_JAVASCRIPT },
        { "override"     , TS_OVERRIDE       , LANGUAGE_JAVASCRIPT },
        { "private"      , TS_PRIVATE        , LANGUAGE_JAVASCRIPT },
        { "protected"    , TS_PROTECTED      , LANGUAGE_JAVASCRIPT },
        { "public"       , TS_PUBLIC         , LANGUAGE_JAVASCRIPT },
        { "readonly"     , TS_READONLY       , LANGUAGE_JAVASCRIPT },
        { "satisfies"    , TS_SATISFIES      , LANGUAGE_JAVASCRIPT },
        { "type"         , TS_TYPE           , LANGUAGE_JAVASCRIPT },

        // CMake options; placeholder value to be replaced later
        { "_-_-_"          , CMAKE_OPTIONS       , LANGUAGE_CMAKE },

        // CMake strings; placeholder value to be replaced later
        { "_-_-_-_"        , CMAKE_COMPILER_FLAG , LANGUAGE_CMAKE },

        // CMake operators
        { "AND",                    CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "COMMAND",                CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "DEFINED",                CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "DEFINED CACHE",          CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "DEFINED ENV",            CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "EQUAL",                  CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "EXISTS",                 CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "GREATER",                CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "GREATER_EQUAL",          CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "IN_LIST",                CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "IS_ABSOLUTE",            CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "IS_DIRECTORY",           CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "IS_EXECUTABLE",          CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "IS_NEWER_THAN",          CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "IS_READABLE",            CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "IS_SYMLINK",             CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "IS_WRITABLE",            CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "LESS",                   CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "LESS_EQUAL",             CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "MATCHES",                CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "NOT",                    CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "OR",                     CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "PATH_EQUAL",             CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "STREQUAL",               CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "STRGREATER",             CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "STRGREATER_EQUAL",       CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "STRLESS",                CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "STRLESS_EQUAL",          CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "TARGET",                 CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "TEST",                   CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_EQUAL",          CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_GREATER",        CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_GREATER_EQUAL",  CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_LESS",           CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "VERSION_LESS_EQUAL",     CMAKE_OPERATORS, LANGUAGE_CMAKE },
        { "XOR",                    CMAKE_OPERATORS, LANGUAGE_CMAKE },

        // CMake literals
        { "TRUE"         , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "True"         , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "true"         , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "FALSE"        , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "False"        , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "false"        , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "ON"           , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "On"           , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "on"           , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "OFF"          , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "Off"          , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "off"          , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "YES"          , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "Yes"          , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "yes"          , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "NO"           , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "No"           , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "no"           , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "Y"            , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "y"            , LITERAL_TRUE  , LANGUAGE_CMAKE },
        { "N"            , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "n"            , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "IGNORE"       , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "Ignore"       , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "ignore"       , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "NOTFOUND"     , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "NotFound"     , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "Notfound"     , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "notFound"     , LITERAL_FALSE , LANGUAGE_CMAKE },
        { "notfound"     , LITERAL_FALSE , LANGUAGE_CMAKE },

        // CMake
        { ";"            , WS                , LANGUAGE_CMAKE },
        { "}"            , CMAKE_RCURLY      , LANGUAGE_CMAKE },
        { "block"        , CMAKE_BLOCK       , LANGUAGE_CMAKE },
        { "endblock"     , CMAKE_ENDBLOCK    , LANGUAGE_CMAKE },
        { "endforeach"   , CMAKE_ENDFOREACH  , LANGUAGE_CMAKE },
        { "endfunction"  , CMAKE_ENDFUNCTION , LANGUAGE_CMAKE },
        { "endif"        , CMAKE_ENDIF ,       LANGUAGE_CMAKE },
        { "elseif"       , CMAKE_ELSEIF      , LANGUAGE_CMAKE },
        { "endmacro"     , CMAKE_ENDMACRO    , LANGUAGE_CMAKE },
        { "endwhile"     , CMAKE_ENDWHILE    , LANGUAGE_CMAKE },
        { "foreach"      , CMAKE_FOREACH     , LANGUAGE_CMAKE },
        { "function"     , CMAKE_FUNCTION    , LANGUAGE_CMAKE },
        { "IN"           , CMAKE_IN          , LANGUAGE_CMAKE },
        { "ITEMS"        , CMAKE_ITEMS       , LANGUAGE_CMAKE },
        { "LISTS"        , CMAKE_LISTS       , LANGUAGE_CMAKE },
        { "macro"        , CMAKE_MACRO       , LANGUAGE_CMAKE },
        { "PROPAGATE"    , CMAKE_PROPAGATE   , LANGUAGE_CMAKE },
        { "RANGE"        , CMAKE_RANGE       , LANGUAGE_CMAKE },
        { "SCOPE_FOR"    , CMAKE_SCOPE_FOR   , LANGUAGE_CMAKE },
        { "ZIP_LISTS"    , CMAKE_ZIP_LISTS   , LANGUAGE_CMAKE },
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
