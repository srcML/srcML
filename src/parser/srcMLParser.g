/*
  srcMLParser.g

  Copyright (C) 2004-2013  SDML (www.sdml.info)

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

#include <string>
#include <deque>
#include <stack>
#include "Mode.hpp"
#include "Options.hpp"

// Macros to introduce trace statements
#define ENTRY_DEBUG //RuleDepth rd(this); fprintf(stderr, "TRACE: %d %d %d %5s%*s %s (%d)\n", inputState->guessing, LA(1), ruledepth, (LA(1) != 20 ? LT(1)->getText().c_str() : "\\n"), ruledepth, "", __FUNCTION__, __LINE__);
#ifdef ENTRY_DEBUG
#define ENTRY_DEBUG_INIT ruledepth(0),
#define ENTRY_DEBUG_START ruledepth = 0;
#endif

#define CATCH_DEBUG //marker();

#define assertMode(m)

enum STMT_TYPE { NONE, VARIABLE, FUNCTION, FUNCTION_DECL, CONSTRUCTOR, CONSTRUCTOR_DECL, DESTRUCTOR, DESTRUCTOR_DECL, SINGLE_MACRO, NULLOPERATOR, DELEGATE_FUNCTION, ENUM_DECL, GLOBAL_ATTRIBUTE, PROPERTY_ACCESSOR, PROPERTY_ACCESSOR_DECL, EXPRESSION, CLASS_DEFN, CLASS_DECL, UNION_DEFN, UNION_DECL, STRUCT_DEFN, STRUCT_DECL, INTERFACE_DEFN, INTERFACE_DECL, ACCESS_REGION };
enum CALLTYPE { NOCALL, CALL, MACRO };

// position in output stream
struct TokenPosition {

    TokenPosition() 
        : token(0), sp(0) {}

    // sets a particular token in the output token stream
    void setType(int type) {

        // set the inner name token to type
        (*token)->setType(type);

        // set this position in the element stack to type
        *sp = type;
    }

    ~TokenPosition() {}

    antlr::RefToken* token;
    int* sp;
};

}

// Included in the generated srcMLParser.cpp file after antlr includes
header "post_include_cpp" {

// Makes sure that a grammar rule forms a complete element
// Ends all modes started in the rule and closes any elements started in the rule.
class CompleteElement {
public:
    CompleteElement(srcMLParser* parent) : parent(parent) {

        // only run if not guessing
        if (parent->inputState->guessing) return;

        start_size = parent->size();
    }

    ~CompleteElement() {

        // only run if not guessing
        if (parent->inputState->guessing) return;

        // Close all the modes opened in the current rule with their elements.
        int n = parent->size() - start_size;
        for (int i = 0; i < n; ++i) {
            parent->endMode();
        }
    }

private:
    srcMLParser* parent;
    int start_size;
};

// Makes sure that a grammar rule forms a complete element
// Closes all elements opened in the rule for this current mode.
class LightweightElement {
public:
    LightweightElement(srcMLParser* parent) : parent(parent) {

        // only run if not guessing
        if (parent->inputState->guessing) return;

        start_size = parent->statev.currentState().size();
    }

    ~LightweightElement() {

        // only run if not guessing
        if (parent->inputState->guessing) return;

        // Close all elements opened by the rule in this mode.
        while (start_size < parent->statev.currentState().size())
            parent->endElement(parent->statev.currentState().openelements.top());
    }

private:
    srcMLParser* parent;
    int start_size;
};

// Makes sure that a grammar rule forms a complete element
// Ends the last opened tag.
class SingleElement {
public:
    SingleElement(srcMLParser* parent) : parent(parent) {}

    ~SingleElement() {

        // only run if not guessing
        if (parent->inputState->guessing) return;

        // end last opened element.
        parent->endElement(parent->statev.currentState().openelements.top());
    }

private:
    srcMLParser* parent;
};

#ifdef ENTRY_DEBUG
class RuleDepth {

public:
    RuleDepth(srcMLParser* t) : pparser(t) { ++pparser->ruledepth; }

    ~RuleDepth() { --pparser->ruledepth; }

private:
    srcMLParser* pparser;
};
#endif

bool srcMLParser::BOOL;

// constructor
srcMLParser::srcMLParser(antlr::TokenStream& lexer, int lang, int parser_options)
   : antlr::LLkParser(lexer,1), Mode(this, lang), cpp_zeromode(false), cpp_skipelse(false), cpp_ifcount(0),
    parseoptions(parser_options), ifcount(0), ENTRY_DEBUG_INIT notdestructor(false)
{
    // make sure we have the correct token set
    if (!_tokenSet_1.member(IF))
        fprintf(stderr, "src2srcml:  Incorrect token set A\n");

    if (!_tokenSet_13.member(INCLUDE))
        fprintf(stderr, "src2srcml:  Incorrect token set B\n");

    if (!_tokenSet_23.member(CLASS))
        fprintf(stderr, "src2srcml:  Incorrect token set C\n");

    // root, single mode
    if (isoption(parseoptions, OPTION_EXPRESSION))
        // root, single mode to allows for an expression without a statement
        startNewMode(MODE_TOP | MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);
    else
       // root, single mode that allows statements to be nested
       startNewMode(MODE_TOP | MODE_STATEMENT | MODE_NEST);
}

// ends all currently open modes
void srcMLParser::endAllModes() {

     // expression mode has an extra mode
     if (isoption(parseoptions, OPTION_EXPRESSION))
         endMode();

     // should only be one mode
     if (size() > 1 && isoption(parseoptions, OPTION_DEBUG))
         emptyElement(SERROR_MODE);

     // end all modes except the last
     while (size() > 1)
         endMode();

     // flush any skipped characters
     flushSkip();

     // end the very last mode which forms the entire unit
     if (size() == 1)
         endLastMode();
}

} /* end include */

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class srcMLParser extends Parser;

options {
    classHeaderSuffix="public Mode";
	k=1;
    importVocab=KeywordLexer;
    defaultErrorHandler=false;
    noConstructors=true;

    // values arrived at through experimentation
    codeGenBitsetTestThreshold=4;
    codeGenMakeSwitchThreshold=5;
}

tokens {
    // entire source file
    SUNIT;

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
	SENUM;

	SIF_STATEMENT;
	STHEN;
	SELSE;

    SWHILE_STATEMENT;
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

    // C Family elements
	STYPEDEF;
	SASM;
	SMACRO_CALL;
	SSIZEOF_CALL;
    SEXTERN;
	SNAMESPACE;
	SUSING_DIRECTIVE;

    // C++
    SALIGNAS;
    SDECLTYPE;
    SLAMBDA_CAPTURE;
    SNOEXCEPT;
	SSIGNAL_ACCESS;

    // cpp directive internal elements
	SCPP_DIRECTIVE;
    SCPP_FILENAME;
    SCPP_NUMBER;

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

    // C# cpp directives
    SCPP_REGION;
    SCPP_ENDREGION;

    // This HAS to mark the end of the CPP directives
	SCPP_ENDIF;

    // Debug elements
    SMARKER;
    SERROR_PARSE;
    SERROR_MODE;

    // Java elements
    SIMPLEMENTS;
    SEXTENDS;
    SIMPORT;
    SPACKAGE;
    SINTERFACE;
    SSYNCHRONIZED_STATEMENT;

    // C#
    SCHECKED_STATEMENT;
    SUNCHECKED_STATEMENT;
    SATTRIBUTE;
    STARGET;
    SUNSAFE_STATEMENT;
    SLOCK_STATEMENT;
    SFIXED_STATEMENT;

    // linq
    SLINQ;
    SFROM;
    SWHERE;
    SSELECT;
    SLET;
    SORDERBY;
    SJOIN;
    SGROUP;
    SIN;
    SON;
    SEQUALS;
    SBY;
    SINTO;

    // misc
    SEMPTY;  // empty statement

    SANNOTATION;

    // Last token used for boundary
    END_ELEMENT_TOKEN;
}

/*
  Included inside of generated class srcMLParser.hpp
*/
{
public:
    friend class CompleteElement;
    friend class LightweightElement;
    friend class SingleElement;

    bool cpp_zeromode;
    bool cpp_skipelse;
    int cpp_ifcount;
    bool isdestructor;
    int parseoptions;
    std::string namestack[2];
    int ifcount;
#ifdef ENTRY_DEBUG
    int ruledepth;
#endif
    bool qmark;
    bool notdestructor;
    bool operatorname;

    struct cppmodeitem {
        cppmodeitem(int current_size)
            : statesize(1, current_size), isclosed(false), skipelse(false) {}

        cppmodeitem() {}

        std::deque<int> statesize;
        bool isclosed;
        bool skipelse;
    };
    std::stack<cppmodeitem> cppmode;


    static bool BOOL;

    // constructor
    srcMLParser(antlr::TokenStream& lexer, int lang = LANGUAGE_CXX, int options = 0);

    // destructor
    ~srcMLParser() {}

    void startUnit() {
        startElement(SUNIT);
        emptyElement(SUNIT);
    }

    // sets to the current token in the output token stream
    void setTokenPosition(TokenPosition& tp) {
        tp.token = CurrentToken();
        tp.sp = &(currentState().openelements.top());
    }

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
start[] { ENTRY_DEBUG_START ENTRY_DEBUG } :

        // end of file
        eof |

        // end of line
        line_continuation | EOL | LINECOMMENT_START |

        comma | { inLanguage(LANGUAGE_JAVA) }? bar |

        { !inTransparentMode(MODE_INTERNAL_END_PAREN) || inPrevMode(MODE_CONDITION) }? rparen[false] |

        // characters with special actions that usually end currently open elements
        { !inTransparentMode(MODE_INTERNAL_END_CURLY) }? block_end |

        terminate |

        // don't confuse with expression block
        { ((inTransparentMode(MODE_CONDITION) ||
            (!inMode(MODE_EXPRESSION) && !inMode(MODE_EXPRESSION_BLOCK | MODE_EXPECT))) 
        && !inTransparentMode(MODE_CALL | MODE_INTERNAL_END_PAREN)
        && (!inLanguage(LANGUAGE_CXX_ONLY) || !inTransparentMode(MODE_INIT | MODE_EXPECT))) || inTransparentMode(MODE_ANONYMOUS) }? lcurly |

        // switch cases @test switch
        { !inMode(MODE_INIT) && (!inMode(MODE_EXPRESSION) || inTransparentMode(MODE_DETECT_COLON)) }?
        colon |

        // process template operator correctly @test template
        { inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST) }? tempope |

        // special default() call for C#
        { LA(1) == DEFAULT && inLanguage(LANGUAGE_CSHARP) && inTransparentMode(MODE_EXPRESSION) && next_token() == LPAREN}? expression_part_default |

        // statements that clearly start with a keyword
        { inMode(MODE_NEST | MODE_STATEMENT) && !inMode(MODE_FUNCTION_TAIL) && (LA(1) != EXTERN || next_token() == TEMPLATE)}? keyword_statements |

        { inLanguage(LANGUAGE_JAVA) && next_token() == LPAREN }? synchronized_statement |

        // statements identified by pattern (i.e., do not start with a keyword)
        { inMode(MODE_NEST | MODE_STATEMENT) && !inMode(MODE_FUNCTION_TAIL) }? pattern_statements |

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
  statements that begin with a unique keyword
*/
keyword_statements[] { ENTRY_DEBUG } :

        // conditional statements
        if_statement | else_statement | switch_statement | switch_case | switch_default |

        // iterative statements
        while_statement | for_statement | do_statement | foreach_statement |

        // jump statements
        return_statement | break_statement | continue_statement | goto_statement |

        // template declarations - both functions and classes
        template_declaration |

        // exception statements
        { inLanguage(LANGUAGE_JAVA) && next_token() == LPAREN }? try_statement_with_resource | try_statement | catch_statement | finally_statement | throw_statement |

        // namespace statements
        namespace_definition | using_namespace_statement |

        // C/C++
        typedef_statement |

        // Java - keyword only detected for Java
        import_statement | package_statement |

        // C# - keyword only detected for C#
        checked_statement | unchecked_statement | lock_statement | fixed_statement | unsafe_statement | yield_statements |

        // C/C++ assembly block
        asm_declaration
;
/*
  Statements, declarations, and definitions that must be matched by pattern.

  Basically we have an identifier and we don't know yet whether it starts an expression
  function definition, function declaration, or even a label.
*/
pattern_statements[] { int secondtoken = 0; int type_count = 0;
        STMT_TYPE stmt_type = NONE; CALLTYPE type = NOCALL;

        // detect the declaration/definition type
        pattern_check(stmt_type, secondtoken, type_count);

        ENTRY_DEBUG } :

        // variable declaration
        { stmt_type == VARIABLE }?
        variable_declaration_statement[type_count] |

        // check for declaration of some kind (variable, function, constructor, destructor
        { stmt_type == FUNCTION_DECL }?
        function_declaration[type_count] |

        // function definition
        { stmt_type == FUNCTION }?
        function_definition[type_count] |

        { stmt_type == CLASS_DEFN }?
        class_definition |

        { stmt_type == INTERFACE_DEFN }?
        interface_definition |

        { stmt_type == CLASS_DECL }?
        class_declaration |

        { stmt_type == STRUCT_DEFN }?
        struct_union_definition[SSTRUCT] |

        { stmt_type == STRUCT_DECL }?
        struct_declaration |

        { stmt_type == UNION_DEFN }?
        struct_union_definition[SUNION] |

        { stmt_type == UNION_DECL }?
        union_declaration |

        { stmt_type == ACCESS_REGION }?
        access_specifier_region |

        // @todo  Why no interface declaration?

        { stmt_type == GLOBAL_ATTRIBUTE }?
        attribute_csharp |

        { stmt_type == PROPERTY_ACCESSOR }?
        property_method[SFUNCTION_DEFINITION] |

        { stmt_type == PROPERTY_ACCESSOR_DECL }?
        property_method[SFUNCTION_DECLARATION] |

        // standalone macro
        { stmt_type == SINGLE_MACRO }?
        macro_call |

        // standalone macro
        { stmt_type == DELEGATE_FUNCTION }?
        delegate_anonymous |

        // constructor
        { stmt_type == CONSTRUCTOR }?
        constructor_definition |

        { stmt_type == CONSTRUCTOR_DECL }?
        constructor_declaration |

        // destructor
        { stmt_type == DESTRUCTOR }?
        destructor_definition |

        // destructor declaration
        { stmt_type == DESTRUCTOR_DECL }?
        destructor_declaration |

        // enum definition as opposed to part of type or declaration
        { stmt_type == ENUM_DECL }?
        enum_definition |

        // "~" which looked like destructor, but isn't
        { stmt_type == NONE }?
        expression_statement_process
        expression_process
        sole_destop |

        // labels to goto
        { secondtoken == COLON }?
        label_statement |

        // extern block as opposed to enum as part of declaration
        { stmt_type == NONE }?
        extern_definition |

        // call
        { isoption(parseoptions, OPTION_CPP) && (inMode(MODE_ACCESS_REGION) || (perform_call_check(type, secondtoken) && type == MACRO)) }?
        macro_call |

        expression_statement[type]
;

