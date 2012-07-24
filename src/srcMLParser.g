/*
  srcMLParser.g

  Copyright (C) 2004-2012  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Comments:

  This is an ANTLR grammar file for the main part of the srcML translator.
  It is a mixture of ANTLR code with C++ code mixed in for the actions.

  The grammar is for the C++ language.  It is unlike typical C++ parsers for many
  reasons:
  
  - ANTLR uses this code to generate a recursive-descent LL(k) parser.  This
  parser starts at the leftmost token and tries to match the tokens to C++
  source code.

  - Additional classes are used to implement an event-driven parser.  Input
  to the parser is a stream of tokens from the lexer.  Output from this parser
  is a new stream of tokens.  The parser user calls nextToken repeatedly to
  process all of the tokens.

  - The parser is designed to be used interactively.  When the nextToken is
  called a minimal number of input tokens are read to generate an output token.
  This makes the parser very responsive and able to issue start statement
  tokens before the end of the statement is reached.

  - The parser insert additional tokens into the input stream corresponding to
  start and end tags.

  Matching:

  - The parser does not attempt to detect invalid C++ code.  It is designed to
  match well-formed C++ code.  It assumes that the input C++ code is valid.

  - Whitespace and comments are handled in StreamMLParser.  They are diverted
  from the input token stream and inserted into the output token stream.  There
  is some processing to match these skipped tokens with the generated tokens
  from the parser.

  - There is no symbol table.  I repeat:  There is no symbol table.  No
  grammar rules are based on the type of an identifier.

  - Keywords are used to identify statements.  They are not used for types.
  Type keywords are in tokens just like other identifiers.

  Implementation:

  - The state of the current parsing is stored in modes.  The modes use flags
  to remember what state the parsing was in during the previous parse.

  - Element start tokens are generated using the method startElement.  The
  starting elements are stored in a stack in the current mode.

  - Element end tokens are generated automatically when a mode ends.  The stack
  of start tokens is popped of and ended automatically.

  - Do not end an element explicitly.  End the mode instead.

  Helpers:

  - The class StreamParser provides stream processing.  The class StreamMLParser
  provides markup language stream processing.  These are template classes which
  use this parser as a template parameter base.

  - The class TokenParser provides the virtual table for methods in StreamParser
  that are called in this parser.

  - Obviously this needs to be untangled but is not as easy as it should be.

  - Additional methods for the parser are declared in class Mode.  These methods
  only provide general support for the parser.  They do not, repeat, do not, contain
  token specific processing.

  Terminology:

  The use of C++ terminology is sometimes contradictory.  This is especially true
  for declarations and definitions, since a definition can also serve as a
  declaration.  The following rules are used:

  declaration - stating that something exists:

      function declaration:  int f();
      class declaration:     class A;
      struct declaration:    struct A;
      union declaration:     union A;
      method declaration:    virtual int f(); // in class

  definition - defining the layout or interface

      function definition:  int f() {}
      class definition:     class A { int a; }
      struct definition:    struct A { int a; }
      union definition:     union A { int a; }
      method definition:    int A::f() {} // in or out of class

  C vs. C++

  Additional keywords in C++ may be identifiers in C.  This is handled in the
  lexer which has symbols for all C++ (and C) keywords, but only will find them in
  the input if in C++ mode.  They are matched as NAME in C mode.
*/

header {
}

// Included in the generated srcMLParser.hpp file after antlr includes
header "post_include_hpp" {

#include <iostream>
#include <iomanip>
#include <stack>
#include "Mode.hpp"
#include "Options.hpp"

// Macros to introduce trace statements
#define ENTRY_DEBUG //RuleDepth rd(this); fprintf(stderr, "TRACE: %d %d  %5s%*s %s (%d)\n", inputState->guessing, LA(1), (LA(1) != 11 ? LT(1)->getText().c_str() : "\\n"), ruledepth, "", __FUNCTION__, __LINE__);
#define CATCH_DEBUG //marker();

#define assertMode(m)

enum DECLTYPE { NONE, VARIABLE, FUNCTION, CONSTRUCTOR, DESTRUCTOR, SINGLE_MACRO, NULLOPERATOR };
enum CALLTYPE { NOCALL, CALL, MACRO };

// position in output stream
struct TokenPosition {

    TokenPosition()
        : token(0), sp(0) {}

    TokenPosition(antlr::RefToken* p_token, int* p_sp)
        : token(p_token), sp(p_sp) {}

    // sets a particular token in the output token stream
    void setType(int type) {

        // set the inner name token to type
        (*token)->setType(type);

        // set this position in the element stack to type
        *sp = type;
    }

    ~TokenPosition() {
    }

    antlr::RefToken* token;
    int* sp;
};

}

// Included in the generated srcMLParser.cpp file after antlr includes
header "post_include_cpp" {

    class LocalMode {

     public:
     LocalMode(srcMLParser* t)
        : pparser(t), oldsize(t->size())
     {}

     ~LocalMode() {
         srcMLParser& fp = *pparser;
         int n = fp.size() - oldsize;
         for (int i = 0; i < n; ++i) {
           fp.endCurrentMode();
         }
     }

     private:
       srcMLParser* pparser;
       const int oldsize;
    };

    class RuleDepth {

     public:
     RuleDepth(srcMLParser* t) : pparser(t) { ++pparser->ruledepth; }
     ~RuleDepth() { --pparser->ruledepth; }

     private:
       srcMLParser* pparser;
    };

srcMLParser::srcMLParser(antlr::TokenStream& lexer, int lang, int parser_options)
   : antlr::LLkParser(lexer,1), Mode(this, lang), zeromode(false), skipelse(false), cppifcount(0), parseoptions(parser_options), ifcount(0), ruledepth(0)

{
    // root, single mode
    if (parseoptions & OPTION_EXPRESSION)
        // root, single mode to allows for an expression without a statement
        startNewMode(MODE_TOP | MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);
    else
       // root, single mode that allows statements to be nested
       startNewMode(MODE_TOP | MODE_NEST | MODE_STATEMENT);
}

// ends all currently open modes
void srcMLParser::endAllModes() {

     // expression mode has an extra mode
     if (parseoptions & OPTION_EXPRESSION)
        endCurrentMode();

     // should only be one mode
     if (size() > 1 && isoption(parseoptions, OPTION_DEBUG))
        emptyElement(SERROR_MODE);

    // end all modes except the last
    while (size() > 1) {
        endCurrentMode();
    }

    // flush any skipped characters
    flushSkip();

    // end the very last mode which forms the entire unit
    if (size() == 1)
        endLastMode();
}

}

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class srcMLParser extends Parser;

options {
    classHeaderSuffix="public Mode";
	k=1;
    importVocab=KeywordCPPLexer;
    defaultErrorHandler=false;
    noConstructors=true;

    // values arrived at through experimentation
    codeGenBitsetTestThreshold=4;
    codeGenMakeSwitchThreshold=5;
}

tokens {
    // entire source file
    SUNIT;

    // single element
    SSINGLE;

    // First token used for boundary
    START_ELEMENT_TOKEN;

    // No output at all.  Only a placeholder
    SNOP;

    // literal types
    SSTRING;        // string marked by double quotes
    SCHAR;          // string or char marked by single quotes
    SLITERAL;       // literal number, constant
    SBOOLEAN;       // boolean literal, i.e., true, false

    // operators
    SOPERATOR;

    // type modifiers
    SMODIFIER;

    // internal statement elements used in multiple statements
    SNAME;
    SONAME;
    SCNAME;
    STYPE;
	SCONDITION;
	SBLOCK;
    SINDEX;

    // statements
	STYPEDEF;
	SENUM;
	SASM;
	SMACRO_CALL;

	SIF_STATEMENT;
	STHEN;
	SELSE;

    SWHILE_STATEMENT;
    SLOCK_STATEMENT;
    SFIXED_STATEMENT;
	SDO_STATEMENT;

	SFOR_STATEMENT;
	SFOREACH_STATEMENT;
    SFOR_GROUP;
	SFOR_INITIALIZATION;
	SFOR_CONDITION;
	SFOR_INCREMENT;

	SEXPRESSION_STATEMENT;
	SEXPRESSION;
	SFUNCTION_CALL;

	SDECLARATION_STATEMENT;
	SDECLARATION;
	SDECLARATION_INITIALIZATION;
	SDECLARATION_RANGE;

	SGOTO_STATEMENT;
	SCONTINUE_STATEMENT;
	SBREAK_STATEMENT;
	SLABEL_STATEMENT;

	SSWITCH;
	SCASE;
	SDEFAULT;

    // functions
    SFUNCTION_DEFINITION;
	SFUNCTION_DECLARATION;
	SFUNCTION_SPECIFIER;
	SRETURN_STATEMENT;
	SPARAMETER_LIST;
	SPARAMETER;
	SKRPARAMETER_LIST;
	SKRPARAMETER;
	SARGUMENT_LIST;
	SARGUMENT;

    // class, struct, union
	SCLASS;
	SCLASS_DECLARATION;
	SSTRUCT;
	SSTRUCT_DECLARATION;
	SUNION;
	SUNION_DECLARATION;
	SDERIVATION_LIST;
	SPUBLIC_ACCESS;
	SPUBLIC_ACCESS_DEFAULT;
	SPRIVATE_ACCESS;
	SPRIVATE_ACCESS_DEFAULT;
	SPROTECTED_ACCESS;
    SMEMBER_INITIALIZATION_LIST;
	SCONSTRUCTOR_DEFINITION;
	SCONSTRUCTOR_DECLARATION;
	SDESTRUCTOR_DEFINITION;
	SDESTRUCTOR_DECLARATION;
	SFRIEND;
	SCLASS_SPECIFIER;

    // extern definition
    SEXTERN;

    // namespaces
	SNAMESPACE;
	SUSING_DIRECTIVE;

    // exception handling
	STRY_BLOCK;
	SCATCH_BLOCK;
	SFINALLY_BLOCK;
	STHROW_STATEMENT;
	STHROW_SPECIFIER;
	STHROW_SPECIFIER_JAVA;

	STEMPLATE;
    STEMPLATE_ARGUMENT;
    STEMPLATE_ARGUMENT_LIST;
    STEMPLATE_PARAMETER;
    STEMPLATE_PARAMETER_LIST;
    SREQUIRES;
    SCLAUSE;

    // cpp internal elements
	SCPP_DIRECTIVE;
    SCPP_FILENAME;

    // cpp directives
	SCPP_ERROR;
	SCPP_PRAGMA;
	SCPP_INCLUDE;
	SCPP_DEFINE;
	SCPP_UNDEF;
	SCPP_LINE;
	SCPP_IF;
	SCPP_IFDEF;
	SCPP_IFNDEF;
	SCPP_THEN;
	SCPP_ELSE;
	SCPP_ELIF;

    // C# cpp directive
    SCPP_REGION;
    SCPP_ENDREGION;

    // This HAS to mark the end of the CPP directives
	SCPP_ENDIF;

    SMARKER;
    SERROR_PARSE;
    SERROR_MODE;

    // Java elements
    SIMPLEMENTS;
    SEXTENDS;
    SIMPORT;
    SPACKAGE;
    SINTERFACE;

    // C++0x elements
    SCONCEPT;
    SCONCEPTMAP;
    SAUTO;

    // C#
    SCHECKED_STATEMENT;
    SUNCHECKED_STATEMENT;

    // misc
    SEMPTY;  // empty statement

    // Last token used for boundary
    END_ELEMENT_TOKEN;
}

/*
  Included inside of generated class srcMLCPPParser.hpp
*/
{
public:

friend class LocalMode;

bool zeromode;
bool skipelse;
int cppifcount;
bool isdestructor;
int parseoptions;
std::string namestack[2];
int ifcount;
int ruledepth;

~srcMLParser() {}

srcMLParser(antlr::TokenStream& lexer, int lang = LANGUAGE_CXX, int options = 0);

struct cppmodeitem {
        cppmodeitem(int current_size)
            : statesize(1, current_size), isclosed(false), skipelse(false)
        {}

        cppmodeitem()
        {}

        std::vector<int> statesize;
        bool isclosed;
        bool skipelse;
};

std::stack<cppmodeitem, std::list<cppmodeitem> > cppmode;

void startUnit() {

   startElement(SUNIT);
   emptyElement(SUNIT);
}

// returns the current token in the output token stream
TokenPosition getTokenPosition() {
        TokenPosition tp(CurrentToken(), &(currentState().callstack.top()));
        return tp;
}

public:

void endAllModes();

}


/*
  start

  Called by nextToken for the smallest amount of parsing to produce a token.
  Extra tokens are buffered in the stream parser.  Any token here can start
  an element or cause the end of an element.

  Whitespace tokens are handled elsewhere and are automagically included
  in the output stream.

  Order of evaluation is important.
*/
start[] { ruledepth = 0; ENTRY_DEBUG } :

        COMMENT_TEXT |

        // end of file
        eof |

        // end of line
        line_continuation | EOL | LINECOMMENT_START |

        comma[true] |

        { !inTransparentMode(MODE_INTERNAL_END_PAREN) || inPrevMode(MODE_CONDITION) }? rparen[true,false] |

        // characters with special actions that usually end currently open elements
        { !inTransparentMode(MODE_INTERNAL_END_CURLY) }? block_end |

        // switch cases @test switch
        { !inMode(MODE_DERIVED) && !inMode(MODE_INIT) && (!inMode(MODE_EXPRESSION) || inTransparentMode(MODE_DETECT_COLON)) }? 
        colon[true] |

        terminate[true] |

        // don't confuse with expression block
        { inTransparentMode(MODE_CONDITION) ||
            (!inMode(MODE_EXPRESSION) && !inMode(MODE_EXPRESSION_BLOCK | MODE_EXPECT)) }? lcurly | 

        // process template operator correctly @test template
        { inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST) }? tempope[true] |

        // context-free grammar statements
        { inMode(MODE_NEST | MODE_STATEMENT) && !inMode(MODE_FUNCTION_TAIL) }? cfg |

        // statements without a context free grammar
        // last chance to match to a syntactical structure
        { inMode(MODE_NEST | MODE_STATEMENT) && !inMode(MODE_FUNCTION_TAIL) }? statements_non_cfg |

        // in the middle of a statement
        statement_part
