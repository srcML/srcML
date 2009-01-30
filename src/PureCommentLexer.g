/*
  PureCommentLexer.g

  Copyright (C) 2008  SDML (www.sdml.info)

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

  Handles comments as separate tokens in the character stream.
*/

header {
   #include <iostream>
   #include "antlr/TokenStreamSelector.hpp"
}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class PureCommentLexer extends Lexer;

options {
    k = 2;
    noConstructors = true;
    defaultErrorHandler = false;
    testLiterals = false;
}

tokens {

    // never explicitly given (only set to)
    // so must be declared
    COMMENT_END;
    LINECOMMENT_END;
    STRING_END;
    CHAR_END;
    CONTROL_CHAR;
}

{
public:

int mode;

bool onpreprocline;

int escapecount;
int asteriskcount;

int n;

bool lasttoken;

PureCommentLexer(const antlr::LexerSharedInputState& state)
	: antlr::CharScanner(state,true), mode(0), onpreprocline(false), escapecount(0), asteriskcount(0), lasttoken(false)
{
}

private:
    antlr::TokenStreamSelector* selector;
public:
    void setSelector(antlr::TokenStreamSelector* selector_) {
        selector=selector_;
    }

    void init(int m, bool onpreproclinestate) {

        escapecount = 0;
        asteriskcount = 0;

        onpreprocline = onpreproclinestate;

        lasttoken = false;

        mode = m;
    }
}

/*
  Any text inside a comment
*/
COMMENT_TEXT { 

    lasttoken = LA(2) == '\n' && (mode == STRING_END || mode == CHAR_END) && onpreprocline;
    if (lasttoken) {
       $setType(mode);
       selector->pop(); 
    }

    if (escapecount > 0)
        --escapecount;

    if (asteriskcount > 0)
        --asteriskcount;

        }
    :
(
        '\000'..'\010' { $setType(CONTROL_CHAR); } |
        '\011' /* '\t' */ |
        '\012' /* '\n' */ { 
              // end at EOL when for line comment, or the end of a string or char on a preprocessor line
              if (mode == LINECOMMENT_END || ((mode == STRING_END || mode == CHAR_END) && onpreprocline)) {
                  $setType(mode); selector->pop(); 
              }
        } |

//        '\015' /* '\r' - misc character since converted to '\n' in input buffer */ |
        '\013'..'\037' { $setType(CONTROL_CHAR); } |
        '\040'..'\041' |
        '\042' /* '\"' */ { if (escapecount == 0 && mode == STRING_END && !lasttoken) { $setType(mode); selector->pop();; } } |
        '\043'..'\045' | 
        '&' { $setText("&amp;"); } |
        '\047' /* '\'' */ { if (escapecount == 0 && mode == CHAR_END && !lasttoken) { $setType(mode); selector->pop(); } } |
        '\050'..'\051' |
        '\052' /* '*' */ { asteriskcount = 2; } |
        '\053'..'\056' |
        '\057' /* '/' */ { if (asteriskcount == 1 && mode == COMMENT_END) { $setType(mode); selector->pop(); } } |
        '\060'..';' | 
        '<' { $setText("&lt;"); } | 
        '=' | 
        '>' { $setText("&gt;"); } |
        '?'..'[' |
        '\\' { if (escapecount == 0) { escapecount = 2; } } |
        ']'..'\377'
)
;
