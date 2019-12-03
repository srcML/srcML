/*!
 * @file TextLexer.g
 *
 * @copyright Copyright (C) 2002-2019 srcML, LLC. (www.srcML.org)
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
 * An antlr Lexer that passes tokens consisting of either
 * whitespace or non-whitespace.  Non-whitespace is output
 * in a TEXT token.  Contiguous word characters are combined
 * in one token.
 *
 * Base lexer for srcML Lexer.
 * Identifies keywords in the stream.  Non-keywords and other text,
 * including whitespace, is passed through unused.
 */

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
    importVocab = CommentTextLexer;
}

tokens {
    BLOCK_COMMENT_START;
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
    std::string delimiter;
}

STRING_START :
    { startline = false; }

    // double quoted string
    // strings are allowed to span multiple lines
    // special case is when it is one a preprocessor line, e.g.,
    // #define a "abc
    // note that the "abc does not end at the end of this line,
    // but the #define must end, so EOL is not a valid string character
    '"' {
        changetotextlexer(STRING_END); 

        atstring = false;
    }
;

protected
RAW_STRING_START :
    { startline = false; }

    // double quoted string
    // strings are allowed to span multiple lines
    // special case is when it is one a preprocessor line, e.g.,
    // #define a "abc
    // note that the "abc does not end at the end of this line,
    // but the #define must end, so EOL is not a valid string character
    '"' RSTRING_DELIMITER ('(')?
    {
        changetotextlexer(RAW_STRING_END, delimiter); 

        atstring = false;
    }
;

protected
RSTRING_DELIMITER:
    { delimiter = ""; }
    (options { greedy = true; } : { delimiter += LA(1); } ~('(' | ')' | '\\' | '\n' | ' ' | '\t' ))*
;

CHAR_START :
    { startline = false; }

    // character literal or single quoted string
    '\'' { $setType(CHAR_START); changetotextlexer(CHAR_END); }
;

CONSTANTS :
    { startline = false; }
    ('0'..'9') (options { greedy = true; } : '0'..'9' | '_')*
    (options { greedy = true; } : '.' | '0'..'9')*
    (options { greedy = true; } : 'e' ('+' | '-')* ('0'..'9')*)?
    (options { greedy = true; } : 'i' { $setType(COMPLEX_NUMBER); })*
    (options { greedy = true; } : NAME)*
    {
        //firstpreprocline = false;
        if (onpreprocline && isline) {
            line_number = atoi(text.substr(_begin, text.length()-_begin).c_str()); 
        }
    }
;

NAME options { testLiterals = true; } :
    { startline = false; }
    ('a'..'z' | 'A'..'Z' | '_' | '\200'..'\377' | '$')
    ((options { greedy = true; } : '0'..'9' | 'a'..'z' | 'A'..'Z' | '_' | '\200'..'\377' | '$')*)
    (
        { text == "L" || text == "U" || text == "u" || text == "u8" }?
        { $setType(STRING_START); } STRING_START |

        { inLanguage(LANGUAGE_CXX) && (text == "R" || text == "u8R" || text == "LR" || text == "UR" || text == "uR") }?
        { $setType(STRING_START); } RAW_STRING_START
    )?
;

// Single-line comments (no EOL)
LINE_COMMENT_START options { testLiterals = true; } { int mode = 0; } : '/' 
    ('/' 
        { mode = LINE_COMMENT_END; }
        (('/' | '!') {
            $setType(LINE_DOXYGEN_COMMENT_START);
            mode = LINE_DOXYGEN_COMMENT_END; 
        })? |
    '*'
        { 
            $setType(BLOCK_COMMENT_START);
            mode = BLOCK_COMMENT_END;
        }
        (
            { inLanguage(LANGUAGE_JAVA) }? '*'
            {
                $setType(JAVADOC_COMMENT_START);
                mode = JAVADOC_COMMENT_END;
            } ('/' { $setType(WHOLE_COMMENT); mode = 0; })? |
            { inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_C) || inLanguage(LANGUAGE_CSHARP) }? ('*' | '!')
            {
                $setType(DOXYGEN_COMMENT_START);
                mode = DOXYGEN_COMMENT_END;
            } ('/' { $setType(WHOLE_COMMENT); mode = 0; })?
        )? |

    // /= is an operator
    '=' )?

    {
        if (mode != 0) {

            changetotextlexer(mode);

            // when we return, we may have eaten the EOL, so we will turn back on startline
            startline = true;

            onpreprocline = false;
        }
    }
;

// whitespace (except for newline)
WS : (
    // single space
    ' '  |

    // horizontal tab
    '\t'
)+ ;

// end of line
EOL : '\n' { 

    // onpreprocline is turned on when on a preprocessor line
    // to prevent mostly string ending problems.
    // it has to be turned back on when the EOL is reached
    onpreprocline = false;

    // mark that we are starting a new line, so preproc
    // can be detected
    startline = true;

    // record to new lines for optional positions
    newline();
    if (isoption(options, SRCML_OPTION_LINE))
        setLine(getLine() + (1 << 16));
    if (isline && line_number > -1)
        setLine((int)(line_number << 16 | (getLine() & 0xFFFF)));
    isline = false;
    line_number = -1;
} ;

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