;
exception
catch[...] {

        CATCH_DEBUG

        // need to consume the token. If we got here because
        // of an error with EOF token, then call EOF directly
        if (LA(1) == 1)
            eof();
        else
            consume();
}

/*
  context-free grammar statements
*/
cfg[] { ENTRY_DEBUG } :

        // conditional statements
        if_statement | else_statement | switch_statement | switch_case | switch_default |

        // iterative statements
        while_statement | for_statement | do_statement | foreach_statement |

        // jump statements
        return_statement | break_statement | continue_statement | goto_statement |

        // template declarations - both functions and classes
        template_declaration |

        // exception statements
        try_statement | catch_statement | finally_statement | throw_statement |

        // namespace statements
        namespace_definition | namespace_directive |

        typedef_statement |

        // java import - keyword only detected for Java
        import_statement |

        // java package - keyword only detected for Java
        package_statement |

        // C#
        checked_statement | 

        unchecked_statement | lock_statement | fixed_statement |

        // assembly block
        asm_declaration
;

/*
  statements non cfg

  Top-level items that must be matched by context.

  Basically we have an identifier and we don't know yet whether it starts an expression
  function definition, function declaration, or even a label.

  Important to keep semantic checks, e.g., (constructor)=>, in place.  Most of these rules
  can start with a name which leaves it ambiguous which to choose.
*/
statements_non_cfg[] { int token = 0; int place = 0; int secondtoken = 0; int fla = 0;
        int type_count = 0; DECLTYPE decl_type = NONE; CALLTYPE type = NOCALL; ENTRY_DEBUG } :

        // class forms for class declarations/definitions as opposed to part of a declaration types
        // must be before checking access_specifier_region
        (class_struct_union_check[token /* token after header */, place])=> class_struct_union[token, place] |

        // class forms sections
        // must be after class_struct_union_check
        { inLanguage(LANGUAGE_CXX_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
        access_specifier_region |

        // check for declaration of some kind (variable, function, constructor, destructor
        { perform_noncfg_check(decl_type, secondtoken, fla, type_count) && decl_type == FUNCTION }?
        function[fla, type_count] |

        // "~" which looked like destructor, but isn't
        { decl_type == NONE }?
        expression_statement_process
        expression_process
        sole_destop |

        // standalone macro
        { decl_type == SINGLE_MACRO }?
        macro_call |

        // variable declaration
        { decl_type == VARIABLE }?
        variable_declaration_statement[type_count] |

        // constructor
        { decl_type == CONSTRUCTOR && fla != TERMINATE }?
        constructor_definition |

        { decl_type == CONSTRUCTOR && fla == TERMINATE }?
        constructor_declaration |

        // destructor
        { decl_type == DESTRUCTOR && fla != TERMINATE }?
        destructor_definition |

        { decl_type == DESTRUCTOR && fla == TERMINATE }?
        destructor_declaration |

        // labels to goto
        { secondtoken == COLON }? label_statement |

        // enum definition as opposed to part of type or declaration
        { decl_type == NONE }?
        enum_definition |

        // extern block as opposed to enum as part of declaration
        { decl_type == NONE }?
        extern_definition |

        // C++0x additional non-cfg statements
        { look_past(AUTO) == CONCEPT }?
        concept_definition |

        { look_past(AUTO) == CONCEPTMAP }?
        conceptmap_definition |

        // call

        { inLanguage(LANGUAGE_C_FAMILY) && perform_call_check(type, secondtoken) && type == MACRO }?
        macro_call |

        expression_statement[type]
;


look_past[int skiptoken] returns [int token] {
    
    int place = mark();
    inputState->guessing++;

    while (LA(1) != antlr::Token::EOF_TYPE && LA(1) == skiptoken)
        consume();

    token = LA(1);

    inputState->guessing--;
    rewind(place);
}:;

// functions
function[int token, int type_count] { /* TokenPosition tp; */ENTRY_DEBUG } :
		{
            // function definitions have a "nested" block statement
            startNewMode(MODE_STATEMENT);

            if (token != LCURLY)
                startElement(SFUNCTION_DECLARATION);
            else
                // start the function definition element
                startElement(SFUNCTION_DEFINITION);

            // record the token position so we can replace it if necessary
//            tp = getTokenPosition();
        }
        function_header[type_count]
        {
//            if (token != LCURLY)
//                tp.setType(SFUNCTION_DECLARATION);
        }
;

perform_call_check[CALLTYPE& type, int secondtoken] returns [bool iscall] {

    iscall = true;

    type = NOCALL;

    int start = mark();
    inputState->guessing++;

    int postnametoken = 0;
    int argumenttoken = 0;
    int postcalltoken = 0;
    try {
        call_check(postnametoken, argumenttoken, postcalltoken);

        guessing_endGuessing();

        // call syntax succeeded
        type = CALL;

        // call syntax succeeded, however post call token is not legitimate
        if (inLanguage(LANGUAGE_C_FAMILY) && (_tokenSet_0.member(postcalltoken) || postcalltoken == NAME || postcalltoken == LCURLY
            || postcalltoken == EXTERN || postcalltoken == STRUCT || postcalltoken == UNION || postcalltoken == CLASS
            || postcalltoken == RCURLY || postcalltoken == 1 /* EOF ? */
            || postcalltoken == TEMPLATE || postcalltoken == PUBLIC || postcalltoken == PRIVATE
            || postcalltoken == PROTECTED))

            type = MACRO;

    } catch (...) {

        type = NOCALL;

        if (inLanguage(LANGUAGE_C_FAMILY) && argumenttoken != 0 && postcalltoken == 0) {

            guessing_endGuessing();

            type = MACRO;
        }

        // single macro call followed by statement_cfg
        else if (inLanguage(LANGUAGE_C_FAMILY) && secondtoken != -1
                 && (_tokenSet_0.member(secondtoken) || secondtoken == LCURLY || secondtoken == 1 /* EOF */
                     || secondtoken == PUBLIC || secondtoken == PRIVATE || secondtoken == PROTECTED))

            type = MACRO;
    }

    inputState->guessing--;
    rewind(start);
ENTRY_DEBUG } :
;

call_check[int& postnametoken, int& argumenttoken, int& postcalltoken] { ENTRY_DEBUG } :

        // detect name, which may be name of macro or even an expression
        function_identifier

        // record token after the function identifier for future use if this
        // fails
        markend[postnametoken]

       (
        { inLanguage(LANGUAGE_C_FAMILY) }?
        // check for proper form of argument list
        call_check_paren_pair[argumenttoken]

        guessing_endGuessing

        // record token after argument list to differentiate between call and macro
        markend[postcalltoken] |

        LPAREN
       )
;

call_check_paren_pair[int& argumenttoken] { bool name = false; ENTRY_DEBUG } :
        LPAREN

        // record token after the start of the argument list
        markend[argumenttoken]

        ( options { greedy = true; } : 

            // recursive nested parentheses
            call_check_paren_pair[argumenttoken] set_bool[name, false] | 

            // special case for something that looks like a declaration
            { !name }?
            NAME set_bool[name, true] |

            // found two names in a row, so this is not an expression
            // cause this to fail by next matching END_ELEMENT_TOKEN
            { name }?
            NAME guessing_endGuessing END_ELEMENT_TOKEN |

            // forbid parentheses (handled recursively) and cfg tokens
            { !_tokenSet_0.member(LA(1)) }? ~(LPAREN | RPAREN | TERMINATE) set_bool[name, false]
        )* 

        RPAREN
;

markend[int& token] { token = LA(1); } :
;

/* Statements CFG */

/*
  while statement, or while part of do statement
*/
while_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with nested statement (after condition)
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the while element
            startElement(SWHILE_STATEMENT);

            // expect a condition to follow the keyword
            startNewMode(MODE_CONDITION | MODE_EXPECT);
        }      
        WHILE 
;

/*
 do while statement
*/
do_statement[] { setFinalToken(); ENTRY_DEBUG } : 
        {
            // statement with nested statement (after condition)
            // set to top mode so that end of block will
            // not end statement itself
            startNewMode(MODE_STATEMENT | MODE_TOP | MODE_DO_STATEMENT);

            // start of do statement
            startElement(SDO_STATEMENT);

            // mode to nest while part of do while statement
            startNewMode(MODE_NEST | MODE_STATEMENT);
        }
        DO
        { /* condition detected in while */}
;

/*
  while part of do statement
*/
do_while[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // mode for do statement is in top mode so that
            // end of the block will not end the statement
            clearMode(MODE_TOP);

            // expect a condition to follow
            startNewMode(MODE_CONDITION | MODE_EXPECT);
        }
        WHILE 
;

/*
  start of for statement
*/
for_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with nested statement after the for group
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the for statement
            startElement(SFOR_STATEMENT);
        }
        FOR
        {
            // statement with nested statement after the for group
            startNewMode(MODE_FOR_GROUP | MODE_EXPECT);
        }
;

/*
  start of foreach statement (C#)
*/
foreach_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with nested statement after the for group
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the for statement
            startElement(SFOREACH_STATEMENT);
        }
        FOREACH
        {
            // statement with nested statement after the for group
            startNewMode(MODE_FOR_GROUP | MODE_EXPECT);
        }
;

/*
  start of for group, i.e., initialization, test, increment
*/
for_group[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // start the for group mode that will end at the next matching
            // parentheses
            replaceMode(MODE_FOR_GROUP, MODE_TOP | MODE_FOR_INITIALIZATION | MODE_IGNORE_TERMINATE |
                        MODE_INTERNAL_END_PAREN | MODE_LIST);

            // start the for heading group element
            startElement(SFOR_GROUP);
        }
        LPAREN
;

/*
  for parameter list initialization
*/
for_initialization_action[] { ENTRY_DEBUG } :
        {
            assertMode(MODE_FOR_INITIALIZATION | MODE_EXPECT);

            // setup next stage for condition in the for group mode
            replaceMode(MODE_FOR_INITIALIZATION, MODE_FOR_CONDITION);

            // setup a mode for initialization that will end with a ";"
            startNewMode(MODE_EXPRESSION | MODE_EXPECT | MODE_STATEMENT | MODE_LIST);

            startElement(SFOR_INITIALIZATION);
        }
    ;

for_initialization[] { int type_count = 0; int fla = 0; int secondtoken = 0; DECLTYPE decl_type = NONE; ENTRY_DEBUG } :
        for_initialization_action
        (
            // explicitly check for a variable declaration since it can easily
            // be confused with an expression
            { perform_noncfg_check(decl_type, secondtoken, fla, type_count) && decl_type == VARIABLE }?
            for_initialization_variable_declaration[type_count] |
            
            // explicitly check for non-terminate so that a large switch statement
            // isn't needed
            expression
        )
;

/*
  Statement for the declaration of a variable or group of variables
  in a for initialization
*/
for_initialization_variable_declaration[int type_count] { ENTRY_DEBUG } :
        {
            // start a new mode for the expression which will end
            // inside of the terminate
            startNewMode(MODE_LIST);

            startElement(SDECLARATION);
        }
        variable_declaration[type_count]
;


/*
  for parameter list condition
*/
for_condition_action[] { ENTRY_DEBUG } :
        {
            assertMode(MODE_FOR_CONDITION | MODE_EXPECT);

            // setup next stage for condition
            replaceMode(MODE_FOR_CONDITION, MODE_FOR_INCREMENT | MODE_INTERNAL_END_PAREN | MODE_LIST);

            // setup a mode for initialization that will end with a ";"
            startNewMode(MODE_EXPRESSION | MODE_EXPECT | MODE_STATEMENT | MODE_LIST );

            startElement(SFOR_CONDITION);
        }
    ;

for_condition[] { ENTRY_DEBUG } :
        for_condition_action

        // non-empty condition
        expression
;

/*
  increment in for parameter list
*/
for_increment[] { ENTRY_DEBUG } :
        { 
            assertMode(MODE_FOR_INCREMENT | MODE_EXPECT);

            clearMode(MODE_EXPECT | MODE_FOR_INCREMENT);

            // setup a mode for initialization that will end with a ";"
            startNewMode(MODE_FOR_INCREMENT | MODE_EXPRESSION | MODE_EXPECT | MODE_STATEMENT | MODE_LIST);

            if (LA(1) == RPAREN)
                // empty increment issued as single element
                emptyElement(SFOR_INCREMENT);
            else
                startElement(SFOR_INCREMENT);
        }
        expression
;

/*
 start of if statement

 if statement is first processed here.  Then prepare for a condition.  The end of the
 condition will setup for the then part of the statement.  The end of the then looks
 ahead for an else.  If so, it ends the then part.  If not, it ends the entire statement.*/
if_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with nested statement
            // detection of else
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_IF);

            ++ifcount;

            // start the if statement
            startElement(SIF_STATEMENT);

            // expect a condition
            // start THEN after condition
            startNewMode(MODE_CONDITION | MODE_EXPECT | MODE_IF_COND);
        }
        IF
;

/*
 else part of if statement

 else is detected on its own, and as part of termination (semicolon or
 end of a block
*/
else_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // treat as a statement with a nested statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_ELSE);

            // start the else part of the if statement
            startElement(SELSE);
        }
        ELSE
;

/*
 start of switch statement
*/
switch_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with nested block
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the switch statement
            startElement(SSWITCH);

            // expect a condition to follow
            startNewMode(MODE_CONDITION | MODE_EXPECT);
        }
        SWITCH 
;

/*
 actions to perform before starting a section

 There are no grammar rules to match.
*/
section_entry_action_first[] :
        {
            // start a new section inside the block with nested statements
            startNewMode(MODE_TOP_SECTION | MODE_TOP | MODE_NEST | MODE_STATEMENT);
        }
;

/*
 actions to perform before starting a section

 There are no grammar rules to match.
*/
section_entry_action[] :
        {
            // end any statements inside the section
            endDownToMode(MODE_TOP);

            // flush any whitespace tokens since sections should
            // end at the last possible place
            flushSkip();

            // end the section inside the block
            endDownOverMode(MODE_TOP_SECTION);
        }
        section_entry_action_first
;

/*
 Yes, case isn't really a statement, but it is treated as one
*/
switch_case[] { setFinalToken(); ENTRY_DEBUG } :
        // start a new section
        section_entry_action
        {
            // start of case element
            startElement(SCASE);

            // expect an expression ended by a colon
            startNewMode(MODE_EXPRESSION | MODE_EXPECT | MODE_DETECT_COLON);
        }
        CASE 
