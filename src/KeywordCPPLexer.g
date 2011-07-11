/*
  KeywordCPPLexer.g

  Copyright (C) 2004-2010  SDML (www.sdml.info)

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

header {
   #include "Language.hpp"
   #include "UTF8CharBuffer.hpp"
   #include "antlr/TokenStreamSelector.hpp"
   #include "PureCommentLexer.hpp"
   #include "srcMLToken.hpp"
}

header "post_include_cpp" {

void KeywordCPPLexer::fillliterals(const pair litarr[], unsigned int size) {

    for (unsigned int i = 0; i < size; ++i)
        literals[litarr[i].s] = litarr[i].n;
}

void KeywordCPPLexer::changetotextlexer(int typeend) {
          selector->push("text"); 
           ((PureCommentLexer* ) (selector->getStream("text")))->init(typeend, onpreprocline);
}
}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class KeywordCPPLexer extends KeywordLexer;

options {
    classHeaderSuffix="public Language";
    k = 1;
    testLiterals = false;
    noConstructors = true;
    defaultErrorHandler = false;
//    codeGenBitsetTestThreshold=20; 
}

tokens {

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
//	FRIEND;
	OPERATOR;
    EXPLICIT;

    // namespaces
	NAMESPACE;
	USING;

    // templates
	TEMPLATE;

    NEW;
    DELETE;

    // specifiers that are not needed for parsing
    /*
    MUTABLE;
    VOLATILE;
    */

    // Java tokens
    IMPORT;
    PACKAGE;
    FINALLY;
    EXTENDS;
    IMPLEMENTS;
    INTERFACE;
    FINAL;
    ABSTRACT;

    // C++0x tokens
    CONCEPT;
    CONCEPTMAP;
    REQUIRES;
    AUTO;
}

