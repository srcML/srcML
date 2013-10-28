/*
  KeywordLexer.g

  Copyright (C) 2004-2013  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

header "pre_include_hpp" {
   #include <cstring>
}

header {
   #include "Language.hpp"
   #include "UTF8CharBuffer.hpp"
   #include "antlr/TokenStreamSelector.hpp"
   #include "CommentTextLexer.hpp"
   #include "srcMLToken.hpp"
}

header "post_include_cpp" {

void KeywordLexer::changetotextlexer(int typeend) {
          selector->push("text"); 
           ((CommentTextLexer* ) (selector->getStream("text")))->init(typeend, onpreprocline, atstring);
}
}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class KeywordLexer extends OperatorLexer;

options {
    classHeaderSuffix="public Language";
    k = 1;
    testLiterals = false;
    noConstructors = true;
    defaultErrorHandler = false;
    importVocab=OperatorLexer;
//    codeGenBitsetTestThreshold=20; 
}

tokens {

    // special identifier
    MAIN;

    // statements
    BREAK;
	CONTINUE;
	GOTO;

    WHILE;
	DO;
	FOR;
	ASM;

    IF;
    ELSE;

	SWITCH;
	CASE;
	DEFAULT;

	ENUM;
	TYPEDEF;

    // aggregate types
    UNION;
	STRUCT;

    // functions
	RETURN;

    // cpp
    INCLUDE;
	DEFINE;
	ELIF;
    ENDIF;
    ERRORPREC;
	IFDEF;
	IFNDEF;
    LINE;
	PRAGMA;
	UNDEF;

	INLINE;

    EXTERN;

    // specifiers that are not needed for parsing
    /*
	AUTO = "auto";
    REGISTER = "register";
    MUTABLE = "mutable";
	CONST = "const";
    VOLATILE = "volatile";
    */
    STATIC; // = "static";

    // Standard type keywords do not need to be identified
    /*
    VOID = "void";
	BOOL = "bool";
	CHAR = "char";
    INT = "int";
    SHORT = "short";
    LONG = "long";
    DOUBLE = "double";
    FLOAT = "float";
    */

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
	SIGNAL;
    VIRTUAL;
	FRIEND;
	OPERATOR;
    EXPLICIT;

    // namespaces
	NAMESPACE;
	USING;

    // templates
	TEMPLATE;

    NEW;
    DELETE;

    // specifiers
    CONST;
    MUTABLE;
    VOLATILE;

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

    // C++0x tokens
    AUTO;

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
    VOLATILE;
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
    SYNCHRONIZED;
}