;

switch_default[] { setFinalToken(); ENTRY_DEBUG } :
        // start a new section
        section_entry_action
        {
            // start of case element
            startElement(SDEFAULT);

            // filler mode ended by the colon
            startNewMode(MODE_STATEMENT);
        }
        DEFAULT
;

/*
  start of return statement
*/
import_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME | MODE_EXPECT);

            // start the return statement
            startElement(SIMPORT);
        }
        IMPORT
;

/*
  start of package statement
*/
package_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME | MODE_EXPECT);

            // start the return statement
            startElement(SPACKAGE);
        }
        PACKAGE
;

/*
  start of return statement
*/
return_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the return statement
            startElement(SRETURN_STATEMENT);
        }
        RETURN
;

/*
  start of break statement
*/
break_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the break statement
            startElement(SBREAK_STATEMENT);
        }
        BREAK
;

/*
  start of continue statement
*/
continue_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the continue statement
            startElement(SCONTINUE_STATEMENT);
        }
        CONTINUE
;

/*
  start of goto statement
*/
goto_statement[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with an expected label name
            // label name is a subset of variable names
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME);

            // start the goto statement
            startElement(SGOTO_STATEMENT);
        }
        GOTO
;

/*
  Complete assembly declaration statement
*/
asm_declaration[] { ENTRY_DEBUG } : 
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the asm statement
            startElement(SASM);
        }
        ASM
        (balanced_parentheses | ~(LCURLY | RCURLY | TERMINATE))*
;

/*
 Examples:
   namespace {}
   namespace name {}
   namespace name1 = name2;

 Past name handled as expression
*/
extern_definition_header[] { ENTRY_DEBUG } :
        EXTERN string_literal
;

extern_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_EXTERN);

            // start the namespace definition
            startElement(SEXTERN);
        }
        EXTERN
;

/*
  Name of extern section
*/
extern_name[] { ENTRY_DEBUG } :
        string_literal
        {
            // nest a block inside the namespace
            setMode(MODE_NEST | MODE_STATEMENT);
        }
;

/*
 Examples:
   namespace {}
   namespace name {}
   namespace name1 = name2;

 Past name handled as expression
*/
namespace_definition[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NAMESPACE | MODE_VARIABLE_NAME);

            // start the namespace definition
            startElement(SNAMESPACE);
        }
        NAMESPACE
;

namespace_alias[] { setFinalToken(); ENTRY_DEBUG } :

        EQUAL 
        {
            // expect a label name
            // label name is a subset of variable names
            setMode(MODE_VARIABLE_NAME);
        }
;

namespace_block[] { ENTRY_DEBUG } :
        {
            // nest a block inside the namespace
            setMode(MODE_NEST | MODE_STATEMENT);
        }
        lcurly 
;

/*
  start of namespace using directive
*/
namespace_directive[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // statement with an expected namespace name after the keywords
            startNewMode(MODE_STATEMENT | MODE_FUNCTION_NAME);

            // start the using directive
            startElement(SUSING_DIRECTIVE);
        }
        USING
;

/* Declarations Definitions CFG */

/*
  class structures and unions
*/
class_struct_union[int token, int place] { ENTRY_DEBUG } :

        { token == LCURLY && place == INTERFACE }?
        interface_definition |

        { token == LCURLY && place == CLASS }?
        class_definition |

        { token == LCURLY && place == STRUCT }?
        struct_union_definition[SSTRUCT] |

        { token == LCURLY && place == UNION }?
        struct_union_definition[SUNION] |

        { place == CLASS }?
        class_declaration |

        { place == STRUCT }?
        struct_declaration |

        { place == UNION }?
        union_declaration
;

/*
  class structures and unions
*/
class_struct_union_check[int& finaltoken, int& othertoken] { finaltoken = 0; othertoken = 0; ENTRY_DEBUG } :

        (java_specifier_mark)* markend[othertoken] (CLASS | STRUCT | UNION | INTERFACE) class_header check_end[finaltoken]
;

check_end[int& token] { /* setFinalToken(); // problem with class */ token = LA(1); ENTRY_DEBUG } :
        LCURLY | TERMINATE | COLON | COMMA | RPAREN
;

/*
*/
class_declaration[] :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class definition
            startElement(SCLASS_DECLARATION);
        }
        (java_specifier_mark)* CLASS class_header
;

/*
*/
class_definition[] :
        {
            bool intypedef = inMode(MODE_TYPEDEF);

            // statement
            startNewMode(MODE_STATEMENT | MODE_BLOCK | MODE_NEST | MODE_CLASS | MODE_DECL);

            // start the class definition
            startElement(SCLASS);

            // java classes end at the end of the block
            if (intypedef || inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP)) {
                setMode(MODE_END_AT_BLOCK);
            }
        }
        (java_specifier_mark)* CLASS (class_header lcurly | lcurly) 
        {

            if (inLanguage(LANGUAGE_CXX_FAMILY) && !inLanguage(LANGUAGE_CSHARP))
                class_default_access_action(SPRIVATE_ACCESS_DEFAULT);
        }
;

enum_class_definition[] :
        {
            bool intypedef = inMode(MODE_TYPEDEF);

            // statement
            startNewMode(MODE_STATEMENT | MODE_BLOCK | MODE_NEST | MODE_CLASS | MODE_DECL);

            // start the class definition
            startElement(SENUM);

            // java classes end at the end of the block
            if (intypedef || inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP)) {
                setMode(MODE_END_AT_BLOCK);
            }
        }
        (java_specifier_mark)* ENUM (class_header lcurly | lcurly) 
        {

            if (inLanguage(LANGUAGE_CXX_FAMILY))
                class_default_access_action(SPRIVATE_ACCESS_DEFAULT);
        }
;

concept_definition[] :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_BLOCK | MODE_NEST | MODE_CLASS | MODE_END_AT_BLOCK);

            // start the class definition
            startElement(SCONCEPT);
        }
        (
            (auto_keyword)* CONCEPT

            identifier[true] 
                {
                    startNewMode(MODE_EXPECT | MODE_DERIVED);

                    startNewMode(MODE_TEMPLATE | MODE_LIST | MODE_EXPECT | MODE_DERIVED);
                }
        )
;

conceptmap_definition[] :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_BLOCK | MODE_NEST | MODE_CLASS | MODE_END_AT_BLOCK);

            // start the class definition
            startElement(SCONCEPTMAP);
        }
        (auto_keyword)* CONCEPTMAP

        complex_name[true] 
;

anonymous_class_definition[] :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_BLOCK | MODE_NEST | MODE_CLASS | MODE_END_AT_BLOCK);

            // start the class definition
            startElement(SCLASS);

        }

        // first name in an anonymous class definition is the class it extends
        // or the interface that it implements
        anonymous_class_super 

        // argument list
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);
        }
        call_argument_list
;

anonymous_class_super[] { LocalMode lm(this); }:
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the super name of an anonymous class
            startElement(SDERIVATION_LIST);
        }
        complex_name[true]
;

interface_definition[] :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_BLOCK | MODE_NEST | MODE_CLASS);

            // start the interface definition
            startElement(SINTERFACE);

            // java interfaces end at the end of the block
            setMode(MODE_END_AT_BLOCK); 
        }
        (java_specifier_mark)* INTERFACE class_header lcurly
;

/*
*/
struct_declaration[] :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class definition
            startElement(SSTRUCT_DECLARATION);
        }
        (java_specifier_mark)* STRUCT class_header
;

struct_union_definition[int element_token] :
        {
            bool intypedef = inMode(MODE_TYPEDEF);

            // statement
            startNewMode(MODE_STATEMENT | MODE_BLOCK | MODE_NEST | MODE_DECL);

            // start the struct definition
            startElement(element_token);

            if (intypedef) {
                setMode(MODE_END_AT_BLOCK);
            }
        }
        (java_specifier_mark)* (STRUCT | UNION) (class_header lcurly | lcurly)

        class_default_access_action[SPUBLIC_ACCESS_DEFAULT]
;

/*
*/
union_declaration[] :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class definition
            startElement(SUNION_DECLARATION);
        }
        (java_specifier_mark)* UNION class_header
;

/*
   Classes and structs in C++ have a default private/public section.  This handles it.
*/
class_default_access_action[int access_token] :
        {
            if (inLanguage(LANGUAGE_CXX_FAMILY) && (SkipBufferSize() > 0 ||
                !(LA(1) == PUBLIC || LA(1) == PRIVATE || LA(1) == PROTECTED))) {

                // setup block section
                section_entry_action_first();

                // start private element
                if (LA(1) == RCURLY && SkipBufferSize() == 0)
                    // empty element for empty (no ws even) block
                    emptyElement(access_token);
                else {
                    // start private element before whitespace
                    startNoSkipElement(access_token);
                    setMode(MODE_ACCESS_REGION);
                }

            /* Have to setup an empty section for anonymouse structs, not sure why */
            } else if (inLanguage(LANGUAGE_C)) {
                section_entry_action_first();
            }
        }
;

/*
 header (part before block) of class (or struct or union)
*/
class_header[] { ENTRY_DEBUG } :

        /*
          TODO
          
          This shouldn't be needed, but uncommenting the predicate causes Java
          to mess up with template parameters, but not C++ ???
        */
        { inLanguage(LANGUAGE_C_FAMILY) }?
        (macro_call_check class_header_base LCURLY)=>
           macro_call class_header_base |

        class_header_base
;

/*
 header (part before block) of class (or struct or union)
*/
class_header_base[] { bool insuper = false; ENTRY_DEBUG } :

        complex_name[true] (

            { inLanguage(LANGUAGE_C_FAMILY) }?
            (options { greedy = true; } : derived)* | 

            { inLanguage(LANGUAGE_JAVA_FAMILY) }?
            (options { greedy = true; } : super_list_java { insuper = true; } extends_list)* 
                ( { if (!insuper) { insuper = true; super_list_java(); } } implements_list)*
                {
                    if (insuper)
                        endCurrentMode();
                }
       )
;

/*
  Each instance of an access specifier defines a region in the class
*/
access_specifier_region[] { ENTRY_DEBUG } : 
        section_entry_action
        {
            // mark access regions to detect statements that only occur in them
            setMode(MODE_ACCESS_REGION);
        }
        (
            {
                // start of case element
                startElement(SPUBLIC_ACCESS);
            }
            PUBLIC |
            {
                // start of case element
                startElement(SPRIVATE_ACCESS);
            }
            PRIVATE |
            {
                // start of case element
                startElement(SPROTECTED_ACCESS);
            }
            PROTECTED
        )
        { setFinalToken(); }
        COLON
;

/*
  left curly brace

  Marks the start of a block.  End of the block is handled in right curly brace
*/
lcurly[] { ENTRY_DEBUG } :
        {
            // special end for conditions
            if (inTransparentMode(MODE_CONDITION)) {
                endDownToMode(MODE_CONDITION);
                endCurrentMode(MODE_CONDITION);
            }
           
            if (inMode(MODE_IF)) {

                // then part of the if statement (after the condition)
                startNewMode(MODE_STATEMENT | MODE_NEST | MODE_THEN);

                // start the then element
                startNoSkipElement(STHEN);
            }

            // special end for constructor member initialization list
            if (inMode(MODE_LIST | MODE_CALL)) {
                // flush any whitespace tokens since sections should
                // end at the last possible place
                flushSkip();

                endCurrentMode(MODE_LIST | MODE_CALL);
            }
        }
        lcurly_base
        {
            // alter the modes set in lcurly_base
            setMode(MODE_TOP | MODE_NEST | MODE_STATEMENT | MODE_LIST);
        }
;

/*
  left curly brace

  Marks the start of a block.  End of the block is handled in right curly brace
*/
lcurly_base[] { setFinalToken(); ENTRY_DEBUG } :
        {  
            // need to pass on class mode to detect constructors for Java
            bool inclassmode = inLanguage(LANGUAGE_JAVA_FAMILY) && inMode(MODE_CLASS);

            startNewMode(MODE_BLOCK);

            if (inclassmode)
                setMode(MODE_CLASS);

            startElement(SBLOCK);

        }
        LCURLY
;

/*
  Marks the end of a block.  Also indicates the end of some open elements.
*/
block_end[] { ENTRY_DEBUG } :
        // handling of if with then block followed by else
        // handle the block, however scope of then completion stops at if
        rcurly
        { 
            // end all statements this statement is nested in
            // special case when ending then of if statement

            // end down to either a block or top section, or to an if, whichever is reached first
            endDownToFirstMode(MODE_BLOCK | MODE_TOP | MODE_IF | MODE_ELSE | MODE_TRY);
        }
        else_handling
        {
            bool endatblock = inMode(MODE_END_AT_BLOCK);

            // some statements end with the block
            if (inMode(MODE_END_AT_BLOCK))
                endCurrentMode(MODE_LOCAL);

            if (endatblock && inTransparentMode(MODE_TEMPLATE))
                endCurrentMode(MODE_LOCAL);

            // looking for a terminate (';').  may have some whitespace before it
            consumeSkippedTokens();

            // some statements end with the block if there is no terminate
            if (inMode(MODE_END_AT_BLOCK_NO_TERMINATE) && LA(1) != TERMINATE)
                endCurrentMode(MODE_LOCAL);

            // if we are in a declaration (as part of a class/struct/union definition)
            // then we needed to markup the (abbreviated) variable declaration
            if (inMode(MODE_DECL) && LA(1) != TERMINATE)
                short_variable_declaration();

            // end of block may lead to adjustment of cpp modes
            cppmode_adjust();
        }

;

/*
  right curly brace

  Not used directly, but called by block_end
*/
rcurly[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // end any elements inside of the block
            endDownToMode(MODE_TOP);

            // flush any whitespace tokens since sections should
            // end at the last possible place
            flushSkip();

            // end any sections inside the mode
            endDownOverMode(MODE_TOP_SECTION);
        }
        RCURLY
        {
            // end the current mode for the block
            // don't end more than one since they may be nested
            endCurrentMode(MODE_TOP);
        }
;

