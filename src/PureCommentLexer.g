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

bool escaped;
bool flipescaped;

bool asterisk;
bool flipasterisk;

bool onpreprocline;

PureCommentLexer(const antlr::LexerSharedInputState& state)
	: antlr::CharScanner(state,true), mode(0), escaped(false), flipescaped(false),
      asterisk(false), flipasterisk(false), onpreprocline(false)
{
}

private:
    antlr::TokenStreamSelector* selector;
public:
    void setSelector(antlr::TokenStreamSelector* selector_) {
        selector=selector_;
    }

    void init(int m, bool onpreproclinestate) {
        escaped = false;
        flipescaped = false;

        asterisk = false;
        flipasterisk = false;

        onpreprocline = onpreproclinestate;

        mode = m;
    }
}

/*
  Any text inside a comment
*/
COMMENT_TEXT { 

    if (!flipescaped) escaped = false; flipescaped = false; 
    if (!flipasterisk) asterisk = false; flipasterisk = false; 

        }
    ://   { mode == COMMENT_END   }? "*/"             { std::cerr << "HERE" << std::endl; $setType(COMMENT_END); selector->pop(); } |

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
        '\012' /* '\n' */ { if (mode == LINECOMMENT_END) { $setType(LINECOMMENT_END); selector->pop(); }
                            if (mode == STRING_END && onpreprocline) { $setType(STRING_END); selector->pop(); }

        } |

        '\013' { $setType(CONTROL_CHAR); $setText("0xb"); } |
        '\014' { $setType(CONTROL_CHAR); $setText("0xc"); } |
        '\015' /* '\r' */ { if (mode == LINECOMMENT_END) { $setType(LINECOMMENT_END); selector->pop(); } 
                            if (mode == STRING_END && onpreprocline) { $setType(STRING_END); selector->pop(); }
        } |
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
        '\042' /* '\"' */ { if (!escaped && mode == STRING_END) { $setType(STRING_END); selector->pop();; } } |
        '\043' |
        '\044' |
        '\045' | 
        '&' { $setText("&amp;"); } |
        '\047' /* '\'' */ { if (!escaped && mode == CHAR_END) { $setType(CHAR_END); selector->pop(); } } |
        '\050' |
        '\051' |
        '\052' /* '*' */ { asterisk = true; flipasterisk = true; } |
        '\053' |
        '\054' |
        '\055' |
        '\056' |
        '\057' /* '/' */ { if (asterisk && mode == COMMENT_END) {  $setType(COMMENT_END); selector->pop(); } } |
        '\060'..';' | 
        '<' { $setText("&lt;"); } | 
        '=' | 
        '>' { $setText("&gt;"); } |
        '?'..'[' |
        '\\' { if (!escaped) { escaped = true; flipescaped = true; } } |
        ']'..'\377'
;

/*
  Finished with comment, so handle and switch back to main lexer
*/

/*
  Encode the control character in the element escape, e.g., for control
  character \001:

    <escape char="0x1"/>

  where the element is prefixed with the src uri prefix (which can be
  changed by options).

  This has to be done here in the lexer because if the character occurs
  in a string or in a comment, they are completely detected and formed
  in the lexer.
*/
