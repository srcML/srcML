/*
  WSLexer.g

  Copyright (C) 2002, 2003, 2004  SDML (www.sdml.info)

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
}

class WSLexer extends Lexer;

options {
    k = 1;
}

tokens {
    FORMFEED;
}

// whitespace (except for newline)
WS :
        (
            ' '  |

            // horizontal tab
            '\t' |

            // vertical tab replaced by space
            '\13' { $setText(' '); } |

            // form feed replaced by space
            '\f' { $setType(FORMFEED); }
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
        ) { onpreprocline = false; }
;