/*
  End any open expressions, match, then close any open elements
*/
terminate[bool final = false] { if(final) setFinalToken(); ENTRY_DEBUG } :

        {
            if (inMode(MODE_IGNORE_TERMINATE)) {

                if (inMode(MODE_FOR_INITIALIZATION | MODE_EXPECT))
                    for_initialization_action();
                else
                    for_condition_action();
            }
        }
        terminate_pre
        terminate_token
        terminate_post
;

terminate_token[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            if (inMode(MODE_NEST | MODE_STATEMENT) && !inMode(MODE_DECL) && !inMode(MODE_IF)) {

                startNewMode(MODE_LOCAL);

                startElement(SEMPTY);
            }
        }
        TERMINATE
    ;

terminate_pre[] { ENTRY_DEBUG } :
        {
            // end any elements inside of the statement
            if (!inMode(MODE_TOP | MODE_NEST | MODE_STATEMENT))
                endDownToFirstMode(MODE_STATEMENT | MODE_EXPRESSION_BLOCK |
                                   MODE_INTERNAL_END_CURLY | MODE_INTERNAL_END_PAREN);
        }
;

terminate_post[] { ENTRY_DEBUG } :
        {
            // end all statements this statement is nested in
            // special case when ending then of if statement
            if (!(parseoptions & OPTION_EXPRESSION) &&
                 (!inMode(MODE_EXPRESSION_BLOCK) || inMode(MODE_EXPECT)) &&
                !inMode(MODE_INTERNAL_END_CURLY) && !inMode(MODE_INTERNAL_END_PAREN)) {

                // end down to either a block or top section, or to an if or else
                endDownToFirstMode(MODE_TOP | MODE_IF | MODE_ELSE);
            }
        }
        else_handling
;

/*
  Handle possible endings of else statements.

  Called from all places that end a statement, and could possibly end the else that the target statement was in.
  I.e., terminate ';', end of a statement block, etc.

  If in an if-statement, relatively straightforward.  Note that we could be ending with multiple else's.

  Special case:  else with no matching if.  This occurs with a) a single else, or more likely with b) an
  else in a preprocessor #if .. #else ... #endif construct (actually, very common).
*/
else_handling[] { ENTRY_DEBUG } :
        {
                // record the current size of the top of the cppmode stack to detect
                // any #else or #endif in the consumeSkippedTokens
                // see below
                unsigned int cppmode_size = !cppmode.empty() ? cppmode.top().statesize.size() : 0;

                // move to the next non-skipped token
                consumeSkippedTokens();

                // catch and finally statements are nested inside of a try, if at that level
                // so if no CATCH or FINALLY, then end now
                bool intry = inMode(MODE_TRY);
                bool restoftry = LA(1) == CATCH || LA(1) == FINALLY;
                if (intry && !restoftry) {
                    endCurrentMode(MODE_TRY);
                    endDownToMode(MODE_TOP);
                }

                // handle parts of if
                if (inTransparentMode(MODE_IF) && !(intry && restoftry)) {

                    // find out if the next token is an else
                    bool nestedelse = LA(1) == ELSE;

                    // if there isn't an ELSE next, then just end like other statements
                    if (!nestedelse) {
                        endDownToMode(MODE_TOP);

                    // when an ELSE is next and already in an else, must end properly (not needed for then)
                    } else if (inMode(MODE_ELSE)) {

                        while (inMode(MODE_ELSE)) {

                            // end the else
                            endCurrentMode(MODE_ELSE);

                            // move to the next non-skipped token
                            consumeSkippedTokens();

                            /*
                              TODO:  Can we only do this if we detect a cpp change?

                              This would occur EVEN if we have an ifcount of 2.
                             */
                            // we have an extra else that is rogue
                            // it either is a single else statement, or part of an #ifdef ... #else ... #endif
                            if (LA(1) == ELSE && ifcount == 1)
                                break;

                            // ending an else means ending an if
                            if (inMode(MODE_IF)) {
                                endCurrentModeSafely(MODE_IF);
                                --ifcount;
                            }
                        }  

                        // following ELSE indicates end of outer then
                        endCurrentModeSafely(MODE_THEN);
                    }

                } else if (inTransparentMode(MODE_ELSE)) {

                    // have an else, but are not in an if.  Could be a fragment,
                    // or could be due to an #ifdef ... #else ... #endif
                    endCurrentModeSafely(MODE_ELSE);
                }

            // update the state size in cppmode if changed from using consumeSkippedTokens
            if (!cppmode.empty() && cppmode_size != cppmode.top().statesize.size()) {

                cppmode.top().statesize.back() = size();

                // remove any finished ones
                if (cppmode.top().isclosed)    {
                        cppmode_cleanup();
                }
            }
        }
;

/*
  Handling when mid-statement
*/
statement_part[] { int type_count; int fla = 0; int secondtoken = 0; DECLTYPE decl_type = NONE; CALLTYPE type = NOCALL; ENTRY_DEBUG } :

        { inMode(MODE_EAT_TYPE) }?
            type_identifier
            update_typecount |

        { inMode(MODE_EAT_VAR_TYPE) }?
            type_identifier
            update_typecount |

        /*
          MODE_FUNCTION_TAIL
        */

        // throw list at end of function header
        { (inLanguage(LANGUAGE_OO)) && inMode(MODE_FUNCTION_TAIL) }?
             throw_list |

        // function specifier at end of function header
        { inLanguage(LANGUAGE_CXX_FAMILY) && inMode(MODE_FUNCTION_TAIL) }?
             function_specifier |

        // K&R function parameters
        { inLanguage(LANGUAGE_C_FAMILY) && inMode(MODE_FUNCTION_TAIL) && 
          perform_noncfg_check(decl_type, secondtoken, fla, type_count) && decl_type == VARIABLE }?
            kr_parameter |

        /*
          MODE_EXPRESSION
        */

        // block right after argument list, e.g., throws list in Java
        { inTransparentMode(MODE_END_LIST_AT_BLOCK) }?
        { endDownToMode(MODE_LIST); endCurrentMode(MODE_LIST); }
            lcurly | 

        // expression block or expressions
        // must check before expression
        { inMode(MODE_EXPRESSION_BLOCK | MODE_EXPECT) }?
             pure_expression_block |

        // start of argument for return or throw statement
        { inMode(MODE_EXPRESSION | MODE_EXPECT) &&
            inLanguage(LANGUAGE_C_FAMILY) && perform_call_check(type, secondtoken) && type == MACRO }?
        macro_call |

        { inMode(MODE_EXPRESSION | MODE_EXPECT) }?
        expression[type] |

        // already in an expression, and run into a keyword
        // so stop the expression, and markup the keyword statement
        { inMode(MODE_EXPRESSION) }?
             terminate_pre
             terminate_post
             cfg |

        // already in an expression
        { inMode(MODE_EXPRESSION) }?
             expression_part |

        // call list in member initialization list
        { inMode(MODE_CALL | MODE_LIST) }?
             call |

        /*
          MODE_VARIABLE_NAME
        */

        // special case for type modifiers
        { inMode(MODE_VARIABLE_NAME | MODE_INIT) }?
             multops |

        { inMode(MODE_VARIABLE_NAME | MODE_INIT) }?
             tripledotop |

        // start of argument for return or throw statement
        { inMode(MODE_VARIABLE_NAME | MODE_INIT) }?
             variable_declaration_nameinit |

        // variable name
        { inMode(MODE_VARIABLE_NAME) }?
             variable_identifier |

        // function identifier
        { inMode(MODE_FUNCTION_NAME) }?
             function_header[0] |

        // function identifier
        { inMode(MODE_FUNCTION_PARAMETER) }?
             parameter_list |

        // start of argument for return or throw statement
        { inMode(MODE_INIT | MODE_EXPECT) && inTransparentMode(MODE_TEMPLATE) }?
             parameter_declaration_initialization |

        // start of argument for return or throw statement
        { inMode(MODE_INIT | MODE_EXPECT) }?
             variable_declaration_initialization |

        // start of argument for return or throw statement
        { inMode(MODE_INIT | MODE_EXPECT) }?
             variable_declaration_range |

        // in an argument list expecting an argument
        { inMode(MODE_ARGUMENT | MODE_LIST) }?
             argument |

        // start of condition for if/while/switch
        { inMode(MODE_PARAMETER | MODE_EXPECT) }?
             parameter |

        /*
          Check for MODE_FOR_CONDITION before template stuff, since it can conflict
        */

        // inside of for group expecting initialization
        { inMode(MODE_FOR_GROUP | MODE_EXPECT) }?
            for_group |

        // inside of for group expecting initialization
        { inMode(MODE_FOR_INITIALIZATION | MODE_EXPECT) }?
            for_initialization |

        // inside of for group expecting initialization
        { inMode(MODE_FOR_CONDITION | MODE_EXPECT) }?
            for_condition |

        // inside of for group expecting initialization
        { inMode(MODE_FOR_INCREMENT | MODE_EXPECT) }?
            for_increment |

        { inTransparentMode(MODE_TEMPLATE) && inMode(MODE_LIST | MODE_EXPECT) }?
             template_param_list |

        // expecting a template parameter
        { inTransparentMode(MODE_TEMPLATE) && inMode(MODE_LIST) }?
             template_param |

        // expecting a template parameter
        { inLanguage(LANGUAGE_CXX_FAMILY) && inMode(MODE_DERIVED) && inMode(MODE_EXPECT) }?
             derived |

        // start of condition for if/while/switch
        { inMode(MODE_CONDITION | MODE_EXPECT) }?
             condition |

        // while part of do statement
        { inMode(MODE_DO_STATEMENT) }?
             do_while |

        { inMode(MODE_NAMESPACE) }?
        namespace_alias |

        { inMode(MODE_NAMESPACE) }?
        namespace_block |

        // string literal of extern
        { inMode(MODE_EXTERN) }?
             extern_name |

        // sometimes end up here, as when for group ends early, or with for-each
        rparen |

        // seem to end up here for colon in ternary operator
        colon_marked
;

lparen_marked[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            incParen();

            if (isoption(parseoptions, OPTION_OPERATOR)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SOPERATOR);
            }
        }
        LPAREN  
;

comma[bool final = false] { if (final) setFinalToken(); ENTRY_DEBUG }:
        {
            // comma ends the current item in a list
            // or ends the current expression
            if (!inTransparentMode(MODE_PARSE_EOL) && (inTransparentMode(MODE_LIST) || inTransparentMode(MODE_NEST | MODE_STATEMENT))) {

                // might want to check for !inMode(MODE_INTERNAL_END_CURLY)
                endDownToFirstMode(MODE_LIST | MODE_STATEMENT);
            }

            // comma in a variable initialization end init of current variable
            if (inMode(MODE_IN_INIT)) {
                endCurrentMode(MODE_IN_INIT);
            }
        }
        COMMA
;

colon_marked[bool final = false] { LocalMode lm(this); if (final) setFinalToken(); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SOPERATOR);
            }
        }
        COLON
;

colon[bool final = false] { if (final) setFinalToken(); ENTRY_DEBUG } :
        {
            if (inTransparentMode(MODE_TOP_SECTION))
                // colon ends the current item in a list
                endDownToMode(MODE_TOP_SECTION);
        }
        COLON
;

/*
  Condition contained in if/while/switch.

  Triggered by mode MODE_CONDITION | MODE_EXPECT and left parentheses.
  Starts condition mode and prepares to handle embedded expression.
  End of the element is handled in condition_rparen.
*/
condition[] { setFinalToken(); ENTRY_DEBUG } :
        {
            assertMode(MODE_CONDITION | MODE_EXPECT);

            // start element condition outside of the left parentheses
            startElement(SCONDITION); 

            // mark the condition mode as the one to stop at a right parentheses
            // non-empty conditions contain an expression
            setMode(MODE_LIST | MODE_EXPRESSION | MODE_EXPECT);
        }
        LPAREN
;

/* Function */

function_pointer_name_grammar[] { ENTRY_DEBUG } :
        LPAREN function_pointer_name_base RPAREN
;

function_pointer_name_base[] { ENTRY_DEBUG } :

        // special case for function pointer names that don't have '*'
        (complex_name[true] RPAREN)=>
            complex_name[true] |

        // special name prefix of namespace or class
        identifier (template_argument_list)* DCOLON function_pointer_name_base |

        // typical function pointer name
        MULTOPS (complex_name[true])*
;

/*
  Everything except the ";" of a function declaration or the block of a
  function definition
*/
function_header[int type_count] { ENTRY_DEBUG } : 

        // no return value functions:  casting operator method and main
        { type_count == 0 }? function_identifier
        { replaceMode(MODE_FUNCTION_NAME, MODE_FUNCTION_PARAMETER | MODE_FUNCTION_TAIL); } |

        function_type[type_count]
;

/*
Guessing mode only
*/
function_tail[] { ENTRY_DEBUG } :
        // at most only one throwlist expected.  0-many is more efficient
        (options { greedy = true; } :

            /* order is important */

            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            function_specifier |

            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            TRY | 

            { inLanguage(LANGUAGE_OO) }?
            complete_throw_list |

            // K&R 
            { inLanguage(LANGUAGE_C) }? (

            // FIXME:  Must be integrated into other C-based languages
            // FIXME:  Wrong markup
            ( macro_call )=> macro_call | 
            { look_past(NAME) == LCURLY }? NAME |
              parameter (MULTOPS | NAME | COMMA)* TERMINATE
            )
        )*
;

perform_noncfg_check[DECLTYPE& type, int& token, int& fla, int& type_count, bool inparam = false] returns [bool isdecl] {

    isdecl = true;

    type = NONE;

    int start = mark();
    inputState->guessing++;

    try {
        noncfg_check(token, fla, type_count, type, inparam);

    } catch (...) {

        if (type == VARIABLE && type_count == 0)
            type_count = 1;
    }

    // may just have a single macro (no parens possibly) before a statement
    if (type == 0 && type_count == 0 && _tokenSet_0.member(LA(1)))
        type = SINGLE_MACRO;

    // may just have an expression 
    if (type == DESTRUCTOR && !inLanguage(LANGUAGE_CXX_FAMILY))
        type = NULLOPERATOR;

    // false constructor for java
    if (inLanguage(LANGUAGE_JAVA_FAMILY) && type == CONSTRUCTOR && fla != LCURLY)
        type = NONE;

    inputState->guessing--;
    rewind(start);
} :
;

