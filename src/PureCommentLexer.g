/*
  PureCommentLexer.g

  Copyright (C) 2008-2011  SDML (www.sdml.info)

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

// particular mode that this lexer should end in
int mode;

// preprocessor lines required unterminated strings to end early
bool onpreprocline;

PureCommentLexer(const antlr::LexerSharedInputState& state)
	: antlr::CharScanner(state,true), mode(0), onpreprocline(false)
{}

private:
    antlr::TokenStreamSelector* selector;

public:
    void setSelector(antlr::TokenStreamSelector* selector_) {
        selector=selector_;
    }

    // reinitialize comment lexer
    void init(int m, bool onpreproclinestate) {

        onpreprocline = onpreproclinestate;

        mode = m;
    }
}

/*
  Any text inside a comment, string, or character

  The idea is to match as much as possible, and put all of that in one token.  If we
  find an escape character, we need to stop the current node, issue the escape character
  and go on.
*/
COMMENT_TEXT { 

    // first time through we can match a control character, but not after that
    // since control characters must be single tokens
    bool first = true;

    // record the previous character
    int prevLA = 0;
    int prevprevLA = 0;

    int realbegin = _begin;
} :

/*
  Changing the type makes it the last token, and only handle a control character
  token as the first token
*/
        ({ _ttype == COMMENT_TEXT &&

            // only allow control characters the first (and only) time through
            (LA(1) > '\037' || LA(1) == '\011' || LA(1) == '\012' || first) }? {

            prevLA = prevprevLA;
            prevprevLA = LA(1);
         }
         (
        '\000'..'\010'
                // will only occur the first time this rule matches, and then will exit
                { $setType(CONTROL_CHAR); } |

        '\011' /* '\t' */ |

        '\012' /* '\n' */ { 

              // make sure to count newlines even when inside of comments
              newline();

              // end at EOL when for line comment, or the end of a string or char on a preprocessor line
              if (mode == LINECOMMENT_END || ((mode == STRING_END || mode == CHAR_END) && onpreprocline)) {
                  $setType(mode); selector->pop();
              }
        } |

//        '\015' /* '\r' - misc character since converted to '\n' in input buffer */ |

        '\013'..'\037'
                // will only occur the first time this rule matches, and then will exit
                { $setType(CONTROL_CHAR); } |

        '\040'..'\041' |

        '\042' /* '\"' */
                { if (prevLA != '\\' && mode == STRING_END) { $setType(mode); selector->pop(); } } |

        '\043'..'\045' | 

        '&'
                { text.erase(realbegin); text += "&amp;"; realbegin += 4; } | 
//        '&'
//                { $setText("&amp;"); } |

        '\047' /* '\'' */
                { if (prevLA != '\\' && mode == CHAR_END) { $setType(mode); selector->pop(); } } |

        '\050'..'\056' |

        '\057' /* '/' */
                { if (prevLA == '*' && mode == COMMENT_END) { $setType(mode); selector->pop(); } } |

        '\060'..';' | 

        '<'
                { text.erase(realbegin); text += "&lt;"; realbegin += 3; } |
//        '<'
//                { $setText("&lt;"); } |
        '=' | 

        '>'
                { text.erase(realbegin); text += "&gt;"; realbegin += 3; } |
//        '>'
//                { $setText("&gt;"); } |

        '?'..'[' |

        '\\'    // wipe out previous escape character
                { if (prevLA == '\\') prevprevLA = 0; } |

        ']'..'\377'
        )
        {

            ++realbegin;

            // not the first character anymoe
            first = false;

            // about to read a newline, or the end of the files.  Line comments need to end before the newline is consumed.
            // strings and characters on a preprocessor line also need to end, even if unterminated
            if (_ttype == COMMENT_TEXT && (LA(1) == '\n' || LA(1) == EOF_CHAR) &&
                (((mode == STRING_END || mode == CHAR_END) && onpreprocline)
                 || (mode == LINECOMMENT_END))) {

                $setType(mode);
                selector->pop();
            }
} )+
;
