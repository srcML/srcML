// SPDX-License-Identifier: GPL-3.0-only
/*!
 * @file CommentTextLexer.g
 *
 * @copyright Copyright (C) 2008-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML translator.
 *
 * Handles comments as separate tokens in the character stream.
 */

header "pre_include_hpp" {
}

header "pre_include_cpp" {
#if defined(__GNUC__)
#endif
#ifdef __clang__
#endif
#if defined(__GNUC__) && !defined(__clang__)
        #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#ifdef _MSC_VER
    #pragma warning(disable : 4365)  // 'argument': conversion from 'int' to 'unsigned int', signed/unsigned mismatch
    #pragma warning(disable : 4101)  // 'pe' unreferenced local variable
    #pragma warning(disable : 4456)  // declaration of 'theRetToken' hides previous local declaration
    #pragma warning(disable : 4242) // 'argument': conversion from 'int' to 'char'
#endif
}

header {
   #include <iostream>
   #include <antlr/TokenStreamSelector.hpp>
   #include <srcml_options.hpp>
   #include <Language.hpp>
}

options {
    language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class CommentTextLexer extends Lexer;

options {
    classHeaderSuffix="public Language";
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
    DQUOTE_DOCSTRING_END;
    DQUOTE_DOXYGEN_END;
    CHAR_END;
    SQUOTE_DOCSTRING_END;
    SQUOTE_DOXYGEN_END;
    CONTROL_CHAR;
    LINE_DOXYGEN_COMMENT_END;
    JAVADOC_COMMENT_END;
    DOXYGEN_COMMENT_END;
    HASHBANG_COMMENT_END;
    HASHTAG_COMMENT_END;
    PY_DQUOTE_STRING_START;
    PY_SIMPLE_DQUOTE_STRING_END;
    PY_SQUOTE_STRING_START;
    PY_SIMPLE_SQUOTE_STRING_END;
    BRACKET_ARGUMENT_END;
    CMAKE_BLOCK_COMMENT_END;
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

    int dquote_count = 0;                 // number of double-quotes for all languages (including Python)
    int dquote_count_py = 0;              // differentiates double-quote Python strings from docstrings
    bool skip_dquote_processing = false;  // ends simple Python strings/docstrings (e.g., "a", """b""", etc.)
    bool is_multiple_dquotes = false;     // two or more consecutive double-quotes
    bool in_dquotes = false;              // current token appears in a double-quote string

    int squote_count = 0;                 // number of single-quotes for all languages (including Python)
    int squote_count_py = 0;              // differentiates single-quote Python strings from docstrings
    bool skip_squote_processing = false;  // ends simple Python strings/docstrings (e.g., 'a', '''b''', etc.)
    bool is_multiple_squotes = false;     // two or more consecutive single-quotes
    bool in_squotes = false;              // current token appears in a single-quote string

    // reset all quotation mark logic variables to their initial state
    void resetQuoteState() {
        dquote_count = 0, dquote_count_py = 0, squote_count = 0, squote_count_py = 0;
        skip_dquote_processing = false, is_multiple_dquotes = false, in_dquotes = false;
        skip_squote_processing = false, is_multiple_squotes = false, in_squotes = false;
    }

    OPTION_TYPE options;

CommentTextLexer(const antlr::LexerSharedInputState& state, int language)
    : antlr::CharScanner(state, true), Language(language), mode(0), onpreprocline(false), noescape(false), delimiter1("")
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

    int lastColumn = 0;
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

    // horizontal tab
    { lastColumn = getColumn(); } '\011' /* '\t' */ {

        // expand tab if option says to
        if (isoption(options, SRCML_PARSER_OPTION_EXPAND_TABS)) {

            // remove the tab character
            text.pop_back();

            // append the spaces
            static const std::string_view spaces = "        ";
            text.append(spaces.substr(0, getColumn() - lastColumn));
        }
    } |

    '\012' /* '\n' */ { 

        // make sure to count newlines even when inside of comments
        newline();
        if (isoption(options, SRCML_OPTION_LINE))
          setLine(getLine() + (1 << 16));

        // end at EOL when for line comment, or the end of a string or char on a preprocessor line
        // Special case in C++ and C with end of comment
        if ((mode == LINE_COMMENT_END || mode == LINE_DOXYGEN_COMMENT_END) && (inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_C)) && prevLA == '\\') {

            ;

        } else if (mode == LINE_COMMENT_END || mode == LINE_DOXYGEN_COMMENT_END || mode == HASHBANG_COMMENT_END || mode == HASHTAG_COMMENT_END || (((mode == STRING_END || mode == RAW_STRING_END) || mode == CHAR_END || mode == BRACKET_ARGUMENT_END) && (onpreprocline /* || rawstring */))) {
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
        {
            dquote_count = 1;

            if (mode == PY_SIMPLE_DQUOTE_STRING_END && (prevLA != '\\' || noescape)) {
                mode = STRING_END;
                resetQuoteState();
                skip_dquote_processing = true;
                $setType(mode);
                selector->pop();
            }

            if (mode == SQUOTE_DOCSTRING_END || mode == SQUOTE_DOXYGEN_END || mode == CHAR_END)
                in_squotes = true;
        }
        (options { greedy = true; } :
            { !skip_dquote_processing && (prevLA != '\\' || noescape) }?
            '\042'
            {
                ++dquote_count;

                // 2 double quotes (+ 1 initial double quote) starts a Python docstring
                if (!in_squotes && mode == PY_DQUOTE_STRING_START && dquote_count == 2)
                    break;

                // 5 double quotes (+ 1 initial double quote) is an empty triple-quoted Python string
                if (!in_squotes && mode == PY_DQUOTE_STRING_START && dquote_count == 5)
                    break;

                // 3 ending double quotes should end the Python docstring
                if (!in_squotes && (mode == DQUOTE_DOCSTRING_END || mode == DQUOTE_DOXYGEN_END) && dquote_count == 3) {
                    resetQuoteState();
                    skip_dquote_processing = true;
                    $setType(mode);
                    selector->pop();
                    break;
                }
            }
        )*
    {
        // ignore Python double-quotes in single-quotes (e.g., '''"""a"""''')
        if (in_squotes)
            in_squotes = (LA(1) != '\047');  // keep ignoring if LA(1) is not a single quote
        else if (skip_dquote_processing)
            skip_dquote_processing = false;
        else {
            switch (mode) {
                case PY_DQUOTE_STRING_START: {
                    dquote_count_py = dquote_count + 1;
                    mode = ((dquote_count_py == 3) ? DQUOTE_DOCSTRING_END : STRING_END);
                    is_multiple_dquotes = true;

                    if (mode == DQUOTE_DOCSTRING_END && LA(1) == '\041')
                        mode = DQUOTE_DOXYGEN_END;

                    // special case for empty strings (e.g., "" and """""", """""""""""", etc.)
                    if (dquote_count_py == 2 || dquote_count_py % 6 == 0) {
                        resetQuoteState();
                        $setType(mode);
                        selector->pop();
                    }
                    break;
                }

                default: {
                    if (
                        !is_multiple_dquotes
                        && (
                            (noescape && dquote_count % 2 == 1)
                            || (!noescape && prevLA != '\\' && mode == STRING_END)
                        )
                    ) {
                        resetQuoteState();
                        $setType(mode);
                        selector->pop();
                    }

                    if (is_multiple_dquotes && (dquote_count_py == dquote_count)) {
                        resetQuoteState();
                        $setType(mode);
                        selector->pop();
                    }
                    break;
                }
            }
        }
    } |

    '\043'..'\045' | 

    '&' |

    '\047' /* '\'' */
        {
            squote_count = 1;

            if (mode == PY_SIMPLE_SQUOTE_STRING_END && (prevLA != '\\' || noescape)) {
                mode = CHAR_END;
                resetQuoteState();
                skip_squote_processing = true;
                $setType(mode);
                selector->pop();
            }

            if (mode == DQUOTE_DOCSTRING_END || mode == DQUOTE_DOXYGEN_END || mode == STRING_END)
                in_dquotes = true;
        }
        (options { greedy = true; } :
            { !skip_squote_processing && (mode == PY_SQUOTE_STRING_START || is_multiple_squotes) && (prevLA != '\\' || noescape) }?
            '\047'
            {
                ++squote_count;

                // 2 single quotes (+ 1 initial single quote) starts a Python docstring
                if (!in_dquotes && mode == PY_SQUOTE_STRING_START && squote_count == 2)
                    break;

                // 5 single quotes (+ 1 initial single quote) is an empty triple-quoted Python string
                if (!in_dquotes && mode == PY_SQUOTE_STRING_START && squote_count == 5)
                    break;

                // 3 ending single quotes should end the Python docstring
                if (!in_dquotes && (mode == SQUOTE_DOCSTRING_END || mode == SQUOTE_DOXYGEN_END) && squote_count == 3) {
                    resetQuoteState();
                    skip_squote_processing = true;
                    $setType(mode);
                    selector->pop();
                    break;
                }
            }
        )*
    {
        // ignore Python single-quotes in double-quotes (e.g., """'''a'''""")
        if (in_dquotes)
            in_dquotes = (LA(1) != '\042');  // keep ignoring if LA(1) is not a double quote
        else if (skip_squote_processing)
            skip_squote_processing = false;
        else {
            switch (mode) {
                case PY_SQUOTE_STRING_START: {
                    squote_count_py = squote_count + 1;
                    mode = ((squote_count_py == 3) ? SQUOTE_DOCSTRING_END : CHAR_END);
                    is_multiple_squotes = true;

                    // found three double-quotes and an exclamation mark
                    if (mode == SQUOTE_DOCSTRING_END && LA(1) == '\041')
                        mode = SQUOTE_DOXYGEN_END;

                    // special case for empty strings (e.g., '' and '''''', '''''''''''', etc.)
                    if (squote_count_py == 2 || squote_count_py % 6 == 0) {
                        resetQuoteState();
                        $setType(mode);
                        selector->pop();
                    }
                    break;
                }

                default: {
                    if (!is_multiple_squotes && (prevLA != '\\' && mode == CHAR_END)) {
                        resetQuoteState();
                        $setType(mode);
                        selector->pop();
                    }

                    if (is_multiple_squotes && (squote_count_py == squote_count)) {
                        resetQuoteState();
                        $setType(mode);
                        selector->pop();
                    }
                    break;
                }
            }
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

    ']' {
        // detect the end of a CMake bracket argument or block comment (both end with '=]' or ']]')
        if ((mode == BRACKET_ARGUMENT_END || mode == CMAKE_BLOCK_COMMENT_END) && (prevLA == '=' || prevLA == ']')) {
            $setType(mode); selector->pop();
        }
    } |

    '^'..'\377') {

        // not the first character anymore
        first = false;

        /* 
            About to read a newline, or the EOF.  Line comments may need
            to end before the newline is consumed. Strings and characters on a preprocessor line also need to end, even if unterminated

            Line comments are not ended if there is a line continuation character for C and C++. They do end the line comment for
            C# and Java
        */
        if (_ttype == COMMENT_TEXT && (inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_C)) && prevprevLA == '\\' && LA(1) == '\n') {

            // line continuation for C++ and C of line comments
            ;

        } else if (_ttype == COMMENT_TEXT &&
            ((LA(1) == '\n' && mode != RAW_STRING_END) || LA(1) == EOF_CHAR) &&
            ((((mode == STRING_END || mode == RAW_STRING_END) || mode == CHAR_END || mode == BRACKET_ARGUMENT_END) && (onpreprocline || mode == RAW_STRING_END))
             || mode == LINE_COMMENT_END || mode == LINE_DOXYGEN_COMMENT_END || mode == HASHBANG_COMMENT_END || mode == HASHTAG_COMMENT_END)) {

            $setType(mode);
            selector->pop();
        }
   } )+
;

protected
RSTRING_DELIMITER:
    { delimiter = ""; }
    (options { greedy = true; } : { delimiter.size() < delimiter1.size() }? { delimiter += static_cast<char>(LA(1)); }
        ~('(' | ')' | '\\' | '\n' | ' ' | '\t' ))*
;