{
public:

bool onpreprocline;
bool startline;
bool atstring;

// map from text of literal to token number, adjusted to language
struct keyword { char const * const text; int token; int language; };

void changetotextlexer(int typeend);

KeywordLexer(UTF8CharBuffer* pinput, const char* encoding, int language)
    : antlr::CharScanner(pinput,true), Language(language), onpreprocline(false), startline(true), atstring(false)
{
    setTokenObjectFactory(srcMLToken::factory);

    keyword keyword_map[] = {
        // common keywords
        { "if"            , IF            , LANGUAGE_ALL }, 
        { "else"          , ELSE          , LANGUAGE_ALL }, 

        { "while"         , WHILE         , LANGUAGE_ALL }, 
        { "for"           , FOR           , LANGUAGE_ALL }, 
        { "do"            , DO            , LANGUAGE_ALL }, 

        { "break"         , BREAK         , LANGUAGE_ALL }, 
        { "continue"      , CONTINUE      , LANGUAGE_ALL }, 

        { "switch"        , SWITCH        , LANGUAGE_ALL }, 
        { "case"          , CASE          , LANGUAGE_ALL }, 
        { "default"       , DEFAULT       , LANGUAGE_ALL }, 

        { "return"        , RETURN        , LANGUAGE_ALL }, 

        { "enum"          , ENUM          , LANGUAGE_ALL }, 

        { "static"        , STATIC        , LANGUAGE_ALL }, 
 
        // operators and special characters
        { ")"             , RPAREN        , LANGUAGE_ALL }, 
	    { ";"             , TERMINATE     , LANGUAGE_ALL }, 
	    { "("             , LPAREN        , LANGUAGE_ALL }, 
	    { "~"             , DESTOP        , LANGUAGE_ALL }, 
	    { ":"             , COLON         , LANGUAGE_ALL }, 
	    { "}"             , RCURLY        , LANGUAGE_ALL }, 
	    { ","             , COMMA         , LANGUAGE_ALL }, 
	    { "]"             , RBRACKET      , LANGUAGE_ALL }, 
	    { "{"             , LCURLY        , LANGUAGE_ALL }, 
	    { "["             , LBRACKET      , LANGUAGE_ALL }, 
	    { "@"             , ATSIGN        , LANGUAGE_JAVA }, 

	    { "&lt;"          , TEMPOPS       , LANGUAGE_ALL }, 
	    { "&gt;"          , TEMPOPE       , LANGUAGE_ALL }, 
	    { "&amp;"         , REFOPS        , LANGUAGE_ALL }, 
	    { "="             , EQUAL         , LANGUAGE_ALL }, 

        { "."             , PERIOD        , LANGUAGE_ALL }, 
        //        { ".*"  , MEMBERPOINTER , LANGUAGE_ALL }, 
        { "*"             , MULTOPS       , LANGUAGE_ALL }, 
        //        { "*="  , MULTIMM       , LANGUAGE_ALL }, 

        // C and C++ specific keywords
        { "main"          , MAIN          , LANGUAGE_C_FAMILY }, 

        { "typedef"       , TYPEDEF       , LANGUAGE_C_FAMILY }, 

        { "include"       , INCLUDE       , LANGUAGE_C_FAMILY }, 
        { "define"        , DEFINE        , LANGUAGE_C_FAMILY }, 
        { "elif"          , ELIF          , LANGUAGE_C_FAMILY }, 
        { "endif"         , ENDIF         , LANGUAGE_C_FAMILY }, 
        { "error"         , ERRORPREC     , LANGUAGE_C_FAMILY }, 
        { "ifdef"         , IFDEF         , LANGUAGE_C_FAMILY }, 
        { "ifndef"        , IFNDEF        , LANGUAGE_C_FAMILY }, 
        { "line"          , LINE          , LANGUAGE_C_FAMILY }, 
        { "pragma"        , PRAGMA        , LANGUAGE_C_FAMILY }, 
        { "undef"         , UNDEF         , LANGUAGE_C_FAMILY }, 

        { "union"         , UNION         , LANGUAGE_C_FAMILY }, 
        { "struct"        , STRUCT        , LANGUAGE_C_FAMILY }, 

        { "inline"        , INLINE        , LANGUAGE_C_FAMILY }, 
        { "extern"        , EXTERN        , LANGUAGE_C_FAMILY }, 

        { "asm"           , ASM           , LANGUAGE_C_FAMILY }, 

        { "goto"          , GOTO          , LANGUAGE_C_FAMILY }, 

        { "const"         , CONST         , LANGUAGE_ALL }, 
        { "mutable"       , MUTABLE       , LANGUAGE_CXX }, 
        { "volatile"      , VOLATILE      , LANGUAGE_CXX_FAMILY }, 

        // add all C++ and Java specific keywords to the literals table

        // exception handling
        { "try"           , TRY           , LANGUAGE_OO }, 
        { "catch"         , CATCH         , LANGUAGE_OO }, 
        { "throw"         , THROW         , LANGUAGE_OO }, 

        // class
        { "class"         , CLASS         , LANGUAGE_OO }, 
        { "public"        , PUBLIC        , LANGUAGE_OO }, 
        { "private"       , PRIVATE       , LANGUAGE_OO }, 
        { "protected"     , PROTECTED     , LANGUAGE_OO }, 
        { "signal"        , SIGNAL        , LANGUAGE_CXX }, 

        { "new"           , NEW           , LANGUAGE_OO }, 

        // add all C++ specific keywords to the literals table
        // class
        { "virtual"       , VIRTUAL       , LANGUAGE_CXX_FAMILY }, 
        { "friend"        , FRIEND        , LANGUAGE_CXX_ONLY }, 
        { "operator"      , OPERATOR      , LANGUAGE_CXX_FAMILY }, 
        { "explicit"      , EXPLICIT      , LANGUAGE_CXX_FAMILY }, 
        
        // namespaces
        { "namespace"     , NAMESPACE     , LANGUAGE_CXX_FAMILY }, 
        { "using"         , USING         , LANGUAGE_CXX_FAMILY }, 
        
        // templates
        { "template"      , TEMPLATE      , LANGUAGE_CXX_FAMILY }, 
        
        { "delete"        , DELETE        , LANGUAGE_CXX_ONLY }, 
        
        // special C++ operators
        { "::"            , DCOLON        , LANGUAGE_CXX_FAMILY }, 
        { "&&"            , RVALUEREF     , LANGUAGE_CXX_FAMILY }, 

        // special C++ constant values
        { "false"         , FALSE         , LANGUAGE_OO }, 
        { "true"          , TRUE          , LANGUAGE_OO }, 

        // specifiers that are not needed for parsing
        /*
          { "mutable"     , MUTABLE       , LANGUAGE_CXX_FAMILY }, 
          { "volatile"    , VOLATILE      , LANGUAGE_CXX_FAMILY }, 
        */

        // add all C++ specific keywords to the literals table
        // concepts
//        { "auto"          , AUTO          , LANGUAGE_CXX_11 }, 

        // add all Java specific keywords to the literals table
        // exception handling
        { "throws"        , THROWS        , LANGUAGE_JAVA }, 
        { "finally"       , FINALLY       , LANGUAGE_JAVA }, 

        // class
        { "interface"     , INTERFACE     , LANGUAGE_JAVA }, 
        { "extends"       , EXTENDS       , LANGUAGE_JAVA }, 
        { "implements"    , IMPLEMENTS    , LANGUAGE_JAVA }, 
        { "super"         , SUPER         , LANGUAGE_JAVA }, 
        
        // import
        { "import"        , IMPORT        , LANGUAGE_JAVA }, 

        // package
        { "package"       , PACKAGE       , LANGUAGE_JAVA }, 

        // final
        { "final"         , FINAL         , LANGUAGE_JAVA }, 

        // abstract
        { "abstract"      , ABSTRACT      , LANGUAGE_JAVA },

        // synchronized
        { "synchronized"  , SYNCHRONIZED  , LANGUAGE_JAVA },

        // add all C# specific keywords to the literals table
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
        { "volatile"      , VOLATILE      , LANGUAGE_CSHARP }, 
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
        selector=selector_;
    }
}

protected
SPECIAL_CHARS :
        '\3'..'\377'
;