// efficient way to view the token after the current LA(1)
next_token[] returns [int token] {

    int place = mark();
    inputState->guessing++;

    // consume current token
    consume();

    token = LA(1);

    inputState->guessing--;
    rewind(place);
} :;

next_token_string[] returns [std::string token] {

    int place = mark();
    inputState->guessing++;

    // consume current token
    consume();

    token = LT(1)->getText();

    inputState->guessing--;
    rewind(place);
} :;

// is the next token one of the parameters
next_token_check[int token1, int token2] returns [bool result] {

    int token = next_token();

    result = token == token1 || token == token2;
} :;

// skips past any skiptokens to get the one after
look_past[int skiptoken] returns [int token] {

    int place = mark();
    inputState->guessing++;

    while (LA(1) == skiptoken)
        consume();

    token = LA(1);

    inputState->guessing--;
    rewind(place);
} :;

// skips past any skiptokens to get the one after
look_past_multiple[int skiptoken1, int skiptoken2, int skiptoken3] returns [int token] {

    int place = mark();
    inputState->guessing++;

    while (LA(1) != antlr::Token::EOF_TYPE && (LA(1) == skiptoken1 || LA(1) == skiptoken2 || LA(1) == skiptoken3))
        consume();

    token = LA(1);

    inputState->guessing--;
    rewind(place);
} :;

// functions
function_declaration[int type_count] { ENTRY_DEBUG } :
		{
            startNewMode(MODE_STATEMENT);

            startElement(SFUNCTION_DECLARATION);
        }
        function_header[type_count]
;

lambda_expression_cpp[] { ENTRY_DEBUG } :
		{

            bool iscall = lambda_call_check();
            if(iscall) {

                // start a new mode that will end after the argument list
                startNewMode(MODE_ARGUMENT | MODE_LIST);

                // start the function call element
                startElement(SFUNCTION_CALL);

            }

            startNewMode(MODE_FUNCTION_PARAMETER | MODE_FUNCTION_TAIL | MODE_ANONYMOUS);      

            startElement(SFUNCTION_DEFINITION);

        }

        lambda_capture

;

lambda_capture[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LIST | MODE_LOCAL | MODE_ARGUMENT);

            startElement(SLAMBDA_CAPTURE);

        }
        (

            LBRACKET 
            // suppress warning most likely compound_name's can match RBRACKET and it also is matched by RBRACKET
            // after wards.  
            (options { warnWhenFollowAmbig = false; } :
            { /* warning suppression */ LA(1) == COMMA }? comma | { LA(1) != RBRACKET }? lambda_capture_argument)* 
            RBRACKET
    
        )
;

lambda_capture_argument[] { CompleteElement element(this); ENTRY_DEBUG } :

        {
            startNewMode(MODE_LOCAL);

            startElement(SARGUMENT);
        }
        // suppress warning of another case where REFOPS or something is in both alts.
        (options { warnWhenFollowAmbig = false; } : lambda_capture_modifiers | compound_name)*
;

lambda_call_check[] returns [bool iscall] { ENTRY_DEBUG 

    iscall = false;

    int start = mark();
    inputState->guessing++;

    try {

        lambda_expression_full_cpp();

        if(LA(1) == LPAREN) iscall = true;

    } catch(...) {}

    inputState->guessing--;
    rewind(start);
} :;

lambda_expression_full_cpp[] { ENTRY_DEBUG } :

        // paren_pair and curly_pair seem to have nondeterminism because both can match LPAREN
        LBRACKET (~RBRACKET)* RBRACKET (options { warnWhenFollowAmbig = false; } : paren_pair)* function_tail curly_pair

;

lambda_capture_modifiers[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (isoption(parseoptions, OPTION_MODIFIER))
                    startElement(SMODIFIER);
        }
        (EQUAL | REFOPS)

;

function_definition[int type_count] { ENTRY_DEBUG } :
		{
            startNewMode(MODE_STATEMENT);

            startElement(SFUNCTION_DEFINITION);
        }
        function_header[type_count]
;

// property methods
property_method[int element] { ENTRY_DEBUG } :
		{
            // function definitions have a "nested" block statement
            startNewMode(MODE_STATEMENT);

            // start the function definition element
            startElement(element);
        }
        (attribute_csharp)*
        property_method_name
;

property_method_name[] { SingleElement element(this); ENTRY_DEBUG } :
		{
            startElement(SNAME);
        }
        (GET | SET | ADD | REMOVE)
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

        // call syntax succeeded
        type = CALL;

        // call syntax succeeded, however post call token is not legitimate
        if (isoption(parseoptions, OPTION_CPP) &&
               (_tokenSet_1.member(postcalltoken) || postcalltoken == NAME
            || (!inLanguage(LANGUAGE_CSHARP) && postcalltoken == LCURLY)
            || postcalltoken == EXTERN || postcalltoken == STRUCT || postcalltoken == UNION || postcalltoken == CLASS
            || (!inLanguage(LANGUAGE_CSHARP) && postcalltoken == RCURLY)
            || postcalltoken == 1 /* EOF ? */
            || postcalltoken == TEMPLATE
            || postcalltoken == PUBLIC || postcalltoken == PRIVATE || postcalltoken == PROTECTED
            || postcalltoken == STATIC || postcalltoken == CONST))

            type = MACRO;

    } catch (...) {

        type = NOCALL;

        if (isoption(parseoptions, OPTION_CPP) && argumenttoken != 0 && postcalltoken == 0)
            type = MACRO;

        // single macro call followed by statement_cfg
        else if (isoption(parseoptions, OPTION_CPP) && secondtoken != -1
                 && (_tokenSet_1.member(secondtoken) || secondtoken == LCURLY || secondtoken == 1 /* EOF */
                     || secondtoken == PUBLIC || secondtoken == PRIVATE || secondtoken == PROTECTED))

            type = MACRO;
    }

    inputState->guessing--;
    rewind(start);

    ENTRY_DEBUG } :;

call_check[int& postnametoken, int& argumenttoken, int& postcalltoken] { ENTRY_DEBUG } :

        // detect name, which may be name of macro or even an expression
        (function_identifier | SIZEOF (DOTDOTDOT)*)

        // record token after the function identifier for future use if this fails
        markend[postnametoken]
        (
            { isoption(parseoptions, OPTION_CPP) }?
            // check for proper form of argument list
            call_check_paren_pair[argumenttoken]

            // record token after argument list to differentiate between call and macro
            markend[postcalltoken] |

            LPAREN
        )
;

call_check_paren_pair[int& argumenttoken, int depth = 0] { bool name = false; ENTRY_DEBUG } :

        LPAREN

        // record token after the start of the argument list
        markend[argumenttoken]
        ( options { greedy = true; } :

            // recursive nested parentheses
            call_check_paren_pair[argumenttoken, depth + 1] set_bool[name, false] |

            // special case for something that looks like a declaration
            { !name || (depth > 0) }?
            identifier set_bool[name, true] |

            // special case for something that looks like a declaration
            { LA(1) == DELEGATE /* eliminates ANTRL warning, will be nop */ }? delegate_anonymous |

            { next_token_check(LCURLY, LPAREN) }?
            lambda_anonymous |

            (LBRACKET (~RBRACKET)* RBRACKET (LPAREN | LCURLY)) => lambda_expression_full_cpp  |

            // found two names in a row, so this is not an expression
            // cause this to fail by explicitly throwing exception
            { depth == 0 }?
            identifier throw_exception[true] |

            // forbid parentheses (handled recursively) and cfg tokens
            { !_tokenSet_1.member(LA(1)) }? ~(LPAREN | RPAREN | TERMINATE) set_bool[name, false]
        )*

        RPAREN
;

// records the current token, even in guessing mode
markend[int& token] { token = LA(1); } :;

/* Keyword Statements */

// while statement
while_statement[] { ENTRY_DEBUG } :
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

// do while statement
do_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement (after condition)
            // set to top mode so that end of block will
            // not end statement itself
            startNewMode(MODE_STATEMENT | MODE_TOP | MODE_DO_STATEMENT);

            // start of do statement
            startElement(SDO_STATEMENT);

            // mode to nest while part of do while statement
            startNewMode(MODE_STATEMENT | MODE_NEST);
        }
        DO
;

// while part of do statement
do_while[] { ENTRY_DEBUG } :
        {
            // mode for do statement is in top mode so that
            // end of the block will not end the statement
            clearMode(MODE_TOP);

            // expect a condition to follow
            startNewMode(MODE_CONDITION | MODE_EXPECT);
        }
        WHILE
;

// start of for statement
for_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement after the for group
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the for statement
            startElement(SFOR_STATEMENT);
        }
        FOR
        {
            // statement with nested statement after the for group
            startNewMode(MODE_EXPECT | MODE_FOR_GROUP);
        }
;

// start of foreach statement (C#)
foreach_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement after the for group
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the for statement
            startElement(SFOREACH_STATEMENT);
        }
        FOREACH
        {
            // statement with nested statement after the for group
            startNewMode(MODE_EXPECT | MODE_FOR_GROUP);
        }
;

// start of for group, i.e., initialization, test, increment
for_group[] { ENTRY_DEBUG } :
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

// for parameter list initialization.  used in multiple places
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

for_initialization[] { int type_count = 0;  int secondtoken = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        for_initialization_action
        (
            // explicitly check for a variable declaration since it can easily
            // be confused with an expression
            { pattern_check(stmt_type, secondtoken, type_count) && stmt_type == VARIABLE }?
            for_initialization_variable_declaration[type_count] |

            expression
        )
;

// Statement for the declaration of a variable or group of variables
// in a for initialization
for_initialization_variable_declaration[int type_count] { ENTRY_DEBUG } :
        {
            // start a new mode for the expression which will end
            // inside of the terminate
            startNewMode(MODE_LIST);

            startElement(SDECLARATION);
        }
        variable_declaration[type_count]
;


// for parameter list condition setup.  Used in multiple places.
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

// for condition
for_condition[] { ENTRY_DEBUG } :
        for_condition_action

        // non-empty condition
        expression
;

// increment in for parameter list
for_increment[] { ENTRY_DEBUG } :
        {
            assertMode(MODE_EXPECT | MODE_FOR_INCREMENT);

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
if_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement
            // detection of else
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_IF);

            ++ifcount;

            // start the if statement
            startElement(SIF_STATEMENT);

            // expect a condition
            // start THEN after condition
            startNewMode(MODE_CONDITION | MODE_EXPECT);
        }
        IF
;

/*
 else part of if statement

 else is detected on its own, and as part of termination (semicolon or
 end of a block
*/
else_statement[] { ENTRY_DEBUG } :
        {
            // treat as a statement with a nested statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_ELSE);

            // start the else part of the if statement
            startElement(SELSE);
        }
        ELSE
;

//  start of switch statement
switch_statement[] { ENTRY_DEBUG } :
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

// actions to perform before first starting a section.  Uses in multiple places.
section_entry_action_first[] :
        {
            // start a new section inside the block with nested statements
            startNewMode(MODE_TOP_SECTION | MODE_TOP | MODE_STATEMENT | MODE_NEST);
        }
;

// actions to perform before starting a section
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

// case treated as a statement
switch_case[] { ENTRY_DEBUG } :
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

// default treated as a statement
switch_default[] { ENTRY_DEBUG } :
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

// import statement
import_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME | MODE_EXPECT);

            // start the return statement
            startElement(SIMPORT);
        }
        IMPORT
;

// package statement
package_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME | MODE_EXPECT);

            // start the return statement
            startElement(SPACKAGE);
        }
        PACKAGE
;

// return statement
return_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the return statement
            startElement(SRETURN_STATEMENT);
        }
        RETURN
;

// yield statements
yield_statements[] { int t = next_token(); ENTRY_DEBUG } :

        { t == RETURN }?
        yield_return_statement |

        { t == BREAK }?
        yield_break_statement
;

yield_specifier[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        YIELD
;

yield_return_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the return statement
            startElement(SRETURN_STATEMENT);
        }
        yield_specifier RETURN
;

yield_break_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the break statement
            startElement(SBREAK_STATEMENT);
        }
        yield_specifier BREAK
;

// break statement
break_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the break statement
            startElement(SBREAK_STATEMENT);
        }
        BREAK
;

// start of continue statement
continue_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the continue statement
            startElement(SCONTINUE_STATEMENT);
        }
        CONTINUE
;

// start of goto statement
goto_statement[] { ENTRY_DEBUG } :
        {
            // statement with an expected label name
            // label name is a subset of variable names
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME);

            // start the goto statement
            startElement(SGOTO_STATEMENT);
        }
        GOTO
;

// complete assembly declaration statement
asm_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the asm statement
            startElement(SASM);
        }
        ASM
        ({ true }? paren_pair | ~(LCURLY | RCURLY | TERMINATE))*
;

// extern definition
extern_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_EXTERN);

            // start the namespace definition
            startElement(SEXTERN);
        }
        EXTERN
;

// name of extern section
extern_name[] { ENTRY_DEBUG } :
        string_literal
        {
            // nest a block inside the namespace
            setMode(MODE_STATEMENT | MODE_NEST);
        }
;

// namespaces
namespace_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NAMESPACE | MODE_VARIABLE_NAME);

            // start the namespace definition
            startElement(SNAMESPACE);
        }
        NAMESPACE
;

namespace_alias[] { ENTRY_DEBUG } :
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
            setMode(MODE_STATEMENT | MODE_NEST);
        }
        lcurly
;

// using directive
namespace_directive[] { ENTRY_DEBUG } :
        {
            // statement with an expected namespace name after the keywords
            startNewMode(MODE_STATEMENT | MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // start the using directive
            startElement(SUSING_DIRECTIVE);
        }
        USING
;

/* Declarations Definitions CFG */

check_end[int& token] { token = LA(1); ENTRY_DEBUG } :
        LCURLY | TERMINATE | COLON | COMMA | RPAREN
;

class_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class definition
            startElement(SCLASS_DECLARATION);
        }
        ({ inLanguage(LANGUAGE_JAVA) }? annotation)*
        ({ inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |
        { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp)*

        (specifier)* CLASS class_post class_header
;

class_preprocessing[int token] { ENTRY_DEBUG } :
        {
            bool intypedef = inMode(MODE_TYPEDEF);

            if (intypedef)
                startElement(STYPE);

            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS | MODE_DECL);

            // start the class definition
            startElement(token);
            // classes end at the end of the block
            if (intypedef || inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP)) {
                setMode(MODE_END_AT_BLOCK);
            }
        }
;

class_preamble[] { ENTRY_DEBUG } :
        // suppress warning probably do to only having ()*
        (options { greedy = true; } : { inLanguage(LANGUAGE_JAVA) }? annotation | { inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |
        { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp)*

        (specifier)*
;

class_definition[] { ENTRY_DEBUG } :
        class_preprocessing[SCLASS]

        class_preamble CLASS class_post (class_header lcurly | lcurly)
        {

            if (inLanguage(LANGUAGE_CXX_ONLY))
                class_default_access_action(SPRIVATE_ACCESS_DEFAULT);
        }
;

class_post[] { ENTRY_DEBUG } :
        (options { greedy = true; } : { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp)*
        (options { greedy = true; } : specifier)*
;

enum_class_definition[] { ENTRY_DEBUG } :
        class_preprocessing[SENUM]

        class_preamble ENUM (class_header lcurly | lcurly)

;

anonymous_class_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS | MODE_END_AT_BLOCK);

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

anonymous_class_super[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the super name of an anonymous class
            startElement(SDERIVATION_LIST);
        }
        compound_name_inner[false]
;

interface_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS);

            // start the interface definition
            startElement(SINTERFACE);

            // java interfaces end at the end of the block
            setMode(MODE_END_AT_BLOCK);
        }
        class_preamble (interface_annotation | INTERFACE) class_header lcurly
