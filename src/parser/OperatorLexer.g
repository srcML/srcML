/*!
 * @file OperatorLexer.g
 *
 * @copyright Copyright (C) 2004-2014  srcML, LLC. (www.srcML.org)
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
 */

header {
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
    importVocab=TextLexer;
}

tokens {
EOL_BACKSLASH;

TEMPOPS;
TEMPOPE;
EQUAL;
LPAREN; // = "(";
DESTOP; // = "~";
LCURLY; // = "{";
RCURLY; // = "}";
LBRACKET; // = "[";
ATLBRACKET; // = "@[";
RBRACKET; // = "]";
COMMA; // = ",";
RPAREN; // = ")";
TERMINATE; // = ";";
PREPROC;
COLON; // = ":";
QMARK;

ASSIGNMENT; // +=, -=, etc.

// Java
BAR; // |

// C++
TRETURN; // ->
MPDEREF;
DOTDEREF;

// C#
LAMBDA;

// define value in master grammar so that it depends on language
DCOLON;

MEMBERPOINTER; // = ".*";
PERIOD; // = ".";
MULTOPS; // = "*";
REFOPS;  // = "&";
RVALUEREF; // = "&&";

DOTDOT;
DOTDOTDOT;

// Objective-C
CSPEC;
MSPEC;

// Apple
BLOCKOP;

// literals
FALSE;
TRUE;

// Other
CUDA;

ATSIGN;

ALLOPERATORS;

EOL_PLACEHOLD;
}

// @todo remove statics possibly breaking point for threading.
OPERATORS options { testLiterals = true; } { bool star = false; int start = LA(1); bool do_not_apply = false;
} : 
        (
            '#' {

            if (startline) {

                $setType(PREPROC); 

                // record that we are on a preprocessor line,
                // primarily so that unterminated strings in
                // a preprocessor line will end at the right spot
                onpreprocline = true; 

                if(isoption(options, SRCML_OPTION_LINE)) {
                    int start = mark();
                    ++inputState->guessing;
                    if(LA(1) == 'l') {
                        consume();  
                        if(LA(1) == 'i') {
                            consume();
                            if(LA(1) ==  'n') {
                                consume();
                                if(LA(1) ==  'e')
                                    isline = true;
                            }
                        }
                    }
                    --inputState->guessing;
                    rewind(start);
                }

            }
        }   |
/*
        ({ !stop && !(gt && (LA(1) == '>' || LA(1) == ':' || LA(1) == '&' || LA(1) == '*')) && (dcoloncount < 2) }?

         ( '*' { gt = true; } | '|' | ':' { ++dcoloncount; } | '`' | '=' { if (LA(1) != '=') stop = true; } | '!' | '%' | '+' | '^' | '-' |
           '&' { text.erase(realbegin); text += "&amp;"; realbegin += 4; gt = true; } | 
           '>' { if (realbegin == _begin) gt = true; text.erase(realbegin); text += "&gt;"; realbegin += 3; } | 
           '<' { text.erase(realbegin); text += "&lt;"; realbegin += 3; gt = true; }) { ++realbegin; } )+ */ 

       '+' { if(inLanguage(LANGUAGE_OBJECTIVE_C) && LA(1) != '+' && LA(1) != '=') $setType(CSPEC); } ('+' | '=' { $setType(ASSIGNMENT); } )? |
       '-' { if(inLanguage(LANGUAGE_OBJECTIVE_C) && LA(1) != '-' && LA(1) != '=') $setType(MSPEC); } 
           ('-' | '=' { $setType(ASSIGNMENT); } | '>' { star = true; $setText("-&gt;"); $setType(TRETURN);})? ({ star }? '*' { $setText("-&gt;*"); $setType(MPDEREF); })? |
       '*' ('=' { $setType(ASSIGNMENT); } )? |
//       '/' ('=')? |
       '%' ('=' { $setType(ASSIGNMENT); } )? |
       '^' { if(LA(1) != '=') $setType(BLOCKOP); } ('=' { $setType(ASSIGNMENT); } )? |
       '|' ('|')? ('=' { $setType(ASSIGNMENT); } )? |
       '`' |
       '!' ('=')? |
       ':' (':')? |
       '=' ('=' | { inLanguage(LANGUAGE_CSHARP) && (lastpos != (getColumn() - 1) || prev == ')' || prev == '#') }? '>' { $setText("=&gt;"); $setType(LAMBDA); } |) |

       '&' { $setText("&amp;"); }
            (options { greedy = true; } : '&' { $setText("&amp;&amp;"); star = true; } | '=' { $setText("&amp;="); $setType(ASSIGNMENT); } )?
             ({ star }? '=' { $setText("&amp;&amp;="); } )? | 
     
       '>' { $setText("&gt;"); } (('>' '=') => '>' '=' { $setText("&gt;&gt;="); $setType(ASSIGNMENT); do_not_apply = true; })? ({ !do_not_apply }? '=' { $setText("&gt;="); })?  { do_not_apply = false; } |

       '<' { $setText("&lt;"); }
            (options { greedy = true; } : '<' { $setText("&lt;&lt;"); } (options { greedy = true; } : { inLanguage(LANGUAGE_CXX) | inLanguage(LANGUAGE_C) }? '<' { $setText("&lt;&lt;&lt;"); $setType(CUDA); })? | '=' { $setText("&lt;="); })?
            ('=' { $setText("&lt;&lt;="); $setType(ASSIGNMENT); })? |

//       '<' { text.erase(realbegin); text += "&lt;"; realbegin += 3; gt = true; realbegin += 3; } 
//            ('<' { text.erase(realbegin); text += "&lt;"; realbegin += 4; gt = true; })? ('=')? |

        // match these as individual operators only
        ',' | ';' | '('..')' | '[' | ']' | '{' | '}' | 

            // names can start with a @ in C#
            '@' { $setType(ATSIGN); }
            ( 
            { inLanguage(LANGUAGE_OBJECTIVE_C) }?
              '(' { $setType(LPAREN); }
            |
            { inLanguage(LANGUAGE_OBJECTIVE_C) }?
              '[' { $setType(ATLBRACKET); }
            |
            { inLanguage(LANGUAGE_OBJECTIVE_C) }?
              '{' { $setType(LCURLY); }
            |
            { inLanguage(LANGUAGE_CSHARP) || inLanguage(LANGUAGE_OBJECTIVE_C) }? NAME
            { $setType(NAME); }
            |
            { inLanguage(LANGUAGE_OBJECTIVE_C) }? CONSTANTS
            { $setType(CONSTANTS); }
            |
            { inLanguage(LANGUAGE_CSHARP) || inLanguage(LANGUAGE_OBJECTIVE_C) }? 
              { if(LA(1) == '"') {
                atstring = true; 
                $setType(STRING_START);
                } else {
                  $setType(CHAR_START);
                }
              }
              STRING_START
            |
            )
        |

        '?' { $setType(QMARK); } ('?' { $setType(OPERATORS); })* | // part of ternary
        '~'  | // has to be separate if part of name

        '.' ({ !inLanguage(LANGUAGE_JAVA) }? '*' { $setType(DOTDEREF); } | '.' ( '.' { $setType(DOTDOTDOT); } | { $setType(DOTDOT); }) | { $setType(CONSTANTS); } CONSTANTS | ) |

        '\\' ( EOL { $setType(EOL_BACKSLASH); } )*
        )
        { startline = false; lastpos = getColumn(); prev = start; }
;