/*
  Figures out if we have a declaration, either variable or function.

  This is pretty complicated as it has to figure out whether it is a declaration or not,
  and whether it is a function or a variable declaration.
*/
noncfg_check[int& token,      /* second token, after name (always returned) */
             int& fla,        /* for a function, TERMINATE or LCURLY, 0 for a variable */
             int& type_count, /* number of tokens in type (not including name) */
             DECLTYPE& type,
             bool inparam     /* are we in a parameter */
        ] { token = 0; fla = 0; type_count = 0; int specifier_count = 0; isdestructor = false;
        type = NONE; bool foundpure = false; bool isoperatorfunction = false; bool isconstructor = false; bool saveisdestructor = false; bool endbracket = false; bool modifieroperator = false; ENTRY_DEBUG } :

        // main pattern for variable declarations, and most function declaration/definitions.
        // trick is to look for function declarations/definitions, and along the way record
        // if a declaration

        // int -> NONE
        // int f -> VARIABLE
        // int f(); -> FUNCTION
        // int f() {} -> FUNCTION

        /*
          Process all the parts of a potential type.  Keep track of total
          parts, specifier parts, and second token
        */
        ({ inLanguage(LANGUAGE_JAVA_FAMILY) || LA(1) != LBRACKET }?

            // was their a bracket on the end?  Need to know for Java
            set_bool[endbracket, inLanguage(LANGUAGE_JAVA_FAMILY) && LA(1) == LBRACKET]

            // record any type modifiers that are also operators
            // this is for disambiguation of destructor declarations from expressions involving
            // the ~ operator
            set_bool[modifieroperator, modifieroperator || LA(1) == REFOPS || LA(1) == MULTOPS]

            (
                // specifiers
                (standard_specifiers)=>
                standard_specifiers set_int[specifier_count, specifier_count + 1] |

                { inLanguage(LANGUAGE_JAVA_FAMILY) }?
                (template_argument_list)=>
                template_argument_list set_int[specifier_count, specifier_count + 1] |

                // typical type name
                complex_name[true] set_bool[foundpure]
                    set_bool[isoperatorfunction, inLanguage(LANGUAGE_CXX_FAMILY) && (isoperatorfunction ||
                             (namestack[0] == "operator" && type_count == 0))] |

                // special function name
                MAIN set_bool[isoperatorfunction, type_count == 0] |

                // type parts that can occur before other type parts (excluding specifiers)
                pure_lead_type_identifier_no_specifiers set_bool[foundpure] |

                // type parts that must only occur after other type parts (excluding specifiers)
                non_lead_type_identifier throw_exception[!foundpure]
            )

            // another type part
            set_int[type_count, type_count + 1]

            // record second (before we parse it) for label detection
            set_int[token, LA(1), type_count == 1]

        )*
        // adjust type tokens to eliminate for last left bracket (only for Java)
        set_int[type_count, endbracket ? type_count - 1 : type_count]

        // have a sequence of type tokens, last one is function/variable name
        // (except for function pointer, which is handled later)
        set_int[type_count, type_count > 1 ? type_count - 1 : 0]

        set_bool[isoperatorfunction, isoperatorfunction || isdestructor]

        // special case for what looks like a destructor declaration
        throw_exception[isdestructor && modifieroperator]

        /*
          We have a declaration (at this point a variable) if we have:
          
            - At least one non-specifier in the type
            - There is nothing in the type (what was the name is the type)
              and it is part of a parameter list
        */
        set_type[type, VARIABLE, ((type_count - specifier_count > 0) ||
                                 (inparam && (LA(1) == RPAREN || LA(1) == COMMA || LA(1) == LBRACKET)))]

        // need to see if we possibly have a constructor/destructor name, with no type
        set_bool[isconstructor,

                 // entire type is specifiers
                 (type_count == specifier_count) &&

                 // inside of a class definition
                 ((inMode(MODE_ACCESS_REGION) && inLanguage(LANGUAGE_CXX_FAMILY)) ||
                  (inMode(MODE_CLASS) && inLanguage(LANGUAGE_JAVA_FAMILY)) ||
                  (inLanguage(LANGUAGE_JAVA_FAMILY) && LA(1) == LPAREN) ||

                 // outside of a class definition, but with properly prefixed name
                 (inLanguage(LANGUAGE_CXX_FAMILY) && namestack[0] != "" && namestack[1] != "" && namestack[0] == namestack[1]))]

        // need to see if we possibly have a constructor/destructor name, with no type
        set_bool[isoperatorfunction, isoperatorfunction || isconstructor]

        // detecting a destructor name uses a data member, since it is detected in during the
        // name detection.  If the parameters use this method, it is reentrant, so cache it
        set_bool[saveisdestructor, isdestructor]

        // we have a declaration, so do we have a function?
        (
            // check for function pointer, which must have a non-specifier part of the type
            (function_pointer_name_grammar eat_optional_macro_call LPAREN)=>
            function_pointer_name_grammar
        
            // what was assumed to be the name of the function is actually part of the type
            set_int[type_count, type_count + 1]

            function_rest[fla] |

            // POF (Plain Old Function)
            // need at least one non-specifier in the type (not including the name)
            { (type_count - specifier_count > 0) || isoperatorfunction }?
            function_rest[fla]
        )

        // since we got this far, we have a function
        set_type[type, FUNCTION]

        // however, we could have a destructor
        set_type[type, DESTRUCTOR, saveisdestructor]

        // could also have a constructor
        set_type[type, CONSTRUCTOR, !saveisdestructor && isconstructor]
;

//monitor { std::cerr << namestack[0] << " " << namestack[1] << std::endl; } :;

//other[bool flag] { std::cerr << flag << std::endl; } :;

throw_exception[bool cond = true] { if (cond) throw antlr::RecognitionException(); } :;

set_type[DECLTYPE& name, DECLTYPE value, bool result = true] { if (result) name = value; } :;

trace[const char*s ] { std::cerr << s << std::endl; } :;
trace_int[int s] { std::cerr << "HERE " << s << std::endl; } :;

//traceLA { std::cerr << "LA(1) is " << LA(1) << " " << LT(1)->getText() << std::endl; } :;

marker[] { LocalMode lm(this); startNewMode(MODE_LOCAL); startElement(SMARKER); } :;

set_int[int& name, int value, bool result = true] { if (result) name = value; } :;

set_bool[bool& variable, bool value = true] { variable = value; } :;

/*
message[const char* s] { std::cerr << s << std::endl; ENTRY_DEBUG } :;

message_int[const char* s, int n]  { std::cerr << s << n << std::endl; ENTRY_DEBUG } :;
*/

function_rest[int& fla] { ENTRY_DEBUG } :

        eat_optional_macro_call

        parameter_list function_tail check_end[fla]
;

/*
  Type of a function.  Includes specifiers
*/
function_type[int type_count] { ENTRY_DEBUG } :
        {
            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_EAT_TYPE);

            setTypeCount(type_count);

            // type element begins
            startElement(STYPE);
        }
        lead_type_identifier
        update_typecount
;

update_typecount[] {} :
        {
            decTypeCount();

            if (getTypeCount() <= 0) {
                endCurrentMode(MODE_LOCAL);
                setMode(MODE_FUNCTION_NAME); 
            } 
        }
;

update_var_typecount[] {} :
        {
            decTypeCount();

            if (getTypeCount() <= 0) {
                endCurrentMode(MODE_LOCAL);
                setMode(MODE_VARIABLE_NAME | MODE_INIT); 
            } 
        }
;

/*
  Type of a function.  Includes specifiers
*/
function_type_check[int& type_count] { type_count = 1; ENTRY_DEBUG } :

        lead_type_identifier
        ( { inLanguage(LANGUAGE_JAVA_FAMILY) || LA(1) != LBRACKET }? type_identifier_count[type_count])*
;

type_identifier_count[int& type_count] { ++type_count; ENTRY_DEBUG } :

        // overloaded parentheses operator
        { LA(1) == OPERATOR }?
        overloaded_operator |

        type_identifier | MAIN
;

deduct[int& type_count] { --type_count; } :;

eat_type[int count] { if (count <= 0) return; ENTRY_DEBUG } :

        type_identifier
        eat_type[count - 1]
;

/*
  throw list for a function
*/
throw_list[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_EXPECT);

            startElement(STHROW_SPECIFIER);
        }
        THROW LPAREN |
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_EXPECT | MODE_END_LIST_AT_BLOCK);

            startElement(STHROW_SPECIFIER_JAVA);
        }
        THROWS
        {
//            endCurrentMode(MODE_LIST | MODE_EXPECT);
        }
;  

/*
  throw list for a function
*/
complete_throw_list[] { bool flag = false; ENTRY_DEBUG } :
        THROW paren_pair | THROWS ( options { greedy = true; } : complex_name_java[true, flag] | COMMA)*
;

/*
   type identifier

*/
pure_lead_type_identifier[] { ENTRY_DEBUG } :

        auto_keyword |

        // specifiers that occur in a type
        (standard_specifiers)=>
        standard_specifiers |

        pure_lead_type_identifier_no_specifiers


        /*
           Anonymous class/struct/union in guessing mode processes
           the entire block
        */

        // anonymous struct definition in a type (guessing)
//        { inputState->guessing }?
//        structures balanced_parentheses |

        /*
           Anonymous class/struct/union in non-guessing mode only
           starts the markup so that the contents of the block
           can be marked.
        */

        // anonymous struct definition in a type
//        { !inputState->guessing }?
//        struct_union_definition[SSTRUCT] |

        // anonymous class definition in a type
//        { !inputState->guessing }?
//        class_definition |

        // anonymous union definition in a type
//        { !inputState->guessing }?
//        struct_union_definition[SUNION] |
;

pure_lead_type_identifier_no_specifiers[] { ENTRY_DEBUG } :

        // class/struct/union before a name in a type, e.g., class A f();
        CLASS | STRUCT | UNION |

        // enum use in a type
        (ENUM variable_identifier (variable_identifier | multops | tripledotop | INLINE))=> ENUM |

        // entire enum definition
        enum_definition_whole
;

java_specifier_mark[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the function specifier
            startElement(SFUNCTION_SPECIFIER);
        }
        (PUBLIC | PRIVATE | PROTECTED | FINAL | STATIC | ABSTRACT | FRIEND | INTERNAL | SEALED | OVERRIDE | REF | OUT | IMPLICIT | EXPLICIT | UNSAFE | READONLY | VOLATILE | DELEGATE /* | { inLanguage(LANGUAGE_CSHARP) }? NEW */)
;

/*
   type identifier

*/
lead_type_identifier[] { ENTRY_DEBUG } :

//        java_specifier_mark |

//        (macro_call_paren identifier)=> macro_call |

        // typical type name
        complex_name[true] |

        pure_lead_type_identifier
;

type_identifier[] { ENTRY_DEBUG } :

        // any identifier that can appear first can appear later
        lead_type_identifier |

        non_lead_type_identifier
;

non_lead_type_identifier[] { bool iscomplex = false; ENTRY_DEBUG } :

        tripledotop |

        { inLanguage(LANGUAGE_C_FAMILY) }? multops |

        { inLanguage(LANGUAGE_JAVA_FAMILY) && look_past(LBRACKET) == RBRACKET }?
        variable_identifier_array_grammar_sub[iscomplex]
;

/*
  A set of balanced parentheses
*/
balanced_parentheses[] :
        LCURLY
        (balanced_parentheses | ~(LCURLY | RCURLY))*
        RCURLY
;

/*
   Name of a function
*/
function_identifier[] { ENTRY_DEBUG } :

        // typical name
        complex_name[true] |

        function_identifier_main |

        // function pointer identifier with name marked separately
        function_pointer_name_grammar
;

function_identifier_main[] { LocalMode lm(this); ENTRY_DEBUG } :
        // special cases for main
        {
            // end all started elements in this rule
            startNewMode(MODE_LOCAL);

            // start of the name element
            startElement(SNAME);
        }
        // main program
        MAIN
;

/*
  overloaded operator name
*/
overloaded_operator[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // end all started elements in this rule
            startNewMode(MODE_LOCAL);

            // start of the name element
            startElement(SNAME);
        }
        OPERATOR
        (
            // special case for 'operator()'
            { LA(1) == LPAREN }? LPAREN RPAREN |

            // general operator name case is anything from 'operator', operators, or names
            (options { greedy = true; } : ~(LPAREN))*
        )
;

variable_identifier_array_grammar_sub[bool& iscomplex] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_LOCAL);

            startElement(SINDEX);
        }
        LBRACKET

        full_expression

        RBRACKET
        {
            iscomplex = true;
        }
;

/*
  Full, complete expression matched all at once (no stream).
  Colon matches range(?) for bits.
*/
full_expression[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }
        (options { greedy = true; } :

        // commas as in a list
        comma |

        // right parentheses, unless we are in a pair of parentheses in an expression 
        { !inTransparentMode(MODE_INTERNAL_END_PAREN) }? rparen[false, false] |

        // argument mode (as part of call)
        { inMode(MODE_ARGUMENT) }? argument |

        // expression with right parentheses if a previous match is in one
        { LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN) }? expression |

        COLON)*
;

/*
   A variable name in an expression.  Includes array names, but not
   function calls
*/
variable_identifier[] { LocalMode lm(this); bool iscomplex = false; TokenPosition tp; ENTRY_DEBUG } :
        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            // outer name
            startElement(SONAME);

            // inner name that may be replaced by a NOP
            startElement(SCNAME);

            // record the token position so we can replace it if necessary
            tp = getTokenPosition();
        }
        (dcolon { iscomplex = true; })*

        simple_name_optional_template[true]

        name_tail[iscomplex, true]

        (options { greedy = true; } : variable_identifier_array_grammar_sub[iscomplex])*
        {
            // non-complex names need to be simplified
            if (!iscomplex)
                // set the token to NOP
                tp.setType(SNOP);
        }
;

/*
  Name including template argument list
*/
simple_name_optional_template[bool marked] { LocalMode lm(this); TokenPosition tp; ENTRY_DEBUG } :
        {
            if (marked) {
                // local mode that is automatically ended by leaving this function
                startNewMode(MODE_LOCAL);

                // start outer name
                startElement(SCNAME);

                // record the name token so we can replace it if necessary
                tp = getTokenPosition();
            }
        }
        mark_namestack identifier[marked] (
            { inLanguage(LANGUAGE_CXX_FAMILY) || inLanguage(LANGUAGE_JAVA_FAMILY) }?
            (template_argument_list)=>
                template_argument_list |

            {
               // if we marked it as a complex name and it isn't, fix
               if (marked)
                   // set the token to NOP
                   tp.setType(SNOP);
            }
       )
