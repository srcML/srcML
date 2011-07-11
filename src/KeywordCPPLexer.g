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

void fillliterals(const pair litarr[], unsigned int size);

void changetotextlexer(int typeend);

KeywordCPPLexer(UTF8CharBuffer* pinput, const char* encoding, int language = LANGUAGE_CXX)
//	: antlr::CharScanner(new UTF8CharBuffer("sub/a.cpp", encoding),true), Language(language), onpreprocline(false), startline(true)	
    : antlr::CharScanner(pinput,true), Language(language), onpreprocline(false), startline(true)
{
    setTokenObjectFactory(srcMLToken::factory);

    pair common[] = {
        { ")", RPAREN },
	    { ";", TERMINATE },
	    { "(", LPAREN },
	    { "~", DESTOP },
	    { ":", COLON },
	    { "}", RCURLY },
	    { ",", COMMA },
	    { "]", RBRACKET },
	    { "{", LCURLY },
	    { "[", LBRACKET },
	    { "@", SPECIAL },

	    { "&lt;", TEMPOPS },
	    { "&gt;", TEMPOPE },
	    { "&amp;", REFOPS },
	    { "=", EQUAL },

        { ".", PERIOD },
//        { ".*", MEMBERPOINTER },
        { "*", MULTOPS },
//        { "*=", MULTIMM },

        // common keywords
        { "if", IF },
        { "else", ELSE },

        { "while", WHILE },
        { "for", FOR },
        { "do", DO },

        { "break", BREAK },
        { "continue", CONTINUE },

        { "switch", SWITCH },
        { "case", CASE },
        { "default", DEFAULT },

        { "return", RETURN },

        { "enum", ENUM },
    };

    fillliterals(common, sizeof(common) / sizeof(common[0]));

    // add all C and C++ specific keywords to the literals table
    if (inLanguage(LANGUAGE_C_FAMILY)) {

        pair cfamily[] = {
            { "main", MAIN },

            { "typedef", TYPEDEF },

            { "include", INCLUDE },
            { "define", DEFINE },
            { "elif", ELIF },
            { "endif", ENDIF },
            { "error", ERRORPREC },
            { "ifdef", IFDEF },
            { "ifndef", IFNDEF },
            { "line", LINE },
            { "pragma", PRAGMA },
            { "undef", UNDEF },

            { "union", UNION },
            { "struct", STRUCT },

            { "inline", INLINE },
            { "extern", EXTERN },

            { "asm", ASM },

            { "goto", GOTO },

            { "static", STATIC },
        };

        fillliterals(cfamily, sizeof(cfamily) / sizeof(cfamily[0]));
    }

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
