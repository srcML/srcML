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
}

class CommentLexer extends WSLexer;

options {
    k = 2;
}

tokens {
    BLOCKCOMMENTEOL;
}

// Single-line comments (no EOL)
LINECOMMENT
    :   "//" (COMMENT_CHAR)*     { justws = false; }
;
exception
catch[...] {
        consume();
}

// Multiple-line comments
BLOCKCOMMENT 
    :   "/*" (options { greedy = false; } : COMMENT_CHAR_NEWLINE)* "*/"      { justws = false; }
//    :   "/*" (options { greedy = false; } : ~('\n' | '\r') | (EOL { $setType(BLOCKCOMMENTEOL); }) )* "*/"
;
exception
catch[...] {
}

protected
COMMENT_CHAR_NEWLINE
    : '\n' | '\r' | COMMENT_CHAR
;

protected
COMMENT_CHAR
// leave out newline, \012, and carriage return, \015.  Also, leave out escaped characters
    : '\011' | '\040'..'\045' | '\047'..';' | '=' | '?'..'\377' | ESCAPED_CHAR | CONTROL_CHAR
;

protected
ESCAPED_CHAR
    : '<' { $setText("&lt;"); } | '>' { $setText("&gt;"); } | '&' { $setText("&amp;"); }
;


