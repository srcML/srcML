/*!
 * @file srcMLParser.g
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

/*!
 * Comments:
 *
 * This is an ANTLR grammar file for the main part of the srcML translator.
 * It is a mixture of ANTLR code with C++ code mixed in for the actions.
 *
 * The grammar is for the C++ language.  It is unlike typical C++ parsers for many
 * reasons:
 *
 * - ANTLR uses this code to generate a recursive-descent LL(k) parser.  This
 * parser starts at the leftmost token and tries to match the tokens to C++
 * source code.
 *
 * - Additional classes are used to implement an event-driven parser.  Input
 * to the parser is a stream of tokens from the lexer.  Output from this parser
 * is a new stream of tokens.  The parser user calls nextToken repeatedly to
 * process all of the tokens.
 *
 * - The parser is designed to be used interactively.  When the nextToken is
 * called a minimal number of input tokens are read to generate an output token.
 * This makes the parser very responsive and able to issue start statement
 * tokens before the end of the statement is reached.
 *
 * - The parser insert additional tokens into the input stream corresponding to
 * start and end tags.
 *
 * Matching:
 *
 * - The parser does not attempt to detect invalid C++ code.  It is designed to
 * match well-formed C++ code.  It assumes that the input C++ code is valid.
 *
 * - Whitespace and comments are handled in StreamMLParser.  They are diverted
 * from the input token stream and inserted into the output token stream.  There
 * is some processing to match these skipped tokens with the generated tokens
 * from the parser.
 *
 * - There is no symbol table.  I repeat:  There is no symbol table.  No
 * grammar rules are based on the type of an identifier.
 *
 * - Keywords are used to identify statements.  They are not used for types.
 * Type keywords are in tokens just like other identifiers.
 *
 * Implementation:
 *
 * - The state of the current parsing is stored in modes.  The modes use flags
 * to remember what state the parsing was in during the previous parse.
 *
 * - Element start tokens are generated using the method startElement.  The
 * starting elements are stored in a stack in the current mode.
 *
 * - Element end tokens are generated automatically when a mode ends.  The stack
 * of start tokens is popped of and ended automatically.
 *
 * - Do not end an element explicitly.  End the mode instead.
 *
 * Helpers:
 *
 * - The class StreamParser provides stream processing.  The class StreamMLParser
 * provides markup language stream processing.  These are template classes which
 * use this parser as a template parameter base.
 *
 * - The class TokenParser provides the virtual table for methods in StreamParser
 * that are called in this parser.
 *
 * - Obviously this needs to be untangled but is not as easy as it should be.
 *
 * - Additional methods for the parser are declared in class Mode.  These methods
 * only provide general support for the parser.  They do not, repeat, do not, contain
 * token specific processing.
 *
 * Terminology:
 *
 * The use of C++ terminology is sometimes contradictory.  This is especially true
 * for declarations and definitions, since a definition can also serve as a
 * declaration.  The following rules are used:
 *
 * declaration - stating that something exists:
 *
 *     function declaration:  int f();
 *     class declaration:     class A;
 *     struct declaration:    struct A;
 *     union declaration:     union A;
 *     method declaration:    virtual int f(); // in class
 *
 * definition - defining the layout or interface
 *
 *     function definition:  int f() {}
 *     class definition:     class A { int a; }
 *     struct definition:    struct A { int a; }
 *     union definition:     union A { int a; }
 *     method definition:    int A::f() {} // in or out of class
 *
 * C vs. C++
 *
 * Additional keywords in C++ may be identifiers in C.  This is handled in the
 * lexer which has symbols for all C++ (and C) keywords, but only will find them in
 * the input if in C++ mode.  They are matched as NAME in C mode.
 */

header "pre_include_hpp" {
#pragma GCC diagnostic ignored "-Wunused-parameter"
}

// Included in the generated srcMLParser.hpp file after antlr includes
header "post_include_hpp" {

#pragma GCC diagnostic warning "-Wunused-parameter"

#include <string>
#include <deque>
#include <stack>
#include "Language.hpp"
#include "ModeStack.hpp"
#include <srcml_types.hpp>
#include <srcml_macros.hpp>
#include <srcml.h>

// Macros to introduce trace statements
#define ENTRY_DEBUG //RuleDepth rd(this); fprintf(stderr, "TRACE: %d %d %d %5s%*s %s (%d)\n", inputState->guessing, LA(1), ruledepth, (LA(1) != EOL ? LT(1)->getText().c_str() : "\\n"), ruledepth, "", __FUNCTION__, __LINE__);
#ifdef ENTRY_DEBUG
#define ENTRY_DEBUG_INIT ruledepth(0),
#define ENTRY_DEBUG_START ruledepth = 0;
#endif

#define CATCH_DEBUG //marker();

#define assertMode(m)

enum STMT_TYPE { 
    NONE, VARIABLE, FUNCTION, FUNCTION_DECL, CONSTRUCTOR, CONSTRUCTOR_DECL, DESTRUCTOR, DESTRUCTOR_DECL,
    SINGLE_MACRO, NULLOPERATOR, ENUM_DEFN, ENUM_DECL, GLOBAL_ATTRIBUTE, PROPERTY_ACCESSOR, PROPERTY_ACCESSOR_DECL,
    EXPRESSION, CLASS_DEFN, CLASS_DECL, UNION_DEFN, UNION_DECL, STRUCT_DEFN, STRUCT_DECL, INTERFACE_DEFN, INTERFACE_DECL, ACCESS_REGION,
    USING_STMT, OPERATOR_FUNCTION, OPERATOR_FUNCTION_DECL, EVENT_STMT, PROPERTY_STMT, ANNOTATION_DEFN, GLOBAL_TEMPLATE
};

enum CALL_TYPE { NOCALL, CALL, MACRO };

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

        ++parent->number_finishing_elements;

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

        --parent->number_finishing_elements;
        if(parent->number_finishing_elements == 0) {

            for(std::vector<std::pair<srcMLState::MODE_TYPE, std::stack<int> > >::const_iterator citr = parent->finish_elements_add.begin(); 
                citr != parent->finish_elements_add.end(); ++citr) {

                parent->startNewMode(citr->first);
                parent->currentState().openelements = citr->second;


            }

            parent->finish_elements_add.clear();

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

        start_size = parent->currentState().size();

        ++parent->number_finishing_elements;

    }

    ~LightweightElement() {

        // only run if not guessing
        if (parent->inputState->guessing) return;

        // Close all elements opened by the rule in this mode.
        while (start_size < parent->currentState().size())
            parent->endElement(parent->currentState().openelements.top());

        --parent->number_finishing_elements;
        if(parent->number_finishing_elements == 0) {

            for(std::vector<std::pair<srcMLState::MODE_TYPE, std::stack<int> > >::const_iterator citr = parent->finish_elements_add.begin(); 
                citr != parent->finish_elements_add.end(); ++citr) {

                parent->startNewMode(citr->first);
                parent->currentState().openelements = citr->second;


            }

            parent->finish_elements_add.clear();

        }

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
        parent->endElement(parent->currentState().openelements.top());
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

// constructor
srcMLParser::srcMLParser(antlr::TokenStream& lexer, int lang, OPTION_TYPE & parser_options)
   : antlr::LLkParser(lexer,1), Language(lang), ModeStack(this), cpp_zeromode(false), cpp_skipelse(false), cpp_ifcount(0),
    parser_options(parser_options), ifcount(0), ENTRY_DEBUG_INIT notdestructor(false), curly_count(0), skip_ternary(false),
    current_column(-1), current_line(-1), nxt_token(-1), last_consumed(-1), wait_terminate_post(false), cppif_duplicate(false),
    number_finishing_elements(0), in_template_param(false)
{

    // root, single mode
    if (isoption(parser_options, SRCML_OPTION_EXPRESSION))
        // root, single mode to allows for an expression without a statement
        startNewMode(MODE_TOP | MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);
    else
       // root, single mode that allows statements to be nested
       startNewMode(MODE_TOP | MODE_STATEMENT | MODE_NEST);
   
}

// ends all currently open modes
void srcMLParser::endAllModes() {

     // expression mode has an extra mode
     /*
       if (isoption(parser_options, SRCML_OPTION_EXPRESSION))
       endMode();
     */

     // should only be one mode
     if (size() > 1 && isoption(parser_options, SRCML_OPTION_DEBUG))
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

#include <srcml_bitset_token_sets.hpp>

} /* end include */

options {
	language="Cpp";
    namespaceAntlr="antlr";
    namespaceStd="std";
}

class srcMLParser extends Parser;

options {
    classHeaderSuffix="public Language, public ModeStack";
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
    SNULL;          // null types null, nullptr
    SCOMPLEX;       // complex numbers

    // operators
    SOPERATOR;

    // type modifiers
    SMODIFIER;

    // internal statement elements used in multiple statements
    SNAME;
    SONAME;
    SCNAME;
    STYPE;
    STYPEPREV;
	SCONDITION;
	SBLOCK;
    SPSEUDO_BLOCK;
    SINDEX;

    // statements
	SENUM;
    SENUM_DECLARATION;

	SIF_STATEMENT;
    STERNARY;
	STHEN;
	SELSE;
	SELSEIF;

    SWHILE_STATEMENT;
	SDO_STATEMENT;

	SFOR_STATEMENT;
	SFOREACH_STATEMENT;
    SFOR_CONTROL;
	SFOR_INITIALIZATION;
	SFOR_CONDITION;
	SFOR_INCREMENT;
    SFOR_LIKE_CONTROL;

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
    SFUNCTION_LAMBDA;
	SFUNCTION_SPECIFIER;
	SRETURN_STATEMENT;
	SPARAMETER_LIST;
	SPARAMETER;
	SKRPARAMETER_LIST;
	SKRPARAMETER;
	SARGUMENT_LIST;
	SARGUMENT;
    SPSEUDO_PARAMETER_LIST;
    SINDEXER_PARAMETER_LIST;

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
    SPROTECTED_ACCESS_DEFAULT;
    SMEMBER_INITIALIZATION_LIST;
    SMEMBER_INITIALIZATION;
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
    SGENERIC_ARGUMENT;
    SGENERIC_ARGUMENT_LIST;
    STEMPLATE_PARAMETER;
    STEMPLATE_PARAMETER_LIST;
    SGENERIC_PARAMETER;
    SGENERIC_PARAMETER_LIST;

    // C Family elements
	STYPEDEF;
	SASM;
	SMACRO_CALL;
	SSIZEOF_CALL;
    SEXTERN;
	SNAMESPACE;
	SUSING_DIRECTIVE;

    // C
    SATOMIC;
    SSTATIC_ASSERT_STATEMENT;
    SGENERIC_SELECTION;
    SGENERIC_SELECTOR;
    SGENERIC_ASSOCIATION_LIST;
    SGENERIC_ASSOCIATION;

    // C++
    SALIGNAS;
    SDECLTYPE;
    SLAMBDA_CAPTURE;
    SNOEXCEPT;
    STYPENAME;
    SALIGNOF;
    STYPEID;
    SSIZEOF_PACK;
    SENUM_CLASS;
    SENUM_CLASS_DECLARATION;
    SOPERATOR_FUNCTION;
    SOPERATOR_FUNCTION_DECL;
    SREF_QUALIFIER;

    // Qt
	SSIGNAL_ACCESS;
    SFOREVER_STATEMENT;
    SEMIT_STATEMENT;

    // cpp directive internal elements
	SCPP_DIRECTIVE;
    SCPP_FILENAME;
    SCPP_NUMBER;
    SCPP_LITERAL;
	SCPP_MACRO_DEFN;
	SCPP_MACRO_VALUE;

    // cpp directives
	SCPP_ERROR;
    SCPP_WARNING;
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
    SCPP_EMPTY;

    // C# cpp directives
    SCPP_REGION;
    SCPP_ENDREGION;

    // Objective-C cpp directives
    SCPP_IMPORT;

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
    SASSERT_STATEMENT;
    SINTERFACE;
    SINTERFACE_DECLARATION;
    SSYNCHRONIZED_STATEMENT;
    SANNOTATION;
    SANNOTATION_DEFN;
    SSTATIC_BLOCK;

    // C#
    SCHECKED_STATEMENT;
    SUNCHECKED_STATEMENT;
    SATTRIBUTE;
    STARGET;
    SUNSAFE_STATEMENT;
    SLOCK_STATEMENT;
    SFIXED_STATEMENT;
    STYPEOF;
    SUSING_STATEMENT;
    SFUNCTION_DELEGATE;
    SEVENT;
    SCONSTRAINT;

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

    // Objective-C
    SRECEIVER;
    SMESSAGE;
    SSELECTOR;
    SPROTOCOL_LIST;
    SCATEGORY;
    SPROTOCOL;
    SREQUIRED_DEFAULT;
    SREQUIRED;
    SOPTIONAL;
    SPROPERTY;
    SATTRIBUTE_LIST;
    SSYNTHESIZE;
    SDYNAMIC;
    SENCODE;
    SAUTORELEASEPOOL;
    SCOMPATIBILITY_ALIAS;
    SNIL;
    SCLASS_INTERFACE;
    SCLASS_IMPLEMENTATION;
    SPROTOCOL_DECLARATION;

    // casting
    SCAST;
    SCONST_CAST;
    SDYNAMIC_CAST;
    SREINTERPRET_CAST;
    SSTATIC_CAST;

    // srcMLOutput used only
    SPOSITION;

    // Other
    SCUDA_ARGUMENT_LIST;

    // OpenMP
    SOMP_DIRECTIVE;
    SOMP_NAME;
    SOMP_CLAUSE;
    SOMP_ARGUMENT_LIST;
    SOMP_ARGUMENT;
    SOMP_EXPRESSION;

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
    OPTION_TYPE & parser_options;
    std::string namestack[2];
    int ifcount;
#ifdef ENTRY_DEBUG
    int ruledepth;
#endif
    bool is_qmark;
    bool notdestructor;
    bool operatorname;
    int curly_count;

    bool skip_ternary;

    int current_column;
    int current_line;
    int nxt_token;
    int last_consumed;
    bool wait_terminate_post;
    bool cppif_duplicate;
    size_t number_finishing_elements;
    std::vector<std::pair<srcMLState::MODE_TYPE, std::stack<int> > > finish_elements_add;
    bool in_template_param;

    static const antlr::BitSet keyword_name_token_set;
    static const antlr::BitSet keyword_token_set;
    static const antlr::BitSet macro_call_token_set;

#ifdef SRCML_BOOST_MPL_LARGE
    static const antlr::BitSet argument_token_set;
#else
    static const antlr::BitSet argument_token_set_one;
    static const antlr::BitSet argument_token_set_two;
    static const antlr::BitSet argument_token_set_three;
#endif

    static const antlr::BitSet enum_preprocessing_token_set;
    static const antlr::BitSet literal_tokens_set;
    static const antlr::BitSet modifier_tokens_set;
    static const antlr::BitSet skip_tokens_set;
    static const antlr::BitSet class_tokens_set;
    static const antlr::BitSet decl_specifier_tokens_set;

    // constructor
    srcMLParser(antlr::TokenStream& lexer, int lang, OPTION_TYPE & options);

    // destructor
    ~srcMLParser() {}

    struct cppmodeitem {
        cppmodeitem(int current_size)
            : statesize(1, current_size), isclosed(false), skipelse(false) {}

        cppmodeitem() {}

        std::deque<int> statesize;
        bool isclosed;
        bool skipelse;

    };

    std::stack<cppmodeitem> cppmode;

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


    virtual void consume() {

        if(!skip_tokens_set.member(LA(1))) last_consumed = LA(1);
        LLkParser::consume();



    }

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

        comma | { inLanguage(LANGUAGE_JAVA) }? bar | { inTransparentMode(MODE_OBJECTIVE_C_CALL) }? rbracket |

        { !inTransparentMode(MODE_INTERNAL_END_PAREN) || inPrevMode(MODE_CONDITION) }? rparen[false] |

        // characters with special actions that usually end currently open elements
        { !inTransparentMode(MODE_INTERNAL_END_CURLY) }? block_end |

        terminate |

        { inMode(MODE_ENUM) }? enum_block |

        // don't confuse with expression block
        { ((inTransparentMode(MODE_CONDITION) ||
            (!inMode(MODE_EXPRESSION) && !inMode(MODE_EXPRESSION_BLOCK | MODE_EXPECT))) 
        && !inTransparentMode(MODE_CALL | MODE_INTERNAL_END_PAREN)
        && (!inLanguage(LANGUAGE_CXX) || !inTransparentMode(MODE_INIT | MODE_EXPECT))) || inTransparentMode(MODE_ANONYMOUS) }? lcurly |

        { inMode(MODE_ARGUMENT_LIST) }? call_argument_list |

        // switch cases @test switch
        { !inMode(MODE_INIT) && (!inMode(MODE_EXPRESSION) || inTransparentMode(MODE_DETECT_COLON)) }?
        colon |

        // process template operator correctly @test template
        { inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST) }? tempope |

        // // special default() call for C#
        // { LA(1) == DEFAULT && inLanguage(LANGUAGE_CSHARP) && inTransparentMode(MODE_EXPRESSION) && next_token() == LPAREN}? expression_part_default |

        // statements that clearly start with a keyword
        { (isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) || (LA(1) != TEMPLATE || next_token() != TEMPOPS))
         && inMode(MODE_NEST | MODE_STATEMENT) && !inMode(MODE_FUNCTION_TAIL) && (LA(1) != TEMPLATE || next_token() == TEMPOPS)
         && !(inLanguage(LANGUAGE_OBJECTIVE_C) && LA(1) == IMPORT)
         && !(LA(1) == ATPROTOCOL && next_token() == LPAREN)
         && (LA(1) != DEFAULT || next_token() == COLON)
         && (LA(1) != CHECKED || next_token() == LCURLY)
         && (LA(1) != UNCHECKED || next_token() == LCURLY)
         && (LA(1) != CXX_TRY || next_token() == LCURLY)
         && (LA(1) != INLINE || next_token() == NAMESPACE)
         && (LA(1) != STATIC || (inLanguage(LANGUAGE_JAVA) && next_token() == LCURLY))
         && (LA(1) != CXX_CATCH || next_token() == LPAREN || next_token() == LCURLY)
         && (LA(1) != ASM || look_past_two(ASM, VOLATILE) == LPAREN) }? keyword_statements |

        { next_token() == LPAREN }? synchronized_statement |

        { inLanguage(LANGUAGE_CXX) && inMode(MODE_USING) }? using_aliasing |

        // statements identified by pattern (i.e., do not start with a keyword)
        { inMode(MODE_NEST | MODE_STATEMENT) && !inMode(MODE_FUNCTION_TAIL)}? pattern_statements |

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
        if_statement | { !isoption(parser_options, SRCML_OPTION_NESTIF) && next_token() == IF }? elseif_statement | else_statement | switch_statement | switch_case | switch_default |

        // iterative statements
        while_statement | for_statement | do_statement | foreach_statement |

        // jump statements
        return_statement | break_statement | continue_statement | goto_statement |

        // template declarations - both functions and classes
        template_declaration |

        // exception statements
        { inLanguage(LANGUAGE_JAVA) && next_token() == LPAREN }? try_statement_with_resource | try_statement | catch_statement | finally_statement | throw_statement |

        // namespace statements
        namespace_definition |

        // C/C++
        typedef_statement | friend_statement | 

        // C
        static_assert_statement |

        // Java - keyword only detected for Java
        import_statement | package_statement | assert_statement | static_block |

        // C# - keyword only detected for C#
        checked_statement | unchecked_statement | lock_statement | fixed_statement | unsafe_statement | yield_statements |

        // C/C++ assembly block
        asm_declaration |

        // Objective-C - kewywords only detected for Objective-C
        objective_c_class | protocol | objective_c_class_end | property_declaration | synthesize_statement | dynamic_statement |

        autoreleasepool_block | compatibility_alias | class_directive |

        // Qt
        forever_statement | emit_statement

;

/*
  Statements, declarations, and definitions that must be matched by pattern.

  Basically we have an identifier and we don't know yet whether it starts an expression
  function definition, function declaration, or even a label.
*/
pattern_statements[] { int secondtoken = 0; int type_count = 0; bool isempty = false; int call_count = 1;
        STMT_TYPE stmt_type = NONE; CALL_TYPE type = NOCALL;

        // detect the declaration/definition type
        pattern_check(stmt_type, secondtoken, type_count);

        ENTRY_DEBUG } :

        // variable declaration
        { stmt_type == VARIABLE }?
        variable_declaration_statement[type_count] |

        // check for Objective-C method
        { stmt_type == FUNCTION_DECL }?
        objective_c_method[SFUNCTION_DECLARATION] |

        { stmt_type == FUNCTION }?
        objective_c_method[SFUNCTION_DEFINITION] |

        // check for declaration of some kind (variable, function, constructor, destructor
        { stmt_type == FUNCTION_DECL }?
        function_declaration[type_count] |

        // function definition
        { stmt_type == FUNCTION }?
        function_definition[type_count] |

        { stmt_type == OPERATOR_FUNCTION_DECL }?
        function_declaration[type_count, SOPERATOR_FUNCTION_DECL] |

        // function definition
        { stmt_type == OPERATOR_FUNCTION }?
        function_definition[type_count, SOPERATOR_FUNCTION] |

        { stmt_type == CLASS_DEFN }?
        class_definition |

        { stmt_type == INTERFACE_DEFN }?
        interface_definition |

        { stmt_type == INTERFACE_DECL }?
        interface_declaration |

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

        { stmt_type == ANNOTATION_DEFN }?
        annotation_definition |

        { stmt_type == ACCESS_REGION }?
        access_specifier_region |

        { inLanguage(LANGUAGE_CXX) && stmt_type == GLOBAL_ATTRIBUTE }?
        attribute_cpp |

        { inLanguage(LANGUAGE_CSHARP) && stmt_type == GLOBAL_ATTRIBUTE }?
        attribute_csharp |

        { stmt_type == PROPERTY_ACCESSOR }?
        property_method[SFUNCTION_DEFINITION] |

        { stmt_type == PROPERTY_ACCESSOR_DECL }?
        property_method[SFUNCTION_DECLARATION] |

        // standalone macro
        { stmt_type == SINGLE_MACRO }?
        macro_call |

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
        { stmt_type == ENUM_DEFN }?
        enum_definition |

        { stmt_type == ENUM_DECL }?
        enum_declaration |

        { stmt_type == USING_STMT }?
        using_namespace_statement |

        // C# property statement
        { stmt_type == PROPERTY_STMT}?
        property_statement[type_count] |

        // C# event statement
        { stmt_type == EVENT_STMT}?
        event_statement[type_count] |

        { stmt_type == GLOBAL_TEMPLATE }?
        template_declaration |

        { stmt_type == NONE && inTransparentMode(MODE_FRIEND) }?
        compound_name |

        // "~" which looked like destructor, but isn't
        { stmt_type == NONE }?
        expression_statement_process
        expression_process
        sole_destop |

        // labels to goto
        { secondtoken == COLON }?
        label_statement |

        macro_label_statement |

        // extern block as opposed to enum as part of declaration
        { stmt_type == NONE }?
        extern_definition |

        // call
        { isoption(parser_options, SRCML_OPTION_CPP) && (inMode(MODE_ACCESS_REGION) || (perform_call_check(type, isempty, call_count, secondtoken) && type == MACRO)) }?
        macro_call |

        { inMode(MODE_ENUM) && inMode(MODE_LIST) }? enum_short_variable_declaration |


        { inLanguage(LANGUAGE_JAVA) && LA(1) == ATSIGN }? annotation |

        expression_statement[type, call_count]
;

// efficient way to view the token after the current LA(1)
next_token[] returns [int token] {

    antlr::RefToken rtoken = LT(1);

    if(rtoken->getColumn() == current_column && rtoken->getLine() == current_line) {

        token = nxt_token;

    } else {

        current_column = rtoken->getColumn();
        current_line = rtoken->getLine();

        int place = mark();
        inputState->guessing++;

        // consume current token
        consume();

        token = LA(1);

        inputState->guessing--;
        rewind(place);

        nxt_token = token;

    }

} :;

// efficient way to view the token after the current next_token
next_token_two[] returns [int token] {

    int place = mark();
    inputState->guessing++;

    // consume current token
    consume();
    consume();

    token = LA(1);

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

// skip past all of the skiptoken1 and skiptoken2 and return the one after
look_past_two[int skiptoken1, int skiptoken2] returns [int token] {

    int place = mark();
    inputState->guessing++;

    while (LA(1) == skiptoken1 || LA(1) == skiptoken2)
        consume();

    token = LA(1);

    inputState->guessing--;
    rewind(place);

} :;

// give the next token as if rule was applied.  If rule can not be applied return -1
look_past_rule[void (srcMLParser::*rule)()] returns [int token] {

    int place = mark();
    inputState->guessing++;

    try {

        (this->*rule)();
        token = LA(1);

    } catch(...) {

        token = -1;

    }

    inputState->guessing--;
    rewind(place);

} :;

/* functions */

// beginning function declaration/header
function_declaration[int type_count, int token = SFUNCTION_DECLARATION] { ENTRY_DEBUG } :
		{
            startNewMode(MODE_STATEMENT);

            startElement(token);
        }
        function_header[type_count]
;

// A function pointer name handle
function_pointer_name_grammar[] { ENTRY_DEBUG } :
        LPAREN function_pointer_name_base RPAREN
;

// inner portion of functon pointer name
function_pointer_name_base[] { ENTRY_DEBUG bool flag = false; } :

        // special case for function pointer names that don't have '*'
        { macro_call_token_set.member(LA(1)) }?
        (compound_name_inner[false])* |

        // special name prefix of namespace or class
        identifier (generic_argument_list)* DCOLON function_pointer_name_base |

        // typical function pointer name
        multops (multops)* (compound_name_inner[false])*

        // optional array declaration
        (variable_identifier_array_grammar_sub[flag])*
;

decl_pre_type[int & type_count] { ENTRY_DEBUG } :

    (

    { decl_specifier_tokens_set.member(LA(1)) }? (specifier | default_specifier | template_specifier) |

    // special case only for functions.  Should only reach here for funciton in Java
    { inLanguage(LANGUAGE_JAVA) && LA(1) == FINAL }? single_keyword_specifier |

    { inLanguage(LANGUAGE_JAVA) }? annotation |

    { inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |

    { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp |

    )

    set_int[type_count, type_count - 1]
;

// header of a function
function_header[int type_count] { ENTRY_DEBUG } :

        // no return value functions:  casting operator method and main
        { type_count == 0 }? function_identifier
        { replaceMode(MODE_FUNCTION_NAME, MODE_FUNCTION_PARAMETER | MODE_FUNCTION_TAIL); } |
        (options { greedy = true; } : { !isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) && next_token() == TEMPOPS }? template_declaration_full set_int[type_count, type_count - 1])*

        (options { greedy = true; } : { type_count > 0 && (LA(1) != OVERRIDE || !inLanguage(LANGUAGE_CXX)) && (decl_specifier_tokens_set.member(LA(1))
            || (inLanguage(LANGUAGE_JAVA) && (LA(1) == ATSIGN || LA(1) == FINAL))
            || (inLanguage(LANGUAGE_CSHARP) && LA(1) == LBRACKET) || (inLanguage(LANGUAGE_CXX) && LA(1) == LBRACKET && next_token() == LBRACKET))}?
                decl_pre_type[type_count] |

        { inLanguage(LANGUAGE_JAVA) }? generic_parameter_list set_int[type_count, type_count - 1])*

        function_type[type_count]
;

// portion of function after paramter list and before block
function_tail[] { ENTRY_DEBUG } :

        (options { greedy = true; } :

            /* order is important */
            { inLanguage(LANGUAGE_CXX_FAMILY)
                && (LA(1) != EQUAL || (inLanguage(LANGUAGE_CXX) && (next_token() == CONSTANTS || next_token() == DEFAULT || next_token() == DELETE))) }?
            function_specifier |

            { inLanguage(LANGUAGE_CXX) }?
            ref_qualifier |

            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            (TRY | CXX_TRY) | 

            { inLanguage(LANGUAGE_OO) }?
            complete_throw_list |

            { inLanguage(LANGUAGE_CXX) }?
            complete_noexcept_list |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}?
            attribute_cpp |

            { inLanguage(LANGUAGE_CXX) }?
            trailing_return |

            { inLanguage(LANGUAGE_JAVA) }?
            annotation_default |

            // K&R
            { inLanguage(LANGUAGE_C) }? (

            // @todo:  Must be integrated into other C-based languages
            // @todo:  Wrong markup
            (simple_identifier paren_pair)=> macro_call |
            { look_past_two(NAME, VOID) == LCURLY }? simple_identifier |
              parameter (MULTOPS | simple_identifier | COMMA)* TERMINATE
            )
        )*
;

// Java annotation default values
annotation_default[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        startNewMode(MODE_LOCAL);

        startElement(SDEFAULT);

    }
    DEFAULT annotation_default_initialization

;

// Java annotation default value initialization
annotation_default_initialization[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        startNewMode(MODE_LOCAL);

        startElement(SDECLARATION_INITIALIZATION);

    }
    complete_expression

;

// Ref qualifiers in function tail
ref_qualifier[]  { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            startElement(SREF_QUALIFIER);
        }
        (
        REFOPS | RVALUEREF
        )
;

// trailing return in function tail
trailing_return[] {  int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :

        TRETURN
        ({ pattern_check(stmt_type, secondtoken, type_count, true) && (stmt_type == FUNCTION || stmt_type == FUNCTION_DECL)}?
        { startNewMode(MODE_TRAILING_RETURN); } function_declaration[type_count] function_identifier parameter_list | set_int[type_count, type_count + 1] parameter_type_count[type_count]
        )
;

// Do the rest of the function and get the end
function_rest[int& fla] { ENTRY_DEBUG } :

        eat_optional_macro_call

        parameter_list function_tail check_end[fla]
;

// function type, including specifiers
function_type[int type_count] { bool is_compound = false; ENTRY_DEBUG } :
        {

            if(type_count == 0) {

                setMode(MODE_FUNCTION_NAME);
                return;

            }

            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_EAT_TYPE | MODE_FUNCTION_TYPE);

            setTypeCount(type_count);

            // type element begins
            startElement(STYPE);
        }
        (options { greedy = true; } : { inputState->guessing && (LA(1) == TYPENAME || LA(1) == CONST) }? (lead_type_identifier))* 

        // match auto keyword first as special case do no warn about ambiguity
        (options { generateAmbigWarnings = false; } : auto_keyword[type_count > 1] |
            { is_class_type_identifier() }? (options { greedy = true; } :
                { !class_tokens_set.member(LA(1)) }? 
                    (options { generateAmbigWarnings = false; } : specifier | { look_past_rule(&srcMLParser::identifier) != LPAREN }? identifier | macro_call) { decTypeCount(); })*
                    class_type_identifier[is_compound] { decTypeCount(); } (options { greedy = true; } : { !is_compound }? multops)* |
        (options { greedy = true; } : { getTypeCount() > 2 }? pure_lead_type_identifier { decTypeCount(); })* (lead_type_identifier | { inLanguage(LANGUAGE_JAVA) }? default_specifier))

        { 

            decTypeCount();
            if(inTransparentMode(MODE_ARGUMENT) && inLanguage(LANGUAGE_CXX))
                return;
        }

        (options { greedy = true; } : {getTypeCount() > 0}? 
            // Mark as name before mark without name
            (options { generateAmbigWarnings = false;} :  keyword_name | type_identifier | { inLanguage(LANGUAGE_JAVA) }? default_specifier) { decTypeCount(); })*

        {
            endMode(MODE_EAT_TYPE);
            setMode(MODE_FUNCTION_NAME);
        }
;

// check the functon type
function_type_check[int& type_count] { type_count = 1; ENTRY_DEBUG } :

        lead_type_identifier
        ( { inLanguage(LANGUAGE_JAVA_FAMILY) || LA(1) != LBRACKET }? type_identifier_count[type_count])*
;

// match a function identifier
function_identifier[] { ENTRY_DEBUG } :

        // Mark as name before mark without name
        (options { generateAmbigWarnings = false; } :  
            // typical name  
            compound_name_inner[false] |

            keyword_name | 

            { function_pointer_name_check() }? 
            function_pointer_name |

            function_identifier_main |

            // function pointer identifier with name marked separately
            function_pointer_name_grammar eat_optional_macro_call
        )
;

// special cases for main
function_identifier_main[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        MAIN
;

// overloaded operator name
overloaded_operator[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SNAME);
        }
        set_bool[operatorname, true]

        OPERATOR 
        {

            startElement(SNAME);

        }

        (
            // special case for 'operator()'
            { LA(1) == LPAREN }? LPAREN RPAREN |

            // general operator name case is anything from 'operator', operators, or names
            (options { greedy = true; } : ~(LPAREN))*
        )
;

// handle a C# lambda expression
lambda_expression_csharp[] { bool first = true; ENTRY_DEBUG } :
		{

            startNewMode(MODE_FUNCTION_TAIL | MODE_ANONYMOUS | MODE_END_AT_COMMA);      

            startElement(SFUNCTION_LAMBDA);

        }

        (options { greedy = true; } : specifier)*
        (parameter_list | lambda_single_parameter) lambda_csharp (options { greedy = true; } : { LA(1) != LCURLY && first }? complete_expression set_bool[first, false])*

;

// handle a C++11 lambda expression
lambda_expression_cpp[] { ENTRY_DEBUG } :
		{

            bool iscall = look_past_rule(&srcMLParser::lambda_expression_full_cpp) == LPAREN;
            if(iscall) {

                // start a new mode that will end after the argument list
                startNewMode(MODE_ARGUMENT | MODE_LIST);

                // start the function call element
                startElement(SFUNCTION_CALL);

            }

            startNewMode(MODE_FUNCTION_PARAMETER | MODE_FUNCTION_TAIL | MODE_ANONYMOUS);      

            startElement(SFUNCTION_LAMBDA);

        }

        lambda_capture
;

// [capture] portion of a C++11 lambda function.
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
            { /* warning suppression */ LA(1) == COMMA }? COMMA | { LA(1) != RBRACKET }? lambda_capture_argument)* 
            RBRACKET
    
        )
