// SPDX-License-Identifier: GPL-3.0-only
/*!
 * @file OperatorLexer.g
 *
 * @copyright Copyright (C) 2004-2019 srcML, LLC. (www.srcML.org)
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
} : (
    '#'
    {
        if (startline) {

            $setType(PREPROC); 

            // record that we are on a preprocessor line,
            // primarily so that unterminated strings in
            // a preprocessor line will end at the right spot
            onpreprocline = true; 
            //firstpreprocline = true;
        }
    } |

    '+' ('+' | '=')? |
    '-' ('-' | '=' | '>' ('*')? )?  |
    '*' ('=')? |
    '%' ('=')? |
    '^' ('=')? |
    '|' ('|')? ('=')? |
    '`' |
    '!' ('=')? |
    ':' (':')? |

    '=' ('=' | { inLanguage(LANGUAGE_CSHARP) && (lastpos != (getColumn() - 1) || prev == ')' || prev == '#') }? '>')? |

    // &, &&, &&=, &=
    '&' ('&')? ('=')? |

    // >, >>=, >=, not >>
    '>' (('>' '=') => '>' '=')? ('=')? |

    // <, << (C/C++), <=, <<< (CUDA)
    '<' ('=' | '<' ({ inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_C) }? '<' | '=')? )? |

    // match these as individual operators only
    ',' | ';' | '('..')' | '[' | ']' | '{' | '}' | 

    // names can start with a @ in C#
    '@' (

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

    '?' ('?')* | // part of ternary
    '~'  | // has to be separate if part of name

    '.' ({ inLanguage(LANGUAGE_C_FAMILY) }? '*' | '.' ('.')? | { $setType(CONSTANTS); } CONSTANTS )? |

    '\\' ( EOL { $setType(EOL_BACKSLASH); } )*
    )
    { startline = false; lastpos = getColumn(); prev = start; }
;
