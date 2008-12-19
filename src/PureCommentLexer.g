/*
  CommentLexer.g

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

/*
  Identifies comments in the character stream.
*/

header {
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
    defaultErrorHandler=false;
}

{
private:
    antlr::TokenStreamSelector* selector;
public:
    void setSelector(antlr::TokenStreamSelector* selector_) {
        selector=selector_;
    }
}

/*
  Any text inside a comment
*/
COMMENT_TEXT
    :   COMMENT_CHAR_NEWLINE
;

/*
  Finished with comment, so handle and switch back to main lexer
*/
COMMENT_END
    :   "*/"
        {
            selector->select("main");
        }
;

protected
COMMENT_CHAR_NEWLINE
    : '\n' | '\r' | COMMENT_CHAR
;

protected
COMMENT_CHAR
    // leave out newline, \012, and carriage return, \015.  Also, leave out escaped characters
    : '\011' | '\040'..'\045' | '\047'..';' | '=' | '?'..'\377' | ESCAPED_CHAR
;

/*
  Escaping character happens in the lexer.  This is for escaped characters that are part of
  comments, strings, etc.  The regular use of angle brackets and '&' are handled as operators
*/
protected
ESCAPED_CHAR
    : '<' { $setText("&lt;"); } | '>' { $setText("&gt;"); } | '&' { $setText("&amp;"); }
;

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
CONTROL_CHAR { int n = 0; char ns[] = { '0', 'x', 0, 0, 0 }; } :
        { 
            n = LA(1);
        }
        (
        ('\000'..'\010')                  { ns[2] = n + '0'; } |
        ('\013'..'\014' | '\016'..'\017') { ns[2] = (n - 0xa) + 'a'; } |
        ('\020'..'\031')                  { ns[2] = '1'; ns[3] = (n - 0x10) + '0'; } |
        ('\032'..'\037')                  { ns[2] = '1'; ns[3] = (n - 0x10 - 0xa) + 'a'; }
        )
        {
            $setText(ns);
        }
;