;

struct_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class definition
            startElement(SSTRUCT_DECLARATION);
        }
        class_preamble STRUCT class_post class_header
;

struct_union_definition[int element_token] { ENTRY_DEBUG } :
        class_preprocessing[element_token]

        class_preamble (STRUCT | UNION) class_post (class_header lcurly | lcurly)
        {
           if (inLanguage(LANGUAGE_CXX_ONLY))
               class_default_access_action(SPUBLIC_ACCESS_DEFAULT);
        }
;

union_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class definition
            startElement(SUNION_DECLARATION);
        }
        class_preamble UNION class_post class_header
;

// default private/public section for C++
class_default_access_action[int access_token] { ENTRY_DEBUG } :
        {
            if (inLanguage(LANGUAGE_CXX_ONLY) && (SkipBufferSize() > 0 ||
                !(LA(1) == PUBLIC || LA(1) == PRIVATE || LA(1) == PROTECTED || LA(1) == SIGNAL))) {

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

class_header[] { ENTRY_DEBUG } :

        { isoption(parseoptions, OPTION_CPP) }?
        (macro_call_check class_header_base LCURLY)=>
           macro_call class_header_base |

        class_header_base
;

class_header_base[] { bool insuper = false; ENTRY_DEBUG } :

        // suppress ()* warning
        { LA(1) != FINAL }? compound_name_inner[false] (options { greedy = true; } : specifier)*

        ({ inLanguage(LANGUAGE_CXX_FAMILY) }? (options { greedy = true; } : derived))*

        // move suppressed ()* warning to begin
        (options { greedy = true; } : { inLanguage(LANGUAGE_CXX_FAMILY) }? generic_type_constraint)*

        ({ inLanguage(LANGUAGE_JAVA_FAMILY) }? (options { greedy = true; } : super_list_java { insuper = true; } extends_list))*
        ({ inLanguage(LANGUAGE_JAVA_FAMILY) }?
            {
                if (!insuper) {
                    insuper = true;
                    super_list_java();
                }
            }
            implements_list
        )*
        {
            if (insuper)
                endMode();
        }
;

// Each instance of an access specifier defines a region in the class
access_specifier_region[] { ENTRY_DEBUG } :
        section_entry_action
        {
            // mark access regions to detect statements that only occur in them
            setMode(MODE_ACCESS_REGION);
        }
        (
            // start access region
            {
                startElement(SPUBLIC_ACCESS);
            }
            PUBLIC |
            {
                startElement(SPRIVATE_ACCESS);
            }
            PRIVATE |
            {
                startElement(SPROTECTED_ACCESS);
            }
            PROTECTED |
            {
                startElement(SSIGNAL_ACCESS);
            }
            SIGNAL

        ) 
    (compound_name)* COLON
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
                endMode(MODE_CONDITION);
            }

            if(inTransparentMode(MODE_TRAILING_RETURN)) {
                endDownToMode(MODE_TRAILING_RETURN);
                endMode(MODE_TRAILING_RETURN);
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

                endMode();
            }
        }
        lcurly_base
        {

            incCurly();

            // alter the modes set in lcurly_base
            setMode(MODE_TOP | MODE_STATEMENT | MODE_NEST | MODE_LIST);

        }
;

// left curly brace.  Used in multiple places
lcurly_base[] { ENTRY_DEBUG } :
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

// end of a block.  Also indicates the end of some open elements.
block_end[] { ENTRY_DEBUG } :
        // handling of if with then block followed by else
        // handle the block, however scope of then completion stops at if
        rcurly
        {
            if (inMode(MODE_ANONYMOUS)) {
                endMode();
                if(LA(1) == LPAREN) { call_argument_list(); }
                return;
            }

            // end all statements this statement is nested in
            // special case when ending then of if statement
            // end down to either a block or top section, or to an if, whichever is reached first
            endDownToModeSet(MODE_BLOCK | MODE_TOP | MODE_IF | MODE_ELSE | MODE_TRY | MODE_ANONYMOUS);

            bool endstatement = inMode(MODE_END_AT_BLOCK);
            bool anonymous_class = inMode(MODE_CLASS) && inMode(MODE_END_AT_BLOCK);

            // some statements end with the block
            if (inMode(MODE_END_AT_BLOCK)) {
                endMode();

                if (inTransparentMode(MODE_TEMPLATE))
                    endMode();
            }

            // looking for a terminate (';')
            // some statements end with the block if there is no terminate
            if (inMode(MODE_END_AT_BLOCK_NO_TERMINATE) && LA(1) != TERMINATE) {
                endstatement = true;
                endMode();
            }

            if (inTransparentMode(MODE_ENUM) && inLanguage(LANGUAGE_CSHARP | LANGUAGE_CXX_ONLY))
                endMode();

            if (!(anonymous_class) && (!(inMode(MODE_CLASS) || inTransparentMode(MODE_ENUM))
                                       || ((inMode(MODE_CLASS) || inTransparentMode(MODE_ENUM)) && endstatement)))
                else_handling();

            // if we are in a declaration (as part of a class/struct/union definition)
            // then we needed to markup the (abbreviated) variable declaration
            if (inMode(MODE_DECL) && LA(1) != TERMINATE)
                short_variable_declaration();

            // @todo  Need a test case that makes this necessary
            // end of block may lead to adjustment of cpp modes
            cppmode_adjust();

        }
;

// right curly brace.  Not used directly, but called by block_end
rcurly[] { ENTRY_DEBUG } :
        {

            // end any elements inside of the block
            endDownToMode(MODE_TOP);

            // flush any whitespace tokens since sections should
            // end at the last possible place
            flushSkip();

            // end any sections inside the mode
            endDownOverMode(MODE_TOP_SECTION);

            if(getCurly() != 0)
                decCurly();

        }
        RCURLY
        {
            // end the current mode for the block
            // don't end more than one since they may be nested
            endMode(MODE_TOP);
        }
;

// terminate (';')
terminate[] { ENTRY_DEBUG } :
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

terminate_token[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (inMode(MODE_STATEMENT | MODE_NEST) && !inMode(MODE_DECL))
                startElement(SEMPTY);
        }
        TERMINATE
;

terminate_pre[] { ENTRY_DEBUG } :
        {
            // end any elements inside of the statement
            if (!inMode(MODE_TOP | MODE_STATEMENT | MODE_NEST))
                endDownToModeSet(MODE_STATEMENT | MODE_EXPRESSION_BLOCK |
                                   MODE_INTERNAL_END_CURLY | MODE_INTERNAL_END_PAREN);

            if(inTransparentMode(MODE_TRAILING_RETURN)) {
                endDownToMode(MODE_TRAILING_RETURN);
                endMode(MODE_TRAILING_RETURN);
            }

        }
;

terminate_post[] { ENTRY_DEBUG } :
        {
            // end all statements this statement is nested in
            // special case when ending then of if statement
            if (!isoption(parseoptions, OPTION_EXPRESSION) &&
                 (!inMode(MODE_EXPRESSION_BLOCK) || inMode(MODE_EXPECT)) &&
                !inMode(MODE_INTERNAL_END_CURLY) && !inMode(MODE_INTERNAL_END_PAREN)) {

                // end down to either a block or top section, or to an if or else
                endDownToModeSet(MODE_TOP | MODE_IF | MODE_ELSE);
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
            // any #else or #endif in consumeSkippedTokens
            // see below
            unsigned int cppmode_size = !cppmode.empty() ? cppmode.top().statesize.size() : 0;

            // catch and finally statements are nested inside of a try, if at that level
            // so if no CATCH or FINALLY, then end now
            bool intry = inMode(MODE_TRY);
            bool restoftry = LA(1) == CATCH || LA(1) == FINALLY;
            if (intry && !restoftry) {
                endMode(MODE_TRY);
                endDownToMode(MODE_TOP);
            }

            // handle parts of if
            if (inTransparentMode(MODE_IF) && !(intry && restoftry)) {

                if (LA(1) != ELSE) {

                    endDownToMode(MODE_TOP);

                // when an ELSE is next and already in an else, must end properly (not needed for then)
                } else if (LA(1) == ELSE && inMode(MODE_ELSE)) {

                    while (inMode(MODE_ELSE)) {

                        // end the else
                        endMode();

                        /*
                          @todo  Can we only do this if we detect a cpp change?
                          This would occur EVEN if we have an ifcount of 2.
                        */
                        // we have an extra else that is rogue
                        // it either is a single else statement, or part of an #ifdef ... #else ... #endif
                        if (LA(1) == ELSE && ifcount == 1)
                            break;

                        // ending an else means ending an if
                        if (inMode(MODE_IF)) {
                            endMode();
                            --ifcount;
                        }
                    }

                    // following ELSE indicates end of outer then
                    if (inMode(MODE_THEN))
                        endMode();
                }
            } else if (inTransparentMode(MODE_ELSE)) {

                // have an else, but are not in an if.  Could be a fragment,
                // or could be due to an #ifdef ... #else ... #endif
                if (inMode(MODE_ELSE))
                    endMode(MODE_ELSE);
            }

            // update the state size in cppmode if changed from using consumeSkippedTokens
            if (!cppmode.empty() && cppmode_size != cppmode.top().statesize.size()) {

                cppmode.top().statesize.back() = size();

                // remove any finished ones
                if (cppmode.top().isclosed)
                        cppmode_cleanup();
            }
        }
;

// mid-statement
statement_part[] { int type_count;  int secondtoken = 0; STMT_TYPE stmt_type = NONE;
                   CALLTYPE type = NOCALL; ENTRY_DEBUG } :

        { inMode(MODE_EAT_TYPE) }?
        type_identifier
        update_typecount[MODE_FUNCTION_NAME] |

        // block right after argument list, e.g., throws list in Java
        { inTransparentMode(MODE_END_LIST_AT_BLOCK) }?
        { endDownToMode(MODE_LIST); endMode(MODE_LIST); }
        lcurly |

        /*
          MODE_EXPRESSION
        */

        // expression block or expressions
        // must check before expression
        { inMode(MODE_EXPRESSION_BLOCK | MODE_EXPECT) }?
        pure_expression_block |

        /*
          MODE_FUNCTION_TAIL
        */

        // throw list at end of function header
        { (inLanguage(LANGUAGE_OO)) && inMode(MODE_FUNCTION_TAIL) }?
        throw_list |

        // throw list at end of function header
        { (inLanguage(LANGUAGE_CXX_ONLY))&& inTransparentMode(MODE_FUNCTION_TAIL) }?
        noexcept_list |

        // throw list at end of function header
        { (inLanguage(LANGUAGE_CXX_ONLY))&& inTransparentMode(MODE_FUNCTION_TAIL) && next_token() == LBRACKET }?
        attribute_cpp |

        // K&R function parameters
        { (inLanguage(LANGUAGE_C) || inLanguage(LANGUAGE_CXX_ONLY)) && inMode(MODE_FUNCTION_TAIL) &&
          pattern_check(stmt_type, secondtoken, type_count) && stmt_type == VARIABLE }?
        kr_parameter[type_count] |

        // function specifier at end of function header
        { inLanguage(LANGUAGE_CXX_FAMILY) && inMode(MODE_FUNCTION_TAIL) }?
        function_specifier |

        // function specifier at end of function header
        { inLanguage(LANGUAGE_CXX_ONLY) && inMode(MODE_FUNCTION_TAIL) }?
        trailing_return |

        // start of argument for return or throw statement
        { inMode(MODE_EXPRESSION | MODE_EXPECT) &&
            isoption(parseoptions, OPTION_CPP) && perform_call_check(type, secondtoken) && type == MACRO }?
        macro_call |

        { inMode(MODE_EXPRESSION | MODE_EXPECT) }?
        expression[type] |

        // already in an expression, and run into a keyword
        // so stop the expression, and markup the keyword statement
        { inMode(MODE_EXPRESSION) }?
        terminate_pre
        terminate_post
        keyword_statements |

        // already in an expression
        { inMode(MODE_EXPRESSION) }?
        expression_part_plus_linq |

        // call list in member initialization list
        { inMode(MODE_CALL | MODE_LIST) && (LA(1) != LCURLY || inLanguage(LANGUAGE_CXX_ONLY)) }?
        call |

        // call list in member initialization list
        { inMode(MODE_CALL | MODE_LIST) && (LA(1) != LCURLY || inLanguage(LANGUAGE_CXX_ONLY)) }?
        sizeof_call |

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
        { inMode(MODE_ENUM) }?
        enum_class_header |

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
        { inMode(MODE_INIT | MODE_EXPECT) && (inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_JAVA)) }?
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

        { inTransparentMode(MODE_TEMPLATE) && inMode(MODE_LIST | MODE_EXPECT) && !inMode(MODE_TEMPLATE_PARAMETER_LIST)}?
        class_declaration |

        { inTransparentMode(MODE_TEMPLATE) && inMode(MODE_LIST | MODE_EXPECT) }?
        template_param_list |

        // expecting a template parameter
        { inTransparentMode(MODE_TEMPLATE) && inMode(MODE_LIST) }?
        template_param |

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

lparen_marked[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            incParen();

            if (isoption(parseoptions, OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        LPAREN
;

// marking comma operator
bar[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR) && !inMode(MODE_PARAMETER))
                startElement(SOPERATOR);
        }
        BAR
;

comma[] { ENTRY_DEBUG } :
        {
            // comma ends the current item in a list
            // or ends the current expression
            if (!inTransparentMode(MODE_PARSE_EOL)
                && (inTransparentMode(MODE_LIST) || inTransparentMode(MODE_STATEMENT)))

                // might want to check for !inMode(MODE_INTERNAL_END_CURLY)
                endDownToModeSet(MODE_LIST | MODE_STATEMENT);

            // comma in a variable initialization end init of current variable
            if (inMode(MODE_IN_INIT))
                endMode(MODE_IN_INIT);
        }
        comma_marked
;

// marking comma operator
comma_marked[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR) && !inMode(MODE_PARAMETER) && !inMode(MODE_ARGUMENT))
                startElement(SOPERATOR);
        }
        COMMA
;

