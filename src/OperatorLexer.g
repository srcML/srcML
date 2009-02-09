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
    k = 1;
    testLiterals = false;
}

tokens {
EOL_BACKSLASH;

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
REFOPS;  // = "&";
MULTIMM; // = "*=";

FALSE;
TRUE;

SPECIAL;

ALLOPERATORS;

EOL_PLACEHOLD;
}

{
}
/*
SPECIAL :
        '@'
;
*/
//ALLOPERATORS options { testLiterals = true; } : 


OPERATORS options { testLiterals = true; } { int realbegin = _begin; } : 
        (
            '#' {

            if (startline) {

                $setType(PREPROC); 

                // record that we are on a preprocessor line,
                // primarily so that unterminated strings in
                // a preprocessor line will end at the right spot
                onpreprocline = true; 
            }
        }   |

            (( '*' | '|' | '.' | ':' | '~' | '`' | '=' | '!' | '%' | '+' | '^' | '-' |
                '&' { text.erase(realbegin); text += "&amp;"; realbegin += 4; } | 
                '<' { text.erase(realbegin); text += "&lt;"; realbegin += 3;  }) { ++realbegin; } )

            (( '*' | '|' | '.' | ':' | '~' | '`' | '=' | '!' | '%' | '+' | '^' | '-' |
                '&' { text.erase(realbegin); text += "&amp;"; realbegin += 4; } | 
                '>' { text.erase(realbegin); text += "&gt;"; realbegin += 3; } | 
                '<' { text.erase(realbegin); text += "&lt;"; realbegin += 3;  }) { ++realbegin; } )* |

        ',' |
        ';' |
        '('..')' |
        '[' | ']' |
        '{' | '}' |
        '@' |

        '$'  |    // not an operator (why is it here?)
        '?'  | // part of ternary

        (">>=")=> ">>=" { $setText("&gt;&gt;="); } |    // immediate right shift

        (">=")=> ">=" { $setText("&gt;="); } |    // immediate right shift

        '>' { $setText("&gt;"); } |

        '\\' ( EOL { $setType(EOL_BACKSLASH); } )
        )
        { startline = false; }
;
