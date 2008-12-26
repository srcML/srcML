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
    k = 1;
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

PureCommentLexer(const antlr::LexerSharedInputState& state)
	: antlr::CharScanner(state,true), mode(0), onpreprocline(false), escapecount(0), asteriskcount(0)
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

        mode = m;
    }
}

/*
  Any text inside a comment
*/
COMMENT_TEXT { 

    if (escapecount > 0)
        --escapecount;

    if (asteriskcount > 0)
        --asteriskcount;

        }
    :
(
        '\000' { $setType(CONTROL_CHAR); $setText("0x0"); } |
        '\001' { $setType(CONTROL_CHAR); $setText("0x1"); } |
        '\002' { $setType(CONTROL_CHAR); $setText("0x2"); } |
        '\003' { $setType(CONTROL_CHAR); $setText("0x3"); } |
        '\004' { $setType(CONTROL_CHAR); $setText("0x4"); } |
        '\005' { $setType(CONTROL_CHAR); $setText("0x5"); } |
        '\006' { $setType(CONTROL_CHAR); $setText("0x6"); } |
        '\007' { $setType(CONTROL_CHAR); $setText("0x7"); } |
        '\010' { $setType(CONTROL_CHAR); $setText("0x8"); } |
        '\011' /* '\t' */ |
        '\012' /* '\n' */ { 
              // end at EOL when for line comment, or the end of a string or char on a preprocessor line
              if (mode == LINECOMMENT_END || ((mode == STRING_END || mode == CHAR_END) && onpreprocline)) {
                  $setType(mode); selector->pop(); 
              }
        } |

        '\013' { $setType(CONTROL_CHAR); $setText("0xb"); } |
        '\014' { $setType(CONTROL_CHAR); $setText("0xc"); } |
        '\015' /* '\r' - misc character since converted to '\n' in input buffer */ |
        '\016' { $setType(CONTROL_CHAR); $setText("0xe"); } |
        '\017' { $setType(CONTROL_CHAR); $setText("0xf"); } |
        '\020' { $setType(CONTROL_CHAR); $setText("0x10"); } |
        '\021' { $setType(CONTROL_CHAR); $setText("0x11"); } |
        '\022' { $setType(CONTROL_CHAR); $setText("0x12"); } |
        '\023' { $setType(CONTROL_CHAR); $setText("0x13"); } |
        '\024' { $setType(CONTROL_CHAR); $setText("0x14"); } |
        '\025' { $setType(CONTROL_CHAR); $setText("0x15"); } |
        '\026' { $setType(CONTROL_CHAR); $setText("0x16"); } |
        '\027' { $setType(CONTROL_CHAR); $setText("0x17"); } |
        '\030' { $setType(CONTROL_CHAR); $setText("0x18"); } |
        '\031' { $setType(CONTROL_CHAR); $setText("0x19"); } |
        '\032' { $setType(CONTROL_CHAR); $setText("0x1a"); } |
        '\033' { $setType(CONTROL_CHAR); $setText("0x1b"); } |
        '\034' { $setType(CONTROL_CHAR); $setText("0x1c"); } |
        '\035' { $setType(CONTROL_CHAR); $setText("0x1d"); } |
        '\036' { $setType(CONTROL_CHAR); $setText("0x1e"); } |
        '\037' { $setType(CONTROL_CHAR); $setText("0x1f"); } |
        '\040' |
        '\041' |
        '\042' /* '\"' */ {/* std::cerr << "HEREREALLYFIRST" << std::endl;*/ if (escapecount == 0 && mode == STRING_END) { /* std::cerr << "HEREFIRST" << std::endl; */ $setType(mode); selector->pop();; } } |
        '\043' |
        '\044' |
        '\045' | 
        '&' { $setText("&amp;"); } |
        '\047' /* '\'' */ { if (escapecount == 0 && mode == CHAR_END) { $setType(mode); selector->pop(); } } |
        '\050' |
        '\051' |
        '\052' /* '*' */ { asteriskcount = 2; } |
        '\053' |
        '\054' |
        '\055' |
        '\056' |
        '\057' /* '/' */ { if (asteriskcount == 1 && mode == COMMENT_END) {  $setType(mode); selector->pop(); } } |
        '\060'..';' | 
        '<' { $setText("&lt;"); } | 
        '=' | 
        '>' { $setText("&gt;"); } |
        '?'..'[' |
        '\\' { if (escapecount == 0) { escapecount = 2; } } |
        ']'..'\377'
)
;
