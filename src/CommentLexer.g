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

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class CommentLexer extends WSLexer;

options {
    k = 2;
}

tokens {
    BLOCKCOMMENTEOL;
    BLOCKCOMMENT;
    LINECOMMENT;
}

// Single-line comments (no EOL)
LINECOMMENT_START
    :   "//"  { selector->push("linecomment"); }
        { 
            // have to reset, since we may eat/get eol
            justws = false;
        }
;

COMMENT_START
    :   "/*" { selector->push("blockcomment"); }
        { 
            // have to reset, since we may eat/get eol
            justws = false;
        }
;

protected
COMMENT_CHAR
    // leave out newline, \012, and carriage return, \015.  Also, leave out escaped characters
    : '\011' | '\040'..'\045' | '\047'..';' | '=' | '?'..'\377' | ESCAPED_CHAR | CONTROL_CHAR
;

/*
  Escaping character happens in the lexer.  This is for escaped characters that are part of
  comments, strings, etc.  The regular use of angle brackets and '&' are handled as operators
*/
protected
ESCAPED_CHAR
    : '<' { $setText("&lt;"); } | '>' { $setText("&gt;"); } | '&' { $setText("&amp;"); }
;
