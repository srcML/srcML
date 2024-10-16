// SPDX-License-Identifier: GPL-3.0-only
/*!
 * @file TextLexer.g
 *
 * @copyright Copyright (C) 2002-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML translator.
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
#ifndef _MSC_VER
#else
    #pragma warning(disable : 4242) // 'argument': conversion from 'int' to 'char'
#endif
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
    importVocab = CommentTextLexer;
}

tokens {
    BLOCK_COMMENT_START;
    JAVADOC_COMMENT_START;
    DOXYGEN_COMMENT_START;
    LINE_DOXYGEN_COMMENT_START;
    CHAR_START;
    BACKTICK_START;
    MACRO_NAME;
    COMPLEX_NUMBER;
    HASHBANG_COMMENT_START;
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
    (options { greedy = true; } : { delimiter += static_cast<char>(LA(1)); } ~('(' | ')' | '\\' | '\n' | ' ' | '\t' ))*
;

CHAR_START :
    { startline = false; }

    // character literal or single quoted string
    '\'' { $setType(CHAR_START); changetotextlexer(CHAR_END); }
;

BACKTICK_START :
    { startline = false; }

    // backtick is a string literal in JavaScript and an operator otherwise
    '`' {
        if (inLanguage(LANGUAGE_JAVASCRIPT)) {
            $setType(BACKTICK_START); changetotextlexer(BACKTICK_END);
        }
        else {
            $setType(OPERATORS);
        }
    }
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
            line_number = atoi(text.substr(_begin, text.length()-_begin).data());
        }
    }
;

NAME options { testLiterals = true; } :
    { startline = false; }
    ('a'..'z' | 'A'..'Z' | '_' | '\200'..'\377' | '$')
    ((options { greedy = true; } : '0'..'9' | 'a'..'z' | 'A'..'Z' | '_' | '\200'..'\377' | '$')*)
    (
        { text == "L"sv || text == "U"sv || text == "u"sv || text == "u8"sv }?
        { $setType(STRING_START); } STRING_START |

        { inLanguage(LANGUAGE_CXX) && (text == "R"sv || text == "u8R"sv || text == "LR"sv || text == "UR"sv || text == "uR"sv) }?
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

// Hashbang comments (JavaScript)
HASHBANG_COMMENT_START :
    {
        if (!inLanguage(LANGUAGE_JAVASCRIPT) && startline) {
            $setType(PREPROC);

            // record that we are on a preprocessor line,
            // primarily so that unterminated strings in
            // a preprocessor line will end at the right spot
            onpreprocline = true;
            //firstpreprocline = true;
        }
    }

    '#' (
        { inLanguage(LANGUAGE_JAVASCRIPT) && LA(1) != '!' }?
            NAME { $setType(NAME); }
    )?

    ('!'
        {
            if (inLanguage(LANGUAGE_JAVASCRIPT)) {
                $setType(HASHBANG_COMMENT_START); changetotextlexer(HASHBANG_COMMENT_END);
            }
        }
    )?
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