;

// argument within the capture portion of a C++11 lambda
lambda_capture_argument[] { bool first = true; CompleteElement element(this); ENTRY_DEBUG } :

        {
            startNewMode(MODE_LOCAL);

            startElement(SARGUMENT);
        }

        // suppress greedy warnings
        ((options { greedy = true; } : lambda_capture_modifiers)* (options { greedy = true; } : { first }? variable_identifier (options { greedy = true; } : lambda_capture_initialization)* set_bool[first, false])*)
;

lambda_capture_initialization[] { CompleteElement element(this); ENTRY_DEBUG } :
   {
        startNewMode(MODE_LOCAL | MODE_END_AT_COMMA);

        startElement(SDECLARATION_INITIALIZATION);
    }

    // suppress warning of another case where REFOPS or something is in both alts.
    EQUAL complete_expression

;

// completely match a C# lambda expression
lambda_expression_full_csharp[] { ENTRY_DEBUG } :

        (options { greedy = true; } : ASYNC)* (variable_identifier | paren_pair) LAMBDA

;

// completely match a C++ lambda expression
lambda_expression_full_cpp[] { ENTRY_DEBUG } :

        // paren_pair and curly_pair seem to have nondeterminism because both can match LPAREN
        LBRACKET (~RBRACKET)* RBRACKET (options { warnWhenFollowAmbig = false; } : paren_pair)* function_tail curly_pair

;

// modifiers that can occur within a lambda capture.
lambda_capture_modifiers[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_MODIFIER))
                    startElement(SMODIFIER);
        }
        (EQUAL | REFOPS)

;

// handle a block expression lambda
block_lambda_expression[] { ENTRY_DEBUG } :
        {

            bool iscall = look_past_rule(&srcMLParser::block_lambda_expression_full) == LPAREN;
            if(iscall) {

                // start a new mode that will end after the argument list
                startNewMode(MODE_ARGUMENT | MODE_LIST);

                // start the function call element
                startElement(SFUNCTION_CALL);

            }

            startNewMode(MODE_FUNCTION_PARAMETER | MODE_FUNCTION_TAIL | MODE_ANONYMOUS);      

            startElement(SFUNCTION_LAMBDA);

        }

        BLOCKOP (options { greedy = true; } : type_identifier)* (options { greedy = true; } : parameter_list)*

;

// completely match block expression lambda
block_lambda_expression_full[] { ENTRY_DEBUG } :

        BLOCKOP (options { greedy = true; } : type_identifier)* (options { greedy = true; } : paren_pair)* curly_pair

;

// handle a Java lambda expression
lambda_expression_java[] { bool first = true; ENTRY_DEBUG } :
        {

            startNewMode(MODE_FUNCTION_TAIL | MODE_ANONYMOUS);      

            startElement(SFUNCTION_LAMBDA);

        }

        (parameter_list | lambda_single_parameter) lambda_java (options { greedy = true; } : { LA(1) != LCURLY && first }? complete_expression set_bool[first, false])*

;

lambda_single_parameter { CompleteElement element(this); ENTRY_DEBUG } :
        {

            startNewMode(MODE_LOCAL);

            startElement(SPSEUDO_PARAMETER_LIST);
            startElement(SPARAMETER);
            startElement(SDECLARATION);
        }
        variable_identifier

;

// lambda character
lambda_java[] { ENTRY_DEBUG } :
        
    TRETURN

    {

        if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
            startElement(SPSEUDO_BLOCK);

    }

;

// handle the beginning of a function definition
function_definition[int type_count, int token = SFUNCTION_DEFINITION] { ENTRY_DEBUG } :
		{
            startNewMode(MODE_STATEMENT);

            startElement(token);
        }
        function_header[type_count]
;

// throw list for a function
throw_list[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_EXPECT);

            startElement(STHROW_SPECIFIER);
        }
        THROW 

        {

            startElement(SARGUMENT_LIST);

        }
        LPAREN |
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_EXPECT | MODE_END_LIST_AT_BLOCK);

            startElement(STHROW_SPECIFIER_JAVA);
        }
        THROWS
;

// The next two rules may be equivalent.

// noexcept list in a function
noexcept_list[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_EXPECT);

            startElement(SNOEXCEPT);
        }
        NOEXCEPT 

        { 

            if(LA(1) != LPAREN) endMode(); 
            else startElement(SARGUMENT_LIST); 

        } 

        (options { greedy = true;} : LPAREN)*

;

// match a thow list completely
complete_throw_list[] { bool is_compound = false; ENTRY_DEBUG } :
        THROW paren_pair | THROWS ( options { greedy = true; } : compound_name_java[is_compound] | COMMA)*
;

// match noexcept list completely
complete_noexcept_list[] { ENTRY_DEBUG } :
        NOEXCEPT (options { greedy = true;} : paren_pair)*
;

/* property methods */

// C# property method GET/SET/ADD/REMOVE
property_method[int element] { ENTRY_DEBUG } :
		{
            // function definitions have a "nested" block statement
            startNewMode(MODE_STATEMENT);

            // start the function definition element
            startElement(element);
        }
        (attribute_csharp | specifier)*
        property_method_name
;

// handle the name portion of a property method
property_method_name[] { SingleElement element(this); ENTRY_DEBUG } :
		{
            startElement(SNAME);
        }
        (GET | SET | ADD | REMOVE)
;

// Objective-C method declaration
objective_c_method[int token = SNOP] { ENTRY_DEBUG } :
    {

        startNewMode(MODE_STATEMENT);

        startElement(token);

    }
    objective_c_method_specifier (options { greedy = true; } : objective_c_method_type)* /*objective_c_selector*/ objective_c_parameter_list

;

objective_c_method_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
    {

        startElement(SFUNCTION_SPECIFIER);

    }
    (CSPEC | MSPEC)

;

// either Objective-C method return type or parameter type
objective_c_method_type[] { CompleteElement element(this); ENTRY_DEBUG } :
        {

            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_LOCAL);

            // type element begins
            startElement(STYPE);

        }

        LPAREN

        (options { greedy = true; } : { inputState->guessing && (LA(1) == TYPENAME || LA(1) == CONST) }? (lead_type_identifier))* 

        // match auto keyword first as special case do no warn about ambiguity
        (options { generateAmbigWarnings = false; } : auto_keyword[true] | lead_type_identifier)


        (options { greedy = true; } : { LA(1) != RPAREN}? 

            // Mark as name before mark without name
            (options { generateAmbigWarnings = false;} :  keyword_name | type_identifier)
        )*

        RPAREN

;

objective_c_parameter_list[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        startNewMode(MODE_FUNCTION_PARAMETER);

        // start the function call element
        startElement(SPARAMETER_LIST);

    }

    objective_c_parameter (objective_c_parameter)*

;

// method parameter name:value pair for Objective_C
objective_c_parameter[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        if(inTransparentMode(MODE_LIST))
            endDownToMode(MODE_LIST);

        startNewMode(MODE_PARAMETER);

        startElement(SPARAMETER);

    }

    objective_c_selector

    (options { greedy = true; } : 

        objective_c_method_type

        // Mark as name before mark without name
        (options { generateAmbigWarnings = false; } : compound_name | keyword_name)

    )*

;

// Objective-C property declaration
property_declaration[] { int type_count = 0;  int secondtoken = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
    {

        startNewMode(MODE_STATEMENT);

        startElement(SPROPERTY);

    }
    PROPERTY (property_attribute_list)*
    { pattern_check(stmt_type, secondtoken, type_count) }?
    variable_declaration[type_count]

;

property_attribute_list[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        startNewMode(MODE_LOCAL);

        startElement(SATTRIBUTE_LIST);

    }
    LPAREN
    (property_attribute | COMMA)*
    RPAREN

;

property_attribute[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        startNewMode(MODE_LOCAL);

        startElement(SATTRIBUTE);

    }
    identifier (property_attribute_initialization)* 

;

property_attribute_initialization[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        startNewMode(MODE_LOCAL);

        //startElement(SDECLARATION_INITIALIZATION);

    }
    EQUAL identifier

;

synthesize_statement[] { ENTRY_DEBUG } :
    {

        startNewMode(MODE_STATEMENT);

        startElement(SSYNTHESIZE);

    }
    SYNTHESIZE property_implementation_inner

;

dynamic_statement[] { ENTRY_DEBUG } :
    {

        startNewMode(MODE_STATEMENT);

        startElement(SDYNAMIC);
        
    }
    DYNAMIC property_implementation_inner

;

property_implementation_inner[] { ENTRY_DEBUG } :

    property_implementation_name (COMMA property_implementation_name)*

;

property_implementation_name[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        startNewMode(MODE_LOCAL);

        startElement(SDECLARATION);

    }
    identifier (property_implementation_initialization)*

;

property_implementation_initialization[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        startNewMode(MODE_LOCAL);

        //startElement(SDECLARATION_INITIALIZATION);

    }
    EQUAL identifier

;

// Check and see if this is a call and what type
perform_call_check[CALL_TYPE& type, bool & isempty, int & call_count, int secondtoken] returns [bool iscall] {

    iscall = true;
    isempty = false;

    type = NOCALL;

    int start = mark();
    inputState->guessing++;

    int save_first = LA(1);

    int postnametoken = 0;
    int argumenttoken = 0;
    int postcalltoken = 0;
    call_count = 0;
    try {
        call_check(postnametoken, argumenttoken, postcalltoken, isempty, call_count);

        // call syntax succeeded
        type = CALL;

        // call syntax succeeded, however post call token is not legitimate
        if (isoption(parser_options, SRCML_OPTION_CPP) &&
            (((!inLanguage(LANGUAGE_OBJECTIVE_C) || !inTransparentMode(MODE_OBJECTIVE_C_CALL)) && (keyword_token_set.member(postcalltoken) || postcalltoken == NAME || postcalltoken == VOID))
            || (!inLanguage(LANGUAGE_CSHARP) && postcalltoken == LCURLY)
            || postcalltoken == EXTERN || postcalltoken == STRUCT || postcalltoken == UNION || postcalltoken == CLASS || postcalltoken == CXX_CLASS
            || (!inLanguage(LANGUAGE_CSHARP) && postcalltoken == RCURLY)
            || postcalltoken == 1 /* EOF ? */
            || postcalltoken == TEMPLATE || postcalltoken == INLINE
            || postcalltoken == PUBLIC || postcalltoken == PRIVATE || postcalltoken == PROTECTED || postcalltoken == SIGNAL
            || postcalltoken == ATREQUIRED || postcalltoken == ATOPTIONAL
            || postcalltoken == STATIC || postcalltoken == CONST)
            && (save_first != DECLTYPE))

            type = MACRO;
        if(inLanguage(LANGUAGE_CSHARP) && (postcalltoken == LAMBDA || postcalltoken == EQUAL))
            type = NOCALL;

    } catch (...) {

        type = NOCALL;

        if (isoption(parser_options, SRCML_OPTION_CPP) && argumenttoken != 0 && postcalltoken == 0)
            type = MACRO;

        // single macro call followed by statement_cfg
        else if (isoption(parser_options, SRCML_OPTION_CPP) && secondtoken != -1
                 && (keyword_token_set.member(secondtoken) || secondtoken == LCURLY || secondtoken == 1 /* EOF */
                     || secondtoken == PUBLIC || secondtoken == PRIVATE || secondtoken == PROTECTED))

            type = MACRO;
    }

    inputState->guessing--;
    rewind(start);

    ENTRY_DEBUG } :;

// check if call is call
call_check[int& postnametoken, int& argumenttoken, int& postcalltoken, bool & isempty, int & call_count] { ENTRY_DEBUG } :

        // detect name, which may be name of macro or even an expression
        (function_identifier | keyword_call_tokens (DOTDOTDOT | generic_argument_list | cuda_argument_list)* | { inLanguage(LANGUAGE_OBJECTIVE_C) }? bracket_pair)

        // record token after the function identifier for future use if this fails
        markend[postnametoken]
        set_bool[isempty, LA(1) == LPAREN && next_token() == RPAREN]
        (
            { isoption(parser_options, SRCML_OPTION_CPP) }?
            // check for proper form of argument list
            (call_check_paren_pair[argumenttoken] set_int[call_count, call_count + 1])*

            // record token after argument list to differentiate between call and macro
            markend[postcalltoken] |

            LPAREN set_int[call_count, 1]
        )
;

// check the contents of a call
call_check_paren_pair[int& argumenttoken, int depth = 0] { bool name = false; ENTRY_DEBUG } :

        LPAREN

        // record token after the start of the argument list
        markend[argumenttoken]
        ( options { greedy = true; generateAmbigWarnings = false;} :

            // recursive nested parentheses
            call_check_paren_pair[argumenttoken, depth + 1] set_bool[name, false] |

            // special case for something that looks like a declaration
            { !name || (depth > 0) }?
            (identifier | generic_selection) set_bool[name, true] |

            keyword_call_tokens (options { greedy = true; } : DOTDOTDOT | generic_argument_list | cuda_argument_list)* |

            // special case for something that looks like a declaration
            { LA(1) == DELEGATE /* eliminates ANTRL warning, will be nop */ }? delegate_anonymous |

            { next_token_check(LCURLY, LPAREN) }?
            lambda_anonymous |

            (LBRACKET (~RBRACKET)* RBRACKET (LPAREN | LCURLY)) => lambda_expression_full_cpp |

            (block_lambda_expression_full) => block_lambda_expression_full |

            { inLanguage(LANGUAGE_OBJECTIVE_C) }? bracket_pair |

            // found two names in a row, so this is not an expression
            // cause this to fail by explicitly throwing exception
            { depth == 0 }?
            (identifier | generic_selection) throw_exception[true] |

            // forbid parentheses (handled recursively) and cfg tokens
            { !keyword_token_set.member(LA(1)) }? ~(LPAREN | RPAREN | TERMINATE) set_bool[name, false]
        )*

        RPAREN
;

perform_ternary_check[] returns [bool is_ternary] {

    is_ternary = false;

    int start = mark();
    inputState->guessing++;

    try {

        ternary_check();
        if(LA(1) == QMARK) is_ternary = true;

        if(!is_qmark) {

            if(LA(1) == TERMINATE) skip_ternary = true;
            if(LA(1) == LCURLY) skip_ternary = true;

        }

    } catch(...) { 

        if(!is_qmark) {

            if(LA(1) == TERMINATE) skip_ternary = true;
            if(LA(1) == LCURLY) skip_ternary = true;

        }

    }

    inputState->guessing--;
    rewind(start);

    ENTRY_DEBUG

}:;

ternary_check[] { ENTRY_DEBUG } :


    // ends are catch alls ok if overlap
    ({ LA(1) != 1 }? (options { generateAmbigWarnings = false; } : paren_pair | bracket_pair (options { greedy = true; } : paren_pair | curly_pair)*
         | ~(QMARK | TERMINATE | LCURLY | COLON | RPAREN | COMMA | RBRACKET | RCURLY | EQUAL | ASSIGNMENT)))

    // ends are catch alls ok if overlap
    ({ LA(1) != 1 }? (options { generateAmbigWarnings = false; } : paren_pair | bracket_pair (options { greedy = true; } : paren_pair | curly_pair)*
         | ~(QMARK | TERMINATE | LCURLY  | COLON | RPAREN | COMMA | RBRACKET | RCURLY | EQUAL | ASSIGNMENT)))* 

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

// Qt forever statement
forever_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement (after condition)
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the while element
            startElement(SFOREVER_STATEMENT);

            // expect a condition to follow the keyword
            //startNewMode(MODE_CONDITION | MODE_EXPECT);
        }
        FOREVER
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

        {

            if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
                startElement(SPSEUDO_BLOCK);

        }
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
            startNewMode(MODE_EXPECT | MODE_FOR_CONTROL);
        }
;

// start of foreach statement (C#/Qt)
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
            if(inLanguage(LANGUAGE_CSHARP))
                startNewMode(MODE_EXPECT | MODE_FOR_CONTROL);
            else
                startNewMode(MODE_EXPECT | MODE_FOR_CONTROL | MODE_END_AT_COMMA);
        }
;

// start of for group, i.e., initialization, test, increment
for_control[] { ENTRY_DEBUG } :
        {
            // start the for group mode that will end at the next matching
            // parentheses
            replaceMode(MODE_FOR_CONTROL, MODE_TOP | MODE_FOR_INITIALIZATION | MODE_IGNORE_TERMINATE |
                        MODE_INTERNAL_END_PAREN | MODE_LIST);

            // start the for heading group element
            startElement(SFOR_CONTROL);
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

// handle initilization portion of a for.
for_initialization[] { int type_count = 0;  int secondtoken = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        for_initialization_action
        (
            // explicitly check for a variable declaration since it can easily
            // be confused with an expression
            { pattern_check(stmt_type, secondtoken, type_count) && stmt_type == VARIABLE }?
            for_initialization_variable_declaration[type_count] |

            { if(secondtoken == COLON) setMode(MODE_RANGED_FOR); } expression
        )
;

// Statement for the declaration of a variable or group of variables
// in a for initialization
for_initialization_variable_declaration[int type_count] { ENTRY_DEBUG } :
        {
            // start a new mode for the expression which will end
            // inside of the terminate
            startNewMode(MODE_LIST);
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

        {

            if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
                startElement(SPSEUDO_BLOCK);

        }
;

/*
 else if construct

 else and if are detected on their own, and as part of termination (semicolon or
 end of a block
*/
elseif_statement[] { ENTRY_DEBUG } :
        {
            // treat as a statement with a nested statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_IF | MODE_ELSE);

            ++ifcount;

            // start the else part of the if statement
            startElement(SELSEIF);
        }
        ELSE 

        {

            // start the if statement
            startElement(SIF_STATEMENT);

            // expect a condition
            // start THEN after condition
            startNewMode(MODE_CONDITION | MODE_EXPECT);
        }
        IF
;

//  start of switch statement
switch_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested block
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_SWITCH);

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
            endDownToModeSet(MODE_TOP);

            // flush any whitespace tokens since sections should
            // end at the last possible place
            flushSkip();

            // end the section inside the block
            endWhileMode(MODE_TOP_SECTION);
        }
        section_entry_action_first
;

// case treated as a statement
switch_case[] { ENTRY_DEBUG } :
        {
            // start a new mode
            startNewMode(MODE_TOP_SECTION | MODE_TOP | MODE_STATEMENT | MODE_NEST | MODE_DETECT_COLON);

            // start of case element
            startElement(SCASE);

            // expect an expression ended by a colon
            startNewMode(MODE_EXPRESSION | MODE_EXPECT);
        }
        (CASE | macro_case_call)
;

// default treated as a statement
switch_default[] { ENTRY_DEBUG } :
        {
            // start a new mode
            startNewMode(MODE_TOP_SECTION | MODE_TOP | MODE_STATEMENT | MODE_NEST | MODE_DETECT_COLON);
  
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

// * as name
multop_name[] { SingleElement element(this); ENTRY_DEBUG } :
        {
                startElement(SNAME);
        }
        MULTOPS
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

// package statement
assert_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the return statement
            startElement(SASSERT_STATEMENT);
        }
        ASSERT
;

static_block  { ENTRY_DEBUG } :

    {
        startNewMode(MODE_STATEMENT | MODE_NEST);
        startElement(SSTATIC_BLOCK);

    }

    STATIC lcurly

;

// C _Static_assert statement
static_assert_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the return statement
            startElement(SSTATIC_ASSERT_STATEMENT);

            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_ARGUMENT_LIST);
        }

        STATIC_ASSERT call_argument_list
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

