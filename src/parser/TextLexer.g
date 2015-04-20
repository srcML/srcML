/*!
 * @file TextLexer.g
 *
 * @copyright Copyright (C) 2002-2014  srcML, LLC. (www.srcML.org)
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
 * An antlr Lexer that passes tokens consisting of either
 * whitespace or non-whitespace.  Non-whitespace is output
 * in a TEXT token.  Contiguous word characters are combined
 * in one token.
 *
 * Base lexer for srcML Lexer.
 * Identifies keywords in the stream.  Non-keywords and other text,
 * including whitespace, is passed through unused.
 */

header {

    #include <iostream>

}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class TextLexer extends Lexer;

options {
	k = 1;
	testLiterals = false; 
    charVocabulary = '\000'..'\377';
    importVocab=CommentTextLexer;
}

tokens {
    COMMENT_START;
    JAVADOC_COMMENT_START;
    DOXYGEN_COMMENT_START;
    LINE_DOXYGEN_COMMENT_START;
    CHAR_START;
    MACRO_NAME;
    COMPLEX_NUMBER;
}

{
public:

bool onpreprocline;
bool rawstring;
std::string delimiter;

}

STRING_START :
        { startline = false; }
        (
            // double quoted string
            // strings are allowed to span multiple lines
            // special case is when it is one a preprocessor line, e.g.,
            // #define a "abc
            // note that the "abc does not end at the end of this line,
            // but the #define must end, so EOL is not a valid string character
            '"' {

                if(rawstring) {
                    while(LA(1) != '(' && LA(1) != '\n') {
                        delimiter += LA(1);
                        consume();
                    }

                    if(LA(1) == '\n') {
                         delimiter = "";
                    } else {
                        match('(');
                    }

                }
                changetotextlexer(STRING_END); } |

            // character literal or single quoted string
            '\'' { $setType(CHAR_START); changetotextlexer(CHAR_END); }
        )
        { atstring = false; rawstring = false; delimiter = ""; }
;

CONSTANTS :
        { startline = false; }
        ('0'..'9') (options { greedy = true; } : '0'..'9' | '_')*
        (options { greedy = true; } : '.' | '0'..'9')*
        (options { greedy = true; } : 'e' ('+' | '-')* ('0'..'9')*)?
        (options { greedy = true; } : 'i' { $setType(COMPLEX_NUMBER); })*
        (options { greedy = true; } : NAME)*

        {
            if(onpreprocline && isline) {
                line_number = atoi(text.substr(_begin, text.length()-_begin).c_str()); 
            }
        }
;

NAME options { testLiterals = true; } { char lastchar = LA(1); } :
        { startline = false; }
        ('a'..'z' | 'A'..'Z' | '_' | '\200'..'\377' | '$')
        (

            { lastchar == 'L' || lastchar == 'U' || lastchar == 'u' }?
            { $setType(STRING_START); } STRING_START |

            { inLanguage(LANGUAGE_CXX) && lastchar == 'R' }?
            { $setType(STRING_START); rawstring = true; } STRING_START |

            { lastchar == 'u' }? ('8' '"')=> '8'
            { $setType(STRING_START); } STRING_START |

            { inLanguage(LANGUAGE_CXX) && lastchar == 'u' }? ('8' 'R' '"')=> '8' 'R'
            { $setType(STRING_START); rawstring = true; } STRING_START |

            { inLanguage(LANGUAGE_CXX) && (lastchar == 'L' || lastchar == 'U' || lastchar == 'u')}? ('R' '"')=> 'R'
            { $setType(STRING_START); rawstring = true; } STRING_START |

            ((options { greedy = true; } : '0'..'9' | 'a'..'z' | 'A'..'Z' | '_' | '\200'..'\377' | '$')*)

/*
            if(false) {
                static const boost::regex macro_name_match("[A-Z][A-Z_]+");
                static const boost::match_flag_type flags = boost::match_default;
            
                std::string temp_name = text.substr(_begin, text.length()-_begin);
                
                std::string::const_iterator start = temp_name.begin();
                std::string::const_iterator end = temp_name.end();
                boost::match_results<std::string::const_iterator> what;
                bool match_res = boost::regex_search(start, end, what, macro_name_match, flags);
                
                bool is_regex_match = match_res && 
                    (what[0].length() == (boost::match_results<std::string::const_iterator>::difference_type)temp_name.size());
                
                if(is_regex_match) $setType(MACRO_NAME);
            }
*/
        )
;

// Single-line comments (no EOL)
LINECOMMENT_START
    :   '/' ('/' { 

                if(inLanguage(LANGUAGE_CXX) && (LA(1) == '/' || LA(1) == '!'))
                    $setType(LINE_DOXYGEN_COMMENT_START);
                
                changetotextlexer(LINECOMMENT_END);

                // when we return, we may have eaten the EOL, so we will turn back on startline
                startline = true;

                onpreprocline = false;
            } |
            '*'
            { 
                if (inLanguage(LANGUAGE_JAVA) && LA(1) == '*') {

                    if(next_char()  != '/')
                        $setType(JAVADOC_COMMENT_START);
                    else
                        $setType(COMMENT_START);

                } else if (inLanguage(LANGUAGE_CXX) && (LA(1) == '*' || LA(1) == '!')) {

                    if(next_char() != '/')
                        $setType(DOXYGEN_COMMENT_START);
                    else
                        $setType(COMMENT_START);

                } else {

                    $setType(COMMENT_START);

                }

                changetotextlexer(COMMENT_END);

                // comment are removed before includes are processed, so we are at the start of a line
                startline = true;
            } |

            '=' { $setType(ASSIGNMENT); } |

            { $setType(OPERATORS); }
        )

;

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
            if(isoption(options, SRCML_OPTION_LINE))
                setLine(getLine() + (1 << 16));
            if(isline && line_number > -1) setLine((int)(line_number << 16 | (getLine() & 0xFFFF)));
            isline = false;
            line_number = -1;
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
