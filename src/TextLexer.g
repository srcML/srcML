/*
  TextLexer.g

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
}

class TextLexer extends OperatorLexer;

options {
	k = 2;
	testLiterals = false; 
}

{
public:

bool onpreprocline;
}

STRING :
        (
        // double quoted string
        '"' (ESC | STRING_CHARACTER | { !onpreprocline }? EOL)* ('"')? |
        'L' '"' (ESC | STRING_CHARACTER | { !onpreprocline }? EOL)* ('"')? |
        'L' (DIGITS | NAMECHAR)* { $setType(NAME); }
        )
        { justws = false; }
;

CHAR :
        // character literal or single quoted string
        '\'' (
                ESC |
                CHAR_CHARACTER
        )* ('\'')?
        { justws = false; }
;

protected
ESC :
        '\\' .
;

CONSTANTS : { justws = false; }
        DIGITS (".")? (DIGITS)?
;

NAME options { testLiterals = true; } :  { justws = false; }
        NAMECHARNOEL (DIGITS | NAMECHAR)*
;

// make sure to match strange characters so that they don't cause errors
OTHER : 
        '\177'..'\377'
;

EOL_BACKSLASH :
        '\\' EOL
;

protected
STRING_CHARACTER
// leave out newline, \012, carriage return, \015, double quote , \042, and backslash, \134.  Also, leave out escaped characters
    : '\0'..'\011' | '\013'..'\014' | '\016'..'\041' | '\043'..'\045' | '\047'..';' | '=' | '?'..'\133' | '\135'..'\377' | ESCAPED_CHAR
;

protected
CHAR_CHARACTER
// leave out newline, \012, carriage return, \015, single quote , \047, and backslash, \134.  Also, leave out escaped characters
    : '\0'..'\011' | '\013'..'\014' | '\016'..'\045' | '\050'..';' | '=' | '?'..'\133' | '\135'..'\377' | ESCAPED_CHAR
;

protected   
DIGITS :
        DIGIT
        (options { greedy = true; } : HEX_DIGIT)*
; 

protected
NAMECHARNOEL :
        'a'..'z' | 'A'..'K' | 'M'..'Z' | '_'
;

protected
NAMECHAR :
        'a'..'z' | 'A'..'Z' | '_'
;

protected
DIGIT :
        '0'..'9'
;

protected
HEX_DIGIT :
        DIGIT | 'x' | 'A'..'F' | 'a'..'f'
;
