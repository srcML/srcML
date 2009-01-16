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

}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class OperatorLexer extends CommentLexer;

options {
    k = 2;
    testLiterals = false;
}

tokens {

TEMPOPS;
TEMPOPE;
EQUAL;
LPAREN; // = "(";
DESTOP; // = "~";
LCURLY; // = "{";
RCURLY; // = "}";
LBRACKET; // = "[";
RBRACKET; // = "]";
COMMA; // = ",";
RPAREN; // = ")";
TERMINATE; // = ";";
PREPROC;
COLON; // = ":";

// define value in master grammar so that it depends on language
DCOLON;

MEMBERPOINTER; // = ".*";
PERIOD; // = ".";
MULTOPS; // = "*";
MULTIMM; // = "*=";

FALSE;
TRUE;

EOL_PLACEHOLD;
}

{
}

SPECIAL :
        '@'
;

ALLOPERATORS options { testLiterals = true; } : 
        (
            '#' {

            if (startline) {

                $setType(PREPROC); 

                // record that we are on a preprocessor line,
                // primarily so that unterminated strings in
                // a preprocessor line will end at the right spot
                onpreprocline = true; 
            }
        }    |
        "*=" |  // immediate multiplication
        '*' |   // multiplication/pointer
        ',' |
        ".*" |  // member pointer selector
        '.' | 
        ':' | "::" |
        ';' |
        '('..')' |
        '[' | ']' |
        '{' | '}' |
        '~' |   // bitwise complement
        '`')
        { startline = false; }
;

OPERATORS : 
        (
        "!=" |    // not equal
        '!'  |    // logical negation
        '$'  |    // not an operator (why is it here?)
        "%=" |    // immediate modulus
        '%'  |    // modulus 
        "&&" { $setText("&amp;&amp;"); } | // logical and
        "&=" { $setText("&amp;="); } | // immediate
        '&'  { $setText("&amp;"); $setType(MULTOPS); } |   // bitwise and / address of
        "++" |    // increment
        "+=" |    // immediate addition
        '+'  |    // addition (binary and unary)
        "--" |    // decrement (pre and post)
        "-=" |    // immediate subtraction
        '-'  |    // subtraction/unary minus
        ("->*")=> "->*" { $setText("-&gt;*"); } |   // member pointer selector
        "->"  { $setText("-&gt;"); } |   // member access
        "/=" |    // immediate division
        '/'  |    // division

        { inLanguage(LANGUAGE_JAVA) }?
        ("<<<")=>
        "<<<" { $setText("&lt;&lt;&lt;"); } |

        ("<<=")=> "<<=" { $setText("&lt;&lt;="); } |    // immediate left shift
        "<<" { $setText("&lt;&lt;"); } |                // left shift
        "<=" { $setText("&lt;="); } |                   // less than or equal to
        '<'  { $setText("&lt;"); $setType(TEMPOPS); } | // less than

        "==" | // equals
        '=' { $setType(EQUAL); } |   // assignment

        (">>=")=> ">>=" { $setText("&gt;&gt;="); } |    // immediate right shift
        ">>" { $setText("&gt;&gt;"); } |                // right shift
        ">=" { $setText("&gt;="); } |                   // greater than or equal to
        '>'  { $setText("&gt;"); $setType(TEMPOPE); } | // greater than

        '?'  | // part of ternary

        '\\' | // 
        "^=" | // immediate bitwise exclusive or
        '^'  | // bitwise exclusive or

        "||" | // logical or
        "|=" | // immediate bitwise inclusive or
        '|'    // bitwise inclusive or
        )
        { startline = false; }
;
