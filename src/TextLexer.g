/*
  TextLexer.g

  Copyright (C) 2002-2008  SDML (www.sdml.info)

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

  An antlr Lexer that passes tokens consisting of either
  whitespace or non-whitespace.  Non-whitespace is output
  in a TEXT token.  Contiguous word characters are combined
  in one token.

  Base lexer for srcML Lexer.
  Identifies keywords in the stream.  Non-keywords and other text,
  including whitespace, is passed through unused.
*/

header {

    #include <iostream>
}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class TextLexer extends OperatorLexer;

options {
	k = 1;
	testLiterals = false; 
}

{
public:

bool onpreprocline;

}

STRING_START :
        { startline = false; }
        (
            // double quoted string
            // strings are allowed to span multiple lines
            // special case is when it is one a preprocessor line, e.g.,
            // #define a "abc
            // note that the "abc does not end at the end of this line,
            // but the #define must end, so EOL is not a valid string character
            '"' { changetotextlexer(STRING_END); }
        )
;

// character literal or single quoted string
CHAR_START :
        { startline = false; }
        '\'' { changetotextlexer(CHAR_END); }
;

CONSTANTS :
        { startline = false; }
        DIGIT (options { greedy = true; } : DIGIT | 'x' | 'A'..'F' | 'a'..'f')*
        ("." | DIGIT)* (NAMECHAR)*
;

NAME options { testLiterals = true; } { char lastchar = LA(1); } :
        { startline = false; }
        NAMECHAR (

            { lastchar == 'L' }?
            { $setType(STRING_START); } STRING_START |

            (DIGIT | NAMECHAR)*
        )
;

protected
NAMECHAR :
        'a'..'z' | 'A'..'Z' | '_'
;

protected
DIGIT :
        '0'..'9'
;