// match a special yield specifier
yield_specifier[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        YIELD
;

// match a special yield specifier followed by return
yield_return_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the return statement
            startElement(SRETURN_STATEMENT);
        }
        yield_specifier RETURN
;

// match a special yield specifier followed by break;
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
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME);

            // start the break statement
            startElement(SBREAK_STATEMENT);
        }
        BREAK
;

// start of continue statement
continue_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME);

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

goto_case[] { LightweightElement element(this); ENTRY_DEBUG } :
    {

        startElement(SNAME);


    }
    CASE (literals | ~TERMINATE)*

;

// complete assembly declaration statement
asm_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the asm statement
            startElement(SASM);
        }
        ASM (options { greedy = true; } : { LA(1) == VOLATILE }? specifier)*
        ({ true }? paren_pair | ~(LCURLY | RCURLY | TERMINATE))*
;

// complete assembly declaration statement
visual_cxx_asm_declaration[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the asm statement
            startElement(SASM);
        }
        VISUAL_CXX_ASM ({ LA(1) == LCURLY}? visual_cxx_asm_declaration_curly_pair | (options { greedy = true; } : visual_cxx_asm_inner)*) (options { greedy = true; } : TERMINATE)*
;

visual_cxx_asm_declaration_curly_pair[] { ENTRY_DEBUG } :

    LCURLY (options { generateAmbigWarnings = false; } : visual_cxx_asm_declaration | visual_cxx_block_inner | ~(RCURLY))* RCURLY

;

visual_cxx_block_inner[] { CompleteElement element(this);  ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the asm statement
            startElement(SASM);
        }
        ({ LA(1) == LCURLY}? visual_cxx_asm_declaration_curly_pair | (visual_cxx_asm_inner (options { greedy = true; } : visual_cxx_asm_inner)*)) (options { greedy = true; } : TERMINATE)*

;

visual_cxx_asm_inner[] { ENTRY_DEBUG } :

     (~(EOL | TERMINATE | RCURLY | VISUAL_CXX_ASM))

;

extern_alias[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        ALIAS

;

// extern definition
extern_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_EXTERN);

            // start the namespace definition
            startElement(SEXTERN);
        }
        EXTERN (extern_alias (options { greedy = true; } : variable_identifier)*)*
;

// name of extern section
extern_name[] { ENTRY_DEBUG } :
        string_literal
        {
            // nest a block inside the namespace
            setMode(MODE_STATEMENT | MODE_NEST);
        }
;

namespace_inline_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        INLINE

;

// namespaces
namespace_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NAMESPACE | MODE_VARIABLE_NAME);

            // start the namespace definition
            startElement(SNAMESPACE);
        }
        (namespace_inline_specifier)* NAMESPACE
;

// a namespace alias
namespace_alias[] { ENTRY_DEBUG } :
        EQUAL
        {
            // expect a label name
            // label name is a subset of variable names
            setMode(MODE_VARIABLE_NAME);
        }
;

// namespace block
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
            startNewMode(MODE_STATEMENT | MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT | MODE_USING);

            // start the using directive
            startElement(SUSING_DIRECTIVE);
        }
        (options { greedy = true; } : { !isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) && next_token() == TEMPOPS }? template_declaration_full)*
        USING
;

using_aliasing[]  { int type_count;  int secondtoken = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startElement(SDECLARATION_INITIALIZATION);
        }
        EQUAL 

        (
            { pattern_check(stmt_type, secondtoken, type_count) && (stmt_type == FUNCTION_DECL || stmt_type == FUNCTION
            || stmt_type == OPERATOR_FUNCTION_DECL || stmt_type == OPERATOR_FUNCTION) }?
            {
                startElement(STYPE);
            }
            function_declaration[type_count] |

            set_int[type_count, type_count + 1] variable_declaration_type[type_count]
            )*

;

//  Objectice-C compatibility alias
compatibility_alias[] { ENTRY_DEBUG } :
    {

        // statement
        startNewMode(MODE_STATEMENT| MODE_VARIABLE_NAME);

        // start the namespace definition
        startElement(SCOMPATIBILITY_ALIAS);

    }
    COMPATIBILITY_ALIAS

;

//  Objectice-C @class directive
class_directive[] { ENTRY_DEBUG } :
    {

        // statement
        startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME | MODE_LIST);

        // start the namespace definition
        startElement(SCLASS_DECLARATION);

    }
    ATCLASS (identifier | COMMA)*

;

protocol_declaration[] { ENTRY_DEBUG } :

    {

        startNewMode(MODE_STATEMENT | MODE_LOCAL);

        startElement(SPROTOCOL_DECLARATION);

    }
   ATPROTOCOL (variable_identifier | COMMA)*

;

// Qt emit statement
emit_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement (after condition)
            startNewMode(MODE_STATEMENT);

            // start the while element
            startElement(SEMIT_STATEMENT);

            startNewMode(MODE_EXPRESSION | MODE_EXPECT);
        }
        EMIT
;

friend_statement[] { ENTRY_DEBUG } :
    {
    
        startNewMode(MODE_STATEMENT | MODE_NEST | MODE_FRIEND);

        startElement(SFRIEND);

    }
    FRIEND

;

/* Declarations Definitions CFG */

// check the ending token
check_end[int& token] { token = LA(1); ENTRY_DEBUG } :
        LCURLY | TERMINATE | COLON | COMMA | RPAREN | EQUAL
;

// handle a class declaration
class_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class definition
            startElement(SCLASS_DECLARATION);
        }

        class_preamble (CLASS | CXX_CLASS) class_post class_header
        (options { greedy = true; } : COMMA class_post class_header)*
;

// class preprocessing items
class_preprocessing[int token] { ENTRY_DEBUG } :
        {
            bool intypedef = inMode(MODE_TYPEDEF);

            if(intypedef)
                startElement(STYPE);

            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS | MODE_DECL);

            if(intypedef)
                setMode(MODE_INNER_DECL);

            // start the class definition
            startElement(token);
            // classes end at the end of the block
            if (intypedef || inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP)) {
                setMode(MODE_END_AT_BLOCK);
            }
        }
;

// handle stuff before CLASS token
class_preamble[] { ENTRY_DEBUG } :
        // suppress warning probably do to only having ()*
        (options { greedy = true; } : { inLanguage(LANGUAGE_JAVA) && next_token() != INTERFACE }? annotation | { inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |
        { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp)*
        (specifier | { LA(1) != TEMPLATE || next_token() != TEMPOPS }? template_specifier | { !isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) }? template_declaration_full)*
;

// a class definition
class_definition[] { ENTRY_DEBUG } :
        class_preprocessing[SCLASS]

        class_preamble (CLASS | CXX_CLASS) class_post (class_header lcurly | lcurly)
        {

            if (inLanguage(LANGUAGE_CXX))
                class_default_access_action(SPRIVATE_ACCESS_DEFAULT);
        }
;

// handle stuff after CLASS token
class_post[] { ENTRY_DEBUG } :
        (options { greedy = true; } : { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp)*
        (options { greedy = true; } : { LA(1) != CRESTRICT && LA(1) != MUTABLE }? specifier)*

;

objective_c_class[] { bool first = true; ENTRY_DEBUG } :

    {

        startNewMode(MODE_STATEMENT | MODE_CLASS);

        if(LA(1) == ATINTERFACE)
            startElement(SCLASS_INTERFACE);
        else
            startElement(SCLASS_IMPLEMENTATION);

        startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK  | MODE_TOP | MODE_CLASS);

    }

    (ATINTERFACE | ATIMPLEMENTATION) ({ first }? objective_c_class_header set_bool[first, false])*

    (lcurly
        {

            class_default_access_action(SPROTECTED_ACCESS_DEFAULT);

        }
    )*
;

protocol[] { ENTRY_DEBUG } :

    { look_past_rule(&srcMLParser::protocol_declaration) == TERMINATE }? protocol_declaration |
    protocol_definition

;



protocol_definition[] { bool first = true; ENTRY_DEBUG } :

    {

        startNewMode(MODE_STATEMENT | MODE_CLASS);

        startElement(SPROTOCOL);

        startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK  | MODE_TOP | MODE_CLASS);

    }

    ATPROTOCOL ({ first }? objective_c_class_header set_bool[first, false])*

    {

        class_default_access_action(SREQUIRED_DEFAULT);

    }

;

// handle class header
objective_c_class_header[] { ENTRY_DEBUG } :

        { isoption(parser_options, SRCML_OPTION_CPP) }?
        (macro_call_check class_header_base LCURLY)=>
           macro_call objective_c_class_header_base |

        objective_c_class_header_base
;

// class header base
objective_c_class_header_base[] { ENTRY_DEBUG } :

        // suppress ()* warning
        identifier

        // suppressed ()* warning
        (options { greedy = true; } : derived)*

        // suppressed ()* warning
        (category)*

        // suppressed ()* warning
        (protocol_list)*

;

objective_c_class_end[] { ENTRY_DEBUG } :

    {

        if(inTransparentMode(MODE_ACCESS_REGION)) {

            endDownToMode(MODE_TOP);

            // flush any whitespace tokens since sections should
            // end at the last possible place
            flushSkip();

            endWhileMode(MODE_TOP_SECTION);

        }


        endDownOverMode(MODE_TOP | MODE_CLASS);

    }

    ATEND

    {
        endDownOverMode(MODE_CLASS);
    }

;

// Handle an enum class
enum_class_definition[] { ENTRY_DEBUG } :
        class_preprocessing[SENUM]
        
        { setMode(MODE_ENUM); }

        class_preamble ENUM (class_header enum_block | enum_block)

;

// Handle an enum class
enum_class_declaration[] { ENTRY_DEBUG } :

        class_preprocessing[SENUM_DECLARATION]
        
        class_preamble ENUM class_post class_header
        (options { greedy = true; } : COMMA class_post class_header)*
;

// anonymous class definition
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

// super within an anonymous class
anonymous_class_super[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the super name of an anonymous class
            startElement(SDERIVATION_LIST);
        }
        compound_name_inner[false]
;

// do an interface definition
interface_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS);

            // start the interface definition
            startElement(SINTERFACE);

            // java interfaces end at the end of the block
            setMode(MODE_END_AT_BLOCK);
        }

        class_preamble INTERFACE class_post class_header lcurly
;


// do an interface declaration
interface_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the interface definition
            startElement(SINTERFACE_DECLARATION);
        }

        class_preamble INTERFACE class_post class_header
        (options { greedy = true; } : COMMA class_post class_header)*
;

// match struct declaration
struct_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class definition
            startElement(SSTRUCT_DECLARATION);
        }

        class_preamble STRUCT class_post class_header
        (options { greedy = true; } : COMMA class_post class_header)*
;

// handle struct union definition
struct_union_definition[int element_token] { ENTRY_DEBUG } :
        class_preprocessing[element_token]

        class_preamble (STRUCT | UNION) class_post (class_header lcurly | lcurly)
        {
           if (inLanguage(LANGUAGE_CXX))
               class_default_access_action(SPUBLIC_ACCESS_DEFAULT);
        }
;

// process union declaration beginning
union_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class definition
            startElement(SUNION_DECLARATION);
        }
        class_preamble UNION class_post class_header
        (options { greedy = true; } : COMMA class_post class_header)*
;

annotation_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS);

            // start the interface definition
            startElement(SANNOTATION_DEFN);

            // java interfaces end at the end of the block
            setMode(MODE_END_AT_BLOCK);
        }
        class_preamble ATSIGN INTERFACE class_header lcurly
;

// default private/public section for C++
class_default_access_action[int access_token] { ENTRY_DEBUG } :
        {
            if ((inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_OBJECTIVE_C)) && (SkipBufferSize() > 0 ||
                !(LA(1) == PUBLIC || LA(1) == PRIVATE || LA(1) == PROTECTED || LA(1) == SIGNAL
                    || LA(1) == ATREQUIRED || LA(1) == ATOPTIONAL))) {

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

// handle class header
class_header[] { ENTRY_DEBUG } :

        { isoption(parser_options, SRCML_OPTION_CPP) && next_token() != DCOLON }?
        (macro_call_check class_header_base LCURLY)=>
           macro_call class_header_base |

        class_header_base
;

// class header base
class_header_base[] { bool insuper = false; ENTRY_DEBUG } :

    {

        setMode(MODE_CLASS_NAME);

    }

        // suppress ()* warning
        ({ LA(1) != FINAL }? compound_name | keyword_name)

    {

        clearMode(MODE_CLASS_NAME);

    }

        (options { greedy = true; } : { LA(1) == FINAL }? specifier)*

        ({ inLanguage(LANGUAGE_CXX_FAMILY) }? (options { greedy = true; } : derived))*

        // move suppressed ()* warning to begin
        (options { greedy = true; } : { inLanguage(LANGUAGE_CXX_FAMILY) }? generic_type_constraint)*

        ({ inLanguage(LANGUAGE_JAVA_FAMILY) }? (options { greedy = true; } : super_list_java { insuper = true; } 
            (extends_list | implements_list) (options { greedy = true; } : extends_list | implements_list)*))*
        {
            if (insuper)
                endMode();
        }
;

// Each instance of an access specifier defines a region in the class
access_specifier_region[] { bool first = true; ENTRY_DEBUG } :
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
            SIGNAL |
            {
                startElement(SREQUIRED);
            }
            ATREQUIRED |
            {
                startElement(SOPTIONAL);
            }
            ATOPTIONAL

        ) 
    ({ !inLanguage(LANGUAGE_OBJECTIVE_C) && first }? (compound_name)* COLON set_bool[first, false])*
;

/*
  left curly brace

  Marks the start of a block.  End of the block is handled in right curly brace
*/
lcurly[] { ENTRY_DEBUG } :
        {

            // special end for conditions
            if (inTransparentMode(MODE_CONDITION) && !inMode(MODE_ANONYMOUS)) {
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
            bool inclassmode = (inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP)) && inMode(MODE_CLASS);

            bool in_function_body = inTransparentMode(MODE_FUNCTION_TAIL);

            startNewMode(MODE_BLOCK);

            if (inclassmode)
                setMode(MODE_CLASS);

            if(in_function_body)
                setMode(MODE_FUNCTION_BODY);

            startElement(SBLOCK);

            ++curly_count;

        }
        LCURLY
        set_bool[skip_ternary, false]
;

// end of a block.  Also indicates the end of some open elements.
block_end[] { bool in_issue_empty = inTransparentMode(MODE_ISSUE_EMPTY_AT_POP); ENTRY_DEBUG } :
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
            bool anonymous_class = (inMode(MODE_CLASS) | inMode(MODE_ENUM)) && inMode(MODE_END_AT_BLOCK);

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

            if (!(anonymous_class) && (!(inMode(MODE_CLASS) || inMode(MODE_ENUM)) || endstatement))
                else_handling();

            // then we needed to markup the (abbreviated) variable declaration
            if (inMode(MODE_DECL) && LA(1) != TERMINATE)
                short_variable_declaration();

            if(!in_issue_empty && inMode(MODE_END_AT_ENDIF | MODE_TOP | MODE_STATEMENT))
                endMode();

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
            endWhileMode(MODE_TOP_SECTION);

            if(getCurly() != 0)
                decCurly();

            --curly_count;

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

// match the actual terminate token
terminate_token[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (inMode(MODE_STATEMENT | MODE_NEST) && (!inMode(MODE_DECL) && !inTransparentMode(MODE_FRIEND)
            && (!inLanguage(LANGUAGE_JAVA) || !inMode(MODE_ENUM | MODE_LIST))))
                startElement(SEMPTY);

            wait_terminate_post = true;

        }
        TERMINATE
        set_bool[skip_ternary, false]
;

// do the pre terminate processing
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

// do the post terminate processing
terminate_post[] {  bool in_issue_empty = inTransparentMode(MODE_ISSUE_EMPTY_AT_POP); ENTRY_DEBUG } :
        {

            // end all statements this statement is nested in
            // special case when ending then of if statement
            if (!isoption(parser_options, SRCML_OPTION_EXPRESSION) &&
                 (!inMode(MODE_EXPRESSION_BLOCK) || inMode(MODE_EXPECT)) &&
                !inMode(MODE_INTERNAL_END_CURLY) && !inMode(MODE_INTERNAL_END_PAREN)
                && !inMode(MODE_STATEMENT | MODE_ISSUE_EMPTY_AT_POP)
                && !inMode(MODE_END_AT_ENDIF)) {

                // end down to either a block or top section, or to an if or else
                endDownToModeSet(MODE_TOP | MODE_IF | MODE_ELSE | MODE_SWITCH);

            }

        }

        else_handling

        {

            if(inMode(MODE_SWITCH))
                endMode();

            if(inMode(MODE_STATEMENT | MODE_ISSUE_EMPTY_AT_POP))
                endMode();

            if(!in_issue_empty && inMode(MODE_END_AT_ENDIF))
                endMode();

            wait_terminate_post = false;

        }

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
            std::deque<int>::size_type cppmode_size = !cppmode.empty() ? cppmode.top().statesize.size() : 0;

            // catch and finally statements are nested inside of a try, if at that level
            // so if no CATCH or FINALLY, then end now
            bool intry = inMode(MODE_TRY);
            bool restoftry = LA(1) == CATCH || LA(1) == CXX_CATCH || LA(1) == FINALLY;
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

                    // when an else but not elseif
                    while (inMode(MODE_ELSE) && !inMode(MODE_IF)) {

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

                    // if in elseif then end it
                    if(inMode(MODE_IF | MODE_ELSE)) {
                        endMode();
                        --ifcount;
                    }

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
                   CALL_TYPE type = NOCALL;  bool isempty = false; int call_count = 0; ENTRY_DEBUG } :

        { inMode(MODE_EAT_TYPE) }?
        type_identifier
        update_typecount[MODE_FUNCTION_NAME] |

        // block right after argument list, e.g., throws list in Java
        { inTransparentMode(MODE_END_LIST_AT_BLOCK) }?
        { endDownToMode(MODE_LIST); endMode(MODE_LIST); }
        lcurly |


        { inMode(MODE_ENUM) && inMode(MODE_LIST) }?
        enum_short_variable_declaration |

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
        { (inLanguage(LANGUAGE_OO)) }?
        throw_list complete_arguments (comma complete_arguments)* { endDownToMode(MODE_LIST); endMode(MODE_LIST); } |

        // throw list at end of function header
        { (inLanguage(LANGUAGE_CXX))&& inMode(MODE_FUNCTION_TAIL) }?
        ref_qualifier |

        // throw list at end of function header
        { (inLanguage(LANGUAGE_CXX))&& inTransparentMode(MODE_FUNCTION_TAIL) }?
        noexcept_list |

        // throw list at end of function header
        { (inLanguage(LANGUAGE_CXX))&& inTransparentMode(MODE_FUNCTION_TAIL) && next_token() == LBRACKET }?
        attribute_cpp |

        // K&R function parameters
        { (inLanguage(LANGUAGE_C) || inLanguage(LANGUAGE_CXX)) && inMode(MODE_FUNCTION_TAIL) &&
          pattern_check(stmt_type, secondtoken, type_count) && stmt_type == VARIABLE }?
        kr_parameter[type_count] |

        // function try block, must be before function_specifier
        { inLanguage(LANGUAGE_CXX_FAMILY) && inMode(MODE_FUNCTION_TAIL) }?
        try_statement |

        // function specifier at end of function header
        { inLanguage(LANGUAGE_CXX_FAMILY) && inMode(MODE_FUNCTION_TAIL)
            && (LA(1) != EQUAL || (inLanguage(LANGUAGE_CXX) && (next_token() == CONSTANTS || next_token() == DEFAULT || next_token() == DELETE))) }?
        function_specifier |

        { inMode(MODE_FUNCTION_TAIL) }?
        function_pointer_initialization |

        // function specifier at end of function header
        { inLanguage(LANGUAGE_CXX) && inMode(MODE_FUNCTION_TAIL) }?
        trailing_return |

        { inLanguage(LANGUAGE_JAVA) && inMode(MODE_FUNCTION_TAIL) }?
        annotation_default |

        { inTransparentMode(MODE_OBJECTIVE_C_CALL | MODE_ARGUMENT_LIST) }?
        (function_identifier (COLON | RBRACKET)) => objective_c_call_message |

        { inTransparentMode(MODE_OBJECTIVE_C_CALL) }?
        (function_identifier (COLON | RBRACKET) | COLON) => objective_c_call_argument |

        // start of argument for return or throw statement
        { inMode(MODE_EXPRESSION | MODE_EXPECT) &&
            isoption(parser_options, SRCML_OPTION_CPP) && perform_call_check(type, isempty, call_count, secondtoken) && type == MACRO }?
        macro_call |

        { inMode(MODE_EXPRESSION | MODE_EXPECT) }?
        expression[type, call_count] |

        // already in an expression, and run into a keyword
        // so stop the expression, and markup the keyword statement
        { inMode(MODE_EXPRESSION)
         && !(inLanguage(LANGUAGE_OBJECTIVE_C) && LA(1) == IMPORT)
         && !(LA(1) == ATPROTOCOL && next_token() == LPAREN)
         && (LA(1) != DEFAULT || next_token() == COLON)
         && (LA(1) != CHECKED || next_token() == LCURLY)
         && (LA(1) != UNCHECKED || next_token() == LCURLY)
         && (LA(1) != CXX_TRY || next_token() == LCURLY)
         && (LA(1) != INLINE || next_token() == NAMESPACE)
         && (LA(1) != STATIC || (inLanguage(LANGUAGE_JAVA) && next_token() == LCURLY))
         && (LA(1) != CXX_CATCH || next_token() == LPAREN || next_token() == LCURLY)
         && (LA(1) != ASM || look_past_two(ASM, VOLATILE) == LPAREN) }?
        terminate_pre
        terminate_post
        keyword_statements |
        // already in an expression
        { inMode(MODE_EXPRESSION) }?
        expression_part_plus_linq |

        // call list in member initialization list
        { inMode(MODE_CALL | MODE_LIST) && (LA(1) != LCURLY || inLanguage(LANGUAGE_CXX)) }?
        member_init |

        // call list in member initialization list
        { inMode(MODE_CALL | MODE_LIST) && (LA(1) != LCURLY || inLanguage(LANGUAGE_CXX)) }?
        keyword_calls |

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
        { inMode(MODE_INIT | MODE_EXPECT) && ((LA(1) == COLON && ((inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP)) || inLanguage(LANGUAGE_JAVA)))
                || LA(1) == IN) }?
        variable_declaration_range |

        // in an argument list expecting an argument
        { inMode(MODE_ARGUMENT | MODE_LIST) }?
        argument |

        // start of condition for if/while/switch
        { inMode(MODE_PARAMETER | MODE_EXPECT) }?
        parameter |

        { inMode(MODE_VARIABLE_NAME) }?
        goto_case | 

        /*
          Check for MODE_FOR_CONDITION before template stuff, since it can conflict
        */

        // inside of for group expecting initialization
        { inMode(MODE_FOR_CONTROL | MODE_EXPECT) }?
        for_control |

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
        colon_marked |

        // markup namespace tag in using namespace
        namespace_definition

;

// mark ( operator
lparen_marked[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            incParen();

            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        LPAREN
;

// marking | operator
bar[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if ((!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR)) && !inMode(MODE_PARAMETER))
                startElement(SOPERATOR);
        }
        BAR
;

// handle comma
comma[] { bool markup_comma = true; ENTRY_DEBUG } :
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

            if(inMode(MODE_LOCAL | MODE_VARIABLE_NAME) && !inTransparentMode(MODE_FOR_CONDITION | MODE_END_AT_COMMA))
                endMode();

            if(inTransparentMode(MODE_ENUM) && inMode(MODE_INIT | MODE_EXPECT))
                endDownToModeSet(MODE_ENUM | MODE_TOP);

            if(inMode(MODE_INIT | MODE_VARIABLE_NAME | MODE_LIST) || inTransparentMode(MODE_FOR_CONDITION | MODE_END_AT_COMMA))
                markup_comma = false;

        }
        comma_marked[markup_comma]
        {
            if(inTransparentMode(MODE_FOR_CONDITION | MODE_END_AT_COMMA)) {

                startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);
                startNoSkipElement(SDECLARATION_RANGE);

            }
        }

;

// marking comma operator
comma_marked[bool markup_comma = true] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (markup_comma && ((!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                 && !inMode(MODE_PARAMETER) && !inMode(MODE_ARGUMENT) && !(inTransparentMode(MODE_IN_INIT) && inMode(MODE_EXPRESSION | MODE_LIST)))
                && !inMode(MODE_ENUM) && !inMode(MODE_INTERNAL_END_CURLY) && !inMode(MODE_INITIALIZATION_LIST))
                startElement(SOPERATOR);
        }
        COMMA
;