;

/*
  Basic single token names

  preprocessor tokens that can also be used as identifiers
*/
identifier[bool marked = false] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            if (marked) {
                // local mode that is automatically ended by leaving this function
                startNewMode(MODE_LOCAL);

                startElement(SNAME);
            }
        }
        (NAME | INCLUDE | DEFINE | ELIF | ENDIF | ERRORPREC | IFDEF | IFNDEF | LINE | PRAGMA | UNDEF |
            SUPER | CHECKED | UNCHECKED | REGION | ENDREGION)
;

/*
  identifier name marked with name element
*/
complex_name[bool marked] { LocalMode lm(this); TokenPosition tp; bool iscomplex_name = false; ENTRY_DEBUG } :
        {
            if (marked) {
                // There is a problem detecting complex names from
                // complex names of operator methods in namespaces or
                // classes for implicit casting, e.g., A::operator String // () {}.
                // Detecting before here means lookahead on all A::B::... names
                // causing a slowdown of almost 20%.  Solution (hack) is to start all complex
                // names as operator methods, then replace by NOP if not.

                // local mode that is automatically ended by leaving this function
                startNewMode(MODE_LOCAL);

                // start outer name
                startElement(SONAME);

                // start inner name
                startElement(SCNAME);

                // record the name token so we can replace it if necessary
                tp = getTokenPosition();
            }
        }
        (
        { inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP) }?
        complex_name_java[marked, iscomplex_name] |

        { inLanguage(LANGUAGE_C) }?
        complex_name_c[marked, iscomplex_name] |

        { !inLanguage(LANGUAGE_JAVA_FAMILY) && !inLanguage(LANGUAGE_C) }?
        complex_name_cpp[marked, iscomplex_name]
        )
        {
            // if we marked it as a complex name and it isn't, fix
            if (marked && !iscomplex_name)
                // set the token to NOP
                tp.setType(SNOP);
        }
;

/*
  identifier name marked with name element
*/
complex_name_cpp[bool marked, bool& iscomplex_name] { namestack[0] = ""; namestack[1] = ""; bool founddestop = false; ENTRY_DEBUG } :

        (DCOLON { iscomplex_name = true; })*
        (DESTOP set_bool[isdestructor] {
            founddestop = true;
        })*
        (simple_name_optional_template[marked] | mark_namestack overloaded_operator)
        name_tail[iscomplex_name, marked]
        { if (founddestop) iscomplex_name = true; }
;

/*
  Identifier markup for C
*/
complex_name_c[bool marked, bool& iscomplex_name] { ENTRY_DEBUG } :
        
        identifier[marked]
;

/*
  Identifier markup for Java
*/
complex_name_java[bool marked, bool& iscomplex_name] { ENTRY_DEBUG } :

        template_argument_list |
        simple_name_optional_template[marked]
        (options { greedy = true; } : (period { iscomplex_name = true; } simple_name_optional_template[marked]))*
;

/*
  sequences of "::" and names
*/
name_tail[bool& iscomplex, bool marked] { ENTRY_DEBUG } :

        // "a::" will cause an exception to be thrown
        ( options { greedy = true; } : 
            (dcolon { iscomplex = true; })
            ( options { greedy = true; } : dcolon)*
            (DESTOP set_bool[isdestructor])*
            (simple_name_optional_template[marked] | mark_namestack overloaded_operator | function_identifier_main)
        )*
;
exception
catch[antlr::RecognitionException] {
}

/* end of new identifiers */

/*
  Specifier for a function
*/
function_specifier[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the function specifier
            startElement(SFUNCTION_SPECIFIER);
        }
        (standard_specifiers |

        // pure virtual specifier
        EQUAL literal |

        simple_name_optional_template[false])
;

/*
  Specifiers for functions, methods, and variables
*/
standard_specifiers[] { LocalMode lm(this); ENTRY_DEBUG } :
        { inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP) }? 
            java_specifier_mark |

        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            startElement(SNAME);
        }
        (VIRTUAL | EXTERN | INLINE | EXPLICIT | STATIC | FRIEND)
;

auto_keyword[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            startElement(SNAME);
        }
        AUTO
;

// constructor definition
constructor_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the constructor declaration
            startElement(SCONSTRUCTOR_DECLARATION);
        }
        constructor_header
;              

// constructor definition
constructor_definition[] { ENTRY_DEBUG } :
        {
            // statement with nested block
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_STATEMENT);

            // start the construction definition
            startElement(SCONSTRUCTOR_DEFINITION);
        }
        constructor_header

        ({ inLanguage(LANGUAGE_CXX_FAMILY) }? try_statement)*

        ({ inLanguage(LANGUAGE_CXX_FAMILY) }? member_initialization_list)*
;

// constructor definition
constructor_header[] { ENTRY_DEBUG } :

        (options { greedy = true; } : 
            specifier_explicit |
            
            { inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP) }? java_specifier_mark |

            { inLanguage(LANGUAGE_JAVA_FAMILY) }? template_argument_list
        )*

        complex_name[true]

        parameter_list
        {
            setMode(MODE_FUNCTION_TAIL);
        }
;

// member initialization list of constructor
member_initialization_list[] { ENTRY_DEBUG } :
        {
            // handle member initialization list as a list of calls
            startNewMode(MODE_LIST | MODE_CALL);

            startElement(SMEMBER_INITIALIZATION_LIST);
        }
        COLON
;

mark_namestack[] { namestack[1] = namestack[0]; namestack[0] = LT(1)->getText(); } :;

identifier_stack[std::string s[]] { s[1] = s[0]; s[0] = LT(1)->getText(); ENTRY_DEBUG } :
        identifier[true]
;

specifier_explicit[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            startElement(SCLASS_SPECIFIER);
        }
        (EXPLICIT | INLINE | VIRTUAL)
;

// destructor definition
destructor_definition[] { ENTRY_DEBUG } :
        {
            // statement with nested block
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_STATEMENT);

            // start the destructor definition
            startElement(SDESTRUCTOR_DEFINITION);
        }
        destructor_header
;

// destructor declaration
destructor_declaration[] { ENTRY_DEBUG } :
        {
            // just a statement
            startNewMode(MODE_STATEMENT);

            // start the destructor declaration
            startElement(SDESTRUCTOR_DECLARATION);
        }
        destructor_header
;              


// destructor header
destructor_header[] { ENTRY_DEBUG } :

        (options { greedy = true; } : specifier_explicit | { LT(1)->getText() == "void" }? identifier[true])*

        complex_name[true]

        parameter_list
        {
            setMode(MODE_FUNCTION_TAIL);
        }
;              

/*
  call  function call, macro, etc.
*/
call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SFUNCTION_CALL);
        }
        function_identifier

        call_argument_list
;

/*
 Argument list for a call, e.g., to a function
*/
call_argument_list[] { ENTRY_DEBUG } :
        {
            // list of parameters
            setMode(MODE_EXPECT | MODE_LIST | MODE_INTERNAL_END_PAREN | MODE_END_ONLY_AT_RPAREN);

            // start the argument list
            startElement(SARGUMENT_LIST);
        }
        LPAREN
;

/*
  call

  function call, macro, etc.
*/

macro_call_check[] { ENTRY_DEBUG } :
        NAME optional_paren_pair
;

eat_optional_macro_call[] {

    bool success = false;

    // find out if we have a macro call
    int start = mark();
    inputState->guessing++;

    try {
        // check for the name
        match(NAME);

        // handle the parentheses
        paren_pair();

        success = true;

    } catch (...) {
    }

    inputState->guessing--;
    rewind(start);

    // when successfull, eat the macro
    if (success)
        macro_call();

    ENTRY_DEBUG
    } :;

macro_call[] { ENTRY_DEBUG } :
        macro_call_inner
        {
            if (inMode(MODE_THEN) && LA(1) == ELSE)
                endCurrentMode(MODE_THEN);
        }
    ;

macro_call_inner[] { LocalMode lm(this); bool first = true; ENTRY_DEBUG } :

        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);
        }
        identifier[true]
        (options { greedy = true; } : { first }?
        {
            // start a mode for the macro argument list
            startNewMode(MODE_LIST | MODE_TOP);

            // start the argument list
            startElement(SARGUMENT_LIST);
        }
        LPAREN

        macro_call_contents

        {
            // end anything started inside of the macro argument list
            endDownToMode(MODE_LIST | MODE_TOP);
        }
        RPAREN
        {
            // end the macro argument list
            endCurrentMode(MODE_LIST | MODE_TOP);
        } set_bool[first, false] )*
;
exception
catch[antlr::RecognitionException] {

        // no end found to macro
        if (isoption(parseoptions, OPTION_DEBUG))
            emptyElement(SERROR_PARSE);
}

macro_call_contents[] { 

            ENTRY_DEBUG

            LocalMode lm(this);

            int parencount = 0;
            bool start = true;
            while (LA(1) != 1 /* EOF? */ && !(parencount == 0 && LA(1) == RPAREN)) {

                if (LA(1) == LPAREN)
                    ++parencount;

                if (LA(1) == RPAREN)
                    --parencount;

                if (inputState->guessing == 0 && start) {
                       // argument with nested expression
                       startNewMode(MODE_ARGUMENT);

                       // start of the try statement
                       startElement(SARGUMENT);

                       start = false;
                }

                if (inputState->guessing == 0 && LA(1) == COMMA && parencount == 0) {
                    endCurrentMode();
                    start = true;
                }
                consume();
            }

        } :;

try_statement[] { ENTRY_DEBUG } :
        try_pseudo_statement
        TRY
        {            
            // looking for a LPAREN.  may have some whitespace before it
            consumeSkippedTokens();

            if (LA(1) == LPAREN) {
                match(LPAREN);

                // expect a parameter list
                startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);
            }
        }
;

try_pseudo_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TRY);

            // start of the try statement
            startElement(STRY_BLOCK);
        }
;

checked_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SCHECKED_STATEMENT);
        }
        CHECKED
;

unchecked_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SUNCHECKED_STATEMENT);
        }
        UNCHECKED
;

catch_statement[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_STATEMENT);

            // start of the catch statement
            startElement(SCATCH_BLOCK);
        }
        CATCH
        {            
            // looking for a LPAREN.  may have some whitespace before it
            consumeSkippedTokens();

            if (LA(1) == LPAREN) {
                match(LPAREN);

                // expect a parameter list
                startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);
            }
        }
;

finally_statement[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_STATEMENT);

            // start of the catch statement
            startElement(SFINALLY_BLOCK);
        }
        FINALLY
;

lock_statement[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_STATEMENT);

            // start of the catch statement
            startElement(SLOCK_STATEMENT);
        }
        LOCK
        {            
            // looking for a LPAREN.  may have some whitespace before it
            consumeSkippedTokens();

            if (LA(1) == LPAREN) {
                match(LPAREN);

                // expect a parameter list
                startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);
            }
        }
;

fixed_statement[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_STATEMENT);

            // start of the catch statement
            startElement(SFIXED_STATEMENT);
        }
        FIXED
        {            
            // looking for a LPAREN.  may have some whitespace before it
            consumeSkippedTokens();

            if (LA(1) == LPAREN) {
                match(LPAREN);

                // expect a parameter list
                startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);
            }
        }
;

throw_statement[] { ENTRY_DEBUG } :
        {
            // statement with expected expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start of throw statement
            startElement(STHROW_STATEMENT);
        }
        THROW
;

expression_statement_process[] { ENTRY_DEBUG } :
        {
            // statement with an embedded expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the element which will end after the terminate
            startElement(SEXPRESSION_STATEMENT);
        }
;

expression_statement[CALLTYPE type = NOCALL] { ENTRY_DEBUG } :

        expression_statement_process

        expression[type]
;

/*
  Statement for the declaration of a variable or group of variables
*/
variable_declaration_statement[int type_count] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            if (!inTransparentMode(MODE_INNER_DECL))
                // start the declaration statement
                startElement(SDECLARATION_STATEMENT);

            // declaration
            startNewMode(MODE_LOCAL);

            if (!inTransparentMode(MODE_INNER_DECL))
                // start the declaration
                startElement(SDECLARATION);
        }
        variable_declaration[type_count]
;

/*
  Statement for the declaration of a variable or group of variables
*/
short_variable_declaration[] { ENTRY_DEBUG } :
        {
            // declaration
            startNewMode(MODE_LOCAL);

            // start the declaration
            startElement(SDECLARATION);

            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);
        }
;

/*
  Declaration of a variable

  Example:
    int a;
    int a = b;
    int a, b;
    int a = b, c = d;
*/
variable_declaration[int type_count] { ENTRY_DEBUG } :
        {
            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);
        }
        variable_declaration_type[type_count]
;

/*
  A simple variable declaration of a single variable including the type,
  name, and initialization block.
*/
variable_declaration_type[int type_count] { ENTRY_DEBUG } :
        {
            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_EAT_VAR_TYPE);

            setTypeCount(type_count);

            // type element begins
            startElement(STYPE);
        }
        lead_type_identifier
        update_var_typecount
;

/*
  Variable declaration name and optional initialization
*/
variable_declaration_nameinit[] { ENTRY_DEBUG } :
        variable_identifier
        {
            // expect a possible initialization
            setMode(MODE_INIT | MODE_EXPECT);
        }
;

/*
  Initialization of a variable in a declaration.  Does not include the equal sign.
*/
function_pointer_initialization[] { ENTRY_DEBUG } :

        EQUAL
        {
            // end the init correctly
            setMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startNoSkipElement(SDECLARATION_INITIALIZATION);
        }
        (options { greedy = true; } : expression)*
;

variable_declaration_initialization[] { ENTRY_DEBUG } :

        EQUAL
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startNoSkipElement(SDECLARATION_INITIALIZATION);
        } |
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);
        }
        call_argument_list
;

variable_declaration_range[] { ENTRY_DEBUG } :

        COLON
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startNoSkipElement(SDECLARATION_RANGE);
        }
;

parameter_declaration_initialization[] { ENTRY_DEBUG } :

        EQUAL
        {
            // end the init correctly
            setMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startNoSkipElement(SDECLARATION_INITIALIZATION);
        }
