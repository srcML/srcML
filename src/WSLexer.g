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
}

tokens {
    FORMFEED;
}

// whitespace (except for newline)
WS :
        (
            // single space
            ' '  |

            // horizontal tab
            '\t' |

            CONTROL_CHAR
        )+
;

// end of line
EOL :   { justws = true; }
        (
        // UNIX
        '\n' | 

        // Mac and DOS
        '\r' (options { greedy = true; } : '\n')?
        // Mac and DOS line endings converted to single newline
        // when output on a platform will be the correct one for the
        // platform used
        { $setText('\n'); }
        )
        { 
            // onpreprocline is turned on when on a preprocessor line
            // to prevent mostly string ending problems.
            // it has to be turned back on when the EOL is reached
            onpreprocline = false;
        }
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
protected
CONTROL_CHAR { std::string s = "<"; int n = 0; char ns[3] = { 0, 0, 0 }; } :
        { 
            s += srcuri;
            s += "escape char=\"0x"; 
            n = LA(1);
        }
        (
        ('\000'..'\010')                  { ns[0] = n + '0'; } |
        ('\013'..'\014' | '\016'..'\017') { ns[0] = (n - 10) + 'a'; } |
        ('\020'..'\031')                  { ns[0] = '1'; ns[1] = (n - 16) + '0'; } |
        ('\032'..'\037')                  { ns[0] = '1'; ns[1] = (n - 26) + 'a'; }
        )
        {
            s += ns;
            s += "\"/>";

            $setText(s);
        }
;
