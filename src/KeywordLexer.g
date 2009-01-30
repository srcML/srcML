/*
  KeywordLexer.g

  Copyright (C) 2002, 2003, 2004  SDML (www.sdml.info)

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

}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class KeywordLexer extends TextLexer;

options {
    k = 1;
    testLiterals = false;
    noConstructors = true;
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
}
{
public:
KeywordLexer::KeywordLexer(std::istream& in)
	: antlr::CharScanner(new antlr::CharBuffer(in),true)
{
	initLiterals();
}

}

protected
SPECIAL_CHARS : { startline = false; }
        '\3'..'\377'
;

