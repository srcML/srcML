/*
  OperatorLexer.g

  Copyright (C) 2004-2012  SDML (www.sdml.info)

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

class OperatorLexer extends TextLexer;

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
QMARK;

// define value in master grammar so that it depends on language
DCOLON;

MEMBERPOINTER; // = ".*";
PERIOD; // = ".";
MULTOPS; // = "*";
REFOPS;  // = "&";
MULTIMM; // = "*=";
RVALUEREF; // = "&&";

DOTDOT;
DOTDOTDOT;

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

CONSTANTS :
        '0'..'9'
    ;

OPERATORS options { testLiterals = true; } { bool star = false; } : 
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
/*
        ({ !stop && !(gt && (LA(1) == '>' || LA(1) == ':' || LA(1) == '&' || LA(1) == '*')) && (dcoloncount < 2) }?

         ( '*' { gt = true; } | '|' | ':' { ++dcoloncount; } | '`' | '=' { if (LA(1) != '=') stop = true; } | '!' | '%' | '+' | '^' | '-' |
           '&' { text.erase(realbegin); text += "&amp;"; realbegin += 4; gt = true; } | 
           '>' { if (realbegin == _begin) gt = true; text.erase(realbegin); text += "&gt;"; realbegin += 3; } | 
           '<' { text.erase(realbegin); text += "&lt;"; realbegin += 3; gt = true; }) { ++realbegin; } )+ */ 

       '+' ('+' | '=')? |
       '-' ('-' | '=' | '>' { star = true; $setText("-&gt;"); })? ({ star }? '*' { $setText("-&gt;*"); })? |
       '*' ('=')? |
//       '/' ('=')? |
       '%' ('=')? |
       '^' ('=')? |
       '|' ('|')? ('=')? |
       '`' |
       '!' ('=')? |
       ':' (':')? |
       '=' ('=')? |

       '&' { $setText("&amp;"); }
            (options { greedy = true; } : '&' { $setText("&amp;&amp;"); star = true; } | '=' { $setText("&amp;="); } )?
             ({ star }? '=' { $setText("&amp;&amp;="); } )? | 
     
       '>' { $setText("&gt;"); } |

       '<' { $setText("&lt;"); }
            (options { greedy = true; } : '<' { $setText("&lt;&lt;"); } | '=' { $setText("&lt;="); })?
            ('=' { $setText("&lt;&lt;="); })? |

//       '<' { text.erase(realbegin); text += "&lt;"; realbegin += 3; gt = true; realbegin += 3; } 
//            ('<' { text.erase(realbegin); text += "&lt;"; realbegin += 4; gt = true; })? ('=')? |

        // match these as individual operators only
        ',' | ';' | '('..')' | '[' | ']' | '{' | '}' | 

            // names can start with a @ in C#
            '@'
            ( 
            { inLanguage(LANGUAGE_CSHARP) }? NAME
            { $setType(NAME); }
            |
            )
        |

        '$'  |    // not an operator (why is it here?)
        '?' { if (inLanguage(LANGUAGE_JAVA_FAMILY)) $setType(QMARK); } | // part of ternary
        '~'  | // has to be separate if part of name

        '.' ('*' | '.' ( '.' { $setType(DOTDOTDOT); } | { $setType(DOTDOT); }) | { $setType(CONSTANTS); } CONSTANTS | ) |

        '\\' ( EOL { $setType(EOL_BACKSLASH); } )*
        )
        { startline = false; }
;