;

pure_expression_block[] { ENTRY_DEBUG } :
        lcurly_base 
        {
            // nesting blocks, not statement
            replaceMode(MODE_NEST | MODE_STATEMENT, MODE_BLOCK | MODE_NEST | MODE_END_AT_BLOCK_NO_TERMINATE);

            // end this expression block correctly
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT);
        }
;

/*
  All possible operators
*/
general_operators[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SOPERATOR);
            }
        }
        (
        OPERATORS | TEMPOPS |
            TEMPOPE ({ SkipBufferSize() == 0 }? TEMPOPE)? ({ SkipBufferSize() == 0 }? TEMPOPE)? ({ SkipBufferSize() == 0 }? EQUAL)? |
    EQUAL | /*MULTIMM |*/ DESTOP | /* MEMBERPOINTER |*/ MULTOPS | REFOPS | DOTDOT | RVALUEREF | QMARK

/*            general_operators_list (options { greedy = true; } : { SkipBufferSize() == 0 }? general_operators_list)* */ |

            // others are not combined
            NEW | DELETE | IN | IS | STACKALLOC | AS
        )
;

sole_new[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SOPERATOR);
            }
        }
        NEW
;

sole_destop[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SOPERATOR);
            }
        }
        DESTOP
;

general_operators_list[] { ENTRY_DEBUG }:
        OPERATORS | TEMPOPS | TEMPOPE | EQUAL | /*MULTIMM |*/ DESTOP | /* MEMBERPOINTER |*/ MULTOPS | REFOPS | DOTDOT | RVALUEREF | QMARK
;

rparen_operator[bool markup = true] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            if (markup && isoption(parseoptions, OPTION_OPERATOR) && !inMode(MODE_END_ONLY_AT_RPAREN)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SOPERATOR);
            }
        }
        RPAREN
    ;

rparen[bool final = false, bool markup = true] { bool isempty = getParen() == 0; ENTRY_DEBUG } :
        {
            if (isempty) {

                // additional right parentheses indicates end of non-list modes
                endDownToFirstMode(MODE_LIST | MODE_PREPROC | MODE_END_ONLY_AT_RPAREN, MODE_ONLY_END_TERMINATE);

                // special case:  Get to here, in for-initalization.  Need an extra end mode
                if (inMode(MODE_VARIABLE_NAME) && inTransparentMode(MODE_FOR_CONDITION))
                    endDownToFirstMode(MODE_FOR_CONDITION);

                // don't markup since not a normal operator
                markup = false;

            } else

                decParen();
        }
        rparen_operator[markup]
        {
            if (isempty) {

                if (inMode(MODE_CONDITION) && inMode(MODE_IF_COND)) {

                    // end the condition
                    endDownOverMode(MODE_CONDITION);

                    // then part of the if statement (after the condition)
                    startNewMode(MODE_STATEMENT | MODE_NEST | MODE_THEN);

                    // start the then element
                    startNoSkipElement(STHEN);
                }

                // end the single mode that started the list
                // don't end more than one since they may be nested
                if (inMode(MODE_LIST))
                    endCurrentMode(MODE_LIST);
            }
        }
;

/*
  All possible operators
*/

/*
  Dot (period) operator
*/
period[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SOPERATOR);
            }
        }
        PERIOD
;

/*
  Namespace operator '::'
*/
dcolon[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SOPERATOR);
            }
        }
        DCOLON
;

/*
   An expression
*/
expression_process[] { ENTRY_DEBUG } : 
        {
            // if expecting an expression start one. except if you are at a right curly brace
            if (inMode(MODE_EXPRESSION | MODE_EXPECT) && LA(1) != RCURLY &&
                !(inMode(MODE_FOR_INCREMENT) && LA(1) == RPAREN)) {

                // use a new mode without the expect so we don't nest expression parts
                startNewMode(MODE_EXPRESSION);

                // start the expression element
                startElement(SEXPRESSION);
            }
        }
;

expression[CALLTYPE type = NOCALL] { ENTRY_DEBUG } : 

        expression_process

        expression_part[type]
;

guessing_startNewMode[State::MODE_TYPE mode]
    { if (inputState->guessing) startNewMode(mode | MODE_GUESSING); ENTRY_DEBUG } : ;

guessing_endDownToMode[State::MODE_TYPE mode]
    { if (inputState->guessing && inTransparentMode(MODE_GUESSING)) endDownToMode(mode | MODE_GUESSING); ENTRY_DEBUG } : ;

guessing_endCurrentModeSafely[State::MODE_TYPE mode]
    { if (inputState->guessing && inTransparentMode(MODE_GUESSING)) endCurrentModeSafely(mode | MODE_GUESSING); ENTRY_DEBUG } : ;

guessing_endGuessing[]
    { if (inTransparentMode(MODE_GUESSING)) endDownOverMode(MODE_GUESSING); ENTRY_DEBUG } : ;

guessing_end[]
    { if (!inputState->guessing && inTransparentMode(MODE_GUESSING)) endDownOverMode(MODE_GUESSING); ENTRY_DEBUG } : ;


/*
   Occurs only within another expression.  The mode is MODE_EXPRESSION.  Only
   elements such as names and function calls are marked up.
*/
expression_part[CALLTYPE type = NOCALL] { guessing_end(); bool flag; ENTRY_DEBUG } :

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW template_argument_list)=> sole_new template_argument_list |
        
        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW function_identifier paren_pair LCURLY)=> sole_new anonymous_class_definition |

        // call
        // distinguish between a call and a macro
        { type == CALL || (perform_call_check(type, -1) && type == CALL) }?

            call

            guessing_startNewMode[MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN] |

        // macro call
        { type == MACRO }? macro_call |

        // general math operators
        general_operators | /* newop | */ period |

        // left parentheses
        lparen_marked
        guessing_startNewMode[MODE_INTERNAL_END_PAREN]
        {
            startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);

        } |

        // right parentheses that only matches a left parentheses of an expression
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        {
            // stop at this matching paren, or a preprocessor statement
            endDownToFirstMode(MODE_INTERNAL_END_PAREN | MODE_PREPROC); 

            endCurrentModeSafely(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN); 
        }
        guessing_endDownToMode[MODE_INTERNAL_END_PAREN]

        guessing_endCurrentModeSafely[MODE_INTERNAL_END_PAREN]

        // treat as operator for operator markup
        rparen[false] |

        // left curly brace
        {
            startNewMode(MODE_EXPRESSION | MODE_LIST);

            startElement(SBLOCK);
        }
        LCURLY
        {
            startNewMode(MODE_EXPRESSION | MODE_EXPECT | MODE_LIST | MODE_INTERNAL_END_CURLY);
        } |

        // right curly brace
        { inTransparentMode(MODE_INTERNAL_END_CURLY) }?
        { 
            endDownToMode(MODE_INTERNAL_END_CURLY);

            endCurrentModeSafely(MODE_INTERNAL_END_CURLY); 
        }
        RCURLY
        { 
            endCurrentModeSafely(MODE_EXPRESSION | MODE_LIST); 
        } |

        // variable or literal
        variable_identifier | string_literal | char_literal | literal | boolean |

        variable_identifier_array_grammar_sub[flag]
;

/*
  Only start and end of strings are put directly through the parser.
  The contents of the string are handled as is whitespace.
*/
string_literal[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // only markup strings in literal option
            if (isoption(parseoptions, OPTION_LITERAL)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the string
                startElement(SSTRING);
            }
        }
        (STRING_START STRING_END)
;

/*
  Only start and end of character are put directly through the parser.
  The contents of the character are handled as is whitespace.
*/
char_literal[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // only markup characters in literal option
            if (isoption(parseoptions, OPTION_LITERAL)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the character
                startElement(SCHAR);
            }
        }
        (CHAR_START CHAR_END)
;

literal[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // only markup literals in literal option
            if (isoption(parseoptions, OPTION_LITERAL)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the literal value
                startElement(SLITERAL);
            }
        }
        CONSTANTS
;

boolean[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // only markup boolean values in literal option
            if (isoption(parseoptions, OPTION_LITERAL)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the literal value
                startElement(SBOOLEAN);
            }
        }
        (TRUE | FALSE)
;

derived[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SDERIVATION_LIST);
        }
        COLON
        (options { greedy = true; } :
            (derive_access)*

            variable_identifier

            (template_argument_list)*
        |
            COMMA
        )*
;

super_list_java[] { ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SDERIVATION_LIST);
        }
;

extends_list[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SEXTENDS);
        }
        EXTENDS
        super_list
;

implements_list[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SIMPLEMENTS);
        }
        IMPLEMENTS
        super_list
;

super_list[] { bool flag = false; ENTRY_DEBUG } :
        (options { greedy = true; } :
            (derive_access)*

            complex_name_java[true, flag]
        |
            COMMA
        )*
;

derive_access[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            startElement(SCLASS_SPECIFIER);
        }
        (VIRTUAL)* (PUBLIC | PRIVATE | PROTECTED) (options { greedy = true; } : VIRTUAL)*
;

parameter_list[] { LocalMode lm(this); bool lastwasparam = false; bool foundparam = false; ENTRY_DEBUG } :
        {
            // list of parameters
            startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);

            // start the parameter list element
            startElement(SPARAMETER_LIST);
        }
        // parameter list must include all possible parts since it is part of
        // function detection
        LPAREN ({ foundparam = true; if (!lastwasparam) empty_element(SPARAMETER, !lastwasparam); lastwasparam = false; } 
        {
            // We are in a parameter list.  Need to make sure we end it down to the start of the parameter list
            if (!inMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT))
                endCurrentMode(MODE_LOCAL);
        } comma |
        full_parameter { foundparam = lastwasparam = true; })* empty_element[SPARAMETER, !lastwasparam && foundparam] rparen[false, false]
;

empty_element[int element, bool cond] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            if (cond) {
                startNewMode(MODE_LOCAL);

                startElement(element);
            }
        }
;

kr_parameter[] { ENTRY_DEBUG } : 
        full_parameter terminate_pre terminate_token
;

full_parameter[] { ENTRY_DEBUG } :

        parameter
        (options { greedy = true; } : parameter_declaration_initialization expression)*
;

argument[] { ENTRY_DEBUG } :
        { getParen() == 0 }? rparen[false,false] |
        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        } 
        (
        { !(LA(1) == RPAREN && inTransparentMode(MODE_INTERNAL_END_PAREN)) }? expression |

        type_identifier
        )
;

/*
  Parameter for a function declaration or definition
*/                
parameter[] { int type_count = 0; int secondtoken = 0; int fla = 0; DECLTYPE decl_type = NONE; ENTRY_DEBUG } :
        {
            // end parameter correctly
            startNewMode(MODE_PARAMETER);

            // start the parameter element
            startElement(SPARAMETER);
        }
        (
        { perform_noncfg_check(decl_type, secondtoken, fla, type_count, true) && decl_type == FUNCTION }?
        function[TERMINATE, type_count]

        function_identifier // pointer_name_grammar

        (macro_call_check)*

        parameter_list 

        (options { greedy = true; } : function_pointer_initialization)* |
        {
            // start the declaration element
            startElement(SDECLARATION);

            if (decl_type != VARIABLE)
                type_count = 1;
        }
        { decl_type == VARIABLE || LA(1) == DOTDOTDOT}?
        parameter_type_count[type_count]
        {
            consumeSkippedTokens();

            // expect a name initialization
            setMode(MODE_VARIABLE_NAME | MODE_INIT);
        }
        ( options { greedy = true; } : variable_declaration_nameinit)*

        )
;

/*
*/
parameter_type_count[int type_count] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);

            // start of type
            startElement(STYPE);
        }
        eat_type[type_count]
        {
            consumeSkippedTokens();
        }

        // sometimes there is no parameter name.  if so, we need to eat it
        ( options { greedy = true; } : multops | tripledotop | LBRACKET RBRACKET)*
;

multops[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (isoption(parseoptions, OPTION_MODIFIER)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SMODIFIER);
            }
        }
        (MULTOPS | REFOPS | RVALUEREF)
;

tripledotop[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (isoption(parseoptions, OPTION_MODIFIER)) {

                // end all elements at end of rule automatically
                startNewMode(MODE_LOCAL);

                // start the modifier
                startElement(SMODIFIER);
            }
        }
        DOTDOTDOT
;

/*
*/
parameter_type[] { LocalMode lm(this); int type_count = 0; int fla = 0; int secondtoken = 0; DECLTYPE decl_type = NONE; ENTRY_DEBUG } :
        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);

            // start of type
            startElement(STYPE);
        }
        { perform_noncfg_check(decl_type, secondtoken, fla, type_count) }?
        eat_type[type_count]
;

/*
  Template
*/

/*
  template declaration
*/
template_declaration[] { ENTRY_DEBUG } :
        {
            // template with nested statement (function or class)
            // expect a template parameter list
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TEMPLATE);

            // start the template
            startElement(STEMPLATE);
        }
        TEMPLATE 
        {
            startNewMode(MODE_TEMPLATE | MODE_LIST | MODE_EXPECT | MODE_TEMPLATE_PARAMETER_LIST);
        }
;

/*
  template parameter list
*/
template_param_list[] { ENTRY_DEBUG } :
        {
            // start the template parameter list
            startElement(STEMPLATE_PARAMETER_LIST);
        }
        tempops
;

/*
requires_clause[] { ENTRY_DEBUG } :
        {
            // template with nested statement (function or class)
            // expect a template parameter list
            startNewMode(MODE_STATEMENT | MODE_NEST);

            startElement(SREQUIRES);
        }
        REQUIRES 
        {
            startNewMode(MODE_LOCAL);

            startElement(SCLAUSE);
        }
        (general_operators)* complex_name[true] (general_operators (general_operators)* complex_name[true])* 
        {
            endCurrentMode(MODE_LOCAL);
        }
        (TERMINATE { endCurrentMode(MODE_STATEMENT | MODE_NEST); } )*
;
*/

/*
  template parameter

  A template parameter is a subset of a general function parameter
*/
template_param[] { ENTRY_DEBUG } :
        {
            // end parameter correctly
            startNewMode(MODE_PARAMETER);

            // start the parameter element
            startElement(STEMPLATE_PARAMETER);
        }
        (
        parameter_type
        {
            consumeSkippedTokens();
        }
        {
            // expect a name initialization
            setMode(MODE_VARIABLE_NAME | MODE_INIT);
        } |
        template_declaration
    )
