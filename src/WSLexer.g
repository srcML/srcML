/*
  WSLexer.g

  Copyright (C) 2002-2006  SDML (www.sdml.info)

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
  Identifies white space in the character stream.
*/

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class WSLexer extends Lexer;

options {
    k = 1;
    charVocabulary = '\000'..'\377';
    importVocab=PureCommentLexer;
}

// whitespace (except for newline)
WS :
        (
            // single space
            ' '  |

            // horizontal tab
            '\t'
        )+
;

// end of line
EOL :
        '\n'
        { 
            // onpreprocline is turned on when on a preprocessor line
            // to prevent mostly string ending problems.
            // it has to be turned back on when the EOL is reached
            onpreprocline = false;

            // mark that we are starting a new line, so preproc
            // can be detected
            startline = true;

            // record to new lines for optional positions
            newline();
        }
;
/*
EOL_BACKSLASH :
        '\\' EOL
    ;
*/
/*
  Encode the control character in the text, so that is can be
  issued in an escape character.
*/
CONTROL_CHAR :
        { startline = true; }
        (
        '\000'..'\010' |
        '\013'..'\014' |
        '\016'..'\037'
        )
;