colon_marked[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        COLON
;

colon[] { ENTRY_DEBUG } :
        {
            // colon ends the current item in a list
            if (inTransparentMode(MODE_TOP_SECTION))
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
condition[] { ENTRY_DEBUG } :
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

/* functions */

function_pointer_name_grammar[] { ENTRY_DEBUG } :
        LPAREN function_pointer_name_base RPAREN
;

function_pointer_name_base[] { ENTRY_DEBUG bool flag = false; } :

        // special case for function pointer names that don't have '*'
        { _tokenSet_13.member(LA(1)) }?
        compound_name_inner[false] |

        // special name prefix of namespace or class
        identifier (template_argument_list)* DCOLON function_pointer_name_base |

        // typical function pointer name
        MULTOPS (compound_name_inner[false])*

        // optional array declaration
        (variable_identifier_array_grammar_sub[flag])*
;

function_header[int type_count] { ENTRY_DEBUG } :

        // no return value functions:  casting operator method and main
        { type_count == 0 }? function_identifier
        { replaceMode(MODE_FUNCTION_NAME, MODE_FUNCTION_PARAMETER | MODE_FUNCTION_TAIL); } |

        function_type[type_count]
;

function_tail[] { ENTRY_DEBUG } :

        (options { greedy = true; } :

            /* order is important */
            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            function_specifier |

            { inLanguage(LANGUAGE_CXX_ONLY) }?
            ref_qualifier |

            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            TRY |

            { inLanguage(LANGUAGE_OO) }?
            complete_throw_list |

            { inLanguage(LANGUAGE_CXX_ONLY) }?
            complete_noexcept_list |

            { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}?
            attribute_cpp |

            { inLanguage(LANGUAGE_CXX_ONLY) }?
            trailing_return |

            // K&R
            { inLanguage(LANGUAGE_C) }? (

            // @todo:  Must be integrated into other C-based languages
            // @todo:  Wrong markup
            (NAME paren_pair)=> macro_call |
            { look_past(NAME) == LCURLY }? NAME |
              parameter (MULTOPS | NAME | COMMA)* TERMINATE
            )
        )*
;

ref_qualifier []  { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (isoption(parseoptions, OPTION_MODIFIER))
                startElement(SMODIFIER);
        }

        REFOPS | RVALUEREF
;

trailing_return [] {  int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :

        TRETURN
        ({ pattern_check(stmt_type, secondtoken, type_count, true) && (stmt_type == FUNCTION || stmt_type == FUNCTION_DECL)}?
        {startNewMode(MODE_TRAILING_RETURN);} function_declaration[type_count] function_identifier parameter_list | function_type[type_count + 1]
        )
;

pattern_check[STMT_TYPE& type, int& token, int& type_count, bool inparam = false] returns [bool isdecl] {

    isdecl = true;

    type = NONE;

    int start = mark();
    inputState->guessing++;

    bool sawenum;
    int posin = 0;
    int fla = 0;

    try {

        pattern_check_core(token, fla, type_count, type, inparam, sawenum, posin);

    } catch (...) {

        if (type == VARIABLE && type_count == 0) {
            type_count = 1;
        }

    }

    // may just have an expression
    if (type == VARIABLE && posin)
        type_count = posin - 1;

    // enum
    else if (type == 0 && sawenum) {
        type = ENUM_DECL;

    }

    // may just have a single macro (no parens possibly) before a statement
    else if (type == 0 && type_count == 0 && _tokenSet_1.member(LA(1)))
        type = SINGLE_MACRO;

    else if(type == 0 && type_count == 1 && (LA(1) == CLASS || LA(1) == STRUCT || LA(1) == UNION))
        type = SINGLE_MACRO;

    // may just have an expression
    else if (type == DESTRUCTOR && !inLanguage(LANGUAGE_CXX_FAMILY))
        type = NULLOPERATOR;

    // declaration form
    else if (type == CONSTRUCTOR && fla == TERMINATE)
        type = CONSTRUCTOR_DECL;

    // declaration form
    else if (type == DESTRUCTOR && fla == TERMINATE)
        type = DESTRUCTOR_DECL;

    // declaration form
    else if (type == FUNCTION && fla == TERMINATE)
        type = FUNCTION_DECL;

    // we actually have a macro and then a constructor
    else if(type == FUNCTION && fla == COLON)
        type = SINGLE_MACRO;

    // not really a destructor
    if (type == DESTRUCTOR_DECL && (!inTransparentMode(MODE_CLASS) || inTransparentMode(MODE_FUNCTION_TAIL)))
        type = EXPRESSION;

    int save_la = LA(1);

    inputState->guessing--;
    rewind(start);

    if(!inMode(MODE_FUNCTION_TAIL) && type == 0 && type_count == 0 
       && _tokenSet_27.member(LA(1)) && (!inLanguage(LANGUAGE_CXX_ONLY) || !(LA(1) == FINAL || LA(1) == OVERRIDE))
       && save_la == TERMINATE)
        type = VARIABLE;

} :;

/*
  Figures out if we have a declaration, either variable or function.

  This is pretty complicated as it has to figure out whether it is a declaration or not,
  and whether it is a function or a variable declaration.
*/
pattern_check_core[int& token,      /* second token, after name (always returned) */
              int& fla,             /* for a function, TERMINATE or LCURLY, 0 for a variable */
              int& type_count,      /* number of tokens in type (not including name) */
              STMT_TYPE& type,      /* type discovered */
              bool inparam,         /* are we in a parameter */
              bool& sawenum,        /* have we seen an enum */
              int& posin            /* */
        ] {
            token = 0;
            fla = 0;
            type_count = 0;
            type = NONE;
            sawenum = false;
            posin = 0;
            isdestructor = false;           // global flag detected during name matching
            int attribute_count = 0;
            int specifier_count = 0;
            bool foundpure = false;
            bool isoperator = false;
            bool isconstructor = false;
            bool saveisdestructor = false;
            bool endbracket = false;
            bool modifieroperator = false;
            qmark = false;
            int real_type_count = 0;
            bool lcurly = false;
        ENTRY_DEBUG } :

        // main pattern for variable declarations, and most function declaration/definitions.
        // trick isv to look for function declarations/definitions, and along the way record
        // if a declaration

        // int -> NONE
        // int f -> VARIABLE
        // int f(); -> FUNCTION
        // int f() {} -> FUNCTION

        /*
          Process all the parts of a potential type.  Keep track of total
          parts, specifier parts, and second token
        */
        { next_token_check(LPAREN, LCURLY) }? DELEGATE set_type[type, DELEGATE_FUNCTION] |
        (
        ({ inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP) || (type_count == 0) || LA(1) != LBRACKET || (LA(1) == LBRACKET && next_token() == LBRACKET) }?

            set_bool[qmark, (qmark || (LA(1) == QMARK)) && inLanguage(LANGUAGE_CSHARP)]

            set_int[posin, LA(1) == IN ? posin = type_count : posin]

            set_bool[isoperator, isoperator || LA(1) == OPERATOR]

            // was their a bracket on the end?  Need to know for Java
            set_bool[endbracket, inLanguage(LANGUAGE_JAVA_FAMILY) && LA(1) == LBRACKET]

            // record any type modifiers that are also operators
            // this is for disambiguation of destructor declarations from expressions involving
            // the ~ operator
            set_bool[modifieroperator, modifieroperator || LA(1) == REFOPS || LA(1) == MULTOPS || LA(1) == QMARK]

            set_bool[sawenum, sawenum || LA(1) == ENUM]
            (
                { _tokenSet_23.member(LA(1)) && (LA(1) != SIGNAL || (LA(1) == SIGNAL && look_past(SIGNAL) == COLON)) && (!inLanguage(LANGUAGE_CXX_ONLY) || (LA(1) != FINAL && LA(1) != OVERRIDE))}?
                set_int[token, LA(1)]
                set_bool[foundpure, foundpure || LA(1) == CONST]
                (specifier | { next_token() == COLON }? SIGNAL)
                set_int[specifier_count, specifier_count + 1]
                set_type[type, ACCESS_REGION,
                        inLanguage(LANGUAGE_CXX) && look_past(NAME) == COLON && (token == PUBLIC || token == PRIVATE || token == PROTECTED || token == SIGNAL)]
                throw_exception[type == ACCESS_REGION] |

                { inLanguage(LANGUAGE_CSHARP) }?
                LBRACKET
                        // suppress warning
                        (options { greedy = true; } : COMMA)*

                        // ~RBRACKET matches these as well suppress warning. 
                        (options { warnWhenFollowAmbig = false; } : (RETURN | EVENT |

                        set_type[type, GLOBAL_ATTRIBUTE, check_global_attribute()]
                        throw_exception[type == GLOBAL_ATTRIBUTE] 
                        identifier))?

                        //complete_expression
                        (~(RBRACKET))*
                RBRACKET
                set_int[attribute_count, attribute_count + 1] |

                { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}?
                LBRACKET LBRACKET

                        //complete_expression
                        (~(RBRACKET))*
                RBRACKET RBRACKET
                set_int[attribute_count, attribute_count + 1] |

                { type_count == attribute_count }?
                property_method_name
                set_type[type, PROPERTY_ACCESSOR, true] |

                { type_count == attribute_count + specifier_count  && (!inLanguage(LANGUAGE_JAVA) 
            || (inLanguage(LANGUAGE_JAVA) && (LA(1) != ATSIGN 
                                             || (LA(1) == ATSIGN && next_token() == INTERFACE)))) }?
                (CLASS               set_type[type, CLASS_DECL]     |
                 STRUCT              set_type[type, STRUCT_DECL]    |
                 UNION               set_type[type, UNION_DECL]     |
                 INTERFACE           set_type[type, INTERFACE_DECL] |
                 ATSIGN INTERFACE set_type[type, INTERFACE_DECL])
                set_bool[lcurly, LA(1) == LCURLY]
                (options { greedy = true; } : { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp)*
                ({ LA(1) == DOTDOTDOT }? DOTDOTDOT set_int[type_count, type_count + 1])*
                class_post
                (class_header | LCURLY)
                set_type[type, CLASS_DEFN,     type == CLASS_DECL     && (LA(1) == LCURLY || lcurly)]
                set_type[type, STRUCT_DEFN,    type == STRUCT_DECL    && (LA(1) == LCURLY || lcurly)]
                set_type[type, UNION_DEFN,     type == UNION_DECL     && (LA(1) == LCURLY || lcurly)]
                set_type[type, INTERFACE_DEFN, type == INTERFACE_DECL && (LA(1) == LCURLY || lcurly)]
                set_type[type, NONE, !(LA(1) == TERMINATE || (LA(1) == LCURLY || lcurly))]
                throw_exception[type != NONE] 
                set_int[type_count, type_count + 1] |

                { inLanguage(LANGUAGE_JAVA_FAMILY) }?
                template_argument_list set_int[specifier_count, specifier_count + 1] |

                { inLanguage(LANGUAGE_JAVA_FAMILY) }?
                annotation
                set_int[attribute_count, attribute_count + 1] |

                // typical type name
                { !inLanguage(LANGUAGE_CSHARP) || LA(1) != ASYNC }?
                set_bool[operatorname, false]
                compound_name set_bool[foundpure]
                    set_bool[isoperator, isoperator || (inLanguage(LANGUAGE_CXX_FAMILY) && 
                             operatorname && type_count == specifier_count)] 
                set_bool[operatorname, false] |

                // special function name
                MAIN set_bool[isoperator, type_count == 0] |

        { inLanguage(LANGUAGE_JAVA) && inMode(MODE_PARAMETER) }? bar |

                // type parts that can occur before other type parts (excluding specifiers)
                // do not match a struct class or union.  If was class/struct/union decl will not reach here.
                // if elaborated type specifier should also be handled above. Reached here because 
                // non-specifier then class/struct/union.
                { LA(1) != LBRACKET && (LA(1) != CLASS && LA(1) != STRUCT && LA(1) != UNION)}?
        ({LA(1) == DECLTYPE }? decltype_full | pure_lead_type_identifier_no_specifiers) set_bool[foundpure] |

                // type parts that must only occur after other type parts (excluding specifiers)
                non_lead_type_identifier throw_exception[!foundpure]
            )

            // another type part
            set_int[type_count, type_count + 1]

            // record second (before we parse it) for label detection
            set_int[token, LA(1), type_count == 1]
        )*

        // special case for property attributes as names, e.g., get, set, etc.
        throw_exception[type == PROPERTY_ACCESSOR && (type_count == attribute_count + 1) && LA(1) == LCURLY]
        set_type[type, PROPERTY_ACCESSOR_DECL, type == PROPERTY_ACCESSOR]
        throw_exception[type == PROPERTY_ACCESSOR_DECL && (type_count == attribute_count + 1) && LA(1) == TERMINATE]
        set_type[type, NONE, type == PROPERTY_ACCESSOR_DECL]

        set_int[real_type_count, type_count]

        // special case for ternary operator on its own
        throw_exception[LA(1) == COLON && qmark]

        // adjust specifier tokens to account for keyword async used as name (only for C#)
        set_int[specifier_count, token == ASYNC ? specifier_count - 1 : specifier_count]

        // adjust type tokens to eliminate for last left bracket (only for Java)
        set_int[type_count, endbracket ? type_count - 1 : type_count]

        // have a sequence of type tokens, last one is function/variable name
        // (except for function pointer, which is handled later)
        set_int[type_count, type_count > 1 ? type_count - 1 : 0]

        // special case for what looks like a destructor declaration
        throw_exception[isdestructor && (modifieroperator || (type_count - specifier_count - attribute_count) > 1 || ((type_count - specifier_count - attribute_count) == 1))]

        /*
          We have a declaration (at this point a variable) if we have:

            - At least one non-specifier in the type
            - There is nothing in the type (what was the name is the type)
              and it is part of a parameter list
        */
        set_type[type, VARIABLE, (((type_count - specifier_count > 0 && (!inMode(MODE_ACCESS_REGION) || LA(1) == TERMINATE || LA(1) == COMMA || LA(1) == BAR || LA(1) == LBRACKET ||
                                              ((inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_C)) && LA(1) == EQUAL)))) ||
                                 (inparam && (LA(1) == RPAREN || LA(1) == COMMA || LA(1) == BAR || LA(1) == LBRACKET ||

                                              ((inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_C)) && LA(1) == EQUAL))))]

        // need to see if we possibly have a constructor/destructor name, with no type
        set_bool[isconstructor,

                 // operator methods may not have non-specifier types also
                 !isoperator &&

                 !isdestructor &&

                 // entire type is specifiers
                 (type_count == (specifier_count + attribute_count)) &&

                 (
                    // inside of a C++ class definition
                    inMode(MODE_ACCESS_REGION) ||

                    // right inside the block of a Java or C# class
                    (inPrevMode(MODE_CLASS) && (inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP))) ||

                    // by itself, but has specifiers so is not a call
                    (specifier_count > 0 && (inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP))) ||

                    // outside of a class definition in C++, but with properly prefixed name
                    (inLanguage(LANGUAGE_CXX_FAMILY) && namestack[0] != "" && namestack[0] == namestack[1])
                )
        ]

        // detecting a destructor name uses a data member, since it is detected in during the
        // name detection.  If the parameters use this method, it is reentrant, so cache it
        set_bool[saveisdestructor, isdestructor]

        // we have a declaration, so do we have a function?
        (
            // check for function pointer, which must have a non-specifier part of the type
            { (inLanguage(LANGUAGE_C) || inLanguage(LANGUAGE_CXX_ONLY)) && real_type_count > 0 }?
            (function_pointer_name_grammar eat_optional_macro_call LPAREN)=>
            function_pointer_name_grammar

            // what was assumed to be the name of the function is actually part of the type
            set_int[type_count, type_count + 1]

            // this ain't a constructor
            set_bool[isconstructor, false]

            function_rest[fla] |

            // POF (Plain Old Function)
            // need at least one non-specifier in the type (not including the name)
            { (type_count - specifier_count > 0) || isoperator || saveisdestructor || isconstructor}?
            function_rest[fla]
        )

        // since we got this far, we have a function
        set_type[type, FUNCTION]

        // however, we could have a destructor
        set_type[type, DESTRUCTOR, saveisdestructor]

        // could also have a constructor
        set_type[type, CONSTRUCTOR, isconstructor && !saveisdestructor && !isoperator]
)
;

