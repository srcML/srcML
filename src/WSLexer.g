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
EOL :   { justws = true; }
        '\n'
        { 
            // onpreprocline is turned on when on a preprocessor line
            // to prevent mostly string ending problems.
            // it has to be turned back on when the EOL is reached
            onpreprocline = false;
        }
;

/*
  Encode the control character in the text, so that is can be
  issued in an escape character.
*/
CONTROL_CHAR :
        '\000' { $setText("0x0"); } |
        '\001' { $setText("0x1"); } |
        '\002' { $setText("0x2"); } |
        '\003' { $setText("0x3"); } |
        '\004' { $setText("0x4"); } |
        '\005' { $setText("0x5"); } |
        '\006' { $setText("0x6"); } |
        '\007' { $setText("0x7"); } |
        '\010' { $setText("0x8"); } |

        '\013' { $setText("0xb"); } |
        '\014' { $setText("0xc"); } |

        '\016' { $setText("0xe"); } |
        '\017' { $setText("0xf"); } |
        '\020' { $setText("0x10"); } |
        '\021' { $setText("0x11"); } |
        '\022' { $setText("0x12"); } |
        '\023' { $setText("0x13"); } |
        '\024' { $setText("0x14"); } |
        '\025' { $setText("0x15"); } |
        '\026' { $setText("0x16"); } |
        '\027' { $setText("0x17"); } |
        '\030' { $setText("0x18"); } |
        '\031' { $setText("0x19"); } |
        '\032' { $setText("0x1a"); } |
        '\033' { $setText("0x1b"); } |
        '\034' { $setText("0x1c"); } |
        '\035' { $setText("0x1d"); } |
        '\036' { $setText("0x1e"); } |
        '\037' { $setText("0x1f"); }
;
