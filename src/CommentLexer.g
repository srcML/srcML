/*
  CommentLexer.g

  Copyright (C) 2002-2009 SDML (www.sdml.info)

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
    k = 1;
}

tokens {

    COMMENT_START;
    JAVADOC_COMMENT_START;
}

{
    #include "PureCommentLexer.hpp"
}

// Single-line comments (no EOL)
LINECOMMENT_START
    :   '/' ('/' { 

              changetotextlexer(LINECOMMENT_END);

              // when we return, we may have eaten the EOL, so we will turn back on startline
              startline = true;

              onpreprocline = false;
            } |
            '*'
            { 
                if (inLanguage(LANGUAGE_JAVA) && LA(1) == '*')
                    $setType(JAVADOC_COMMENT_START);
                else
                    $setType(COMMENT_START);

                changetotextlexer(COMMENT_END);

                // comment are removed before includes are processed, so we are at the start of a line
                startline = true;
            } |

            '=' { $setType(OPERATORS); } |

            { $setType(OPERATORS); }
        )
;