// C# global attribute target
check_global_attribute[] returns [bool flag] {
        const std::string& s = LT(1)->getText();

        flag = s == "module" || s == "assembly";
} :;

/*
  Utility rules

  Work even in guessing mode, which explicit code segments cannot
*/

/* Throws an exception if the condition is true */
throw_exception[bool condition = true] { if (condition) throw antlr::RecognitionException(); } :;

/* sets the declaration type to a value if the condition is true */
set_type[STMT_TYPE& name, STMT_TYPE value, bool condition = true] { if (condition) name = value; } :;

/* sets the int to a value if the condition is true */
set_int[int& name, int value, bool condition = true] { if (condition) name = value; } :;

/* sets the bool to a value */
set_bool[bool& variable, bool value = true] { variable = value; } :;

trace[const char*s ] { std::cerr << s << std::endl; } :;

/*
trace_int[int s] { std::cerr << "HERE " << s << std::endl; } :;
traceLA { std::cerr << "LA(1) is " << LA(1) << " " << LT(1)->getText() << std::endl; } :;
marker[] { CompleteElement element(this); startNewMode(MODE_LOCAL); startElement(SMARKER); } :;
*/

function_rest[int& fla] { ENTRY_DEBUG } :

        eat_optional_macro_call

        parameter_list function_tail check_end[fla]
;

// function type, including specifiers
function_type[int type_count] { ENTRY_DEBUG } :
        {
            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_EAT_TYPE);

            setTypeCount(type_count);

            // type element begins
            startElement(STYPE);
        }
        (options { greedy = true; } : TYPENAME)* lead_type_identifier

        { 
            decTypeCount();
            if(inTransparentMode(MODE_ARGUMENT) && inLanguage(LANGUAGE_CXX_ONLY))
                return;
        }
        (options { greedy = true; } : {getTypeCount() > 0}? type_identifier { decTypeCount(); })*
        {
            endMode(MODE_EAT_TYPE);
            setMode(MODE_FUNCTION_NAME);
        }
;

update_typecount[State::MODE_TYPE mode] {} :
        {
            decTypeCount();
            if(inTransparentMode(MODE_ARGUMENT) && inLanguage(LANGUAGE_CXX_ONLY))
                return;

            if (getTypeCount() <= 0) {
                endMode();
                setMode(mode);
            }
        }
;

function_type_check[int& type_count] { type_count = 1; ENTRY_DEBUG } :

        lead_type_identifier
        ( { inLanguage(LANGUAGE_JAVA_FAMILY) || LA(1) != LBRACKET }? type_identifier_count[type_count])*
;

type_identifier_count[int& type_count] { ++type_count; ENTRY_DEBUG } :

        // overloaded parentheses operator
        { LA(1) == OPERATOR /* turns off ANTLR warning, and is nooped */ }?
        overloaded_operator |
        type_identifier |
        MAIN
;

type_identifier_count_check returns [int type_count] {


    int start = mark();
    ++inputState->guessing;

    type_count = type_identifier_count_check_core();

    rewind(start);
    --inputState->guessing;
} :;

type_identifier_count_check_core returns [int type_count] { type_count = 0; ENTRY_DEBUG } :

        (type_identifier_count[type_count])*

;

/*
deduct[int& type_count] { --type_count; } :;
*/

eat_type[int & count] { if (count <= 0 || LA(1) == BAR) return; ENTRY_DEBUG } :

        type_identifier

        set_int[count, count - 1]
        eat_type[count]

;

// throw list for a function
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
;

noexcept_list[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_EXPECT);

            startElement(SNOEXCEPT);
        }
        NOEXCEPT { if(LA(1) != LPAREN) endMode(); } (LPAREN)*
;

noexcept_operator[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_EXPECT);

            startElement(SNOEXCEPT);
        }
        NOEXCEPT { if(LA(1) != LPAREN) endMode(); } (options { greedy = true;} : LPAREN)*
;

complete_throw_list[] { ENTRY_DEBUG } :
        THROW paren_pair | THROWS ( options { greedy = true; } : compound_name_java | COMMA)*
;

complete_noexcept_list[] { ENTRY_DEBUG } :
        NOEXCEPT (options { greedy = true;} : paren_pair)*
;

// type identifier
pure_lead_type_identifier[] { ENTRY_DEBUG } :

        // specifiers that occur in a type
		{ _tokenSet_23.member(LA(1)) }?
        specifier |

        { inLanguage(LANGUAGE_CSHARP) && look_past(COMMA) == RBRACKET }?
        LBRACKET (COMMA)* RBRACKET |

        { inLanguage(LANGUAGE_JAVA) }? annotation |

        { inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |

        { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp |

        pure_lead_type_identifier_no_specifiers
;

pure_lead_type_identifier_no_specifiers[] { ENTRY_DEBUG } :

        // class/struct/union before a name in a type, e.g., class A f();
        TYPENAME | class_lead_type_identifier |

        // enum use in a type
        { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
        (ENUM variable_identifier (variable_identifier | multops | tripledotop | INLINE))=> ENUM |

        // entire enum definition
        { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
        enum_definition_complete |

        {inputState->guessing}? decltype_full | decltype_call

;

class_lead_type_identifier[]  { SingleElement element(this); ENTRY_DEBUG } :
        {
            if(inTransparentMode(MODE_TEMPLATE))
                startElement(SNAME);
            else
                startElement(SNOP);
        }
        (CLASS | STRUCT | UNION)
;

lead_type_identifier[] { ENTRY_DEBUG } :

//        specifier |

//        (macro_call_paren identifier)=> macro_call |

        // typical type name
        { LA(1) != ASYNC && (inLanguage(LANGUAGE_CXX_ONLY) || (LA(1) != FINAL && LA(1) != OVERRIDE)) }?
        compound_name |

        pure_lead_type_identifier
;

type_identifier[] { ENTRY_DEBUG } :

        // any identifier that can appear first can appear later
        // true suppresses warning.  antlr forms rules as LA(1) && (true )
        // so this does nothing.
        { true }? lead_type_identifier |

        non_lead_type_identifier
;

non_lead_type_identifier[] { bool iscomplex = false; ENTRY_DEBUG } :

        tripledotop |

        { inLanguage(LANGUAGE_C_FAMILY) }? multops |

        { inLanguage(LANGUAGE_JAVA_FAMILY) && look_past(LBRACKET) == RBRACKET }?
        variable_identifier_array_grammar_sub[iscomplex]
;

decltype_call[] { ENTRY_DEBUG} :
        {

            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_DECLTYPE);

            // start the macro call element
            startElement(SDECLTYPE);
         
        }
        DECLTYPE complete_argument_list
;

decltype_full[] { ENTRY_DEBUG } :
        DECLTYPE paren_pair
;

function_identifier[] { ENTRY_DEBUG } :

        // typical name
        compound_name_inner[false] |

        function_identifier_main |

        { inLanguage(LANGUAGE_CSHARP) }?
        function_identifier_default |

        // function pointer identifier with name marked separately
        function_pointer_name_grammar eat_optional_macro_call
;

qmark_marked[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        QMARK
;

function_identifier_default[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        DEFAULT
;

// special cases for main
function_identifier_main[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        MAIN
;

// overloaded operator name
overloaded_operator[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        set_bool[operatorname, true]
        OPERATOR
        (
            // special case for 'operator()'
            { LA(1) == LPAREN }? LPAREN RPAREN |

            // general operator name case is anything from 'operator', operators, or names
            (options { greedy = true; } : ~(LPAREN))*
        )
;

/* linq expressions */
linq_expression[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SLINQ);
        }
        linq_expression_pure
        (options { greedy = true; } : linq_expression_pure)*
;

linq_expression_pure[] { ENTRY_DEBUG } :
        linq_from | linq_where | linq_select | linq_let | linq_group | linq_join | linq_orderby
;

linq_from[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SFROM);
        }
        FROM linq_expression_complete (options { greedy = true; } : linq_in)*
;

linq_in[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SIN);
        }
        IN linq_expression_complete
;

linq_where[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SWHERE);
        }
        WHERE linq_expression_complete
;

linq_select[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SSELECT);
        }
        SELECT linq_expression_complete
;

linq_let[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SLET);
        }
        LET linq_expression_complete
;

linq_group[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SGROUP);
        }
        GROUP linq_expression_complete
        (options { greedy = true; } : linq_by)*
        (options { greedy = true; } : linq_into)*
;

linq_by[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SBY);
        }
        BY linq_expression_complete
;

linq_into[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SINTO);
        }
        INTO linq_expression_complete
;

linq_join[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SJOIN);
        }
        JOIN linq_expression_complete

        (options { greedy = true; } : linq_in | linq_on | linq_equals | linq_into)*
;

linq_on[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SON);
        }
        ON linq_expression_complete
;

linq_equals[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SEQUALS);
        }
        EQUALS linq_expression_complete
;

linq_orderby[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SORDERBY);
        }
        ORDERBY linq_expression_complete

        (options { greedy = true; } : linq_ascending | linq_descending)*

        (options { greedy = true; } : COMMA linq_expression_complete (options { greedy = true; } : linq_ascending | linq_descending)* )*
;

linq_ascending[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        ASCENDING
;

linq_descending[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        DESCENDING
;

variable_identifier_array_grammar_sub[bool& iscomplex] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            iscomplex = true;

            // start a mode to end at right bracket with expressions inside
            if (inLanguage(LANGUAGE_CSHARP))
                startNewMode(MODE_LOCAL | MODE_TOP | MODE_LIST | MODE_END_AT_COMMA);
            else
                startNewMode(MODE_LOCAL | MODE_TOP | MODE_LIST);

            startElement(SINDEX);
        }
        LBRACKET

        variable_identifier_array_grammar_sub_contents

        RBRACKET
;

variable_identifier_array_grammar_sub_contents{ ENTRY_DEBUG } :
        { !inLanguage(LANGUAGE_CSHARP) }? complete_expression |

        { inLanguage(LANGUAGE_CSHARP) }? (options { greedy = true; } : { LA(1) != RBRACKET }?
            ({ /* stop warning */ LA(1) == COMMA }? COMMA | complete_expression)
        )*
;

attribute_csharp[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT);

            startElement(SATTRIBUTE);
        }
        LBRACKET

        // do not warn as identifier list and colon are in complete expression as well, but need special processing here.
        (options { warnWhenFollowAmbig = false; } : { next_token() == COLON }? attribute_csharp_target COLON)*

        complete_expression

        RBRACKET
;

attribute_csharp_target[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(STARGET);
        }
        (RETURN | EVENT | identifier_list)
;

attribute_cpp[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT);

            startElement(SATTRIBUTE);
        }
        LBRACKET LBRACKET

        complete_expression

        RBRACKET RBRACKET
;

// Do a complete argument list
complete_argument_list[] { ENTRY_DEBUG } :
        call_argument_list complete_arguments
;

// Full, complete expression matched all at once (no stream).
// Colon matches range(?) for bits.
complete_arguments[] { CompleteElement element(this); int count_paren = 1; ENTRY_DEBUG } :
        { getParen() == 0 }? rparen[false] |
        { getCurly() == 0 }? rcurly_argument |
        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        }
        (options {warnWhenFollowAmbig = false; } : { count_paren > 0 }?
        ({ LA(1) == LPAREN }? expression { ++count_paren; } |

        { LA(1) == RPAREN }? expression { --count_paren; } |

        expression |
        comma
        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        }

        ))*

;

complete_expression[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }
        (options { greedy = true; } :

        // commas as in a list
        { inTransparentMode(MODE_END_ONLY_AT_RPAREN) || !inTransparentMode(MODE_END_AT_COMMA)}?
        comma |

        // right parentheses, unless we are in a pair of parentheses in an expression
        { !inTransparentMode(MODE_INTERNAL_END_PAREN) }? rparen[false] |

        // argument mode (as part of call)
        { inMode(MODE_ARGUMENT) }? argument |

        // expression with right parentheses if a previous match is in one
        { LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN) }? expression |

        COLON)*
;

linq_expression_complete[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }
        (options { greedy = true; } :

        // commas as in a list
        comma |

        // right parentheses, unless we are in a pair of parentheses in an expression
        { !inTransparentMode(MODE_INTERNAL_END_PAREN) }? rparen[false] |

        // argument mode (as part of call)
        { inMode(MODE_ARGUMENT) }? argument |

        // expression with right parentheses if a previous match is in one
        { LA(1) != ASCENDING && LA(1) != DESCENDING && LA(1) != ON && LA(1) != BY && LA(1) != FROM && LA(1) != SELECT && LA(1) != LET && LA(1) != WHERE && LA(1) != ORDERBY && LA(1) != GROUP && LA(1) != JOIN && LA(1) != IN && LA(1) != EQUALS && LA(1) != INTO && (LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN)) }? expression_setup_linq |

        COLON)*
;

// variable name in an expression.  Includes array names, but not function calls
variable_identifier[] { ENTRY_DEBUG } :
        compound_name
;

// name including template argument list
simple_name_optional_template[] { CompleteElement element(this); TokenPosition tp; ENTRY_DEBUG } :
        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            // start outer name
            startElement(SCNAME);

            // record the name token so we can replace it if necessary
            setTokenPosition(tp);
        }
        push_namestack identifier (
            { inLanguage(LANGUAGE_CXX_FAMILY) || inLanguage(LANGUAGE_JAVA_FAMILY) }?
            (template_argument_list)=>
                template_argument_list |

            {
               // set the token to NOP since we did not find a template argument list
               tp.setType(SNOP);
            }
       )
;

identifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
                startElement(SNAME);
        }
        identifier_list
;

identifier_list[] { ENTRY_DEBUG } :
            NAME | INCLUDE | DEFINE | ELIF | ENDIF | ERRORPREC | IFDEF | IFNDEF | LINE | PRAGMA | UNDEF |
            SUPER | CHECKED | UNCHECKED | REGION | ENDREGION | GET | SET | ADD | REMOVE | ASYNC | YIELD |
            SIGNAL | FINAL | OVERRIDE |

            // C# linq
            FROM | WHERE | SELECT | LET | ORDERBY | ASCENDING | DESCENDING | GROUP | BY | JOIN | ON | EQUALS |
            INTO | THIS
;

// most basic name
simple_identifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        NAME
;

