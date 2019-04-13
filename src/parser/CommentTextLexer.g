/*!
 * @file CommentTextLexer.g
 *
 * @copyright Copyright (C) 2008-2011  srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML translator.
 *
 * The srcML translator is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML translator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML translator; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Handles comments as separate tokens in the character stream.
 */

header "pre_include_hpp" {
   #include <cstring>
   #pragma GCC diagnostic ignored "-Wunused-parameter"
}

header {
   #pragma GCC diagnostic warning "-Wunused-parameter"
   #include <iostream>
   #include "antlr/TokenStreamSelector.hpp"
   #include <srcml_types.hpp>
   #include <srcml_macros.hpp>
   #include <srcml.h>
}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class CommentTextLexer extends Lexer;

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

// ignore character escapes
bool noescape;

bool rawstring;

std::string delimiter;

bool isline;

long line_number;

OPTION_TYPE options;

CommentTextLexer(const antlr::LexerSharedInputState& state)
	: antlr::CharScanner(state,true), mode(0), onpreprocline(false), noescape(false), rawstring(false), delimiter(""), isline(false), line_number(-1)
{}

private:
    antlr::TokenStreamSelector* selector;

public:
    void setSelector(antlr::TokenStreamSelector* selector_) {
        selector=selector_;
    }

    // reinitialize comment lexer
    void init(int m, bool onpreproclinestate, bool nescape = false, bool rstring = false, std::string dstring = "", bool is_line = false, long lnumber = -1, OPTION_TYPE op = 0) {

        onpreprocline = onpreproclinestate;

        mode = m;

        noescape = nescape;
        
        rawstring = rstring;

        delimiter = dstring;

        isline = is_line;

        line_number = lnumber;

        options = op;
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

    std::string::size_type realbegin = _begin;

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

        { if(rawstring && first && LA(1) == '\012') {

                rawstring = false;
                $setType(mode);
                selector->pop();
                goto newline_break;
            } 
        }
        '\012' /* '\n' */ { 

              // make sure to count newlines even when inside of comments
              newline();
              if(isoption(options, SRCML_OPTION_LINE))
                  setLine(getLine() + (1 << 16));

              // end at EOL when for line comment, or the end of a string or char on a preprocessor line
              if (mode == LINECOMMENT_END || ((mode == STRING_END || mode == CHAR_END) && (onpreprocline || rawstring))) {

                  rawstring = false;
                  $setType(mode); selector->pop();
              }
        } |

//        '\015' /* '\r' - misc character since converted to '\n' in input buffer */ |

        '\013'..'\037'
                // will only occur the first time this rule matches, and then will exit
                { $setType(CONTROL_CHAR); } |

        '\040'..'\041' |

        '\042' /* '\"' */
                {
                    if (noescape) {

                            int count = 1;
                            while (LA(1) == '\042') {
                                match("\"");
                                ++count;
                            }

                            if (count % 2 == 1) {
                                $setType(mode); selector->pop();
                            }

                    } else if ((prevLA != '\\') && mode == STRING_END && !rawstring) {
                        $setType(mode); selector->pop();
                    } 
                } |

        '\043'..'\045' | 

        '&'
                { text.erase(realbegin); text += "&amp;"; realbegin += 4; } | 
//        '&'
//                { $setText("&amp;"); } |

        '\047' /* '\'' */
                { if (prevLA != '\\' && mode == CHAR_END) { $setType(mode); selector->pop(); } } |

        '\050' |

        '\051' /* ')' */ { if(rawstring) {

                std::string::size_type pos = 0;
                while(pos < delimiter.size() && LA(1) == delimiter[pos] && LA(1) != '\n') {
                    ++pos;
                    consume();
                }

                if(pos == delimiter.size() && LA(1) != '\n') {
                    rawstring = false;
                }

            }
        } |

        '\052'..'\056' |

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
                { if (prevLA == '\\') prevprevLA = 0; } 

                {
                    if ((mode == STRING_END || mode == CHAR_END) && onpreprocline) {

                        // skip over whitespace after line continuation character
                        while (LA(1) == ' ') {
                            consume();
                            prevLA = 0;
                            prevprevLA = 0;
                        }

                        // treat newline as part of string
                        if (LA(1) == '\n') {

                            consume();
                            newline();
                            if(isoption(options, SRCML_OPTION_LINE))
                                setLine(getLine() + (1 << 16));
                            prevLA = 0;
                            prevprevLA = 0;
                        }
                    }
                }

            |

        ']'..'\377'
        )
        {
         newline_break:

            ++realbegin;

            // not the first character anymore
            first = false;

            // about to read a newline, or the end of the files.  Line comments need to end before the newline is consumed.
            // strings and characters on a preprocessor line also need to end, even if unterminated
            if (_ttype == COMMENT_TEXT && (LA(1) == '\n' || LA(1) == EOF_CHAR) &&
                (((mode == STRING_END || mode == CHAR_END) && (onpreprocline || rawstring))
                 || (mode == LINECOMMENT_END))) {
                rawstring = false;
                $setType(mode);
                selector->pop();
            }

        _saveIndex = _saveIndex + 0;

       } )+
;