{
public:

bool onpreprocline;
bool startline;

struct pair { char const * const s; int n; };

// map from text of literal to token number, adjusted to language
struct keyword { char const * const text; int token; int language; };

void fillliterals(const pair litarr[], unsigned int size);

void changetotextlexer(int typeend);

KeywordCPPLexer(UTF8CharBuffer* pinput, const char* encoding, int language = LANGUAGE_CXX)
//	: antlr::CharScanner(new UTF8CharBuffer("sub/a.cpp", encoding),true), Language(language), onpreprocline(false), startline(true)	
    : antlr::CharScanner(pinput,true), Language(language), onpreprocline(false), startline(true)
{
    setTokenObjectFactory(srcMLToken::factory);

    keyword keyword_map[] = {
        // common keywords
        { "if"          , IF, LANGUAGE_ALL },
        { "else"        , ELSE, LANGUAGE_ALL },

        { "while"       , WHILE, LANGUAGE_ALL },
        { "for"         , FOR, LANGUAGE_ALL },
        { "do"          , DO, LANGUAGE_ALL },

        { "break"       , BREAK, LANGUAGE_ALL },
        { "continue"    , CONTINUE, LANGUAGE_ALL },

        { "switch"      , SWITCH, LANGUAGE_ALL },
        { "case"        , CASE, LANGUAGE_ALL },
        { "default"     , DEFAULT, LANGUAGE_ALL },

        { "return"      , RETURN, LANGUAGE_ALL },

        { "enum"        , ENUM, LANGUAGE_ALL },

        // operators and special characters
        { ")"           , RPAREN, LANGUAGE_ALL },
	    { ";"           , TERMINATE, LANGUAGE_ALL },
	    { "("           , LPAREN, LANGUAGE_ALL },
	    { "~"           , DESTOP, LANGUAGE_ALL },
	    { ":"           , COLON, LANGUAGE_ALL },
	    { "}"           , RCURLY, LANGUAGE_ALL },
	    { ","           , COMMA, LANGUAGE_ALL },
	    { "]"           , RBRACKET, LANGUAGE_ALL },
	    { "{"           , LCURLY, LANGUAGE_ALL },
	    { "["           , LBRACKET, LANGUAGE_ALL },
	    { "@"           , SPECIAL, LANGUAGE_ALL },

	    { "&lt;"        , TEMPOPS, LANGUAGE_ALL },
	    { "&gt;"        , TEMPOPE, LANGUAGE_ALL },
	    { "&amp;"       , REFOPS, LANGUAGE_ALL },
	    { "="           , EQUAL, LANGUAGE_ALL },

        { "."           , PERIOD, LANGUAGE_ALL },
        //        { ".*"        , MEMBERPOINTER, LANGUAGE_ALL },
        { "*"           , MULTOPS, LANGUAGE_ALL },
        //        { "*="        , MULTIMM, LANGUAGE_ALL },

        // C and C++ specific keywords
        { "main"    , MAIN, LANGUAGE_C_FAMILY },

        { "typedef" , TYPEDEF, LANGUAGE_C_FAMILY },

        { "include" , INCLUDE, LANGUAGE_C_FAMILY },
        { "define"  , DEFINE, LANGUAGE_C_FAMILY },
        { "elif"    , ELIF, LANGUAGE_C_FAMILY },
        { "endif"   , ENDIF, LANGUAGE_C_FAMILY },
        { "error"   , ERRORPREC, LANGUAGE_C_FAMILY },
        { "ifdef"   , IFDEF, LANGUAGE_C_FAMILY },
        { "ifndef"  , IFNDEF, LANGUAGE_C_FAMILY },
        { "line"    , LINE, LANGUAGE_C_FAMILY },
        { "pragma"  , PRAGMA, LANGUAGE_C_FAMILY },
        { "undef"   , UNDEF, LANGUAGE_C_FAMILY },

        { "union"   , UNION, LANGUAGE_C_FAMILY },
        { "struct"  , STRUCT, LANGUAGE_C_FAMILY },

        { "inline"  , INLINE, LANGUAGE_C_FAMILY },
        { "extern"  , EXTERN, LANGUAGE_C_FAMILY },

        { "asm"     , ASM, LANGUAGE_C_FAMILY },

        { "goto"    , GOTO, LANGUAGE_C_FAMILY },

        { "static"  , STATIC, LANGUAGE_C_FAMILY },
    };

    // fill up the literals for the language that we are parsing
    for (unsigned int i = 0; i < (sizeof(keyword_map) / sizeof(keyword_map[0])); ++i)
        if (inLanguage(keyword_map[i].language))
            literals[keyword_map[i].text] = keyword_map[i].token;

    // add all C++ and Java specific keywords to the literals table
    if (inLanguage(LANGUAGE_OO)) {

        pair language_oo[] = {

            // exception handling
            { "try", TRY },
            { "catch", CATCH },
            { "throw", THROW },

            // class
            { "class", CLASS },
            { "public", PUBLIC },
            { "private", PRIVATE },
            { "protected", PROTECTED },

            { "new", NEW },
        };

        fillliterals(language_oo, sizeof(language_oo) / sizeof(language_oo[0]));
    }

    // add all C++ specific keywords to the literals table
    if (inLanguage(LANGUAGE_CXX_FAMILY)) {

        pair language_cxx_family[] = {

            // class
            { "virtual", VIRTUAL }, 
            //     { "friend", FRIEND },
            { "operator", OPERATOR },
            { "explicit", EXPLICIT },

            // namespaces
            { "namespace", NAMESPACE },
            { "using", USING },

            // templates
            { "template", TEMPLATE },

            { "delete", OPERATORS },

            // special C++ operators
            { "::", DCOLON },

            // special C++ constant values
            { "false", FALSE },
            { "true", TRUE },

            // specifiers that are not needed for parsing
        /*
            { "mutable", MUTABLE },
            { "volatile", VOLATILE },
        */
        };

        fillliterals(language_cxx_family, sizeof(language_cxx_family) / sizeof(language_cxx_family[0]));
    }

    // add all C++ specific keywords to the literals table
    if (inLanguage(LANGUAGE_CXX_0X)) {

        pair language_cxx_0x[] = {

            // concepts
            { "concept", CONCEPT },
            { "concept_map", CONCEPTMAP },
            { "requires", REQUIRES },
            { "auto", AUTO },
        };

        fillliterals(language_cxx_0x, sizeof(language_cxx_0x) / sizeof(language_cxx_0x[0]));
    }

    // add all Java specific keywords to the literals table
    if (inLanguage(LANGUAGE_JAVA)) {

        pair language_java[] = {

            // exception handling
            { "throws", THROWS },
            { "finally", FINALLY },

            // class
            { "interface", INTERFACE },
            { "extends", EXTENDS },
            { "implements", IMPLEMENTS },
        
            // import
            { "import", IMPORT },

            // package
            { "package", PACKAGE },

            // final
            { "final", FINAL },

            // abstract
            { "abstract", ABSTRACT },

        };

        fillliterals(language_java, sizeof(language_java) / sizeof(language_java[0]));
    }
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