// mark COLON
colon_marked[] { bool in_ternary = inTransparentMode(MODE_TERNARY | MODE_THEN); bool markup_colon = true; LightweightElement element(this); ENTRY_DEBUG } :
        {

            if(in_ternary) {

                endDownToMode(MODE_THEN);
                flushSkip();
                endMode(MODE_THEN);
                startNewMode(MODE_ELSE | MODE_EXPRESSION | MODE_EXPECT);
                startElement(SELSE);

                markup_colon = false;

            }

            // only needed when ranged for and not a declaration
            if(inTransparentMode(MODE_RANGED_FOR)) {

                // start a new mode that will end after the argument list
                startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

                // start the initialization element
                startElement(SDECLARATION_RANGE);

                markup_colon = false;

            }

            if(inLanguage(LANGUAGE_OBJECTIVE_C) && inTransparentMode(MODE_INTERNAL_END_CURLY)) {

                endDownToMode(MODE_INTERNAL_END_CURLY);

            }

            if (markup_colon && !(in_ternary && isoption(parser_options, SRCML_OPTION_TERNARY))
                && (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                && (!inLanguage(LANGUAGE_OBJECTIVE_C) || !inMode(MODE_INTERNAL_END_CURLY)))
                startElement(SOPERATOR);

        }

        COLON
;

// process colon not marked.
colon[] { ENTRY_DEBUG } :
        {
            // colon ends the current item in a list
            if (inTransparentMode(MODE_TOP_SECTION))
                endDownToMode(MODE_TOP_SECTION);

        }
        COLON
        {
            if(inMode(MODE_DETECT_COLON))
                endMode(MODE_DETECT_COLON);
        }
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

        {
            int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; 
            pattern_check(stmt_type, secondtoken, type_count);
            if(stmt_type == VARIABLE) {
                startNewMode(MODE_INTERNAL_END_PAREN);
                for_initialization_variable_declaration(type_count);
            }

        }

;

// perform an arbitrary look ahead looking for a pattern
pattern_check[STMT_TYPE& type, int& token, int& type_count, bool inparam = false] returns [bool isdecl] {

    isdecl = true;

    /** @todo may need to also pass and check other such as template and attribute/annotations */
    int specifier_count;
    int attribute_count;
    int template_count;
    type = NONE;

    int start = mark();
    inputState->guessing++;

    bool sawtemplate;
    bool sawcontextual;
    int posin = 0;
    int fla = 0;

    try {

        pattern_check_core(token, fla, type_count, specifier_count, attribute_count, template_count, type, inparam, sawtemplate, sawcontextual, posin);

    } catch (...) {

        if (type == VARIABLE && type_count == 0) {
            type_count = 1;
        }

    }

    if(type == VARIABLE && inTransparentMode(MODE_CONDITION) && LA(1) != EQUAL)
        type = NONE;

    if(type == NONE && (sawtemplate || (sawcontextual && type_count > 0))
     && (!keyword_name_token_set.member(LA(1)) || LA(1) == MULTOPS || LA(1) == REFOPS || LA(1) == RVALUEREF || LA(1) == TERMINATE))
        type = VARIABLE;
    
    // may just have an expression
    if (type == VARIABLE && posin)
        type_count = posin - 1;

    // may just have a single macro (no parens possibly) before a statement
    else if (type == 0 && type_count == 0 && keyword_token_set.member(LA(1)))
        type = SINGLE_MACRO;

    else if(type == 0 && type_count == 1 && (LA(1) == CLASS || LA(1) == CXX_CLASS || LA(1) == STRUCT || LA(1) == UNION)) {

        pattern_check(type, token, type_count, inparam);
        type_count += 1;

        if(type == CLASS_DECL || type == CLASS_DEFN || type == UNION_DECL || type == UNION_DEFN || type == STRUCT_DECL || type == STRUCT_DEFN || type == ENUM_DECL || type == ENUM_DEFN || type == 0) {

            type = SINGLE_MACRO;
            type_count = 1;

        }

    }

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
    else if (type == FUNCTION && (fla == TERMINATE || fla == COMMA || fla == EQUAL))
        type = FUNCTION_DECL;

    // declaration form
    else if (type == OPERATOR_FUNCTION && (fla == TERMINATE || fla == COMMA || fla == EQUAL))
        type = OPERATOR_FUNCTION_DECL;

    // we actually have a macro and then a constructor
    else if(type == FUNCTION && fla == COLON)
        type = SINGLE_MACRO;

    // not really a destructor
    if (type == DESTRUCTOR_DECL && (!inTransparentMode(MODE_CLASS) || inTransparentMode(MODE_FUNCTION_TAIL)))
        type = EXPRESSION;

    if((type == FUNCTION || type == FUNCTION_DECL) && fla == COMMA && !inparam)
        type = VARIABLE;

    int save_la = LA(1);

    inputState->guessing--;
    rewind(start);

    if(type == VARIABLE && type_count == (specifier_count + attribute_count + template_count))
        ++type_count;

    if(!inMode(MODE_FUNCTION_TAIL) && type == 0 && type_count == 0 
       && (enum_preprocessing_token_set.member(LA(1)) || LA(1) == DECLTYPE) && (!inLanguage(LANGUAGE_CXX) || !(LA(1) == FINAL || LA(1) == OVERRIDE))
       && save_la == TERMINATE) {

        type = VARIABLE;
        type_count = 1;

    }

    if(type == NONE && LA(1) == TEMPLATE)
        type = GLOBAL_TEMPLATE;

} :;

/*
  Figures out if we have a declaration, either variable or function.

  This is pretty complicated as it has to figure out whether it is a declaration or not,
  and whether it is a function or a variable declaration.
*/
pattern_check_core[int& token,      /* second token, after name (always returned) */
              int& fla,             /* for a function, TERMINATE or LCURLY, 0 for a variable */
              int& type_count,      /* number of tokens in type (not including name) */
              int& specifier_count, /* number of tokens that are specifiers */
              int& attribute_count, /* number of tokens that are attributes */
              int& template_count,  /* number of tokens that are templates */
              STMT_TYPE& type,      /* type discovered */
              bool inparam,         /* are we in a parameter */
              bool& sawtemplate,    /* have we seen a template */
              bool& sawcontextual,  /* have we seen a contextual keyword */
              int& posin            /* */
        ] {
            token = 0;
            int parameter_pack_pos = -1;
            fla = 0;
            type_count = 0;
            specifier_count = 0;
            attribute_count = 0;
            template_count = 0;
            type = NONE;
            sawtemplate = false;
            sawcontextual= false;
            posin = 0;
            isdestructor = false;           // global flag detected during name matching
            bool foundpure = false;
            bool isoperator = false;
            bool ismain = false;
            bool isconstructor = false;
            bool saveisdestructor = false;
            bool endbracket = false;
            bool modifieroperator = false;
            bool is_c_class_identifier = false;
            is_qmark = false;
            int real_type_count = 0;
            bool lcurly = false;
            bool is_event = false;
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
        (
        ({ ((inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP) || (type_count == 0)) || (LA(1) != LBRACKET || next_token() == LBRACKET))
         && (LA(1) != IN || !inTransparentMode(MODE_FOR_CONDITION)) }?

            set_bool[is_qmark, (is_qmark || (LA(1) == QMARK)) && inLanguage(LANGUAGE_CSHARP)]

            set_int[posin, LA(1) == IN ? posin = type_count : posin]
            set_int[parameter_pack_pos, LA(1) == DOTDOTDOT ? parameter_pack_pos = type_count : parameter_pack_pos]

            set_bool[isoperator, isoperator || LA(1) == OPERATOR]

            // was their a bracket on the end?  Need to know for Java
            set_bool[endbracket, inLanguage(LANGUAGE_JAVA_FAMILY) && LA(1) == LBRACKET]

            // record any type modifiers that are also operators
            // this is for disambiguation of destructor declarations from expressions involving
            // the ~ operator
            set_bool[modifieroperator, modifieroperator || LA(1) == REFOPS || LA(1) == MULTOPS || LA(1) == QMARK]

            set_bool[sawcontextual, sawcontextual || LA(1) == CRESTRICT || LA(1) == MUTABLE]
            (

                { 
#ifdef SRCML_BOOST_MPL_LARGE
                    argument_token_set.member(LA(1))
#else
                    (argument_token_set_one.member(LA(1)) || argument_token_set_two.member(LA(1)) || argument_token_set_three.member(LA(1)))
#endif
                    && (LA(1) != SIGNAL || (LA(1) == SIGNAL && look_past(SIGNAL) == COLON)) && (!inLanguage(LANGUAGE_CXX) || (LA(1) != FINAL && LA(1) != OVERRIDE))
                     && (LA(1) != TEMPLATE || next_token() != TEMPOPS) && (LA(1) != ATOMIC || next_token() != LPAREN)
                 }?
                set_int[token, LA(1)]
                set_bool[foundpure, foundpure || (LA(1) == CONST || LA(1) == TYPENAME)]
                (options { generateAmbigWarnings = false; } : EXTERN (options { greedy = true; } : ALIAS set_int[specifier_count, specifier_count + 1])* |
                    { LA(1) != NEW || (inLanguage(LANGUAGE_CSHARP) && (inPrevMode(MODE_CLASS) || specifier_count > 0)) }? specifier | template_specifier set_bool[sawtemplate, true] |
                    { next_token() == COLON }? SIGNAL | ATREQUIRED | ATOPTIONAL | { inLanguage(LANGUAGE_JAVA) }? default_specifier)
                set_int[specifier_count, specifier_count + 1]
                set_type[type, ACCESS_REGION,
                        ((inLanguage(LANGUAGE_CXX) && look_past_two(NAME, VOID) == COLON) || inLanguage(LANGUAGE_OBJECTIVE_C)) 
                        && (token == PUBLIC || token == PRIVATE || token == PROTECTED || token == SIGNAL || token == ATREQUIRED || token == ATOPTIONAL)]
                throw_exception[type == ACCESS_REGION] |
                { !isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) }? template_declaration_full set_int[template_count, template_count + 1] | 

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

                { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}?
                LBRACKET LBRACKET

                        //complete_expression
                        (~(RBRACKET))*
                RBRACKET RBRACKET
                set_type[type, GLOBAL_ATTRIBUTE]
                set_int[attribute_count, attribute_count + 1] |

                { type_count == (attribute_count + specifier_count) }?
                property_method_name
                set_type[type, PROPERTY_ACCESSOR, true] |

                { type_count == attribute_count + specifier_count + template_count  && (!inLanguage(LANGUAGE_JAVA) 
            || (inLanguage(LANGUAGE_JAVA) && (LA(1) != ATSIGN 
                                             || (LA(1) == ATSIGN && next_token() == INTERFACE))))
                                              && (!inLanguage(LANGUAGE_CXX)
                                               || (!keyword_name_token_set.member(next_token())
                                                || (next_token() == LBRACKET && next_token_two() == LBRACKET)))
                                               }?
                (CLASS               set_type[type, CLASS_DECL]       |
                 CXX_CLASS           set_type[type, CLASS_DECL]       |
                 STRUCT              set_type[type, STRUCT_DECL]      |
                 UNION               set_type[type, UNION_DECL]       |
                 INTERFACE           set_type[type, INTERFACE_DECL]   |
                 ATSIGN INTERFACE    set_type[type, ANNOTATION_DEFN])
                set_bool[lcurly, LA(1) == LCURLY]
                (options { greedy = true; } : { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp)*
                ({ LA(1) == DOTDOTDOT }? DOTDOTDOT set_int[type_count, type_count + 1])*
                class_post
                (class_header | LCURLY)
                set_type[type, CLASS_DEFN,     type == CLASS_DECL     && (LA(1) == LCURLY || lcurly)]
                set_type[type, STRUCT_DEFN,    type == STRUCT_DECL    && (LA(1) == LCURLY || lcurly)]
                set_type[type, UNION_DEFN,     type == UNION_DECL     && (LA(1) == LCURLY || lcurly)]
                set_type[type, INTERFACE_DEFN, type == INTERFACE_DECL && (LA(1) == LCURLY || lcurly)]
                set_type[type, NONE, !(LA(1) == TERMINATE || LA(1) == COMMA || LA(1) == LCURLY || lcurly)]
                throw_exception[type != NONE]
                set_bool[foundpure]
                set_int[type_count, type_count + 1] |

                { type_count == attribute_count + specifier_count + template_count }?
                (ENUM set_type[type, ENUM_DECL])
                set_bool[lcurly, LA(1) == LCURLY]
                (options { greedy = true; } : { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp)*
                ({ LA(1) == DOTDOTDOT }? DOTDOTDOT set_int[type_count, type_count + 1])*
                ({ inLanguage(LANGUAGE_JAVA) }? class_header | { inLanguage(LANGUAGE_CSHARP)}? variable_identifier (derived)* | enum_class_header | LCURLY)
                set_type[type, ENUM_DEFN, type == ENUM_DECL && (LA(1) == LCURLY || lcurly)]
                set_type[type, NONE, !(LA(1) == TERMINATE || LA(1) == COMMA || LA(1) == LCURLY || lcurly)]
                throw_exception[type != NONE]
                set_bool[foundpure]
                set_int[type_count, type_count + 1] |

                (
                    USING set_type[type, USING_STMT]
                    throw_exception[true]
                ) |

                { inLanguage(LANGUAGE_JAVA_FAMILY) }?
                generic_argument_list set_int[specifier_count, specifier_count + 1] |

                { inLanguage(LANGUAGE_JAVA_FAMILY) }?
                annotation
                set_int[attribute_count, attribute_count + 1] |

                // typical type name
                { !inLanguage(LANGUAGE_CSHARP) || LA(1) != ASYNC }?
                set_bool[operatorname, false]
                compound_name set_bool[foundpure]
                set_bool[isoperator, isoperator || (inLanguage(LANGUAGE_CXX_FAMILY) && 
                             operatorname)] 
                set_bool[operatorname, false] |

                // always count as a name for now since is always used as a type or type modifier
                auto_keyword[false] | 

                EVENT set_bool[is_event] |

                // special function name
                MAIN set_bool[ismain, type_count == 0] |

                { is_c_class_identifier || keyword_name_token_set.member(next_token()) }?
                     keyword_name |

        { inLanguage(LANGUAGE_JAVA) && inMode(MODE_PARAMETER) }? bar |

                // type parts that can occur before other type parts (excluding specifiers)
                // do not match a struct class or union.  If was class/struct/union decl will not reach here.
                // if elaborated type specifier should also be handled above. Reached here because 
                // non-specifier then class/struct/union.
                { LA(1) != LBRACKET && (LA(1) != CLASS && LA(1) != CXX_CLASS && LA(1) != STRUCT && LA(1) != UNION) }?
                ({ LA(1) == DECLTYPE }? type_specifier_call | { next_token() == LPAREN }? atomic | pure_lead_type_identifier_no_specifiers) set_bool[foundpure] |

                // type parts that must only occur after other type parts (excluding specifiers)
                non_lead_type_identifier throw_exception[!foundpure]
            )

            // another type part
            set_int[type_count, type_count + 1]

            // record second (before we parse it) for label detection
            set_int[token, LA(1), type_count == 1]
        )*

        // special case for property attributes as names, e.g., get, set, etc.
        throw_exception[type == PROPERTY_ACCESSOR && (type_count == attribute_count + specifier_count + 1) && LA(1) == LCURLY]
        set_type[type, PROPERTY_ACCESSOR_DECL, type == PROPERTY_ACCESSOR]
        throw_exception[type == PROPERTY_ACCESSOR_DECL && (type_count == attribute_count + specifier_count + 1) && LA(1) == TERMINATE]
        set_type[type, NONE, type == PROPERTY_ACCESSOR_DECL]

        set_int[real_type_count, type_count]

        // special case for ternary operator on its own
        throw_exception[LA(1) == COLON && is_qmark]

        // adjust specifier tokens to account for keyword async used as name (only for C#)
        set_int[specifier_count, token == ASYNC ? specifier_count - 1 : specifier_count]

        // adjust type tokens to eliminate for last left bracket (only for Java)
        set_int[type_count, endbracket ? type_count - 1 : type_count]

        // have a sequence of type tokens, last one is function/variable name
        // (except for function pointer, which is handled later).
        // Using also has no name so counter operation.
        set_int[type_count, inMode(MODE_USING) ? type_count + 1 : type_count]

        set_int[type_count, type_count > 1 && inLanguage(LANGUAGE_CXX) && parameter_pack_pos >= 0
             && parameter_pack_pos == (type_count - 1) ? type_count + 1 : type_count]

        set_int[type_count, type_count > 1 ? type_count - 1 : 0]

        // special case for what looks like a destructor declaration
        // @todo need a case where == 1 then , merge it with > 1
        throw_exception[isdestructor && (modifieroperator || (type_count - specifier_count - attribute_count - template_count) > 1 || ((type_count - specifier_count - attribute_count - template_count) == 1))]

        // check if an event
        set_type[type, EVENT_STMT, is_event]
        throw_exception[is_event]

        // check if property
        set_type[type, PROPERTY_STMT, inLanguage(LANGUAGE_CSHARP) && (type_count - specifier_count) > 0 && LA(1) == LCURLY]
        throw_exception[type == PROPERTY_STMT]

        /*
          We have a declaration (at this point a variable) if we have:

            - At least one non-specifier in the type
            - There is nothing in the type (what was the name is the type)
              and it is part of a parameter list

            For now attribute and template counts are left out on purpose.
        */
        /*! @todo verify this is correct */
        set_type[type, VARIABLE, ((((type_count - specifier_count - template_count) > 0 && LA(1) != OPERATORS && LA(1) != CSPEC && LA(1) != MSPEC
                && ((inLanguage(LANGUAGE_CXX) && !inMode(MODE_ACCESS_REGION)) || LA(1) == TERMINATE || LA(1) == COMMA || LA(1) == BAR || LA(1) == LBRACKET
                                              || (LA(1) == LPAREN && next_token() != RPAREN) || LA(1) == LCURLY || LA(1) == EQUAL || LA(1) == IN
                                              || ((inTransparentMode(MODE_FOR_CONDITION) || inLanguage(LANGUAGE_C) || inLanguage(LANGUAGE_CXX)) && LA(1) == COLON)
                                              || (inLanguage(LANGUAGE_CSHARP) && LA(1) == RBRACKET)))) ||
                                                (inparam && (LA(1) == RPAREN || LA(1) == COMMA || LA(1) == BAR || LA(1) == LBRACKET || LA(1) == EQUAL || LA(1) == IN
                                                    || (inLanguage(LANGUAGE_CSHARP) && LA(1) == RBRACKET))))]

        // need to see if we possibly have a constructor/destructor name, with no type
        set_bool[isconstructor,

                 // operator methods may not have non-specifier types also
                 !isoperator &&

                 !ismain &&

                 !isdestructor &&

                 !inLanguage(LANGUAGE_OBJECTIVE_C) &&

                 // entire type is specifiers
                 (type_count == (specifier_count + attribute_count + template_count)) &&

                 (
                    // inside of a C++ class definition
                    inMode(MODE_ACCESS_REGION) ||

                    (inTransparentMode(MODE_ACCESS_REGION) && inMode(MODE_TEMPLATE)) ||

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

            (
                // check for function pointer, which must have a non-specifier part of the type
                { (inLanguage(LANGUAGE_C) || inLanguage(LANGUAGE_CXX)) && real_type_count > 0 }?
                (function_pointer_name_grammar eat_optional_macro_call LPAREN)=>
                function_pointer_name_grammar

                // what was assumed to be the name of the function is actually part of the type
                set_int[type_count, type_count + 1]

                // this ain't a constructor
                set_bool[isconstructor, false]

                function_rest[fla] |

                // POF (Plain Old Function)
                // need at least one non-specifier in the type (not including the name)
                { (type_count - specifier_count - attribute_count - template_count > 0) || isoperator || ismain || saveisdestructor || isconstructor}?
                function_rest[fla]
            ) |

            { real_type_count == 0 && specifier_count == 0 && attribute_count == 0 }? (objective_c_method set_int[fla, LA(1)] throw_exception[fla != TERMINATE && fla != LCURLY])

        )
    
        // default to variable in function body.  However, if anonymous function (does not end in :) not a variable
        throw_exception[inTransparentMode(MODE_FUNCTION_BODY) && type == VARIABLE && fla == TERMINATE]

        // since we got this far, we have a function
        set_type[type, FUNCTION, !isoperator]

        set_type[type, OPERATOR_FUNCTION, isoperator]

        // however, we could have a destructor
        set_type[type, DESTRUCTOR, saveisdestructor]

        // could also have a constructor
        set_type[type, CONSTRUCTOR, isconstructor && !saveisdestructor && !isoperator && !ismain]
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


trace_int[int s] { std::cerr << "HERE " << s << std::endl; } :;
/*traceLA { std::cerr << "LA(1) is " << LA(1) << " " << LT(1)->getText() << std::endl; } :;
marker[] { CompleteElement element(this); startNewMode(MODE_LOCAL); startElement(SMARKER); } :;
*/

// update type count
update_typecount[srcMLState::MODE_TYPE mode] {} :
        {
            decTypeCount();
            if(inTransparentMode(MODE_ARGUMENT) && inLanguage(LANGUAGE_CXX))
                return;

            if (getTypeCount() <= 0) {
                endMode();
                setMode(mode);
            }
        }
;

// count type identifiers
type_identifier_count[int& type_count] { ++type_count; ENTRY_DEBUG } :

        // overloaded parentheses operator
        { LA(1) == OPERATOR /* turns off ANTLR warning, and is nooped */ }?
        overloaded_operator |
        type_identifier |
        MAIN
;

// check the type identifier count
type_identifier_count_check returns [int type_count] {


    int start = mark();
    ++inputState->guessing;

    type_count = type_identifier_count_check_core();

    rewind(start);
    --inputState->guessing;
} :;

// core functionality for type identifier count check
type_identifier_count_check_core returns [int type_count] { type_count = 0; ENTRY_DEBUG } :

        (type_identifier_count[type_count])*

;

/*
// --a;
deduct[int& type_count] { --type_count; } :;
*/

// consume a type
eat_type[int & count] { if (count <= 0 || LA(1) == BAR) return; ENTRY_DEBUG } :

        // Mark as name before mark without name
        (options { generateAmbigWarnings = false;} :  keyword_name | type_identifier)

        set_int[count, count - 1]
        eat_type[count]

;

// type identifier
pure_lead_type_identifier[] { ENTRY_DEBUG } :

        // ambigous on template keyword from template specifier and probably class_preamble template
        (options { generateAmbigWarnings = false; } : 
        // specifiers that occur in a type

        { 
#ifdef SRCML_BOOST_MPL_LARGE
            argument_token_set.member(LA(1))
#else
            (argument_token_set_one.member(LA(1)) || argument_token_set_two.member(LA(1)) || argument_token_set_three.member(LA(1)))
#endif
        }?
        specifier | template_specifier | auto_keyword[true] |

        { inLanguage(LANGUAGE_CSHARP) && look_past(COMMA) == RBRACKET }?
        LBRACKET (COMMA)* RBRACKET |

        { inLanguage(LANGUAGE_JAVA) }? annotation |

        { inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |

        { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp |

        pure_lead_type_identifier_no_specifiers
        )
;

// type identifier
pure_lead_type_identifier_no_specifiers[] { ENTRY_DEBUG } :

        // class/struct/union before a name in a type, e.g., class A f();
        (options { generateAmbigWarnings = false; } :

        class_lead_type_identifier | typename_keyword |

        // enum use in a type
        { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
        (ENUM variable_identifier (variable_identifier | multops | tripledotop | INLINE))=> ENUM |

        // entire enum definition
        { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
        enum_definition_complete |

        { LA(1) == DECLTYPE }? type_specifier_call | atomic

        )

;

// more lead type identifier
class_lead_type_identifier[]  { SingleElement element(this); ENTRY_DEBUG } :
        {

            if(inTransparentMode(MODE_TEMPLATE))
                startElement(SNAME);
            else
                startElement(SNOP);

        }
        (CLASS | CXX_CLASS | STRUCT | UNION | ENUM)
;

// type identifier
lead_type_identifier[] { ENTRY_DEBUG } :

//        specifier |

//        (macro_call_paren identifier)=> macro_call |

        // typical type name
        { LA(1) != ASYNC && (inLanguage(LANGUAGE_CXX) || (LA(1) != FINAL && LA(1) != OVERRIDE)) && 
            LA(1) != CRESTRICT && LA(1) != MUTABLE }?
        compound_name |

        pure_lead_type_identifier
;

// type identifier
type_identifier[] { ENTRY_DEBUG } :

        // any identifier that can appear first can appear later
        // true suppresses warning.  antlr forms rules as LA(1) && (true )
        // so this does nothing.
        { true }? lead_type_identifier |

        non_lead_type_identifier
;

// type identifier
non_lead_type_identifier[] { bool iscomplex = false; ENTRY_DEBUG } :

        tripledotop |

        { inLanguage(LANGUAGE_C_FAMILY) }? multops |

        { inLanguage(LANGUAGE_JAVA_FAMILY) && look_past(LBRACKET) == RBRACKET }?
        variable_identifier_array_grammar_sub[iscomplex]
;

type_specifier_call[] { ENTRY_DEBUG } :

    { inputState->guessing }? (decltype_call_full) | decltype_call

;

// C++11 markup decltype 
decltype_call[] { CompleteElement element(this); int save_type_count = getTypeCount(); ENTRY_DEBUG } :
        {

            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the macro call element
            startElement(SDECLTYPE);
         
        }
        DECLTYPE complete_argument_list
        { setTypeCount(save_type_count); }
;

// C++ completely match without markup decltype
decltype_call_full[] { ENTRY_DEBUG } :
        DECLTYPE paren_pair
;

// C11 markup _Atomic 
atomic[] { ENTRY_DEBUG } :

    { next_token() == LPAREN }? ({ inputState->guessing }? atomic_call_full | atomic_call) | atomic_specifier

;

// C11 markup _Atomic as specifier
atomic_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        ATOMIC

;

// C11 markup _Atomic as call
atomic_call[] { CompleteElement element(this);  int save_type_count = getTypeCount(); ENTRY_DEBUG } :
        {

            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the macro call element
            startElement(SATOMIC);
         
        }
        ATOMIC (options { greedy = true; } : complete_argument_list)?
        { setTypeCount(save_type_count); }

;

// C++ completely match without markup _Atomic
atomic_call_full[] { ENTRY_DEBUG } :
        ATOMIC (options { greedy = true; } : paren_pair)?
;

// qmark
qmark_name[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        QMARK
;

qmark_marked[] { bool in_ternary = inTransparentMode(MODE_TERNARY | MODE_CONDITION); LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!(in_ternary && isoption(parser_options, SRCML_OPTION_TERNARY)) && (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR)))
                startElement(SOPERATOR);
        }
        QMARK ({ SkipBufferSize() == 0 }? QMARK)?

;

qmark[] { is_qmark = true; ENTRY_DEBUG } :
        {
            if(inTransparentMode(MODE_TERNARY | MODE_CONDITION))
                endDownToMode(MODE_CONDITION);
        }

        qmark_marked
        {
            if(inTransparentMode(MODE_TERNARY | MODE_CONDITION)) {

                endMode(MODE_CONDITION);
                startNewMode(MODE_THEN | MODE_EXPRESSION | MODE_EXPECT);
                startNoSkipElement(STHEN);

            }
        }
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

// match linq expressions
linq_expression_pure[] { ENTRY_DEBUG } :
        linq_from | linq_where | linq_select | linq_let | linq_group | linq_join | linq_orderby
;

// a linq from
linq_from[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SFROM);
        }
        FROM linq_expression_complete (options { greedy = true; } : linq_in)*
;

// a linq in
linq_in[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SIN);
        }
        IN linq_expression_complete
;

// a linq where
linq_where[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SWHERE);
        }
        WHERE linq_expression_complete
;

// a linq select
linq_select[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SSELECT);
        }
        SELECT linq_expression_complete
;

// a linq let
linq_let[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SLET);
        }
        LET linq_expression_complete
;

// a linq group
linq_group[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SGROUP);
        }
        GROUP linq_expression_complete
        (options { greedy = true; } : linq_by)*
        (options { greedy = true; } : linq_into)*
;

// linq by
linq_by[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SBY);
        }
        BY linq_expression_complete
