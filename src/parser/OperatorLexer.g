// SPDX-License-Identifier: GPL-3.0-only
/*!
 * @file OperatorLexer.g
 *
 * @copyright Copyright (C) 2004-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML translator.
 */

header {
#ifndef _MSC_VER
#else
    #pragma warning(disable : 4456) // declaration of 'theRetToken' hides previous local declaration
    #pragma warning(disable : 4242) // 'argument': conversion from 'int' to 'char'
#endif

   #include <iostream>
}

options {
    language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class OperatorLexer extends TextLexer;

options {
    k = 1;
    testLiterals = false;
    importVocab = TextLexer;
}

tokens {
    EOL_BACKSLASH;

    TEMPOPS;    // "<";
    TEMPOPE;    // ">";
    EQUAL;      // "=";
    LPAREN;     // "(";
    DESTOP;     // "~";
    LCURLY;     // "{";
    RCURLY;     // "}";
    LBRACKET;   // "[";
    ATLBRACKET; // "@[";
    RBRACKET;   // "]";
    COMMA;      // ",";
    RPAREN;     // ")";
    TERMINATE;  // ";";
    PREPROC;
    COLON;      // ":";
    QMARK;

    // Java
    BAR;        // "|"

    // C++
    TRETURN;    // ->
    MPDEREF;
    DOTDEREF;

    // C#
    LAMBDA;

    // define value in master grammar so that it depends on language
    DCOLON;

    MEMBERPOINTER;  // ".*";
    PERIOD;         // ".";
    MULTOPS;        // "*";
    REFOPS;         // "&";
    RVALUEREF;      // "&&";

    DOTDOT;
    DOTDOTDOT;

    // Objective-C
    CSPEC;
    MSPEC;

    // Apple
    BLOCKOP;

    // literals
    LITERAL_FALSE;
    LITERAL_TRUE;

    // Other
    CUDA;

    ATSIGN;

    ALLOPERATORS;

    EOL_PLACEHOLD;
}

OPERATORS options { testLiterals = true; } {
    int start = LA(1);
    int xmlcount = 0;

    char prevchar = '\000';      // [JavaScript] records previous char before it is consumed
    char stringtoken = '\000';   // [JavaScript] denotes a string: '"', '\'', or '`'
    char commenttoken = '\000';  // [JavaScript] denotes a comment: 'l', 'b', 'h', or 'j'

    bool ignorenexttoken = false;
    bool isarrow = false;
    bool isselfclosing = false;
    bool wasescape = false;
    bool isjsx = false;
    bool endjsx = false;
    bool recordtag = false;

    std::string starttag = "<";
    std::string dummytag;

    // update the previous two non-whitespace characters
    this->updateNonWhitespaceCharacters();

    // determine if "<" starts a JSX literal in JavaScript
    if (inLanguage(LANGUAGE_JAVASCRIPT) && start == '<') {
        // if the prior token is a keyword that can contain many expressions afterward (e.g., "as", "return", etc.)
        // or the prior tokens are JS_YIELD and MULTOPS (e.g., "yield *"), then '<' starts a JSX literal
        if (
            srcMLParser::keyword_expression_pair_js_token_set.member(TokenLookbackJavaScript::lastTokenType())
            || (
                // special case: "yield *" syntax
                TokenLookbackJavaScript::lastTokenTypeTwo() == srcMLParser::JS_YIELD
                && TokenLookbackJavaScript::lastTokenType() == srcMLParser::MULTOPS
            )
        ) {
            isjsx = true;
        }
        // handle all other cases by looking ahead
        else {
            isjsx = this->isJSXLiteral();
        }
    }
} : (
    // # (C++/Python/JavaScript), #! (Python/JavaScript)
    '#' (
        { (inLanguage(LANGUAGE_PYTHON) || inLanguage(LANGUAGE_JAVASCRIPT)) && LA(1) == '!' }?
            { $setType(HASHBANG_COMMENT_START); changetotextlexer(HASHBANG_COMMENT_END); } |

        { (inLanguage(LANGUAGE_PYTHON) || inLanguage(LANGUAGE_JAVASCRIPT)) && LA(1) != '!' }?
            { $setType(HASHTAG_COMMENT_START); changetotextlexer(HASHTAG_COMMENT_END); } |

        // Names can include '#' (JavaScript)
        { inLanguage(LANGUAGE_JAVASCRIPT) && LA(1) != '!' }?
            NAME { $setType(NAME); } |

        { startline }?
            {
                $setType(PREPROC);

                // record that we are on a preprocessor line,
                // primarily so that unterminated strings in
                // a preprocessor line will end at the right spot
                onpreprocline = true;
                //firstpreprocline = true;
            }
    )? |

    '+' ('+' | '=')? |
    '-' ('-' | '=' | '>' ('*')? )? |

    // *, *=, ** (Python/JavaScript), **= (Python/JavaScript)
    '*' ({ inLanguage(LANGUAGE_PYTHON) || inLanguage(LANGUAGE_JAVASCRIPT) }? '*')? ('=')? |

    '%' ('=')? |
    '^' ('=')? |
    '|' ('|')? ('=')? |

    // !, !=, !== (JavaScript)
    '!' ('=' ({ inLanguage(LANGUAGE_JAVASCRIPT) }? '=')?)? |

    // :, := (Python), ::
    ':' ({ inLanguage(LANGUAGE_PYTHON) }? '=')? (':')? |

    // =, ==, => (C#/JavaScript), === (JavaScript)
    '=' ('=' ({ inLanguage(LANGUAGE_JAVASCRIPT) }? '=')? | { (inLanguage(LANGUAGE_CSHARP) && (lastpos != (getColumn() - 1) || prev == ')' || prev == '#')) || inLanguage(LANGUAGE_JAVASCRIPT) }? '>')? |

    // &, &&, &&=, &=
    '&' ('&')? ('=')? |

    // >, >>=, >=, >>> (JavaScript), >>>= (JavaScript), not >>
    '>' (('>' '=') => '>' '=' | { inLanguage(LANGUAGE_JAVASCRIPT) }? ('>' '>' '=') => '>' '>' '=')? ('=')? |

    // <, << (C/C++), <=, <<< (CUDA), <> (Python), <!-- (JavaScript), JSX (JavaScript)
    '<' (
        { inLanguage(LANGUAGE_JAVASCRIPT) && isjsx }?
        // add characters to starttag to create the starting tag
        (options { greedy = true; } :

            // ignore spaces and tabs
            (' ' | '\t') { wasescape = false; } |

            // ignore newlines
            ('\n') {
                wasescape = false;

                // no longer in a hashbang or line comment
                if (commenttoken == 'h' || commenttoken == 'l')
                    commenttoken = '\000';
            } |

            // ignore strings (including those in comments)
            { prevchar = (char)LA(1); } ('"' | '\'' | '`') {
                // found closing character for a string
                if (!wasescape && prevchar == stringtoken && commenttoken == '\000') {
                    stringtoken = '\000';
                }
                // found starting character for a string
                else if (!wasescape && stringtoken == '\000' && commenttoken == '\000') {
                    stringtoken = prevchar;
                }

                wasescape = false;
            } |

            // ignore JSX comments, but allow '<' to begin a JSX literal starting tag
            ('<') {
                // do not add to starttag if inside a string or a comment
                if (stringtoken != '\000' || commenttoken != '\000')
                    continue;

                wasescape = false;
            }
            (('!' '-' '-') => '!' '-' '-' { commenttoken = 'j'; })?
            {
                // only apply this logic if not inside a comment
                if (commenttoken == '\000') {
                    // only add '<' to starttag if there are no other '<'
                    if (std::count(starttag.begin(), starttag.end(), '<') == 0) {
                        starttag += '<';
                    }
                    else {
                        starttag += ' ';
                        ignorenexttoken = true;
                    }
                }
            } |

            // '>' will end a JSX literal starting tag if outside a string or comment
            ('>') {
                // do not add to starttag if inside a string or a comment
                if (stringtoken != '\000' || commenttoken != '\000')
                    continue;

                // ignore '>' from nested tag as to not mess up xmlcount
                if (ignorenexttoken) {
                    starttag += ' ';
                }
                // found '>'; only add it to starttag if there are no other '>'
                else {
                    if (std::count(starttag.begin(), starttag.end(), '>') == 0)
                        starttag += '>';
                    else
                        starttag += ' ';
                }

                // '>' indicates the end of the JSX tag
                if (!wasescape && !isarrow && !ignorenexttoken)
                    break;

                ignorenexttoken = false;
                wasescape = false;
            } |

            // "-->" will end a JSX comment
            ('-') { starttag += '-'; wasescape = false; } ({ stringtoken == '\000' && commenttoken == 'j' }? ('-' '>') => '-' '>' { commenttoken = '\000'; })? |

            // ignore hashbang comments (e.g., "#! ...")
            ('#') { starttag += '#'; wasescape = false; } ({ stringtoken == '\000' && commenttoken != 'h' }? '!' { commenttoken = 'h'; })? |

            // ignore line comments (e.g., "// ...") and block comments (e.g., "/* ... */")
            ('/') { starttag += '/'; wasescape = false; } ({ stringtoken == '\000' && commenttoken != 'l' }? '/' { commenttoken = 'l'; })? ({ stringtoken == '\000' && commenttoken != 'b' }? '*' { commenttoken = 'b'; })? |

            // "*/" will end a block comment
            ('*') { starttag += '*'; wasescape = false; } ({ stringtoken == '\000' && commenttoken == 'b' }? '/' { commenttoken = '\000'; })? |

            // ignore backslashes
            ('\\') { wasescape = true; } |

            // ignore arrows (e.g., "=>")
            ('=') { starttag += '='; wasescape = false; } ({ !isarrow }? '>' { isarrow = true; })? |

            { prevchar = (char)LA(1); } ~(' ' | '\t' | '\n' | '"' | '\'' | '`' | '<' | '>' | '-' | '#' | '/' | '*' | '\\' | '=') {
                // do not add to starttag if inside a string or a comment
                if (stringtoken != '\000' || commenttoken != '\000')
                    continue;

                // add the most recently consumed character to the starting tag (if it is not an arrow)
                if (!isarrow)
                    starttag += prevchar;

                isarrow = false;
                wasescape = false;
            }
        )*

        {
            prevchar = '\000';
            stringtoken = '\000';
            commenttoken = '\000';
            ignorenexttoken = false;
            isarrow = false;
            wasescape = false;

            // if the starting tag is a self-closing tag, ignore the processing step below
            if (starttag.size() > 1 && starttag.substr(starttag.size() - 2) == "/>")
                isselfclosing = true;
            else
                ++xmlcount;
        }

        (
            // consume tokens until reaching the closing tag (skip this step for a self-closing tag)
            { !isselfclosing }?
            (options { greedy = true; } :
                // failsafe to ensure ANTLR-generated version breaks properly
                { endjsx }? { break; } |

                // skip any escaped characters in the JSX literal
                ('\\') ~('\000') |

                // ignore whitespace and tabs
                (' ' | '\t') |

                // ignore newline characters (which end hashbang and line comments)
                ('\n') { if (commenttoken == 'h' || commenttoken == 'l') commenttoken = '\000'; } |

                // process string contents separately
                { stringtoken != '\000' }? ~('\\' | ' ' | '\t' | '\n' | '"' | '\'' | '`') |

                // '<' denotes the start of a JSX tag (unless found in a string or comment)
                { stringtoken == '\000' && commenttoken == '\000' }?
                (
                    // always consume '<', but ignore JSX comments
                    ('<') (('!' '-' '-') => '!' '-' '-' { commenttoken = 'j'; })?

                    {
                        // only apply this logic if not inside a comment
                        if (commenttoken == '\000') {
                            recordtag = true;

                            // only add '<' to dummytag if there are no other '<'
                            if (std::count(dummytag.begin(), dummytag.end(), '<') == 0) {
                                dummytag += '<';
                            }
                            else {
                                dummytag += ' ';
                                ignorenexttoken = true;
                            }
                        }
                    }
                ) |

                // '>' ends the current JSX tag (unless found in a string)
                { recordtag && stringtoken == '\000' }?
                ('>') {
                    // ignore '>' from nested tag as to not mess up xmlcount
                    if (ignorenexttoken) {
                        dummytag += ' ';
                    }
                    // found '>'; only add it to dummytag if there are no other '>'
                    else {
                        if (std::count(dummytag.begin(), dummytag.end(), '>') == 0)
                            dummytag += '>';
                        else
                            dummytag += ' ';
                    }

                    // check if dummytag is a starting tag or ending tag
                    if (!ignorenexttoken && dummytag.size() > 1) {
                        // found a starting tag
                        if (dummytag[1] != '/' && dummytag[1] != '!' && dummytag.substr(dummytag.size() - 2) != "/>")
                            ++xmlcount;

                        // special case: ending tag with no tag name
                        else if (dummytag == "</>")
                            --xmlcount;

                        // found an ending tag
                        else if (dummytag[1] == '/' && dummytag[1] != '!' && dummytag.substr(dummytag.size() - 2) != "/>")
                            --xmlcount;

                        // set a flag to end the JSX literal
                        if (xmlcount == 0)
                            endjsx = true;

                        dummytag.clear();
                        recordtag = false;
                    }

                    // reset ignore next '>' detection
                    ignorenexttoken = false;

                    // failsafe to ensure ANTLR-generated version breaks properly
                    if (endjsx)
                        break;
                } |

                // start a string (not in a comment)
                { stringtoken == '\000' && commenttoken == '\000' }? { prevchar = (char)LA(1); } ('"' | '\'' | '`') { stringtoken = prevchar; } |

                // end a string
                { LA(1) == stringtoken }? ('"' | '\'' | '`') { stringtoken = '\000'; } |

                // start a comment
                { commenttoken == '\000' }?
                (
                    // ignore hashbang comments (e.g., "#! ...")
                    ('#') { if (recordtag) dummytag += '#'; } ({ commenttoken != 'h' }? '!' { commenttoken = 'h'; })? |

                    // ignore line comments (e.g., "// ...") and block comments (e.g., "/* ... */")
                    ('/') { if (recordtag) dummytag += '/'; } ({ commenttoken != 'l' }? '/' { commenttoken = 'l'; })? ({ commenttoken != 'b' }? '*' { commenttoken = 'b'; })?
                ) |

                // end a comment
                { commenttoken == 'b' || commenttoken == 'j' }?
                (
                    // "-->" will end a JSX comment
                    ('-') { if (recordtag) dummytag += '-'; } ({ commenttoken == 'j' }? ('-' '>') => '-' '>' { commenttoken = '\000'; })? |

                    // "*/" will end a block comment
                    ('*') { if (recordtag) dummytag += '*'; } ({ commenttoken == 'b' }? '/' { commenttoken = '\000'; })?
                ) |

                // ignore arrows (e.g., "=>")
                { recordtag }? ('=') { dummytag += '='; } ({ recordtag }? '>')? |

                // process comment contents separately
                { commenttoken != '\000' }? ~('\\' | ' ' | '\t' | '\n' | '-' | '*') |

                // do not record characters that are outside of a tag
                { !recordtag }? ~('\\' | '<' | ' ' | '\t' | '\n') |

                // record the main content in a JSX literal
                { recordtag }? { prevchar = (char)LA(1); } ~('\\' | '<' | '>' | ' ' | '\t' | '\n') { dummytag += prevchar; }
            )*
        )?
        { $setType(JS_JSX_LITERAL); } |

        { inLanguage(LANGUAGE_JAVASCRIPT) }? ('!' '-' '-') => '!' '-' '-' { $setType(HTML_COMMENT_START); changetotextlexer(HTML_COMMENT_END); } |

        { inLanguage(LANGUAGE_PYTHON) }? '>' |

        '<' ({ inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_C) }? '<' | '=')? ({ inLanguage(LANGUAGE_JAVASCRIPT) }? '<')* |

        '='
    )? |

    // match these as individual operators only
    ',' | ';' | '('..')' | '[' | ']' | '{' | '}' | 

    // names can start with a @ in C#
    '@' (
        { inLanguage(LANGUAGE_JAVASCRIPT) }?
          '@' { $setType(TS_DATSIGN); }
        |
        { inLanguage(LANGUAGE_PYTHON) }?
          '='
        |
        { inLanguage(LANGUAGE_OBJECTIVE_C) }?
          '(' 
        |
        { inLanguage(LANGUAGE_OBJECTIVE_C) }?
          '['
        |
        { inLanguage(LANGUAGE_OBJECTIVE_C) }?
          '{'
        |
        { inLanguage(LANGUAGE_CSHARP) || inLanguage(LANGUAGE_OBJECTIVE_C) }?
            NAME { $setType(NAME); }
        |
        { inLanguage(LANGUAGE_OBJECTIVE_C) }?
            CONSTANTS { $setType(CONSTANTS); }
        |
        { inLanguage(LANGUAGE_CSHARP) || inLanguage(LANGUAGE_OBJECTIVE_C) }? {
            $setType(CHAR_START);
        }
        CHAR_START |

        { inLanguage(LANGUAGE_CSHARP) || inLanguage(LANGUAGE_OBJECTIVE_C) }? {
            atstring = true; 
            $setType(STRING_START);
        }
        STRING_START )? |

    // ?, ??, etc. (part of ternary); ?. (JavaScript), ??= (JavaScript)
    '?' ('?')* ({ inLanguage(LANGUAGE_JAVASCRIPT) }? '.')? ({ inLanguage(LANGUAGE_JAVASCRIPT) }? '=')? |

    '~' | // has to be separate if part of name

    '.' ({ inLanguage(LANGUAGE_C_FAMILY) }? '*' | '.' ('.')? | { $setType(CONSTANTS); } CONSTANTS )? |
    

    '\\' ({ inLanguage(LANGUAGE_PYTHON) }? EOL { $setType(EOL_BACKSLASH); } | (EOL { $setType(EOL_BACKSLASH); })*)
    )
    { startline = false; lastpos = getColumn(); prev = start; }
;