compound_name[] { CompleteElement element(this); bool iscompound = false; ENTRY_DEBUG } :
        compound_name_inner[true]
        (options { greedy = true; } : {(!inLanguage(LANGUAGE_CXX_ONLY) || next_token() != LBRACKET)}? variable_identifier_array_grammar_sub[iscompound] |
        { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp)*

;

compound_name_inner[bool index] { CompleteElement element(this); TokenPosition tp; bool iscompound = false; ENTRY_DEBUG } :
        {
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
            setTokenPosition(tp);
        }
        (
        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        compound_name_java[iscompound] |

        { inLanguage(LANGUAGE_CSHARP) }?
        compound_name_csharp[iscompound] |

        { inLanguage(LANGUAGE_C) }?
        compound_name_c[iscompound] |

        { !inLanguage(LANGUAGE_JAVA_FAMILY) && !inLanguage(LANGUAGE_C) && !inLanguage(LANGUAGE_CSHARP) }?
        compound_name_cpp[iscompound]
        )

        (options { greedy = true; } : { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp)*

        (options { greedy = true; } : { index && !inTransparentMode(MODE_EAT_TYPE) && (!inLanguage(LANGUAGE_CXX_ONLY) || next_token() != LBRACKET)}?
            variable_identifier_array_grammar_sub[iscompound]
        )*

        {
            // if it isn't a compound name, nop the element
            if (!iscompound)
                // set the token to NOP
                tp.setType(SNOP);
        }
;

compound_name_cpp[bool& iscompound = BOOL] { namestack[0] = namestack[1] = ""; ENTRY_DEBUG } :

        (dcolon { iscompound = true; })*
        (DESTOP set_bool[isdestructor] { iscompound = true; })*
        (simple_name_optional_template | push_namestack overloaded_operator)
        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multops)*

        // "a::" causes an exception to be thrown
        ( options { greedy = true; } :
            (dcolon { iscompound = true; } | period { iscompound = true; })
            ( options { greedy = true; } : dcolon)*
            (DESTOP set_bool[isdestructor])*
            (multops)*
            (simple_name_optional_template | push_namestack overloaded_operator | function_identifier_main)
            (options { greedy = true; } : { look_past_multiple(MULTOPS, REFOPS, RVALUEREF) == DCOLON }? multops)*
        )*

        { notdestructor = LA(1) == DESTOP; }
;
exception
catch[antlr::RecognitionException] {
}

compound_name_csharp[bool& iscompound = BOOL] { namestack[0] = namestack[1] = ""; ENTRY_DEBUG } :

        (dcolon { iscompound = true; })*
        (DESTOP set_bool[isdestructor] { iscompound = true; })*
        (simple_name_optional_template | push_namestack overloaded_operator)
        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multops)*

        // "a::" causes an exception to be thrown
        ( options { greedy = true; } :
            (dcolon { iscompound = true; } | period { iscompound = true; })
            ( options { greedy = true; } : dcolon)*
            (multops)*
            (DESTOP set_bool[isdestructor])*
            (simple_name_optional_template | push_namestack overloaded_operator | function_identifier_main)
            (options { greedy = true; } : multops)*
        )*
;
exception
catch[antlr::RecognitionException] {
}

compound_name_c[bool& iscompound = BOOL] { ENTRY_DEBUG } :

        identifier
        ( options { greedy = true; } :
            period { iscompound = true; }
            identifier
        )*
;

compound_name_java[bool& iscompound = BOOL] { ENTRY_DEBUG } :

        template_argument_list |
        simple_name_optional_template
        (options { greedy = true; } : (period { iscompound = true; } simple_name_optional_template))*
;

function_specifier[] { CompleteElement element(this); ENTRY_DEBUG } :
        { LA(1) == WHERE }? generic_type_constraint |

        ({ LA(1) != ASYNC }? specifier |

        // pure virtual specifier = default and = delete
        EQUAL (literal | function_equal_specifier) |

        simple_name_optional_template)
;

function_equal_specifier[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup strings in literal option
            startElement(SFUNCTION_SPECIFIER);
        }
        (

            DEFAULT | DELETE

        )

;

specifier[] { ENTRY_DEBUG } :
        single_keyword_specifier | alignas_specifier
;

single_keyword_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        (
            // access
            PUBLIC | PRIVATE | PROTECTED |

            // C++
            FINAL | STATIC | ABSTRACT | FRIEND | { inLanguage(LANGUAGE_CSHARP) }? NEW | MUTABLE |
            CONSTEXPR | THREADLOCAL |

            // C# & Java
            INTERNAL | SEALED | OVERRIDE | REF | OUT | IMPLICIT | EXPLICIT | UNSAFE | READONLY | VOLATILE |
            DELEGATE | PARTIAL | EVENT | ASYNC | VIRTUAL | EXTERN | INLINE | IN | PARAMS |
            { inLanguage(LANGUAGE_JAVA) }? (SYNCHRONIZED | NATIVE | STRICTFP | TRANSIENT) |

            CONST
        )
;

alignas_specifier[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL | MODE_ARGUMENT);

            startElement(SALIGNAS);
        }
        ALIGNAS

        ({ inputState->guessing }? paren_pair | 

        complete_argument_list)

;

constructor_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the constructor declaration
            startElement(SCONSTRUCTOR_DECLARATION);
        }
        constructor_header
;

constructor_definition[] { ENTRY_DEBUG } :
        {
            // statement with nested block
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the construction definition
            startElement(SCONSTRUCTOR_DEFINITION);
        }
        constructor_header

        ({ inLanguage(LANGUAGE_CXX_FAMILY) }? try_statement)*

        ({ inLanguage(LANGUAGE_CXX_FAMILY) }? member_initialization_list)*
;

constructor_header[] { ENTRY_DEBUG } :

        (options { greedy = true; } :

            { inLanguage(LANGUAGE_JAVA) }? annotation |

            { inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |

            { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp |

            specifier |

            { inLanguage(LANGUAGE_JAVA_FAMILY) }? template_argument_list
        )*
        compound_name_inner[false]
        parameter_list
        {
            setMode(MODE_FUNCTION_TAIL);
        }
;

member_initialization_list[] { ENTRY_DEBUG } :
        {
            // handle member initialization list as a list of calls
            startNewMode(MODE_LIST | MODE_CALL);

            startElement(SMEMBER_INITIALIZATION_LIST);
        }
        COLON
;

push_namestack[] { namestack[1].swap(namestack[0]); namestack[0] = LT(1)->getText(); } :;

identifier_stack[std::string s[]] { s[1].swap(s[0]); s[0] = LT(1)->getText(); ENTRY_DEBUG } :
        identifier
;

destructor_definition[] { ENTRY_DEBUG } :
        {
            // statement with nested block
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the destructor definition
            startElement(SDESTRUCTOR_DEFINITION);
        }
        destructor_header
;

destructor_declaration[] { ENTRY_DEBUG } :
        {
            // just a statement
            startNewMode(MODE_STATEMENT);

            // start the destructor declaration
            startElement(SDESTRUCTOR_DECLARATION);
        }
        destructor_header
;

destructor_header[] { ENTRY_DEBUG } :

        (options { greedy = true; } :

            { inLanguage(LANGUAGE_JAVA) }? annotation |

            { inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |

            { inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp |

            specifier |

            // @todo  'void' should be detected in lexer
            { LT(1)->getText() == "void" }? simple_identifier
        )*
        compound_name_inner[false]
        parameter_list
        {
            setMode(MODE_FUNCTION_TAIL);
        }
;

interface_annotation[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // start the function call element
            startElement(SANNOTATION);
        }
        ATSIGN INTERFACE

;

annotation[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SANNOTATION);
        }
        ATSIGN

        function_identifier

        // warnings seem to be caused by antlr ()* ending the rules.
        // first greedy eliminates LPAREN LCURLY
        (options { greedy = true; } : call_argument_list 
        // second greedy get rid of rparen
        (options { greedy = true; } : { LA(1) != RPAREN && LA(1) != COMMA }? annotation_argument 
        // third greedy gets rid of comma
        (options { greedy = true; } : comma)*)* rparen)*
;

// call  function call, macro, etc.
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

call_argument_list[] { ENTRY_DEBUG } :
        {
            // list of parameters
            setMode(MODE_EXPECT | MODE_LIST | MODE_INTERNAL_END_PAREN | MODE_END_ONLY_AT_RPAREN);

            // start the argument list
            startElement(SARGUMENT_LIST);
        }
        (LPAREN | { setMode(MODE_INTERNAL_END_CURLY); } LCURLY)
;

sizeof_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SSIZEOF_CALL);
        }
        SIZEOF
        (DOTDOTDOT)*
        call_argument_list
;

macro_call_check[] { ENTRY_DEBUG } :
        NAME (paren_pair)*
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
                endMode(MODE_THEN);
        }
;

macro_call_inner[] { CompleteElement element(this); bool first = true; ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);
        }
        identifier
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
            endMode(MODE_LIST | MODE_TOP);
        } set_bool[first, false] )*
;
exception
catch[antlr::RecognitionException] {

        // no end found to macro
        if (isoption(parseoptions, OPTION_DEBUG))
            emptyElement(SERROR_PARSE);
}

macro_pattern_name[]  { SingleElement element(this); ENTRY_DEBUG } :
        {

            startElement(SNAME);

        }
        MACRO_NAME
;

macro_pattern_call[] { ENTRY_DEBUG } :

        macro_pattern_call_inner
/*        {
            if (inMode(MODE_THEN) && LA(1) == ELSE)
                endMode(MODE_THEN);
        }
*/
;

macro_pattern_call_inner[] { CompleteElement element(this); bool first = true; ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);
        }
        macro_pattern_name
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
            endMode(MODE_LIST | MODE_TOP);
        } 
        set_bool[first, false] )*
;
exception
catch[antlr::RecognitionException] {

        // no end found to macro
        if (isoption(parseoptions, OPTION_DEBUG))
            emptyElement(SERROR_PARSE);
}

macro_call_contents[] {

    ENTRY_DEBUG

    CompleteElement element(this);

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
            endMode();
            start = true;
        }
        consume();
    }

} :;

try_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TRY);

            // start of the try statement
            startElement(STRY_BLOCK);
        }
        TRY
;

try_statement_with_resource[] {  int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TRY);

            // start of the try statement
            startElement(STRY_BLOCK);

            // expect a condition to follow the keyword
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPECT | MODE_INTERNAL_END_PAREN);
        }
        TRY LPAREN
        (
            // explicitly check for a variable declaration since it can easily
            // be confused with an expression
            { pattern_check(stmt_type, secondtoken, type_count) && stmt_type == VARIABLE }?
            for_initialization_variable_declaration[type_count] |

            {
                // use a new mode without the expect so we don't nest expression parts
                startNewMode(MODE_EXPRESSION);

                // start the expression element
                startElement(SEXPRESSION);
            }
            // explicitly check for non-terminate so that a large switch statement
            // isn't needed
            expression
        )

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

unsafe_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SUNSAFE_STATEMENT);
        }
        UNSAFE
;

using_namespace_statement[] { ENTRY_DEBUG } :

        { inLanguage(LANGUAGE_CSHARP) && next_token() == LPAREN }?
        using_statement |

        namespace_directive
;

using_statement[] { int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SUSING_DIRECTIVE);

            // expect a condition to follow the keyword
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPECT | MODE_INTERNAL_END_PAREN);
        }
        USING LPAREN
        (
            // explicitly check for a variable declaration since it can easily
            // be confused with an expression
            { pattern_check(stmt_type, secondtoken, type_count) && stmt_type == VARIABLE }?
            for_initialization_variable_declaration[type_count] |

            {
                // use a new mode without the expect so we don't nest expression parts
                startNewMode(MODE_EXPRESSION);

                // start the expression element
                startElement(SEXPRESSION);
            }
            // explicitly check for non-terminate so that a large switch statement
            // isn't needed
            expression
        )
;

lock_statement[] { int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SLOCK_STATEMENT);

            // expect a condition to follow the keyword
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPECT | MODE_INTERNAL_END_PAREN);
        }
        LOCK LPAREN
        (
            // explicitly check for a variable declaration since it can easily
            // be confused with an expression
            { pattern_check(stmt_type, secondtoken, type_count) && stmt_type == VARIABLE }?
            for_initialization_variable_declaration[type_count] |

            {
                // use a new mode without the expect so we don't nest expression parts
                startNewMode(MODE_EXPRESSION);

                // start the expression element
                startElement(SEXPRESSION);
            }
            // explicitly check for non-terminate so that a large switch statement
            // isn't needed
            expression
        )
;

synchronized_statement[] { int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SSYNCHRONIZED_STATEMENT);

            // expect a condition to follow the keyword
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPECT | MODE_INTERNAL_END_PAREN);
        }
        SYNCHRONIZED LPAREN
        (
            // explicitly check for a variable declaration since it can easily
            // be confused with an expression
            { pattern_check(stmt_type, secondtoken, type_count) && stmt_type == VARIABLE }?
            for_initialization_variable_declaration[type_count] |

            {
                // use a new mode without the expect so we don't nest expression parts
                startNewMode(MODE_EXPRESSION);

                // start the expression element
                startElement(SEXPRESSION);
            }
            // explicitly check for non-terminate so that a large switch statement
            // isn't needed
            expression
        )
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
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the catch statement
            startElement(SCATCH_BLOCK);
        }
        CATCH
        {
            // parameter list is unmarked with a single parameter
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
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the catch statement
            startElement(SFINALLY_BLOCK);
        }
        FINALLY
;

lambda_anonymous[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_ANONYMOUS);
        }
        lambda_marked

        /* completely parse a function until it is done */
        (options { greedy = true; } : { inputState->guessing }? curly_pair)*
;

delegate_anonymous[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_ANONYMOUS);

            // start of the catch statement
            startElement(SFUNCTION_DEFINITION);
        }
        delegate_marked
        (options { greedy = true; } : parameter_list)*

        /* completely parse a function until it is done */
        (options { greedy = true; } : { inputState->guessing }? curly_pair)*

;

delegate_marked[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        DELEGATE
;

lambda_marked[] { ENTRY_DEBUG } :
        LAMBDA
;

fixed_statement[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the catch statement
            startElement(SFIXED_STATEMENT);
        }
        FIXED
        {
            // looking for a LPAREN
            if (LA(1) == LPAREN)
                parameter_list();
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

variable_declaration_statement[int type_count] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            if (!inTransparentMode(MODE_INNER_DECL) || inTransparentMode(MODE_CLASS))
                // start the declaration statement
                startElement(SDECLARATION_STATEMENT);

            // declaration
            startNewMode(MODE_LOCAL);

            if (!inTransparentMode(MODE_INNER_DECL) || inTransparentMode(MODE_CLASS))
                // start the declaration
                startElement(SDECLARATION);
        }
        variable_declaration[type_count]
;

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

variable_declaration[int type_count] { ENTRY_DEBUG } :
        {
            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);
        }
        variable_declaration_type[type_count]
;

variable_declaration_type[int type_count] { ENTRY_DEBUG } :
        {
            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_EAT_TYPE);

            setTypeCount(type_count);

            // type element begins
            startElement(STYPE);
        }
        lead_type_identifier { if(!inTransparentMode(MODE_TYPEDEF)) decTypeCount(); } 
        (options { greedy = true; } : { !inTransparentMode(MODE_TYPEDEF) && getTypeCount() > 0 }? type_identifier { decTypeCount(); })* 
        update_typecount[MODE_VARIABLE_NAME | MODE_INIT]
;

