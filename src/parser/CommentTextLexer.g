/*!
 * @file CommentTextLexer.g
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Handles comments as separate tokens in the character stream.
 */

header "pre_include_hpp" {
   #include <cstring>
   #pragma GCC diagnostic ignored "-Wunknown-pragmas"
   #pragma GCC diagnostic ignored "-Wunknown-warning-option"
   #pragma GCC diagnostic ignored "-Wunused-parameter"
   #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
}

header {
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
    WHOLE_COMMENT;
    BLOCK_COMMENT_END;
    LINE_COMMENT_END;
    RAW_STRING_END;
    STRING_END;
    CHAR_END;
    CONTROL_CHAR;
    LINE_DOXYGEN_COMMENT_END;
    JAVADOC_COMMENT_END;
    DOXYGEN_COMMENT_END;
}

{
public:

// particular mode that this lexer should end in
int mode;

// preprocessor lines required unterminated strings to end early
bool onpreprocline;

// ignore character escapes
bool noescape;

std::string delimiter1;

std::string delimiter;

int dquote_count = 0;

OPTION_TYPE options;

CommentTextLexer(const antlr::LexerSharedInputState& state)
    : antlr::CharScanner(state,true), mode(0), onpreprocline(false), noescape(false), delimiter1("")
{}

private:
    antlr::TokenStreamSelector* selector;

public:
    void setSelector(antlr::TokenStreamSelector* selector_) {
        selector=selector_;
    }

    // reinitialize comment lexer
    void init(int m, bool onpreproclinestate, bool nescape = false, std::string dstring = "", bool /* is_line */ = false, long /* lnumber */ = -1, OPTION_TYPE op = 0) {

        onpreprocline = onpreproclinestate;
        mode = m;
        noescape = nescape;
        delimiter1 = dstring;
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
        if (isoption(options, SRCML_OPTION_LINE))
          setLine(getLine() + (1 << 16));

        // end at EOL when for line comment, or the end of a string or char on a preprocessor line
        if (mode == LINE_COMMENT_END || mode == LINE_DOXYGEN_COMMENT_END || (((mode == STRING_END || mode == RAW_STRING_END) || mode == CHAR_END) && (onpreprocline /* || rawstring */))) {
          $setType(mode);
          selector->pop();
        }
    } |

    //        '\015' /* '\r' - misc character since converted to '\n' in input buffer */ |

    '\013'..'\037'
        // will only occur the first time this rule matches, and then will exit
        { $setType(CONTROL_CHAR); } |

    '\040'..'\041' |

    '\042' /* '\"' */
        { dquote_count = 1; }
        (options { greedy = true; } : { prevLA != '\\' }? '\042' { ++dquote_count; })*
    {
        if ((noescape && (dquote_count % 2 == 1)) ||
            (!noescape && (prevLA != '\\') && (mode == STRING_END))) {
            $setType(mode);
            selector->pop();
        }
    } |

    '\043'..'\045' | 

    '&' |

    '\047' /* '\'' */ {
        if (prevLA != '\\' && mode == CHAR_END) {
            $setType(mode);
            selector->pop();
        }
    } |

    '\050' |

    '\051' /* ')' */
        // collect the rstring delimiter
        ({ mode == RAW_STRING_END }? RSTRING_DELIMITER)?
        {
            // for R-strings (C++) compare the at the end to the one from the start
            // after the delimiter, there must be a quote to end the string
            if (mode == RAW_STRING_END && delimiter == delimiter1 && LA(1) == '"') {
                mode = STRING_END;
            }
        } |

    '\052'..'\056' |

    '\057' /* '/' */
        { if (prevLA == '*' && ((mode == BLOCK_COMMENT_END) ||
                                (mode == JAVADOC_COMMENT_END) ||
                                (mode == DOXYGEN_COMMENT_END) ) )
            { $setType(mode); selector->pop(); }
        } |

    '\060'..';' | 

    '<' |
    '=' | 
    '>' |
    '?'..'[' |

    '\\' { 
        // wipe out previous escape character
        if (prevLA == '\\') {
            prevprevLA = 0;
        }
    } |

    ']'..'\377') {

        // not the first character anymore
        first = false;

        /* 
            About to read a newline, or the EOF.  Line comments need
            to end before the newline is consumed. Strings and characters on a preprocessor line also need to end, even if unterminated
        */
        if (_ttype == COMMENT_TEXT &&
            ((LA(1) == '\n' && mode != RAW_STRING_END) || LA(1) == EOF_CHAR) &&
            ((((mode == STRING_END || mode == RAW_STRING_END) || mode == CHAR_END) && (onpreprocline || mode == RAW_STRING_END))
             || mode == LINE_COMMENT_END || mode == LINE_DOXYGEN_COMMENT_END)) {

            $setType(mode);
            selector->pop();
        }
   } )+
;

protected
RSTRING_DELIMITER:
    { delimiter = ""; }
    (options { greedy = true; } : { delimiter.size() < delimiter1.size() }? { delimiter += LA(1); } 
        ~('(' | ')' | '\\' | '\n' | ' ' | '\t' ))*
;
