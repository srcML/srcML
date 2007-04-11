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
LPAREN = "(";
DESTOP = "~";
LCURLY = "{";
RCURLY = "}";
LBRACKET = "[";
RBRACKET = "]";
COMMA = ",";
RPAREN = ")";
TERMINATE = ";";
PREPROC;
//PREPROC = "#";
COLON = ":";

// define value in master grammar so that it depends on language
DCOLON;

PERIOD = ".";
MULTOPS = "*";
MULTIMM = "*=";

EOL_PLACEHOLD;
}

{
}

SPECIAL :
        '@'
;

ALLOPERATORS options { testLiterals = true; } : 
        '#' { if (justws) { $setType(PREPROC); onpreprocline = true; } } |
        '*' | "*=" |
        ',' |
        '.' | 
        ':' | "::" |
        ';' |
        '('..')' |
        '[' | ']' |
        '{' | '}' |
        '~' |
        '`'
;

OPERATORS : 
        '!'  |
        '$'  |
        '%'  |
        "&&" { $setText("&amp;&amp;"); } |
        "&=" { $setText("&amp;="); } |
        '&'  { $setText("&amp;"); $setType(MULTOPS); } |
        "+=" |
        '+'  |
        "--" |
        '-'  |
        "-=" |
        "->" { $setText("-&gt;"); } |
        "/=" |
        '/'  |

        { inLanguage(LANGUAGE_JAVA) }?
        ("<<<")=>
        "<<<" { $setText("&lt;&lt;&lt;"); } |
        ("<<=")=>
        "<<=" { $setText("&lt;&lt;="); } |
        "<<" { $setText("&lt;&lt;"); } |
        "<=" { $setText("&lt;="); } |
        '<'  { $setText("&lt;"); $setType(TEMPOPS); } | 

        "==" | '=' { $setType(EQUAL); } |

        ">>" { $setText("&gt;&gt;"); } |
        ">=" { $setText("&gt;="); } |
        '>'  { $setText("&gt;"); $setType(TEMPOPE); } | 

        '?'  |

        '\\' |
        "^="  |
        '^'  |

        "||" |
        "|=" |
        '|'
;