;

// linq into
linq_into[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SINTO);
        }
        INTO linq_expression_complete
;

// linq join
linq_join[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SJOIN);
        }
        JOIN linq_expression_complete

        (options { greedy = true; } : linq_in | linq_on | linq_equals | linq_into)*
;

// linq on
linq_on[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SON);
        }
        ON linq_expression_complete
;

// linq equals
linq_equals[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SEQUALS);
        }
        EQUALS linq_expression_complete
;

// linq orderby
linq_orderby[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SORDERBY);
        }
        ORDERBY linq_expression_complete

        (options { greedy = true; } : linq_ascending | linq_descending)*

        (options { greedy = true; } : COMMA linq_expression_complete (options { greedy = true; } : linq_ascending | linq_descending)* )*
;

// linq ascending
linq_ascending[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        ASCENDING
;

// linq descending
linq_descending[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        DESCENDING
;

// variables array index
variable_identifier_array_grammar_sub[bool& iscomplex] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            iscomplex = true;

            // start a mode to end at right bracket with expressions inside
            if (inLanguage(LANGUAGE_CSHARP) || (LA(1) == ATLBRACKET && LANGUAGE_OBJECTIVE_C))
                startNewMode(MODE_LOCAL | MODE_TOP | MODE_LIST | MODE_END_AT_COMMA);
            else
                startNewMode(MODE_LOCAL | MODE_TOP | MODE_LIST);

            startElement(SINDEX);
        }
        (LBRACKET | ATLBRACKET)

        variable_identifier_array_grammar_sub_contents

        RBRACKET
;

// contents of array index
variable_identifier_array_grammar_sub_contents{ ENTRY_DEBUG } :
        { !inLanguage(LANGUAGE_CSHARP) && !inLanguage(LANGUAGE_OBJECTIVE_C) }? complete_expression |

        { inLanguage(LANGUAGE_CSHARP) || inLanguage(LANGUAGE_OBJECTIVE_C) }? (options { greedy = true; } : { LA(1) != RBRACKET }?
            ({ /* stop warning */ LA(1) == COMMA }? COMMA | complete_expression)
        )*
;

// handle C# attribute
attribute_csharp[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_END_AT_COMMA);

            startElement(SATTRIBUTE);
        }
        LBRACKET

        // do not warn as identifier list and colon are in complete expression as well, but need special processing here.
        (options { warnWhenFollowAmbig = false; } : { next_token() == COLON }? attribute_csharp_target COLON)*

        attribute_inner_list

        RBRACKET
;

// handle target for C# target
attribute_csharp_target[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(STARGET);
        }
        (RETURN | EVENT | identifier_list)
;

// inner attribute list handling
attribute_inner_list[] { ENTRY_DEBUG } :

    complete_expression (COMMA complete_expression)*

;

// C++11 attributes
attribute_cpp[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_END_AT_COMMA);

            startElement(SATTRIBUTE);
        }
        LBRACKET LBRACKET

        attribute_inner_list

        RBRACKET RBRACKET
;

// Do a complete argument list
complete_argument_list[] { ENTRY_DEBUG } :
        call_argument_list (options { greedy = true; } : { LA(1) != RPAREN && LA(1) != RCURLY }? complete_arguments)* rparen[false]
;

// Full, complete expression matched all at once (no stream).
complete_arguments[] { CompleteElement element(this); int count_paren = 1; CALL_TYPE type = NOCALL; 
    bool isempty = false; int call_count = 0; ENTRY_DEBUG } :
        { getParen() == 0 }? rparen[false] |
        { getCurly() == 0 }? rcurly_argument |
        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        }
        (options { warnWhenFollowAmbig = false; } : { count_paren > 0 && (count_paren != 1 || LA(1) != RPAREN) }?

            (options { generateAmbigWarnings = false; } :
                { LA(1) == LPAREN }? expression { ++count_paren; } |

                { LA(1) == RPAREN }? expression { --count_paren; } |

                { perform_call_check(type, isempty, call_count, -1) && type == CALL }? { if(!isempty) ++count_paren; }
                    expression_process (call[call_count] | keyword_calls) complete_arguments |

                expression | (type_identifier) => expression_process type_identifier |

                comma
                {
                    // argument with nested expression
                    startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

                    // start the argument
                    startElement(SARGUMENT);
                }
            )
        )*

;

// Full, complete expression matched all at once (no stream).
// May be better version of complete_expression
complete_default_parameter[] { CompleteElement element(this); int count_paren = 0; CALL_TYPE type = NOCALL; 
    bool isempty = false; int call_count = 0; ENTRY_DEBUG } : 
       { getParen() == 0 }? rparen[false] |
        { getCurly() == 0 }? rcurly_argument |
        {
            // argument with nested expression
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }
        (options {warnWhenFollowAmbig = false; } : { (LA(1) != RPAREN && LA(1) != COMMA) || count_paren > 0 }?

        ({ LA(1) == LPAREN }? expression set_int[count_paren, count_paren + 1] |

        { LA(1) == RPAREN && inputState->guessing }? rparen set_int[count_paren, count_paren - 1] |

        { LA(1) == RPAREN && !inputState->guessing}? expression set_int[count_paren, count_paren - 1] |

        { perform_call_check(type, isempty, call_count, -1) && type == CALL }? 
        set_int[count_paren, isempty ? count_paren : count_paren + 1] expression |

        expression | comma

        ))*

;


// match a complete objective_c_call no stream
complete_objective_c_call[] { CompleteElement element(this); int bracket_count = 0; ENTRY_DEBUG} :

    { inputState->guessing }? bracket_pair |

    {
        // start a mode to end at right bracket with expressions inside
        if(!inMode(MODE_EXPRESSION) || inMode(MODE_EXPRESSION | MODE_EXPECT))
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        else 
            startNewMode(MODE_TOP);

    }

    (options { greedy = true; } :

            // end of objective c call
            { inTransparentMode(MODE_OBJECTIVE_C_CALL) && bracket_count }? rbracket set_int[bracket_count, bracket_count - 1] |

            // objective c argument list
            { LA(1) == LBRACKET }? expression set_int[bracket_count, bracket_count + 1] |

            // objective c argument list
            { inTransparentMode(MODE_OBJECTIVE_C_CALL | MODE_ARGUMENT_LIST) }?
            (function_identifier (COLON | RBRACKET)) => objective_c_call_message |

            // objective c argument
            { inTransparentMode(MODE_OBJECTIVE_C_CALL) }?
            (function_identifier (COLON | RBRACKET) | COLON) => objective_c_call_argument |

            // commas as in a list
            { inTransparentMode(MODE_END_ONLY_AT_RPAREN) || !inTransparentMode(MODE_END_AT_COMMA)}?
            comma |

            // right parentheses, unless we are in a pair of parentheses in an expression
            { !inTransparentMode(MODE_INTERNAL_END_PAREN) }? rparen[false] |

            // argument mode (as part of call)
            { inMode(MODE_ARGUMENT) }? argument |

            // expression with right parentheses if a previous match is in one
            { LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN) }? expression |

            colon_marked

    )*

;

// match a complete expression no stream
complete_expression[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);

        }
        (options { greedy = true; } :

            // commas as in a list
            { (inTransparentMode(MODE_END_ONLY_AT_RPAREN) && (getFirstMode(MODE_END_ONLY_AT_RPAREN | MODE_END_AT_COMMA) & MODE_END_AT_COMMA) == 0) || !inTransparentMode(MODE_END_AT_COMMA) }?
            comma |

            // right parentheses, unless we are in a pair of parentheses in an expression
            { !inTransparentMode(MODE_INTERNAL_END_PAREN) }? rparen[false] |

            { inLanguage(LANGUAGE_OBJECTIVE_C) && LA(1) == LBRACKET }? complete_objective_c_call |

            // argument mode (as part of call)
            { inMode(MODE_ARGUMENT) }? argument |

            // expression with right parentheses if a previous match is in one
            { LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN) }? expression |

            colon_marked

        )*
;


// match a linq_expression completely
linq_expression_complete[] { CompleteElement element(this); int count_paren = 0; ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }
        (options {warnWhenFollowAmbig = false; } : { LA(1) != RPAREN || count_paren > 0 }?


            { (LA(1) != RPAREN || count_paren > 0) && try_linq_expression_complete_inner(count_paren) }? linq_expression_complete_inner[count_paren, true]
            
        )*
;

try_linq_expression_complete_inner[int & count_paren] returns[bool success = false] {

    int start = mark();
    ++inputState->guessing;

    try {

        linq_expression_complete_inner(count_paren);
        success = true;

    } catch(antlr::RecognitionException & e) {

        success = false;

    }

    rewind(start);
    --inputState->guessing;


} :;

linq_expression_complete_inner[int & count_paren, bool update = false] { CALL_TYPE type = NOCALL; bool isempty = false; int call_count = 0; ENTRY_DEBUG } :

    // commas as in a list
    comma |

    // right parentheses, unless we are in a pair of parentheses in an expression
    { LA(1) == LPAREN }? expression_setup_linq ({ update }? set_int[count_paren, count_paren + 1])? |

    { LA(1) == RPAREN && inputState->guessing }? rparen ({ update }? set_int[count_paren, count_paren - 1])? |

    { LA(1) == RPAREN && !inputState->guessing}? expression_setup_linq ({ update }? set_int[count_paren, count_paren - 1])? |

    { perform_call_check(type, isempty, call_count, -1) && type == CALL }? 
    ({ update }? set_int[count_paren, isempty ? count_paren : count_paren + 1])? expression_setup_linq |

    // argument mode (as part of call)
    { inMode(MODE_ARGUMENT) }? argument |

    // expression with right parentheses if a previous match is in one
    { LA(1) != ASCENDING && LA(1) != DESCENDING && LA(1) != ON && LA(1) != BY && LA(1) != FROM && LA(1) != SELECT 
        && LA(1) != LET && LA(1) != WHERE && LA(1) != ORDERBY && LA(1) != GROUP && LA(1) != JOIN && LA(1) != IN 
        && LA(1) != EQUALS && LA(1) != INTO && (LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN)) }? expression_setup_linq |

    COLON

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
            { inLanguage(LANGUAGE_CXX_FAMILY) || inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_OBJECTIVE_C) }?
            (generic_argument_list)=>
                generic_argument_list (options { greedy = true; } : generic_type_constraint)* |

            (cuda_argument_list) => cuda_argument_list |

            {
               // set the token to NOP since we did not find a template argument list
               tp.setType(SNOP);
            }
       )
;

// name including template argument list
simple_name_optional_template_optional_specifier[] { CompleteElement element(this); TokenPosition tp; bool is_nop = true; ENTRY_DEBUG } :
        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            // start outer name
            startElement(SCNAME);

            // record the name token so we can replace it if necessary
            setTokenPosition(tp);
        }
        push_namestack (template_specifier { is_nop = false; })* identifier
    (
        (generic_argument_list)=> generic_argument_list (options { greedy = true; } : generic_type_constraint)*  |

        (cuda_argument_list) => cuda_argument_list |

        {
            // set the token to NOP since we did not find a template argument list
            if(is_nop)
                tp.setType(SNOP);
        }
    )

;

// an identifier
identifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
                startElement(SNAME);
        }
        identifier_list
;

// the list of identifiers that are also marked up as tokens for other things.
identifier_list[] { ENTRY_DEBUG } :
            NAME | INCLUDE | DEFINE | ELIF | ENDIF | ERRORPREC | IFDEF | IFNDEF | LINE | PRAGMA | UNDEF |
            WARNING | SUPER | REGION | ENDREGION | GET | SET | ADD | REMOVE | ASYNC | YIELD |
            SIGNAL | FINAL | OVERRIDE | VOID | ASM |

            // C# linq
            FROM | WHERE | SELECT | LET | ORDERBY | ASCENDING | DESCENDING | GROUP | BY | JOIN | ON | EQUALS |
            INTO | THIS | ALIAS |

            // Objective-C
            IMPORT | ATPROTOCOL |

            // C
            CRESTRICT | MUTABLE | CXX_TRY | CXX_CATCH/*| CXX_CLASS| THROW | CLASS | PUBLIC | PRIVATE | PROTECTED | NEW |
            SIGNAL | FOREACH | FOREVER | VIRTUAL | FRIEND | OPERATOR | EXPLICIT | NAMESPACE | USING |
            DELETE | FALSE | TRUE | FINAL | OVERRIDE | CONSTEXPR | NOEXCEPT | THREADLOCAL | NULLPTR |
            DECLTYPE | ALIGNAS | TYPENAME | ALIGNOF*/

;

// most basic name
simple_identifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        (
        NAME | VOID
        )
;

typename_keyword[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            if(!inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST))
                startElement(STYPENAME);
            else
                startElement(SNAME);
        }
        TYPENAME
;

function_pointer_name_check[] returns[bool is_fp_name = false] {

    if(LA(1) == LPAREN && (inLanguage(LANGUAGE_C) || inLanguage(LANGUAGE_CXX))) {

        ++inputState->guessing;
        int start = mark();

        try {

            function_pointer_name_grammar();
            is_fp_name = LA(1) == PERIOD || LA(1) == TRETURN
                || (inLanguage(LANGUAGE_CXX) && (LA(1) == MPDEREF || LA(1) == DOTDEREF));

        } catch(...) {}
       

        rewind(start);
        --inputState->guessing;

    }

ENTRY_DEBUG } :;

function_pointer_name[] { CompleteElement element(this); ENTRY_DEBUG }:

        {

            startNewMode(MODE_LOCAL);

            startElement(SNAME);

        }

        pointer_dereference (period | member_pointer | member_pointer_dereference | dot_dereference)

        ({ function_pointer_name_check() }? pointer_dereference (period | member_pointer | member_pointer_dereference | dot_dereference))*

        compound_name_inner[false]
        
    ;

pointer_dereference[] { ENTRY_DEBUG bool flag = false; } :

    lparen_marked

    // special case for function pointer names that don't have '*'
    (
        { macro_call_token_set.member(LA(1)) }?
        (compound_name_inner[false])* |

        // special name prefix of namespace or class
        identifier (generic_argument_list (generic_type_constraint)*)* DCOLON pointer_dereference |

        // typical function pointer name
        // need greedy for general operators and possibly end
        general_operators (options { greedy = true; } : general_operators)* (options { greedy = true; } : compound_name_inner[false])*

        // optional array declaration
        (variable_identifier_array_grammar_sub[flag])*

    )

    rparen[true]

;

// Markup names
compound_name[] { CompleteElement element(this); bool iscompound = false; ENTRY_DEBUG } :
        compound_name_inner[true]
        (options { greedy = true; } : {(!inLanguage(LANGUAGE_CXX) || next_token() != LBRACKET)}? variable_identifier_array_grammar_sub[iscompound] |
        { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp)*

;

// name markup internals
compound_name_inner[bool index] { CompleteElement element(this); TokenPosition tp; bool iscompound = false; ENTRY_DEBUG 
} :
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

        { inLanguage(LANGUAGE_OBJECTIVE_C) }?
        compound_name_objective_c[iscompound] |

        { inLanguage(LANGUAGE_C) }?
        compound_name_c[iscompound] |

        { !inLanguage(LANGUAGE_JAVA_FAMILY) && !inLanguage(LANGUAGE_C) && !inLanguage(LANGUAGE_CSHARP) && !inLanguage(LANGUAGE_OBJECTIVE_C) }?
        compound_name_cpp[iscompound] |

        macro_type_name_call 
        )

        (options { greedy = true; } : { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp)*

        (options { greedy = true; } : { index && /*!inTransparentMode(MODE_EAT_TYPE) &&*/ (!inLanguage(LANGUAGE_CXX) || next_token() != LBRACKET)}?
            variable_identifier_array_grammar_sub[iscompound]
        )*

        {
            // if it isn't a compound name, nop the element
            if (!iscompound)
                // set the token to NOP
                tp.setType(SNOP);
        }
;

multops_star[] { ENTRY_DEBUG } :

    (options { greedy = true; } : multops)*

;

// C++ compound name handling
compound_name_cpp[bool& iscompound] { namestack[0] = namestack[1] = ""; ENTRY_DEBUG } :

        (options { greedy = true; } : { !in_template_param }? typename_keyword { iscompound = true; })*
        (dcolon { iscompound = true; })*
        (DESTOP set_bool[isdestructor] { iscompound = true; })*
        (typename_keyword | simple_name_optional_template | push_namestack overloaded_operator)
        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multops)*

        // "a::" causes an exception to be thrown
        ( options { greedy = true; } :
            ({ !modifier_tokens_set.member(last_consumed) }? dcolon { iscompound = true; } | (period | member_pointer | member_pointer_dereference | dot_dereference) { iscompound = true; })
            (options { greedy = true; } : dcolon)*
            (DESTOP set_bool[isdestructor])*
            (multops)*
            (simple_name_optional_template_optional_specifier | push_namestack overloaded_operator | function_identifier_main | keyword_identifier)
            //(options { greedy = true; } : { look_past_rule(&srcMLParser::multops_star) == DCOLON }? multops)*
        )*

        { notdestructor = LA(1) == DESTOP; }

;
exception
catch[antlr::RecognitionException] {
}

// compound name for C#
compound_name_csharp[bool& iscompound] { namestack[0] = namestack[1] = ""; ENTRY_DEBUG } :

        (modifiers_csharp)*
        (dcolon { iscompound = true; })*
        (DESTOP set_bool[isdestructor] { iscompound = true; })*
        (simple_name_optional_template | push_namestack overloaded_operator)
        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multops)*

        // "a::" causes an exception to be thrown
        ( options { greedy = true; } :
            ({ !modifier_tokens_set.member(last_consumed) }? dcolon { iscompound = true; } | (period | member_pointer) { iscompound = true; })
            ( options { greedy = true; } : dcolon)*
            (multops)*
            (DESTOP set_bool[isdestructor])*
            (simple_name_optional_template | push_namestack overloaded_operator | function_identifier_main)
            //(options { greedy = true; } : { look_past_rule(&srcMLParser::multops_star) == DCOLON }? multops)*
        )*

;
exception
catch[antlr::RecognitionException] {
}

// compound name for C
compound_name_c[bool& iscompound] { ENTRY_DEBUG } :

        (simple_name_optional_template | generic_selection) (options { greedy = true; }: { !inTransparentMode(MODE_EXPRESSION) && (LA(1) == MULTOPS || LA(1) == BLOCKOP) }? multops)*

        ( options { greedy = true; } :
            (period | member_pointer) { iscompound = true; }
            ({ LA(1) == MULTOPS || LA(1) == BLOCKOP }? multops)*
            identifier
        )*

;

// compound name for C
compound_name_objective_c[bool& iscompound] { ENTRY_DEBUG } :

        (simple_name_optional_template | generic_selection) (options { greedy = true; }: { !inTransparentMode(MODE_EXPRESSION) && (LA(1) == MULTOPS || LA(1) == BLOCKOP) }? multops)*

        ( options { greedy = true; } :
            (period | member_pointer) { iscompound = true; }
            ({ LA(1) == MULTOPS || LA(1) == BLOCKOP }? multops)*
            simple_name_optional_template
        )*

;

// compound name for Java
compound_name_java[bool& iscompound] { ENTRY_DEBUG } :

        generic_argument_list |
        simple_name_optional_template
        (options { greedy = true; } : (period { iscompound = true; } (keyword_name | simple_name_optional_template | { next_token() == TERMINATE }? multop_name)))*

;


keyword_name[] { CompleteElement element(this); TokenPosition tp; bool iscompound = false; ENTRY_DEBUG } :
        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            // start outer name
            startElement(SONAME);

            // start inner name
            startElement(SCNAME);

            // record the name token so we can replace it if necessary
            setTokenPosition(tp);
        }

        keyword_name_inner[iscompound]

        (options { greedy = true; } : { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp)*

        (options { greedy = true; } : { /*!inTransparentMode(MODE_EAT_TYPE) &&*/ (!inLanguage(LANGUAGE_CXX) || next_token() != LBRACKET)}?
            variable_identifier_array_grammar_sub[iscompound]
        )*

        {
            // if it isn't a compound name, nop the element
            if (!iscompound)
                // set the token to NOP
                tp.setType(SNOP);
        }

;

// C++ compound name handling
keyword_name_inner[bool& iscompound] { namestack[0] = namestack[1] = ""; ENTRY_DEBUG } :

        //( options { greedy = true; } : dcolon)*
        //(DESTOP set_bool[isdestructor] { iscompound = true; })*
        keyword_identifier
        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multops)*

        // "a::" causes an exception to be thrown
        ( options { greedy = true; } :
            (dcolon { iscompound = true; } | (period | member_pointer | member_pointer_dereference | dot_dereference) { iscompound = true; })
            ( options { greedy = true; } : dcolon)*
            (DESTOP set_bool[isdestructor])*
            (multops)*
            (simple_name_optional_template_optional_specifier | push_namestack overloaded_operator | function_identifier_main | keyword_identifier)
            (options { greedy = true; } : { look_past_rule(&srcMLParser::multops_star) == DCOLON }? multops)*
        )*

        { notdestructor = LA(1) == DESTOP; }

;
exception
catch[antlr::RecognitionException] {
}

// an identifier
keyword_identifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
                startElement(SNAME);
        }
        (CLASS | CXX_CLASS)
        
;

// Specifier for a function
function_specifier[] { CompleteElement element(this); ENTRY_DEBUG } :
        { LA(1) == WHERE }? generic_type_constraint |

        ({ LA(1) != ASYNC }? specifier |

        // pure virtual specifier, = default, and = delete
        EQUAL (literal | function_equal_specifier) |

        simple_name_optional_template)
;

// function declaration can be set to delete or default.
function_equal_specifier[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup strings in literal option
            startElement(SFUNCTION_SPECIFIER);
        }
        (

            DEFAULT | DELETE

        )

;

// mark specifiers
specifier[] { ENTRY_DEBUG } :
        single_keyword_specifier | alignas_specifier | macro_specifier_call | atomic

;

// match a single word specifier
single_keyword_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        (
            // access
            PUBLIC | PRIVATE | PROTECTED |

            // C++
            FINAL | STATIC | ABSTRACT | { inLanguage(LANGUAGE_CSHARP) }? NEW | MUTABLE |
            CONSTEXPR | THREAD_LOCAL |

            // C
            REGISTER | RESTRICT | NORETURN | COMPLEX | IMAGINARY |

            // C/C++ mode
            CRESTRICT | 

            // C# & Java
            INTERNAL | SEALED | OVERRIDE | IMPLICIT | EXPLICIT | UNSAFE | READONLY | VOLATILE |
            DELEGATE | PARTIAL | ASYNC | VIRTUAL | EXTERN | INLINE | IN | PARAMS |
            { inLanguage(LANGUAGE_JAVA) }? (SYNCHRONIZED | NATIVE | STRICTFP | TRANSIENT) |

            CONST |

            // Apple
            BLOCK | WEAK | STRONG

        )
;

// C/C++ auto specifier (as either type name or specifier)
auto_keyword[bool is_specifier] { SingleElement element(this); ENTRY_DEBUG } :
        {
            if(is_specifier)
                startElement(SFUNCTION_SPECIFIER);
            else
                startElement(SNAME);
        }
        AUTO
;

// match a single word specifier
template_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        TEMPLATE

;

// C++11 specifier
alignas_specifier[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL | MODE_ARGUMENT);

            startElement(SALIGNAS);
        }
        ALIGNAS

        ({ inputState->guessing }? paren_pair | 

        complete_argument_list)

;

// default specifier (Java Methods)
default_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        DEFAULT

;

this_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }
        THIS

;

// A constructor declaration
constructor_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the constructor declaration
            startElement(SCONSTRUCTOR_DECLARATION);
        }
        constructor_header
;

// A constructor definition
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

// header portion of constructor
constructor_header[] { ENTRY_DEBUG } :

        (options { greedy = true; } :

            { inLanguage(LANGUAGE_JAVA) }? annotation |

            { inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp |

            specifier | { next_token() != TEMPOPS }? template_specifier | template_declaration_full |

            { inLanguage(LANGUAGE_JAVA_FAMILY) }? generic_parameter_list
        )*
        compound_name_inner[false]
        parameter_list
        {
            setMode(MODE_FUNCTION_TAIL);
        }
;

// member initialization list markup
member_initialization_list[] { ENTRY_DEBUG } :
        {
            // handle member initialization list as a list of calls
            startNewMode(MODE_LIST | MODE_CALL | MODE_INITIALIZATION_LIST);

            startElement(SMEMBER_INITIALIZATION_LIST);
        }
        COLON
;

// call  function call, macro, etc.
member_init[] { ENTRY_DEBUG } :
        {

            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_ARGUMENT_LIST);

            // start the function call element
            startElement(SMEMBER_INITIALIZATION);

        }
        function_identifier call_argument_list
        
;

// push name onto namestack
push_namestack[] { namestack[1].swap(namestack[0]); namestack[0] = LT(1)->getText(); } :;

