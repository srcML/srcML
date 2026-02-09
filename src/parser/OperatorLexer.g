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

    char prevtoken = '\000';
    char stringtoken = '\000';

    bool ignorenexttoken = false;
    bool isarrow = false;
    bool islinecomment = false;
    bool isblockcomment = false;
    bool ishashbangcomment = false;
    bool isjsxcomment = false;
    bool isselfclosing = false;
    bool wasescape = false;
    bool isxml = false;
    bool recordtag = false;

    std::string starttag = "<";
    std::string dummytag;

    // update the previous two non-whitespace characters
    this->updateNonWhitespaceCharacters();

    // if the previous token is a keyword that can contain one or more expressions
    // afterward (e.g., "as", "return", etc.), then '<' starts a JSX literal
    if (
        inLanguage(LANGUAGE_JAVASCRIPT)
        && start == '<'
        && srcMLParser::keyword_expression_pair_js_token_set.member(TokenLookbackJavaScript::lastTokenType())
    ) {
        isxml = true;
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
        {
            inLanguage(LANGUAGE_JAVASCRIPT)
            && LA(1) != '!'                                                  // do not mark JSX comments (e.g., "<!--") as JSX literals
            && (
                isxml                                                        // case: keyword + '<'
                || lookaheadMinusTwo == '*'                                  // case: "yield *" syntax
                || lookaheadMinusTwo == '('                                  // case: parenthesized JSX tags
                || lookaheadMinusTwo == '#'                                  // case: the prior code was a hashbang comment
                || lookaheadMinusTwo == '<'                                  // case: the prior code was a JSX comment
                || lookaheadMinusTwo == '='                                  // case: initialization
                || lookaheadMinusTwo == ','                                  // case: JSX tags in a list (e.g., argument list)
                || lookaheadMinusTwo == '['                                  // case: first expression in an array
                || lookaheadMinusTwo == '{'                                  // case: first expression in an object
                || lookaheadMinusTwo == '?'                                  // case: first expression in a ternary
                || lookaheadMinusTwo == ':'                                  // case: additional expressions in properties and ternaries
                || (lookaheadMinusThree == '=' && lookaheadMinusTwo == '>')  // case: first expression in a lambda
            )
        }?
        // add characters to starttag to create the starting tag
        (options { greedy = true; } :

            // ignore spaces and tabs
            (' ' | '\t') { if (wasescape) wasescape = false; } |

            // ignore newlines
            ('\n') {
                if (wasescape)
                    wasescape = false;

                if (ishashbangcomment)
                    ishashbangcomment = false;

                if (islinecomment)
                    islinecomment = false;
            } |

            // ignore strings
            { prevtoken = (char)LA(1); } ('"' | '\'' | '`') {
                // found closing character for a string
                if (!wasescape && prevtoken == stringtoken) {
                    stringtoken = '\000';
                }
                // found starting character for a string
                else if (!wasescape) {
                    stringtoken = prevtoken;
                }

                if (wasescape)
                    wasescape = false;
            } |

            // ignore JSX comments
            ('<') { starttag += '<'; } (('!' '-' '-') => '!' '-' '-' { isjsxcomment = true; })? |

            // "-->" will end a JSX comment
            ('-') { starttag += '-'; } ({ isjsxcomment }? ('-' '>') => '-' '>' { isjsxcomment = false; })? |

            // ignore hashbang comments (e.g., "#! ...")
            ('#') { starttag += '#'; } ({ !ishashbangcomment }? '!' { ishashbangcomment = true; })? |

            // ignore line comments (e.g., "// ...") and block comments (e.g., "/* ... */")
            ('/') { starttag += '/'; } ({ !islinecomment }? '/' { islinecomment = true; })? ({ !isblockcomment }? '*' { isblockcomment = true; })? |

            // "*/" will end a block comment
            ('*') { starttag += '*'; } ({ isblockcomment }? '/' { isblockcomment = false; })? |

            // ignore backslashes
            ('\\') { wasescape = true; } |

            // ignore equal signs
            ('=') { isarrow = true; if (wasescape) wasescape = false; } |

            { prevtoken = (char)LA(1); } ~(' ' | '\t' | '\n' | '"' | '\'' | '`' | '<' | '-' | '#' | '/' | '*' | '\\' | '=') {
                // do not end the JSX literal at an arrow (e.g., "=>")
                if (isarrow && prevtoken != '>')
                    isarrow = false;

                // do not add to starttag if inside a string or a comment
                if (stringtoken != '\000' || islinecomment || isblockcomment || ishashbangcomment || isjsxcomment)
                    continue;

                // add the most recently consumed character to the starting tag (if it is not an arrow)
                if (!isarrow)
                    starttag += prevtoken;

                // '>' indicates the end of the JSX tag
                if (!wasescape && !isarrow && prevtoken == '>')
                    break;

                if (isarrow)
                    isarrow = false;

                if (wasescape)
                    wasescape = false;
            }
        )*

        {
            prevtoken = '\000';
            stringtoken = '\000';
            islinecomment = false;
            isblockcomment = false;
            ishashbangcomment = false;
            isjsxcomment = false;
            wasescape = false;

            // if the starting tag is a self-closing tag, ignore the processing step below
            if (starttag.size() > 1 && starttag.substr(starttag.size() - 2) == "/>")
                isselfclosing = true;
            else
                ++xmlcount;
        }

        (
            // consume tokens until reaching the closing tag (skip this for a self-closing tag)
            { !isselfclosing }?
            (options { greedy = true; } :

                ('\000') |

                { prevtoken = (char)LA(1); } ~('\000') {
                    // found closing character for a string
                    if (!wasescape && prevtoken == stringtoken) {
                        stringtoken = '\000';
                    }
                    // found starting character for a string
                    else if (!wasescape && (prevtoken == '"' || prevtoken == '\'' || prevtoken == '`')) {
                        stringtoken = prevtoken;
                    }

                    // reset escaped character detection
                    if (wasescape)
                        wasescape = false;

                    // record if the next character will be escaped
                    if (prevtoken == '\\')
                        wasescape = true;

                    // do not add to dummytag if inside a string
                    if (stringtoken != '\000')
                        continue;

                    // '<' denotes the start of a JSX tag
                    if (prevtoken == '<')
                        recordtag = true;

                    // in a JSX tag, but not all content should be recorded
                    if (recordtag) {
                        // ignore '>' from nested tag as to not mess up xmlcount
                        if (ignorenexttoken && prevtoken == '>') {
                            dummytag += ' ';
                        }
                        // found '<'; only add it to dummytag if there are no other '<'
                        else if (prevtoken == '<') {
                            if (std::count(dummytag.begin(), dummytag.end(), '<') == 0) {
                                dummytag += '<';
                            }
                            else {
                                dummytag += ' ';
                                ignorenexttoken = true;
                            }
                        }
                        // found '>'; only add it to dummytag if there are no other '>'
                        else if (prevtoken == '>') {
                            if (std::count(dummytag.begin(), dummytag.end(), '>') == 0)
                                dummytag += '>';
                            else
                                dummytag += ' ';
                        }
                        else {
                            dummytag += prevtoken;
                        }
                    }

                    // check if dummytag is a starting tag or ending tag
                    if (!ignorenexttoken && dummytag.size() > 1 && prevtoken == '>') {
                        // remove all spaces, tabs, and newline characters from the starting tag
                        if (!dummytag.empty()) {
                            dummytag.erase(std::remove(dummytag.begin(), dummytag.end(), ' '), dummytag.end());
                            dummytag.erase(std::remove(dummytag.begin(), dummytag.end(), '\t'), dummytag.end());
                            dummytag.erase(std::remove(dummytag.begin(), dummytag.end(), '\n'), dummytag.end());
                        }

                        // found a starting tag
                        if (dummytag[1] != '/' && dummytag[1] != '!' && dummytag.substr(dummytag.size() - 2) != "/>")
                            ++xmlcount;

                        // special case: ending tag with no tag name
                        else if (dummytag == "</>")
                            --xmlcount;

                        // found an ending tag
                        else if (dummytag[1] == '/' && dummytag[1] != '!' && dummytag.substr(dummytag.size() - 2) != "/>")
                            --xmlcount;

                        // exit
                        if (xmlcount == 0)
                            break;

                        dummytag.clear();
                        recordtag = false;
                    }

                    // reset ignore next '>' detection
                    if (ignorenexttoken && prevtoken == '>')
                        ignorenexttoken = false;
                }
            )*
        )?
        { $setType(JS_JSX_LITERAL); } |

        { inLanguage(LANGUAGE_JAVASCRIPT) }? ('!' '-' '-') { $setType(JSX_COMMENT_START); changetotextlexer(JSX_COMMENT_END); } |

        { inLanguage(LANGUAGE_PYTHON) }? '>' |

        '<' ({ inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_C) }? '<' | '=')? |

        '='
    )? |

    // match these as individual operators only
    ',' | ';' | '('..')' | '[' | ']' | '{' | '}' | 

    // names can start with a @ in C#
    '@' (

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