// Variable declaration name and optional initialization
variable_declaration_nameinit[] { bool isthis = LA(1) == THIS;
        ENTRY_DEBUG } :
        ({ inLanguage(LANGUAGE_CSHARP) }? compound_name_inner[false] | compound_name)
        {
            // expect a possible initialization
            setMode(MODE_INIT | MODE_EXPECT);

            if (isthis && LA(1) == LBRACKET) {

                indexer_parameter_list();

                endDownToMode(MODE_LIST);

                match(RBRACKET);

                endMode();
                endMode();
            }
        }
;

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
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startElement(SDECLARATION_INITIALIZATION);
        } IN |
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);
        }
        call_argument_list
;

variable_declaration_range[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startElement(SDECLARATION_RANGE);
        }
        COLON
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
            replaceMode(MODE_STATEMENT | MODE_NEST, MODE_BLOCK | MODE_NEST | MODE_END_AT_BLOCK_NO_TERMINATE);

            // end this expression block correctly
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT);
        }
;

// All possible operators
general_operators[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        (
            OPERATORS | TRETURN | TEMPOPS |
            TEMPOPE ({ SkipBufferSize() == 0 }? TEMPOPE)? ({ SkipBufferSize() == 0 }? TEMPOPE)? ({ SkipBufferSize() == 0 }? EQUAL)? |
            EQUAL | /*MULTIMM |*/ DESTOP | /* MEMBERPOINTER |*/ MULTOPS | REFOPS | DOTDOT | RVALUEREF |
            QMARK ({ SkipBufferSize() == 0 }? QMARK)? | { inLanguage(LANGUAGE_JAVA) }? BAR |

            // others are not combined
            NEW | DELETE | IN | IS | STACKALLOC | AS | AWAIT | LAMBDA
        )
;

sole_new[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        NEW
;

sole_destop[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        DESTOP
;

general_operators_list[] { ENTRY_DEBUG } :
        OPERATORS | TEMPOPS | TEMPOPE | EQUAL | /*MULTIMM |*/ DESTOP | /* MEMBERPOINTER |*/ MULTOPS | REFOPS |
        DOTDOT | RVALUEREF | QMARK
;

rparen_operator[bool markup = true] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (markup && isoption(parseoptions, OPTION_OPERATOR) && !inMode(MODE_END_ONLY_AT_RPAREN))
                startElement(SOPERATOR);
        }
        RPAREN
    ;

rparen[bool markup = true] { bool isempty = getParen() == 0; bool update_type = false; ENTRY_DEBUG } :
        {
            if (isempty) {

                // additional right parentheses indicates end of non-list modes
                endDownToModeSet(MODE_LIST | MODE_PREPROC | MODE_END_ONLY_AT_RPAREN | MODE_ONLY_END_TERMINATE);

                // special case:  Get to here, in for-initalization.  Need an extra end mode
                if (inMode(MODE_VARIABLE_NAME) && inTransparentMode(MODE_FOR_CONDITION))
                    endDownToModeSet(MODE_FOR_CONDITION);

                // don't markup since not a normal operator
                markup = false;

            } else

                decParen();
        }
        rparen_operator[markup]
        {
            if (isempty) {

                // special handling for then part of an if statement
                // only when in a condition of an if statement
                if (inMode(MODE_CONDITION) && inPrevMode(MODE_IF)) {

                    // end the condition
                    endMode(MODE_CONDITION);

                    // then part of the if statement (after the condition)
                    startNewMode(MODE_STATEMENT | MODE_NEST | MODE_THEN);

                    // start the then element
                    startNoSkipElement(STHEN);
                }

                // end the single mode that started the list
                // don't end more than one since they may be nested
                update_type = inMode(MODE_DECLTYPE);
                if (inMode(MODE_LIST))
                    endMode(MODE_LIST);
            }

            if(update_type) {

                if(!inTransparentMode(MODE_VARIABLE_NAME)) {
                    while(getTypeCount() - 1 > 0) {
                        type_identifier();
                        decTypeCount();
                    }
                    endMode(MODE_EAT_TYPE);
                    setMode(MODE_FUNCTION_NAME);
                } else
                    update_typecount(MODE_VARIABLE_NAME | MODE_INIT);
            }
        }
;

rcurly_argument[] { bool isempty = getCurly() == 0; ENTRY_DEBUG } :
        {
            if(isempty) {

                // additional right parentheses indicates end of non-list modes
                endDownToModeSet(MODE_LIST | MODE_PREPROC | MODE_END_ONLY_AT_RPAREN | MODE_ONLY_END_TERMINATE | MODE_INTERNAL_END_CURLY);
            }

        }
        RCURLY
        {

            // end the single mode that started the list
            // don't end more than one since they may be nested
            if (isempty && inMode(MODE_LIST))
                endDownOverMode(MODE_LIST);
            
            else if(inTransparentMode(MODE_EXPRESSION | MODE_LIST))
                endDownOverMode(MODE_EXPRESSION | MODE_LIST);

            if(!isempty)
                decCurly();
        }

;

// Dot (period) operator
period[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        PERIOD
;

// Namespace operator '::'
dcolon[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        DCOLON
;

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

        expression_part_plus_linq[type]
;

expression_setup_linq[CALLTYPE type = NOCALL] { ENTRY_DEBUG } :

        expression_process

        expression_part[type]
;

expression_part_plus_linq[CALLTYPE type = NOCALL] { ENTRY_DEBUG } :

        { inLanguage(LANGUAGE_CSHARP) && next_token() != RPAREN && next_token_string().find('=') == std::string::npos }?
        (linq_expression_pure)=> linq_expression |

        expression_part[type]
;

expression_part[CALLTYPE type = NOCALL] { bool flag; ENTRY_DEBUG } :

        // cast
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        UNION |

        // cast
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        CLASS |

        { next_token() == LPAREN }?
        delegate_anonymous |

        { next_token() == LCURLY }?
        lambda_anonymous |

        (LBRACKET (~RBRACKET)* RBRACKET (LPAREN | LCURLY)) => lambda_expression_cpp |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW template_argument_list)=> sole_new template_argument_list |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW function_identifier paren_pair LCURLY)=> sole_new anonymous_class_definition |

        { notdestructor }? sole_destop { notdestructor = false; } |

        // call
        // distinguish between a call and a macro
        { type == CALL || (perform_call_check(type, -1) && type == CALL) }?

            // Added argument to correct markup of default parameters using a call.
            // normally call claims left paren and start calls argument.
            // however I believe parameter_list matches a right paren of the call.
           (call | sizeof_call) argument |
            
        // macro call
        { type == MACRO }? macro_call |

        // general math operators
        // looks like general operators and variable identifier can match same thing
        (options { generateAmbigWarnings = false; } : general_operators
        {
            if (inLanguage(LANGUAGE_CXX_FAMILY) && LA(1) == DESTOP)
                general_operators();
        }
        | /* newop | */ period |

        // left parentheses
        lparen_marked
        {
            startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);
        } |

        // right parentheses that only matches a left parentheses of an expression
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        {
            // stop at this matching paren, or a preprocessor statement
            endDownToModeSet(MODE_INTERNAL_END_PAREN | MODE_PREPROC);
            
            if (inMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN))
                endMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);
        }

        // treat as operator for operator markup
        rparen[true] |

        // left curly brace
        {
            startNewMode(MODE_EXPRESSION | MODE_LIST);

            startElement(SBLOCK);
        }
        LCURLY
        {
            incCurly();
            startNewMode(MODE_EXPRESSION | MODE_EXPECT | MODE_LIST | MODE_INTERNAL_END_CURLY);
        } |
        { inTransparentMode(MODE_INTERNAL_END_CURLY) }?
        {

            if(!inTransparentMode(MODE_CALL) && !inTransparentMode(MODE_INIT)) {
                endDownToMode(MODE_INTERNAL_END_CURLY);

                endMode(MODE_INTERNAL_END_CURLY);

            }
        }
        rcurly_argument |

        // variable or literal
        variable_identifier) | literals | noexcept_operator | 

        variable_identifier_array_grammar_sub[flag]
;

expression_part_default[CALLTYPE type = NOCALL] { ENTRY_DEBUG } :

        expression_process

        call argument
;

// rule for literals
literals[] { ENTRY_DEBUG } :
        string_literal | char_literal | literal | boolean
;

// Only start and end of strings are put directly through the parser.
// The contents of the string are handled as whitespace.
string_literal[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup strings in literal option
            if (isoption(parseoptions, OPTION_LITERAL))
                startElement(SSTRING);
        }
        (STRING_START STRING_END)
;

// Only start and end of character are put directly through the parser.
// The contents of the character are handled as whitespace.
char_literal[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup characters in literal option
            if (isoption(parseoptions, OPTION_LITERAL))
                startElement(SCHAR);
        }
        (CHAR_START CHAR_END)
;

literal[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup literals in literal option
            if (isoption(parseoptions, OPTION_LITERAL))
                startElement(SLITERAL);
        }
        (CONSTANTS | NULLPTR)
;

boolean[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup boolean values in literal option
            if (isoption(parseoptions, OPTION_LITERAL))
                startElement(SBOOLEAN);
        }
        (TRUE | FALSE)
;

derived[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SDERIVATION_LIST);
        }
        COLON
        (options { greedy = true; } :
            { LA(1) != WHERE }? (
            (derive_access)*

            variable_identifier
            ({ inLanguage(LANGUAGE_CSHARP) }? period variable_identifier)*

            (options { greedy = true; } : template_argument_list)*
            )
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

extends_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SEXTENDS);
        }
        EXTENDS
        super_list
;

implements_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SIMPLEMENTS);
        }
        IMPLEMENTS
        super_list
;

super_list[] { ENTRY_DEBUG } :
        (options { greedy = true; } :
            (derive_access)*

            compound_name_java
        |
            COMMA
        )*
;

derive_access[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCLASS_SPECIFIER);
        }
        (VIRTUAL)* (PUBLIC | PRIVATE | PROTECTED) (options { greedy = true; } : VIRTUAL)*
;

parameter_list[] { CompleteElement element(this); bool lastwasparam = false; bool foundparam = false; ENTRY_DEBUG } :
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
                endMode();
        } comma | { inLanguage(LANGUAGE_JAVA) }? bar |
        complete_parameter { foundparam = lastwasparam = true; })* empty_element[SPARAMETER, !lastwasparam && foundparam] rparen[false]
;

indexer_parameter_list[] { bool lastwasparam = false; ENTRY_DEBUG } :
        {
            // list of parameters
            startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);

            // start the parameter list element
            startElement(SPARAMETER_LIST);
        }
        // parameter list must include all possible parts since it is part of
        // function detection
        LBRACKET
        { startNewMode(MODE_LIST); }
        ({ if (!lastwasparam) empty_element(SPARAMETER, !lastwasparam); lastwasparam = false; }
        {
            // We are in a parameter list.  Need to make sure we end it down to the start of the parameter list
//            if (!inMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT))
//                endMode();
        } comma |

        complete_parameter { lastwasparam = true; })*
;

empty_element[int ele, bool cond] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (cond)
                startElement(ele);
        }
;

kr_parameter[int type_count] { ENTRY_DEBUG } :
        kr_parameter_type[type_count] kr_parameter_name kr_parameter_terminate
        //complete_parameter terminate_pre terminate_token
;

kr_parameter_type[int type_count] { ENTRY_DEBUG} :
        // suppress ()* warning
        variable_declaration_statement[type_count] (options { greedy = true; } : { inMode(MODE_EAT_TYPE) }? type_identifier update_typecount[MODE_FUNCTION_NAME])* 
;

kr_parameter_name[] { ENTRY_DEBUG } :
        ((comma)* variable_declaration_nameinit)*
;

kr_parameter_terminate[] { ENTRY_DEBUG } :
    terminate_pre terminate_token { endDownToModeSet(MODE_FUNCTION_TAIL); }
;

complete_parameter[] { ENTRY_DEBUG } :
        parameter
        // suppress ()* warning
        (options { greedy = true; } : parameter_declaration_initialization (options { greedy = true; } : {LA(1) != RPAREN }? expression)*)*
;

argument[] { ENTRY_DEBUG } :
        { getParen() == 0 }? rparen[false] |
        { getCurly() == 0 }? rcurly_argument |
        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        }
        (
        { !((LA(1) == RPAREN && inTransparentMode(MODE_INTERNAL_END_PAREN)) || (LA(1) == RCURLY && inTransparentMode(MODE_INTERNAL_END_CURLY))) }? expression |

        type_identifier
        )

;

annotation_argument[] { ENTRY_DEBUG } :
        { getParen() == 0 }? rparen[false] |
        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        }
        // suppress warning of ()*
        (options { greedy = true; } :
        { !(LA(1) == RPAREN) }? expression |

        type_identifier
        )*
;

parameter[] { int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            // end parameter correctly
            startNewMode(MODE_PARAMETER);

            // start the parameter element
            startElement(SPARAMETER);
        }
        (

            { pattern_check(stmt_type, secondtoken, type_count, true) && stmt_type == FUNCTION }?
            function_declaration[type_count]

            function_identifier // pointer_name_grammar

            (macro_call_check)*

            parameter_list

            (options { greedy = true; } : function_pointer_initialization)* |
            {
                // start the declaration element
                startElement(SDECLARATION);

                if (stmt_type != VARIABLE)
                    type_count = 1;
            }
            { stmt_type == VARIABLE || LA(1) == DOTDOTDOT}?
            parameter_type_count[type_count]
            // suppress warning caused by ()*
            (options { greedy = true; } : { LA(1) == BAR }? bar set_int[type_count, type_count > 1 ? type_count - 1 : 1] parameter_type_count[type_count])*
            {
                // expect a name initialization
                setMode(MODE_VARIABLE_NAME | MODE_INIT);
            }
            ( options { greedy = true; } : variable_declaration_nameinit)*
        )
;

parameter_type_count[int & type_count] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);

            // start of type
            startElement(STYPE);
        }
        eat_type[type_count]

        // sometimes there is no parameter name.  if so, we need to eat it
        ( options { greedy = true; } : multops | tripledotop | LBRACKET RBRACKET)*
;

multops[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (isoption(parseoptions, OPTION_MODIFIER))
                startElement(SMODIFIER);
        }
        (MULTOPS | REFOPS | RVALUEREF | { inLanguage(LANGUAGE_CSHARP) }? QMARK set_bool[qmark, true])
;

tripledotop[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (isoption(parseoptions, OPTION_MODIFIER))
                startElement(SMODIFIER);
        }
        DOTDOTDOT
;

parameter_type[] { CompleteElement element(this); int type_count = 0; int secondtoken = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);

            // start of type
            startElement(STYPE);
        }
        { pattern_check(stmt_type, secondtoken, type_count) && (type_count ? type_count : (type_count = 1))}?
        eat_type[type_count]
;

// Template
template_declaration[] { ENTRY_DEBUG } :
        {
            // template with nested statement (function or class)
            // expect a template parameter list
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TEMPLATE);

            // start the template
            startElement(STEMPLATE);
        }
        (template_specifier)* TEMPLATE
        {
            if(LA(1) == CLASS)
                startNewMode(MODE_TEMPLATE | MODE_LIST | MODE_EXPECT);
            else
                startNewMode(MODE_TEMPLATE | MODE_LIST | MODE_EXPECT | MODE_TEMPLATE_PARAMETER_LIST);
        }