// identifier stack
identifier_stack[std::string s[]] { s[1].swap(s[0]); s[0] = LT(1)->getText(); ENTRY_DEBUG } :
        identifier
;

// destructor definition
destructor_definition[] { ENTRY_DEBUG } :
        {
            // statement with nested block
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the destructor definition
            startElement(SDESTRUCTOR_DEFINITION);
        }
        destructor_header

        ({ inLanguage(LANGUAGE_CXX_FAMILY) }? try_statement)*

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

// a destructor header
destructor_header[] { ENTRY_DEBUG } :

        (options { greedy = true; } :

            { inLanguage(LANGUAGE_JAVA) }? annotation |

            { inLanguage(LANGUAGE_CSHARP) }? attribute_csharp |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp |

             specifier | { next_token() != TEMPOPS }? template_specifier | template_declaration_full | 

            { LA(1) == VOID }? simple_identifier
        )*
        compound_name_inner[false]
        parameter_list
        {
            setMode(MODE_FUNCTION_TAIL);
        }
;

// Java annotation
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
call[int call_count = 1] { ENTRY_DEBUG } :
        {

            do {

                // start a new mode that will end after the argument list
                startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_ARGUMENT_LIST);

                // start the function call element
                startElement(SFUNCTION_CALL);

            } while(--call_count > 0);

        }
        ({inLanguage(LANGUAGE_OBJECTIVE_C) }? objective_c_call | function_identifier call_argument_list)
        
;

// argument list to a call
call_argument_list[] { ENTRY_DEBUG } :
        {
            // list of parameters
            replaceMode(MODE_ARGUMENT_LIST, MODE_EXPECT | MODE_LIST | MODE_INTERNAL_END_PAREN | MODE_END_ONLY_AT_RPAREN);

            // start the argument list
            startElement(SARGUMENT_LIST);
        }
        (LPAREN | { setMode(MODE_INTERNAL_END_CURLY); } LCURLY)
;

// function call for Objective_C
objective_c_call[] { ENTRY_DEBUG } :
    {

        // start a new mode that will end after the argument list
        startNewMode(MODE_OBJECTIVE_C_CALL);

        // start the function call element
        startElement(SFUNCTION_CALL);

        startNewMode(MODE_ARGUMENT_LIST | MODE_LIST);

    }

    LBRACKET
    objective_c_call_receiver

;

// function call object for Objective_C
objective_c_call_receiver[] { ENTRY_DEBUG } :
    {

        startNewMode(MODE_EXPRESSION | MODE_EXPECT);

        // start the function call element
        startElement(SRECEIVER);
    
    }
    expression

;

// function call message for Objective_C
objective_c_call_message[] { ENTRY_DEBUG } :
    {

        endDownToMode(MODE_ARGUMENT_LIST);
        clearMode(MODE_ARGUMENT_LIST);

        // start the function call element
        startElement(SMESSAGE);

    }
    objective_c_call_argument

;

// function call argument name:value pair for Objective_C
objective_c_call_argument[] { bool first = true; ENTRY_DEBUG } :
    {

        if(inTransparentMode(MODE_LIST))
            endDownToMode(MODE_LIST);

        startNewMode(MODE_ARGUMENT);

    }
    objective_c_selector (options { greedy = true; } : { first && LA(1) != RBRACKET }? argument set_bool[first, false])*
;

// function call message for Objective_C
objective_c_selector[] { CompleteElement element(this); ENTRY_DEBUG } :
    {
        startNewMode(MODE_LOCAL);

        startElement(SSELECTOR);
    }
    (function_identifier (options { greedy = true; } : COLON)* | COLON)

;

ternary_expression[] { ENTRY_DEBUG } :
    {
        startNewMode(MODE_TERNARY);
        startElement(STERNARY);
 
        startNewMode(MODE_CONDITION);
        startElement(SCONDITION);
        startNewMode(MODE_TERNARY_CONDITION | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT);
    }
    (
        { LA(1) == LPAREN }?
        expression_process lparen_marked
            {
                startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);
            } 
        ({ perform_ternary_check() }? ternary_expression)? |
        expression_no_ternary
)
;

// an expression
expression_no_ternary[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :

        expression_process

        expression_part_plus_linq_no_ternary[type, call_count]
;

// expression with linq
expression_part_plus_linq_no_ternary[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :

        { inLanguage(LANGUAGE_CSHARP) && next_token() != RPAREN && next_token() != ASSIGNMENT && next_token() != EQUAL }?
        (linq_expression_pure)=> linq_expression |

        expression_part_no_ternary[type, call_count]
;
expression_part_no_ternary[CALL_TYPE type = NOCALL, int call_count = 1] { bool flag; bool isempty = false; bool end_for_incr = false; ENTRY_DEBUG } :

        // cast
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        UNION |

        // cast
        { inTransparentMode(MODE_INTERNAL_END_PAREN) && (LA(1) != CXX_CLASS || !keyword_name_token_set.member(next_token())) }?
        (CLASS | CXX_CLASS) |

        { next_token() == LPAREN || next_token() == LCURLY }?
        delegate_anonymous |

        { next_token() == LCURLY }?
        lambda_anonymous |

        { inLanguage(LANGUAGE_CSHARP) }?
        (lambda_expression_full_csharp) => lambda_expression_csharp |

        { inLanguage(LANGUAGE_CXX) }?
        (bracket_pair (options { warnWhenFollowAmbig = false; } : paren_pair)* function_tail LCURLY) => lambda_expression_cpp |

        { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
        (block_lambda_expression_full) => block_lambda_expression |

        { inLanguage(LANGUAGE_JAVA) }?
        ((paren_pair | variable_identifier) TRETURN) => lambda_expression_java |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW generic_argument_list)=> sole_new generic_argument_list |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW function_identifier paren_pair LCURLY)=> sole_new anonymous_class_definition |

        { notdestructor }? sole_destop { notdestructor = false; } |

        { next_token() != LPAREN && next_token() != DOTDOTDOT }? sizeof_unary_expression |

        // call
        // distinguish between a call and a macro
        { type == CALL || (perform_call_check(type, isempty, call_count, -1) && type == CALL) }?

            // Added argument to correct markup of default parameters using a call.
            // normally call claims left paren and start calls argument.
            // however I believe parameter_list matches a right paren of the call.
           (call[call_count] | keyword_calls) argument |

        // macro call
        { type == MACRO }? macro_call |

        // general math operators
        // looks like general operators and variable identifier can match same thing
        (options { generateAmbigWarnings = false; } : general_operators
        {
            if (inLanguage(LANGUAGE_CXX_FAMILY) && LA(1) == DESTOP)
                general_operators();
        }
        | qmark | /* newop | */ period | member_pointer | member_pointer_dereference | dot_dereference |

        // left parentheses
        { function_pointer_name_check() }?
        function_pointer_name |
        lparen_marked
        {
            startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);
        } |

        // right parentheses that only matches a left parentheses of an expression
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        {

            end_for_incr = inTransparentMode(MODE_FOR_INCREMENT);

            // stop at this matching paren, or a preprocessor statement
            endDownToModeSet(MODE_INTERNAL_END_PAREN | MODE_PREPROC);
            
            if (inMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN))
                endMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);

            end_for_incr = end_for_incr && !inTransparentMode(MODE_FOR_INCREMENT);

        }

        // treat as operator for operator markup
        rparen[!end_for_incr, end_for_incr] |

        // left curly brace
        {
            startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_TOP);

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
        variable_identifier | keyword_name | auto_keyword[false] | single_keyword_specifier) | literals | noexcept_list | 

        variable_identifier_array_grammar_sub[flag]
;

// Keyword based calls with special markup
keyword_calls[] { ENTRY_DEBUG } :

    // C++
    sizeof_call | alignof_call | typeid_call | const_cast_call | dynamic_cast_call | reinterpret_cast_call | static_cast_call | decltype_call |


    // Objective-C
    encode_call | selector_call |

    // C#
    typeof_call | { inLanguage(LANGUAGE_CSHARP) }? default_call | checked_call | unchecked_call

;


keyword_call_tokens[] { ENTRY_DEBUG } :

    // C++
    SIZEOF | ALIGNOF | TYPEID | CONST_CAST | DYNAMIC_CAST | REINTERPRET_CAST | STATIC_CAST | DECLTYPE |

    // Objective-C
    ENCODE | SELECTOR |

    // C#
    TYPEOF | DEFAULT | CHECKED | UNCHECKED

;

// sizeof unary_expression
sizeof_unary_expression[] { CompleteElement element(this); ENTRY_DEBUG } :
    {
        startNewMode(MODE_LOCAL);

        startElement(SSIZEOF_CALL);

    }
    SIZEOF
    variable_identifier
;

// sizeof(...)
sizeof_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            if(next_token() != DOTDOTDOT)
                startElement(SSIZEOF_CALL);
            else
                startElement(SSIZEOF_PACK);

        }
        SIZEOF
        (DOTDOTDOT)*
        call_argument_list
;

// alignof
alignof_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SALIGNOF);
        }
        ALIGNOF
        call_argument_list
;

// typeid
typeid_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(STYPEID);
        }
        TYPEID
        call_argument_list
;

// const_cast
const_cast_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SCONST_CAST);
        }
        CONST_CAST (generic_argument_list)*
        call_argument_list
;

// dynamic_cast
dynamic_cast_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SDYNAMIC_CAST);
        }
        DYNAMIC_CAST (generic_argument_list)*
        call_argument_list
;

// reinterpret_cast
reinterpret_cast_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SREINTERPRET_CAST);
        }
        REINTERPRET_CAST (generic_argument_list)*
        call_argument_list
;

// static_cast
static_cast_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SSTATIC_CAST);
        }
        STATIC_CAST (generic_argument_list)*
        call_argument_list
;

// @encode(...)
encode_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SENCODE);
        }
        ENCODE
        call_argument_list
;

// @selector(...)
selector_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SSELECTOR);
        }
        SELECTOR
        call_argument_list
;

// typeof
typeof_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(STYPEOF);
        }
        TYPEOF
        call_argument_list
;

// default
default_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SDEFAULT);
        }
        DEFAULT
        call_argument_list
;

// checked
checked_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SCHECKED_STATEMENT);
        }
        CHECKED
        call_argument_list
;


// unchecked
unchecked_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SUNCHECKED_STATEMENT);
        }
        UNCHECKED
        call_argument_list
;
// check if macro call
macro_call_check[] { ENTRY_DEBUG } :
        simple_identifier (options { greedy = true; } : paren_pair)*
;

// eat an optional macro call
eat_optional_macro_call[] {

    bool success = false;

    // find out if we have a macro call
    int start = mark();
    inputState->guessing++;

    try {
        // check for the name
        if(LA(1) == NAME)
            match(NAME);
        else 
            match(VOID);

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

// markup macro call
macro_call[] { ENTRY_DEBUG } :
        macro_call_inner
        {
            if (inMode(MODE_THEN) && LA(1) == ELSE)
                endMode(MODE_THEN);
        }
;

// handle the actual macro call
macro_call_inner[] { CompleteElement element(this); bool first = true; ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);
        }
        (identifier | CXX_CLASS)
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
        if (isoption(parser_options, SRCML_OPTION_DEBUG))
            emptyElement(SERROR_PARSE);
}

// do a macro call.
macro_pattern_call[] { CompleteElement element(this) ;ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);

            startNewMode(MODE_LOCAL);
            startElement(SNAME);

        }

        MACRO_NAME
        { endMode(); }
        macro_call_argument_list

;

// do a macro call.
macro_type_name_call[] { CompleteElement element(this) ;ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);

            startNewMode(MODE_LOCAL);
            startElement(SNAME);

        }

        MACRO_TYPE_NAME
        { endMode(); }
        macro_call_argument_list

;


// do a macro call.
macro_case_call[] { CompleteElement element(this) ;ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);

            startNewMode(MODE_LOCAL);
            startElement(SNAME);

        }

        MACRO_CASE
        { endMode(); }
        macro_call_argument_list

;

// do a macro call.
macro_label_call[] { CompleteElement element(this) ;ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);

            startNewMode(MODE_LOCAL);
            startElement(SNAME);

        }

        MACRO_LABEL
        { endMode(); }
        macro_call_argument_list

;

// do a macro call.
macro_specifier_call[] { CompleteElement element(this) ;ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SFUNCTION_SPECIFIER);
            startElement(SMACRO_CALL);

            startNewMode(MODE_LOCAL);
            startElement(SNAME);

        }

        MACRO_SPECIFIER
        { endMode(); }
        macro_call_argument_list

;

// handle the actual macro call
macro_call_argument_list[] { bool first = true; ENTRY_DEBUG } :
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
        if (isoption(parser_options, SRCML_OPTION_DEBUG))
            emptyElement(SERROR_PARSE);
}

// handle macro list/pattern name by itself
macro_type_name[]  { SingleElement element(this); ENTRY_DEBUG } :
        {

            startElement(SNAME);

        }
        MACRO_TYPE_NAME
;

// inner part of call
macro_type_name_call_inner[] { CompleteElement element(this); bool first = true; ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);
        }
        macro_type_name
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
        if (isoption(parser_options, SRCML_OPTION_DEBUG))
            emptyElement(SERROR_PARSE);
}

// contents of macro call
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

        if(literal_tokens_set.member(LA(1)))
            literals();
        else
            consume();

    }

} :;

// try statement
try_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TRY);

            // start of the try statement
            startElement(STRY_BLOCK);
        }
        (TRY | CXX_TRY)
;

// try statement with resources
try_statement_with_resource[] { ENTRY_DEBUG } :
        for_like_statement_pre[STRY_BLOCK]

        TRY

        for_like_statement_post 

;

// a checked statement
checked_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SCHECKED_STATEMENT);
        }
        CHECKED
;

// unsafe statement
unsafe_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SUNSAFE_STATEMENT);
        }
        UNSAFE
;

// using namespace 
using_namespace_statement[] { ENTRY_DEBUG } :

        { inLanguage(LANGUAGE_CSHARP) && next_token() == LPAREN }?
        using_statement |

        namespace_directive
;

// using statement
using_statement[] { ENTRY_DEBUG } :

        // sometimes doing something like this does not work in antlr because it looks for something like EOF instead of nothing.
        // However, this seems to work in this case possibly, becaused it is used with tokens required afterwards.
        for_like_statement_pre[SUSING_STATEMENT]

        USING 

        for_like_statement_post

;

for_like_statement_pre[int tag] { ENTRY_DEBUG } :
        {
            
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(tag);

        }

;

for_like_statement_post[] { int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :

    {

        // expect a condition to follow the keyword
        startNewMode(MODE_TOP | MODE_LIST | MODE_EXPECT | MODE_INTERNAL_END_PAREN);

        startElement(SFOR_LIKE_CONTROL);

    }
    LPAREN
    {

        startNewMode(MODE_EXPRESSION | MODE_EXPECT | MODE_STATEMENT | MODE_INTERNAL_END_PAREN | MODE_LIST);

        //startElement(SFOR_INITIALIZATION);
    }

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

// lock statement
lock_statement[] { ENTRY_DEBUG } :

        for_like_statement_pre[SLOCK_STATEMENT]

        LOCK

        for_like_statement_post

;

// fix the statement
fixed_statement[] { ENTRY_DEBUG } :

        for_like_statement_pre[SFIXED_STATEMENT]

        FIXED

        for_like_statement_post
;

// a synchonized statement
synchronized_statement[] { ENTRY_DEBUG } :

        for_like_statement_pre[SSYNCHRONIZED_STATEMENT]

        SYNCHRONIZED

        for_like_statement_post
;

// unchecked statement
unchecked_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SUNCHECKED_STATEMENT);
        }

        UNCHECKED

;

// a synchonized statement
autoreleasepool_block[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(SAUTORELEASEPOOL);

        }
        AUTORELEASEPOOL lcurly

;

// the catch statement
catch_statement[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the catch statement
            startElement(SCATCH_BLOCK);
        }
        (CATCH | CXX_CATCH) (parameter_list)*
;

// finally statement
finally_statement[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the catch statement
            startElement(SFINALLY_BLOCK);
        }
        FINALLY
;

// lambda function
lambda_anonymous[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_ANONYMOUS);
        }
        lambda_csharp

        /* completely parse a function until it is done */
        (options { greedy = true; } : { inputState->guessing }? curly_pair)*
;

// anonymous delegate
delegate_anonymous[] { ENTRY_DEBUG } :
        {
            // treat catch block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_ANONYMOUS);

            // start of the catch statement
            startElement(SFUNCTION_DELEGATE);
        }
        delegate_marked
        (options { greedy = true; } : parameter_list)*

        /* completely parse a function until it is done */
        (options { greedy = true; } : { inputState->guessing }? curly_pair)*

;

// mark the delegate function
delegate_marked[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }
        DELEGATE
;

lambda_csharp[] { ENTRY_DEBUG } :

    LAMBDA

    {

        if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY) {

            startElement(SPSEUDO_BLOCK);

        } else if(LA(1) == LCURLY)
            startNewMode(MODE_FUNCTION_TAIL | MODE_ANONYMOUS);

    }

;

// throw statement
throw_statement[] { ENTRY_DEBUG } :
        {
            // statement with expected expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start of throw statement
            startElement(STHROW_STATEMENT);
        }
        THROW
;

// C _Generic (generic selection)
generic_selection[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement 
            startNewMode(MODE_LOCAL);

            // start the generic
            startElement(SGENERIC_SELECTION);

            startNewMode(MODE_LIST);

        }
        (
            { inputState->guessing }? GENERIC_SELECTION paren_pair | 
            GENERIC_SELECTION LPAREN generic_selection_selector COMMA generic_selection_association_list rparen[false]
        )

;

generic_selection_selector[] { CompleteElement element(this); ENTRY_DEBUG } :

    {
        startNewMode(MODE_LOCAL);

        startElement(SGENERIC_SELECTOR);

    }
    generic_selection_complete_expression

;

// generic selection association list
generic_selection_association_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // list of parameters
            setMode(MODE_EXPECT | MODE_LIST | MODE_INTERNAL_END_PAREN |  MODE_END_ONLY_AT_RPAREN | MODE_ASSOCIATION_LIST);

            // start the argument list
            startElement(SGENERIC_ASSOCIATION_LIST);
        }
        (COMMA | { LA(1) != RPAREN }? generic_selection_association)*
        //(LPAREN | { setMode(MODE_INTERNAL_END_CURLY); } LCURLY)
;

generic_selection_complete_expression[] { CompleteElement element(this); int count_paren = 1; CALL_TYPE type = NOCALL; 
    bool isempty = false; int call_count = 0; ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_END_AT_COMMA);

            // start the argument list
            startElement(SEXPRESSION);
        }

        (options {warnWhenFollowAmbig = false; } : { count_paren > 0 && (LA(1) != COMMA || !inMode(MODE_END_AT_COMMA)) && (count_paren != 1 || LA(1) != RPAREN) }?

            (
            { !inMode(MODE_END_AT_COMMA) }? comma |

            // argument mode (as part of call)
            { inMode(MODE_ARGUMENT) && LA(1) != RPAREN && LA(1) != RCURLY }? complete_arguments |

            { LA(1) == LPAREN }? expression { ++count_paren; } |

            { LA(1) == RPAREN }? expression { --count_paren; } |

            { perform_call_check(type, isempty, call_count, -1) && type == CALL }? { if(!isempty) ++count_paren; } 
                expression_process (call[call_count] | keyword_calls) complete_arguments  |

            expression
            )
        )*
;

// a generic selection association
generic_selection_association[] { CompleteElement element(this); ENTRY_DEBUG } :

        {
            // argument with nested expression
            startNewMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SGENERIC_ASSOCIATION);
        }

        generic_selection_association_type COLON generic_selection_complete_expression

;

generic_selection_association_type[] { int type_count = 0; int secondtoken = 0;  STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :

    {

        setMode(MODE_ASSOCIATION_TYPE);

    }

    (
    { pattern_check(stmt_type, secondtoken, type_count, true) }?
    variable_declaration_type[type_count + 1] | generic_selection_association_default
    )

    {

        clearMode(MODE_ASSOCIATION_TYPE);

    }

;

generic_selection_association_default[] { SingleElement element(this); ENTRY_DEBUG} :

    {

            startNewMode(MODE_LOCAL);

            startElement(STYPE);

    }
    DEFAULT

;


// an expression statement pre processing
expression_statement_process[] { ENTRY_DEBUG } :
        {

            bool inenumclass = (inLanguage(LANGUAGE_JAVA_FAMILY) && inTransparentMode(MODE_ENUM) && inMode(MODE_CLASS));

            // statement with an embedded expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the element which will end after the terminate
            if(!inenumclass)
                startElement(SEXPRESSION_STATEMENT);
        }
;

// an expression statement
expression_statement[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :

        expression_statement_process

        expression[type, call_count]
;

// declartion statement
variable_declaration_statement[int type_count] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            if(!inTransparentMode(MODE_TYPEDEF) || inTransparentMode(MODE_CLASS | MODE_INNER_DECL))
                // start the declaration statement
                startElement(SDECLARATION_STATEMENT);

        }

        variable_declaration[type_count]
;

// processing for short variable declaration
short_variable_declaration[] { ENTRY_DEBUG } :
        {
            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // declaration
            startNewMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // start the declaration
            startElement(SDECLARATION);
        }
;

// more of the inner part of a declaration
variable_declaration[int type_count] { ENTRY_DEBUG } :
        {

            bool output_decl = !inTransparentMode(MODE_TYPEDEF) || inTransparentMode(MODE_CLASS | MODE_INNER_DECL);

            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // declaration
            startNewMode(MODE_LOCAL| MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            if(inTransparentMode(MODE_FOR_CONDITION | MODE_END_AT_COMMA))
                setMode(MODE_LIST);

            if(output_decl)

                // start the declaration
                startElement(SDECLARATION);

        }

        (options { greedy = true; } : { !isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) && next_token() == TEMPOPS }? template_declaration_full set_int[type_count, type_count - 1])*

        (options { greedy = true; } : { type_count > 0 && (LA(1) != OVERRIDE || !inLanguage(LANGUAGE_CXX)) && (decl_specifier_tokens_set.member(LA(1)) || (inLanguage(LANGUAGE_JAVA) && LA(1) == ATSIGN) 
            || (inLanguage(LANGUAGE_CSHARP) && LA(1) == LBRACKET) || (inLanguage(LANGUAGE_CXX) && LA(1) == LBRACKET && next_token() == LBRACKET))}?
                decl_pre_type[type_count])*

        variable_declaration_type[type_count]
;

// declaration type
variable_declaration_type[int type_count] {  bool is_compound = false; ENTRY_DEBUG } :
        {

            if(type_count == 0) {

                if(inTransparentMode(MODE_ARGUMENT) && inLanguage(LANGUAGE_CXX))
                    return;

                setMode(MODE_VARIABLE_NAME | MODE_INIT);
                return;

            }

            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_EAT_TYPE);

            setTypeCount(type_count);

            // type element begins
            startElement(STYPE);
        }

        // match auto keyword first as special case do no warn about ambiguity
        (options { generateAmbigWarnings = false; } : 
            { LA(1) == CXX_CLASS && keyword_name_token_set.member(next_token()) }? keyword_name | auto_keyword[type_count > 1] |
            { is_class_type_identifier() }? (options { greedy = true; } : 
                { !class_tokens_set.member(LA(1)) }? 
                    (options { generateAmbigWarnings = false; } : specifier | { look_past_rule(&srcMLParser::identifier) != LPAREN }? identifier | macro_call) { decTypeCount(); })*
                    class_type_identifier[is_compound] { decTypeCount(); } (options { greedy = true; } : { !is_compound }?  multops)* |
            lead_type_identifier | EVENT)
        { if(!inTransparentMode(MODE_TYPEDEF)) decTypeCount(); } 

        (options { greedy = true; } : { !inTransparentMode(MODE_TYPEDEF) && getTypeCount() > 0 }?
        (options { generateAmbigWarnings = false; } : keyword_name | type_identifier) { decTypeCount(); })* 
        update_typecount[MODE_VARIABLE_NAME | MODE_INIT]
;

specifiers_or_macro[] { bool first = true; ENTRY_DEBUG } :

    (options { greedy = true; } : specifier)*

    (options { greedy = true; } : { first && !class_tokens_set.member(LA(1)) }? macro_call set_bool[first, false])*

    (options { greedy = true; } : specifier)*


;

is_class_type_identifier[] returns[bool is_class_type = false] { ENTRY_DEBUG 

    if(inputState->guessing || inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST) || inTransparentMode(MODE_ASSOCIATION_TYPE))
        return is_class_type;

    int token = look_past_rule(&srcMLParser::specifiers_or_macro);

    if(class_tokens_set.member(token))
        is_class_type = true;


} :;

class_type_identifier[bool & is_compound] { CompleteElement element(this); ENTRY_DEBUG } :

    {

        startNewMode(MODE_LOCAL);

     
        startElement(SNAME);

    }

    class_type_identifier_keyword class_type_compound_name[is_compound]

;

class_type_compound_name[bool & is_compound] { ; ENTRY_DEBUG } :

    {

        startNewMode(MODE_EXPRESSION);

    }

    (
    { inLanguage(LANGUAGE_JAVA_FAMILY) }?
    compound_name_java[is_compound] |

    { inLanguage(LANGUAGE_CSHARP) }?
    compound_name_csharp[is_compound] |

    { inLanguage(LANGUAGE_OBJECTIVE_C) }?
    compound_name_objective_c[is_compound] |

    { inLanguage(LANGUAGE_C) }?
    compound_name_c[is_compound] |

    { !inLanguage(LANGUAGE_JAVA_FAMILY) && !inLanguage(LANGUAGE_C) && !inLanguage(LANGUAGE_CSHARP) && !inLanguage(LANGUAGE_OBJECTIVE_C) }?
    compound_name_cpp[is_compound] | keyword_name_inner[is_compound] |

    macro_type_name_call 
    )

    (options { greedy = true; } : { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp)*

    {

        endMode();

    }

;

// more lead type identifier
class_type_identifier_keyword[]  { SingleElement element(this); ENTRY_DEBUG } :
        {

            startElement(SNAME);

        }
        (CLASS | CXX_CLASS | STRUCT | UNION | ENUM)
;

// Variable declaration name and optional initialization
variable_declaration_nameinit[] { bool isthis = LA(1) == THIS;
        ENTRY_DEBUG } :

         {

             if(!inMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT)
              && inMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT)
              && !inTransparentMode(MODE_TYPEDEF)
              && !inTransparentMode(MODE_USING)) {

                startNewMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

                // start the declaration
                startElement(SDECLARATION);
                emptyElement(STYPEPREV);

            }

        }

        // Mark as name before mark without name
        (options { generateAmbigWarnings = false;} :  { inLanguage(LANGUAGE_CSHARP) }? compound_name_inner[!isthis] | compound_name | keyword_name)
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

