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
    int numstarttags = 1;

    char prevtoken;

    bool iscomment = true;
    bool isempty = false;
    bool isselfclosing = false;
    bool isxml = false;
    bool recordtag = false;

    std::string keyword;
    std::string starttag;
    std::string endtag = "</";
    std::string dummytag;

    // update the previous two non-whitespace characters
    this->updateNonWhitespaceCharacters();

    if (inLanguage(LANGUAGE_JAVASCRIPT) && start == '<') {
        keyword = keywordLookback;

        // convert newlines and tabs into spaces
        std::replace(keyword.begin(), keyword.end(), '\n', ' ');
        std::replace(keyword.begin(), keyword.end(), '\t', ' ');

        // if any of these keywords are found, then '<' starts an XML literal
        if (
            keyword.find(" as ") != std::string::npos || keyword.find(" case ") != std::string::npos
            || keyword.find(" default ") != std::string::npos || keyword.find(" export ") != std::string::npos
            || keyword.find(" in ") != std::string::npos || keyword.find(" of ") != std::string::npos
            || keyword.find(" return ") != std::string::npos || keyword.find(" throw ") != std::string::npos
            || keyword.find(" yield ") != std::string::npos || keyword.find(" yield* ") != std::string::npos
        ) {
            isxml = true;
        }
    }

    this->recordCurrentCharacter();
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

    // <, << (C/C++), <=, <<< (CUDA), <> (Python), <!-- (JavaScript), XML (JavaScript)
    '<' (
        {
            inLanguage(LANGUAGE_JAVASCRIPT)
            && LA(1) != '!'                   // do not mark XML comments as XML literals
            && (
                isxml                         // common case: keyword + '<'
                || lookaheadMinusTwo == '('   // common case: parenthesized XML tags
                || lookaheadMinusTwo == '#'   // edge case: the prior code was a hashbang comment
                || lookaheadMinusTwo == '<'   // edge case: the prior code was an XML comment
            )
        }?
        // record the first character in the starting tag name so it is not lost
        { starttag += (char)LA(1); }

        (options { greedy = true; } :
            ~('>') { starttag += (char)LA(1); }
        )*

        {
            // remove any attributes and the closing '>' from the starting tag
            // do not do this if the tag is a self-closing tag (e.g., "<unit/>")
            if (starttag.size() > 1 && starttag.substr(starttag.size() - 2) != "/>") {
                int firstspace = std::numeric_limits<int>::max();
                int firsttab = std::numeric_limits<int>::max();
                int firstnewline = std::numeric_limits<int>::max();

                if (starttag.find(' ') != std::string::npos)
                    firstspace = starttag.find(' ');
                if (starttag.find('\t') != std::string::npos)
                    firsttab = starttag.find('\t');
                if (starttag.find('\n') != std::string::npos)
                    firstnewline = starttag.find('\n');

                int firstwhitespace = std::min({firstspace, firsttab, firstnewline});

                std::string temptag = starttag.substr(0, firstwhitespace);
                starttag = temptag;
            }

            // create the end tag using the starting tag name
            if (!starttag.empty())
                endtag += starttag;

            // complete the starting tag by adding a '<' (if applicable)
            if (starttag.front() != '<')
                starttag.insert(0, 1, '<');

            // remove extraneous whitespace tokens from the final result (possible if tag had attributes)
            while (endtag.back() == '\n' || endtag.back() == '\t' || endtag.back() == ' ')
                endtag.pop_back();

            // if the closing tag is "</>", then the starting tag was "<>" (i.e., empty)
            if (endtag == "</>") {
                isempty = true;
            }
            // add closing '>' if it is not there (possible if tag had attributes)
            else if (endtag.back() != '>') {
                endtag += '>';
            }

            // found a self-closing tag (e.g., "<unit/>")
            if (!isempty && endtag.substr(endtag.size() - 2) == "/>") {
                isselfclosing = true;
            }
            // found a regular starting tag (e.g., "<unit>")
            else {
                // discard any characters after (and including) the first space
                size_t space_loc = endtag.find(' ');
                if (space_loc != std::string::npos)
                    endtag.erase(space_loc);

                // add closing '>' back if it is not there
                if (endtag.back() != '>') {
                    endtag += '>';
                }
            }
        }

        // consume tokens until reaching the closing tag
        (options { greedy = true; } :

            ('\n') { recordtag = false; dummytag.clear(); } |
            { prevtoken = (char)LA(1); } ~('\n') {
                if (prevtoken == '<')
                    recordtag = true;

                if (recordtag)
                    dummytag += prevtoken;

                // found a nested tag with the same name as the starting tag
                if (dummytag == starttag)
                    ++numstarttags;

                // found a closing tag for the starting tag; only break if top-level
                if (dummytag == endtag) {
                    if (numstarttags == 1) {
                        break;
                    }
                    else {
                        dummytag.clear();
                        --numstarttags;
                    }
                }

                if (prevtoken == '>') {
                    // found the end of a self-closing tag (since '/' was already consumed)
                    if (isselfclosing)
                        break;

                    recordtag = false;
                }
            }
        )*

        { $setType(JS_XML_LITERAL); } |

        { inLanguage(LANGUAGE_JAVASCRIPT) }? ('!' '-' '-') { $setType(XML_COMMENT_START); changetotextlexer(XML_COMMENT_END); } |

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