;

template_specifier{ SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        EXTERN
    ;

template_param_list[] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_PARAMETER | MODE_LIST);

            // start the template parameter list
            startElement(STEMPLATE_PARAMETER_LIST);
        }
        tempops
;

template_param[] { ENTRY_DEBUG } :
        {
            // end parameter correctly
            startNewMode(MODE_LOCAL);

            // start the parameter element
            startElement(STEMPLATE_PARAMETER);
        }

        // Both can contain extern however an extern template should not be a template param so should not be a problem
        (options { generateAmbigWarnings = false; } :
        parameter_type
        {
            // expect a name initialization
            setMode(MODE_VARIABLE_NAME | MODE_INIT);
        } |

        template_inner_full
    )
;

template_inner_full[] { ENTRY_DEBUG int type_count = 0; int secondtoken = 0; STMT_TYPE stmt_type = NONE; } :

        template_parameter_list_full
        { pattern_check(stmt_type, secondtoken, type_count) && (type_count ? type_count : (type_count = 1))}?
        eat_type[type_count]
        {
            endMode();

            // expect a name initialization
            setMode(MODE_VARIABLE_NAME | MODE_INIT);
        }

;

template_parameter_list_full[] { ENTRY_DEBUG } :

        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);
            
            // start of type
            startElement(STYPE);
        }

        template_declaration template_param_list template_param (template_declaration_initialization)* tempope { if(inMode(MODE_TEMPLATE)) endMode();}

;

template_declaration_initialization[] { ENTRY_DEBUG } :
        EQUAL
        {
            // end the init correctly
            setMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startNoSkipElement(SDECLARATION_INITIALIZATION);
        } compound_name

;

template_argument_list[] { CompleteElement element(this); std::string namestack_save[2]; ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(STEMPLATE_ARGUMENT_LIST);
        }
        savenamestack[namestack_save]

        tempops (options { generateAmbigWarnings = false; } : COMMA | template_argument)* tempope

        (options { greedy = true; } : generic_type_constraint)*

        restorenamestack[namestack_save]
;

generic_type_constraint[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SWHERE);
        }
        WHERE compound_name_inner[false] COLON
        (compound_name_inner[false] | CLASS | STRUCT | NEW LPAREN RPAREN)
        (options { greedy = true; } : COMMA (compound_name_inner[false] | CLASS | STRUCT | NEW LPAREN RPAREN))*
;

savenamestack[std::string namestack_save[]] { namestack_save[0].swap(namestack[0]); namestack_save[1].swap(namestack[1]); ENTRY_DEBUG } :;

restorenamestack[std::string namestack_save[]] { namestack[0].swap(namestack_save[0]); namestack[1].swap(namestack_save[1]); ENTRY_DEBUG } :;

template_argument[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(STEMPLATE_ARGUMENT);

            if(inLanguage(LANGUAGE_CXX_ONLY))
               startElement(SEXPRESSION);
        }
        (options { greedy = true; } :
        { LA(1) != SUPER && LA(1) != QMARK }?

        ((options { generateAmbigWarnings = false; } : { LA(1) != IN }? template_operators)*

        (type_identifier | literals)
            (options { generateAmbigWarnings = false; } : template_operators)*
            ) |

            template_extends_java |

            template_super_java | qmark_marked |
            template_argument_expression
        )+
;

template_argument_expression[] { ENTRY_DEBUG } :

        lparen_marked
        ({ LA(1) != RPAREN }? ({ true }? general_operators | (variable_identifier)=>variable_identifier | literals | type_identifier | template_argument_expression))*
       rparen_operator[true]

;

// All possible operators
template_operators[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (isoption(parseoptions, OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        (
        OPERATORS | TRETURN | TEMPOPS | EQUAL | MULTOPS | REFOPS | DOTDOT | RVALUEREF |
        QMARK | NEW | DELETE | IN | IS | STACKALLOC | AS | AWAIT | LAMBDA
        )
;

template_extends_java[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SEXTENDS);
        }
        EXTENDS
        compound_name_java
;


template_super_java[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SDERIVATION_LIST);
        }
        SUPER
        compound_name_java
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

tempope[] { ENTRY_DEBUG } :
        {
            // end down to the mode created by the start template operator
            endDownToMode(MODE_LIST);
        }
        TEMPOPE
        {
            // end the mode created by the start template operator
            while (inMode(MODE_LIST))
                endMode(MODE_LIST);
        }
;

label_statement[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the label element
            startElement(SLABEL_STATEMENT);
        }
        identifier COLON
;

typedef_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_EXPECT | MODE_VARIABLE_NAME | MODE_ONLY_END_TERMINATE);

            // start the typedef element
            startElement(STYPEDEF);

            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_INNER_DECL | MODE_TYPEDEF | MODE_END_AT_BLOCK_NO_TERMINATE);
        }
        TYPEDEF
;

paren_pair[] :
        LPAREN (paren_pair | ~(LPAREN | RPAREN))* RPAREN
;

curly_pair[] :
        LCURLY (curly_pair | ~(LCURLY | RCURLY))* RCURLY
;

// See if there is a semicolon terminating a statement inside a block at the top level
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
} :
        TERMINATE
;

enum_definition[] { ENTRY_DEBUG } :
        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (enum_class_definition nested_terminate)=>enum_class_definition |

        { inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP) }?
        {
            // statement
            // end init correctly
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION_BLOCK | MODE_VARIABLE_NAME | MODE_EXPECT | MODE_ENUM | MODE_END_AT_BLOCK);

            // start the enum definition element
            startElement(SENUM);
        }
        class_preamble
        ENUM |
        {
            // statement
            // end init correctly
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION_BLOCK | MODE_VARIABLE_NAME | MODE_EXPECT | MODE_ENUM);

            // start the enum definition element
            startElement(SENUM);
        }
        ENUM
;

enum_class_header[] {} :
        (CLASS | STRUCT)* 
        ({ inLanguage(LANGUAGE_CXX_ONLY) && next_token() == LBRACKET}? attribute_cpp)*
        variable_identifier (COLON enum_type)*

    ;

enum_type { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(STYPE);
        }
        // suppress warning compound_name seems to have some tokens in common with specifier.
        (options { generateAmbigWarnings = false; } : specifier | compound_name)*
    ;

// Complete definition of an enum.  Used for enum's embedded in typedef's where the entire
// enum must be parsed since it is part of the type.
enum_definition_complete[] { CompleteElement element(this); ENTRY_DEBUG } :
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
            endDownToMode(MODE_TOP);
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
            endAllModes();
        }
        EOF
;

/*
    Preprocessor

    Match on the directive itself not the entire directive
*/
preprocessor[] { ENTRY_DEBUG

        int directive_token = 0;
        bool markblockzero = false;

        TokenPosition tp;
        TokenPosition tp_directive;

        // parse end of line
        startNewMode(MODE_PARSE_EOL);

        // mode for any preprocessor elements
        startNewMode(MODE_PREPROC);
        } :

        {
            // assume error.  will set to proper one later
            startElement(SCPP_ERROR);

            setTokenPosition(tp);
        }
        PREPROC markend[directive_token]
        {

            startNewMode(MODE_LOCAL);

            startElement(SCPP_DIRECTIVE);
            setTokenPosition(tp_directive);
        }

        // Suppress warnings that should be caused by empty rule.
        (options { generateAmbigWarnings = false; } : 
        INCLUDE
        {
            endMode();

            tp.setType(SCPP_INCLUDE);
        }
        (cpp_filename)* |

        DEFINE
        {
            endMode();

            tp.setType(SCPP_DEFINE);
        }
        cpp_symbol_optional |

        IFNDEF
        {
            endMode();

            tp.setType(SCPP_IFNDEF);
        }
        cpp_symbol_optional |

        UNDEF
        {
            endMode();

            tp.setType(SCPP_UNDEF);
        }
        cpp_symbol_optional |

        IF
            { markblockzero = false; }
        {
            endMode();

            tp.setType(SCPP_IF);
        }
        cpp_condition[markblockzero] |

        ELIF
        {
            endMode();

            tp.setType(SCPP_ELIF);
        }
        cpp_condition[markblockzero] |

        ELSE
        {
            endMode();

            tp.setType(SCPP_ELSE);
        } |

        ENDIF
        {
            endMode();

            tp.setType(SCPP_ENDIF);
        } |

        IFDEF
        {
            endMode();

            tp.setType(SCPP_IFDEF);
        }
            cpp_symbol_optional |

        LINE
        {
            endMode();

            tp.setType(SCPP_LINE);
        }
            cpp_linenumber

            (cpp_filename)* |

        PRAGMA
        {
            endMode();

            tp.setType(SCPP_PRAGMA);
        } |

        ERRORPREC
        {
            endMode();

            tp.setType(SCPP_ERROR);
        } |

        NAME
        {
            endMode();

            tp.setType(SCPP_ERROR);
        } |

        REGION
        {
            endMode();

            tp.setType(SCPP_REGION);
        } |

        ENDREGION
        {
            endMode();

            tp.setType(SCPP_ENDREGION);
        } |

        /* blank preproc */
        // suppress ()* warning
        { tp_directive.setType(SNOP); endMode(); } (options { greedy = true; } : cpp_garbage)*

        )
        eol_skip[directive_token, markblockzero]
;
exception
catch[...] {
        eol_skip(directive_token, markblockzero);
}

cpp_garbage[] :

 ~(EOL | LINECOMMENT_START | COMMENT_START | JAVADOC_COMMENT_START | DOXYGEN_COMMENT_START | LINE_DOXYGEN_COMMENT_START | EOF)

;

eol_skip[int directive_token, bool markblockzero] {

    while (LA(1) != EOL &&
           LA(1) != LINECOMMENT_START &&
           LA(1) != COMMENT_START &&
           LA(1) != JAVADOC_COMMENT_START &&
           LA(1) != DOXYGEN_COMMENT_START &&
           LA(1) != LINE_DOXYGEN_COMMENT_START &&
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
            endDownOverMode(MODE_PREPROC);

            endMode(MODE_PARSE_EOL);
ENTRY_DEBUG } :
        (EOL | LINECOMMENT_START | COMMENT_START | JAVADOC_COMMENT_START | DOXYGEN_COMMENT_START | LINE_DOXYGEN_COMMENT_START | eof)
        eol_post[directive_token, markblockzero]
;

eol_post[int directive_token, bool markblockzero] {

        // Flags to control skipping of #if 0 and #else.
        // Once in these modes, stay in these modes until the matching #endif is reached
        // cpp_ifcount used to indicate which #endif matches the #if or #else
        switch (directive_token) {

            case IF :
            case IFDEF :
            case IFNDEF :

                // start a new blank mode for new zero'ed blocks
                if (!cpp_zeromode && markblockzero) {

                    // start a new blank mode for if
                    cpp_zeromode = true;

                    // keep track of nested if's (inside the #if 0) so we know when
                    // we reach the proper #endif
                    cpp_ifcount = 0;
                }

                // another if reached
                ++cpp_ifcount;

                // create new context for #if (and possible #else)
                if (!isoption(parseoptions, OPTION_CPP_TEXT_ELSE) && !inputState->guessing)
                    cppmode.push(cppmodeitem(size()));

                break;

            case ELSE :
            case ELIF :

                // #else reached for #if 0 that started this mode
                if (cpp_zeromode && cpp_ifcount == 1)
                    cpp_zeromode = false;

                // not in skipped #if, so skip #else until #endif of #if is reached
                else if (!cpp_zeromode) {
                    cpp_skipelse = true;
                    cpp_ifcount = 1;
                }

                if (isoption(parseoptions, OPTION_CPP_TEXT_ELSE) && !inputState->guessing) {

                    // create an empty cppmode for #if if one doesn't exist
                    if (cppmode.empty())
                        cppmode.push(cppmodeitem(size()));

                    // add new context for #else in current #if
                    cppmode.top().statesize.push_back(size());

                    if (!cpp_zeromode && cppmode.top().statesize.front() > size())
                            cppmode.top().skipelse = true;
                }
                break;

            case ENDIF :

                // another #if ended
                --cpp_ifcount;

                // #endif reached for #if 0 that started this mode
                if (cpp_zeromode && cpp_ifcount == 0)
                    cpp_zeromode = false;

                // #endif reached for #else that started this mode
                if (cpp_skipelse && cpp_ifcount == 0)
                    cpp_skipelse = false;

                if (isoption(parseoptions, OPTION_CPP_TEXT_ELSE) && !inputState->guessing &&
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
                - in zero block (cpp_zeromode) and not marking #if 0
                - when processing only #if part, not #else
                - when guessing and in else (unless in zero block)
                - when ??? for cppmode
        */
        if ((!isoption(parseoptions, OPTION_CPP_MARKUP_IF0) && cpp_zeromode) ||
            (isoption(parseoptions, OPTION_CPP_TEXT_ELSE) && cpp_skipelse) ||
            (inputState->guessing && cpp_skipelse) ||
            (!cppmode.empty() && !cppmode.top().isclosed && cppmode.top().skipelse)
        ) {
            while (LA(1) != PREPROC && LA(1) != 1 /* EOF */)
                consume();
        }

        ENTRY_DEBUG
} :;

// remove any finished or unneeded cppmodes
cppmode_cleanup[] {

        bool equal = true;
        for (unsigned int i = 0; i < cppmode.top().statesize.size(); ++i)
            if (cppmode.top().statesize[i] != cppmode.top().statesize[0]) {
                equal = false;
                break;
            }

        if (!cppmode.empty() && (equal || cppmode.top().statesize.size() == 2))
            cppmode.pop();

        ENTRY_DEBUG 
} :;

// ended modes that may lead to needed updates
cppmode_adjust[] {

    if (!isoption(parseoptions, OPTION_CPP_TEXT_ELSE) &&
        !cppmode.empty() &&
        cppmode.top().isclosed &&
        size() < cppmode.top().statesize.back())

        if (size() == cppmode.top().statesize[cppmode.top().statesize.size() - 1 - 1]) {

            // end if part of cppmode
            while (size() > cppmode.top().statesize.front())
                endMode();

            // done with this cppmode
            cppmode.pop();
       }

    ENTRY_DEBUG 
} :;

line_continuation[] { ENTRY_DEBUG } :
        {
            // end all preprocessor modes
            endDownOverMode(MODE_PARSE_EOL);
        }
        EOL_BACKSLASH
;

cpp_condition[bool& markblockzero] { CompleteElement element(this); ENTRY_DEBUG } :

        set_bool[markblockzero, LA(1) == CONSTANTS && LT(1)->getText() == "0"]

        complete_expression
;

cpp_symbol[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            // start of the name element
            startElement(SNAME);
        }
        NAME
;

cpp_symbol_optional[] { ENTRY_DEBUG } :
        (options { greedy = true; } : cpp_symbol)*
;

cpp_filename[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCPP_FILENAME);
        }
        (string_literal | char_literal | TEMPOPS (~(TEMPOPE | EOL))* TEMPOPE)
;

cpp_linenumber[] { SingleElement element(this); bool first = true; ENTRY_DEBUG } :
        (options { greedy = true; } : { if(first) { startElement(SCPP_NUMBER); first = false; } } literal)*
;