;

/*
  template argument list
*/
template_argument_list[] { LocalMode lm(this); std::string namestack_save[2]; ENTRY_DEBUG } : 
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(STEMPLATE_ARGUMENT_LIST);
        }
        savenamestack[namestack_save]
        tempops (COMMA | template_argument)* tempope
        restorenamestack[namestack_save]
;

savenamestack[std::string namestack_save[]] { namestack_save[0] = namestack[0]; namestack_save[1] = namestack[1]; ENTRY_DEBUG } :;

restorenamestack[std::string namestack_save[]] { namestack[0] = namestack_save[0]; namestack[1] = namestack_save[1]; ENTRY_DEBUG } :;

/*
  template argument
*/
template_argument[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(STEMPLATE_ARGUMENT);
        }
        ( options { greedy = true; } : 
            { LA(1) != SUPER }?
            type_identifier |

            literal | char_literal | string_literal | boolean | 

            template_extends_java |

            template_super_java | QMARK
        )+
;

template_extends_java[] { LocalMode lm(this); bool iscomplex = false; ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SEXTENDS);
        }
        EXTENDS
        complex_name_java[true, iscomplex]
;


template_super_java[] { LocalMode lm(this); bool iscomplex = false; ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SDERIVATION_LIST);
        }
        SUPER
        complex_name_java[true, iscomplex]
;


tempops[] { ENTRY_DEBUG } :
        {
            // make sure we are in a list mode so that we can end correctly
            // some uses of tempope will have their own mode
            if (!inMode(MODE_LIST))
                startNewMode(MODE_LIST);
        }
        TEMPOPS
;

tempope[bool final = false] { if (final) setFinalToken(); ENTRY_DEBUG } :
        {
            // end down to the mode created by the start template operator
            endDownToMode(MODE_LIST);
        }
        TEMPOPE
        {
            // end the mode created by the start template operator
            endCurrentModeSafely(MODE_LIST);

            setMode(MODE_REQUIRES);
        }
;

/*
  label statement
*/
label_statement[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the label element
            startElement(SLABEL_STATEMENT);
        } 
        identifier[true] COLON
;

/*
  typedef_statement
*/
typedef_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_EXPECT | MODE_VARIABLE_NAME | MODE_ONLY_END_TERMINATE);

            // start the typedef element
            startElement(STYPEDEF);
        }
        TYPEDEF
        {
            if (LA(1) == CLASS || LA(1) == UNION || LA(1) == STRUCT) {

                // end all elements started in this rule
                startNewMode(MODE_LOCAL | MODE_TYPEDEF | MODE_END_AT_BLOCK_NO_TERMINATE);

                // start of the type
                startElement(STYPE);
            } else
                // statement
                startNewMode(MODE_NEST | MODE_STATEMENT | MODE_INNER_DECL);
        }
        (
        /* Never going to see a Java specifier before a class in C++, and never going
           to see a TYPEDEF in a Java program, but needed for grammar ambiguity */
        (java_specifier_mark | CLASS)=>
        class_definition |

        struct_union_definition[LA(1) == STRUCT ? SSTRUCT : SUNION])*
;

paren_pair[] :
        LPAREN (paren_pair | ~(LPAREN | RPAREN))* RPAREN
;

optional_paren_pair[] {

    if (LA(1) != LPAREN)
        return;

    consume();

    int parencount = 1;
    while (parencount > 0 && LA(1) != antlr::Token::EOF_TYPE) {

        if (LA(1) == RPAREN)
            --parencount;
        else if (LA(1) == LPAREN)
            ++parencount;

        consume();
    }

    ENTRY_DEBUG
} :;

/*
  See if there is a semicolon terminating a statement inside a block at the top level
*/        
nested_terminate[] {

    int parencount = 0;
    int bracecount = 0;
    while (LA(1) != antlr::Token::EOF_TYPE) {

        if (LA(1) == RPAREN)
            --parencount;
        else if (LA(1) == LPAREN)
            ++parencount;

        if (LA(1) == RCURLY)
            --bracecount;
        else if (LA(1) == LCURLY)
            ++bracecount;

        if (bracecount < 0)
            break;

        if (LA(1) == TERMINATE && parencount == 0 && bracecount == 0)
            break;

        consume();
    }
}:
        TERMINATE
;
        
/*
  Definition of an enum.  Start of the enum only
*/
enum_definition[] { ENTRY_DEBUG } :
        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (enum_class_definition nested_terminate)=> enum_class_definition |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        {
            // statement
            // end init correctly
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION_BLOCK | MODE_VARIABLE_NAME | MODE_EXPECT);

            // start the enum definition element
            startElement(SENUM);
        }
        (java_specifier_mark)*
        ENUM |
        {
            // statement
            // end init correctly
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION_BLOCK | MODE_VARIABLE_NAME | MODE_EXPECT);

            // start the enum definition element
            startElement(SENUM);
        }
        ENUM
;

/*
  Complete definition of an enum.  Used for enum's embedded in typedef's where the entire
  enum must be parsed since it is part of the type.
*/
enum_definition_whole[] { LocalMode lm(this); ENTRY_DEBUG } :
        enum_definition

        (variable_identifier)*

        // start of enum definition block

        {
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_BLOCK | MODE_NEST);

            startElement(SBLOCK);
        }
        LCURLY

        (options { greedy = true; } : { LA(1) != RCURLY || inTransparentMode(MODE_INTERNAL_END_CURLY) }?
        expression | comma)*

        // end of enum definition block
        {
            endDownToMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_BLOCK | MODE_NEST);
        }
        RCURLY
;

/*
  end of file

  Reached the end of the input.  Must now make sure to end any open elements.  Open elements indicate
  either syntax error in the code, or a translation error.

  EOF marks the end of all processing, so it must occur after any ending modes
*/
eof[] :
        {
            // end all modes
            endAllModes();
        }
        EOF
;

/*
    Preprocessor

    Match on the directive itself not the entire directive
*/
preprocessor[] {
        int directive_token = 0;
        bool markblockzero = false;

        TokenPosition tp;

        // parse end of line
        startNewMode(MODE_PARSE_EOL);

        // mode for any preprocessor elements
        startNewMode(MODE_PREPROC);
        } :
        {
            // assume error.  will set to proper one later
            startElement(SCPP_ERROR);

            tp = getTokenPosition();
        }
        PREPROC markend[directive_token]
        {
            startNewMode(MODE_LOCAL);

            startElement(SCPP_DIRECTIVE);
        }
        (
        INCLUDE
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_INCLUDE);
        }
        cpp_filename |

        DEFINE
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_DEFINE);
        }
        cpp_symbol_optional |

        IFNDEF
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_IFNDEF);
        }
        cpp_symbol_optional |

        UNDEF
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_UNDEF);
        }
        cpp_symbol_optional |

        IF
            { markblockzero = false; }
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_IF);
        }
        cpp_condition[markblockzero] |

        ELIF
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_ELIF);
        }
        cpp_condition[markblockzero] |

        ELSE
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_ELSE);
        } |

        ENDIF
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_ENDIF);
        } |

        IFDEF
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_IFDEF);
        }
            cpp_symbol_optional |

        LINE
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_LINE);
        }
            cpp_linenumber

            cpp_filename |

        PRAGMA
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_PRAGMA);
        } |

        ERRORPREC
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_ERROR);
        } |

        NAME
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_ERROR);
        } |

        REGION
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_REGION);
        } |

        ENDREGION
        {
            endCurrentMode(MODE_LOCAL);

            tp.setType(SCPP_ENDREGION);
        } |

        /* blank preproc */

        /* skip over anything, start with stuff defined before */
        (~(NAME | ERRORPREC | INCLUDE | DEFINE | IF | ENDIF | IFNDEF | UNDEF | ELIF | ELSE | IFDEF | LINE | PRAGMA | EOL | LINECOMMENT_START | COMMENT_START | JAVADOC_COMMENT_START | REGION | ENDREGION))?

        )
        eol_skip[directive_token, markblockzero]
;
exception
catch[antlr::RecognitionException] {
        eol_skip(directive_token, markblockzero);
}

eol_skip[int directive_token, bool markblockzero] { 

    while (LA(1) != EOL && 
           LA(1) != LINECOMMENT_START && 
           LA(1) != COMMENT_START && 
           LA(1) != JAVADOC_COMMENT_START && 
           LA(1) != 1 /* EOF? */
        )
                consume();
    ENTRY_DEBUG } :
    eol[directive_token, markblockzero]
;

/*
  end of line

  Only used for ending preprocessor, and only those directives who end on the current
  line.
*/
eol[int directive_token, bool markblockzero] {
            // end all preprocessor modes
            endDownToMode(MODE_PREPROC);

            endCurrentMode(MODE_PREPROC);

            endCurrentMode(MODE_PARSE_EOL);
ENTRY_DEBUG } :
        (EOL | LINECOMMENT_START | eof)
        eol_post[directive_token, markblockzero]
;

eol_post[int directive_token, bool markblockzero] {
            // Flags to control skipping of #if 0 and #else.
            // Once in these modes, stay in these modes until the matching #endif is reached
            // cppifcount used to indicate which #endif matches the #if or #else
            switch (directive_token) {

                case IF :
                case IFDEF :
                case IFNDEF :

                    // start a new blank mode for new zero'ed blocks
                    if (!zeromode && markblockzero) {

                        // start a new blank mode for if
                        zeromode = true;

                        // keep track of nested if's (inside the #if 0) so we know when
                        // we reach the proper #endif
                        cppifcount = 0;
                    }

                    // another if reached
                    ++cppifcount;

                    // create new context for #if (and possible #else)
                    if (checkOption(OPTION_CPP_MARKUP_ELSE) && !inputState->guessing) {

                        cppmode.push(cppmodeitem(size()));
                    }

                    break;

                case ELSE :
                case ELIF :

                    // #else reached for #if 0 that started this mode
                    if (zeromode && cppifcount == 1)
                        zeromode = false;

                    // not in skipped #if, so skip #else until #endif of #if is reached
                    if (!zeromode) {
                        skipelse = true;
                        cppifcount = 1;
                    }

                    if (!checkOption(OPTION_CPP_MARKUP_ELSE) && !inputState->guessing) {

                        // create an empty cppmode for #if if one doesn't exist
                        if (cppmode.empty())
                            cppmode.push(cppmodeitem(size()));

                        // add new context for #else in current #if
                        cppmode.top().statesize.push_back(size()); 
                    
                        if (!zeromode) {
                            if (cppmode.top().statesize.front() > size())
                                cppmode.top().skipelse = true;
                        }
                    }

                    break;

                case ENDIF :

                    // another #if ended
                    --cppifcount;

                    // #endif reached for #if 0 that started this mode
                    if (zeromode && cppifcount == 0)
                        zeromode = false;

                    // #endif reached for #else that started this mode
                    if (skipelse && cppifcount == 0)
                        skipelse = false;

                    if (!checkOption(OPTION_CPP_MARKUP_ELSE) && !inputState->guessing &&
                        !cppmode.empty()) {

                        // add new context for #endif in current #if
                        cppmode.top().statesize.push_back(size()); 

                        // reached #endif so finished adding to this mode
                        cppmode.top().isclosed = true;

                        // remove any finished modes
                        cppmode_cleanup();
                    }

                default :
                    break;
            }

        /*
            Skip elements when:
                - in zero block (zeromode) and not marking #if 0
                - when processing only #if part, not #else
                - when guessing and in else (unless in zero block)
                - when ??? for cppmode
        */
        if ((!checkOption(OPTION_CPP_MARKUP_IF0) && zeromode) ||
            (!checkOption(OPTION_CPP_MARKUP_ELSE) && skipelse) ||
            (inputState->guessing && skipelse) ||
            (!cppmode.empty() && !cppmode.top().isclosed && cppmode.top().skipelse)
        ) {
            while (LA(1) != PREPROC && LA(1) != 1 /* EOF */)
                consume();
        }

        ENTRY_DEBUG } :
;

// remove any finished or unneeded cppmodes
cppmode_cleanup[] {

        bool equal = true;
        for (unsigned int i = 0; i < cppmode.top().statesize.size(); ++i) {
            if (cppmode.top().statesize[i] != cppmode.top().statesize[0])
                equal = false;
            }

            if (!cppmode.empty() && (equal || cppmode.top().statesize.size() == 2)) {
                cppmode.pop();
            }
        ENTRY_DEBUG } :
;

// ended modes that may lead to needed updates
cppmode_adjust[] {

    if (checkOption(OPTION_CPP_MARKUP_ELSE) && !cppmode.empty() && 
        cppmode.top().isclosed == true &&
        size() < cppmode.top().statesize.back()) {

           if (size() == cppmode.top().statesize[cppmode.top().statesize.size() - 1 - 1]) {
                
                // end if part of cppmode
                while (size() > cppmode.top().statesize.front())
                    endCurrentMode();

                // done with this cppmode
                cppmode.pop();
           }
    }

    ENTRY_DEBUG } :
;

line_continuation[] { setFinalToken(); ENTRY_DEBUG } :
        {
            // end all preprocessor modes
            endDownOverMode(MODE_PARSE_EOL);
        }
        EOL_BACKSLASH
;

cpp_condition[bool& markblockzero] { LocalMode lm(this); ENTRY_DEBUG } :

        set_bool[markblockzero, LA(1) == CONSTANTS && LT(1)->getText() == "0"]

        full_expression
;

cpp_symbol[] { LocalMode lm(this); ENTRY_DEBUG } :
        {
            // end all started elements in this rule
            startNewMode(MODE_LOCAL);

            // start of the name element
            startElement(SNAME);
        }
        NAME
;

cpp_symbol_optional[] { ENTRY_DEBUG } :
        (options { greedy = true; } : cpp_symbol)*
;

cpp_filename[] { LocalMode lm(this); ENTRY_DEBUG } :
        (
        {
            startNewMode(MODE_PREPROC);

            startElement(SCPP_FILENAME);
        }
        (string_literal | char_literal | TEMPOPS (~(TEMPOPE))* TEMPOPE))*
;

cpp_linenumber[] :
        (options { greedy = true; } : literal)*
;
