/*
  KeywordLexer.g

  Copyright (C) 2002, 2003, 2004, 2005  SDML (www.sdml.info)

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
   #include <iostream>
   #include <sstream>
   #include "Language.h"
   #include "UTF8CharBuffer.h"
}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class KeywordCPPLexer extends KeywordLexer;

options {
    classHeaderSuffix="public Language";
    k = 2;
    testLiterals = false;
    noConstructors = true;
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

    // C++0x tokens
    CONCEPT;
    CONCEPTMAP;
    REQUIRES;
    AUTO;
}

{
public:

bool onpreprocline;
bool justws;
std::string srcuri;

KeywordCPPLexer(std::istream& in, const char* encoding, const std::string& src, int language = LANGUAGE_CXX)
	: antlr::CharScanner(new UTF8CharBuffer(encoding, in),true), Language(language), onpreprocline(false), justws(true), srcuri(src != "" ? src + ":" : "")
{
    initLiterals();

    // common keywords
    literals["if"] = IF;
    literals["else"] = ELSE;

    literals["while"] = WHILE;
    literals["for"] = FOR;
    literals["do"] = DO;

    literals["break"] = BREAK;
    literals["continue"] = CONTINUE;

    literals["switch"] = SWITCH;
    literals["case"] = CASE;
    literals["default"] = DEFAULT;

    literals["return"] = RETURN;

    literals["enum"] = ENUM;

    // add all C and C++ specific keywords to the literals table
    if (inLanguage(LANGUAGE_C_FAMILY)) {

        literals["main"] = MAIN;

        literals["typedef"] = TYPEDEF;

        literals["include"] = INCLUDE;
        literals["define"] = DEFINE;
        literals["elif"] = ELIF;
        literals["endif"] = ENDIF;
        literals["error"] = ERRORPREC;
        literals["ifdef"] = IFDEF;
        literals["ifndef"] = IFNDEF;
        literals["line"] = LINE;
        literals["pragma"] = PRAGMA;
        literals["undef"] = UNDEF;

        literals["union"] = UNION;
        literals["struct"] = STRUCT;

        literals["inline"] = INLINE;
        literals["extern"] = EXTERN;

        literals["asm"] = ASM;

        literals["goto"] = GOTO;
    }

    // add all C++ and Java specific keywords to the literals table
    if (inLanguage(LANGUAGE_OO)) {
        // exception handling
        literals["try"] = TRY;
        literals["catch"] = CATCH;
        literals["throw"] = THROW;

        // class
        literals["class"] = CLASS;
        literals["public"] = PUBLIC;
        literals["private"] = PRIVATE;
        literals["protected"] = PROTECTED;

        literals["new"] = NEW;
    }

    // add all C++ specific keywords to the literals table
    if (inLanguage(LANGUAGE_CXX_FAMILY)) {

        // class
        literals["virtual"]= VIRTUAL; 
        // literals["friend"]= FRIEND;
        literals["operator"]= OPERATOR;
        literals["explicit"]= EXPLICIT;

        // namespaces
        literals["namespace"] = NAMESPACE;
        literals["using"] = USING;

        // templates
        literals["template"] = TEMPLATE;

        literals["delete"] = DELETE;

        // special C++ operators
        literals["::"] = DCOLON;

        // special C++ constant values
        literals["false"] = FALSE;
        literals["true"] = TRUE;

        // specifiers that are not needed for parsing
        /*
        literals["mutable"] = MUTABLE;
        literals["volatile"] = VOLATILE;
        */
    }

    // add all C++ specific keywords to the literals table
    if (inLanguage(LANGUAGE_CXX_0X)) {

        // concepts
        literals["concept"] = CONCEPT;
        literals["concept_map"] = CONCEPTMAP;
        literals["auto"] = AUTO;
    }

    // add all Java specific keywords to the literals table
    if (inLanguage(LANGUAGE_JAVA)) {

        // exception handling
        literals["throws"] = THROWS;
        literals["finally"] = FINALLY;

        // class
        literals["interface"] = INTERFACE;
        literals["extends"] = EXTENDS;
        literals["implements"] = IMPLEMENTS;
        
        // import
        literals["import"] = IMPORT;

        // package
        literals["package"] = PACKAGE;

        // final
        literals["final"] = FINAL;
//        literals["static"] = STATIC;
    }
}

}

protected
SPECIAL_CHARS :
        '\3'..'\377'
;
