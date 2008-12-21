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
	k = 2;
	testLiterals = false; 
}

{
public:

bool onpreprocline;
}

STRING_START :
        (
            // double quoted string
            // strings are allowed to span multiple lines
            // special case is when it is one a preprocessor line, e.g.,
            // #define a "abc
            // note that the "abc does not end at the end of this line,
            // but the #define must end, so EOL is not a valid string character
            '"' { 
                selector->push("text"); 
                ((PureCommentLexer* ) (selector->getStream("text")))->init(STRING_END, onpreprocline);
            } |

            'L' '"' { 
                selector->push("text"); 
                ((PureCommentLexer* ) (selector->getStream("text")))->init(STRING_END, onpreprocline);
            } |

            'L' (DIGITS | NAMECHAR)* { $setType(NAME); }
        )
        { justws = false; }
;

// character literal or single quoted string
CHAR_START :
        '\'' { 
            selector->push("text"); 
            ((PureCommentLexer* ) (selector->getStream("text")))->init(CHAR_END, onpreprocline);

            justws = false;
        }
;

CONSTANTS : { justws = false; }
        DIGITS (".")? (DIGITS)? ('u' | 'U' | 'f' | 'F' | 'l' | 'L')?
;

NAME options { testLiterals = true; } :  { justws = false; }
        NAMECHARNOEL (DIGITS | NAMECHAR)*
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