// declartion statement
property_statement[int type_count] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            startElement(SPROPERTY);

            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // declaration
            startNewMode(MODE_LOCAL| MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

        }

        (options { greedy = true; } : { !isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) && next_token() == TEMPOPS }? template_declaration_full set_int[type_count, type_count - 1])*

        (options { greedy = true; } : { type_count > 0 && (LA(1) != OVERRIDE || !inLanguage(LANGUAGE_CXX)) && (decl_specifier_tokens_set.member(LA(1)) || (inLanguage(LANGUAGE_JAVA) && LA(1) == ATSIGN) 
            || (inLanguage(LANGUAGE_CSHARP) && LA(1) == LBRACKET) || (inLanguage(LANGUAGE_CXX) && LA(1) == LBRACKET && next_token() == LBRACKET))}?
                decl_pre_type[type_count])*

        variable_declaration_type[type_count]
;

// declartion statement
event_statement[int type_count] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            startElement(SEVENT);

            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // declaration
            startNewMode(MODE_LOCAL| MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

        }

        (options { greedy = true; } : { !isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) && next_token() == TEMPOPS }? template_declaration_full set_int[type_count, type_count - 1])*

        (options { greedy = true; } : { type_count > 0 && (LA(1) != OVERRIDE || !inLanguage(LANGUAGE_CXX)) && (decl_specifier_tokens_set.member(LA(1)) || (inLanguage(LANGUAGE_JAVA) && LA(1) == ATSIGN) 
            || (inLanguage(LANGUAGE_CSHARP) && LA(1) == LBRACKET) || (inLanguage(LANGUAGE_CXX) && LA(1) == LBRACKET && next_token() == LBRACKET))}?
                decl_pre_type[type_count])*

        EVENT set_int[type_count, type_count - 1]

        variable_declaration_type[type_count]
;

// initializtion of a function pointer.
function_pointer_initialization[] { ENTRY_DEBUG } :
        {
            // end the init correctly
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startElement(SDECLARATION_INITIALIZATION);
        }
        EQUAL
        complete_default_parameter
;

// initialization of a declared variable
variable_declaration_initialization[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startElement(SDECLARATION_INITIALIZATION);
        }
        EQUAL |
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);
        }
        call_argument_list
;

// bit field
variable_declaration_range[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startElement(SDECLARATION_RANGE);
        }

        (COLON | IN) 
;

// parameter variable initialization
parameter_declaration_initialization[] { ENTRY_DEBUG } :
        {
            // end the init correctly
            setMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startElement(SDECLARATION_INITIALIZATION);
        }
        EQUAL
;

// expression block beginning and setup
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
            if ((!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                && (LA(1) != IN || !inTransparentMode(MODE_FOR_CONDITION)))
                startElement(SOPERATOR);
        }
        (
            OPERATORS | ASSIGNMENT | TEMPOPS |
            TEMPOPE (options { greedy = true;  } : ({ SkipBufferSize() == 0 }? TEMPOPE) ({ SkipBufferSize() == 0 }? TEMPOPE)?
             | ({ inLanguage(LANGUAGE_JAVA) && LT(1)->getText() == "&gt;&gt;=" }? ASSIGNMENT))? |
            EQUAL | /*MULTIMM |*/ DESTOP | /* MEMBERPOINTER |*/ MULTOPS | REFOPS | DOTDOT | RVALUEREF | { inLanguage(LANGUAGE_JAVA) }? BAR |

            // others are not combined
            NEW | DELETE | IN | IS | STACKALLOC | AS | AWAIT | LAMBDA | DOTDOTDOT |

            // Objective-C
            CSPEC | MSPEC |

            // Apple
            BLOCKOP

        )
;

// only new operator
sole_new[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        NEW
;

// only ~
sole_destop[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        DESTOP
;

/** list of operators @todo is this still needed */
general_operators_list[] { ENTRY_DEBUG } :
        OPERATORS | ASSIGNMENT  | TEMPOPS | TEMPOPE | EQUAL | /*MULTIMM |*/ DESTOP | /* MEMBERPOINTER |*/ MULTOPS | REFOPS |
        DOTDOT | RVALUEREF | QMARK | CSPEC | MSPEC | BLOCKOP | REF | OUT | DOTDOTDOT
;

// mark up )
rparen_operator[bool markup = true] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (markup && (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                 && !inMode(MODE_END_ONLY_AT_RPAREN))
                startElement(SOPERATOR);
        }
        RPAREN
    ;

//processing on )
rparen[bool markup = true, bool end_for_incr = false] { bool isempty = getParen() == 0; ENTRY_DEBUG } :
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

            if(inMode(MODE_ASSOCIATION_LIST))
                endMode(MODE_ASSOCIATION_LIST);

            if(end_for_incr || inMode(MODE_LIST | MODE_FOR_CONDITION))
                setMode(MODE_END_FOR_CONTROL);

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

                    if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
                        startElement(SPSEUDO_BLOCK);


                    if(cppif_duplicate) {

                        std::stack<int> open_elements;
                        open_elements.push(STHEN);
                        if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
                            open_elements.push(SPSEUDO_BLOCK);

                        dupMode(open_elements);

                    }

                    cppif_duplicate = false;

                }

                // end while condition, etc. and output pseudo block  @todo may need to have a MODE_WHILE
                if(inMode(MODE_LIST | MODE_CONDITION) && inPrevMode(MODE_STATEMENT | MODE_NEST)) {

                    endMode(MODE_LIST);
                    if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
                        startElement(SPSEUDO_BLOCK);

                    if(cppif_duplicate) {

                        std::stack<int> open_elements;
                        if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
                            open_elements.push(SPSEUDO_BLOCK);

                        dupMode(open_elements);

                    }

                    cppif_duplicate = false;


                // end for group and output pseudo block @todo make sure does not hid other things that use for grammar
                } else if(end_for_incr) {

                    if(inMode(MODE_LIST))
                        endMode(MODE_LIST);
    
                    if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
                        startElement(SPSEUDO_BLOCK);

                    if(cppif_duplicate) {

                        std::stack<int> open_elements;
                        if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
                            open_elements.push(SPSEUDO_BLOCK);

                        dupMode(open_elements);

                    }

                    cppif_duplicate = false;

                } else if(inMode(MODE_LIST | MODE_FOR_CONDITION)) {

                    endMode(MODE_FOR_CONDITION);
                    if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)
                        startElement(SPSEUDO_BLOCK);

                    if(cppif_duplicate) {

                        std::stack<int> open_elements;
                        if(isoption(parser_options, SRCML_OPTION_PSEUDO_BLOCK) && LA(1) != LCURLY)                        
                            open_elements.push(SPSEUDO_BLOCK);

                        dupMode(open_elements);

                    }

                    cppif_duplicate = false;

                } else

                // end the single mode that started the list
                // don't end more than one since they may be nested
                if (inMode(MODE_LIST))
                    endMode(MODE_LIST);

            }

        }
;

// } matching and processing
rcurly_argument[] { bool isempty = getCurly() == 0; ENTRY_DEBUG } :
        {

            if(isempty) {

                // additional right parentheses indicates end of non-list modes
                if(inTransparentMode(MODE_LIST))
                    endDownToModeSet(MODE_LIST | MODE_PREPROC | MODE_END_ONLY_AT_RPAREN | MODE_ONLY_END_TERMINATE | MODE_INTERNAL_END_CURLY);
            }

        }
        RCURLY
        {

            // end the single mode that started the list
            // don't end more than one since they may be nested
            if (isempty && inMode(MODE_LIST))
                while(inMode(MODE_LIST) && (!inMode(MODE_INTERNAL_END_PAREN) || inMode(MODE_END_ONLY_AT_RPAREN)))
                    endMode(MODE_LIST);
            
            else if(inTransparentMode(MODE_EXPRESSION | MODE_LIST | MODE_TOP))
                endWhileMode(MODE_EXPRESSION | MODE_LIST | MODE_TOP);

            if(!isempty)
                decCurly();
        }

;

rbracket[] { ENTRY_DEBUG } :
    {

        endDownOverMode(MODE_LIST);

    }

    RBRACKET

    {

        if(inMode(MODE_OBJECTIVE_C_CALL)) {

            endDownOverMode(MODE_OBJECTIVE_C_CALL);

        }

    }

; 

// Dot (period) operator
period[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        PERIOD
;

// -> operator
member_pointer[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
      TRETURN  
;

// ->* operator
member_pointer_dereference[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
      MPDEREF  
;

// .* operator
dot_dereference[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        DOTDEREF
;

// Namespace operator '::'
dcolon[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        DCOLON
;

// process portion of expression
expression_process[] { ENTRY_DEBUG } :
        {
            // if expecting an expression start one. except if you are at a right curly brace
            if (inMode(MODE_EXPRESSION | MODE_EXPECT) && LA(1) != RCURLY &&
                !(inMode(MODE_FOR_INCREMENT) && LA(1) == RPAREN)) {

                // use a new mode without the expect so we don't nest expression parts
                startNewMode(MODE_EXPRESSION);

                if(inPrevMode(MODE_TERNARY_CONDITION))
                   setMode(MODE_TERNARY_CONDITION);

                // start the expression element
                startElement(SEXPRESSION);
            }
        }
;

// an expression
expression[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :

        expression_process

        expression_part_plus_linq[type, call_count]
;

// setup for expression linq
expression_setup_linq[CALL_TYPE type = NOCALL] { ENTRY_DEBUG } :

        expression_process

        expression_part[type]
;

// expression with linq
expression_part_plus_linq[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :

        { inLanguage(LANGUAGE_CSHARP) && next_token() != RPAREN && next_token() != ASSIGNMENT && next_token() != EQUAL }?
        (linq_expression_pure)=> linq_expression |

        expression_part[type, call_count]
;

// the expression part
expression_part[CALL_TYPE type = NOCALL, int call_count = 1] { bool flag; bool isempty = false; bool end_for_incr = false; ENTRY_DEBUG } :

       { isoption(parser_options, SRCML_OPTION_TERNARY) && !skip_ternary && !inMode(MODE_TERNARY_CONDITION)
            && (!inLanguage(LANGUAGE_JAVA) || !inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST))
            && perform_ternary_check() }? ternary_expression |

        // cast
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        UNION |

        // cast
        { inTransparentMode(MODE_INTERNAL_END_PAREN) && (LA(1) != CXX_CLASS || !keyword_name_token_set.member(next_token())) }?
        (CLASS | CXX_CLASS) |

        { next_token() == LPAREN || next_token() == LCURLY }?
        delegate_anonymous |

        { next_token() == LCURLY }?
        lambda_anonymous |

        { inLanguage(LANGUAGE_CSHARP) }?
        (lambda_expression_full_csharp) => lambda_expression_csharp |

        { inLanguage(LANGUAGE_CXX) }?
        (bracket_pair (options { warnWhenFollowAmbig = false; } : paren_pair)* function_tail LCURLY) => lambda_expression_cpp |

        { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
        (block_lambda_expression_full) => block_lambda_expression |

        { inLanguage(LANGUAGE_JAVA) }?
        ((paren_pair | variable_identifier) TRETURN) => lambda_expression_java |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW generic_argument_list)=> sole_new generic_argument_list |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW function_identifier paren_pair LCURLY)=> sole_new anonymous_class_definition |

        { notdestructor }? sole_destop { notdestructor = false; } |

        { next_token() != LPAREN && next_token() != DOTDOTDOT }? sizeof_unary_expression |

        // call
        // distinguish between a call and a macro
        { type == CALL || (perform_call_check(type, isempty, call_count, -1) && type == CALL) }?

            // Added argument to correct markup of default parameters using a call.
            // normally call claims left paren and start calls argument.
            // however I believe parameter_list matches a right paren of the call.
           (call[call_count] | keyword_calls) argument |

        // macro call
        { type == MACRO }? macro_call |

        { inLanguage(LANGUAGE_OBJECTIVE_C) }?
        objective_c_call | 

        // general math operators
        // looks like general operators and variable identifier can match same thing
        (options { generateAmbigWarnings = false; } : general_operators
        {
            if (inLanguage(LANGUAGE_CXX_FAMILY) && LA(1) == DESTOP)
                general_operators();
        }
        | qmark | /* newop | */ period | member_pointer | member_pointer_dereference | dot_dereference |

        // left parentheses
        { function_pointer_name_check() }?
        function_pointer_name |
        lparen_marked
        {
            startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);
        }

        // can have (ternary) in a ternary condition
        (options { greedy = true; } : { isoption(parser_options, SRCML_OPTION_TERNARY) && !skip_ternary && inMode(MODE_TERNARY_CONDITION)
            && (!inLanguage(LANGUAGE_JAVA) || !inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST))
            && perform_ternary_check() }? ternary_expression)* |

        // right parentheses that only matches a left parentheses of an expression
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        {

            end_for_incr = inTransparentMode(MODE_FOR_INCREMENT);

            // stop at this matching paren, or a preprocessor statement
            endDownToModeSet(MODE_INTERNAL_END_PAREN | MODE_PREPROC);
            
            if (inMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN))
                endMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);

            end_for_incr = end_for_incr && !inTransparentMode(MODE_FOR_INCREMENT);

        }

        // treat as operator for operator markup
        rparen[!end_for_incr, end_for_incr] |

        // left curly brace
        {
            startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_TOP);

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
        variable_identifier | keyword_name | auto_keyword[false] | single_keyword_specifier) | literals | noexcept_list | 

        variable_identifier_array_grammar_sub[flag]
;

// rule for literals
literals[] { ENTRY_DEBUG } :
        string_literal | char_literal | literal | boolean | null_literal | complex_literal | nil_literal
;

// Only start and end of strings are put directly through the parser.
// The contents of the string are handled as whitespace.
string_literal[bool markup = true] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup strings in literal option
            if (markup && (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_LITERAL)))
                startElement(SSTRING);
        }
        (STRING_START STRING_END)
;

// Only start and end of character are put directly through the parser.
// The contents of the character are handled as whitespace.
char_literal[bool markup = true] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup characters in literal option
            if (markup && (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_LITERAL)))
                startElement(SCHAR);
        }
        (CHAR_START CHAR_END)
;

// literals
null_literal[]{ LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup literals in literal option
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_LITERAL))
                startElement(SNULL);
        }
        (NULLPTR | NULLLITERAL)
;

// literals
nil_literal[]{ LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup literals in literal option
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_LITERAL))
                startElement(SNIL);
        }
        NIL
;

// complex numbers
complex_literal[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup literals in literal option
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_LITERAL))
                startElement(SCOMPLEX);
        }
        COMPLEX_NUMBER ({ (LT(1)->getText() == "+" || LT(1)->getText() == "-") && next_token() == CONSTANTS }? OPERATORS CONSTANTS)?
  
;


// literal numbers
literal[bool markup = true] { LightweightElement element(this); TokenPosition tp; ENTRY_DEBUG } :
        {
            // only markup literals in literal option
            if (markup && (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_LITERAL))) {

                startElement(SLITERAL);

                setTokenPosition(tp);

            }

        }
        CONSTANTS ({ (LT(1)->getText() == "+" || LT(1)->getText() == "-") && next_token() == COMPLEX_NUMBER }? OPERATORS COMPLEX_NUMBER {  tp.setType(SCOMPLEX); })?
;


// booleans
boolean[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup boolean values in literal option
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_LITERAL))
                startElement(SBOOLEAN);
        }
        (TRUE | FALSE)
;

// a derived class
derived[] { CompleteElement element(this); bool first = true; ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SDERIVATION_LIST);
        }
        COLON
        (options { greedy = true; } :
            { LA(1) != WHERE && (!inLanguage(LANGUAGE_OBJECTIVE_C) || first) }? (
            (derive_access)*

            ({ inLanguage(LANGUAGE_OBJECTIVE_C) }? identifier | variable_identifier)
            ({ inLanguage(LANGUAGE_CSHARP) }? period variable_identifier)*

            (options { greedy = true; } : { !inLanguage(LANGUAGE_OBJECTIVE_C) }? generic_argument_list)*

            set_bool[first, false]
            )
        |
            COMMA
        )*
;

// super list
super_list_java[] { ENTRY_DEBUG } :
        {
            // end all elements at end of rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SDERIVATION_LIST);
        }
;

// extends list
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

// implements list
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

// super list
super_list[] { ENTRY_DEBUG } :
        (options { greedy = true; } :
            (derive_access)*

            variable_identifier
        |
            COMMA
        )*
;

// a derive access
derive_access[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCLASS_SPECIFIER);
        }
        (VIRTUAL)* (PUBLIC | PRIVATE | PROTECTED) (options { greedy = true; } : VIRTUAL)*
;

// do a parameter list
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

// indexer parameter list
indexer_parameter_list[] { bool lastwasparam = false; ENTRY_DEBUG } :
        {
            // list of parameters
            startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);

            // start the parameter list element
            startElement(SINDEXER_PARAMETER_LIST);
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

// an empty element
empty_element[int ele, bool cond] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (cond)
                startElement(ele);
        }
;

// k & r C parameter
kr_parameter[int type_count] { ENTRY_DEBUG } :
        kr_parameter_type[type_count] kr_parameter_name kr_parameter_terminate
        //complete_parameter terminate_pre terminate_token
;

// k & r C parameter type
kr_parameter_type[int type_count] { ENTRY_DEBUG} :
        // suppress ()* warning
        variable_declaration_statement[type_count] (options { greedy = true; } : { inMode(MODE_EAT_TYPE) }? type_identifier update_typecount[MODE_FUNCTION_NAME])* 
;

// k & r C parameter name
kr_parameter_name[] { ENTRY_DEBUG } :
        ((comma)* variable_declaration_nameinit)*
;

// k & r C terminate
kr_parameter_terminate[] { ENTRY_DEBUG } :
    terminate_pre terminate_token { endDownToModeSet(MODE_FUNCTION_TAIL); }
;

// complete parameter
complete_parameter[] { ENTRY_DEBUG } :
        parameter
        // suppress ()* warning
        (options { greedy = true; } : parameter_declaration_initialization complete_default_parameter)*
;

// an argument
argument[] { ENTRY_DEBUG } :
        { getParen() == 0 }? rparen[false] |
        { getCurly() == 0 }? rcurly_argument |
        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        }

        (options { greedy = true; } : { inLanguage(LANGUAGE_CSHARP)  && look_past_rule(&srcMLParser::identifier) == COLON }? argument_named_csharp)*
        (options { greedy = true; } : { inLanguage(LANGUAGE_CSHARP) }? argument_modifier_csharp)*

        (
        { !((LA(1) == RPAREN && inTransparentMode(MODE_INTERNAL_END_PAREN)) || (LA(1) == RCURLY && inTransparentMode(MODE_INTERNAL_END_CURLY))) }? expression |

        (type_identifier) => expression_process type_identifier
        )

;

argument_modifier_csharp[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_MODIFIER))
                startElement(SMODIFIER);
        }
        (OUT | REF)

;

argument_named_csharp[] { ENTRY_DEBUG } :

        identifier COLON

;

// annotation argument
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
        { inputState->guessing }? RCURLY | 
        { !(LA(1) == RPAREN) }? expression | type_identifier)*

;

// a parameter
parameter[] { int type_count = 0; int secondtoken = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            // end parameter correctly
            startNewMode(MODE_PARAMETER);

            // start the parameter element
            startElement(SPARAMETER);
        }
        (

            { pattern_check(stmt_type, secondtoken, type_count, true) && (stmt_type == FUNCTION_DECL || stmt_type == FUNCTION
                || stmt_type == OPERATOR_FUNCTION_DECL || stmt_type == OPERATOR_FUNCTION) }?
            function_declaration[type_count]

            function_identifier // pointer_name_grammar

            (macro_call_check)*

            parameter_list |
   
            parameter_type_variable[type_count, stmt_type]

        )
;

// handle parameter type if not function_decl
parameter_type_variable[int type_count, STMT_TYPE stmt_type] { bool output_type = true; ENTRY_DEBUG } :
        {

                // start the declaration element
                startElement(SDECLARATION);

                if (stmt_type != VARIABLE)
                    type_count = 1;

                int look_past_token = 0;
                output_type = !((inLanguage(LANGUAGE_JAVA) || inLanguage(LANGUAGE_CSHARP)) && type_count == 1 && LA(1) != DOTDOTDOT && inTransparentMode(MODE_FUNCTION_TAIL | MODE_ANONYMOUS)
                    && ((look_past_token = look_past_rule(&srcMLParser::type_identifier)) == COMMA ||
                        look_past_token == RPAREN || look_past_token == TRETURN || look_past_token == LAMBDA));


        }

        (
        { stmt_type == VARIABLE || stmt_type == CLASS_DECL || stmt_type == STRUCT_DECL || stmt_type == UNION_DECL || stmt_type == ENUM_DECL|| LA(1) == DOTDOTDOT }?
        (parameter_type_count[type_count, output_type])
        // suppress warning caused by ()*
        (options { greedy = true; } : bar set_int[type_count, type_count > 1 ? type_count - 1 : 1] parameter_type_count[type_count])*
        {
            // expect a name initialization
            setMode(MODE_VARIABLE_NAME | MODE_INIT);
        }
        ( options { greedy = true; } : variable_declaration_nameinit)*
        )

;

// count types in parameter
parameter_type_count[int & type_count, bool output_type = true] { CompleteElement element(this); bool is_compound = false; ENTRY_DEBUG } :
        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);

            // start of type
            if(output_type)
                startElement(STYPE);
        }


        // match auto keyword first as special case do no warn about ambiguity
        ((options { generateAmbigWarnings = false; } : this_specifier | auto_keyword[type_count > 1] |
         { is_class_type_identifier() }? (options { greedy = true; } :
            { !class_tokens_set.member(LA(1)) }?
                (options { generateAmbigWarnings = false; } : specifier | { look_past_rule(&srcMLParser::identifier) != LPAREN }? identifier | macro_call) set_int[type_count, type_count - 1])*
                class_type_identifier[is_compound] set_int[type_count, type_count - 1] (options { greedy = true; } : { !is_compound }? multops)* |
         type_identifier) set_int[type_count, type_count - 1] (options { greedy = true;} : eat_type[type_count])?)

        // sometimes there is no parameter name.  if so, we need to eat it
        ( options { greedy = true; generateAmbigWarnings = false; } : multops | tripledotop | LBRACKET RBRACKET |
         { next_token() == MULTOPS || next_token() == REFOPS || next_token() == RVALUEREF || (inLanguage(LANGUAGE_CSHARP) &&  next_token() == QMARK) || next_token() == BLOCKOP }? type_identifier)*
;

// Modifier ops
multops[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_MODIFIER))
                startElement(SMODIFIER);
        }
        (MULTOPS | REFOPS | RVALUEREF | { inLanguage(LANGUAGE_CSHARP) }? QMARK set_bool[is_qmark, true] | BLOCKOP)
;

modifiers_csharp[] { LightweightElement element(this); ENTRY_DEBUG } :
    {
        // markup type modifiers if option is on
        if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_MODIFIER))
            startElement(SMODIFIER);
    }
    (REF | OUT)

 ;

// ...
tripledotop[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_MODIFIER))
                startElement(SMODIFIER);
        }
        DOTDOTDOT
;

// do a parameter type
parameter_type[] { CompleteElement element(this); int type_count = 0; int secondtoken = 0; STMT_TYPE stmt_type = NONE; bool is_compound = false; ENTRY_DEBUG } :
        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);

            // start of type
            startElement(STYPE);
        }
        { pattern_check(stmt_type, secondtoken, type_count) && (type_count ? type_count : (type_count = 1))}?

        // match auto keyword first as special case do no warn about ambiguity
        ((options { generateAmbigWarnings = false; } : auto_keyword[type_count > 1] |
         { is_class_type_identifier() }? (options { greedy = true; } :
            { !class_tokens_set.member(LA(1)) }?
                (options { generateAmbigWarnings = false; } : specifier | { look_past_rule(&srcMLParser::identifier) != LPAREN }? identifier | macro_call) set_int[type_count, type_count - 1])*
                class_type_identifier[is_compound] set_int[type_count, type_count - 1] (options { greedy = true; } : { !is_compound }? multops)* |
         type_identifier) set_int[type_count, type_count - 1] (options { greedy = true;} : eat_type[type_count])?)
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
        TEMPLATE
        {
            if(LA(1) == CLASS || LA(1) == CXX_CLASS)
                startNewMode(MODE_TEMPLATE | MODE_LIST | MODE_EXPECT);
            else
                startNewMode(MODE_TEMPLATE | MODE_LIST | MODE_EXPECT | MODE_TEMPLATE_PARAMETER_LIST);
        }
;

// start parameter list for templates
template_param_list[] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_PARAMETER | MODE_LIST);

            // start the template parameter list
            startElement(STEMPLATE_PARAMETER_LIST);
        }
        tempops
;

// parameter in template
template_param[] { in_template_param = true; ENTRY_DEBUG } :
        {
            // end parameter correctly
            startNewMode(MODE_LOCAL);

            // start the parameter element
            startElement(STEMPLATE_PARAMETER);
        }

        // Both can contain extern however an extern template should not be a template param so should not be a problem
        (options { generateAmbigWarnings = false; } :
        { LA(1) != TEMPLATE }? parameter_type
        {
            // expect a name initialization

            setMode(MODE_VARIABLE_NAME | MODE_INIT);
        }  
        (options { greedy = true; } : { !isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) }? variable_declaration_nameinit)* |

        template_inner_full
    )
    set_bool[in_template_param, false]
;
exception
catch[antlr::RecognitionException] {

    in_template_param = false;
    throw antlr::RecognitionException();

}

// complete inner full for template
template_inner_full[] { ENTRY_DEBUG int type_count = 0; int secondtoken = 0; STMT_TYPE stmt_type = NONE; } :

        template_in_parameter_list_full
        { pattern_check(stmt_type, secondtoken, type_count) && (type_count ? type_count : (type_count = 1))}?
        eat_type[type_count]
        {
            endMode();

            // expect a name initialization
            setMode(MODE_VARIABLE_NAME | MODE_INIT);
        }
        (options { greedy = true; } : { !isoption(parser_options, SRCML_OPTION_WRAP_TEMPLATE) }? variable_declaration_nameinit)*

;

// entire template parameter list
template_in_parameter_list_full[] { ENTRY_DEBUG } :

        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);
            
            // start of type
            startElement(STYPE);
        }

        template_declaration_full

;

template_declaration_full[] { ENTRY_DEBUG } :

    template_declaration template_param_list (template_param (template_declaration_initialization)* (comma)*)* tempope { if(inMode(MODE_TEMPLATE)) endMode();}

;

// template initialization
template_declaration_initialization[] { ENTRY_DEBUG } :
        {
            // end the init correctly
            setMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startElement(SDECLARATION_INITIALIZATION);
        }
        EQUAL expression
;

// generic argument list
generic_argument_list[] { CompleteElement element(this); std::string namestack_save[2];  bool in_function_type = false; ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            in_function_type = inPrevMode(MODE_FUNCTION_TYPE);

            if(!inLanguage(LANGUAGE_JAVA) || (!inTransparentMode(MODE_CLASS_NAME) && !in_function_type))
                startElement(SGENERIC_ARGUMENT_LIST);
            else
                startElement(STEMPLATE_PARAMETER_LIST);
   
        }
        savenamestack[namestack_save]

        tempops (options { generateAmbigWarnings = false; } : COMMA | template_argument[in_function_type])* tempope

        restorenamestack[namestack_save]
;

// generic parameter list
generic_parameter_list[] { CompleteElement element(this); std::string namestack_save[2];  ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SGENERIC_PARAMETER_LIST);
   
        }
        savenamestack[namestack_save]

        tempops (options { generateAmbigWarnings = false; } : COMMA | generic_parameter)* tempope

        restorenamestack[namestack_save]
;

// generic parameter
generic_parameter[] { CompleteElement element(this); ENTRY_DEBUG } :
        {

            // local mode
            startNewMode(MODE_LOCAL);

            startElement(STEMPLATE_PARAMETER);

        }
        (options { greedy = true; } :
        { LA(1) != SUPER && LA(1) != QMARK }?

        (options { generateAmbigWarnings = false; } : generic_specifiers_csharp)*
        ((options { generateAmbigWarnings = false; } : { LA(1) != IN }? template_operators)*

        (type_identifier | literals)
            (options { generateAmbigWarnings = false; } : template_operators)*
            ) |

            template_extends_java |

            template_super_java | qmark_name |
            template_argument_expression
        )+ 
;

// CUDA argument list
cuda_argument_list[] { CompleteElement element(this); std::string namestack_save[2]; ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SCUDA_ARGUMENT_LIST);
        }
        savenamestack[namestack_save]

        cuda_start (options { generateAmbigWarnings = false; } : COMMA | template_argument)* cuda_end

        restorenamestack[namestack_save]
;

// beginning of cuda argument list
cuda_start[] { ENTRY_DEBUG } :
        {
            // make sure we are in a list mode so that we can end correctly
            // some uses of tempope will have their own mode
            if (!inMode(MODE_LIST))
                startNewMode(MODE_LIST);
        }
        CUDA
;

// end of cuda argument list
cuda_end[] { ENTRY_DEBUG } :
        {
            // end down to the mode created by the start cuda argument list operator
            endDownToMode(MODE_LIST);
        }
        TEMPOPE TEMPOPE TEMPOPE
        {
            // end the mode created by the start cuda argument list operator
            while (inMode(MODE_LIST))
                endMode(MODE_LIST);
        }
;

// generic type constraint
generic_type_constraint[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SWHERE);
        }
        WHERE compound_name_inner[false] COLON type_constraint (options { greedy = true; } : COMMA type_constraint)*

;

type_constraint[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SCONSTRAINT);
        }

        (compound_name_inner[false] | CLASS | CXX_CLASS | STRUCT | new_constraint)
;

new_constraint[] { ENTRY_DEBUG } :

    new_name LPAREN RPAREN

;


new_name[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        NEW

;

protocol_list[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        // local mode
        startNewMode(MODE_LOCAL | MODE_LIST);

        startElement(SPROTOCOL_LIST);

    }
    TEMPOPS identifier (COMMA identifier)* TEMPOPE

;

category[] { CompleteElement element(this); ENTRY_DEBUG } :
    {

        // local mode
        startNewMode(MODE_LOCAL);

        startElement(SCATEGORY);

    }
    LPAREN (identifier)* RPAREN

;

// save the namestack
savenamestack[std::string namestack_save[]] { namestack_save[0].swap(namestack[0]); namestack_save[1].swap(namestack[1]); ENTRY_DEBUG } :;

// restore the namestack
restorenamestack[std::string namestack_save[]] { namestack[0].swap(namestack_save[0]); namestack[1].swap(namestack_save[1]); ENTRY_DEBUG } :;

// template argument
template_argument[bool in_function_type = false] { CompleteElement element(this); ENTRY_DEBUG } :
        {

            // local mode
            startNewMode(MODE_LOCAL);

            if(!inLanguage(LANGUAGE_JAVA) || (!inTransparentMode(MODE_CLASS_NAME) && !in_function_type))
               startElement(SGENERIC_ARGUMENT);
            else
               startElement(STEMPLATE_PARAMETER);

            if(inLanguage(LANGUAGE_CXX) | inLanguage(LANGUAGE_C))
               startElement(SEXPRESSION);
        }
        (options { greedy = true; } :
        { LA(1) != SUPER && LA(1) != QMARK }?

        (options { generateAmbigWarnings = false; } : generic_specifiers_csharp)*
        ((options { generateAmbigWarnings = false; } : { LA(1) != IN }? template_operators)*

        (type_identifier | literals)
            (options { generateAmbigWarnings = false; } : template_operators)*
            ) |

            template_extends_java |

            template_super_java | qmark_name |
            template_argument_expression
        )+ 
;

// template argument expression
template_argument_expression[] { ENTRY_DEBUG } :

        lparen_marked
        
        // qmark matches before template argument expression is fine
        ({ LA(1) != RPAREN }? (options { generateAmbigWarnings = false; } : general_operators | qmark | (variable_identifier)=>variable_identifier | literals | type_identifier | template_argument_expression))*
       rparen_operator[true]

;

// All possible operators
template_operators[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!isoption(parser_options, SRCML_OPTION_OPTIONAL_MARKUP) || isoption(parser_options, SRCML_OPTION_OPERATOR))
                startElement(SOPERATOR);
        }
        (
        OPERATORS | TRETURN | TEMPOPS | EQUAL | MULTOPS | REFOPS | DOTDOT | RVALUEREF |
        QMARK | NEW | DELETE | IN | IS | STACKALLOC | AS | AWAIT | LAMBDA
        )
;

generic_specifiers_csharp[] { LightweightElement element(this); ENTRY_DEBUG } :
    {
        startElement(SFUNCTION_SPECIFIER);
    }
    (IN | OUT)
;

// template extends
template_extends_java[] { CompleteElement element(this); bool is_compound = false; ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SEXTENDS);
        }
        EXTENDS
        compound_name_java[is_compound]
;

// template super 
template_super_java[] { CompleteElement element(this); bool is_compound = false; ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SDERIVATION_LIST);
        }
        SUPER
        compound_name_java[is_compound]
;

// beginning of template parameter list
tempops[] { ENTRY_DEBUG } :
        {
            // make sure we are in a list mode so that we can end correctly
            // some uses of tempope will have their own mode
            if (!inMode(MODE_LIST))
                startNewMode(MODE_LIST);
        }
        TEMPOPS
;

// end of template parameter list
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

// a label
label_statement[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the label element
            startElement(SLABEL_STATEMENT);
        }
        (identifier | keyword_identifier) COLON
;

// a label
macro_label_statement[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the label element
            startElement(SLABEL_STATEMENT);
        }
        macro_label_call COLON
;

// typedef
typedef_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_EXPECT | MODE_VARIABLE_NAME | MODE_ONLY_END_TERMINATE);

            // start the typedef element
            startElement(STYPEDEF);

            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TYPEDEF | MODE_END_AT_BLOCK_NO_TERMINATE);
        }
        TYPEDEF
;

// matching set of parenthesis
paren_pair[] { ENTRY_DEBUG } :
        LPAREN (paren_pair | qmark | ~(QMARK | LPAREN | RPAREN))* RPAREN
;

// matching set of curly braces
curly_pair[] { ENTRY_DEBUG } :
        LCURLY (curly_pair | qmark | ~(QMARK | LCURLY | RCURLY))* RCURLY
;

// matching set of brackets
bracket_pair[] { ENTRY_DEBUG } :
        LBRACKET (bracket_pair | qmark | ~(QMARK | LBRACKET | RBRACKET))* RBRACKET
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

enum_preprocessing[bool decl] { ENTRY_DEBUG} :

        {
            bool intypedef = inMode(MODE_TYPEDEF);

            if (intypedef)
                startElement(STYPE);

            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_ENUM | MODE_DECL);

            // start the enum definition
            if(inLanguage(LANGUAGE_CXX) && (next_token() == CLASS || next_token() == CXX_CLASS || next_token() == STRUCT || next_token() == UNION)) {

                if(decl)
                    startElement(SENUM_CLASS_DECLARATION);
                else
                    startElement(SENUM_CLASS);

            } else if(decl) {

                startElement(SENUM_DECLARATION);

            } else {

                 startElement(SENUM);

             }

            // classes end at the end of the block
            if (intypedef) {
                setMode(MODE_END_AT_BLOCK);
            }
        }
;


// definition of an enum
enum_definition[] { ENTRY_DEBUG } :
        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        enum_class_definition |

        { inLanguage(LANGUAGE_CSHARP) }?
        enum_csharp_definition |
        enum_preprocessing[false] (options { greedy = true; } : specifier)* ENUM (options { greedy = true; } : enum_class_header)*
;

// declaration of an enum
enum_declaration[] { ENTRY_DEBUG } :
        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        enum_class_declaration |

        { inLanguage(LANGUAGE_CSHARP) }?
        enum_csharp_declaration |
        enum_preprocessing[true] (options { greedy = true; } : specifier)* ENUM (options { greedy = true; } : enum_class_header)*
;

// header for enum class
enum_class_header[] {} :
        (CLASS | CXX_CLASS | STRUCT | UNION)* 
        ({ inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET}? attribute_cpp)*
        variable_identifier (COLON enum_type)* (options { greedy = true; } : COMMA variable_identifier (COLON enum_type)*)*

    ;

// type portion of enum
enum_type { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(STYPE);
        }
        // suppress warning compound_name seems to have some tokens in common with specifier.
        (options { generateAmbigWarnings = false; } : specifier | compound_name)*
    ;

enum_csharp_definition[] { ENTRY_DEBUG } :

    // may need to modifiy to work with enum_decl
    enum_preprocessing[false] class_preamble ENUM (options { greedy = true; } : variable_identifier)* ({ inLanguage(LANGUAGE_CXX_FAMILY) }? (options { greedy = true; } : derived))*

;

enum_csharp_declaration[] { ENTRY_DEBUG } :

    // may need to modifiy to work with enum_decl
    enum_preprocessing[true] class_preamble ENUM (options { greedy = true; } : variable_identifier)* ({ inLanguage(LANGUAGE_CXX_FAMILY) }? (options { greedy = true; } : derived))*
    (COMMA (options { greedy = true; } : variable_identifier)* ({ inLanguage(LANGUAGE_CXX_FAMILY) }? (options { greedy = true; } : derived))*)*

;

// Complete definition of an enum.  Used for enum's embedded in typedef's where the entire
// enum must be parsed since it is part of the type.
enum_definition_complete[] { CompleteElement element(this); ENTRY_DEBUG } :
        enum_definition

        (variable_identifier)*

        // start of enum definition block
        {
            startNewMode(MODE_TOP | MODE_LIST | MODE_DECL | MODE_EXPECT | MODE_BLOCK | MODE_NEST);

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

// enum block beginning and setup
enum_block[] { ENTRY_DEBUG } :
        lcurly_base
        {
            // nesting blocks, not statement
            if(inLanguage(LANGUAGE_JAVA))
                setMode(MODE_TOP | MODE_STATEMENT | MODE_NEST | MODE_LIST | MODE_BLOCK | MODE_ENUM);
            else {

                setMode(MODE_TOP | MODE_STATEMENT | MODE_NEST | MODE_LIST | MODE_BLOCK | MODE_ENUM);

            }
        }
;

// processing for short variable declaration
enum_short_variable_declaration[] { ENTRY_DEBUG } :
        {
            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // declaration
            startNewMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // start the declaration
            startElement(SDECLARATION);
        }
        variable_declaration_nameinit
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
            startElement(SCPP_EMPTY);

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
        (options { greedy = true; } : cpp_define_name (options { greedy = true; } : cpp_define_value)*)* |

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
            (options { generateAmbigWarnings = false; } : cpp_linenumber

            (cpp_filename)* | { inLanguage(LANGUAGE_CSHARP) }? cpp_symbol_optional) |

        PRAGMA
        {
            endMode();

            tp.setType(SCPP_PRAGMA);
        } ({ isoption(parser_options, SRCML_OPTION_OPENMP) }? omp_directive | (options { generateAmbigWarnings = false; } : cpp_literal | cpp_symbol)*) |

        ERRORPREC
        {
            endMode();

            tp.setType(SCPP_ERROR);
        } (cpp_literal)* |

        WARNING
        {
            endMode();

            tp.setType(SCPP_WARNING);
        } (cpp_literal)* |

        REGION
        {
            endMode();

            tp.setType(SCPP_REGION);
        } (cpp_symbol)* |

        ENDREGION
        {
            endMode();

            tp.setType(SCPP_ENDREGION);
        } |

        IMPORT
        {
            endMode();

            tp.setType(SCPP_IMPORT);
        }
        (cpp_filename)* |

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

// do all the cpp garbage
cpp_garbage[] :

 ~(EOL | LINECOMMENT_START | COMMENT_START | JAVADOC_COMMENT_START | DOXYGEN_COMMENT_START | LINE_DOXYGEN_COMMENT_START) /* @todo re-add EOF antlr is generating 1 for EOF but EOF hold -1 there is token
 with 1 EOF_ however, adding fails to generate */
;

cpp_check_end[] returns[bool is_end = false] {

 if(LA(1) == EOL || LA(1) == LINECOMMENT_START || LA(1) == COMMENT_START || LA(1) == JAVADOC_COMMENT_START || LA(1) == DOXYGEN_COMMENT_START || LA(1) == LINE_DOXYGEN_COMMENT_START || LA(1) == EOF || LA(1) == 1)  /* EOF */
     return true;

 return false;

}:;

// skip to eol
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
            endWhileMode(MODE_PREPROC);

            endMode(MODE_PARSE_EOL);
ENTRY_DEBUG } :

        {
            if(directive_token == ENDIF) {

                bool end_statement = inMode(MODE_END_AT_ENDIF);
                while(inMode(MODE_END_AT_ENDIF))
                    endMode();

                if(end_statement)
                    else_handling();


            }

            if(LA(1) == 1) {

                eol_post(directive_token, markblockzero);
                return;
            }

        }

        (EOL | LINECOMMENT_START | COMMENT_START | JAVADOC_COMMENT_START | DOXYGEN_COMMENT_START | LINE_DOXYGEN_COMMENT_START | eof)
        eol_post[directive_token, markblockzero]
;

cppendif_skip[] {

    int prev = -1;
    int endif_count = 1;

    while(endif_count && LA(1) != 1 /* EOF */) {

        if((prev == PREPROC && LA(1) == IF) || LA(1) == IFDEF || LA(1) == IFNDEF)
            ++endif_count;

        if(LA(1) == ENDIF)
            --endif_count;

        prev = LA(1);
        consume();
    }

}:;

cppif_end_count_check[] returns [std::list<int> end_order] {

    int start = mark();
    std::list<int> op_stack;
    ++inputState->guessing;

    std::list<int>::size_type save_size = 0;

    int prev = -1;
    while(LA(1) != ENDIF && !(prev == PREPROC && LA(1) == ELSE) && LA(1) != 1 /* EOF */) {

        if((prev == PREPROC && LA(1) == IF) || LA(1) == IFDEF || LA(1) == IFNDEF) {
            cppendif_skip();
            continue;
        }

        if(LA(1) == ELIF) save_size = end_order.size();

        if(LA(1) == LPAREN) op_stack.push_back(LPAREN);
        if(LA(1) == RPAREN) {
            if(!op_stack.empty() && op_stack.back() == LPAREN) op_stack.pop_back();
            else end_order.push_back(RPAREN);
        }

        if(LA(1) == LCURLY) op_stack.push_back(LCURLY);
        if(LA(1) == RCURLY) {
            if(!op_stack.empty() && op_stack.back() == LCURLY) op_stack.pop_back();
            else end_order.push_back(RCURLY);
        }

        if(LA(1) == TERMINATE && !wait_terminate_post && (inTransparentMode(MODE_EXPRESSION | MODE_STATEMENT) || inMode(MODE_END_FOR_CONTROL))) {
            end_order.push_back(TERMINATE);

        }

        prev = LA(1);
        consume();

    }

    if(LA(1) == 1 /* EOF */) {

        end_order.clear();

    }

    if(LA(1) == ENDIF) end_order.resize(save_size);

    while(!op_stack.empty() && !end_order.empty()) {

        op_stack.pop_front();
        end_order.pop_front();

    }

    --inputState->guessing;

    rewind(start);

ENTRY_DEBUG } :;

// post processing for eol
eol_post[int directive_token, bool markblockzero] {

        // Flags to control skipping of #if 0 and #else.
        // Once in these modes, stay in these modes until the matching #endif is reached
        // cpp_ifcount used to indicate which #endif matches the #if or #else
        switch (directive_token) {

            case IF :
            case IFDEF :
            case IFNDEF :

                // should work unless also creates a dangling lcurly or lparen
                // in which case may need to run on everthing except else.
                if(isoption(parser_options, SRCML_OPTION_CPPIF_CHECK) && !inputState->guessing) {

                    std::list<int> end_order = cppif_end_count_check();
                    srcMLState::MODE_TYPE current_mode = getMode();

                    // @todo When C++11 is default, switch to ranged for or at least auto keyword.
                    for(std::list<int>::iterator pos = end_order.begin(); pos != end_order.end(); ++pos) {

                        if(*pos == RCURLY) {    
                            setMode(MODE_TOP | MODE_STATEMENT | MODE_NEST | MODE_LIST | MODE_BLOCK);
                            startNewMode(current_mode | MODE_ISSUE_EMPTY_AT_POP);
                            addElement(SBLOCK);

                        }

                        if(inTransparentMode(MODE_CONDITION) && *pos == RPAREN) {

                            std::stack<int> open_elements;
                            open_elements.push(SCONDITION);

                            /** @todo Could have multipl endings of a name as well.  However, just correct double ending of condition. */
                            if(number_finishing_elements)
                                finish_elements_add.push_back(std::pair<const srcMLState::MODE_TYPE, std::stack<int> >(MODE_CONDITION | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_ISSUE_EMPTY_AT_POP, open_elements));
                            else
                                insertModeAfter(MODE_CONDITION | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT,
                                                MODE_CONDITION | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_ISSUE_EMPTY_AT_POP,
                                                open_elements);


                        }

                        if(*pos == TERMINATE) {

                            if(inMode(MODE_CONDITION) && inPrevMode(MODE_IF)) {

                                cppif_duplicate = true;

                            } else if(inMode(MODE_LIST | MODE_CONDITION) && inPrevMode(MODE_STATEMENT | MODE_NEST)) {

                                cppif_duplicate = true;

                            } else if(inMode(MODE_END_FOR_CONTROL) || inMode(MODE_LIST | MODE_FOR_CONDITION)) {

                                cppif_duplicate = true;

                            } else {

                                dupDownOverMode(MODE_STATEMENT);

                            }

                        }

                    }


                }                      

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
                if (!isoption(parser_options, SRCML_OPTION_CPP_TEXT_ELSE) && !inputState->guessing)
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

                if (isoption(parser_options, SRCML_OPTION_CPP_TEXT_ELSE) && !inputState->guessing) {

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

                if (isoption(parser_options, SRCML_OPTION_CPP_TEXT_ELSE) && !inputState->guessing &&
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
        if ((!isoption(parser_options, SRCML_OPTION_CPP_MARKUP_IF0) && cpp_zeromode) ||
            (isoption(parser_options, SRCML_OPTION_CPP_TEXT_ELSE) && cpp_skipelse) ||
            (isoption(parser_options, SRCML_OPTION_CPP_TEXT_ELSE) && inputState->guessing && cpp_skipelse) ||
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
        for (std::deque<int>::size_type i = 0; i < cppmode.top().statesize.size(); ++i)
            if (cppmode.top().statesize[i] != cppmode.top().statesize[0]) {
                equal = false;
                break;
            }

        if (!cppmode.empty() && (equal || cppmode.top().statesize.size() == 2))
            cppmode.pop();

        ENTRY_DEBUG 
} :;

// line continuation character
line_continuation[] { ENTRY_DEBUG } :
        {
            // end all preprocessor modes
            endWhileMode(MODE_PARSE_EOL);
        }
        EOL_BACKSLASH
;

// condition in cpp
cpp_condition[bool& markblockzero] { CompleteElement element(this); ENTRY_DEBUG } :

        set_bool[markblockzero, LA(1) == CONSTANTS && LT(1)->getText() == "0"]

        cpp_complete_expression
;

// an expression
cpp_expression[CALL_TYPE type = NOCALL] { ENTRY_DEBUG } :

        { !inputState->guessing }?
        (expression_process
            expression_part_plus_linq[type]
        ) | cpp_garbage

;
exception
catch[...] {

        startNewMode(MODE_LOCAL);
        startElement(SNAME);
        consume();
        endMode();

}

// match a complete expression no stream
cpp_complete_expression[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }
        (options { greedy = true; } : { !cpp_check_end() }? 

        (   
        // commas as in a list
        { inTransparentMode(MODE_END_ONLY_AT_RPAREN) || !inTransparentMode(MODE_END_AT_COMMA)}?
        comma |

        // right parentheses, unless we are in a pair of parentheses in an expression
        { !inTransparentMode(MODE_INTERNAL_END_PAREN) }? rparen[false] |

        // argument mode (as part of call)
        { inMode(MODE_ARGUMENT) }? argument |

        // expression with right parentheses if a previous match is in one
        { LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN) }? cpp_expression |

        COLON
        )
        )*
;

// symbol in cpp
cpp_symbol[] { ENTRY_DEBUG } :
        (options { generateAmbigWarnings = false; } :
            
            simple_identifier | 

        {

            startNewMode(MODE_LOCAL);
            startElement(SNAME);

        }

        cpp_garbage

        {

        endMode();

        }
    )

;

cpp_define_name[] { CompleteElement element(this);
    int line_pos = LT(1)->getLine();
    std::string::size_type pos = LT(1)->getColumn() + LT(1)->getText().size();
} :

        {
            startNewMode(MODE_LOCAL);

            startElement(SCPP_MACRO_DEFN);
        }
        cpp_symbol (options { greedy = true; } : { line_pos == LT(1)->getLine() && pos == (unsigned)LT(1)->getColumn() }? cpp_define_parameter_list)*
;

cpp_define_parameter_list[] { CompleteElement element(this); bool lastwasparam = false; bool foundparam = false; ENTRY_DEBUG } :
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
        } comma |
        cpp_define_parameter { foundparam = lastwasparam = true; })* empty_element[SPARAMETER, !lastwasparam && foundparam] rparen[false]
;


cpp_define_parameter[] { int type_count = 1; ENTRY_DEBUG } :
        {
            // end parameter correctly
            startNewMode(MODE_PARAMETER);

            // start the parameter element
            startElement(SPARAMETER);
        }
        parameter_type_count[type_count]
;


cpp_define_value[] { ENTRY_DEBUG } :
        {
            startElement(SCPP_MACRO_VALUE);
        }
        cpp_garbage (options { greedy = true; } : cpp_garbage)*
;

// optional symbol cpp 
cpp_symbol_optional[] { ENTRY_DEBUG } :
        (options { greedy = true; } : cpp_symbol)*
;

// filename cpp
cpp_filename[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCPP_FILENAME);
        }
        (string_literal[false] | char_literal[false] | TEMPOPS (~(TEMPOPE | EOL))* TEMPOPE)
;

// linenumber in cpp
cpp_linenumber[] { SingleElement element(this); bool first = true; ENTRY_DEBUG } :
        (options { greedy = true; } : { if(first) { startElement(SCPP_NUMBER); first = false; } } literal[false])*
;

// literal in cpp
cpp_literal[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCPP_LITERAL);
        }
        (string_literal[false] | char_literal[false] | TEMPOPS (~(TEMPOPE | EOL))* TEMPOPE)
;

omp_directive[] { CompleteElement element(this); ENTRY_DEBUG} :
    {
        startNewMode(MODE_LOCAL);

        startElement(SOMP_DIRECTIVE);
    }

    OMP_OMP (options { generateAmbigWarnings = false; } : COMMA | { next_token() == LPAREN }? omp_clause | omp_name)*

;

omp_name[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOMP_NAME);
        }
        cpp_garbage

;


omp_clause[] { CompleteElement element(this); ENTRY_DEBUG} :
    {
        startNewMode(MODE_LOCAL);

        startElement(SOMP_CLAUSE);
    }

    omp_name omp_argument_list

;

omp_argument_list[] { CompleteElement element(this); ENTRY_DEBUG} :
    {
        startNewMode(MODE_LOCAL);

        startElement(SOMP_ARGUMENT_LIST);
    }

    (

    { next_token() != RPAREN }? LPAREN omp_argument (COMMA omp_argument)* RPAREN |
    LPAREN RPAREN

    )

;

omp_argument[] { CompleteElement element(this); ENTRY_DEBUG} :
    {
        startNewMode(MODE_LOCAL);

        startElement(SOMP_ARGUMENT);
        startElement(SOMP_EXPRESSION);
    }

    (~(RPAREN | COMMA))*

;
