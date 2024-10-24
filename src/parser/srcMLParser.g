// SPDX-License-Identifier: GPL-3.0-only
/*!
 * @file srcMLParser.g
 * 
 * @copyright Copyright (C) 2004-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML translator.
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
 * - The class StreamMLParser provides markup language stream processing.  These are
 * template classes which use this parser as a template parameter base.
 *
 * - The class TokenParser provides the virtual table for methods in StreamMLParser
 * that are called in this parser.
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

header "pre_include_cpp" {
#if defined(__GNUC__)
#endif
#ifdef __clang__
#endif
#if defined(__GNUC__) && !defined(__clang__)
#endif
#ifdef _MSC_VER
    #pragma warning(disable : 4365)  // 'argument': conversion from 'int' to 'unsigned int', signed/unsigned mismatch
    #pragma warning(disable : 4100)  // 'pe' unreferenced local variable
    #pragma warning(disable : 4101)  // 'pe' unreferenced local variable
#endif
}

// Included in the generated srcMLParser.hpp file after antlr includes
header "post_include_hpp" {
#include <string>
#include <string_view>
#include <deque>
#include <array>
#include <stack>
#include <Language.hpp>
#include <ModeStack.hpp>
#include <srcml_options.hpp>
#undef CONST
#undef VOID
#undef DELETE
#undef INTERFACE
#undef OUT
#undef IN
#undef THIS

using namespace ::std::literals::string_view_literals;

// Commented-out code
//#define DEBUG_PARSER

// Macros to introduce trace statements
#ifdef DEBUG_PARSER
class RuleTrace {
public:
    RuleTrace(int guessing, int token, int rd, std::string text, const char* fun, int line) :
        guessing(guessing), token(token), rd(rd), text(text), fun(fun), line(line)
    {
        fprintf(stderr, "TRACE: %d %d %d %5s%*s %s (%d)\n", guessing, token, rd, text.data(), rd, "", fun, line);
    }

    ~RuleTrace() {
        fprintf(stderr, "  END: %d %d %d %5s%*s %s (%d)\n", guessing, token, rd, text.data(), rd, "", fun, line);
    }

private:
    int guessing;
    int token;
    int rd;
    std::string text;
    const char* fun;
    int line;
};

// Macros to introduce RuleTrace statements
#define ENTRY_DEBUG RuleDepth rd(this); RuleTrace tr(inputState->guessing, LA(1), ruledepth, (LA(1) != EOL ? LT(1)->getText() : std::string("\\n")), __FUNCTION__, __LINE__);
#define ENTRY_DEBUG_START ruledepth = 0;
#else
#define ENTRY_DEBUG
#define ENTRY_DEBUG_START
#endif

#define CATCH_DEBUG // Commented-out code: marker();

#define assertMode(m)

enum STMT_TYPE {
    NONE, VARIABLE, FUNCTION, FUNCTION_DECL, CONSTRUCTOR, CONSTRUCTOR_DECL, DESTRUCTOR, DESTRUCTOR_DECL,
    SINGLE_MACRO, NULLOPERATOR, ENUM_DEFN, ENUM_DECL, GLOBAL_ATTRIBUTE, PROPERTY_ACCESSOR, PROPERTY_ACCESSOR_DECL,
    EXPRESSION, CLASS_DEFN, CLASS_DECL, UNION_DEFN, UNION_DECL, STRUCT_DEFN, STRUCT_DECL, INTERFACE_DEFN, INTERFACE_DECL, ACCESS_REGION,
    USING_STMT, OPERATOR_FUNCTION, OPERATOR_FUNCTION_DECL, EVENT_STMT, PROPERTY_STMT, ANNOTATION_DEFN, GLOBAL_TEMPLATE, DELEGATE_TYPE
};

enum CALL_TYPE {
    NOCALL, CALL, MACRO
};

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

// Ensures that a grammar rule forms a complete element.
// Ends all modes started in the rule and closes any elements started in the rule.
class CompleteElement {
public:
    CompleteElement(srcMLParser* parent) : parent(parent) {
        // only run if not guessing
        if (parent->inputState->guessing)
            return;

        ++parent->number_finishing_elements;

        start_size = parent->size();
    }

    ~CompleteElement() {
        // only run if not guessing
        if (parent->inputState->guessing)
            return;

        // close all the modes opened in the current rule with their elements
        int n = parent->size() - start_size;
        for (int i = 0; i < n; ++i) {
            parent->endMode();
        }

        --parent->number_finishing_elements;

        if (parent->number_finishing_elements == 0) {
            for (auto& citr : parent->finish_elements_add) {
                parent->startNewMode(citr.first);
                parent->currentState().openelements = citr.second;
            }

            parent->finish_elements_add.clear();
        }
    }

private:
    srcMLParser* parent;
    int start_size;
};

// Ensures that a grammar rule forms a complete element.
// Closes all elements opened in the rule for this current mode.
class LightweightElement {
public:
    LightweightElement(srcMLParser* parent) : parent(parent) {
        // only run if not guessing
        if (parent->inputState->guessing)
            return;

        start_size = parent->currentState().size();

        ++parent->number_finishing_elements;
    }

    ~LightweightElement() {
        // only run if not guessing
        if (parent->inputState->guessing)
            return;

        // close all elements opened by the rule in this mode
        while (start_size < parent->currentState().size())
            parent->endElement(parent->currentState().openelements.top());

        --parent->number_finishing_elements;

        if (parent->number_finishing_elements == 0) {
            for (auto& citr : parent->finish_elements_add) {
                parent->startNewMode(citr.first);
                parent->currentState().openelements = citr.second;
            }

            parent->finish_elements_add.clear();
        }
    }

private:
    srcMLParser* parent;
    int start_size;
};

// Ensures that a grammar rule forms a complete element.
// Ends the last opened tag.
class SingleElement {
public:
    SingleElement(srcMLParser* parent) : parent(parent) {}

    ~SingleElement() {
        // only run if not guessing
        if (parent->inputState->guessing)
            return;

        // end last opened element
        if (!parent->currentState().openelements.empty())
            parent->endElement(parent->currentState().openelements.top());
    }

private:
    srcMLParser* parent;
};

#ifdef ENTRY_DEBUG
class RuleDepth {
public:
    RuleDepth(srcMLParser* t) : pparser(t) {
        ++pparser->ruledepth;
    }

    ~RuleDepth() {
        --pparser->ruledepth;
    }

private:
    srcMLParser* pparser;
};
#endif

// constructor
srcMLParser::srcMLParser(antlr::TokenStream& lexer, int lang, const OPTION_TYPE& parser_options)
    : antlr::LLkParser(lexer,1), Language(lang), ModeStack(), parser_options(parser_options)
{
    // root, single mode that allows statements to be nested
    startNewMode(MODE_TOP | MODE_STATEMENT | MODE_NEST);
}

// ends all currently open modes
void srcMLParser::endAllModes() {
    // should only be one mode
    if (size() > 1 && isoption(parser_options, SRCML_OPTION_DEBUG))
        emptyElement(SERROR_MODE);

    if (isPaused()) {
        while (size() > 1)
            endMode();

        nopStreamStart();
    }

    resumeStream();

    // end all modes except the last
    while (size() > 1)
        endMode();

    // process any skip tokens that are not ended properly (e.g., "//" with EOF)
    completeSkip();

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

    // no output at all, only a placeholder
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
    SCONTENT;
    SINDEX;

    // statements
    SENUM;
    SENUM_DECLARATION;

    SIF_STATEMENT;
    SIF;
    STERNARY;
    STHEN;
    SELSE;
    SELSEIF;

    SWHILE_STATEMENT;
    SDO_STATEMENT;

    SFOR_STATEMENT;
    SFOREACH_STATEMENT;
    SCONTROL;
    SCONTROL_INITIALIZATION;
    SCONTROL_CONDITION;
    SCONTROL_INCREMENT;
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
    SDERIVATION;
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
    SPERMITS;
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
    SEMPTY;     // empty statement

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

    // other
    SCUDA_ARGUMENT_LIST;

    // OpenMP
    SOMP_DIRECTIVE;
    SOMP_NAME;
    SOMP_CLAUSE;
    SOMP_ARGUMENT_LIST;
    SOMP_ARGUMENT;
    SOMP_EXPRESSION;

    SCOMMENT;
    SLINECOMMENT;
    SLINE_DOXYGEN_COMMENT;
    SJAVADOC_COMMENT;
    SDOXYGEN_COMMENT;
}

/*
  Included inside of generated class srcMLParser.hpp
*/
{
public:
    friend class CompleteElement;
    friend class LightweightElement;
    friend class SingleElement;

    bool cpp_zeromode = false;
    bool cpp_skipelse = false;
    int cpp_ifcount = 0;
    bool isdestructor = false;
    const OPTION_TYPE parser_options = 0;
    std::array<std::string, 2> namestack;
    int ifcount = 0;
#ifdef ENTRY_DEBUG
    int ruledepth = 0;
#endif
    bool is_qmark = false;
    bool notdestructor = false;
    bool operatorname = false;
    std::stack<std::string> class_namestack;

    bool skip_ternary = false;

    int current_column = -1;
    int current_line = -1;
    int nxt_token = -1;
    int last_consumed = -1;
    bool wait_terminate_post = false;
    bool cppif_duplicate = false;
    size_t number_finishing_elements = 0;
    std::vector<std::pair<srcMLState::MODE_TYPE, std::stack<int>>> finish_elements_add;
    bool in_template_param = false;
    int start_count = 0;

    static const antlr::BitSet keyword_name_token_set;
    static const antlr::BitSet keyword_token_set;
    static const antlr::BitSet macro_call_token_set;
    static const antlr::BitSet argument_token_set;
    static const antlr::BitSet enum_preprocessing_token_set;
    static const antlr::BitSet literal_tokens_set;
    static const antlr::BitSet modifier_tokens_set;
    static const antlr::BitSet skip_tokens_set;
    static const antlr::BitSet class_tokens_set;
    static const antlr::BitSet decl_specifier_tokens_set;
    static const antlr::BitSet identifier_list_tokens_set;
    static const antlr::BitSet whitespace_token_set;

    // constructor
    srcMLParser(antlr::TokenStream& lexer, int lang, const OPTION_TYPE& options);

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
        if (!skip_tokens_set.member((unsigned int) LA(1)))
            last_consumed = LA(1);

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
start[] { ++start_count; ENTRY_DEBUG_START ENTRY_DEBUG } :
        // end of file
        eof |

        // end of line
        line_continuation | EOL | LINE_COMMENT_START | LINE_DOXYGEN_COMMENT_START |

        comma |

        { inLanguage(LANGUAGE_JAVA) }?
        bar |

        { inTransparentMode(MODE_OBJECTIVE_C_CALL) }?
        rbracket |

        { !inTransparentMode(MODE_INTERNAL_END_PAREN) || inPrevMode(MODE_CONDITION) }?
        rparen[false] |

        // characters with special actions that usually end currently open elements
        { !inTransparentMode(MODE_INTERNAL_END_CURLY) }?
        block_end |

        terminate |

        { inMode(MODE_ENUM) }?
        enum_block |

        // namespace block does not have block content element
        { inMode(MODE_NAMESPACE) }?
        lcurly[false] |

        // do not confuse with expression block
        {
            (
                (
                    inTransparentMode(MODE_CONDITION)
                    || (
                        !inMode(MODE_EXPRESSION)
                        && !inMode(MODE_EXPRESSION_BLOCK | MODE_EXPECT)
                    )
                )
                && !inTransparentMode(MODE_CALL | MODE_INTERNAL_END_PAREN)
                && (
                    !inLanguage(LANGUAGE_CXX)
                    || !inTransparentMode(MODE_INIT | MODE_EXPECT)
                )
            )
            || inTransparentMode(MODE_ANONYMOUS)
        }?
        lcurly |

        { inMode(MODE_ARGUMENT_LIST) }?
        call_argument_list |

        // switch cases @test switch
        {
            !inMode(MODE_INIT)
            && (
                !inMode(MODE_EXPRESSION)
                || inTransparentMode(MODE_DETECT_COLON)
            )
        }?
        colon |

        // process template operator correctly @test template
        { inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST) }?
        tempope |

        // Commented-out code
        // special default() call for C#
        // { LA(1) == DEFAULT && inLanguage(LANGUAGE_CSHARP) && inTransparentMode(MODE_EXPRESSION) && next_token() == LPAREN }?
        // expression_part_default |

        // statements that clearly start with a keyword
        {
            (
                LA(1) != TEMPLATE
                || next_token() != TEMPOPS
            )
            && inMode(MODE_NEST | MODE_STATEMENT)
            && !inMode(MODE_FUNCTION_TAIL)
            && (
                LA(1) != TEMPLATE
                || next_token() == TEMPOPS
            )
            && !(
                inLanguage(LANGUAGE_OBJECTIVE_C)
                && LA(1) == IMPORT
            )
            && !(
                LA(1) == ATPROTOCOL
                && next_token() == LPAREN
            )
            && (
                LA(1) != DEFAULT
                || next_token() == COLON
            )
            && (
                LA(1) != CHECKED
                || next_token() == LCURLY
            )
            && (
                LA(1) != UNCHECKED
                || next_token() == LCURLY
            )
            && (
                LA(1) != CXX_TRY
                || next_token() == LCURLY
            )
            && (
                LA(1) != INLINE
                || next_token() == NAMESPACE
            )
            && (
                LA(1) != STATIC
                || (
                    inLanguage(LANGUAGE_JAVA)
                    && next_token() == LCURLY
                )
            )
            && (
                LA(1) != CXX_CATCH
                || next_token() == LPAREN
                || next_token() == LCURLY
            )
            && (
                LA(1) != ASM
                || look_past_two(ASM, VOLATILE) == LPAREN
            )
            && (
                LA(1) != EMIT
                || emit_statement_check()
            )
        }?
        keyword_statements |

        { next_token() == LPAREN }?
        synchronized_statement |

        { inLanguage(LANGUAGE_CXX) && inMode(MODE_USING) }?
        using_aliasing |

        // statements identified by pattern (i.e., do not start with a keyword)
        { inMode(MODE_NEST | MODE_STATEMENT) && !inMode(MODE_FUNCTION_TAIL) }?
        pattern_statements |

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
  keyword_statements

  Statements that begin with a unique keyword.
*/
keyword_statements[] { ENTRY_DEBUG } :
        // conditional statements
        if_statement |

        { next_token() == IF }?
        elseif_statement |

        else_statement | switch_statement | switch_case | switch_default |

        // iterative statements
        while_statement | for_statement | do_statement | foreach_statement |

        // jump statements
        return_statement | break_statement | continue_statement | goto_statement |

        // template declarations - both functions and classes
        template_declaration |

        // exception statements
        { inLanguage(LANGUAGE_JAVA) && next_token() == LPAREN }?
        try_statement_with_resource |

        try_statement | catch_statement | finally_statement | throw_statement |

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
  pattern_statements

  Statements, declarations, and definitions that must be matched by pattern.

  Basically we have an identifier and we do not know yet whether it starts an expression,
  function definition, function declaration, or even a label.
*/
pattern_statements[] {
        int secondtoken = 0;
        int type_count = 0;
        int after_token = 0;
        bool isempty = false;
        int call_count = 1;
        STMT_TYPE stmt_type = NONE;
        CALL_TYPE type = NOCALL;

        // detect the declaration/definition type
        pattern_check(stmt_type, secondtoken, type_count, after_token);

        ENTRY_DEBUG
} :
        // variable declaration
        { stmt_type == VARIABLE }?
        variable_declaration_statement[type_count] |

        // check for Objective-C method
        { stmt_type == FUNCTION_DECL }?
        objective_c_method[SFUNCTION_DECLARATION] |

        { stmt_type == FUNCTION }?
        objective_c_method[SFUNCTION_DEFINITION] |

        // check for declaration of some kind (variable, function, constructor, destructor)
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

        // "~" which looked like destructor, but is not
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

        // delegate type
        { stmt_type == DELEGATE_TYPE }?
        delegate_type[type_count] |

        // call
        {
            isoption(parser_options, SRCML_PARSER_OPTION_CPP)
            && (
                inMode(MODE_ACCESS_REGION)
                || (
                    perform_call_check(type, isempty, call_count, secondtoken)
                    && type == MACRO
                )
            )
        }?
        macro_call |

        { inMode(MODE_ENUM) && inMode(MODE_LIST) }?
        enum_short_variable_declaration |

        { inLanguage(LANGUAGE_JAVA) }?
        annotation |

        expression_statement[type, call_count]
;

/*
  next_token

  An efficient way to view the token after the current LA(1).
*/
next_token[] returns [unsigned int token] {
        if (LT(1)->getColumn() == current_column && LT(1)->getLine() == current_line) {
            token = nxt_token;
        } else {
            current_column = LT(1)->getColumn();
            current_line = LT(1)->getLine();

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

/*
  next_token_two

  An efficient way to view the token after the current next_token.
*/
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

/*
  next_token_check

  Checks if the next token is one of the parameters.
*/
next_token_check[int token1, int token2] returns [bool result] {
        int token = next_token();

        result = token == token1 || token == token2;
} :;

/*
  look_past

  Skips past any skiptokens to get the one after.
*/
look_past[int skiptoken] returns [int token] {
        unsigned int place = mark();
        inputState->guessing++;

        while (LA(1) == skiptoken)
            consume();

        token = LA(1);

        inputState->guessing--;
        rewind(place);
} :;

/*
  look_past_two

  Skips past all of the skiptoken1 and skiptoken2 and returns the one after.
*/
look_past_two[int skiptoken1, int skiptoken2] returns [int token] {
        int place = mark();
        inputState->guessing++;

        while (LA(1) == skiptoken1 || LA(1) == skiptoken2)
            consume();

        token = LA(1);

        inputState->guessing--;
        rewind(place);
} :;

/*
  look_past_rule

  Gives the next token as if the rule was applied.  If the rule can not be applied, return -1.
*/
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

/*
  function_declaration

  The beginning function declaration/header.
*/
function_declaration[int type_count, int token = SFUNCTION_DECLARATION] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT);

            startElement(token);
        }

        function_header[type_count]
;

/*
  function_pointer_name_grammar

  A function pointer name handle.
*/
function_pointer_name_grammar[] { ENTRY_DEBUG } :
        LPAREN
        function_pointer_name_base
        RPAREN
;

/*
  function_pointer_name_base

  The inner portion of the functon pointer name.
*/
function_pointer_name_base[] { ENTRY_DEBUG bool flag = false; } :
        // special case for function pointer names that do not have '*'
        { !inMode(MODE_EXPRESSION) && macro_call_token_set.member(LA(1)) }?
        (compound_name_inner[false])* |

        // special name prefix of namespace or class
        identifier
        (generic_argument_list)*
        DCOLON
        function_pointer_name_base |

        // typical function pointer name
        multops
        (multops)*
        (compound_name_inner[false])*

        // optional array declaration
        (variable_identifier_array_grammar_sub[flag])*
;

/*
  decl_pre_type_annotation
*/
decl_pre_type_annotation[int& type_count] { ENTRY_DEBUG } :
        (
            // Commented-out code
            // special case for Java functions only
            // { inLanguage(LANGUAGE_JAVA) && LA(1) == FINAL }?
            // single_keyword_specifier |

            { inLanguage(LANGUAGE_JAVA) }?
            annotation |

            { inLanguage(LANGUAGE_CSHARP) }?
            attribute_csharp |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp
        )

        set_int[type_count, type_count - 1]
;

/*
  function_header

  The header of a function.
*/
function_header[int type_count] { ENTRY_DEBUG } :
        // no return value functions: casting operator method and main
        { type_count == 0 }?
        function_identifier
        {
            replaceMode(MODE_FUNCTION_NAME, MODE_FUNCTION_PARAMETER | MODE_FUNCTION_TAIL);
        } |

        (options { greedy = true; } :
            { next_token() == TEMPOPS }?
            template_declaration_full
            set_int[type_count, type_count - 1]
        )*

        (options { greedy = true; } :
            {
                type_count > 0
                && (
                    LA(1) != OVERRIDE
                    || !inLanguage(LANGUAGE_CXX)
                )
                && (
                    (
                        inLanguage(LANGUAGE_JAVA)
                        && (
                            LA(1) == ATSIGN
                            /* Commented-out code: || LA(1) == FINAL */
                        )
                    )
                    || (
                        inLanguage(LANGUAGE_CSHARP)
                        && LA(1) == LBRACKET
                    )
                    || (
                        inLanguage(LANGUAGE_CXX)
                        && LA(1) == LBRACKET
                        && next_token() == LBRACKET
                    )
                )
            }?
            decl_pre_type_annotation[type_count]
        )*

        function_type[type_count]
;

/*
  function_tail

  The portion of the function after the parameter list and before the block.
*/
function_tail[] { ENTRY_DEBUG } :
        (options { greedy = true; } :
            // order is important
            {
                inLanguage(LANGUAGE_CXX_FAMILY)
                && (
                    LA(1) != EQUAL
                    || (
                        inLanguage(LANGUAGE_CXX)
                        && (
                            next_token() == CONSTANTS
                            || next_token() == DEFAULT
                            || next_token() == DELETE
                        )
                    )
                )
            }?
            function_specifier |

            { inLanguage(LANGUAGE_CXX) }?
            ref_qualifier |

            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            (TRY | CXX_TRY) |

            { inLanguage(LANGUAGE_OO) }?
            complete_throw_list |

            { inLanguage(LANGUAGE_CXX) }?
            complete_noexcept_list |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp |

            { inLanguage(LANGUAGE_CXX) }?
            trailing_return |

            { inLanguage(LANGUAGE_JAVA) }?
            annotation_default |

            // K&R
            { inLanguage(LANGUAGE_C) }?
            // macros
            (
                (simple_identifier paren_pair) => macro_call |

                { look_past_two(NAME, VOID) == LCURLY }?
                simple_identifier |

                parameter
                (MULTOPS | simple_identifier | COMMA)*
                TERMINATE
            )
        )*
;

/*
  annotation_default

  Java annotation default values.
*/
annotation_default[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SDEFAULT);
        }

        DEFAULT
        annotation_default_initialization
;

/*
  annotation_default_initialization

  Java annotation default value initialization.
*/
annotation_default_initialization[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SDECLARATION_INITIALIZATION);
        }

        complete_expression
;

/*
  ref_qualifier

  Reference qualifiers in the function tail.
*/
ref_qualifier[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // markup type modifiers if option is on
            startElement(SREF_QUALIFIER);
        }

        (REFOPS | RVALUEREF)
;

/*
  trailing_return

  Trailing return in function tail.
*/
trailing_return[] { int type_count = 0; int secondtoken = 0; int after_token = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        TRETURN

        (
            {
                pattern_check(stmt_type, secondtoken, type_count, after_token, true)
                && (
                    stmt_type == FUNCTION
                    || stmt_type == FUNCTION_DECL
                )
            }?
            {
                startNewMode(MODE_TRAILING_RETURN);
            }
            function_declaration[type_count]
            function_identifier
            parameter_list |

            set_int[type_count, type_count + 1]
            parameter_type_count[type_count]
        )
;

/*
  function_rest

  Process the rest of the function and get to the end.
*/
function_rest[int& fla] { ENTRY_DEBUG } :
        eat_optional_macro_call

        parameter_list
        function_tail
        check_end[fla]
;

/*
  function_type

  The function type, including specifiers.
*/
function_type[int type_count] { bool is_compound = false; ENTRY_DEBUG } :
        {
            if (type_count == 0) {
                setMode(MODE_FUNCTION_NAME);
                return;
            }

            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_EAT_TYPE | MODE_FUNCTION_TYPE);

            // type element begins
            startElement(STYPE);
        }

        (options { greedy = true; } :
            { decl_specifier_tokens_set.member(LA(1)) }?
            (specifier | default_specifier | template_specifier)
            set_int[type_count, type_count - 1] |

            { inLanguage(LANGUAGE_JAVA) }?
            generic_parameter_list
            set_int[type_count, type_count - 1]
        )*

        {
            if (type_count == 0) {
                endMode(MODE_EAT_TYPE);
                setMode(MODE_FUNCTION_NAME);
                return;
            }

            setTypeCount(type_count);
        }

        (options { greedy = true; } :
            {
                inputState->guessing
                && (
                    LA(1) == TYPENAME
                    || LA(1) == CONST
                )
            }?
            (lead_type_identifier)
        )*

        // match auto keyword first as special case do no warn about ambiguity
        (options { generateAmbigWarnings = false; } :
            auto_keyword[type_count > 1] |

            { is_class_type_identifier() }?
            (options { greedy = true; } :
                { !class_tokens_set.member(LA(1)) }?
                (options { generateAmbigWarnings = false; } :
                    specifier |

                    { look_past_rule(&srcMLParser::identifier) != LPAREN }?
                    identifier |

                    macro_call
                )
                {
                    decTypeCount();
                }
            )*
            class_type_identifier[is_compound]
            {
                decTypeCount();
            }
            (options { greedy = true; } : { !is_compound }? multops)* |

            (options { greedy = true; } :
                { getTypeCount() > 2 }?
                pure_lead_type_identifier
                {
                    decTypeCount();
                }
            )*
            (lead_type_identifier | { inLanguage(LANGUAGE_JAVA) }? default_specifier)
        )

        {
            decTypeCount();

            if (inTransparentMode(MODE_ARGUMENT) && inLanguage(LANGUAGE_CXX))
                return;
        }

        (options { greedy = true; } :
            { getTypeCount() > 0 }?
            // Mark as name before mark without name
            (options { generateAmbigWarnings = false; } :
                keyword_name | type_identifier |

                { inLanguage(LANGUAGE_JAVA) }?
                default_specifier
            )
            {
                decTypeCount();
            }
        )*

        {
            endMode(MODE_EAT_TYPE);
            setMode(MODE_FUNCTION_NAME);
        }
;

/*
  function_type_check

  Checks the functon type.
*/
function_type_check[int& type_count] { type_count = 1; ENTRY_DEBUG } :
        lead_type_identifier
        ({ inLanguage(LANGUAGE_JAVA_FAMILY) || LA(1) != LBRACKET }? type_identifier_count[type_count])*
;

/*
  function_identifier

  Matches a function identifier.
*/
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
            function_pointer_name_grammar
            eat_optional_macro_call
        )
;

/*
  function_identifier_main

  Handles special cases for main.
*/
function_identifier_main[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        MAIN
;

/*
  overloaded_operator

  Handles an overloaded operator name.
*/
overloaded_operator[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SNAME);
        }

        set_bool[operatorname, true]
        OPERATOR

        (
            // special case for 'operator()'
            { LA(1) == LPAREN }?
            {
                startElement(SNAME);
            }
            LPAREN
            RPAREN |

            // for form operator type
            { LA(1) != DESTOP }?
            (compound_name) => compound_name |

            // general operator name case is anything else
            {
                startElement(SNAME);
            }

            (options { greedy = true; } : ~(LPAREN))*
        )
;

/*
  lambda_expression_csharp

  Handles a C# lambda expression.
*/
lambda_expression_csharp[] { bool first = true; ENTRY_DEBUG } :
        {
            startNewMode(MODE_FUNCTION_TAIL | MODE_ANONYMOUS | MODE_END_AT_COMMA);

            startElement(SFUNCTION_LAMBDA);
        }

        (options { greedy = true; } : specifier)*
        (parameter_list | lambda_single_parameter)
        lambda_csharp

        (options { greedy = true; } :
            { LA(1) != LCURLY && first }?
            complete_expression set_bool[first, false]
        )*
;

/*
  lambda_expression_cpp

  Handles a C++11 lambda expression.
*/
lambda_expression_cpp[] { ENTRY_DEBUG } :
        {
            bool iscall = look_past_rule(&srcMLParser::lambda_expression_full_cpp) == LPAREN;

            if (iscall) {
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

/*
  lambda_capture

  Handles the "[capture]" portion of a C++11 lambda function.
*/
lambda_capture[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LIST | MODE_LOCAL | MODE_ARGUMENT);

            startElement(SLAMBDA_CAPTURE);
        }

        (
            LBRACKET

            // suppress warning where "compound_name"s can match RBRACKET and is also matched by RBRACKET afterwards
            (options { warnWhenFollowAmbig = false; } :
                { /* warning suppression */ LA(1) == COMMA }?
                COMMA |

                { LA(1) != RBRACKET }?
                lambda_capture_argument
            )*

            RBRACKET
        )
;

/*
  lambda_capture_argument

  Handles the argument within the capture portion of a C++11 lambda.
*/
lambda_capture_argument[] { bool first = true; CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SARGUMENT);
        }

        // suppress greedy warnings
        (
            (options { greedy = true; } : lambda_capture_modifiers)*

            (options { greedy = true; } :
                { first }?
                variable_identifier
                (options { greedy = true; } : lambda_capture_initialization)*
                set_bool[first, false]
            )*
        )
;

/*
  lambda_capture_initialization
*/
lambda_capture_initialization[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL | MODE_END_AT_COMMA);

            startElement(SDECLARATION_INITIALIZATION);
        }

        // suppress warning of another case where REFOPS or something is in both alts.
        EQUAL
        complete_expression
;

/*
  lambda_expression_full_csharp

  Used to match a C# lambda expression completely.
*/
lambda_expression_full_csharp[] { ENTRY_DEBUG } :
        (options { greedy = true; } : ASYNC)*
        (variable_identifier | paren_pair)
        LAMBDA
;

/*
  lambda_expression_full_cpp

  Used to match a C++ lambda expression completely.
*/
lambda_expression_full_cpp[] { ENTRY_DEBUG } :
        // paren_pair and curly_pair seem to have nondeterminism because both can match LPAREN
        LBRACKET
        (~RBRACKET)*
        RBRACKET

        (options { warnWhenFollowAmbig = false; } : paren_pair)*
        function_tail
        curly_pair
;

/*
  lambda_capture_modifiers

  Handles modifiers that can occur within a lambda capture.
*/
lambda_capture_modifiers[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SMODIFIER);
        }

        (EQUAL | REFOPS)
;

/*
  block_lambda_expression

  Handles a block expression lambda.
*/
block_lambda_expression[] { ENTRY_DEBUG } :
        {
            bool iscall = look_past_rule(&srcMLParser::block_lambda_expression_full) == LPAREN;

            if (iscall) {
                // start a new mode that will end after the argument list
                startNewMode(MODE_ARGUMENT | MODE_LIST);

                // start the function call element
                startElement(SFUNCTION_CALL);
            }

            startNewMode(MODE_FUNCTION_PARAMETER | MODE_FUNCTION_TAIL | MODE_ANONYMOUS);

            startElement(SFUNCTION_LAMBDA);
        }

        BLOCKOP
        (options { greedy = true; } : type_identifier)*
        (options { greedy = true; } : parameter_list)*
;

/*
  block_lambda_expression_full

  Used to match a block expression lambda completely.
*/
block_lambda_expression_full[] { ENTRY_DEBUG } :
        BLOCKOP
        (options { greedy = true; } : type_identifier)*
        (options { greedy = true; } : paren_pair)*
        curly_pair
;

/*
  lambda_expression_java

  Handles a Java lambda expression.
*/
lambda_expression_java[] { bool first = true; ENTRY_DEBUG } :
        {
            startNewMode(MODE_FUNCTION_TAIL | MODE_ANONYMOUS);

            startElement(SFUNCTION_LAMBDA);
        }

        (parameter_list | lambda_single_parameter)
        lambda_java

        (options { greedy = true; } :
            { LA(1) != LCURLY && first }?
            complete_expression
            set_bool[first, false]
        )*
;

/*
  lambda_single_parameter
*/
lambda_single_parameter { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SPSEUDO_PARAMETER_LIST);

            startElement(SPARAMETER);

            startElement(SDECLARATION);
        }

        variable_identifier
;

/*
  lambda_java

  Handles a lambda character in Java.
*/
lambda_java[] { ENTRY_DEBUG } :
        TRETURN

        {
            if (LA(1) != LCURLY) {
                startNoSkipElement(SPSEUDO_BLOCK);
                startNoSkipElement(SCONTENT);
            }
        }
;

/*
  function_definition

  Handles the beginning of a function definition.
*/
function_definition[int type_count, int token = SFUNCTION_DEFINITION] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT);

            startElement(token);
        }

        function_header[type_count]
;

/*
  throw_list

  Handles a throw list for a function.
*/
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

/*
  noexcept_list

  Handles a noexcept list in a function.
*/
noexcept_list[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_EXPECT);

            startElement(SNOEXCEPT);
        }

        NOEXCEPT

        {
            if (LA(1) != LPAREN)
                endMode();
            else
                startElement(SARGUMENT_LIST);
        }

        (options { greedy = true; } : LPAREN)*
;

/*
  complete_throw_list

  Used to match a throw list completely.
*/
complete_throw_list[] { bool is_compound = false; ENTRY_DEBUG } :
        THROW
        paren_pair |

        THROWS
        (options { greedy = true; } : compound_name_java[is_compound] | COMMA)*
;

/*
  complete_noexcept_list

  Used to match a noexcept list completely.
*/
complete_noexcept_list[] { ENTRY_DEBUG } :
        NOEXCEPT
        (options { greedy = true; } : paren_pair)*
;

/* property methods */

/*
  property_method

  Handles a C# property method (GET/SET/ADD/REMOVE).
*/
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

/*
  property_method_name

  Handles the name portion of a property method.
*/
property_method_name[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        (GET | SET | ADD | REMOVE)
;

/*
  objective_c_method

  Handles Objective-C method declaration.
*/
objective_c_method[int token = SNOP] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT);

            startElement(token);
        }

        objective_c_method_specifier
        (options { greedy = true; } : objective_c_method_type)*
        /* Commented-out code: objective_c_selector */
        objective_c_parameter_list
;

/*
  objective_c_method_specifier
*/
objective_c_method_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        (CSPEC | MSPEC)
;

/*
  objective_c_method_type

  Handles either an Objective-C method return type or a parameter type.
*/
objective_c_method_type[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_LOCAL);

            // type element begins
            startElement(STYPE);
        }

        LPAREN

        (options { greedy = true; } :
            {
                inputState->guessing
                && (
                    LA(1) == TYPENAME
                    || LA(1) == CONST
                )
            }?
            (lead_type_identifier)
        )*

        // match auto keyword first as a special case; do not warn about ambiguity
        (options { generateAmbigWarnings = false; } : auto_keyword[true] | lead_type_identifier)

        (options { greedy = true; } :
            { LA(1) != RPAREN }?
            // Mark as name before mark but without name
            (options { generateAmbigWarnings = false; } : keyword_name | type_identifier)
        )*

        RPAREN
;

/*
  objective_c_parameter_list
*/
objective_c_parameter_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_FUNCTION_PARAMETER);

            // start the function call element
            startElement(SPARAMETER_LIST);
        }

        objective_c_parameter
        (objective_c_parameter)*
;

/*
  objective_c_parameter

  Handles a method parameter "name:value" pair for Objective-C.
*/
objective_c_parameter[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            if (inTransparentMode(MODE_LIST))
                endDownToMode(MODE_LIST);

            startNewMode(MODE_PARAMETER);

            startElement(SPARAMETER);
        }

        objective_c_selector

        (options { greedy = true; } :
            objective_c_method_type

            // Mark as name before mark but without name
            (options { generateAmbigWarnings = false; } : compound_name | keyword_name)
        )*
;

/*
  property_declaration

  Handles an Objective-C property declaration.
*/
property_declaration[] { int type_count = 0; int secondtoken = 0; int after_token = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT);

            startElement(SPROPERTY);
        }

        PROPERTY

        (property_attribute_list)*

        { pattern_check(stmt_type, secondtoken, type_count, after_token) }?
        variable_declaration[type_count]
;

/*
  property_attribute_list
*/
property_attribute_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SATTRIBUTE_LIST);
        }

        LPAREN
        (property_attribute | COMMA)*
        RPAREN
;

/*
  property_attribute
*/
property_attribute[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SATTRIBUTE);
        }

        identifier
        (property_attribute_initialization)*
;

/*
  property_attribute_initialization
*/
property_attribute_initialization[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            // Commented-out code
            // startElement(SDECLARATION_INITIALIZATION);
        }

        EQUAL
        identifier
;

/*
  synthesize_statement
*/
synthesize_statement[] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT);

            startElement(SSYNTHESIZE);
        }

        SYNTHESIZE
        property_implementation_inner
;

/*
  dynamic_statement
*/
dynamic_statement[] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT);

            startElement(SDYNAMIC);
        }

        DYNAMIC
        property_implementation_inner
;

/*
  property_implementation_inner
*/
property_implementation_inner[] { ENTRY_DEBUG } :
        property_implementation_name
        (COMMA property_implementation_name)*
;

/*
  property_implementation_name
*/
property_implementation_name[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SDECLARATION);
        }

        identifier
        (property_implementation_initialization)*
;

/*
  property_implementation_initialization
*/
property_implementation_initialization[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            // Commented-out code
            // startElement(SDECLARATION_INITIALIZATION);
        }

        EQUAL
        identifier
;

/*
  perform_call_check

  Checks to see if this is a call and what type it is.
*/
perform_call_check[CALL_TYPE& type, bool& isempty, int& call_count, int secondtoken] returns [bool iscall] {
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

            // call syntax succeeded, however post-call token is not legitimate
            if (
                isoption(parser_options, SRCML_PARSER_OPTION_CPP)
                && (
                    (
                        (
                            !inLanguage(LANGUAGE_OBJECTIVE_C)
                            || !inTransparentMode(MODE_OBJECTIVE_C_CALL)
                        )
                        && (
                            keyword_token_set.member(postcalltoken)
                            || postcalltoken == NAME
                            || postcalltoken == VOID
                        )
                    )
                    || (
                        !inLanguage(LANGUAGE_CSHARP)
                        && postcalltoken == LCURLY
                    )
                    || postcalltoken == EXTERN
                    || postcalltoken == STRUCT
                    || postcalltoken == UNION
                    || postcalltoken == CLASS
                    || postcalltoken == CXX_CLASS
                    || (
                        !inLanguage(LANGUAGE_CSHARP)
                        && postcalltoken == RCURLY
                    )
                    || (
                        postnametoken != 1
                        && postcalltoken == 1 /* EOF ? */
                    )
                    || postcalltoken == TEMPLATE
                    || postcalltoken == INLINE
                    || postcalltoken == PUBLIC
                    || postcalltoken == PRIVATE
                    || postcalltoken == PROTECTED
                    || postcalltoken == SIGNAL
                    || postcalltoken == ATREQUIRED
                    || postcalltoken == ATOPTIONAL
                    || postcalltoken == STATIC
                    || postcalltoken == CONST
                )
                && (save_first != DECLTYPE)
            )
                type = MACRO;

            if (inLanguage(LANGUAGE_CSHARP) && (postcalltoken == LAMBDA || postcalltoken == EQUAL))
                type = NOCALL;

        } catch (...) {
            type = NOCALL;

            if (isoption(parser_options, SRCML_PARSER_OPTION_CPP) && argumenttoken != 0 && postcalltoken == 0)
                type = MACRO;

            // single macro call followed by statement_cfg
            else if (
                isoption(parser_options, SRCML_PARSER_OPTION_CPP)
                && secondtoken != -1
                && (
                    keyword_token_set.member(secondtoken)
                    || secondtoken == LCURLY
                    || secondtoken == 1 /* EOF */
                    || secondtoken == PUBLIC
                    || secondtoken == PRIVATE
                    || secondtoken == PROTECTED
                )
            )
                type = MACRO;
        }

        if (type == CALL && postnametoken == 1)
            type = NOCALL;

        inputState->guessing--;
        rewind(start);

        ENTRY_DEBUG
} :;

/*
  call_check

  Used to check if a call is a call.
*/
call_check[int& postnametoken, int& argumenttoken, int& postcalltoken, bool& isempty, int& call_count] { ENTRY_DEBUG } :
        // detect name, which may be the name of a macro or even an expression
        (
            function_identifier |

            keyword_call_tokens
            (DOTDOTDOT | generic_argument_list | cuda_argument_list)* |

            { inLanguage(LANGUAGE_OBJECTIVE_C) }?
            bracket_pair
        )

        // record token after the function identifier for future use if this fails
        markend[postnametoken]

        set_bool[
            isempty,
            (
                LA(1) == LPAREN
                && next_token() == RPAREN
            )
            || (
                inLanguage(LANGUAGE_CXX)
                && LA(1) == LCURLY
                && next_token() == RCURLY
            )
        ]

        (
            { isoption(parser_options, SRCML_PARSER_OPTION_CPP) }?
            // check for proper form of argument list
            (call_check_paren_pair[argumenttoken] set_int[call_count, call_count + 1])*
            // record token after argument list to differentiate between call and macro
            markend[postcalltoken] |

            LPAREN
            set_int[call_count, 1]
        )
;

/*
  call_check_paren_pair

  Used to check the contents of a call.
*/
call_check_paren_pair[int& argumenttoken, int depth = 0] { int call_token = LA(1); bool name = false; ENTRY_DEBUG } :
        (LPAREN | { inLanguage(LANGUAGE_CXX) }? LCURLY)

        // record token after the start of the argument list
        markend[argumenttoken]

        (options { greedy = true; generateAmbigWarnings = false; } :
            // recursive nested parentheses
            call_check_paren_pair[argumenttoken, depth + 1]
            set_bool[name, false] |

            // special case for something that looks like a declaration
            { !name || (depth > 0) }?
            (identifier | generic_selection)
            set_bool[name, true] |

            keyword_call_tokens
            (options { greedy = true; } :
                DOTDOTDOT |
                generic_argument_list |
                cuda_argument_list
            )* |

            // special case for something that looks like a declaration
            { LA(1) == DELEGATE /* eliminates ANTRL warning, will be nop */ }?
            delegate_anonymous |

            { next_token_check(LCURLY, LPAREN) }?
            lambda_anonymous |

            (LBRACKET (~RBRACKET)* RBRACKET (LPAREN | LCURLY)) => lambda_expression_full_cpp |

            (block_lambda_expression_full) => block_lambda_expression_full |

            { inLanguage(LANGUAGE_OBJECTIVE_C) }?
            bracket_pair |

            // found two names in a row, so this is not an expression; cause this to fail by explicitly throwing an exception
            { depth == 0 }?
            (identifier | generic_selection)
            throw_exception[true] |

            // forbid parentheses (handled recursively) and cfg tokens
            { call_token == LPAREN && !keyword_token_set.member(LA(1)) }?
            ~(LPAREN | RPAREN | TERMINATE)
            set_bool[name, false] |

            { call_token == LCURLY && inLanguage(LANGUAGE_CXX) && !keyword_token_set.member(LA(1)) }?
            ~(LCURLY | RCURLY | TERMINATE)
            set_bool[name, false]
        )*

        (
            { call_token == LPAREN }?
            RPAREN |

            { call_token == LCURLY && inLanguage(LANGUAGE_CXX) }?
            RCURLY
        )
;

/*
  perform_ternary_check
*/
perform_ternary_check[] returns [bool is_ternary] {
        is_ternary = false;

        int start = mark();
        inputState->guessing++;

        try {
            ternary_check();
            if (LA(1) == QMARK)
                is_ternary = true;
        } catch(...) {}

        if (!is_qmark && (LA(1) == TERMINATE || LA(1) == LCURLY))
            skip_ternary = true;

        inputState->guessing--;
        rewind(start);

        ENTRY_DEBUG
} :;

/*
  ternary_check
*/
ternary_check[] { ENTRY_DEBUG } :
        // ends are catch alls; it is ok if they overlap
        (
            { LA(1) != 1 }?
            (options { generateAmbigWarnings = false; } :
                { identifier_list_tokens_set.member(LA(1)) }?
                compound_name_inner[false] |

                paren_pair |

                bracket_pair
                (options { greedy = true; } : paren_pair | curly_pair)* |

                ~(QMARK | TERMINATE | LCURLY | COLON | RPAREN | COMMA | RBRACKET | RCURLY | EQUAL | ASSIGNMENT)
            )
        )

        // ends are catch alls; it is ok if they overlap
        (
            { LA(1) != 1 }?
            (options { generateAmbigWarnings = false; } :
                { identifier_list_tokens_set.member(LA(1)) }?
                compound_name_inner[false] |

                paren_pair |

                bracket_pair
                (options { greedy = true; } : paren_pair | curly_pair)* |

                ~(QMARK | TERMINATE | LCURLY | COLON | RPAREN | COMMA | RBRACKET | RCURLY | EQUAL | ASSIGNMENT)
            )
        )*
;

/*
  markend

  Records the current token, even in guessing mode.
*/
markend[int& token] {
        token = LA(1);
} :;

/* Keyword Statements */

/*
  while_statement

  Handles a while statement.
*/
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

/*
  forever_statement

  Handles a Qt forever statement.
*/
forever_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement (after condition)
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the forever element
            startElement(SFOREVER_STATEMENT);

            // Commented-out code
            // expect a condition to follow the keyword
            // startNewMode(MODE_CONDITION | MODE_EXPECT);
        }

        FOREVER
;

/*
  do_statement

  Handles a do-while statement.
*/
do_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement (after condition)
            // set to top mode so that end of block will not end the statement itself
            startNewMode(MODE_STATEMENT | MODE_TOP | MODE_DO_STATEMENT);

            // start of do statement
            startElement(SDO_STATEMENT);

            // mode to nest while part of do while statement
            startNewMode(MODE_STATEMENT | MODE_NEST);
        }

        DO

        {
            if (LA(1) != LCURLY) {
                startNoSkipElement(SPSEUDO_BLOCK);

                startNoSkipElement(SCONTENT);
            }
        }
;

/*
  do_while

  Handles the "while" part of a do-while statement.
*/
do_while[] { ENTRY_DEBUG } :
        {
            // mode for do statement is in top mode so that end of the block will not end the statement
            clearMode(MODE_TOP);

            // expect a condition to follow
            startNewMode(MODE_CONDITION | MODE_EXPECT);
        }

        WHILE
;

/*
  for_statement

  Handles the start of a for statement.
*/
for_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement after the control group
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the for statement
            startElement(SFOR_STATEMENT);
        }

        FOR

        {
            // statement with nested statement after the control group
            startNewMode(MODE_EXPECT | MODE_CONTROL);
        }
;

/*
  foreach_statement

  Handles the start of a foreach statement. (C#/Qt)
*/
foreach_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement after the control group
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start the foreach statement
            startElement(SFOREACH_STATEMENT);
        }

        FOREACH

        {
            // statement with nested statement after the control group
            if (inLanguage(LANGUAGE_CSHARP))
                startNewMode(MODE_EXPECT | MODE_CONTROL);
            else
                startNewMode(MODE_EXPECT | MODE_CONTROL | MODE_END_AT_COMMA);
        }
;

/*
  control_group

  Handles the start of a control group. (i.e., initialization, test, increment)
*/
control_group[] { ENTRY_DEBUG } :
        {
            // start the control group mode that will end at the next matching parentheses
            replaceMode(MODE_CONTROL, MODE_TOP | MODE_CONTROL_INITIALIZATION | MODE_IGNORE_TERMINATE | MODE_INTERNAL_END_PAREN | MODE_LIST);

            // start the for heading group element
            if (inPrevMode(MODE_IF)) {
                startElement(SCONDITION);
            } else {
                startElement(SCONTROL);
            }
        }

        LPAREN
;

/*
  is_control_terminate
*/
is_control_terminate[] returns [bool is_terminate = false] {
        int state = mark();
        ++inputState->guessing;

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

            if (parencount < 0 || bracecount < 0) {
                break;
            }

            if (LA(1) == TERMINATE && parencount == 0 && bracecount == 0) {
                is_terminate = true;
                break;
            }

            consume();
        }

        --inputState->guessing;
        rewind(state);
} :;

/*
  control_initialization_pre
*/
control_initialization_pre[] { ENTRY_DEBUG } :
        {
            // check for ";" in if control
            if (inMode(MODE_CONTROL_INITIALIZATION) && inPrevMode(MODE_IF) && !is_control_terminate()) {
                replaceMode(MODE_CONTROL_INITIALIZATION, MODE_CONTROL_CONDITION);
            }
        }

        (
            // inside of control group expecting initialization
            { inMode(MODE_CONTROL_INITIALIZATION | MODE_EXPECT) }?
            control_initialization |

            // inside of control group expecting condition
            { inMode(MODE_CONTROL_CONDITION | MODE_EXPECT) }?
            control_condition
        )
;

/*
  control_initialization_action

  Handles parameter list initialization.  Used in multiple places.
*/
control_initialization_action[] { ENTRY_DEBUG } :
        {
            assertMode(MODE_CONTROL_INITIALIZATION | MODE_EXPECT);

            // setup next stage for condition in the control group mode
            replaceMode(MODE_CONTROL_INITIALIZATION, MODE_CONTROL_CONDITION);

            bool in_if_mode = inPrevMode(MODE_IF);

            // setup a mode for initialization that will end with a ";"
            startNewMode(MODE_EXPRESSION | MODE_EXPECT | MODE_STATEMENT | MODE_LIST);

            if (!in_if_mode) {
                startElement(SCONTROL_INITIALIZATION);
            } else {
                startElement(SDECLARATION_STATEMENT);
            }
        }
;

/*
  control_initialization

  Handles the initilization portion of a "for" initialization.
*/
control_initialization[] { int type_count = 0; int secondtoken = 0; int after_token = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        control_initialization_action

        (
            // explicitly check for a variable declaration since it can easily be confused with an expression
            { pattern_check(stmt_type, secondtoken, type_count, after_token) && stmt_type == VARIABLE }?
            control_initialization_variable_declaration[type_count] |

            { if (secondtoken == COLON)
                setMode(MODE_RANGED_FOR);
            }

            expression
        )
;

/*
  control_initialization_variable_declaration

  Handles a statement for the declaration of a variable (or a group of variables) in a "for" initialization.
*/
control_initialization_variable_declaration[int type_count] { ENTRY_DEBUG } :
        {
            // start a new mode for the expression which will end inside of the terminate
            startNewMode(MODE_LIST);
        }

        variable_declaration[type_count]
;

/*
  control_condition_action

  Handles "for" parameter list condition setup.  Used in multiple places.
*/
control_condition_action[] { ENTRY_DEBUG } :
        {
            assertMode(MODE_CONTROL_CONDITION | MODE_EXPECT);

            bool in_if_mode = inPrevMode(MODE_IF);

            // setup next stage control condition
            if (in_if_mode) {
                // switch back to if-processing
                replaceMode(MODE_TOP | MODE_CONTROL_CONDITION | MODE_IGNORE_TERMINATE | MODE_INTERNAL_END_PAREN, MODE_LIST | MODE_EXPRESSION);
            } else {
                replaceMode(MODE_CONTROL_CONDITION, MODE_CONTROL_INCREMENT | MODE_INTERNAL_END_PAREN | MODE_LIST);

                // setup a mode for initialization that will end with a ";"
                startNewMode(MODE_EXPRESSION | MODE_EXPECT | MODE_LIST | MODE_STATEMENT);

                startElement(SCONTROL_CONDITION);
            }
        }
;

/*
  control_condition

  Handles a "for" condition.
*/
control_condition[] { bool in_if_condition = inPrevMode(MODE_IF); ENTRY_DEBUG } :
        control_condition_action

        (
            // non-empty condition
            { in_if_condition }?
            condition_inner |

            { LA(1) != RPAREN }?
            expression |

            rparen[false]
        )
;

/*
  control_increment

  Handles an increment in a "for" parameter list.
*/
control_increment[] { ENTRY_DEBUG } :
        {
            assertMode(MODE_EXPECT | MODE_CONTROL_INCREMENT);

            clearMode(MODE_EXPECT | MODE_CONTROL_INCREMENT);

            // setup a mode for initialization that will end with a ";"
            startNewMode(MODE_CONTROL_INCREMENT | MODE_EXPRESSION | MODE_EXPECT | MODE_STATEMENT | MODE_LIST);

            if (LA(1) == RPAREN)
                // empty increment issued as single element
                emptyElement(SCONTROL_INCREMENT);
            else
                startElement(SCONTROL_INCREMENT);
        }

        expression
;

/*
  if_statement

  An "if" statement is first processed here.  Then prepare for a condition.  The end of the
  condition will set up for the "then" part of the statement.  The end of the "then" looks
  ahead for an "else".  If so, it ends the "then" part.  If not, it ends the entire statement.
*/
if_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement; detection of else
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_IF | MODE_IF_STATEMENT);

            // start if sequence container
            startElement(SIF_STATEMENT);

            ++ifcount;

            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_IF | MODE_ELSE);

            // start the if statement
            startElement(SIF);

            // expect a condition; start THEN after condition
            startNewMode(MODE_EXPECT | MODE_CONTROL | MODE_CONDITION);
        }

        IF

        {
            if (LA(1) == CONSTEXPR) {
                constexpr_specifier();
            }
        }
;

/*
  else_statement

  Handles the "else" part of an "if" statement. The "else" part is detected on its own, and as
  part of termination. (semicolon or end of a block)
*/
else_statement[] { ENTRY_DEBUG } :
        {
            // catchup for isolated else
            if (!inMode(MODE_IF_STATEMENT)) {
                // statement with nested statement; detection of else
                startNewMode(MODE_STATEMENT | MODE_NEST | MODE_IF | MODE_IF_STATEMENT);

                // start if sequence container
                startElement(SIF_STATEMENT);

                ++ifcount;
            }

            // treat as a statement with a nested statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_ELSE);

            // start the else part of the if statement
            startElement(SELSE);
        }

        ELSE

        {
            if (LA(1) != LCURLY) {
                startNoSkipElement(SPSEUDO_BLOCK);
                startNoSkipElement(SCONTENT);
            }
        }
;

/*
  elseif_statement

  Handles the "else if" construct. The "else" and "if" parts are detected on their own, and as
  part of termination. (semicolon or end of a block)
*/
elseif_statement[] { ENTRY_DEBUG } :
        {
            // catchup for isolated else
            if (!inMode(MODE_IF_STATEMENT)) {

                // statement with nested statement; detection of else
                startNewMode(MODE_STATEMENT | MODE_NEST | MODE_IF | MODE_IF_STATEMENT);

                // start if sequence container
                startElement(SIF_STATEMENT);

                ++ifcount;
            }

            // treat as a statement with a nested statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_IF | MODE_ELSE);

            // start the else part of the if statement
            startElement(SELSEIF);
        }

        ELSE

        {
            // Commented-out code
            // start the if statement
            // startElement(SIF_STATEMENT);

            // expect a condition; start THEN after condition
            startNewMode(MODE_EXPECT | MODE_CONTROL | MODE_CONDITION);
        }

        IF

        {
            if (LA(1) == CONSTEXPR) {
                constexpr_specifier();
            }
        }
;

/*
  switch_statement

  Handles the start of a "switch" statement.
*/
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

/*
  section_entry_action

  Handles the actions to perform before first starting a section.  Used in multiple places.
*/
section_entry_action_first[] :
        {
            // start a new section inside the block with nested statements
            startNewMode(MODE_TOP_SECTION | MODE_TOP | MODE_STATEMENT | MODE_NEST);
        }
;

/*
  section_entry_action

  Handles the actions to perform before starting a section.
*/
section_entry_action[] :
        {
            // end any statements inside the section
            endDownToModeSet(MODE_TOP);

            // flush any whitespace tokens since sections should end at the last possible place
            flushSkip();

            // end the section inside the block
            endWhileMode(MODE_TOP_SECTION);
        }

        section_entry_action_first
;

/*
  switch_case

  Note that "case" is treated as a statement.
*/
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

/*
  switch_default

  Note that "default" is treated as a statement.
*/
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

/*
  import_statement

  Handles an "import" statement.
*/
import_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME | MODE_EXPECT);

            // start the import statement
            startElement(SIMPORT);
        }

        IMPORT
;

/*
  multop_name

  Handles the cases where "*" is used as a name.
*/
multop_name[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        MULTOPS
;

/*
  package_statement

  Handles a "package" statement.
*/
package_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME | MODE_EXPECT);

            // start the package statement
            startElement(SPACKAGE);
        }

        PACKAGE
;

/*
  assert_statement

  Handles an "assert" statement.
*/
assert_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the assert statement
            startElement(SASSERT_STATEMENT);
        }

        ASSERT
;

/*
  static_block
*/
static_block { ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT | MODE_NEST);

            startElement(SSTATIC_BLOCK);
        }

        STATIC
        lcurly
;

/*
  static_assert_statement

  Handles a "_Static_assert" statement. (C)
*/
static_assert_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the static assert statement
            startElement(SSTATIC_ASSERT_STATEMENT);

            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_ARGUMENT_LIST);
        }

        STATIC_ASSERT
        call_argument_list
;

/*
  return_statement

  Handles a "return" statement.
*/
return_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the return statement
            startElement(SRETURN_STATEMENT);
        }

        RETURN
;

/*
  yield_statements

  Handles "yield" statements.
*/
yield_statements[] { int t = next_token(); ENTRY_DEBUG } :
        { t == RETURN }?
        yield_return_statement |

        { t == BREAK }?
        yield_break_statement
;

/*
  yield_specifier

  Used to match a special "yield" specifier.
*/
yield_specifier[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        YIELD
;

/*
  yield_return_statement

  Used to match a special "yield" specifier followed by a "return" keyword.
*/
yield_return_statement[] { ENTRY_DEBUG } :
        {
            // statement with a possible expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the return statement
            startElement(SRETURN_STATEMENT);
        }

        yield_specifier
        RETURN
;

/*
  yield_break_statement

  Used to match a special "yield" specifier followed by a "break" keyword.
*/
yield_break_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the break statement
            startElement(SBREAK_STATEMENT);
        }

        yield_specifier
        BREAK
;

/*
  break_statement

  Used to handle a "break" statement.
*/
break_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME);

            // start the break statement
            startElement(SBREAK_STATEMENT);
        }

        BREAK
;

/*
  continue_statement

  Handles the start of a "continue" statement.
*/
continue_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME);

            // start the continue statement
            startElement(SCONTINUE_STATEMENT);
        }

        CONTINUE
;

/*
  goto_statement

  Handles the start of a "goto" statement.
*/
goto_statement[] { ENTRY_DEBUG } :
        {
            // statement with an expected label name; the label name is a subset of variable names
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME);

            // start the goto statement
            startElement(SGOTO_STATEMENT);
        }

        GOTO
;

/*
  goto_case
*/
goto_case[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        CASE
        (literals | ~TERMINATE)*
;

/*
  asm_declaration

  Used to complete an assembly declaration statement.
*/
asm_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the asm statement
            startElement(SASM);
        }

        ASM

        (options { greedy = true; } :
            { LA(1) == VOLATILE }?
            specifier
        )*

        ({ true }? paren_pair | ~(LCURLY | RCURLY | TERMINATE))*
;

/*
  visual_cxx_asm_declaration

  Used to complete an assembly declaration statement.
*/
visual_cxx_asm_declaration[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the asm statement
            startElement(SASM);
        }

        VISUAL_CXX_ASM

        (
            { LA(1) == LCURLY }?
            visual_cxx_asm_declaration_curly_pair |
            
            (options { greedy = true; } : visual_cxx_asm_inner)*
        )

        (options { greedy = true; } : TERMINATE)*
;

/*
  visual_cxx_asm_declaration_curly_pair
*/
visual_cxx_asm_declaration_curly_pair[] { ENTRY_DEBUG } :
        LCURLY

        (options { generateAmbigWarnings = false; } :
            visual_cxx_asm_declaration |

            visual_cxx_block_inner |

            ~(RCURLY)
        )*

        RCURLY
;

/*
  visual_cxx_block_inner
*/
visual_cxx_block_inner[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the asm statement
            startElement(SASM);
        }

        (
            { LA(1) == LCURLY }?
            visual_cxx_asm_declaration_curly_pair |
            
            (
                visual_cxx_asm_inner
                (options { greedy = true; } : visual_cxx_asm_inner)*
            )
        )

        (options { greedy = true; } : TERMINATE)*
;

/*
  visual_cxx_asm_inner
*/
visual_cxx_asm_inner[] { ENTRY_DEBUG } :
        (~(EOL | TERMINATE | RCURLY | VISUAL_CXX_ASM))
;

/*
  extern_alias
*/
extern_alias[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        ALIAS
;

/*
  extern_definition

  Handles an "extern" definition.
*/
extern_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_EXTERN);

            // start the extern definition
            startElement(SEXTERN);
        }

        EXTERN

        (
            extern_alias
            (options { greedy = true; } : variable_identifier)*
        )*
;

/*
  extern_name

  Handles the name of an "extern" section.
*/
extern_name[] { ENTRY_DEBUG } :
        string_literal

        {
            // nest a block inside the namespace
            setMode(MODE_STATEMENT | MODE_NEST);
        }
;

/*
  namespace_inline_specifier
*/
namespace_inline_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        INLINE
;

/*
  namespace_definition

  Handles namespaces.
*/
namespace_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NAMESPACE | MODE_VARIABLE_NAME);

            // start the namespace definition
            startElement(SNAMESPACE);
        }

        (namespace_inline_specifier)*
        NAMESPACE
;

/*
  namespace_alias

  Handles a namespace alias.
*/
namespace_alias[] { ENTRY_DEBUG } :
        EQUAL

        {
            // expect a label name; a label name is a subset of variable names
            setMode(MODE_VARIABLE_NAME);
        }
;

/*
  namespace_block

  Handles a namespace block.
*/
namespace_block[] { ENTRY_DEBUG } :
        {
            // nest a block inside the namespace
            setMode(MODE_STATEMENT | MODE_NEST);
        }

        lcurly[false]
;

/*
  namespace_directive

  Handles a namespace "using" directive.
*/
namespace_directive[] { ENTRY_DEBUG } :
        {
            // statement with an expected namespace name after the keywords
            startNewMode(MODE_STATEMENT | MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT | MODE_USING);

            // start the using directive
            startElement(SUSING_DIRECTIVE);
        }

        (options { greedy = true; } :
            { next_token() == TEMPOPS }?
            template_declaration_full
        )*

        USING
;

/*
  using_aliasing
*/
using_aliasing[] { int type_count = 0; int secondtoken = 0; int after_token = 0; STMT_TYPE stmt_type = NONE; ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the initialization element
            startElement(SDECLARATION_INITIALIZATION);
        }

        EQUAL

        (
            {
                pattern_check(stmt_type, secondtoken, type_count, after_token)
                && (
                    stmt_type == FUNCTION_DECL
                    || stmt_type == FUNCTION
                    || stmt_type == OPERATOR_FUNCTION_DECL
                    || stmt_type == OPERATOR_FUNCTION
                )
            }?
            {
                startElement(STYPE);
            }
            function_declaration[type_count] |

            set_int[type_count, type_count + 1]
            variable_declaration_type[type_count]
        )*
;

/*
  compatibility_alias

  Handles an Objective-C compatibility alias.
*/
compatibility_alias[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT| MODE_VARIABLE_NAME);

            // start the compatibility alias definition
            startElement(SCOMPATIBILITY_ALIAS);

        }

        COMPATIBILITY_ALIAS
;

/*
  class_directive

  Handles an Objective-C "@class" directive.
*/
class_directive[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_VARIABLE_NAME | MODE_LIST);

            // start the class declaration definition
            startElement(SCLASS_DECLARATION);
        }

        ATCLASS
        (identifier | COMMA)*
;

/*
  protocol_declaration
*/
protocol_declaration[] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT | MODE_LOCAL);

            startElement(SPROTOCOL_DECLARATION);
        }

        ATPROTOCOL
        (variable_identifier | COMMA)*
;

/*
  emit_statement_check
*/
emit_statement_check[] returns [bool is_emit_stmt] {
        ENTRY_DEBUG

        if (LA(1) != EMIT)
            return false;

        is_emit_stmt = false;

        int state = mark();
        ++inputState->guessing;

        consume();

        CALL_TYPE type = NOCALL;
        bool isempty = false;
        int call_count = 0;

        if (perform_call_check(type, isempty, call_count, -1) && type == CALL)
            is_emit_stmt = true;

        --inputState->guessing;
        rewind(state);
} :;

/*
  emit_statement

  Handles a Qt "emit" statement.
*/
emit_statement[] { ENTRY_DEBUG } :
        {
            // statement with nested statement (after condition)
            startNewMode(MODE_STATEMENT);

            // start the emit statement
            startElement(SEMIT_STATEMENT);

            startNewMode(MODE_EXPRESSION | MODE_EXPECT);
        }

        EMIT
;

/*
  friend_statement
*/
friend_statement[] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_FRIEND);

            startElement(SFRIEND);
        }

        FRIEND
;

/* Declarations Definitions CFG */

/*
  check_end

  Used to check the ending token.
*/
check_end[int& token] { token = LA(1); ENTRY_DEBUG } :
        LCURLY | TERMINATE | COLON | COMMA | RPAREN | EQUAL
;

/*
  class_declaration

  Handles a "class" declaration.
*/
class_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the class declaration
            startElement(SCLASS_DECLARATION);
        }

        class_preamble
        (CLASS | CXX_CLASS)
        class_post
        class_header

        (options { greedy = true; } :
            COMMA
            class_post
            class_header
        )*
;

/*
  class_preprocessing

  Handles "class" preprocessing items.
*/
class_preprocessing[int token] { ENTRY_DEBUG } :
        {
            bool intypedef = inMode(MODE_TYPEDEF);

            if (intypedef)
                startElement(STYPE);

            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS | MODE_DECL);

            if (intypedef)
                setMode(MODE_INNER_DECL);

            // start the class definition
            startElement(token);

            // classes end at the end of the block
            if (intypedef || inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP)) {
                setMode(MODE_END_AT_BLOCK);
            }
        }
;

/*
  class_preamble

  Handles anything that occurs before the "CLASS" token.
*/
class_preamble[] { ENTRY_DEBUG } :
        // suppress warning likely due to only having ()*
        (options { greedy = true; } :
            { inLanguage(LANGUAGE_JAVA) && next_token() != INTERFACE }?
            annotation |

            { inLanguage(LANGUAGE_CSHARP) }?
            attribute_csharp |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp
        )*

        (
            specifier |

            {inLanguage(LANGUAGE_JAVA)}? non_sealed_specifier |

            { LA(1) != TEMPLATE || next_token() != TEMPOPS }?
            template_specifier |

            { true }?
            template_declaration_full
        )*
;

/*
  class_definition

  Handles a "class" definition.
*/
class_definition[] { ENTRY_DEBUG } :
        class_preprocessing[SCLASS]
        class_preamble

        (CLASS | CXX_CLASS | RECORD)

        class_post
        (class_header lcurly[false] | lcurly[false])

        {
            if (inLanguage(LANGUAGE_CXX))
                class_default_access_action(SPRIVATE_ACCESS_DEFAULT);
        }
;

/*
  class_post

  Handles anything that occurs after the "CLASS" token.
*/
class_post[] { ENTRY_DEBUG } :
        (options { greedy = true; } :
            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp
        )*

        (options { greedy = true; } :
            { LA(1) != CRESTRICT && LA(1) != MUTABLE }?
            specifier
        )*
;

/*
  objective_c_class
*/
objective_c_class[] { bool first = true; ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT | MODE_CLASS);

            if (LA(1) == ATINTERFACE)
                startElement(SCLASS_INTERFACE);
            else
                startElement(SCLASS_IMPLEMENTATION);

            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_TOP | MODE_CLASS);
        }

        (ATINTERFACE | ATIMPLEMENTATION)

        (
            { first }?
            objective_c_class_header
            set_bool[first, false]
        )*

        (
            lcurly[false]

            {
                class_default_access_action(SPROTECTED_ACCESS_DEFAULT);
            }
        )*
;

/*
  protocol
*/
protocol[] { ENTRY_DEBUG } :
        { look_past_rule(&srcMLParser::protocol_declaration) == TERMINATE }?
        protocol_declaration |

        protocol_definition
;

/*
  protocol_definition
*/
protocol_definition[] { bool first = true; ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT | MODE_CLASS);

            startElement(SPROTOCOL);
        }

        ATPROTOCOL

        (
            { first }?
            objective_c_class_header
            set_bool[first, false]
        )*

        {
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_TOP | MODE_CLASS);

            class_default_access_action(SREQUIRED_DEFAULT);
        }
;

/*
  objective_c_class_header

  Handles an Objective-C "class" header.
*/
objective_c_class_header[] { ENTRY_DEBUG } :
        { isoption(parser_options, SRCML_PARSER_OPTION_CPP) }?
        (macro_call_check class_header_base LCURLY) => macro_call
        objective_c_class_header_base |

        objective_c_class_header_base
;

/*
  objective_c_class_header_base

  Handles an Objective-C "class" header base.
*/
objective_c_class_header_base[] { ENTRY_DEBUG } :
        // suppress ()* warning
        identifier

        // suppressed ()* warning
        (options { greedy = true; } : derived_list)*

        // suppressed ()* warning
        (category)*

        // suppressed ()* warning
        (protocol_list)*
;

/*
  objective_c_class_end
*/
objective_c_class_end[] { ENTRY_DEBUG } :
        {
            if (inTransparentMode(MODE_ACCESS_REGION)) {
                endDownToMode(MODE_TOP);

                // flush any whitespace tokens since sections should end at the last possible place
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

/*
  enum_class_definition

  Handles an "enum" class definition.
*/
enum_class_definition[] { ENTRY_DEBUG } :
        class_preprocessing[SENUM]

        {
            setMode(MODE_ENUM);
        }

        class_preamble
        ENUM
        (class_header enum_block | enum_block)
;

/*
  enum_class_declaration

  Handles an "enum" class declaration.
*/
enum_class_declaration[] { ENTRY_DEBUG } :
        class_preprocessing[SENUM_DECLARATION]
        class_preamble

        ENUM

        class_post
        class_header

        (options { greedy = true; } :
            COMMA
            class_post
            class_header
        )*
;

/*
  anonymous_class_definition

  Handles an anonymous class definition.
*/
anonymous_class_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS | MODE_END_AT_BLOCK | MODE_NO_BLOCK_CONTENT);

            // start the class definition
            startElement(SCLASS);
        }

        // first name in an anonymous class definition is the class it extends or the interface that it implements
        anonymous_class_super

        // argument list
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_NO_BLOCK_CONTENT);
        }

        call_argument_list
;

/*
  anonymous_class_super

  Handles cases where a "super" appears within an anonymous class.
*/
anonymous_class_super[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_LOCAL);

            // start the super name of an anonymous class
            startElement(SDERIVATION);
        }

        compound_name_inner[false]
;

/*
  interface_definition

  Handles an "interface" definition.
*/
interface_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS);

            // start the interface definition
            startElement(SINTERFACE);

            // java interfaces end at the end of the block
            setMode(MODE_END_AT_BLOCK);
        }

        class_preamble

        INTERFACE

        class_post
        class_header
        lcurly[false]
;

/*
  interface_declaration

  Handles an "interface" declaration.
*/
interface_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the interface declaration
            startElement(SINTERFACE_DECLARATION);
        }

        class_preamble

        INTERFACE

        class_post
        class_header

        (options { greedy = true; } :
            COMMA
            class_post
            class_header
        )*
;

/*
  struct_declaration

  Handles a "struct" declaration.
*/
struct_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the struct declaration
            startElement(SSTRUCT_DECLARATION);
        }

        class_preamble

        STRUCT

        class_post
        class_header

        (options { greedy = true; } :
            COMMA
            class_post
            class_header
        )*
;

/*
  struct_union_definition

  Handles a "struct" "union" definition.
*/
struct_union_definition[int element_token] { ENTRY_DEBUG } :
        class_preprocessing[element_token]
        class_preamble

        (STRUCT | UNION)

        class_post
        (class_header lcurly[false] | lcurly[false])

        {
            if (inLanguage(LANGUAGE_CXX))
                class_default_access_action(SPUBLIC_ACCESS_DEFAULT);
        }
;

/*
  union_declaration

  Handles the beginning of a "union" declaration.
*/
union_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the union declaration
            startElement(SUNION_DECLARATION);
        }

        class_preamble

        UNION

        class_post
        class_header

        (options { greedy = true; } :
            COMMA
            class_post
            class_header
        )*
;

/*
  annotation_definition
*/
annotation_definition[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_CLASS);

            // start the annotation definition
            startElement(SANNOTATION_DEFN);

            // java annotations end at the end of the block
            setMode(MODE_END_AT_BLOCK);
        }

        class_preamble

        ATSIGN
        INTERFACE

        class_header
        lcurly[false]
;

/*
  class_default_access_action

  Handles default private/public section for C++
*/
class_default_access_action[int access_token] { ENTRY_DEBUG } :
        {
            if (
                (
                    inLanguage(LANGUAGE_CXX)
                    || inLanguage(LANGUAGE_OBJECTIVE_C)
                )
                && (
                    SkipBufferSize() > 0
                    || !(
                        LA(1) == PUBLIC
                        || LA(1) == PRIVATE
                        || LA(1) == PROTECTED
                        || LA(1) == SIGNAL
                        || LA(1) == ATREQUIRED
                        || LA(1) == ATOPTIONAL
                    )
                )
            ) {
                // create block section
                section_entry_action_first();

                // start private element
                if (LA(1) == RCURLY && SkipBufferSize() == 0)
                    // empty element for empty (no whitespace even) block
                    emptyElement(access_token);
                else {
                    // start private element before whitespace
                    startNoSkipElement(access_token);
                    setMode(MODE_ACCESS_REGION);
                }
            } else if (inLanguage(LANGUAGE_C)) {
                // have to create an empty section for anonymous structs, not sure why
                section_entry_action_first();
            }
        }
;

/*
  class_header

  Handles a "class" header.
*/
class_header[] { ENTRY_DEBUG } :
        { isoption(parser_options, SRCML_PARSER_OPTION_CPP) && next_token() != DCOLON }?
        (macro_call_check class_header_base LCURLY) => macro_call
        class_header_base |

        class_header_base
;

/*
  class_header_base

  Handles a "class" header base.
*/
class_header_base[] { bool insuper = false; ENTRY_DEBUG } :
        {
            setMode(MODE_CLASS_NAME);

            class_namestack.push(LT(1)->getText());
        }

        // suppress ()* warning
        (
            { LA(1) != FINAL }?
            compound_name |

            keyword_name
        )

        {
            clearMode(MODE_CLASS_NAME);
        }

        (options { greedy = true; } :
            { LA(1) == FINAL 
                || inLanguage(LANGUAGE_JAVA) && LA(1) == SEALED }?
            specifier |
            {inLanguage(LANGUAGE_JAVA) && LA(1) == NON}?
            non_sealed_specifier
        )*

        (
            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            (options { greedy = true; } : derived_list)
        )*

        // move suppressed ()* warning to begin
        (options { greedy = true; } :
            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            generic_type_constraint
        )*

        (options { greedy = true; } :
            { inLanguage(LANGUAGE_JAVA_FAMILY) }?
            parameter_list
        )*

        (options { greedy = true; } :
            { inLanguage(LANGUAGE_JAVA_FAMILY) }?
            (options { greedy = true; } :
                super_list_java

                {
                    insuper = true;
                }

                (extends_list | implements_list | permits_list)
                (options { greedy = true; } : extends_list | implements_list | permits_list)*
            )
        )*

        {
            if (insuper)
                endMode();
        }
;

/*
  access_specifier_region

  Each instance of an access specifier defines a region in a class.
*/
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

        (
            { !inLanguage(LANGUAGE_OBJECTIVE_C) && first }?
            (compound_name)*
            COLON
            set_bool[first, false]
        )*
;

/*
  lcurly

  Marks the start of a block.  The end of the block is handled in rcurly.
*/
lcurly[bool content = true] { ENTRY_DEBUG } :
        {
            if (inMode(MODE_NO_BLOCK_CONTENT))
                content = false;

            // special end for conditions
            if (inTransparentMode(MODE_CONDITION) && !inMode(MODE_ANONYMOUS) && !inMode(MODE_CLASS)) {
                endDownToMode(MODE_CONDITION);
                endMode(MODE_CONDITION);
            }

            if (inTransparentMode(MODE_TRAILING_RETURN)) {
                endDownToMode(MODE_TRAILING_RETURN);
                endMode(MODE_TRAILING_RETURN);
            }

            if (inMode(MODE_IF)) {
                // then part of the if statement (after the condition)
                startNewMode(MODE_STATEMENT | MODE_NEST | MODE_THEN);

                // Commented-out code
                // start the then element
                // startNoSkipElement(STHEN);
            }

            // special end for constructor member initialization list
            if (inMode(MODE_LIST | MODE_CALL)) {
                // flush any whitespace tokens since sections should end at the last possible place
                flushSkip();

                endMode();
            }
        }

        lcurly_base[content]

        {
            incCurly();

            // alter the modes set in lcurly_base
            setMode(MODE_TOP | MODE_STATEMENT | MODE_NEST | MODE_LIST);
        }
;

/*
  lcurly_base

  Handles the left curly brace base.  Used in multiple places.
*/
lcurly_base[bool content = true] { ENTRY_DEBUG } :
        {
            // need to pass on class mode to detect constructors for java
            bool inclassmode = (inLanguage(LANGUAGE_JAVA_FAMILY) || inLanguage(LANGUAGE_CSHARP)) && inMode(MODE_CLASS);

            bool in_function_body = inTransparentMode(MODE_FUNCTION_TAIL);

            startNewMode(MODE_BLOCK);

            if (inclassmode)
                setMode(MODE_CLASS);

            if (in_function_body)
                setMode(MODE_FUNCTION_BODY);

            startElement(SBLOCK);
        }

        LCURLY

        {
            if (content) {
                startNewMode(MODE_BLOCK_CONTENT);
                startNoSkipElement(SCONTENT);
            }
        }

        set_bool[skip_ternary, false]
;

/*
  block_end

  Handles the end of a block.  Also indicates the end of some open elements.
  For "if" with "then" followed by "else," the block is handled, but the scope of completion stops at "if."
*/
block_end[] { bool in_issue_empty = inTransparentMode(MODE_ISSUE_EMPTY_AT_POP); ENTRY_DEBUG } :
        rcurly

        {
            if (inMode(MODE_ANONYMOUS)) {
                endMode();

                if (LA(1) == LPAREN) {
                    call_argument_list();
                }

                return;
            }

            // end all the statements this statement is nested in
            // special case when ending then of if statement: end down to either a block or top section, or to an if, whichever is reached first
            endDownToModeSet(MODE_BLOCK | MODE_TOP | MODE_IF | MODE_ELSE | MODE_TRY | MODE_ANONYMOUS);

            bool endstatement = inMode(MODE_END_AT_BLOCK);
            bool anonymous_class = (inMode(MODE_CLASS) || inMode(MODE_ENUM)) && inMode(MODE_END_AT_BLOCK);

            // some statements end with the block
            if (inMode(MODE_END_AT_BLOCK)) {
                endMode();

                if (inTransparentMode(MODE_TEMPLATE))
                    endMode();
            }

            // looking for a terminate character (';'); some statements end with the block if there is no terminate
            if (inMode(MODE_END_AT_BLOCK_NO_TERMINATE) && LA(1) != TERMINATE) {
                endstatement = true;
                endMode();
            }

            if (!(anonymous_class) && (!(inMode(MODE_CLASS) || inMode(MODE_ENUM)) || endstatement))
                else_handling();

            // if true, we need to markup the (abbreviated) variable declaration
            if (inMode(MODE_DECL) && LA(1) != TERMINATE)
                short_variable_declaration();

            if (!in_issue_empty && inMode(MODE_END_AT_ENDIF | MODE_TOP | MODE_STATEMENT))
                endMode();
        }
;

/*
  rcurly

  Handles a right curly brace.  Not used directly, but called by block_end.
*/
rcurly[] { ENTRY_DEBUG } :
        {
            // end any elements inside of the block; this is basically endDownToMode(MODE_TOP) but checks for class ending
            if (inTransparentMode(MODE_TOP)) {
                while (size() > 1 && !inMode(MODE_TOP)) {
                    if (inMode(MODE_CLASS))
                        if (!class_namestack.empty()) {
                            class_namestack.pop();
                        }

                    endMode();
                }
            }

            // flush any whitespace tokens since sections should end at the last possible place
            flushSkip();

            if (isPaused()) {
                nopStreamStart();
            }

            // end any sections inside the mode
            endWhileMode(MODE_TOP_SECTION);

            if (inMode(MODE_BLOCK_CONTENT))
                endMode(MODE_BLOCK_CONTENT);

            if (getCurly() != 0)
                decCurly();
        }

        RCURLY

        {
            // end the current mode for the block; do not end more than one since they may be nested
            endMode(MODE_TOP);
        }
;

/*
  terminate

  Begins the process of handling a terminate character (';').
*/
terminate[] { ENTRY_DEBUG resumeStream(); } :
        {
            if (inMode(MODE_IGNORE_TERMINATE)) {
                if (inMode(MODE_CONTROL_INITIALIZATION | MODE_EXPECT))
                    control_initialization_action();
                else
                    control_condition_action();
            }
        }

        terminate_pre
        terminate_token
        terminate_post
;

/*
  terminate_token

  Matches the actual terminate token (';').
*/
terminate_token[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (
                inMode(MODE_STATEMENT | MODE_NEST)
                && (
                    !inMode(MODE_DECL)
                    && !inTransparentMode(MODE_FRIEND)
                    && (
                        !inLanguage(LANGUAGE_JAVA)
                        || !inMode(MODE_ENUM | MODE_LIST)
                    )
                )
            )
                startElement(SEMPTY);

            wait_terminate_post = true;
        }

        TERMINATE
        set_bool[skip_ternary, false]
;

/*
  terminate_pre

  Handles pre-terminate processing.
*/
terminate_pre[] { ENTRY_DEBUG } :
        {
            // end any elements inside of the statement
            if (!inMode(MODE_TOP | MODE_STATEMENT | MODE_NEST))
                endDownToModeSet(MODE_STATEMENT | MODE_EXPRESSION_BLOCK | MODE_INTERNAL_END_CURLY | MODE_INTERNAL_END_PAREN);

            if (inTransparentMode(MODE_TRAILING_RETURN)) {
                endDownToMode(MODE_TRAILING_RETURN);
                endMode(MODE_TRAILING_RETURN);
            }
        }
;

/*
  terminate_post

  Handles the post-terminate processing.
*/
terminate_post[] { bool in_issue_empty = inTransparentMode(MODE_ISSUE_EMPTY_AT_POP); ENTRY_DEBUG } :
        {
            // end all the statements this statement is nested in; special case when ending then of if statement
            if (
                (
                    !inMode(MODE_EXPRESSION_BLOCK)
                    || inMode(MODE_EXPECT)
                )
                && !inMode(MODE_INTERNAL_END_CURLY)
                && !inMode(MODE_INTERNAL_END_PAREN)
                && !inMode(MODE_STATEMENT | MODE_ISSUE_EMPTY_AT_POP)
                && !inMode(MODE_END_AT_ENDIF)
            ) {
                // end down to either a block or top section, or to an if or else
                endDownToModeSet(MODE_TOP | MODE_IF | MODE_ELSE | MODE_SWITCH);
            }
        }

        else_handling

        {
            if (inMode(MODE_SWITCH))
                endMode();

            if (inMode(MODE_STATEMENT | MODE_ISSUE_EMPTY_AT_POP))
                endMode();

            if (!in_issue_empty && inMode(MODE_END_AT_ENDIF))
                endMode();

            wait_terminate_post = false;
        }
;

/*
  else_handling

  Handles the possible endings of "else" statements.

  Called from all places that end a statement, and could possibly end the "else" that the target statement was in.
  I.e., terminate (";"), end of a statement block, etc.

  If in an if-statement, relatively straightforward.  Note that we could be ending with multiple "else"s.

  Special case: "else" with no matching "if."  This occurs with either a single "else" or with an "else" in a
  preprocessor "#if ... #else ... #endif" construct (this is very common).
*/
else_handling[] { ENTRY_DEBUG } :
        {
            // record the current size of the top of the cppmode stack to detect any #else or #endif in consumeSkippedTokens (see below)
            auto cppmode_size = !cppmode.empty() ? cppmode.top().statesize.size() : 0;

            // catch and finally statements are nested inside of a try, if at that level; if no CATCH or FINALLY, then end now
            bool intry = inMode(MODE_TRY);
            bool in_for_like_list = inMode(MODE_FOR_LIKE_LIST);
            bool restoftry = LA(1) == CATCH || LA(1) == CXX_CATCH || LA(1) == FINALLY;

            if (intry && !restoftry) {
                endMode(MODE_TRY);
                endDownToMode(MODE_TOP);
            }

            // handle parts of if
            if (inTransparentMode(MODE_IF) && !(intry && restoftry) && !in_for_like_list) {
                if (LA(1) != ELSE) {
                    endDownToMode(MODE_TOP);
                // when an ELSE is next and already in an else, must end properly (not needed for then)
                } else if (inMode(MODE_ELSE)) {
                    // when an else but not elseif
                    while (inMode(MODE_ELSE) && !inMode(MODE_IF)) {
                        // end the else
                        endMode();

                        // we have an extra else that is rogue; it either is a single else statement, or part of an #ifdef ... #else ... #endif
                        if (LA(1) == ELSE && ifcount == 1)
                            break;

                        // ending an else means ending an if
                        if (inMode(MODE_IF)) {
                            if (inMode(MODE_IF_STATEMENT))
                                --ifcount;

                            endMode();
                        }
                    }

                    // following ELSE indicates end of outer then
                    if (inMode(MODE_THEN))
                        endMode();

                    // if in elseif then end it
                    if (inMode(MODE_IF | MODE_ELSE)) {
                        if (inMode(MODE_IF_STATEMENT))
                            --ifcount;

                        endMode();
                    }
                }
            } else if (inTransparentMode(MODE_ELSE)) {
                // have an else not in an if; could be a fragment, or could be due to an #ifdef ... #else ... #endif
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

/*
  statement_part

  Handles being in the middle of a statement.
*/
statement_part[] {
        int type_count;
        int secondtoken = 0;
        int after_token = 0;
        STMT_TYPE stmt_type = NONE;
        CALL_TYPE type = NOCALL;
        bool isempty = false;
        int call_count = 0;
        ENTRY_DEBUG
} :
        { inMode(MODE_EAT_TYPE) }?
        type_identifier
        update_typecount[MODE_FUNCTION_NAME] |

        // block right after argument list, e.g., throws list in Java
        { inTransparentMode(MODE_END_LIST_AT_BLOCK) }?
        { endDownToMode(MODE_LIST); endMode(MODE_LIST); }
        lcurly |

        { inMode(MODE_ENUM) && inMode(MODE_LIST) }?
        enum_short_variable_declaration |

        { inMode(MODE_FOR_LIKE_LIST) }?
        for_like_list_item |

        /*
          MODE_EXPRESSION
        */

        // expression block or expressions; must check before expression
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
        throw_list
        complete_arguments
        (comma complete_arguments)*
        {
            endDownToMode(MODE_LIST);
            endMode(MODE_LIST);
        } |

        // throw list at end of function header
        { (inLanguage(LANGUAGE_CXX)) && inMode(MODE_FUNCTION_TAIL) }?
        ref_qualifier |

        // throw list at end of function header
        { (inLanguage(LANGUAGE_CXX)) && inTransparentMode(MODE_FUNCTION_TAIL) }?
        noexcept_list |

        // throw list at end of function header
        {
            (inLanguage(LANGUAGE_CXX))
            && inTransparentMode(MODE_FUNCTION_TAIL)
            && next_token() == LBRACKET
        }?
        attribute_cpp |

        // K&R function parameters
        {
            (inLanguage(LANGUAGE_C) || inLanguage(LANGUAGE_CXX))
            && inMode(MODE_FUNCTION_TAIL)
            && pattern_check(stmt_type, secondtoken, type_count, after_token)
            && stmt_type == VARIABLE
            && after_token != LCURLY
        }?
        kr_parameter[type_count] |

        // function try block, must be before function_specifier
        { inLanguage(LANGUAGE_CXX_FAMILY) && inMode(MODE_FUNCTION_TAIL) }?
        try_statement |

        // function specifier at end of function header
        {
            inLanguage(LANGUAGE_CXX_FAMILY)
            && inMode(MODE_FUNCTION_TAIL)
            && (
                LA(1) != EQUAL
                || (
                    inLanguage(LANGUAGE_CXX)
                    && (
                        next_token() == CONSTANTS
                        || next_token() == DEFAULT
                        || next_token() == DELETE
                    )
                )
            )
        }?
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
        {
            inMode(MODE_EXPRESSION | MODE_EXPECT)
            && isoption(parser_options, SRCML_PARSER_OPTION_CPP)
            && perform_call_check(type, isempty, call_count, secondtoken)
            && type == MACRO
        }?
        macro_call |

        { inMode(MODE_EXPRESSION | MODE_EXPECT) }?
        expression[type, call_count] |

        // already in an expression and ran into a keyword; stop the expression and markup the keyword statement
        {
            inMode(MODE_EXPRESSION)
            && !(
                inLanguage(LANGUAGE_OBJECTIVE_C)
                && LA(1) == IMPORT
            )
            && !(
                LA(1) == ATPROTOCOL
                && next_token() == LPAREN
            )
            && (
                LA(1) != DEFAULT
                || next_token() == COLON
            )
            && (
                LA(1) != CHECKED
                || next_token() == LCURLY
            )
            && (
                LA(1) != UNCHECKED
                || next_token() == LCURLY
            )
            && (
                LA(1) != CXX_TRY
                || next_token() == LCURLY
            )
            && (
                LA(1) != INLINE
                || next_token() == NAMESPACE
            )
            && (
                LA(1) != STATIC
                || (
                    inLanguage(LANGUAGE_JAVA)
                    && next_token() == LCURLY
                )
            )
            && (
                LA(1) != CXX_CATCH
                || next_token() == LPAREN
                || next_token() == LCURLY
            )
            && (
                LA(1) != ASM
                || look_past_two(ASM, VOLATILE) == LPAREN
            )
            && (
                LA(1) != EMIT
                || emit_statement_check()
            )
        }?
        terminate_pre
        terminate_post
        keyword_statements |

        // already in an expression
        { inMode(MODE_EXPRESSION) }?
        expression_part_plus_linq |

        // call list in member initialization list
        {
            inMode(MODE_CALL | MODE_LIST)
            && (
                LA(1) != LCURLY
                || inLanguage(LANGUAGE_CXX)
            )
        }?
        member_init |

        // call list in member initialization list
        {
            inMode(MODE_CALL | MODE_LIST)
            && (
                LA(1) != LCURLY
                || inLanguage(LANGUAGE_CXX)
            )
        }?
        keyword_calls |

        /*
          MODE_VARIABLE_NAME
        */

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
        {
            inMode(MODE_INIT | MODE_EXPECT)
            && (
                (
                    LA(1) == COLON
                    && (
                        (
                            inLanguage(LANGUAGE_C_FAMILY)
                            && !inLanguage(LANGUAGE_CSHARP)
                        )
                        || inLanguage(LANGUAGE_JAVA)
                    )
                )
                || LA(1) == IN
            )
        }?
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
          Check for MODE_CONTROL_CONDITION before template stuff, since it can conflict
        */

        // inside of control group expecting initialization
        { inMode(MODE_CONTROL | MODE_EXPECT) }?
        control_group |

        {
            inMode(MODE_CONTROL_INITIALIZATION | MODE_EXPECT)
            || inMode(MODE_CONTROL_CONDITION | MODE_EXPECT)
        }?
        control_initialization_pre |

        // inside of control group expecting initialization
        { inMode(MODE_CONTROL_INCREMENT | MODE_EXPECT) }?
        control_increment |

        {
            inTransparentMode(MODE_TEMPLATE)
            && inMode(MODE_LIST | MODE_EXPECT)
            && !inMode(MODE_TEMPLATE_PARAMETER_LIST)
        }?
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

        // sometimes end up here if a control group ends early or with a for-each
        rparen |

        // seem to end up here for colon in ternary operator
        colon_marked |

        // markup namespace tag using namespace
        namespace_definition
;

/*
  lparen_marked

  Used to mark the left parenthesis operator.
*/
lparen_marked[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            incParen();

            startElement(SOPERATOR);
        }

        LPAREN
;

/*
  bar

  Used to mark the "|" operator.
*/
bar[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!inMode(MODE_PARAMETER))
                startElement(SOPERATOR);
        }

        BAR
;

/*
  comma

  Used to handle commas.
*/
comma[] { bool markup_comma = true; ENTRY_DEBUG } :
        {
            // comma ends the current item in a list or ends the current expression
            if (
                !inTransparentMode(MODE_PARSE_EOL)
                && (
                    inTransparentMode(MODE_LIST)
                    || inTransparentMode(MODE_STATEMENT)
                )
            )
                // might want to check for !inMode(MODE_INTERNAL_END_CURLY)
                endDownToModeSet(MODE_LIST | MODE_STATEMENT);

            // comma in a variable initialization ends init of current variable
            if (inMode(MODE_IN_INIT))
                endMode(MODE_IN_INIT);

            if (inMode(MODE_LOCAL | MODE_VARIABLE_NAME) && !inTransparentMode(MODE_CONTROL_CONDITION | MODE_END_AT_COMMA))
                endMode();

            if (inMode(MODE_INIT | MODE_EXPECT | MODE_ENUM))
                endDownToModeSet(MODE_ENUM | MODE_TOP);

            if (inMode(MODE_INIT | MODE_VARIABLE_NAME | MODE_LIST) || inTransparentMode(MODE_CONTROL_CONDITION | MODE_END_AT_COMMA))
                markup_comma = false;
        }

        comma_marked[markup_comma]

        {
            if (inTransparentMode(MODE_CONTROL_CONDITION | MODE_END_AT_COMMA)) {
                startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);
                startNoSkipElement(SDECLARATION_RANGE);
            }
        }
;

/*
  comma_marked

  Used to mark the comma (",") operator.
*/
comma_marked[bool markup_comma = true] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (
                markup_comma
                && (
                    !inMode(MODE_PARAMETER)
                    && !inMode(MODE_ARGUMENT)
                    && !(
                        inTransparentMode(MODE_IN_INIT)
                        && inMode(MODE_EXPRESSION | MODE_LIST)
                    )
                )
                && !inMode(MODE_ENUM)
                && !inMode(MODE_INTERNAL_END_CURLY)
                && !inMode(MODE_INITIALIZATION_LIST)
            )
                startElement(SOPERATOR);
        }

        COMMA
;

/*
  colon_marked

  Used to mark a colon (":").
*/
colon_marked[] {
        bool in_ternary = inTransparentMode(MODE_TERNARY | MODE_THEN);
        bool markup_colon = true;
        LightweightElement element(this);
        ENTRY_DEBUG
} :
        {
            if (in_ternary) {
                endDownToMode(MODE_THEN);

                flushSkip();

                endMode(MODE_THEN);

                startNewMode(MODE_ELSE | MODE_EXPRESSION | MODE_EXPECT);

                startElement(SELSE);

                markup_colon = false;
            }

            // only needed for a ranged for and not a declaration
            if (inTransparentMode(MODE_RANGED_FOR)) {
                // start a new mode that will end after the argument list
                startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

                // start the initialization element
                startElement(SDECLARATION_RANGE);

                markup_colon = false;
            }

            if (inLanguage(LANGUAGE_OBJECTIVE_C) && inTransparentMode(MODE_INTERNAL_END_CURLY)) {
                endDownToMode(MODE_INTERNAL_END_CURLY);
            }

            if (
                markup_colon
                && !(
                    in_ternary
                    && true
                )
                && (
                    !inLanguage(LANGUAGE_OBJECTIVE_C)
                    || !inMode(MODE_INTERNAL_END_CURLY)
                )
            )
                startElement(SOPERATOR);
        }

        COLON
;

/*
  colon

  Used to process a colon that was not marked.
*/
colon[] { ENTRY_DEBUG } :
        {
            // colon ends the current item in a list
            if (inTransparentMode(MODE_TOP_SECTION))
                endDownToMode(MODE_TOP_SECTION);
        }

        COLON

        {
            if (inMode(MODE_DETECT_COLON))
                endMode(MODE_DETECT_COLON);
        }
;

/*
  condition

  Condition contained in "if"/"while"/"switch."  Triggered by left parentheses in mode MODE_CONDITION or MODE_EXPECT.
  Starts condition mode and prepares to handle an embedded expression.  The end of the element is handled in condition_rparen.
*/
condition[] { ENTRY_DEBUG } :
        {
            assertMode(MODE_CONDITION | MODE_EXPECT);

            // start element condition outside of the left parentheses
            startElement(SCONDITION);

            // mark the condition mode as the one to stop at a right parentheses; non-empty conditions contain an expression
            setMode(MODE_LIST | MODE_EXPRESSION | MODE_EXPECT);
        }

        LPAREN
        condition_inner
;

/*
  condition_inner
*/
condition_inner[] {
        ENTRY_DEBUG

        int type_count = 0;
        int secondtoken = 0;
        int after_token = 0;
        STMT_TYPE stmt_type = NONE;

        pattern_check(stmt_type, secondtoken, type_count, after_token);

        if (stmt_type == VARIABLE) {
            startNewMode(MODE_INTERNAL_END_PAREN);
        }
} :
        { stmt_type == VARIABLE }?
        control_initialization_variable_declaration[type_count] |

        { LA(1) != RPAREN }?
        expression |

        rparen[false]
;

/*
  pattern_check

  Performs an arbitrary look ahead, looking for a pattern.
*/
pattern_check[STMT_TYPE& type, int& token, int& type_count, int& after_token, bool inparam = false] returns [bool isdecl] {
        isdecl = true;

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
            pattern_check_core(
                token,
                fla,
                type_count,
                specifier_count,
                attribute_count,
                template_count,
                type,
                inparam,
                sawtemplate,
                sawcontextual,
                posin
            );
        } catch (...) {
            if (type == VARIABLE && type_count == 0) {
                type_count = 1;
            }
        }

        if (
            type == VARIABLE
            && inTransparentMode(MODE_CONDITION)
            && LA(1) != EQUAL
        )
            type = NONE;

        if (
            type == NONE
            && (
                sawtemplate
                || (
                    sawcontextual
                    && type_count > 0
                )
            )
            && (
                !keyword_name_token_set.member(LA(1))
                || LA(1) == MULTOPS
                || LA(1) == REFOPS
                || LA(1) == RVALUEREF
                || LA(1) == TERMINATE
                || LA(1) == RECORD
            )
        )
            type = VARIABLE;

        // may just have an expression
        if (type == VARIABLE && posin)
            type_count = posin - 1;

        // may just have a single macro (no parens possibly) before a statement
        else if (
            type == 0
            && type_count == 0
            && keyword_token_set.member(LA(1))
        )
            type = SINGLE_MACRO;

        else if (
            type == 0
            && type_count == 1
            && (
                LA(1) == CLASS
                || LA(1) == CXX_CLASS
                || LA(1) == STRUCT
                || LA(1) == UNION
            )
        ) {
            pattern_check(type, token, type_count, after_token, inparam);

            type_count += 1;

            if (
                type == CLASS_DECL
                || type == CLASS_DEFN
                || type == UNION_DECL
                || type == UNION_DEFN
                || type == STRUCT_DECL
                || type == STRUCT_DEFN
                || type == ENUM_DECL
                || type == ENUM_DEFN
                || type == 0
            ) {
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
        else if (
            type == FUNCTION
            && (
                fla == TERMINATE
                || fla == COMMA
                || fla == EQUAL
            )
        )
            type = FUNCTION_DECL;

        // declaration form
        else if (
            type == OPERATOR_FUNCTION
            && (
                fla == TERMINATE
                || fla == COMMA
                || fla == EQUAL
            )
        )
            type = OPERATOR_FUNCTION_DECL;

        // we actually have a macro and then a constructor
        else if (type == FUNCTION && fla == COLON)
            type = SINGLE_MACRO;

        // not really a destructor
        if (
            type == DESTRUCTOR_DECL
            && (
                !inTransparentMode(MODE_CLASS)
                || inTransparentMode(MODE_FUNCTION_TAIL)
            )
        )
            type = EXPRESSION;

        if (
            (
                type == FUNCTION
                || type == FUNCTION_DECL
            )
            && fla == COMMA
            && !inparam
        )
            type = VARIABLE;

        after_token = LA(1);

        inputState->guessing--;
        rewind(start);

        if (
            type == VARIABLE
            && type_count == (specifier_count + attribute_count + template_count)
        )
            ++type_count;

        if (
            !inMode(MODE_FUNCTION_TAIL)
            && type == 0
            && type_count == 0
            && (
                enum_preprocessing_token_set.member(LA(1))
                || LA(1) == DECLTYPE
            )
            && (
                !inLanguage(LANGUAGE_CXX)
                || !(LA(1) == FINAL
                || LA(1) == OVERRIDE)
            )
            && after_token == TERMINATE
        ) {
            type = VARIABLE;
            type_count = 1;
        }

        if (type == NONE && LA(1) == TEMPLATE)
            type = GLOBAL_TEMPLATE;

        if (
            inLanguage(LANGUAGE_CSHARP)
            && type == FUNCTION_DECL
            && fla == TERMINATE
            && LA(1) == DELEGATE
        ) {
            type_count -= 1;
            type = DELEGATE_TYPE;
        }

        if (
            inLanguage(LANGUAGE_CSHARP)
            && type == FUNCTION_DECL
            && fla == TERMINATE
            && (
                token == DELEGATE
                && (
                    LA(1) == PUBLIC
                    || LA(1) == PRIVATE
                    || LA(1) == PROTECTED
                )
            )
        ) {
            type_count -= 2;
            type = DELEGATE_TYPE;
        }
} :;

/*
  pattern_check_core

  Figures out if we have a declaration, either variable or function. This is pretty complicated as it has to
  decide whether it is a declaration or not, and whether it is a function or a variable declaration.
*/
pattern_check_core[
        int& token,           /* second token, after name (always returned) */
        int& fla,             /* for a function, TERMINATE or LCURLY, 0 for a variable */
        int& type_count,      /* number of tokens in type (not including name) */
        int& specifier_count, /* number of tokens that are specifiers */
        int& attribute_count, /* number of tokens that are attributes */
        int& template_count,  /* number of tokens that are templates */
        STMT_TYPE& type,      /* type discovered */
        bool inparam,         /* are we in a parameter */
        bool& sawtemplate,    /* have we seen a template */
        bool& sawcontextual,  /* have we seen a contextual keyword */
        int& posin
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
        isdestructor = false; /* global flag detected during name matching */

        bool foundpure = false;
        bool isoperator = false;
        bool ismain = false;
        bool isconstructor = false;
        bool saveisdestructor = false;
        bool endbracket = false;
        bool modifieroperator = false;
        bool is_c_class_identifier = false;
        bool is_record = false;

        is_qmark = false;
        int real_type_count = 0;
        bool lcurly = false;
        bool is_event = false;

        ENTRY_DEBUG
} :
        // main pattern for variable declarations, and most function declaration/definitions
        // the trick is to look for function declarations/definitions, and along the way record if it is a declaration

        // Commented-out code
        // int -> NONE
        // int f -> VARIABLE
        // int f(); -> FUNCTION
        // int f() {} -> FUNCTION

        /*
          Process all the parts of a potential type.  Keep track of total parts, specifier parts, and second token.
        */

        (
            (
                {
                    (
                        (
                            inLanguage(LANGUAGE_JAVA_FAMILY)
                            || inLanguage(LANGUAGE_CSHARP)
                            || (type_count == 0)
                        )
                        || (
                            LA(1) != LBRACKET
                            || next_token() == LBRACKET
                        )
                    )
                    && (
                        LA(1) != IN
                        || !inTransparentMode(MODE_CONTROL_CONDITION)
                    )
                }?
                set_bool[is_qmark, (is_qmark || (LA(1) == QMARK)) && inLanguage(LANGUAGE_CSHARP)]

                set_int[posin, LA(1) == IN ? posin = type_count : posin]
                set_int[parameter_pack_pos, LA(1) == DOTDOTDOT ? parameter_pack_pos = type_count : parameter_pack_pos]

                set_bool[isoperator, isoperator || LA(1) == OPERATOR]

                // indicates whether a bracket was at the end; necessary for Java
                set_bool[endbracket, inLanguage(LANGUAGE_JAVA_FAMILY) && LA(1) == LBRACKET]

                // record any type modifiers that are also operators
                // this is for disambiguation of destructor declarations from expressions involving the ~ operator
                set_bool[modifieroperator, modifieroperator || LA(1) == REFOPS || LA(1) == MULTOPS || LA(1) == QMARK]

                set_bool[sawcontextual, sawcontextual || LA(1) == CRESTRICT || LA(1) == MUTABLE]

                (
                    {
                        argument_token_set.member(LA(1))
                        && (
                            LA(1) != SIGNAL
                            || (
                                LA(1) == SIGNAL
                                && look_past(SIGNAL) == COLON
                            )
                        )
                        && (
                            !inLanguage(LANGUAGE_CXX)
                            || (
                                /* Commented-out code: LA(1) != FINAL && */
                                LA(1) != OVERRIDE
                            )
                        )
                        && (
                            LA(1) != TEMPLATE
                            || next_token() != TEMPOPS
                        )
                        && (
                            LA(1) != ATOMIC
                            || next_token() != LPAREN
                        )
                    }?

                    set_int[token, LA(1)]
                    set_bool[foundpure, foundpure || (LA(1) == CONST || LA(1) == TYPENAME)]

                    (options { generateAmbigWarnings = false; } :
                        EXTERN

                        (options { greedy = true; } :
                            ALIAS
                            set_int[specifier_count, specifier_count + 1]
                        )* |

                        {
                            LA(1) != NEW
                            || (
                                inLanguage(LANGUAGE_CSHARP)
                                && (
                                    inPrevMode(MODE_CLASS)
                                    || specifier_count > 0
                                )
                            )
                        }?
                        specifier |

                        template_specifier
                        set_bool[sawtemplate, true] |

                        { next_token() == COLON }?
                        SIGNAL |

                        ATREQUIRED | ATOPTIONAL |

                        { inLanguage(LANGUAGE_JAVA) }?
                        default_specifier |

                        { inLanguage(LANGUAGE_JAVA) }?
                        non_sealed_specifier
                    )

                    set_int[specifier_count, specifier_count + 1]

                    set_type[
                        type,
                        ACCESS_REGION,
                        (
                            (
                                inLanguage(LANGUAGE_CXX)
                                && look_past_two(NAME, VOID) == COLON
                            )
                            || inLanguage(LANGUAGE_OBJECTIVE_C)
                        )
                        && (
                            token == PUBLIC
                            || token == PRIVATE
                            || token == PROTECTED
                            || token == SIGNAL
                            || token == ATREQUIRED
                            || token == ATOPTIONAL
                        )
                    ]

                    throw_exception[type == ACCESS_REGION] |

                    { true }?
                    template_declaration_full
                    set_int[template_count, template_count + 1] |

                    { inLanguage(LANGUAGE_CSHARP) }?
                    LBRACKET

                    // suppress warning
                    (options { greedy = true; } : COMMA)*

                    // ~RBRACKET matches these as well
                    // suppress warning
                    (options { warnWhenFollowAmbig = false; } :
                        (
                            RETURN | EVENT |
                            set_type[type, GLOBAL_ATTRIBUTE, check_global_attribute()]

                            throw_exception[type == GLOBAL_ATTRIBUTE]
                            identifier
                        )
                    )?

                    // complete_expression
                    (~(RBRACKET))*

                    RBRACKET

                    set_int[attribute_count, attribute_count + 1] |

                    { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
                    LBRACKET
                    LBRACKET

                    // Commented-out code
                    // complete_expression

                    (~(RBRACKET))*

                    RBRACKET
                    RBRACKET

                    set_type[type, GLOBAL_ATTRIBUTE]
                    set_int[attribute_count, attribute_count + 1] |

                    { type_count == (attribute_count + specifier_count) }?
                    property_method_name
                    set_type[type, PROPERTY_ACCESSOR, true] |

                    {
                        type_count == attribute_count + specifier_count + template_count
                        && (
                            !inLanguage(LANGUAGE_JAVA)
                            || (
                                inLanguage(LANGUAGE_JAVA)
                                && (
                                    LA(1) != ATSIGN
                                    || (
                                        LA(1) == ATSIGN
                                        && next_token() == INTERFACE
                                    )
                                )
                            )
                        )
                        && (
                            !inLanguage(LANGUAGE_CXX)
                            || (
                                !keyword_name_token_set.member(next_token())
                                || (
                                    next_token() == LBRACKET
                                    && next_token_two() == LBRACKET
                                )
                            )
                        )
                    }?
                    (
                        CLASS
                        set_type[type, CLASS_DECL] |

                        CXX_CLASS
                        set_type[type, CLASS_DECL] |

                        RECORD
                        set_type[type, CLASS_DECL] set_bool[is_record, true] set_bool[sawcontextual, true] |

                        STRUCT
                        set_type[type, STRUCT_DECL] |

                        UNION
                        set_type[type, UNION_DECL] |

                        INTERFACE
                        set_type[type, INTERFACE_DECL] |

                        ATSIGN
                        INTERFACE
                        set_type[type, ANNOTATION_DEFN]
                    )

                    set_bool[lcurly, LA(1) == LCURLY]

                    (options { greedy = true; } :
                        { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
                        attribute_cpp
                    )*

                    (
                        { LA(1) == DOTDOTDOT }?
                        DOTDOTDOT
                        set_int[type_count, type_count + 1]
                    )*

                    class_post
                    (class_header | LCURLY)

                    set_type[
                        type,
                        CLASS_DEFN,
                        type == CLASS_DECL
                        && (
                            LA(1) == LCURLY
                            || lcurly
                        )
                    ]

                    set_type[
                        type,
                        STRUCT_DEFN,
                        type == STRUCT_DECL
                        && (
                            LA(1) == LCURLY
                            || lcurly
                        )
                    ]

                    set_type[
                        type,
                        UNION_DEFN,
                        type == UNION_DECL
                        && (
                            LA(1) == LCURLY
                            || lcurly
                        )
                    ]

                    set_type[
                        type,
                        INTERFACE_DEFN,
                        type == INTERFACE_DECL
                        && (
                            LA(1) == LCURLY
                            || lcurly
                        )
                    ]

                    set_type[
                        type,
                        NONE,
                        !(
                            LA(1) == TERMINATE
                            || LA(1) == COMMA 
                            || LA(1) == LCURLY
                            || lcurly
                        )
                        || (is_record && type == CLASS_DECL)
                    ]

                    throw_exception[type != NONE]

                    set_bool[foundpure]
                    set_int[type_count, type_count + 1] |

                    { type_count == attribute_count + specifier_count + template_count }?
                    (ENUM set_type[type, ENUM_DECL])

                    set_bool[lcurly, LA(1) == LCURLY]

                    (options { greedy = true; } :
                        { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
                        attribute_cpp
                    )*

                    (
                        { LA(1) == DOTDOTDOT }?
                        DOTDOTDOT
                        set_int[type_count, type_count + 1]
                    )*

                    (
                        { inLanguage(LANGUAGE_JAVA) }?
                        class_header |

                        { inLanguage(LANGUAGE_CSHARP) }?
                        variable_identifier
                        (derived_list)* |

                        enum_class_header |

                        LCURLY
                    )

                    set_type[
                        type,
                        ENUM_DEFN,
                        type == ENUM_DECL
                        && (
                            LA(1) == LCURLY
                            || lcurly
                        )
                    ]

                    set_type[
                        type,
                        NONE,
                        !(
                            LA(1) == TERMINATE
                            || LA(1) == COMMA
                            || LA(1) == LCURLY
                            || lcurly
                        )
                    ]

                    throw_exception[type != NONE]

                    set_bool[foundpure]
                    set_int[type_count, type_count + 1] |

                    (
                        USING
                        set_type[type, USING_STMT]
                        throw_exception[true]
                    ) |

                    { inLanguage(LANGUAGE_JAVA_FAMILY) }?
                    generic_argument_list
                    set_int[specifier_count, specifier_count + 1] |

                    { inLanguage(LANGUAGE_JAVA_FAMILY) }?
                    annotation
                    set_int[attribute_count, attribute_count + 1] |

                    // typical type name
                    { !inLanguage(LANGUAGE_CSHARP) || LA(1) != ASYNC }?
                    set_bool[operatorname, false]
                    compound_name set_bool[foundpure]
                    set_bool[isoperator, isoperator || (inLanguage(LANGUAGE_CXX_FAMILY) && operatorname)]
                    set_bool[operatorname, false] |

                    // always count as a name for now since is always used as a type or type modifier
                    auto_keyword[false] |

                    EVENT
                    set_bool[is_event] |

                    // special function name
                    MAIN
                    set_bool[ismain, type_count == 0] |

                    { is_c_class_identifier || keyword_name_token_set.member(next_token()) }?
                    keyword_name |

                    { inLanguage(LANGUAGE_JAVA) && inMode(MODE_PARAMETER) }?
                    bar |

                    // type parts that can occur before other type parts (excluding specifiers)
                    // do not match a struct, class, or union; if was class/struct/union, decl will not reach here
                    // if elaborated type specifier should also be handled above; if reached here because non-specifier, then class/struct/union
                    {
                        LA(1) != LBRACKET
                        && (
                            LA(1) != CLASS
                            && LA(1) != CXX_CLASS
                            && LA(1) != STRUCT
                            && LA(1) != UNION
                        )
                    }?
                    (
                        { LA(1) == DECLTYPE }?
                        type_specifier_call |

                        { next_token() == LPAREN }?
                        atomic |

                        pure_lead_type_identifier_no_specifiers
                    )
                    set_bool[foundpure] |

                    // type parts that must only occur after other type parts (excluding specifiers)
                    non_lead_type_identifier
                    throw_exception[!foundpure]
                )

                // another type part
                set_int[type_count, type_count + 1]

                // record second (before we parse it) for label detection
                set_int[token, LA(1), type_count == 1]
            )*

            // special case for property attributes as names, e.g., get, set, etc.
            throw_exception[
                type == PROPERTY_ACCESSOR
                && (type_count == attribute_count + specifier_count + 1)
                && LA(1) == LCURLY
            ]
            set_type[type, PROPERTY_ACCESSOR_DECL, type == PROPERTY_ACCESSOR]

            throw_exception[
                type == PROPERTY_ACCESSOR_DECL
                && (type_count == attribute_count + specifier_count + 1)
                && LA(1) == TERMINATE
            ]
            set_type[type, NONE, type == PROPERTY_ACCESSOR_DECL]

            set_int[real_type_count, type_count]

            // special case for ternary operator on its own
            throw_exception[LA(1) == COLON && is_qmark]

            // adjust specifier tokens to account for keyword async used as name (C# only)
            set_int[specifier_count, token == ASYNC ? specifier_count - 1 : specifier_count]

            // adjust type tokens to eliminate for last left bracket (Java only)
            set_int[type_count, endbracket ? type_count - 1 : type_count]

            // have a sequence of type tokens, last one is function/variable name (except for function pointer, which is handled later)
            // using also has no name so counter operation
            set_int[type_count, inMode(MODE_USING) ? type_count + 1 : type_count]

            set_int[
                type_count,
                type_count > 1
                && inLanguage(LANGUAGE_CXX)
                && parameter_pack_pos >= 0
                && parameter_pack_pos == (type_count - 1)
                ? type_count + 1 : type_count
            ]

            set_int[type_count, type_count > 1 ? type_count - 1 : 0]

            // special case for what looks like a destructor declaration
            throw_exception[
                isdestructor
                && (
                    modifieroperator
                    || (type_count - specifier_count - attribute_count - template_count) > 1
                    || ((type_count - specifier_count - attribute_count - template_count) == 1)
                )
            ]

            // check if an event
            set_type[type, EVENT_STMT, is_event]
            throw_exception[is_event]

            // check if a property
            set_type[
                type,
                PROPERTY_STMT,
                inLanguage(LANGUAGE_CSHARP)
                && (type_count - specifier_count) > 0
                && LA(1) == LCURLY
            ]
            throw_exception[type == PROPERTY_STMT]

            /*
              We have a declaration (at this point a variable) if we have:
              - At least one non-specifier in the type
              - There is nothing in the type (what was the name is the type) and it is part of a parameter list

              For now, attribute and template counts are left out on purpose.
            */
            set_type[
                type,
                VARIABLE,
                (
                    (
                        (
                            (type_count - specifier_count - template_count) > 0
                            && LA(1) != OPERATORS
                            && LA(1) != CSPEC
                            && LA(1) != MSPEC
                            && (
                                (
                                    inLanguage(LANGUAGE_CXX)
                                    && !inMode(MODE_ACCESS_REGION)
                                )
                                || LA(1) == 1
                                || LA(1) == TERMINATE
                                || LA(1) == COMMA
                                || LA(1) == BAR
                                || LA(1) == LBRACKET
                                || (
                                    LA(1) == LPAREN
                                    && next_token() != RPAREN
                                )
                                || LA(1) == LCURLY
                                || LA(1) == EQUAL
                                || LA(1) == IN
                                || (
                                    (
                                        inTransparentMode(MODE_CONTROL_CONDITION)
                                        || inLanguage(LANGUAGE_C)
                                        || inLanguage(LANGUAGE_CXX)
                                    )
                                    && LA(1) == COLON
                                )
                                || (
                                    inLanguage(LANGUAGE_CSHARP)
                                    && LA(1) == RBRACKET
                                )
                            )
                        )
                    )
                    || (
                        inparam
                        && (
                            LA(1) == RPAREN
                            || LA(1) == COMMA
                            || LA(1) == BAR
                            || LA(1) == LBRACKET
                            || LA(1) == EQUAL
                            || LA(1) == IN
                            || (
                                inLanguage(LANGUAGE_CSHARP)
                                && LA(1) == RBRACKET
                            )
                        )
                    )
                )
            ]

            // need to see if we possibly have a constructor/destructor name, with no type
            set_bool[
                isconstructor,
                // operator methods may not have non-specifier types also
                !isoperator

                && !ismain
                && !isdestructor
                && !inLanguage(LANGUAGE_OBJECTIVE_C)

                // entire type is specifiers
                && (type_count == (specifier_count + attribute_count + template_count))

                && (
                    // inside of a C++ class definition; must match class name
                    (
                        inMode(MODE_ACCESS_REGION)
                        && !class_namestack.empty()
                        && class_namestack.top() == namestack[0]
                    )

                    || (inTransparentMode(MODE_ACCESS_REGION) && inMode(MODE_TEMPLATE))

                    // directly inside the block of a Java or C# class
                    || (
                        inPrevMode(MODE_CLASS)
                        && (
                            inLanguage(LANGUAGE_JAVA_FAMILY)
                            || inLanguage(LANGUAGE_CSHARP)
                        )
                    )

                    // by itself, but has specifiers so it is not a call
                    || (
                        specifier_count > 0
                        && (
                            inLanguage(LANGUAGE_JAVA_FAMILY)
                            || inLanguage(LANGUAGE_CSHARP)
                        )
                    )

                    // outside of a class definition in C++, but with a properly prefixed name
                    || (
                        inLanguage(LANGUAGE_CXX_FAMILY)
                        && namestack[0] != ""
                        && namestack[0] == namestack[1]
                    )
                )
            ]

            // detecting a destructor name uses a data member, since it is detected in during name detection
            // if the parameters use this method, it is a reentrant, so cache it
            set_bool[saveisdestructor, isdestructor]

            // we have a declaration, but do we have a function?
            (
                (
                    // check for function pointer, which must have a non-specifier part of the type
                    {
                        (
                            inLanguage(LANGUAGE_C)
                            || inLanguage(LANGUAGE_CXX)
                        )
                        && real_type_count > 0
                    }?
                    (function_pointer_name_grammar eat_optional_macro_call LPAREN) => function_pointer_name_grammar

                    // what was assumed to be the name of the function is actually part of the type
                    set_int[type_count, type_count + 1]

                    // this is not a constructor
                    set_bool[isconstructor, false]

                    function_rest[fla] |

                    // POF (Plain Old Function)
                    // need at least one non-specifier in the type (not including the name)
                    {
                        (type_count - specifier_count - attribute_count - template_count > 0)
                        || isoperator
                        || ismain
                        || saveisdestructor
                        || isconstructor
                    }?
                    function_rest[fla]
                ) |

                {
                    real_type_count == 0
                    && specifier_count == 0
                    && attribute_count == 0
                }?
                (
                    objective_c_method
                    set_int[fla, LA(1)]
                    throw_exception[fla != TERMINATE && fla != LCURLY]
                )
            )
        
            // default to variable in function body; however, if it is an anonymous function (does not end in ":"), then it is not a variable
            throw_exception[
                (
                    inTransparentMode(MODE_FUNCTION_BODY)
                    && type == VARIABLE
                    && fla == TERMINATE
                )
                || (
                    inLanguage(LANGUAGE_JAVA)
                    && inMode(MODE_ENUM)
                    && (
                        fla == COMMA
                        || fla == TERMINATE
                    )
                )
            ]

            // since we made it this far, we have a function
            set_type[type, FUNCTION, !isoperator]
            set_type[type, OPERATOR_FUNCTION, isoperator]

            // however, we could have a destructor
            set_type[type, DESTRUCTOR, saveisdestructor]

            // however, we could also have a constructor
            set_type[
                type,
                CONSTRUCTOR,
                isconstructor
                && !saveisdestructor
                && !isoperator
                && !ismain
            ]
        )
;

/*
  check_global_attribute

  Handles a C# global attribute target.
*/
check_global_attribute[] returns [bool flag] {
        flag = LT(1)->getText() == "module"sv || LT(1)->getText() == "assembly"sv;
} :;

/*
  Utility rules

  Work even in guessing mode, where explicit code segments do not.
*/

/*
  throw_exception

  Throws an exception if the condition is true.
*/
throw_exception[bool condition = true] {
        if (condition)
            throw antlr::RecognitionException();
} :;

/*
  set_type

  Sets the declaration type to a value if the condition is true.
*/
set_type[STMT_TYPE& name, STMT_TYPE value, bool condition = true] {
        if (condition)
            name = value;
} :;

/*
  set_int

  Sets the integer to a value if the condition is true.
*/
set_int[int& name, int value, bool condition = true] {
        if (condition)
            name = value;
} :;

/*
  set_bool

  Sets the boolean to a value.
*/
set_bool[bool& variable, bool value = true] {
        variable = value;
} :;

/*
  trace
*/
trace[const char*s] {
        std::cerr << s << std::endl;
} :;

/*
  trace_int
*/
trace_int[int s] {
        std::cerr << "HERE " << s << std::endl;
} :;

// Commented-out code
/*
traceLA { std::cerr << "LA(1) is " << LA(1) << " " << LT(1)->getText() << std::endl; } :;
marker[] { CompleteElement element(this); startNewMode(MODE_LOCAL); startElement(SMARKER); } :;
*/

/*
  update_typecount

  Used to update the type count.
*/
update_typecount[srcMLState::MODE_TYPE mode] {} :
        {
            decTypeCount();

            if (inTransparentMode(MODE_ARGUMENT) && inLanguage(LANGUAGE_CXX))
                return;

            if (getTypeCount() <= 0) {
                endMode();
                setMode(mode);
            }
        }
;

/*
  type_identifier_count

  Used to count type identifiers.
*/
type_identifier_count[int& type_count] { ++type_count; ENTRY_DEBUG } :
        // overloaded parentheses operator
        { LA(1) == OPERATOR /* turns off ANTLR warning */ }?
        overloaded_operator |

        type_identifier |

        MAIN
;

/*
  type_identifier_count_check

  Checks the type identifier count.
*/
type_identifier_count_check returns [int type_count] {
        int start = mark();
        ++inputState->guessing;

        type_count = type_identifier_count_check_core();

        rewind(start);
        --inputState->guessing;
} :;

/*
  type_identifier_count_check_core

  Handles the core functionality for type_identifier_count_check.
*/
type_identifier_count_check_core returns [int type_count] { type_count = 0; ENTRY_DEBUG } :
        (type_identifier_count[type_count])*
;

// Commented-out code
/*
// --a;
deduct[int& type_count] { --type_count; } :;
*/

/*
  eat_type

  Used to consume a type.
*/
eat_type[int& count] {
        if (count <= 0 || LA(1) == BAR)
            return;

        ENTRY_DEBUG
} :
        // Mark as name before mark without name
        (options { generateAmbigWarnings = false; } : keyword_name | type_identifier)

        set_int[count, count - 1]
        eat_type[count]
;

/*
  pure_lead_type_identifier
*/
pure_lead_type_identifier[] { ENTRY_DEBUG } :
        // ambigous on template keyword from template specifier and probably class_preamble template
        (options { generateAmbigWarnings = false; } :
            // specifiers that occur in a type
            { argument_token_set.member(LA(1)) }?
            specifier |

            template_specifier | auto_keyword[true] |

            { inLanguage(LANGUAGE_CSHARP) && look_past(COMMA) == RBRACKET }?
            LBRACKET
            (COMMA)*
            RBRACKET |

            { inLanguage(LANGUAGE_JAVA) }?
            annotation |

            { inLanguage(LANGUAGE_CSHARP) }?
            attribute_csharp |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp |

            pure_lead_type_identifier_no_specifiers
        )
;

/*
  pure_lead_type_identifier_no_specifiers
*/
pure_lead_type_identifier_no_specifiers[] { ENTRY_DEBUG } :
        // class/struct/union before a name in a type, e.g., class A f();
        (options { generateAmbigWarnings = false; } :
            class_lead_type_identifier | typename_keyword |

            // enum use in a type
            { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
            (
                ENUM
                variable_identifier
                (variable_identifier | multops | tripledotop | INLINE)
            ) => ENUM |

            // entire enum definition
            { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
            enum_definition_complete |

            { LA(1) == DECLTYPE }?
            type_specifier_call |

            atomic
        )
;

/*
  class_lead_type_identifier
*/
class_lead_type_identifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            if (inTransparentMode(MODE_TEMPLATE))
                startElement(SNAME);
            else
                startElement(SNOP);
        }

        (CLASS | CXX_CLASS | STRUCT | UNION | ENUM)
;

/*
  lead_type_identifier
*/
lead_type_identifier[] { ENTRY_DEBUG } :
        // Commented-out code
        // specifier |
        // (macro_call_paren identifier)=> macro_call |

        // typical type name
        {
            LA(1) != ASYNC
            && (
                inLanguage(LANGUAGE_CXX)
                || (
                    LA(1) != FINAL
                    && LA(1) != OVERRIDE
                )
            )
            && LA(1) != CRESTRICT
            && LA(1) != MUTABLE
        }?
        compound_name |

        pure_lead_type_identifier
;

/*
  type_identifier
*/
type_identifier[] { ENTRY_DEBUG } :
        // any identifier that can appear first and can appear later as true suppresses the warning
        // antlr forms rules as LA(1) && (true), so this does nothing
        { true }?
        lead_type_identifier |

        non_lead_type_identifier
;

/*
  non_lead_type_identifier
*/
non_lead_type_identifier[] { bool iscomplex = false; ENTRY_DEBUG } :
        tripledotop |

        { inLanguage(LANGUAGE_C_FAMILY) }?
        multops |

        { inLanguage(LANGUAGE_JAVA_FAMILY) && look_past(LBRACKET) == RBRACKET }?
        variable_identifier_array_grammar_sub[iscomplex]
;

/*
  type_specifier_call
*/
type_specifier_call[] { ENTRY_DEBUG } :
        { inputState->guessing }?
        (decltype_call_full) |

        decltype_call
;

/*
  decltype_call

  Used to mark a "decltype" specifier (C++11).
*/
decltype_call[] { CompleteElement element(this); int save_type_count = getTypeCount(); ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the decltype element
            startElement(SDECLTYPE);
        }

        DECLTYPE
        complete_argument_list

        {
            setTypeCount(save_type_count);
        }
;

/*
  decltype_call_full

  Used to completely match a "decltype" without marking it up (C++).
*/
decltype_call_full[] { ENTRY_DEBUG } :
        DECLTYPE
        paren_pair
;

/*
  atomic

  Used to mark an "_Atomic" keyword (C++11).
*/
atomic[] { ENTRY_DEBUG } :
        { next_token() == LPAREN }?
        (
            { inputState->guessing }?
            atomic_call_full |

            atomic_call
        ) |

        atomic_specifier
;

/*
  atomic_specifier

  Used to mark "_Atomic" as a specifier (C++11).
*/
atomic_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        ATOMIC
;

/*
  atomic_call

  Used to mark "_Atomic" as a call (C++11).
*/
atomic_call[] { CompleteElement element(this); int save_type_count = getTypeCount(); ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the macro call element
            startElement(SATOMIC);
        }

        ATOMIC
        (options { greedy = true; } : complete_argument_list)?

        {
            setTypeCount(save_type_count);
        }
;

/*
  atomic_call_full

  Used to completely match an "_Atomic" without marking it up (C++).
*/
atomic_call_full[] { ENTRY_DEBUG } :
        ATOMIC
        (options { greedy = true; } : paren_pair)?
;

/*
  qmark_name
*/
qmark_name[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        QMARK
;

/*
  qmark_marked
*/
qmark_marked[] { bool in_ternary = inTransparentMode(MODE_TERNARY | MODE_CONDITION); LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (!(in_ternary && true))
                startElement(SOPERATOR);
        }

        QMARK
        ({ SkipBufferSize() == 0 }? QMARK)?
;

/*
  qmark
*/
qmark[] { is_qmark = true; ENTRY_DEBUG } :
        {
            if (inTransparentMode(MODE_TERNARY | MODE_CONDITION))
                endDownToMode(MODE_CONDITION);
        }

        qmark_marked

        {
            if (inTransparentMode(MODE_TERNARY | MODE_CONDITION)) {
                endMode(MODE_CONDITION);

                startNewMode(MODE_THEN | MODE_EXPRESSION | MODE_EXPECT);

                startNoSkipElement(STHEN);
            }
        }
;

/*
  linq expressions
*/

/*
  linq_expression
*/
linq_expression[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SLINQ);
        }

        linq_expression_pure
        (options { greedy = true; } : linq_expression_pure)*
;

/*
  linq_expression_pure

  Used to match linq expressions.
*/
linq_expression_pure[] { ENTRY_DEBUG } :
        linq_from | linq_where | linq_select | linq_let | linq_group | linq_join | linq_orderby
;

/*
  linq_from

  Handles a linq "from" keyword.
*/
linq_from[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SFROM);
        }

        FROM
        linq_expression_complete
        (options { greedy = true; } : linq_in)*
;

/*
  linq_in

  Handles a linq "in" keyword.
*/
linq_in[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SIN);
        }

        IN
        linq_expression_complete
;

/*
  linq_where

  Hanles a linq "where" keyword.
*/
linq_where[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SWHERE);
        }

        WHERE
        linq_expression_complete
;

/*
  linq_select

  Handles a linq "select" keyword.
*/
linq_select[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SSELECT);
        }

        SELECT
        linq_expression_complete
;

/*
  linq_let

  Handles a linq "let" keyword.
*/
linq_let[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SLET);
        }

        LET
        linq_expression_complete
;

/*
  linq_group

  Handles a linq "group" keyword.
*/
linq_group[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SGROUP);
        }

        GROUP
        linq_expression_complete
        (options { greedy = true; } : linq_by)*
        (options { greedy = true; } : linq_into)*
;

/*
  linq_by

  Handles a linq "by" keyword.
*/
linq_by[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SBY);
        }

        BY
        linq_expression_complete
;

/*
  linq_into

  Handles a linq "into" keyword.
*/
linq_into[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SINTO);
        }

        INTO
        linq_expression_complete
;

/*
  linq_join

  Handles a linq "join" keyword.
*/
linq_join[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SJOIN);
        }

        JOIN
        linq_expression_complete
        (options { greedy = true; } : linq_in | linq_on | linq_equals | linq_into)*
;

/*
  linq_on

  Handles a linq "on" keyword.
*/
linq_on[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SON);
        }

        ON
        linq_expression_complete
;

/*
  linq_equals

  Handles a linq "equals" keyword.
*/
linq_equals[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SEQUALS);
        }

        EQUALS
        linq_expression_complete
;

/*
  linq_orderby

  Handles a linq "orderby" keyword.
*/
linq_orderby[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SORDERBY);
        }

        ORDERBY
        linq_expression_complete
        (options { greedy = true; } : linq_ascending | linq_descending)*

        (options { greedy = true; } :
            COMMA
            linq_expression_complete
            (options { greedy = true; } : linq_ascending | linq_descending)*
        )*
;

/*
  linq_ascending

  Handles a linq "ascending" keyword.
*/
linq_ascending[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        ASCENDING
;

/*
  linq_descending

  Handles a linq "descending" keyword.
*/
linq_descending[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        DESCENDING
;

/*
  variable_identifier_array_grammar_sub

  Handles a variables array index.
*/
variable_identifier_array_grammar_sub[bool& iscomplex] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            iscomplex = true;

            // start a mode to end at right bracket with expressions inside
            if (
                inLanguage(LANGUAGE_CSHARP)
                || (
                    LA(1) == ATLBRACKET
                    && inLanguage(LANGUAGE_OBJECTIVE_C)
                )
            )
                startNewMode(MODE_LOCAL | MODE_TOP | MODE_LIST | MODE_END_AT_COMMA);
            else
                startNewMode(MODE_LOCAL | MODE_TOP | MODE_LIST);

            startElement(SINDEX);
        }

        (LBRACKET | ATLBRACKET)
        variable_identifier_array_grammar_sub_contents
        RBRACKET
;

/*
  variable_identifier_array_grammar_sub_contents

  Handles the contents of a variables array index.
*/
variable_identifier_array_grammar_sub_contents { bool found_expr = false; bool is_expr = false; ENTRY_DEBUG } :
        { !inLanguage(LANGUAGE_CSHARP) && !inLanguage(LANGUAGE_OBJECTIVE_C) }?
        complete_expression |

        { inLanguage(LANGUAGE_CSHARP) || inLanguage(LANGUAGE_OBJECTIVE_C) }?
        (options { greedy = true; } :
            { LA(1) != RBRACKET }?
            (
                { LA(1) == COMMA /* stop warning */ }?
                {
                    if (!found_expr) {
                        empty_element(SEXPRESSION, true);
                    }
                }
                COMMA
                {
                    found_expr = false;
                } |

                complete_expression
                {
                    found_expr = true;
                }
            )

            set_bool[is_expr, true]
        )*

        {
            if (is_expr && !found_expr) {
                empty_element(SEXPRESSION, true);
            }
        }
;

/*
  attribute_csharp

  Handles a C# attribute.
*/
attribute_csharp[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_END_AT_COMMA);

            startElement(SATTRIBUTE);
        }

        LBRACKET

        // do not warn; identifier list and colon are in complete expression as well, but need special processing here
        (options { warnWhenFollowAmbig = false; } :
            { next_token() == COLON }?
            attribute_csharp_target
            COLON
        )*

        attribute_inner_list
        RBRACKET
;

/*
  attribute_csharp_target

  Handles a target for a C# attribute.
*/
attribute_csharp_target[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(STARGET);
        }

        (RETURN | EVENT | identifier_list)
;

/*
  attribute_inner_list

  Handles the inner portion of an attribute list.
*/
attribute_inner_list[] { ENTRY_DEBUG } :
        complete_expression
        (COMMA complete_expression)*
;

/*
  attribute_cpp

  Handles a C++11 attribute.
*/
attribute_cpp[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_END_AT_COMMA);

            startElement(SATTRIBUTE);
        }

        LBRACKET
        LBRACKET

        attribute_inner_list

        RBRACKET
        RBRACKET
;

/*
  complete_argument_list

  Used to match an argument list completely.
*/
complete_argument_list[] { ENTRY_DEBUG } :
        call_argument_list

        (options { greedy = true; } :
            { LA(1) != RPAREN && LA(1) != RCURLY }?
            complete_arguments
        )*

        rparen[false]
;

/*
  complete_arguments

  Matches a full, complete expression all at once (no stream).
*/
complete_arguments[] {
        CompleteElement element(this);
        int count_paren = 1;
        CALL_TYPE type = NOCALL;
        bool isempty = false;
        int call_count = 0;

        ENTRY_DEBUG
} :
        { getParen() == 0 }?
        rparen[false] |

        { getCurly() == 0 }?
        rcurly_argument |

        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        }

        (options { warnWhenFollowAmbig = false; } :
            {
                count_paren > 0
                && (
                    count_paren != 1
                    || LA(1) != RPAREN
                )
            }?
            (options { generateAmbigWarnings = false; } :
                { LA(1) == LPAREN }?
                expression
                {
                    ++count_paren;
                } |

                { LA(1) == RPAREN }?
                expression
                {
                    --count_paren;
                } |

                { perform_call_check(type, isempty, call_count, -1) && type == CALL }?
                {
                    if (!isempty) {
                        ++count_paren;
                    }
                }
                expression_process
                (call[call_count] | keyword_calls)
                complete_arguments |

                expression |

                (type_identifier) => expression_process type_identifier |

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

/*
  complete_default_parameter

  Matches a full, complete expression all at once (no stream).  Might be a better version of complete_expression.
*/
complete_default_parameter[] {
        CompleteElement element(this);
        int count_paren = 0;
        CALL_TYPE type = NOCALL;
        bool isempty = false;
        int call_count = 0;

        ENTRY_DEBUG
} :
        { getParen() == 0 }?
        rparen[false] |

        { getCurly() == 0 }?
        rcurly_argument |

        {
            // argument with nested expression
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }

        (options {warnWhenFollowAmbig = false; } :
            {
                (
                    LA(1) != RPAREN
                    && LA(1) != COMMA
                )
                || count_paren > 0
            }?
            (
                { LA(1) == LPAREN }?
                expression
                set_int[count_paren, count_paren + 1] |

                { LA(1) == RPAREN && inputState->guessing }?
                rparen
                set_int[count_paren, count_paren - 1] |

                { LA(1) == RPAREN && !inputState->guessing }?
                expression
                set_int[count_paren, count_paren - 1] |

                { perform_call_check(type, isempty, call_count, -1) && type == CALL }?
                set_int[count_paren, isempty ? count_paren : count_paren + 1]
                expression |

                expression |

                comma
            )
        )*
;

/*
  complete_objective_c_call

  Matches a complete Objective-C call (no stream).
*/
complete_objective_c_call[] { CompleteElement element(this); int bracket_count = 0; ENTRY_DEBUG } :
        { inputState->guessing }?
        bracket_pair |

        {
            // start a mode to end at right bracket with expressions inside
            if (!inMode(MODE_EXPRESSION) || inMode(MODE_EXPRESSION | MODE_EXPECT))
                startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
            else 
                startNewMode(MODE_TOP);
        }

        (options { greedy = true; } :
            // end of objective c call
            { inTransparentMode(MODE_OBJECTIVE_C_CALL) && bracket_count }?
            rbracket
            set_int[bracket_count, bracket_count - 1] |

            // objective c argument list
            { LA(1) == LBRACKET }?
            expression
            set_int[bracket_count, bracket_count + 1] |

            // objective c argument list
            { inTransparentMode(MODE_OBJECTIVE_C_CALL | MODE_ARGUMENT_LIST) }?
            (function_identifier (COLON | RBRACKET)) => objective_c_call_message |

            // objective c argument
            { inTransparentMode(MODE_OBJECTIVE_C_CALL) }?
            (
                function_identifier
                (COLON | RBRACKET) |
                
                COLON
            ) => objective_c_call_argument |

            // commas as in a list
            { inTransparentMode(MODE_END_ONLY_AT_RPAREN) || !inTransparentMode(MODE_END_AT_COMMA) }?
            comma |

            // right parenthesis, unless we are in a pair of parentheses in an expression
            { !inTransparentMode(MODE_INTERNAL_END_PAREN) }?
            rparen[false] |

            // argument mode (as part of call)
            { inMode(MODE_ARGUMENT) }?
            argument |

            // expression with right parentheses if a previous match is in one
            { LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN) }?
            expression |

            colon_marked
        )*
;

/*
  complete_expression

  Matches a complete expression (no stream).
*/
complete_expression[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }

        (options { greedy = true; } :
            // commas as in a list
            {
                (
                    inTransparentMode(MODE_END_ONLY_AT_RPAREN)
                    && (getFirstMode(MODE_END_ONLY_AT_RPAREN | MODE_END_AT_COMMA) & MODE_END_AT_COMMA) == 0
                )
                || !inTransparentMode(MODE_END_AT_COMMA)
            }?
            comma |

            // right parenthesis, unless we are in a pair of parentheses in an expression
            { !inTransparentMode(MODE_INTERNAL_END_PAREN) }?
            rparen[false] |

            { inLanguage(LANGUAGE_OBJECTIVE_C) && LA(1) == LBRACKET }?
            complete_objective_c_call |

            // argument mode (as part of call)
            { inMode(MODE_ARGUMENT) }?
            argument |

            // expression with right parentheses if a previous match is in one
            { LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN) }?
            expression |

            colon_marked
        )*
;

/*
  linq_expression_complete

  Matches a linq expression completely.
*/
linq_expression_complete[] { CompleteElement element(this); int count_paren = 0; ENTRY_DEBUG } :
        {
            // start a mode to end at right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }

        (options { warnWhenFollowAmbig = false; } :
            { LA(1) != RPAREN || count_paren > 0 }?
            {
                (
                    LA(1) != RPAREN
                    || count_paren > 0
                )
                && try_linq_expression_complete_inner(count_paren)
            }?
            linq_expression_complete_inner[count_paren, true]
        )*
;

/*
  try_linq_expression_complete_inner
*/
try_linq_expression_complete_inner[int& count_paren] returns [bool success = false] {
        int start = mark();
        ++inputState->guessing;

        try {
            linq_expression_complete_inner(count_paren);
            success = true;
        } catch (antlr::RecognitionException& e) {
            success = false;
        }

        rewind(start);
        --inputState->guessing;
} :;

/*
  linq_expression_complete_inner
*/
linq_expression_complete_inner[int& count_paren, bool update = false] {
        CALL_TYPE type = NOCALL;
        bool isempty = false;
        int call_count = 0;

        ENTRY_DEBUG
} :
        // commas as in a list
        comma |

        // right parenthesis, unless we are in a pair of parentheses in an expression
        { LA(1) == LPAREN }?
        expression_setup_linq
        ({ update }? set_int[count_paren, count_paren + 1])? |

        { LA(1) == RPAREN && inputState->guessing }?
        rparen
        ({ update }? set_int[count_paren, count_paren - 1])? |

        { LA(1) == RPAREN && !inputState->guessing }?
        expression_setup_linq
        ({ update }? set_int[count_paren, count_paren - 1])? |

        { perform_call_check(type, isempty, call_count, -1) && type == CALL }?
        ({ update }? set_int[count_paren, isempty ? count_paren : count_paren + 1])?
        expression_setup_linq |

        // argument mode (as part of call)
        { inMode(MODE_ARGUMENT) }?
        argument |

        // expression with right parentheses if a previous match is in one
        {
            LA(1) != ASCENDING
            && LA(1) != DESCENDING
            && LA(1) != ON
            && LA(1) != BY
            && LA(1) != FROM
            && LA(1) != SELECT
            && LA(1) != LET
            && LA(1) != WHERE
            && LA(1) != ORDERBY
            && LA(1) != GROUP
            && LA(1) != JOIN
            && LA(1) != IN
            && LA(1) != EQUALS
            && LA(1) != INTO
            && (
                LA(1) != RPAREN
                || inTransparentMode(MODE_INTERNAL_END_PAREN)
            )
        }?
        expression_setup_linq |

        COLON
;

/*
  variable_identifier

  Handles a variable name in an expression.  Includes array names, but not function calls.
*/
variable_identifier[] { ENTRY_DEBUG } :
        compound_name
;

/*
  simple_name_optional_template

  Handles a name (including a template argument list).
*/
simple_name_optional_template[bool push = true] { CompleteElement element(this); TokenPosition tp; ENTRY_DEBUG } :
        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            // start outer name
            startElement(SCNAME);

            // record the name token so we can replace it if necessary
            setTokenPosition(tp);
        }

        push_namestack[push]
        identifier

        (
            {
                inLanguage(LANGUAGE_CXX_FAMILY)
                || inLanguage(LANGUAGE_JAVA_FAMILY)
                || inLanguage(LANGUAGE_OBJECTIVE_C)
            }?
            { generic_argument_list_check() }?
            (generic_argument_list) => generic_argument_list
            /* Commented-out code: (options { greedy = true; } : generic_type_constraint)* */ |

            (cuda_argument_list) => cuda_argument_list |

            {
                // set the token to NOP since we did not find a template argument list
                tp.setType(SNOP);
            }
        )
;

/*
  simple_name_optional_template_destop

  Handles a name (including a template argument list).  Used for destructor identifiers.
*/
simple_name_optional_template_destop[bool push = true] { CompleteElement element(this); TokenPosition tp; ENTRY_DEBUG } :
        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            // start outer name
            startElement(SCNAME);

            // record the name token so we can replace it if necessary
            setTokenPosition(tp);
        }

        identifier_destop[push]

        (
            {
                inLanguage(LANGUAGE_CXX_FAMILY)
                || inLanguage(LANGUAGE_JAVA_FAMILY)
                || inLanguage(LANGUAGE_OBJECTIVE_C)
            }?
            { generic_argument_list_check() }?
            (generic_argument_list) => generic_argument_list
            /* Commented-out code: (options { greedy = true; } : generic_type_constraint)* */ |

            (cuda_argument_list) => cuda_argument_list |

            {
                // set the token to NOP since we did not find a template argument list
                tp.setType(SNOP);
            }
        )
;

/*
  simple_name_optional_template_optional_specifier

  Handles a name (including a template argument list).  Used for optional specifiers.
*/
simple_name_optional_template_optional_specifier[bool push = true] { CompleteElement element(this); TokenPosition tp; ENTRY_DEBUG } :
        push_namestack[push]
        (template_specifier /* Commented-out code: { is_nop = false; } */ )*
        identifier

        (
            { generic_argument_list_check() }?
            (generic_argument_list) => generic_argument_list
            (options { greedy = true; } : generic_type_constraint)* |

            (cuda_argument_list) => cuda_argument_list |
        )
;

/*
  simple_name_optional_template_optional_specifier_destop

  Handles a name (including a template argument list).  Used for optional specifiers with destructor identifiers.
*/
simple_name_optional_template_optional_specifier_destop[bool push = true] {
        CompleteElement element(this);
        TokenPosition tp;
        bool is_nop = true;

        ENTRY_DEBUG
} :
        {
            // local mode that is automatically ended by leaving this function
            startNewMode(MODE_LOCAL);

            // start outer name
            startElement(SCNAME);

            // record the name token so we can replace it if necessary
            setTokenPosition(tp);
        }

        identifier_optional_specifier_destop[push, is_nop]

        (
            { generic_argument_list_check() }?
            (generic_argument_list) => generic_argument_list
            (options { greedy = true; } : generic_type_constraint)* |

            (cuda_argument_list) => cuda_argument_list |

            {
                // set the token to NOP since we did not find a template argument list
                if (is_nop)
                    tp.setType(SNOP);
            }
        )
;

/*
  identifier_destop

  Handles a destructor identifier.
*/
identifier_destop[bool push = true] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        DESTOP
        push_namestack[push]
        identifier_list
;

/*
  identifier_optional_specifier_destop

  Handles an optional specifier with destructor identifiers.
*/
identifier_optional_specifier_destop[bool push, bool& is_nop] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        DESTOP
        push_namestack[push]
        (template_specifier { is_nop = false; })*
        identifier_list
;

/*
  identifier

  Handles an identifier.
*/
identifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        identifier_list
;

/*
  identifier_list

  Handles the list of identifiers that are also marked up as tokens for other things.
*/
identifier_list[] { ENTRY_DEBUG } :
        NAME | INCLUDE | DEFINE | ELIF | ENDIF | ERRORPREC | IFDEF | IFNDEF | LINE | PRAGMA | UNDEF |
        WARNING | SUPER | REGION | ENDREGION | GET | SET | ADD | REMOVE | ASYNC | YIELD | FINAL |
        OVERRIDE | VOID | ASM |

        // C# linq
        FROM | WHERE | SELECT | LET | ORDERBY | ASCENDING | DESCENDING | GROUP | BY | JOIN | ON |
        EQUALS | INTO | THIS | ALIAS |

        // Objective-C
        IMPORT | ATPROTOCOL |

        // C
        CRESTRICT | MUTABLE | CXX_TRY | CXX_CATCH |

        // Java
        RECORD | SEALED | PERMITS | NON |

        // Commented-out code; Not sure why these are commented out
        /*
        CXX_CLASS | THROW | CLASS | PUBLIC | PRIVATE | PROTECTED | NEW | VIRTUAL | FRIEND | OPERATOR |
        EXPLICIT | NAMESPACE | USING | DELETE | LITERAL_FALSE | LITERAL_TRUE | FINAL | OVERRIDE |
        CONSTEXPR | NOEXCEPT | THREADLOCAL | NULLPTR | DECLTYPE | ALIGNAS | TYPENAME | ALIGNOF |
        */

        // Qt
        EMIT | FOREACH | SIGNAL | FOREVER
;

/*
  simple_identifier

  Handles the most basic form of a name.
*/
simple_identifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        (NAME | VOID)
;

/*
  typename_keyword
*/
typename_keyword[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            if (!inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST))
                startElement(STYPENAME);
            else
                startElement(SNAME);
        }

        TYPENAME
;

/*
  function_pointer_name_check
*/
function_pointer_name_check[] returns [bool is_fp_name = false] {
        if (
            LA(1) == LPAREN
            && (
                inLanguage(LANGUAGE_C)
                || inLanguage(LANGUAGE_CXX)
            )
        ) {
            ++inputState->guessing;
            int start = mark();

            try {
                function_pointer_name_grammar();

                is_fp_name =
                    LA(1) == PERIOD
                    || LA(1) == TRETURN
                    || (
                        inLanguage(LANGUAGE_CXX)
                        && (
                            LA(1) == MPDEREF
                            || LA(1) == DOTDEREF
                        )
                    );
            } catch (...) {}

            rewind(start);
            --inputState->guessing;
        }

        ENTRY_DEBUG
} :;

/*
  function_pointer_name
*/
function_pointer_name[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SNAME);
        }

        pointer_dereference
        (period | member_pointer | member_pointer_dereference | dot_dereference)

        (
            { function_pointer_name_check() }?
            pointer_dereference
            (period | member_pointer | member_pointer_dereference | dot_dereference)
        )*

        compound_name_inner[false]
;

/*
  pointer_dereference
*/
pointer_dereference[] { ENTRY_DEBUG bool flag = false; } :
        lparen_marked

        // special case for function pointer names that do not have a '*'
        (
            { macro_call_token_set.member(LA(1)) }?
            (compound_name_inner[false])* |

            // special name prefix of namespace or class
            identifier
            (generic_argument_list (generic_type_constraint)*)*
            DCOLON
            pointer_dereference |

            // typical function pointer name; need greedy for general operators and possibly end
            general_operators
            (options { greedy = true; } : general_operators)*
            (options { greedy = true; } : compound_name_inner[false])*

            // optional array declaration
            (variable_identifier_array_grammar_sub[flag])*
        )

        rparen[true]
;

/*
  compound_name

  Used to mark names.
*/
compound_name[] { CompleteElement element(this); bool iscompound = false; ENTRY_DEBUG } :
        compound_name_inner[true]

        (options { greedy = true; } :
            { (!inLanguage(LANGUAGE_CXX) || next_token() != LBRACKET) }?
            variable_identifier_array_grammar_sub[iscompound] |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp
        )*
;

/*
  compound_name_inner

  Used to mark the inner portion of a name.

  There is a problem detecting complex names from complex names of operator methods in namespaces or classes for implicit casting
  (e.g., A::operator String // () {}).  Detecting before here means lookahead on all A::B::... names, causing a slowdown of
  almost 20%.  The solution ("hack") is to start all complex names as operator methods, then replace by NOP if not.
*/
compound_name_inner[bool index] { CompleteElement element(this); TokenPosition tp; bool iscompound = false; ENTRY_DEBUG } :
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

        (
            { inLanguage(LANGUAGE_JAVA_FAMILY) }?
            compound_name_java[iscompound] |

            { inLanguage(LANGUAGE_CSHARP) }?
            compound_name_csharp[iscompound] |

            { inLanguage(LANGUAGE_OBJECTIVE_C) }?
            compound_name_objective_c[iscompound] |

            { inLanguage(LANGUAGE_C) }?
            compound_name_c[iscompound] |

            { inLanguage(LANGUAGE_CXX) }?
            compound_name_cpp[iscompound] |

            macro_type_name_call
        )

        (options { greedy = true; } :
            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp
        )*

        (options { greedy = true; } :
            {
                index
                /* Commented-out code: && !inTransparentMode(MODE_EAT_TYPE) */
                && (
                    !inLanguage(LANGUAGE_CXX)
                    || next_token() != LBRACKET
                )
            }?
            variable_identifier_array_grammar_sub[iscompound]
        )*

        {
            // if it is not a compound name, nop the element
            if (!iscompound)
                // set the token to NOP
                tp.setType(SNOP);
        }
;

/*
  multops_star
*/
multops_star[] { ENTRY_DEBUG } :
        (options { greedy = true; } : multops)*
;

/*
  compound_name_cpp

  Handles a compound name (C++).
*/
compound_name_cpp[bool& iscompound] { namestack.fill(""); bool iscolon = false; ENTRY_DEBUG } :
        (options { greedy = true; } :
            { !in_template_param }?
            typename_keyword
            {
                iscompound = true;
            }
        )*

        (
            dcolon
            {
                iscompound = true;
            }
        )*

        (
            set_bool[isdestructor]
            { /* Commented-out code: iscompound = true; */ }
            simple_name_optional_template_destop |

            typename_keyword |

            simple_name_optional_template | 

            push_namestack
            overloaded_operator
        )

        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multops)*

        // "a::" causes an exception to be thrown
        (options { greedy = true; } :
            (
                { !modifier_tokens_set.member(last_consumed) }?
                dcolon
                set_bool[iscolon, true]
                {
                    iscompound = true;
                } |

                (period | member_pointer | member_pointer_dereference | dot_dereference)
                clearnamestack
                {
                    iscompound = true;
                }
            )

            (options { greedy = true; } : dcolon)*

            (
                set_bool[isdestructor]
                simple_name_optional_template_optional_specifier_destop[iscolon] |

                (multops)*

                (
                    simple_name_optional_template_optional_specifier[iscolon] |

                    push_namestack
                    overloaded_operator |

                    function_identifier_main |

                    keyword_identifier
                )
            )

            // Commented-out code: (options { greedy = true; } : { look_past_rule(&srcMLParser::multops_star) == DCOLON }? multops)*
        )*

        {
            notdestructor = LA(1) == DESTOP;
        }
;
exception
catch[antlr::RecognitionException&] {}

/*
  compound_name_csharp

  Handles a compound name (C#).
*/
compound_name_csharp[bool& iscompound] { namestack.fill(""); ENTRY_DEBUG } :
        (modifiers_csharp)*

        (
            dcolon
            {
                iscompound = true;
            }
        )*

        (
            set_bool[isdestructor]
            { /* Commented-out code: iscompound = true; */ }
            simple_name_optional_template_destop |

            simple_name_optional_template |

            push_namestack
            overloaded_operator
        )

        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multops)*

        // "a::" causes an exception to be thrown
        (options { greedy = true; } :
            (
                { !modifier_tokens_set.member(last_consumed) }?
                dcolon
                {
                    iscompound = true;
                } |

                (period | member_pointer)
                {
                    iscompound = true;
                }
            )

            (options { greedy = true; } : dcolon)*

            (
                set_bool[isdestructor]
                simple_name_optional_template_destop |

                (multops)*

                (
                    simple_name_optional_template |

                    push_namestack
                    overloaded_operator |

                    function_identifier_main
                )
            )

            // Commented-out code: (options { greedy = true; } : { look_past_rule(&srcMLParser::multops_star) == DCOLON }? multops)*
        )*
;
exception
catch[antlr::RecognitionException&] {}

/*
  compound_name_c

  Handles a compound name (C).
*/
compound_name_c[bool& iscompound] { ENTRY_DEBUG } :
        (simple_name_optional_template | generic_selection)

        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multopblockop)*

        (options { greedy = true; } :
            (period | member_pointer)

            {
                iscompound = true;
            }

            (multopblockop)*
            identifier
        )*
;

/*
  compound_name_objective_c

  Handles a compound name (Objective-C).
*/
compound_name_objective_c[bool& iscompound] { ENTRY_DEBUG } :
        (simple_name_optional_template | generic_selection)

        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multopblockop)*

        (options { greedy = true; } :
            (period | member_pointer)

            {
                iscompound = true;
            }

            (multopblockop)*
            simple_name_optional_template
        )*
;

/*
  compound_name_java

  Handles a compound name (Java).
*/
compound_name_java[bool& iscompound] { ENTRY_DEBUG } :
        generic_argument_list | simple_name_optional_template

        (options { greedy = true; } :
            (
                period

                {
                    iscompound = true;
                }

                (
                    keyword_name |

                    simple_name_optional_template |

                    { next_token() == TERMINATE }?
                    multop_name
                )
            )
        )*
;

/*
  keyword_name
*/
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

        (options { greedy = true; } :
            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp
        )*

        (options { greedy = true; } :
            {
                /* Commented-out code: !inTransparentMode(MODE_EAT_TYPE) && */
                (
                    !inLanguage(LANGUAGE_CXX)
                    || next_token() != LBRACKET
                )
            }?
            variable_identifier_array_grammar_sub[iscompound]
        )*

        {
            // if it is not a compound name, nop the element
            if (!iscompound)
                // set the token to NOP
                tp.setType(SNOP);
        }
;

/*
  keyword_name_inner

  Handles a compound name (C++).
*/
keyword_name_inner[bool& iscompound] { namestack.fill(""); ENTRY_DEBUG } :
        // Commented-out code: (options { greedy = true; } : dcolon)*
        // Commented-out code: (DESTOP set_bool[isdestructor] { iscompound = true; })*

        keyword_identifier

        (options { greedy = true; } : { !inTransparentMode(MODE_EXPRESSION) }? multops)*

        // "a::" causes an exception to be thrown
        (options { greedy = true; } :
            (
                dcolon
                {
                    iscompound = true;
                } |

                (period | member_pointer | member_pointer_dereference | dot_dereference)
                {
                    iscompound = true;
                }
            )

            (options { greedy = true; } : dcolon)*

            (DESTOP set_bool[isdestructor])*

            (multops)*

            (
                simple_name_optional_template_optional_specifier |
                push_namestack overloaded_operator |
                function_identifier_main |
                keyword_identifier
            )

            (options { greedy = true; } :
                { look_past_rule(&srcMLParser::multops_star) == DCOLON }?
                multops
            )*
        )*

        {
            notdestructor = LA(1) == DESTOP;
        }
;
exception
catch[antlr::RecognitionException&] {}

/*
  keyword_identifier

  Handles a keyword identifier.
*/
keyword_identifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        (CLASS | CXX_CLASS)
;

/*
  function_specifier

  Handles a specifier for a function.
*/
function_specifier[] { CompleteElement element(this); ENTRY_DEBUG } :
        { LA(1) == WHERE }?
        generic_type_constraint |

        (
            { LA(1) != ASYNC }?
            specifier |

            // pure virtual specifier, = default, and = delete
            EQUAL
            (literal | function_equal_specifier) |

            simple_name_optional_template
        )
;

/*
  function_equal_specifier

  Handles "=delete" and "=default" in a function declaration.
*/
function_equal_specifier[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            // only markup strings in literal option
            startElement(SFUNCTION_SPECIFIER);
        }

        (DEFAULT | DELETE)
;

/*
  specifier

  Used to mark specifiers.
*/
specifier[] { ENTRY_DEBUG } :
        single_keyword_specifier | alignas_specifier | macro_specifier_call
;

/*
  single_keyword_specifier

  Used to match a single word specifier.
*/
single_keyword_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        (
            // access
            PUBLIC | PRIVATE | PROTECTED |

            // C++
            FINAL | STATIC | ABSTRACT |
            { inLanguage(LANGUAGE_CSHARP) }? NEW |
            MUTABLE | CONSTEXPR | THREAD_LOCAL |

            // C
            REGISTER | RESTRICT | NORETURN | COMPLEX | IMAGINARY |

            // C/C++ mode
            CRESTRICT |

            // C# and Java
            INTERNAL | SEALED | OVERRIDE | IMPLICIT | EXPLICIT | UNSAFE | READONLY | VOLATILE |
            DELEGATE | PARTIAL | ASYNC | VIRTUAL | EXTERN | INLINE | IN | PARAMS |
            { inLanguage(LANGUAGE_JAVA) }? (SYNCHRONIZED | NATIVE | STRICTFP | TRANSIENT) |

            CONST |

            // Apple
            BLOCK | WEAK | STRONG
        )
;

/*
  auto_keyword

  Handles an "auto" specifier as either a type name or specifier (C/C++).
*/
auto_keyword[bool is_specifier] { SingleElement element(this); ENTRY_DEBUG } :
        {
            if (is_specifier)
                startElement(SFUNCTION_SPECIFIER);
            else
                startElement(SNAME);
        }

        AUTO
;

/*
  template_specifier

  Used to match a "template" specifier.
*/
template_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        TEMPLATE
;

/*
  constexpr_specifier

  Used to match a "constexpr" specifier.
*/
constexpr_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        CONSTEXPR
;

/*
  alignas_specifier

  Handles an "alignas" specifier (C++11).
*/
alignas_specifier[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL | MODE_ARGUMENT);

            startElement(SALIGNAS);
        }

        ALIGNAS

        (
            { inputState->guessing }?
            paren_pair |

            complete_argument_list
        )
;

/*
  default_specifier

  Handles a "default" specifier (Java methods).
*/
default_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        DEFAULT
;

/*
  this_specifier
*/
this_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        THIS
;

non_sealed_specifier[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        NON MSPEC SEALED
;

/*
  constructor_declaration

  Handles a constructor declaration.
*/
constructor_declaration[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the constructor declaration
            startElement(SCONSTRUCTOR_DECLARATION);
        }

        constructor_header
;

/*
  constructor_definition

  Handles a constructor definition.
*/
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

/*
  constructor_header

  Handles the header portion of a constructor.
*/
constructor_header[] { ENTRY_DEBUG } :
        (options { greedy = true; } :
            { inLanguage(LANGUAGE_JAVA) }?
            annotation |

            { inLanguage(LANGUAGE_CSHARP) }?
            attribute_csharp |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp |

            specifier |

            { next_token() != TEMPOPS }?
            template_specifier |

            template_declaration_full |

            { inLanguage(LANGUAGE_JAVA_FAMILY) }?
            generic_parameter_list
        )*

        compound_name_inner[false]
        parameter_list

        {
            setMode(MODE_FUNCTION_TAIL);
        }
;

/*
  member_initialization_list

  Marks up a member initialization list.
*/
member_initialization_list[] { ENTRY_DEBUG } :
        {
            // handle member initialization list as a list of calls
            startNewMode(MODE_LIST | MODE_CALL | MODE_INITIALIZATION_LIST);

            startElement(SMEMBER_INITIALIZATION_LIST);
        }

        COLON
;

/*
  member_init

  Handles a call, function call, macro, etc.
*/
member_init[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_ARGUMENT_LIST);

            // start the function call element
            startElement(SMEMBER_INITIALIZATION);
        }

        function_identifier
        call_argument_list 
;

/*
  push_namestack

  Used to push a name onto the namestack.
*/
push_namestack[bool push = true] {
        if (!push)
            return;

        namestack[1] = std::move(namestack[0]);
        namestack[0] = LT(1)->getText();
} :;

/*
  identifier_stack
*/
identifier_stack[decltype(namestack)& s] { s[1] = std::move(s[0]); s[0] = LT(1)->getText(); ENTRY_DEBUG } :
        identifier
;

/*
  destructor_definition
*/
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

/*
  destructor_declaration
*/
destructor_declaration[] { ENTRY_DEBUG } :
        {
            // just a statement
            startNewMode(MODE_STATEMENT);

            // start the destructor declaration
            startElement(SDESTRUCTOR_DECLARATION);
        }

        destructor_header
;

/*
  destructor_header
*/
destructor_header[] { ENTRY_DEBUG } :
        (options { greedy = true; } :
            { inLanguage(LANGUAGE_JAVA) }?
            annotation |

            { inLanguage(LANGUAGE_CSHARP) }?
            attribute_csharp |

            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp |

            specifier |

            { next_token() != TEMPOPS }?
            template_specifier |

            template_declaration_full |

            { LA(1) == VOID }?
            simple_identifier
        )*

        compound_name_inner[false]
        parameter_list

        {
            setMode(MODE_FUNCTION_TAIL);
        }
;

/*
  annotation

  Handles a Java annotation.
*/
annotation[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the function call element
            startElement(SANNOTATION);
        }

        ATSIGN
        function_identifier

        // warnings seem to be caused by antlr ()* ending the rules

        // first greedy eliminates LPAREN LCURLY
        (options { greedy = true; } :
            call_argument_list

            // second greedy get rid of rparen
            (options { greedy = true; } :
                { LA(1) != RPAREN && LA(1) != COMMA }?
                annotation_argument

                // third greedy gets rid of comma
                (options { greedy = true; } :
                    comma
                )*
            )*

            rparen
        )*
;

/*
  call

  Handles a function call, macro, etc.
*/
call[int call_count = 1] { ENTRY_DEBUG } :
        {
            do {
                // start a new mode that will end after the argument list
                startNewMode(MODE_ARGUMENT | MODE_LIST | MODE_ARGUMENT_LIST | MODE_FUNCTION_CALL);

                // start the function call element
                startElement(SFUNCTION_CALL);
            } while (--call_count > 0);
        }

        (
            { inLanguage(LANGUAGE_OBJECTIVE_C) }?
            objective_c_call |

            function_identifier
            call_argument_list
        )
;

/*
  call_argument_list

  Handles an argument list used for a call.
*/
call_argument_list[] { ENTRY_DEBUG } :
        {
            // list of parameters
            replaceMode(MODE_ARGUMENT_LIST, MODE_EXPECT | MODE_LIST | MODE_INTERNAL_END_PAREN | MODE_END_ONLY_AT_RPAREN);

            // start the argument list
            startElement(SARGUMENT_LIST);
        }

        (
            LPAREN |

            {
                setMode(MODE_INTERNAL_END_CURLY);
            }

            LCURLY
        )
;

/*
  objective_c_call

  Handles an Objective-C function call.
*/
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

/*
  objective_c_call_receiver

  Handles an Objective-C function call object.
*/
objective_c_call_receiver[] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the receiver element
            startElement(SRECEIVER);
        }

        expression
;

/*
  objective_c_call_message

  Handles an Objective-C function call message.
*/
objective_c_call_message[] { ENTRY_DEBUG } :
        {
            endDownToMode(MODE_ARGUMENT_LIST);

            clearMode(MODE_ARGUMENT_LIST);

            // start the message element
            startElement(SMESSAGE);
        }

        objective_c_call_argument
;

/*
  objective_c_call_argument

  Handles an Objective-C function call argument "name:value" pair.
*/
objective_c_call_argument[] { bool first = true; ENTRY_DEBUG } :
        {
            if (inTransparentMode(MODE_LIST))
                endDownToMode(MODE_LIST);

            startNewMode(MODE_ARGUMENT);
        }

        objective_c_selector

        (options { greedy = true; } :
            { first && LA(1) != RBRACKET }?
            argument
            set_bool[first, false]
        )*
;

/*
  objective_c_selector

  Handles an Objective-C selector.
*/
objective_c_selector[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SSELECTOR);
        }

        (
            function_identifier

            (options { greedy = true; } : COLON)* |

            COLON
        )
;

/*
  ternary_expression
*/
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
            expression_process
            lparen_marked

            {
                startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);
            }

            ({ perform_ternary_check() }? ternary_expression)? |

            expression_no_ternary
        )
;

/*
  expression_no_ternary

  Handles an expression (that is known to not be a ternary).  Used in ternary_expression.
*/
expression_no_ternary[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :
        expression_process

        expression_part_plus_linq_no_ternary[type, call_count]
;

/*
  expression_part_plus_linq_no_ternary

  Handles an expression with linq (that is known to not be a ternary).  Used in expression_no_ternary.
*/
expression_part_plus_linq_no_ternary[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :
        {
            inLanguage(LANGUAGE_CSHARP)
            && next_token() != RPAREN
            && next_token() != ASSIGNMENT
            && next_token() != EQUAL
        }?
        (linq_expression_pure) => linq_expression |

        expression_part_no_ternary[type, call_count]
;

/*
  expression_part_no_ternary
*/
expression_part_no_ternary[CALL_TYPE type = NOCALL, int call_count = 1] {
        bool flag;
        bool isempty = false;
        bool end_control_incr = false;

        ENTRY_DEBUG
} :
        // cast
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        UNION |

        // cast
        {
            inTransparentMode(MODE_INTERNAL_END_PAREN)
            && (
                LA(1) != CXX_CLASS
                || !keyword_name_token_set.member(next_token())
            )
        }?
        (CLASS | CXX_CLASS) |

        { next_token() == LPAREN || next_token() == LCURLY }?
        delegate_anonymous |

        { next_token() == LCURLY }?
        lambda_anonymous |

        { inLanguage(LANGUAGE_CSHARP) }?
        (lambda_expression_full_csharp) => lambda_expression_csharp |

        { inLanguage(LANGUAGE_CXX) }?
        (
            bracket_pair
            (options { warnWhenFollowAmbig = false; } : paren_pair)*
            function_tail
            LCURLY
        ) => lambda_expression_cpp |

        { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
        (block_lambda_expression_full) => block_lambda_expression |

        { inLanguage(LANGUAGE_JAVA) }?
        ((paren_pair | variable_identifier) TRETURN) => lambda_expression_java |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW generic_argument_list) => sole_new generic_argument_list |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (
            NEW
            function_identifier
            paren_pair
            LCURLY
        ) => sole_new anonymous_class_definition |

        { notdestructor }?
        sole_destop { notdestructor = false; } |

        { next_token() != LPAREN && next_token() != DOTDOTDOT }?
        sizeof_unary_expression |

        // call
        // need to distinguish between a call and a macro
        {
            type == CALL
            || (
                perform_call_check(type, isempty, call_count, -1)
                && type == CALL
            )
        }?
        // added argument to correct markup of default parameters using a call
        // normally call claims left paren and starts call argument; however, I believe parameter_list matches a right paren of the call
        (call[call_count] | keyword_calls)
        argument |

        // macro call
        { type == MACRO }?
        macro_call |

        // general math operators
        // looks like general operators and variable identifiers can match same thing
        (
            options { generateAmbigWarnings = false; } :
                general_operators

                {
                    if (inLanguage(LANGUAGE_CXX_FAMILY) && LA(1) == DESTOP)
                        general_operators();
                } |

                qmark |
                period |
                member_pointer |
                member_pointer_dereference |
                dot_dereference |
                /* Commented-out code: newop | */

                // left parentheses
                { function_pointer_name_check() }?
                function_pointer_name |

                lparen_marked

                {
                    startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);
                } |

                // right parentheses that only matches the left parentheses of an expression
                { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
                {
                    end_control_incr = inTransparentMode(MODE_CONTROL_INCREMENT);

                    // stop at this matching paren, or a preprocessor statement
                    endDownToModeSet(MODE_INTERNAL_END_PAREN | MODE_PREPROC);

                    if (inMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN))
                        endMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);

                    end_control_incr = end_control_incr && !inTransparentMode(MODE_CONTROL_INCREMENT);
                }
                // treat as operator for operator markup
                rparen[!end_control_incr, end_control_incr] |

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
                    if (
                        !inTransparentMode(MODE_CALL)
                        && !inTransparentMode(MODE_INIT)
                        && !inTransparentMode(MODE_FUNCTION_CALL)
                    ) {
                        endDownToMode(MODE_INTERNAL_END_CURLY);

                        endMode(MODE_INTERNAL_END_CURLY);
                    }
                }
                rcurly_argument |

                // variable or literal
                variable_identifier |

                keyword_name |

                auto_keyword[false] |

                single_keyword_specifier
        ) |

        literals |

        noexcept_list |

        variable_identifier_array_grammar_sub[flag]
;

/*
  keyword_calls

  Handles keyword-based calls with special markup.
*/
keyword_calls[] { ENTRY_DEBUG } :
        // C++
        sizeof_call | alignof_call | typeid_call | const_cast_call | dynamic_cast_call |
        reinterpret_cast_call | static_cast_call | decltype_call |

        // Objective-C
        encode_call | selector_call |

        // C#
        typeof_call |
        { inLanguage(LANGUAGE_CSHARP) }? default_call |
        checked_call | unchecked_call
;

/*
  keyword_call_tokens
*/
keyword_call_tokens[] { ENTRY_DEBUG } :
        // C++
        SIZEOF | ALIGNOF | TYPEID | CONST_CAST | DYNAMIC_CAST |
        REINTERPRET_CAST | STATIC_CAST | DECLTYPE |

        // Objective-C
        ENCODE | SELECTOR |

        // C#
        TYPEOF | DEFAULT | CHECKED | UNCHECKED
;

/*
  sizeof_unary_expression
*/
sizeof_unary_expression[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SSIZEOF_CALL);
        }

        SIZEOF
        variable_identifier
;

/*
  sizeof_call

  Handles a "sizeof(...)" call.
*/
sizeof_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the sizeof call element
            if (next_token() != DOTDOTDOT)
                startElement(SSIZEOF_CALL);
            else
                startElement(SSIZEOF_PACK);
        }

        SIZEOF
        (DOTDOTDOT)*
        call_argument_list
;

/*
  alignof_call

  Handles an "alignof()" call.
*/
alignof_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the alignof element
            startElement(SALIGNOF);
        }

        ALIGNOF
        call_argument_list
;

/*
  typeid_call

  Handles a "typeid()" call.
*/
typeid_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the typeid element
            startElement(STYPEID);
        }

        TYPEID
        call_argument_list
;

/*
  const_cast_call

  Handles a "const_cast<>()" call.
*/
const_cast_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the const cast element
            startElement(SCONST_CAST);
        }

        CONST_CAST
        (generic_argument_list)*
        call_argument_list
;

/*
  dynamic_cast_call

  Handles a "dynamic_cast<>()" call.
*/
dynamic_cast_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the dynamic cast element
            startElement(SDYNAMIC_CAST);
        }

        DYNAMIC_CAST
        (generic_argument_list)*
        call_argument_list
;

/*
  reinterpret_cast_call

  Handles a "reinterpret_cast<>()" call.
*/
reinterpret_cast_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the reinterpret cast element
            startElement(SREINTERPRET_CAST);
        }

        REINTERPRET_CAST
        (generic_argument_list)*
        call_argument_list
;

/*
  static_cast_call

  Handles a "static_cast<>()" call.
*/
static_cast_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the static cast element
            startElement(SSTATIC_CAST);
        }

        STATIC_CAST
        (generic_argument_list)*
        call_argument_list
;

/*
  encode_call

  Handles an "@encode(...)" call.
*/
encode_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the encode element
            startElement(SENCODE);
        }

        ENCODE
        call_argument_list
;

/*
  selector_call

  Handles a "@selector(...)" call.
*/
selector_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the selector element
            startElement(SSELECTOR);
        }

        SELECTOR
        call_argument_list
;

/*
  typeof_call

  Handles a "typeof" call.
*/
typeof_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the typeof element
            startElement(STYPEOF);
        }

        TYPEOF
        call_argument_list
;

/*
  default_call

  Handles a "default" call.
*/
default_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the default element
            startElement(SDEFAULT);
        }

        DEFAULT
        call_argument_list
;

/*
  checked_call

  Handles a "checked" call.
*/
checked_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the checked statement
            startElement(SCHECKED_STATEMENT);
        }

        CHECKED
        call_argument_list
;

/*
  unchecked_call

  Handles an "unchecked" call.
*/
unchecked_call[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);

            // start the unchecked statement
            startElement(SUNCHECKED_STATEMENT);
        }

        UNCHECKED
        call_argument_list
;

/*
  macro_call_check

  Checks for a macro call.
*/
macro_call_check[] { ENTRY_DEBUG } :
        simple_identifier
        (options { greedy = true; } : paren_pair)*
;

/*
  eat_optional_macro_call

  Eats an optional macro call.
*/
eat_optional_macro_call[] {
        bool success = false;

        // find out if we have a macro call
        int start = mark();
        inputState->guessing++;

        try {
            // check for the name
            if (LA(1) == NAME)
                match(NAME);
            else 
                match(VOID);

            // handle the parentheses
            paren_pair();

            success = true;
        } catch (...) {}

        inputState->guessing--;
        rewind(start);

        // when successfull, eat the macro
        if (success)
            macro_call();

        ENTRY_DEBUG
} :;

/*
  macro_call

  Used to start marking a macro call.
*/
macro_call[] { ENTRY_DEBUG } :
        macro_call_inner

        {
            if (inMode(MODE_THEN) && LA(1) == ELSE)
                endMode(MODE_THEN);
        }
;

/*
  macro_call_inner

  Handles the actual macro call.  Used in macro_call.
*/
macro_call_inner[] { CompleteElement element(this); bool first = true; ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);
        }

        (identifier | CXX_CLASS)

        (options { greedy = true; } :
            { first }?
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

            set_bool[first, false]
        )*
;
exception
catch[antlr::RecognitionException&] {
        // no end found to macro
        if (isoption(parser_options, SRCML_OPTION_DEBUG))
            emptyElement(SERROR_PARSE);
}

/*
  macro_pattern_call
*/
macro_pattern_call[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);

            startNewMode(MODE_LOCAL);

            startElement(SNAME);
        }

        MACRO_NAME

        {
            endMode();
        }

        macro_call_argument_list
;

/*
  macro_type_name_call
*/
macro_type_name_call[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);

            startNewMode(MODE_LOCAL);

            startElement(SNAME);
        }

        MACRO_TYPE_NAME

        {
            endMode();
        }

        macro_call_argument_list
;

/*
  macro_case_call
*/
macro_case_call[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);

            startNewMode(MODE_LOCAL);

            startElement(SNAME);
        }

        MACRO_CASE

        {
            endMode();
        }

        macro_call_argument_list
;

/*
  macro_label_call
*/
macro_label_call[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);

            startNewMode(MODE_LOCAL);

            startElement(SNAME);
        }

        MACRO_LABEL

        {
            endMode();
        }

        macro_call_argument_list
;

/*
  macro_specifier_call
*/
macro_specifier_call[] { CompleteElement element(this); ENTRY_DEBUG } :
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

        {
            endMode();
        }

        macro_call_argument_list
;

/*
  macro_call_argument_list

  Also used to handle the actual macro call.  Used in multiple places.
*/
macro_call_argument_list[] { bool first = true; ENTRY_DEBUG } :
        (options { greedy = true; } :
            { first }?
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

            set_bool[first, false]
        )*
;
exception
catch[antlr::RecognitionException&] {
        // no end found to macro
        if (isoption(parser_options, SRCML_OPTION_DEBUG))
            emptyElement(SERROR_PARSE);
}

/*
  macro_type_name

  Handles a macro list/pattern name by itself.
*/
macro_type_name[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        MACRO_TYPE_NAME
;

/*
  macro_type_name_call_inner
*/
macro_type_name_call_inner[] { CompleteElement element(this); bool first = true; ENTRY_DEBUG } :
        {
            // start a mode for the macro that will end after the argument list
            startNewMode(MODE_STATEMENT | MODE_TOP);

            // start the macro call element
            startElement(SMACRO_CALL);
        }

        macro_type_name

        (options { greedy = true; } :
            { first }?
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

            set_bool[first, false]
        )*
;
exception
catch[antlr::RecognitionException&] {
        // no end found to macro
        if (isoption(parser_options, SRCML_OPTION_DEBUG))
            emptyElement(SERROR_PARSE);
}

/*
  macro_call_contents

  Handles the contents of a macro call.
*/
macro_call_contents[] {
        ENTRY_DEBUG

        CompleteElement element(this);

        int parencount = 0;
        bool start = true;

        while (
            LA(1) != 1 /* EOF? */
            && !(
                parencount == 0
                && LA(1) == RPAREN
            )
        ) {
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

            if (
                inputState->guessing == 0
                && LA(1) == COMMA
                && parencount == 0
            ) {
                endMode();
                start = true;
            }

            if (literal_tokens_set.member(LA(1)))
                literals();
            else
                consume();
        }
} :;

/*
  try_statement

  Handles a try statement.
*/
try_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TRY);

            // start of the try statement
            startElement(STRY_BLOCK);
        }

        (TRY | CXX_TRY)
;

/*
  try_statement_with_resource

  Used to handle a try statement with resources.
*/
try_statement_with_resource[] { ENTRY_DEBUG } :
        {
            // treat try block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TRY);

            // start of the try statement
            startElement(STRY_BLOCK);
        }

        TRY
        for_like_statement_post 
;

/*
  checked_statement

  Handles a checked statement.  Sometimes appears inside of a try statement in C#.
*/
checked_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the checked statement
            startElement(SCHECKED_STATEMENT);
        }

        CHECKED
;

/*
  unsafe_statement
*/
unsafe_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the unsafe statement
            startElement(SUNSAFE_STATEMENT);
        }

        UNSAFE
;

/*
  using_namespace_statement
*/
using_namespace_statement[] { ENTRY_DEBUG } :
        { inLanguage(LANGUAGE_CSHARP) && next_token() == LPAREN }?
        using_statement |

        namespace_directive
;

/*
  using_statement

  Handles a using statement.  Used in using_namespace_statement.
*/
using_statement[] { ENTRY_DEBUG } :
        // typically, doing something like this does not work in antlr because it looks for something like EOF instead of nothing
        // however, it seems to work in this case, possibly because it is used with tokens required afterward
        for_like_statement_pre[SUSING_STATEMENT]

        USING

        for_like_statement_post
;

/*
  for_like_statement_pre
*/
for_like_statement_pre[int tag] { ENTRY_DEBUG } :
        {
            // treat try block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the try statement
            startElement(tag);
        }
;

/*
  for_like_statement_post
*/
for_like_statement_post[] { ENTRY_DEBUG } :
        {
            // expect a condition to follow the keyword
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPECT | MODE_INTERNAL_END_PAREN);

            startElement(SFOR_LIKE_CONTROL);
        }

        LPAREN

        {
            startNewMode(MODE_FOR_LIKE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_STATEMENT | MODE_INTERNAL_END_PAREN | MODE_LIST);
            
            // Commented-out code
            // startElement(SCONTROL_INITIALIZATION);
        }

        for_like_list_item
;

/*
  for_like_list_item
*/
for_like_list_item[] {
        int type_count = 0;
        int secondtoken = 0;
        int after_token = 0;
        STMT_TYPE stmt_type = NONE;

        ENTRY_DEBUG
} :
        // explicitly check for a variable declaration since it can easily be confused with an expression
        { pattern_check(stmt_type, secondtoken, type_count, after_token) && stmt_type == VARIABLE }?
        control_initialization_variable_declaration[type_count] |

        {
            // use a new mode without the expect so we do not nest expression parts
            startNewMode(MODE_EXPRESSION);

            // start the expression element
            startElement(SEXPRESSION);
        }

        // explicitly check for non-terminate to avoid using a large switch statement
        expression
;

/*
  lock_statement
*/
lock_statement[] { ENTRY_DEBUG } :
        for_like_statement_pre[SLOCK_STATEMENT]

        LOCK

        for_like_statement_post
;

/*
  fixed_statement
*/
fixed_statement[] { ENTRY_DEBUG } :
        for_like_statement_pre[SFIXED_STATEMENT]

        FIXED

        for_like_statement_post
;

/*
  synchronized_statement
*/
synchronized_statement[] { ENTRY_DEBUG } :
        for_like_statement_pre[SSYNCHRONIZED_STATEMENT]

        SYNCHRONIZED

        for_like_statement_post
;

/*
  unchecked_statement
*/
unchecked_statement[] { ENTRY_DEBUG } :
        {
            // treat try block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the unchecked statement
            startElement(SUNCHECKED_STATEMENT);
        }

        UNCHECKED
;

/*
  autoreleasepool_block
*/
autoreleasepool_block[] { ENTRY_DEBUG } :
        {
            // treat try block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the autorelease pool statement
            startElement(SAUTORELEASEPOOL);
        }

        AUTORELEASEPOOL
        lcurly
;

/*
  catch_statement
*/
catch_statement[] { ENTRY_DEBUG } :
        {
            // treat catch block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the catch statement
            startElement(SCATCH_BLOCK);
        }

        (CATCH | CXX_CATCH)
        (parameter_list)*
;

/*
  finally_statement
*/
finally_statement[] { ENTRY_DEBUG } :
        {
            // treat catch block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST);

            // start of the catch statement
            startElement(SFINALLY_BLOCK);
        }

        FINALLY
;

/*
  lambda_anonymous
*/
lambda_anonymous[] { ENTRY_DEBUG } :
        {
            // treat catch block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_ANONYMOUS);
        }

        lambda_csharp

        // completely parse a function until it is done
        (options { greedy = true; } : { inputState->guessing }? curly_pair)*
;

/*
  delegate_type
*/
delegate_type[int type_count] { ENTRY_DEBUG } :
        {
            // treat catch block as a nested block statement
            startNewMode(MODE_STATEMENT);

            // start of the delegate statement
            startElement(SFUNCTION_DELEGATE);
        }

        (derive_access)*
        DELEGATE
        function_header[type_count]
;

/*
  delegate_anonymous
*/
delegate_anonymous[] { ENTRY_DEBUG } :
        {
            // treat catch block as a nested block statement
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_ANONYMOUS);

            // start of the delegate statement
            startElement(SFUNCTION_DELEGATE);
        }

        delegate_marked
        (options { greedy = true; } : parameter_list)*

        // completely parse a function until it is done
        (options { greedy = true; } : { inputState->guessing }? curly_pair)*
;

/*
  delegate_marked

  Used to mark the delegate function.
*/
delegate_marked[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        DELEGATE
;

/*
  lambda_csharp
*/
lambda_csharp[] { ENTRY_DEBUG } :
        LAMBDA

        {
            if (LA(1) != LCURLY) {
                startNoSkipElement(SPSEUDO_BLOCK);
                startNoSkipElement(SCONTENT);
            } else if (LA(1) == LCURLY) {
                startNewMode(MODE_FUNCTION_TAIL | MODE_ANONYMOUS);
            }
        }
;

/*
  throw_statement
*/
throw_statement[] { ENTRY_DEBUG } :
        {
            // statement with an expected expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start of a throw statement
            startElement(STHROW_STATEMENT);
        }

        THROW
;

/*
  generic_selection

  Handles a "_Generic" selection (C).
*/
generic_selection[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            // start the generic selection statement
            startElement(SGENERIC_SELECTION);

            startNewMode(MODE_LIST);
        }

        (
            { inputState->guessing }?
            GENERIC_SELECTION
            paren_pair |

            GENERIC_SELECTION
            LPAREN
            generic_selection_selector
            COMMA
            generic_selection_association_list

            rparen[false]
        )
;

/*
  generic_selection_selector
*/
generic_selection_selector[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SGENERIC_SELECTOR);
        }

        generic_selection_complete_expression
;

/*
  generic_selection_association_list
*/
generic_selection_association_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // list of parameters
            setMode(MODE_EXPECT | MODE_LIST | MODE_INTERNAL_END_PAREN | MODE_END_ONLY_AT_RPAREN | MODE_ASSOCIATION_LIST);

            // start the association list
            startElement(SGENERIC_ASSOCIATION_LIST);
        }

        (
            COMMA |

            { LA(1) != RPAREN }?
            generic_selection_association
        )*

        // Commented-out code
        // (LPAREN | { setMode(MODE_INTERNAL_END_CURLY); } LCURLY)
;

/*
  generic_selection_complete_expression
*/
generic_selection_complete_expression[] {
        CompleteElement element(this);
        int count_paren = 1;
        CALL_TYPE type = NOCALL;
        bool isempty = false;
        int call_count = 0;

        ENTRY_DEBUG
} :
        {
            // start a mode to end at a right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_END_AT_COMMA);

            // start the expression statement
            startElement(SEXPRESSION);
        }

        (options { warnWhenFollowAmbig = false; } :
            {
                count_paren > 0
                && (
                    LA(1) != COMMA
                    || !inMode(MODE_END_AT_COMMA)
                )
                && (
                    count_paren != 1
                    || LA(1) != RPAREN
                )
            }?
            (
                { !inMode(MODE_END_AT_COMMA) }?
                comma |

                // argument mode (as part of call)
                {
                    inMode(MODE_ARGUMENT)
                    && LA(1) != RPAREN
                    && LA(1) != RCURLY
                }?
                complete_arguments |

                { LA(1) == LPAREN }?
                expression
                {
                    ++count_paren;
                } |

                { LA(1) == RPAREN }?
                expression
                {
                    --count_paren;
                } |

                { perform_call_check(type, isempty, call_count, -1) && type == CALL }?
                { 
                    if (!isempty)
                        ++count_paren;
                }
                expression_process
                (call[call_count] | keyword_calls)
                complete_arguments |

                expression
            )
        )*
;

/*
  generic_selection_association
*/
generic_selection_association[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // argument with a nested expression
            startNewMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the generic association statement
            startElement(SGENERIC_ASSOCIATION);
        }

        generic_selection_association_type
        COLON
        generic_selection_complete_expression
;

/*
  generic_selection_association_type
*/
generic_selection_association_type[] {
        int type_count = 0;
        int secondtoken = 0;
        int after_token = 0;
        STMT_TYPE stmt_type = NONE;

        ENTRY_DEBUG
} :
        {
            setMode(MODE_ASSOCIATION_TYPE);
        }

        (
            { pattern_check(stmt_type, secondtoken, type_count, after_token, true) }?
            variable_declaration_type[type_count + 1] |

            generic_selection_association_default
        )

        {
            clearMode(MODE_ASSOCIATION_TYPE);
        }
;

/*
  generic_selection_association_default
*/
generic_selection_association_default[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(STYPE);
        }

        DEFAULT
;

/*
  expression_statement_process

  Handles expression statement preprocessing.
*/
expression_statement_process[] { ENTRY_DEBUG } :
        {
            bool inenumclass = (inLanguage(LANGUAGE_JAVA_FAMILY) && inTransparentMode(MODE_ENUM) && inMode(MODE_CLASS));

            // statement with an embedded expression
            startNewMode(MODE_STATEMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the element that will end after the terminate
            if (!inenumclass)
                startElement(SEXPRESSION_STATEMENT);
        }
;

/*
  expression_statement

  Handles an expression statement.
*/
expression_statement[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :
        expression_statement_process

        {
            pauseStream();
        }

        expression[type, call_count]
;

/*
  variable_declaration_statement
*/
variable_declaration_statement[int type_count] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_STATEMENT);

            if (!inTransparentMode(MODE_TYPEDEF) || inTransparentMode(MODE_CLASS | MODE_INNER_DECL)) {
                // start the declaration statement
                startElement(SDECLARATION_STATEMENT);

                pauseStream();
            }
        }

        variable_declaration[type_count]
;

/*
  short_variable_declaration

  Handles processing for short variable declaration.
*/
short_variable_declaration[] { ENTRY_DEBUG } :
        {
            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // declaration
            startNewMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // start the declaration statement
            startElement(SDECLARATION);
        }
;

/*
  variable_declaration

  Handles more of the inner part of a declaration.
*/
variable_declaration[int type_count] { ENTRY_DEBUG } :
        {
            bool output_decl = !inTransparentMode(MODE_TYPEDEF) || inTransparentMode(MODE_CLASS | MODE_INNER_DECL);

            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // declaration
            startNewMode(MODE_LOCAL| MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            if (inTransparentMode(MODE_CONTROL_CONDITION | MODE_END_AT_COMMA))
                setMode(MODE_LIST);

            if (output_decl)
                // start the declaration statement
                startElement(SDECLARATION);
        }

        (options { greedy = true; } :
            { next_token() == TEMPOPS }?
            template_declaration_full
            set_int[type_count, type_count - 1]
        )*

        (options { greedy = true; } :
            {
                type_count > 0
                && (
                    LA(1) != OVERRIDE
                    || !inLanguage(LANGUAGE_CXX)
                )
                && (
                    (
                        inLanguage(LANGUAGE_JAVA)
                        && LA(1) == ATSIGN
                    )
                    || (
                        inLanguage(LANGUAGE_CSHARP)
                        && LA(1) == LBRACKET
                    )
                    || (
                        inLanguage(LANGUAGE_CXX)
                        && LA(1) == LBRACKET
                        && next_token() == LBRACKET
                    )
                )
            }?
            decl_pre_type_annotation[type_count]
        )*

        variable_declaration_type[type_count]
;

/*
  variable_declaration_type
*/
variable_declaration_type[int type_count] { bool is_compound = false; ENTRY_DEBUG } :
        {
            if (type_count == 0) {
                if (inTransparentMode(MODE_ARGUMENT) && inLanguage(LANGUAGE_CXX))
                    return;

                setMode(MODE_VARIABLE_NAME | MODE_INIT);
                return;
            }

            // start a mode for the type that will end in this grammar rule
            startNewMode(MODE_EAT_TYPE);

            // type element begins
            startElement(STYPE);
        }

        (options { greedy = true; } :
            { decl_specifier_tokens_set.member(LA(1)) }?
            (specifier | default_specifier | template_specifier)
            set_int[type_count, type_count - 1]
        )*

        {
            if (type_count == 0) {
                endMode(MODE_EAT_TYPE);
                return;
            }

            setTypeCount(type_count);
        }

        // match auto keyword first as a special case; do not warn about ambiguity
        (options { generateAmbigWarnings = false; } :
            { LA(1) == CXX_CLASS && keyword_name_token_set.member(next_token()) }?
            keyword_name |

            auto_keyword[type_count > 1] |

            { is_class_type_identifier() }?
            (options { greedy = true; } :
                { !class_tokens_set.member(LA(1)) }?
                (options { generateAmbigWarnings = false; } :
                    specifier |

                    { look_past_rule(&srcMLParser::identifier) != LPAREN }?
                    identifier |

                    macro_call
                )
                {
                    decTypeCount();
                }
            )*
            class_type_identifier[is_compound]
            {
                decTypeCount();
            }
            (options { greedy = true; } : { !is_compound }?  multops)* |

            lead_type_identifier |

            EVENT
        )

        {
            if (!inTransparentMode(MODE_TYPEDEF))
                decTypeCount();
        } 

        (options { greedy = true; } :
            { !inTransparentMode(MODE_TYPEDEF) && getTypeCount() > 0 }?
            (options { generateAmbigWarnings = false; } : keyword_name | type_identifier)
            {
                decTypeCount();
            }
        )*

        update_typecount[MODE_VARIABLE_NAME | MODE_INIT]
;

/*
  specifiers_or_macro
*/
specifiers_or_macro[] { bool first = true; ENTRY_DEBUG } :
        (options { greedy = true; } : specifier)*

        (options { greedy = true; } :
            { first && !class_tokens_set.member(LA(1)) }?
            macro_call
            set_bool[first, false]
        )*

        (options { greedy = true; } : specifier)*
;

/*
  is_class_type_identifier
*/
is_class_type_identifier[] returns [bool is_class_type = false] {
        ENTRY_DEBUG

        if (
            inputState->guessing
            || inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST)
            || inTransparentMode(MODE_ASSOCIATION_TYPE)
        )
            return is_class_type;

        int token = look_past_rule(&srcMLParser::specifiers_or_macro);

        if (class_tokens_set.member(token))
            is_class_type = true;
} :;

/*
  class_type_identifier
*/
class_type_identifier[bool& is_compound] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);
        
            startElement(SNAME);
        }

        class_type_identifier_keyword
        class_type_compound_name[is_compound]
;

/*
  class_type_compound_name
*/
class_type_compound_name[bool& is_compound] { ENTRY_DEBUG } :
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

            {
                !inLanguage(LANGUAGE_JAVA_FAMILY)
                && !inLanguage(LANGUAGE_C)
                && !inLanguage(LANGUAGE_CSHARP)
                && !inLanguage(LANGUAGE_OBJECTIVE_C)
            }?
            compound_name_cpp[is_compound] |

            keyword_name_inner[is_compound] |

            macro_type_name_call
        )

        (options { greedy = true; } :
            { inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }?
            attribute_cpp
        )*

        {
            endMode();
        }
;

/*
  class_type_identifier
*/
class_type_identifier_keyword[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        (CLASS | CXX_CLASS | STRUCT | UNION | ENUM)
;

/*
  variable_declaration_nameinit

  Handles a variable declaration name and optional initialization.
*/
variable_declaration_nameinit[] { bool isthis = LA(1) == THIS; bool instypeprev = false; ENTRY_DEBUG } :
        {
            if (
                !inMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT)
                && inMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT)
                && !inTransparentMode(MODE_TYPEDEF)
                && !inTransparentMode(MODE_USING)
            ) {
                startNewMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

                // start the declaration statement
                startElement(SDECLARATION);

                startNewMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

                startElement(STYPEPREV);

                instypeprev = true;
            }
        }

        (multops)*

        {
            if (instypeprev)
                endMode();
        }

        // Mark as name before mark without name
        (options { generateAmbigWarnings = false; } :
            { inLanguage(LANGUAGE_CSHARP) }?
            compound_name_inner[!isthis] |

            compound_name |

            keyword_name
        )

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

/*
  property_statement
*/
property_statement[int type_count] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_NO_BLOCK_CONTENT);

            startElement(SPROPERTY);

            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT | MODE_NO_BLOCK_CONTENT);

            // declaration
            startNewMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT | MODE_NO_BLOCK_CONTENT);
        }

        (options { greedy = true; } :
            { next_token() == TEMPOPS }?
            template_declaration_full
            set_int[type_count, type_count - 1]
        )*

        (options { greedy = true; } :
            {
                type_count > 0
                && (
                    LA(1) != OVERRIDE
                    || !inLanguage(LANGUAGE_CXX)
                )
                && (
                    (
                        inLanguage(LANGUAGE_JAVA)
                        && LA(1) == ATSIGN
                    )
                    || (
                        inLanguage(LANGUAGE_CSHARP)
                        && LA(1) == LBRACKET
                    )
                    || (
                        inLanguage(LANGUAGE_CXX)
                        && LA(1) == LBRACKET
                        && next_token() == LBRACKET
                    )
                )
            }?
            decl_pre_type_annotation[type_count]
        )*

        variable_declaration_type[type_count]
;

/*
  event_statement
*/
event_statement[int type_count] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            startElement(SEVENT);

            // variable declarations may be in a list
            startNewMode(MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // declaration
            startNewMode(MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);
        }

        (options { greedy = true; } :
            { next_token() == TEMPOPS }?
            template_declaration_full
            set_int[type_count, type_count - 1]
        )*

        (options { greedy = true; } :
            {
                type_count > 0
                && (
                    LA(1) != OVERRIDE
                    || !inLanguage(LANGUAGE_CXX)
                )
                && (
                    decl_specifier_tokens_set.member(LA(1))
                    || (
                        inLanguage(LANGUAGE_JAVA)
                        && LA(1) == ATSIGN
                    )
                    || (
                        inLanguage(LANGUAGE_CSHARP)
                        && LA(1) == LBRACKET
                    )
                    || (
                        inLanguage(LANGUAGE_CXX)
                        && LA(1) == LBRACKET
                        && next_token() == LBRACKET
                    )
                )
            }?
            decl_pre_type_annotation[type_count]
        )*

        EVENT
        set_int[type_count, type_count - 1]

        variable_declaration_type[type_count]
;

/*
  function_pointer_initialization

  Handles the initializtion of a function pointer.
*/
function_pointer_initialization[] { ENTRY_DEBUG } :
        {
            // end the init correctly
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the declaration initialization statement
            startElement(SDECLARATION_INITIALIZATION);
        }

        EQUAL
        complete_default_parameter
;

/*
  variable_declaration_initialization

  Handles the initialization of a declared variable.
*/
variable_declaration_initialization[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the declaration initialization statement
            startElement(SDECLARATION_INITIALIZATION);
        }

        EQUAL |

        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_ARGUMENT | MODE_LIST);
        }

        call_argument_list
;

/*
  variable_declaration_range

  Handles a bit field.
*/
variable_declaration_range[] { ENTRY_DEBUG } :
        {
            // start a new mode that will end after the argument list
            startNewMode(MODE_LIST | MODE_IN_INIT | MODE_EXPRESSION | MODE_EXPECT);

            // start the declaration range statement
            startElement(SDECLARATION_RANGE);
        }

        (COLON | IN)
;

/*
  parameter_declaration_initialization

  Handles the initialization of a parameter variable.
*/
parameter_declaration_initialization[] { ENTRY_DEBUG } :
        {
            // end the init correctly
            setMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the declaration initialization statement
            startElement(SDECLARATION_INITIALIZATION);
        }

        EQUAL
;

/*
  pure_expression_block

  Handles the beginning and setup of an expression block.
*/
pure_expression_block[] { ENTRY_DEBUG } :
        lcurly_base

        {
            // nesting blocks, not statement
            replaceMode(MODE_STATEMENT | MODE_NEST, MODE_BLOCK | MODE_NEST | MODE_END_AT_BLOCK_NO_TERMINATE);

            // end this expression block correctly
            startNewMode(MODE_TOP | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT);
        }
;

/*
  general_operators

  Handles all possible operators.
*/
general_operators[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if ((LA(1) != IN || !inTransparentMode(MODE_CONTROL_CONDITION)))
                startElement(SOPERATOR);
        }

        (
            OPERATORS |

            ASSIGNMENT |

            TEMPOPS |

            TEMPOPE

            (options { greedy = true; } :
                ({ SkipBufferSize() == 0 }? TEMPOPE)

                ({ SkipBufferSize() == 0 }? TEMPOPE)? |

                ({ inLanguage(LANGUAGE_JAVA) && LT(1)->getText() == ">>="sv }? ASSIGNMENT)
            )? |

            /* Commented-out code: MULTIMM | */
            /* Commented-out code: MEMBERPOINTER | */

            EQUAL | DESTOP | MULTOPS | REFOPS | DOTDOT | RVALUEREF |
            { inLanguage(LANGUAGE_JAVA) }? BAR |

            // others are not combined
            NEW | DELETE | IN | IS | STACKALLOC | AS | AWAIT | LAMBDA | DOTDOTDOT |

            // Objective-C
            CSPEC | MSPEC |

            // Apple
            BLOCKOP
        )
;

/*
  sole_new

  Handles the "new" operator only.
*/
sole_new[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOPERATOR);
        }

        NEW
;

/*
  sole_destop

  Handles the "~" operator only.
*/
sole_destop[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOPERATOR);
        }

        DESTOP
;

/*
  rparen_operator

  Used to mark up a right parenthesis.
*/
rparen_operator[bool markup = true] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (markup && !inMode(MODE_END_ONLY_AT_RPAREN))
                startElement(SOPERATOR);
        }

        RPAREN
;

/*
  rparen

  Handles right parenthesis processing.
*/
rparen[bool markup = true, bool end_control_incr = false] { bool isempty = getParen() == 0; ENTRY_DEBUG } :
        {
            if (isempty) {
                // additional right parentheses indicates end of non-list modes
                endDownToModeSet(MODE_LIST | MODE_PREPROC | MODE_END_ONLY_AT_RPAREN | MODE_ONLY_END_TERMINATE);

                // special case: need an extra end mode for for-initalization
                if (inMode(MODE_VARIABLE_NAME) && inTransparentMode(MODE_CONTROL_CONDITION))
                    endDownToModeSet(MODE_CONTROL_CONDITION);

                // do not markup since it is not a normal operator
                markup = false;
            } else {
                decParen();
            }

            if (inMode(MODE_ASSOCIATION_LIST))
                endMode(MODE_ASSOCIATION_LIST);

            if (end_control_incr || inMode(MODE_LIST | MODE_CONTROL_CONDITION))
                setMode(MODE_END_CONTROL);
        }

        rparen_operator[markup]

        {
            if (isempty) {
                // special handling for the then part of an if statement; only accessed when in the condition of an if statement
                if (inMode(MODE_CONDITION) && inPrevMode(MODE_IF)) {
                    // end the condition
                    endMode(MODE_CONDITION);

                    // then part of the if statement (after the condition)
                    startNewMode(MODE_STATEMENT | MODE_NEST | MODE_THEN);

                    // Commented-out code
                    // start the then element
                    // startNoSkipElement(STHEN);

                    if (LA(1) != LCURLY) {
                        startNoSkipElement(SPSEUDO_BLOCK);
                        startNoSkipElement(SCONTENT);
                    }

                    if (cppif_duplicate) {
                        std::stack<int> open_elements;

                        // Commented-out code
                        // open_elements.push(STHEN);

                        if (LA(1) != LCURLY)
                            open_elements.push(SPSEUDO_BLOCK);

                        dupMode(open_elements);
                    }

                    cppif_duplicate = false;
                }

                // end while condition, etc. and output pseudo block
                if (inMode(MODE_LIST | MODE_CONDITION) && inPrevMode(MODE_STATEMENT | MODE_NEST)) {
                    endMode(MODE_LIST);

                    if (LA(1) != LCURLY) {
                        startNoSkipElement(SPSEUDO_BLOCK);
                        startNoSkipElement(SCONTENT);
                    }

                    if (cppif_duplicate) {
                        std::stack<int> open_elements;

                        if (LA(1) != LCURLY)
                            open_elements.push(SPSEUDO_BLOCK);

                        dupMode(open_elements);
                    }

                    cppif_duplicate = false;
                // end control group and output pseudo block
                } else if (end_control_incr) {
                    if (inMode(MODE_LIST))
                        endMode(MODE_LIST);

                    if (LA(1) != LCURLY) {
                        startNoSkipElement(SPSEUDO_BLOCK);
                        startNoSkipElement(SCONTENT);
                    }

                    if (cppif_duplicate) {
                        std::stack<int> open_elements;

                        if (LA(1) != LCURLY)
                            open_elements.push(SPSEUDO_BLOCK);

                        dupMode(open_elements);
                    }

                    cppif_duplicate = false;
                } else if (inMode(MODE_LIST | MODE_CONTROL_CONDITION)) {
                    endMode(MODE_CONTROL_CONDITION);

                    if (LA(1) != LCURLY) {
                        startNoSkipElement(SPSEUDO_BLOCK);
                        startNoSkipElement(SCONTENT);
                    }

                    if (cppif_duplicate) {
                        std::stack<int> open_elements;

                        if (LA(1) != LCURLY)
                            open_elements.push(SPSEUDO_BLOCK);

                        dupMode(open_elements);
                    }

                    cppif_duplicate = false;
                } else {
                // end the single mode that started the list; do not end more than one since they may be nested
                if (inMode(MODE_LIST))
                    endMode(MODE_LIST);
                }
            }
        }
;

/*
  rcurly_argument

  Handles right curly brace matching and processing.
*/
rcurly_argument[] { bool isempty = getCurly() == 0; ENTRY_DEBUG } :
        {
            if (isempty) {
                // additional right parentheses indicates end of non-list modes
                if (inTransparentMode(MODE_LIST))
                    endDownToModeSet(MODE_LIST | MODE_PREPROC | MODE_END_ONLY_AT_RPAREN | MODE_ONLY_END_TERMINATE | MODE_INTERNAL_END_CURLY);
            }
        }

        RCURLY

        {
            // end the single mode that started the list; do not end more than one since they may be nested
            if (isempty && inMode(MODE_LIST)) {
                while (
                    inMode(MODE_LIST)
                    && (
                        !inMode(MODE_INTERNAL_END_PAREN)
                        || inMode(MODE_END_ONLY_AT_RPAREN)
                    )
                    && !inMode(MODE_INITIALIZATION_LIST)
                ) {
                    endMode(MODE_LIST);
                }
            } else if (inTransparentMode(MODE_EXPRESSION | MODE_LIST | MODE_TOP)) {
                endWhileMode(MODE_EXPRESSION | MODE_LIST | MODE_TOP);
            }

            if (!isempty)
                decCurly();
        }
;

/*
  rbracket
*/
rbracket[] { ENTRY_DEBUG } :
        {
            endDownOverMode(MODE_LIST);
        }

        RBRACKET

        {
            if (inMode(MODE_OBJECTIVE_C_CALL)) {
                endDownOverMode(MODE_OBJECTIVE_C_CALL);
            }
        }
;

/*
  period

  Handles the "dot" (period) operator.
*/
period[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOPERATOR);
        }

        PERIOD
;

/*
  member_pointer

  Handles the "->" operator.
*/
member_pointer[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOPERATOR);
        }

        TRETURN
;

/*
  member_pointer_dereference

  Handles the "->*" operator.
*/
member_pointer_dereference[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOPERATOR);
        }

        MPDEREF
;

/*
  dot_dereference

  Handles the ".*" operator.
*/
dot_dereference[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOPERATOR);
        }

        DOTDEREF
;

/*
  dcolon

  Handles a namespace operator ("::").
*/
dcolon[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOPERATOR);
        }

        DCOLON
;

/*
  expression_process

  Used to process a portion of an expression.
*/
expression_process[] { ENTRY_DEBUG } :
        {
            // if expecting an expression, then start one except if you are at a right curly brace
            if (
                inMode(MODE_EXPRESSION | MODE_EXPECT)
                && LA(1) != RCURLY
                && !(
                    inMode(MODE_CONTROL_INCREMENT)
                    && LA(1) == RPAREN
                )
            ) {
                // use a new mode without the expect so we do not nest expression parts
                startNewMode(MODE_EXPRESSION);

                if (inPrevMode(MODE_TERNARY_CONDITION))
                    setMode(MODE_TERNARY_CONDITION);

                // start the expression statement
                startElement(SEXPRESSION);
            }
        }
;

/*
  expression

  Used to handle an expression.
*/
expression[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :
        expression_process

        expression_part_plus_linq[type, call_count]
;

/*
  expression_setup_linq

  Handles setup for a linq expression.
*/
expression_setup_linq[CALL_TYPE type = NOCALL] { ENTRY_DEBUG } :
        expression_process

        expression_part[type]
;

/*
  expression_part_plus_linq

  Handles a linq expression.
*/
expression_part_plus_linq[CALL_TYPE type = NOCALL, int call_count = 1] { ENTRY_DEBUG } :
        {
            inLanguage(LANGUAGE_CSHARP)
            && next_token() != RPAREN
            && next_token() != ASSIGNMENT
            && next_token() != EQUAL
        }?
        (linq_expression_pure) => linq_expression |

        expression_part[type, call_count]
;

/*
  expression_part
*/
expression_part[CALL_TYPE type = NOCALL, int call_count = 1] {
        bool flag;
        bool isempty = false;
        bool end_control_incr = false;

        ENTRY_DEBUG
} :
        {
            !skip_ternary
            && !inMode(MODE_TERNARY_CONDITION)
            && (
                !inLanguage(LANGUAGE_JAVA)
                || !inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST)
            )
            && perform_ternary_check()
        }?
        ternary_expression |

        // cast
        { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
        UNION |

        // cast
        {
            inTransparentMode(MODE_INTERNAL_END_PAREN)
            && (
                LA(1) != CXX_CLASS
                || !keyword_name_token_set.member(next_token())
            )
        }?
        (CLASS | CXX_CLASS) |

        { next_token() == LPAREN || next_token() == LCURLY }?
        delegate_anonymous |

        { next_token() == LCURLY }?
        lambda_anonymous |

        { inLanguage(LANGUAGE_CSHARP) }?
        (lambda_expression_full_csharp) => lambda_expression_csharp |

        { inLanguage(LANGUAGE_CXX) }?
        (
            bracket_pair
            (options { warnWhenFollowAmbig = false; } : paren_pair)*
            function_tail
            LCURLY
        ) => lambda_expression_cpp |

        { inLanguage(LANGUAGE_C_FAMILY) && !inLanguage(LANGUAGE_CSHARP) }?
        (block_lambda_expression_full) => block_lambda_expression |

        { inLanguage(LANGUAGE_JAVA) }?
        ((paren_pair | variable_identifier) TRETURN) => lambda_expression_java |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (NEW generic_argument_list) => sole_new
        generic_argument_list |

        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        (
            NEW
            function_identifier
            paren_pair
            LCURLY
        ) => sole_new
        anonymous_class_definition |

        { notdestructor }?
        sole_destop
        {
            notdestructor = false;
        } |

        { next_token() != LPAREN && next_token() != DOTDOTDOT }?
        sizeof_unary_expression |

        // call
        // need to distinguish between a call and a macro
        {
            type == CALL
            || (
                perform_call_check(type, isempty, call_count, -1)
                && type == CALL
            )
        }?
        // added argument to correct markup of default parameters using a call
        // normally call claims left paren and starts call argument; however, I believe parameter_list matches a right paren of the call
        (call[call_count] | keyword_calls)
        argument |

        // macro call
        { type == MACRO }?
        macro_call |

        { inLanguage(LANGUAGE_OBJECTIVE_C) }?
        objective_c_call |

        // general math operators
        // looks like general operators and variable identifiers can match same thing
        (options { generateAmbigWarnings = false; } :
            general_operators

            {
                if (inLanguage(LANGUAGE_CXX_FAMILY) && LA(1) == DESTOP)
                    general_operators();
            } |

            qmark |
            period |
            member_pointer |
            member_pointer_dereference |
            dot_dereference |
            /* Commented-out code: newop | */

            // left parentheses
            { function_pointer_name_check() }?
            function_pointer_name |

            lparen_marked

            {
                startNewMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);
            }

            // can have (ternary) in a ternary condition
            (options { greedy = true; } :
                {
                    !skip_ternary
                    && inMode(MODE_TERNARY_CONDITION)
                    && (
                        !inLanguage(LANGUAGE_JAVA)
                        || !inTransparentMode(MODE_TEMPLATE_PARAMETER_LIST)
                    )
                    && perform_ternary_check()
                }?
                ternary_expression
            )* |

            // right parentheses that only matches a left parentheses of an expression
            { inTransparentMode(MODE_INTERNAL_END_PAREN) }?
            {
                end_control_incr = inTransparentMode(MODE_CONTROL_INCREMENT);

                // stop at this matching parenthesis, or a preprocessor statement
                endDownToModeSet(MODE_INTERNAL_END_PAREN | MODE_PREPROC);
                
                if (inMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN))
                    endMode(MODE_EXPRESSION | MODE_LIST | MODE_INTERNAL_END_PAREN);

                end_control_incr = end_control_incr && !inTransparentMode(MODE_CONTROL_INCREMENT);
            }

            // treat as operator for operator markup
            rparen[!end_control_incr, end_control_incr] |

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
                if (
                    !inTransparentMode(MODE_CALL)
                    && !inTransparentMode(MODE_INIT)
                    && !inTransparentMode(MODE_FUNCTION_CALL)
                ) {
                    endDownToMode(MODE_INTERNAL_END_CURLY);

                    endMode(MODE_INTERNAL_END_CURLY);
                }
            }
            rcurly_argument |

            // variable or literal
            variable_identifier |

            keyword_name |

            auto_keyword[false] |

            single_keyword_specifier
        ) |

        literals |

        noexcept_list |

        variable_identifier_array_grammar_sub[flag]
;

/*
  literals

  Handles various rules for literals.
*/
literals[] { ENTRY_DEBUG } :
        string_literal | char_literal | literal | boolean | null_literal | complex_literal | nil_literal
;

/*
  string_literal

  Only the start and the end of strings are put directly through the parser.  The contents of the string are handled as whitespace.
*/
string_literal[bool markup = true] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (markup)
                startElement(SSTRING);
        }

        (STRING_START (STRING_END | RAW_STRING_END))
;

/*
  char_literal
*/
char_literal[bool markup = true] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (markup)
                startElement(SCHAR);
        }

        (CHAR_START CHAR_END)
;

/*
  null_literal
*/
null_literal[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNULL);
        }

        (NULLPTR | NULLLITERAL)
;

/*
  nil_literal
*/
nil_literal[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNIL);
        }

        NIL
;

/*
  complex_literal

  Handles complex numbers.
*/
complex_literal[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCOMPLEX);
        }

        COMPLEX_NUMBER

        (
            {
                (
                    LT(1)->getText() == "+"sv
                    || LT(1)->getText() == "-"sv
                )
                && next_token() == CONSTANTS
            }?
            OPERATORS
            CONSTANTS
        )?
;

/*
  literal

  Handles literal numbers.
*/
literal[bool markup = true] { LightweightElement element(this); TokenPosition tp; ENTRY_DEBUG } :
        {
            if (markup) {
                startElement(SLITERAL);

                setTokenPosition(tp);
            }
        }

        CONSTANTS

        (
            {
                (
                    LT(1)->getText() == "+"sv
                    || LT(1)->getText() == "-"sv
                )
                && next_token() == COMPLEX_NUMBER
            }?
            OPERATORS
            COMPLEX_NUMBER

            {
                if (markup)
                    tp.setType(SCOMPLEX);
            }
        )?
;

/*
  boolean

  Handles booleans.
*/
boolean[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SBOOLEAN);
        }

        (LITERAL_TRUE | LITERAL_FALSE)
;

/*
  derived_list

  Handles a "derived_list" class.
*/
derived_list[] { CompleteElement element(this); bool first = true; ENTRY_DEBUG } :
        {
            // end all elements at the end of the rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SDERIVATION_LIST);
        }

        COLON

        (options { greedy = true; } :
            {
                LA(1) != WHERE
                && (
                    !inLanguage(LANGUAGE_OBJECTIVE_C)
                    || first
                )
            }?
            (derived set_bool[first, false]) |

            COMMA
        )*
;

/*
  derived
*/
derived[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // end all elements at the end of the rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation statement
            startElement(SDERIVATION);
        }

        ({ inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }? attribute_cpp)*

        (derive_virtual | derive_access)*

        (
            { inLanguage(LANGUAGE_OBJECTIVE_C) }?
            identifier |

            variable_identifier
        )

        (
            { inLanguage(LANGUAGE_CSHARP) }?
            period
            variable_identifier
        )*

        (options { greedy = true; } :
            { !inLanguage(LANGUAGE_OBJECTIVE_C) }?
            generic_argument_list
        )*
;

/*
  super_list_java
*/
super_list_java[] { ENTRY_DEBUG } :
        {
            // end all elements at the end of the rule automatically
            startNewMode(MODE_LOCAL);

            // start the derivation list
            startElement(SDERIVATION_LIST);
        }
;

/*
  extends_list
*/
extends_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // end all elements at the end of the rule automatically
            startNewMode(MODE_LOCAL);

            // start the extends statement
            startElement(SEXTENDS);
        }

        EXTENDS
        super_list
;

/*
  implements_list
*/
implements_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // end all elements at the end of the rule automatically
            startNewMode(MODE_LOCAL);

            // start the implements statement
            startElement(SIMPLEMENTS);
        }

        IMPLEMENTS
        super_list
;

/*
  permits_list
*/
permits_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // end all elements at the end of the rule automatically
            startNewMode(MODE_LOCAL);

            // start the permits statement
            startElement(SPERMITS);
        }

        PERMITS
        super_list
;


/*
  super_list
*/
super_list[] { ENTRY_DEBUG } :
        (options { greedy = true; } : derived | COMMA)*
;

/*
  derive_access
*/
derive_access[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCLASS_SPECIFIER);
        }

        (PUBLIC | PRIVATE | PROTECTED)
;

/*
  derive_virtual
*/
derive_virtual[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCLASS_SPECIFIER);
        }

        VIRTUAL
;

/*
  parameter_list
*/
parameter_list[] { CompleteElement element(this); bool lastwasparam = false; bool foundparam = false; ENTRY_DEBUG } :
        {
            // list of parameters
            startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);

            // start the parameter list statement
            startElement(SPARAMETER_LIST);
        }

        // parameter list must include all possible parts since it is part of a function detection
        LPAREN

        (
            {
                foundparam = true;

                if (!lastwasparam)
                    empty_element(SPARAMETER, !lastwasparam);

                lastwasparam = false;
            }

            {
                // we are in a parameter list; need to make sure we end it down to the start of the parameter list
                if (!inMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT))
                    endMode();
            }

            comma |

            { inLanguage(LANGUAGE_JAVA) }?
            bar |

            complete_parameter

            {
                foundparam = lastwasparam = true;
            }
        )*

        empty_element[SPARAMETER, !lastwasparam && foundparam]
        rparen[false]
;

/*
  indexer_parameter_list
*/
indexer_parameter_list[] { bool lastwasparam = false; ENTRY_DEBUG } :
        {
            // list of parameters
            startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);

            // start the indexer parameter list statement
            startElement(SINDEXER_PARAMETER_LIST);
        }

        // parameter list must include all possible parts since it is a part of function detection
        LBRACKET

        {
            startNewMode(MODE_LIST);
        }

        (
            {
                if (!lastwasparam)
                    empty_element(SPARAMETER, !lastwasparam);

                lastwasparam = false;
            }

            /* Commented-out code:
            // we are in a parameter list; need to make sure we end it down to the start of the parameter list
            {
                if (!inMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT))
                    endMode();
            } */

            comma |

            complete_parameter

            {
                lastwasparam = true;
            }
        )*
;

/*
  empty_element

  Handles an empty element.
*/
empty_element[int ele, bool cond] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            if (cond)
                startElement(ele);
        }
;

/*
  kr_parameter

  Handles a parameter in K&R syntax (C).
*/
kr_parameter[int type_count] { ENTRY_DEBUG } :
        kr_parameter_type[type_count]
        kr_parameter_name
        kr_parameter_terminate

        /* Commented-out code: complete_parameter terminate_pre terminate_token */
;

/*
  kr_parameter_type

  Handles a parameter type in K&R syntax (C).
*/
kr_parameter_type[int type_count] { ENTRY_DEBUG } :
        // suppress ()* warning
        variable_declaration_statement[type_count]

        (options { greedy = true; } :
            { inMode(MODE_EAT_TYPE) }?
            type_identifier
            update_typecount[MODE_FUNCTION_NAME]
        )*
;

/*
  kr_parameter_name

  Handles a parameter name in K&R syntax (C).
*/
kr_parameter_name[] { ENTRY_DEBUG } :
        ((comma)* variable_declaration_nameinit)*
;

/*
  kr_parameter_terminate

  Handles a parameter terminate in K&R syntax (C).
*/
kr_parameter_terminate[] { ENTRY_DEBUG } :
        terminate_pre
        terminate_token

        {
            endDownToModeSet(MODE_FUNCTION_TAIL);
        }
;

/*
  complete_parameter
*/
complete_parameter[] { ENTRY_DEBUG } :
        parameter

        // suppress ()* warning
        (options { greedy = true; } :
            parameter_declaration_initialization
            complete_default_parameter
        )*
;

/*
  argument
*/
argument[] { ENTRY_DEBUG } :
        { getParen() == 0 }?
        rparen[false] |

        { getCurly() == 0 }?
        rcurly_argument |

        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        }

        (options { greedy = true; } :
            { inLanguage(LANGUAGE_CSHARP) && look_past_rule(&srcMLParser::identifier) == COLON }?
            argument_named_csharp
        )*

        (options { greedy = true; } :
            { inLanguage(LANGUAGE_CSHARP) }?
            argument_modifier_csharp
        )*

        (
            {
                !(
                    (
                        LA(1) == RPAREN
                        && inTransparentMode(MODE_INTERNAL_END_PAREN)
                    )
                    || (
                        LA(1) == RCURLY
                        && inTransparentMode(MODE_INTERNAL_END_CURLY)
                    )
                )
            }?
            expression |

            (type_identifier) => expression_process
            type_identifier
        )
;

/*
  argument_modifier_csharp
*/
argument_modifier_csharp[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SMODIFIER);
        }

        (OUT | REF)
;

/*
  argument_named_csharp
*/
argument_named_csharp[] { ENTRY_DEBUG } :
        identifier
        COLON
;

/*
  annotation_argument
*/
annotation_argument[] { ENTRY_DEBUG } :
        { getParen() == 0 }?
        rparen[false] |

        {
            // argument with nested expression
            startNewMode(MODE_ARGUMENT | MODE_EXPRESSION | MODE_EXPECT);

            // start the argument
            startElement(SARGUMENT);
        }

        // suppress warning of ()*
        (options { greedy = true; } :
            { inputState->guessing }?
            RCURLY |

            { !(LA(1) == RPAREN) }?
            expression |

            type_identifier
        )*
;

/*
  parameter
*/
parameter[] {
        int type_count = 0;
        int secondtoken = 0;
        int after_token = 0;
        STMT_TYPE stmt_type = NONE;

        ENTRY_DEBUG
} :
        {
            // end parameter correctly
            startNewMode(MODE_PARAMETER);

            // start the parameter statement
            startElement(SPARAMETER);
        }

        (
            {
                pattern_check(stmt_type, secondtoken, type_count, after_token, true)
                && (
                    stmt_type == FUNCTION_DECL
                    || stmt_type == FUNCTION
                    || stmt_type == OPERATOR_FUNCTION_DECL
                    || stmt_type == OPERATOR_FUNCTION
                )
            }?
            function_declaration[type_count]
            function_identifier
            // Commented-out code: pointer_name_grammar
            (macro_call_check)*
            parameter_list |

            parameter_type_variable[type_count, stmt_type]
        )
;

/*
  parameter_type_variable

  Handles a parameter type if it is not a FUNCTION_DECL.
*/
parameter_type_variable[int type_count, STMT_TYPE stmt_type] { bool output_type = true; ENTRY_DEBUG } :
        {
            // start the declaration statement
            startElement(SDECLARATION);

            if (stmt_type != VARIABLE)
                type_count = 1;

            int look_past_token = 0;

            output_type = !(
                (
                    inLanguage(LANGUAGE_JAVA)
                    || inLanguage(LANGUAGE_CSHARP)
                )
                && type_count == 1
                && LA(1) != DOTDOTDOT
                && inTransparentMode(MODE_FUNCTION_TAIL | MODE_ANONYMOUS)
                && (
                    (look_past_token = look_past_rule(&srcMLParser::type_identifier)) == COMMA
                    || look_past_token == RPAREN
                    || look_past_token == TRETURN
                    || look_past_token == LAMBDA
                )
            );
        }

        (
            {
                stmt_type == VARIABLE
                || stmt_type == CLASS_DECL
                || stmt_type == STRUCT_DECL
                || stmt_type == UNION_DECL
                || stmt_type == ENUM_DECL
                || LA(1) == DOTDOTDOT
            }?
            (parameter_type_count[type_count, output_type])

            // suppress warning caused by ()*
            (options { greedy = true; } :
                bar
                set_int[type_count, type_count > 1 ? type_count - 1 : 1]
                parameter_type_count[type_count]
            )*

            {
                // expect a name initialization
                setMode(MODE_VARIABLE_NAME | MODE_INIT);
            }

            (options { greedy = true; } : variable_declaration_nameinit)*
        )
;

/*
  parameter_type_count

  Used to count types in a parameter.
*/
parameter_type_count[int& type_count, bool output_type = true] {
        CompleteElement element(this);
        bool is_compound = false;
        
        ENTRY_DEBUG
} :
        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);

            // start of type
            if (output_type)
                startElement(STYPE);
        }

        // match auto keyword first as a special case; do not warn about ambiguity
        (
            (options { generateAmbigWarnings = false; } :
                this_specifier |

                auto_keyword[type_count > 1] |

                { is_class_type_identifier() }?
                (options { greedy = true; } :
                    { !class_tokens_set.member(LA(1)) }?
                    (options { generateAmbigWarnings = false; } :
                        specifier |

                        { look_past_rule(&srcMLParser::identifier) != LPAREN }?
                        identifier |

                        macro_call
                    )

                    set_int[type_count, type_count - 1]
                )*
                class_type_identifier[is_compound]
                set_int[type_count, type_count - 1]
                (options { greedy = true; } : { !is_compound }? multops)* |

                type_identifier
            )

            set_int[type_count, type_count - 1]
            (options { greedy = true; } : eat_type[type_count])?
        )

        // sometimes there is no parameter name; if so, we need to eat it
        (options { greedy = true; generateAmbigWarnings = false; } :
            multops |

            tripledotop |

            LBRACKET
            RBRACKET |

            {
                next_token() == MULTOPS
                || next_token() == REFOPS
                || next_token() == RVALUEREF
                || (
                    inLanguage(LANGUAGE_CSHARP)
                    && next_token() == QMARK
                )
                || next_token() == BLOCKOP
            }?
            type_identifier
        )*
;

/*
  multops

  A modifier operation.
*/
multops[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SMODIFIER);
        }

        (
            MULTOPS |

            REFOPS |

            RVALUEREF |

            { inLanguage(LANGUAGE_CSHARP) }?
            QMARK
            set_bool[is_qmark, true] |

            BLOCKOP
        )
;

/*
  multopblockop

  A modifier operation.
*/
multopblockop[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SMODIFIER);
        }

        (MULTOPS | BLOCKOP)
;

/*
  modifiers_csharp
*/
modifiers_csharp[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SMODIFIER);
        }

        (REF | OUT)
;

/*
  tripledotop

  Handles a triple dot ("...") operator.
*/
tripledotop[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SMODIFIER);
        }

        DOTDOTDOT
;

/*
  parameter_type

  Handles a parameter type.
*/
parameter_type[] {
        CompleteElement element(this);
        int type_count = 0;
        int secondtoken = 0;
        int after_token = 0;
        STMT_TYPE stmt_type = NONE;
        bool is_compound = false;

        ENTRY_DEBUG
} :
        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);

            // start of type
            startElement(STYPE);
        }

        {
            pattern_check(stmt_type, secondtoken, type_count, after_token)
            && (type_count ? type_count : (type_count = 1))
        }?
        // match auto keyword first as a special case; do not warn about ambiguity
        (
            (options { generateAmbigWarnings = false; } :
                auto_keyword[type_count > 1] |

                { is_class_type_identifier() }?
                (options { greedy = true; } :
                    { !class_tokens_set.member(LA(1)) }?
                    (options { generateAmbigWarnings = false; } :
                        specifier |

                        { look_past_rule(&srcMLParser::identifier) != LPAREN }?
                        identifier |

                        macro_call
                    )

                    set_int[type_count, type_count - 1]
                )*
                class_type_identifier[is_compound]
                set_int[type_count, type_count - 1]
                (options { greedy = true; } : { !is_compound }? multops)* |

                type_identifier
            )

            set_int[type_count, type_count - 1]
            (options { greedy = true; } : eat_type[type_count])?
        )
;

/*
  template_declaration
*/
template_declaration[] { ENTRY_DEBUG } :
        {
            // template with a nested statement (function or class); expect a template parameter list
            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TEMPLATE);

            // start the template
            startElement(STEMPLATE);
        }

        TEMPLATE

        {
            if (LA(1) == CLASS || LA(1) == CXX_CLASS)
                startNewMode(MODE_TEMPLATE | MODE_LIST | MODE_EXPECT);
            else
                startNewMode(MODE_TEMPLATE | MODE_LIST | MODE_EXPECT | MODE_TEMPLATE_PARAMETER_LIST);
        }
;

/*
  template_param_list

  Starts a parameter list for templates.
*/
template_param_list[] { ENTRY_DEBUG } :
        {
            startNewMode(MODE_PARAMETER | MODE_LIST);

            // start the template parameter list
            startElement(STEMPLATE_PARAMETER_LIST);
        }

        tempops
;

/*
  template_param

  Handles a parameter in a template.
*/
template_param[] { in_template_param = true; ENTRY_DEBUG } :
        {
            // end the parameter correctly
            startNewMode(MODE_LOCAL);

            // start the template parameter statement
            startElement(STEMPLATE_PARAMETER);
        }

        // Both can contain extern; however, an extern template should not be a template param (so it should not be a problem)
        (options { generateAmbigWarnings = false; } :
            { LA(1) != TEMPLATE }?
            parameter_type

            {
                // expect a name initialization
                setMode(MODE_VARIABLE_NAME | MODE_INIT);
            }

            (options { greedy = true; } :
                { true }?
                variable_declaration_nameinit
            )* |

            template_inner_full
        )

        set_bool[in_template_param, false]
;
exception
catch[antlr::RecognitionException&] {
        in_template_param = false;
        throw antlr::RecognitionException();
}

/*
  template_inner_full
*/
template_inner_full[] {
        ENTRY_DEBUG

        int type_count = 0;
        int secondtoken = 0;
        int after_token = 0;
        STMT_TYPE stmt_type = NONE;
} :
        template_in_parameter_list_full

        {
            pattern_check(stmt_type, secondtoken, type_count, after_token)
            && (type_count ? type_count : (type_count = 1))
        }?
        eat_type[type_count]

        {
            endMode();

            // expect a name initialization
            setMode(MODE_VARIABLE_NAME | MODE_INIT);
        }

        (options { greedy = true; } :
            { true }?
            variable_declaration_nameinit
        )*
;

/*
  template_in_parameter_list_full

  Handles an entire template parameter list.
*/
template_in_parameter_list_full[] { ENTRY_DEBUG } :
        {
            // local mode so start element will end correctly
            startNewMode(MODE_LOCAL);

            // start of a type
            startElement(STYPE);
        }

        template_declaration_full
;

/*
  template_declaration_full
*/
template_declaration_full[] { ENTRY_DEBUG } :
        template_declaration
        template_param_list

        (
            template_param
            (template_declaration_initialization)*
            (comma)*
        )*

        tempope

        { if (inMode(MODE_TEMPLATE))
            endMode();
        }
;

/*
  template_declaration_initialization
*/
template_declaration_initialization[] { ENTRY_DEBUG } :
        {
            // end the init correctly
            setMode(MODE_EXPRESSION | MODE_EXPECT);

            // start the declaration initialization statement
            startElement(SDECLARATION_INITIALIZATION);
        }

        EQUAL
        expression
;

/*
  generic_argument_list_check
*/
generic_argument_list_check[] returns [bool is_generic_argument_list] {
        is_generic_argument_list = false;

        int start = mark();
        inputState->guessing++;

        int parencount = 0;
        int bracecount = 0;

        while (LA(1) != antlr::Token::EOF_TYPE) {
            if (LA(1) == RPAREN)
                --parencount;
            else if (LA(1) == LPAREN)
                ++parencount;

            if (parencount < 0) {
                break;
            }

            if (LA(1) == TEMPOPE) {
                is_generic_argument_list = true;
                break;
            }

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

        inputState->guessing--;
        rewind(start);

        ENTRY_DEBUG
} :;

/*
        Commented-out code:

        savenamestack[namestack_save]
        tempops
        (options { generateAmbigWarnings = false; } : COMMA | template_argument[in_function_type])*
        tempope
        restorenamestack[namestack_save]
*/

/*
  generic_argument_list
*/
generic_argument_list[] {
        CompleteElement element(this);
        decltype(namestack) namestack_save;
        bool in_function_type = false;

        ENTRY_DEBUG
} :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            in_function_type = inPrevMode(MODE_FUNCTION_TYPE);

            if (
                !inLanguage(LANGUAGE_JAVA)
                || (
                    !inTransparentMode(MODE_CLASS_NAME)
                    && !in_function_type)
            )
                startElement(SGENERIC_ARGUMENT_LIST);
            else
                startElement(STEMPLATE_PARAMETER_LIST);
        }

        savenamestack[namestack_save]

        tempops
        (options { generateAmbigWarnings = false; } :
            COMMA |
            
            template_argument[in_function_type]
        )*
        tempope

        restorenamestack[namestack_save]
;

/*
  generic_parameter_list
*/
generic_parameter_list[] { CompleteElement element(this); decltype(namestack) namestack_save; ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SGENERIC_PARAMETER_LIST);
        }

        savenamestack[namestack_save]

        tempops
        (options { generateAmbigWarnings = false; } :
            COMMA |

            generic_parameter
        )*
        tempope

        restorenamestack[namestack_save]
;

/*
  generic_parameter
*/
generic_parameter[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(STEMPLATE_PARAMETER);
        }

        (options { greedy = true; } :
            { LA(1) != SUPER && LA(1) != QMARK }?
            (options { generateAmbigWarnings = false; } : generic_specifiers_csharp)*
            (
                (options { generateAmbigWarnings = false; } :
                    { LA(1) != IN }?
                    template_operators
                )*

                (type_identifier | literals)

                (options { generateAmbigWarnings = false; } : template_operators)*
            ) |

            template_extends_java |

            template_super_java |

            qmark_name |

            template_argument_expression
        )+
;

/*
  cuda_argument_list
*/
cuda_argument_list[] { CompleteElement element(this); decltype(namestack) namestack_save; ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SCUDA_ARGUMENT_LIST);
        }

        savenamestack[namestack_save]

        cuda_start
        (options { generateAmbigWarnings = false; } : COMMA | template_argument)*
        cuda_end

        restorenamestack[namestack_save]
;

/*
  cuda_start

  Handles the beginning of a CUDA argument list.
*/
cuda_start[] { ENTRY_DEBUG } :
        {
            // ensure we are in a list mode so that we can end correctly; some uses of tempope will have their own mode
            if (!inMode(MODE_LIST))
                startNewMode(MODE_LIST);
        }

        CUDA
;

/*
  cuda_end

  Handles the end of a CUDA argument list.
*/
cuda_end[] { ENTRY_DEBUG } :
        {
            // end down to the mode created by the cuda_start argument list operator
            endDownToMode(MODE_LIST);
        }

        TEMPOPE
        TEMPOPE
        TEMPOPE

        {
            // end the mode created by the cuda_start argument list operator
            while (inMode(MODE_LIST))
                endMode(MODE_LIST);
        }
;

/*
  generic_type_constraint
*/
generic_type_constraint[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SWHERE);
        }

        WHERE
        compound_name_inner[false]
        COLON

        type_constraint
        (options { greedy = true; } :
            COMMA
            type_constraint
        )*
;

/*
  type_constraint
*/
type_constraint[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SCONSTRAINT);
        }

        (compound_name_inner[false] | CLASS | CXX_CLASS | STRUCT | new_constraint)
;

/*
  new_constraint
*/
new_constraint[] { ENTRY_DEBUG } :
        new_name
        LPAREN
        RPAREN
;

/*
  new_name
*/
new_name[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SNAME);
        }

        NEW
;

/*
  protocol_list
*/
protocol_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL | MODE_LIST);

            startElement(SPROTOCOL_LIST);
        }

        TEMPOPS

        identifier
        (COMMA identifier)*

        TEMPOPE
;

/*
  category
*/
category[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            startElement(SCATEGORY);
        }

        LPAREN
        (identifier)*
        RPAREN
;

/*
  savenamestack

  Used to save the namestack.
*/
savenamestack[decltype(namestack)& namestack_save] {
        namestack_save.swap(namestack);
        ENTRY_DEBUG
} :;

/*
  restorenamestack

  Used to restore the namestack.
*/
restorenamestack[decltype(namestack)& namestack_save] {
        namestack.swap(namestack_save);
        ENTRY_DEBUG
} :;

/*
  clearnamestack

  Used to clear the namestack.
*/
clearnamestack[] {
        namestack.fill("");
        ENTRY_DEBUG
} :;

/*
  template_argument
*/
template_argument[bool in_function_type = false] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // local mode
            startNewMode(MODE_LOCAL);

            if (
                !inLanguage(LANGUAGE_JAVA)
                || (
                    !inTransparentMode(MODE_CLASS_NAME)
                    && !in_function_type
                )
            )
               startElement(SGENERIC_ARGUMENT);
            else
               startElement(STEMPLATE_PARAMETER);

            if (inLanguage(LANGUAGE_CXX) || inLanguage(LANGUAGE_C))
               startElement(SEXPRESSION);
        }

        (options { greedy = true; } :
            { LA(1) != SUPER && LA(1) != QMARK }?
            (options { generateAmbigWarnings = false; } : generic_specifiers_csharp)*
            (
                (options { generateAmbigWarnings = false; } :
                    { LA(1) != IN }?
                    template_operators
                )*
                (
                    type_identifier |

                    { !inLanguage(LANGUAGE_JAVA) }?
                    literals
                )
                (options { generateAmbigWarnings = false; } : template_operators)*
            ) |

            template_extends_java |

            template_super_java |

            qmark_name |

            { !inLanguage(LANGUAGE_JAVA) }?
            template_argument_expression
        )+
;

/*
  template_argument_expression
*/
template_argument_expression[] { ENTRY_DEBUG } :
        lparen_marked

        // qmark matches before template argument expression, which is fine
        (
            { LA(1) != RPAREN }?
            (
                options { generateAmbigWarnings = false; } :
                    general_operators |

                    qmark |

                    (variable_identifier) => variable_identifier |

                    literals |

                    type_identifier |

                    template_argument_expression |

                    COMMA
            )
        )*

        rparen_operator[true]
;

/*
  template_operators

  Handles all possible template operators.
*/
template_operators[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOPERATOR);
        }

        (
            OPERATORS | TRETURN | TEMPOPS | EQUAL | MULTOPS | REFOPS | DOTDOT | RVALUEREF |
            QMARK | NEW | DELETE | IN | IS | STACKALLOC | AS | AWAIT | LAMBDA
        )
;

/*
  generic_specifiers_csharp
*/
generic_specifiers_csharp[] { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(SFUNCTION_SPECIFIER);
        }

        (IN | OUT)
;

/*
  template_extends_java
*/
template_extends_java[] { CompleteElement element(this); bool is_compound = false; ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SEXTENDS);
        }

        EXTENDS
        (annotation)*
        compound_name_java[is_compound]
;

/*
  template_super_java
*/
template_super_java[] { CompleteElement element(this); bool is_compound = false; ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SDERIVATION);
        }

        SUPER
        (annotation)*
        compound_name_java[is_compound]
;

/*
  tempops

  Handles the beginning of a template parameter list.
*/
tempops[] { ENTRY_DEBUG } :
        {
            // ensure we are in a list mode so that we can end correctly; some uses of tempope will have their own mode
            if (!inMode(MODE_LIST))
                startNewMode(MODE_LIST);
        }

        TEMPOPS
;

/*
  tempope

  Handles the end of a template parameter list.
*/
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

/*
  label_statement
*/
label_statement[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the label statement
            startElement(SLABEL_STATEMENT);
        }

        (identifier | keyword_identifier)
        COLON
;

/*
  macro_label_statement
*/
macro_label_statement[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT);

            // start the label statement
            startElement(SLABEL_STATEMENT);
        }

        macro_label_call
        COLON
;

/*
  typedef_statement
*/
typedef_statement[] { ENTRY_DEBUG } :
        {
            // statement
            startNewMode(MODE_STATEMENT | MODE_EXPECT | MODE_VARIABLE_NAME | MODE_ONLY_END_TERMINATE);

            // start the typedef statement
            startElement(STYPEDEF);

            startNewMode(MODE_STATEMENT | MODE_NEST | MODE_TYPEDEF | MODE_END_AT_BLOCK_NO_TERMINATE);
        }

        TYPEDEF
;

/*
  paren_pair

  Used to match a set of parentheses.
*/
paren_pair[] { ENTRY_DEBUG } :
        LPAREN

        (
            paren_pair |

            qmark |

            ~(QMARK | LPAREN | RPAREN)
        )*

        RPAREN
;

/*
  curly_pair

  Used to match a set of curly braces.
*/
curly_pair[] { ENTRY_DEBUG } :
        LCURLY

        (
            curly_pair |

            qmark |

            ~(QMARK | LCURLY | RCURLY)
        )*

        RCURLY
;

/*
  bracket_pair

  Used to match a set of brackets.
*/
bracket_pair[] { ENTRY_DEBUG } :
        LBRACKET

        (
            bracket_pair |

            qmark |

            ~(QMARK | LBRACKET | RBRACKET)
        )*

        RBRACKET
;

/*
  nested_terminate

  Used to see if there is a semicolon terminating a statement inside of a block at the top level.
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

            if (
                LA(1) == TERMINATE
                && parencount == 0
                && bracecount == 0
            )
                break;

            consume();
        }
} :
        TERMINATE
;

/*
  enum_preprocessing
*/
enum_preprocessing[bool decl] { ENTRY_DEBUG } :
        {
            bool intypedef = inMode(MODE_TYPEDEF);

            if (intypedef)
                startElement(STYPE);

            // statement
            auto mode = MODE_STATEMENT | MODE_NEST | MODE_BLOCK | MODE_ENUM | MODE_DECL;

            if (inLanguage(LANGUAGE_CSHARP))
                mode |= MODE_END_AT_BLOCK_NO_TERMINATE;

            startNewMode(mode);

            // start the enum definition
            if (
                inLanguage(LANGUAGE_CXX)
                && (
                    next_token() == CLASS
                    || next_token() == CXX_CLASS
                    || next_token() == STRUCT
                    || next_token() == UNION
                )
            ) {
                if (decl)
                    startElement(SENUM_CLASS_DECLARATION);
                else
                    startElement(SENUM_CLASS);
            } else if (decl) {
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

/*
  enum_definition
*/
enum_definition[] { ENTRY_DEBUG } :
        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        enum_class_definition |

        { inLanguage(LANGUAGE_CSHARP) }?
        enum_csharp_definition |

        enum_preprocessing[false]
        (options { greedy = true; } : specifier)*
        ENUM
        (options { greedy = true; } : enum_class_header)*
;

/*
  enum_declaration
*/
enum_declaration[] { ENTRY_DEBUG } :
        { inLanguage(LANGUAGE_JAVA_FAMILY) }?
        enum_class_declaration |

        { inLanguage(LANGUAGE_CSHARP) }?
        enum_csharp_declaration |

        enum_preprocessing[true]
        (options { greedy = true; } : specifier)*
        ENUM
        (options { greedy = true; } : enum_class_header)*
;

/*
  enum_class_header
*/
enum_class_header[] {} :
        (CLASS | CXX_CLASS | STRUCT | UNION)*
        ({ inLanguage(LANGUAGE_CXX) && next_token() == LBRACKET }? attribute_cpp)*

        variable_identifier
        (COLON enum_type)*

        (options { greedy = true; } :
            COMMA
            variable_identifier
            (COLON enum_type)*
        )*
;

/*
  enum_type
*/
enum_type { LightweightElement element(this); ENTRY_DEBUG } :
        {
            startElement(STYPE);
        }

        // suppress warning that compound_name seems to have some tokens in common with specifier
        (options { generateAmbigWarnings = false; } : specifier | compound_name)*
;

/*
  enum_csharp_definition

  May need to be modified to work with ENUM_DECL.
*/
enum_csharp_definition[] { ENTRY_DEBUG } :
        enum_preprocessing[false]
        class_preamble
        ENUM

        (options { greedy = true; } : variable_identifier)*

        (
            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            (options { greedy = true; } : derived_list)
        )*
;

/*
  enum_csharp_declaration

  May need to be modified to work with ENUM_DECL.
*/
enum_csharp_declaration[] { ENTRY_DEBUG } :
        enum_preprocessing[true]
        class_preamble
        ENUM

        (options { greedy = true; } : variable_identifier)*

        (
            { inLanguage(LANGUAGE_CXX_FAMILY) }?
            (options { greedy = true; } : derived_list)
        )*

        (
            COMMA
            (options { greedy = true; } : variable_identifier)*

            (
                { inLanguage(LANGUAGE_CXX_FAMILY) }?
                (options { greedy = true; } : derived_list)
            )*
        )*
;

/*
  enum_definition_complete

  Used for "enum"s embedded in "typedef"s where the entire enum must be parsed since it is part of the type.
*/
enum_definition_complete[] { CompleteElement element(this); ENTRY_DEBUG } :
        enum_definition

        (variable_identifier)*

        // start of the enum definition block
        {
            startNewMode(MODE_TOP | MODE_LIST | MODE_DECL | MODE_EXPECT | MODE_BLOCK | MODE_NEST);

            startElement(SBLOCK);
        }

        LCURLY

        (options { greedy = true; } :
            { LA(1) != RCURLY || inTransparentMode(MODE_INTERNAL_END_CURLY) }?
            expression |

            comma
        )*

        // end of the enum definition block
        {
            endDownToMode(MODE_TOP);
        }

        RCURLY
;

/*
  enum_block

  Handles the beginning and setup of an enum block.
*/
enum_block[] { ENTRY_DEBUG } :
        lcurly_base[false]

        {
            // nesting blocks, not statements
            setMode(MODE_TOP | MODE_STATEMENT | MODE_NEST | MODE_LIST | MODE_BLOCK | MODE_ENUM);
        }
;

/*
  enum_short_variable_declaration

  Handles processing for a short enum variable declaration.
*/
enum_short_variable_declaration[] { ENTRY_DEBUG } :
        {
            // variable declarations may be in a list
            startNewMode(MODE_ENUM | MODE_LIST | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // declaration
            startNewMode(MODE_ENUM | MODE_LOCAL | MODE_VARIABLE_NAME | MODE_INIT | MODE_EXPECT);

            // start the declaration statement
            startElement(SDECLARATION);
        }

        variable_declaration_nameinit
;

/*
  eof

  Reached the end of the input.  Need to make sure to end any open elements.
  Open elements indicate either a syntax error in the code, or a translation error.

  EOF marks the end of all processing, so it must occur after any ending modes.
*/
eof[] { ENTRY_DEBUG } :
        {
            endAllModes();
        }

        EOF
;

/*
  preprocessor

  Used to match on the directive itself (not the entire directive).
*/
preprocessor[] {
        ENTRY_DEBUG

        int directive_token = 0;
        bool markblockzero = false;

        TokenPosition tp;
        TokenPosition tp_directive;

        // parse the end of line
        startNewMode(MODE_PARSE_EOL);

        // mode for any preprocessor elements
        startNewMode(MODE_PREPROC);
} :
        {
            // assume there is an error; will properly set one later
            startElement(SCPP_EMPTY);

            setTokenPosition(tp);
        }

        PREPROC
        markend[directive_token]

        {
            startNewMode(MODE_LOCAL);

            startElement(SCPP_DIRECTIVE);

            setTokenPosition(tp_directive);
        }

        // suppress warnings that should be caused by the empty rule
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
            (options { greedy = true; } :
                cpp_define_name
                (options { greedy = true; } : cpp_define_value)*
            )* |

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
            {
                markblockzero = false;

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
            (options { generateAmbigWarnings = false; } :
                cpp_linenumber
                (cpp_filename)* |

                { inLanguage(LANGUAGE_CSHARP) }?
                cpp_symbol_optional
            ) |

            PRAGMA
            {
                endMode();

                tp.setType(SCPP_PRAGMA);
            }
            (
                (OMP_OMP) => omp_directive |

                (CHAR_START | STRING_START | TEMPOPS) => cpp_literal |

                cpp_symbol
            )* |

            ERRORPREC
            {
                endMode();

                tp.setType(SCPP_ERROR);
            }
            (cpp_literal)* |

            WARNING
            {
                endMode();

                tp.setType(SCPP_WARNING);
            }
            (cpp_literal)* |

            REGION
            {
                endMode();

                tp.setType(SCPP_REGION);
            }
            (cpp_symbol)* |

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

            // blank preproc; suppress ()* warning
            {
                tp_directive.setType(SNOP);
                endMode();
            }

            (options { greedy = true; } : cpp_garbage)*
        )

        eol_skip[directive_token, markblockzero]
;
exception
catch[...] {
        eol_skip(directive_token, markblockzero);
}

/*
  cpp_garbage

  Handles all of the C++ garbage.
*/
cpp_garbage[] :
        ~(EOL | LINE_COMMENT_START | BLOCK_COMMENT_START | JAVADOC_COMMENT_START | DOXYGEN_COMMENT_START | LINE_DOXYGEN_COMMENT_START)
;

/*
  cpp_check_end
*/
cpp_check_end[] returns [bool is_end = false] {
        if (
            LA(1) == EOL
            || LA(1) == LINE_COMMENT_START
            || LA(1) == BLOCK_COMMENT_START
            || LA(1) == JAVADOC_COMMENT_START
            || LA(1) == DOXYGEN_COMMENT_START
            || LA(1) == LINE_DOXYGEN_COMMENT_START
            || LA(1) == EOF
            || LA(1) == 1
        ) /* EOF */ {
            return true;
        }
} :;

/*
  eol_skip

  Used to skip to the EOL.
*/
eol_skip[int directive_token, bool markblockzero] {
        while (
            LA(1) != EOL
            && LA(1) != LINE_COMMENT_START
            && LA(1) != BLOCK_COMMENT_START
            && LA(1) != JAVADOC_COMMENT_START
            && LA(1) != DOXYGEN_COMMENT_START
            && LA(1) != LINE_DOXYGEN_COMMENT_START
            && LA(1) != 1 /* EOF? */
        ) {
            consume();
        }

        ENTRY_DEBUG
} :
        eol[directive_token, markblockzero]
;

/*
  eol

  Only used for ending a preprocessor, and only those directives that end on the current line.
*/
eol[int directive_token, bool markblockzero] {
        // end all preprocessor modes
        endWhileMode(MODE_PREPROC);

        endMode(MODE_PARSE_EOL);

        ENTRY_DEBUG
} :
        {
            if (directive_token == ENDIF) {
                bool end_statement = inMode(MODE_END_AT_ENDIF);

                while (inMode(MODE_END_AT_ENDIF))
                    endMode();

                if (end_statement)
                    else_handling();
            }

            if (LA(1) == 1) {
                eol_post(directive_token, markblockzero);
                return;
            }
        }

        (EOL | LINE_COMMENT_START | BLOCK_COMMENT_START | JAVADOC_COMMENT_START | DOXYGEN_COMMENT_START | LINE_DOXYGEN_COMMENT_START | eof)
        eol_post[directive_token, markblockzero]
;

/*
  cppendif_skip
*/
cppendif_skip[] {
        int prev = -1;
        int endif_count = 1;

        while (endif_count && LA(1) != 1 /* EOF */) {
            if (
                (
                    prev == PREPROC
                    && LA(1) == IF
                )
                || LA(1) == IFDEF
                || LA(1) == IFNDEF
            )
                ++endif_count;

            if (LA(1) == ENDIF)
                --endif_count;

            prev = LA(1);
            consume();
        }
} :;

/*
  cppif_end_count_check
*/
cppif_end_count_check[] returns [std::list<int> end_order] {
        int start = mark();
        std::list<int> op_stack;
        ++inputState->guessing;

        std::list<int>::size_type save_size = 0;

        int prev = -1;

        while (
            LA(1) != ENDIF
            && !(
                prev == PREPROC
                && LA(1) == ELSE
            )
            && LA(1) != 1 /* EOF */
        ) {
            if (
                (
                    prev == PREPROC
                    && LA(1) == IF
                )
                || LA(1) == IFDEF
                || LA(1) == IFNDEF
            ) {
                cppendif_skip();
                continue;
            }

            if (LA(1) == ELIF)
                save_size = end_order.size();

            if (LA(1) == LPAREN)
                op_stack.push_back(LPAREN);

            if (LA(1) == RPAREN) {
                if (!op_stack.empty() && op_stack.back() == LPAREN)
                    op_stack.pop_back();
                else
                    end_order.push_back(RPAREN);
            }

            if (LA(1) == LCURLY)
                op_stack.push_back(LCURLY);

            if (LA(1) == RCURLY) {
                if (!op_stack.empty() && op_stack.back() == LCURLY)
                    op_stack.pop_back();
                else
                    end_order.push_back(RCURLY);
            }

            if (
                LA(1) == TERMINATE
                && !wait_terminate_post
                && (
                    inTransparentMode(MODE_EXPRESSION | MODE_STATEMENT)
                    || inMode(MODE_END_CONTROL)
                )
            ) {
                end_order.push_back(TERMINATE);
            }

            prev = LA(1);
            consume();
        }

        if (LA(1) == 1 /* EOF */) {
            end_order.clear();
        }

        if (LA(1) == ENDIF)
            end_order.resize(save_size);

        while (!op_stack.empty() && !end_order.empty()) {
            op_stack.pop_front();
            end_order.pop_front();
        }

        --inputState->guessing;

        rewind(start);

        ENTRY_DEBUG
} :;

/*
  eol_post

  Handles post-processing for EOL.
*/
eol_post[int directive_token, bool markblockzero] {
        // flags used to control "#if 0" and "#else" skipping
        // once in these modes, stay in these modes until the matching "#endif" is reached
        // cpp_ifcount is used to indicate which "#endif" matches the "#if" or "#else"
        switch (directive_token) {
            case IF :

            case IFDEF :

            case IFNDEF :
                // should work unless also creates a dangling lcurly or lparen, in which case may need to run on everything except else
                // leaving off for now, with no option; test thoroughly, and then turn on by default
                #ifdef _MSC_VER
                #pragma warning (push, 0)
                #endif

                if (false && !inputState->guessing) {
                    #ifdef _MSC_VER
                    #pragma warning (pop)
                    #endif

                    for (auto& item : cppif_end_count_check()) {
                        if (item == RCURLY) {
                            setMode(MODE_TOP | MODE_STATEMENT | MODE_NEST | MODE_LIST | MODE_BLOCK);

                            startNewMode(getMode() | MODE_ISSUE_EMPTY_AT_POP);

                            addElement(SBLOCK);
                        }

                        if (inTransparentMode(MODE_CONDITION) && item == RPAREN) {
                            std::stack<int> open_elements;
                            open_elements.push(SCONDITION);

                            if (number_finishing_elements) {
                                finish_elements_add.push_back(
                                    std::make_pair(
                                        MODE_CONDITION | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_ISSUE_EMPTY_AT_POP,
                                        open_elements
                                    )
                                );
                            } else {
                                insertModeAfter(
                                    MODE_CONDITION | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT,
                                    MODE_CONDITION | MODE_LIST | MODE_EXPRESSION | MODE_EXPECT | MODE_ISSUE_EMPTY_AT_POP,
                                    open_elements
                                );
                            }
                        }

                        if (item == TERMINATE) {
                            if (inMode(MODE_CONDITION) && inPrevMode(MODE_IF)) {
                                cppif_duplicate = true;
                            } else if (inMode(MODE_LIST | MODE_CONDITION) && inPrevMode(MODE_STATEMENT | MODE_NEST)) {
                                cppif_duplicate = true;
                            } else if (inMode(MODE_END_CONTROL) || inMode(MODE_LIST | MODE_CONTROL_CONDITION)) {
                                cppif_duplicate = true;
                            } else {
                                dupDownOverMode(MODE_STATEMENT);
                            }
                        }
                    }
                }

                // start a new blank mode for the new zeroed blocks
                if (!cpp_zeromode && markblockzero) {
                    // start a new blank mode for if
                    cpp_zeromode = true;

                    // keep track of nested "if"s (inside the "#if 0") so we know when we reach the proper "#endif"
                    cpp_ifcount = 0;
                }

                // found another if
                ++cpp_ifcount;

                // create new context for "#if" (and possible "#else")
                if (!isoption(parser_options, SRCML_PARSER_OPTION_CPP_TEXT_ELSE) && !inputState->guessing)
                    cppmode.push(cppmodeitem(size()));

                break;

            case ELSE :

            case ELIF :
                // found the "#else" for "#if 0" that started this mode
                if (cpp_zeromode && cpp_ifcount == 1)
                    cpp_zeromode = false;

                // not in skipped "#if," so skip "#else" until "#endif" of "#if" is reached
                else if (!cpp_zeromode) {
                    cpp_skipelse = true;
                    cpp_ifcount = 1;
                }

                if (isoption(parser_options, SRCML_PARSER_OPTION_CPP_TEXT_ELSE) && !inputState->guessing) {
                    // create an empty cppmode for "#if" if one does not exist
                    if (cppmode.empty())
                        cppmode.push(cppmodeitem(size()));

                    // add new context for "#else" in current "#if"
                    cppmode.top().statesize.push_back(size());

                    if (!cpp_zeromode && cppmode.top().statesize.front() > size())
                        cppmode.top().skipelse = true;
                }

                break;

            case ENDIF :
                // reached the end of an "#if"
                --cpp_ifcount;

                // found the "#else" for "#if 0" that started this mode
                if (cpp_zeromode && cpp_ifcount == 0)
                    cpp_zeromode = false;

                // found the "#else" for "#else 0" that started this mode
                if (cpp_skipelse && cpp_ifcount == 0)
                    cpp_skipelse = false;

                if (
                    isoption(parser_options, SRCML_PARSER_OPTION_CPP_TEXT_ELSE)
                    && !inputState->guessing
                    && !cppmode.empty()
                ) {
                    // add new context for "#endif" in current "#if"
                    cppmode.top().statesize.push_back(size());

                    // reached "#endif," so we are finished adding to this mode
                    cppmode.top().isclosed = true;

                    // remove any finished modes
                    cppmode_cleanup();
                }

            default :
                break;
        }

        /*
          Skip elements when:
            - in a zero block (cpp_zeromode) and not marking "#if 0"
            - when processing only the "#if" part, not "#else"
            - when guessing and in else (unless in zero block)
            - when ??? for cppmode
        */
        if (
            (
                !isoption(parser_options, SRCML_PARSER_OPTION_CPP_MARKUP_IF0)
                && cpp_zeromode
            )
            || (
                isoption(parser_options, SRCML_PARSER_OPTION_CPP_TEXT_ELSE)
                && cpp_skipelse
            )
            || (
                isoption(parser_options, SRCML_PARSER_OPTION_CPP_TEXT_ELSE)
                && inputState->guessing
                && cpp_skipelse
            )
            || (
                !cppmode.empty()
                && !cppmode.top().isclosed
                && cppmode.top().skipelse
            )
        ) {
            while (LA(1) != PREPROC && LA(1) != 1 /* EOF */)
                consume();
        }

        ENTRY_DEBUG
} :;

/*
  cppmode_cleanup

  Used to remove any finished or unneeded cppmodes.
*/
cppmode_cleanup[] {
        bool equal = true;

        for (auto& state : cppmode.top().statesize) {
            if (state != cppmode.top().statesize[0]) {
                equal = false;
                break;
            }
        }

        if (
            !cppmode.empty()
            && (
                equal
                || cppmode.top().statesize.size() == 2
            )
        )
            cppmode.pop();

        ENTRY_DEBUG
} :;

/*
  line_continuation

  Handles a line continuation character.
*/
line_continuation[] { ENTRY_DEBUG } :
        {
            // end all preprocessor modes
            endWhileMode(MODE_PARSE_EOL);
        }

        EOL_BACKSLASH
;

/*
  cpp_condition

  Handles a condition in C++.
*/
cpp_condition[bool& markblockzero] { CompleteElement element(this); ENTRY_DEBUG } :
        set_bool[markblockzero, LA(1) == CONSTANTS && LT(1)->getText() == "0"sv]

        cpp_complete_expression
;

/*
  cpp_expression

  Handles an expression in C++.
*/
cpp_expression[CALL_TYPE type = NOCALL] { ENTRY_DEBUG } :
        { !inputState->guessing }?
        (expression_process expression_part_plus_linq[type]) |

        cpp_garbage
;
exception
catch[...] {
        startNewMode(MODE_LOCAL);
        startElement(SNAME);
        consume();
        endMode();
}

/*
  cpp_complete_expression

  Used to match a complete expression (no stream) in C++.
*/
cpp_complete_expression[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            // start a mode to end at the right bracket with expressions inside
            startNewMode(MODE_TOP | MODE_EXPECT | MODE_EXPRESSION);
        }

        (options { greedy = true; } :
            { !cpp_check_end() }?
            (
                // commas in a list
                { inTransparentMode(MODE_END_ONLY_AT_RPAREN) || !inTransparentMode(MODE_END_AT_COMMA) }?
                comma |

                // right parenthesis, unless we are in a pair of parentheses in an expression
                { !inTransparentMode(MODE_INTERNAL_END_PAREN) }?
                rparen[false] |

                // argument mode (as part of call)
                { inMode(MODE_ARGUMENT) }?
                argument |

                // expression with right parentheses if a previous match is in one
                { LA(1) != RPAREN || inTransparentMode(MODE_INTERNAL_END_PAREN) }?
                cpp_expression |

                COLON
            )
        )*
;

/*
  cpp_symbol

  Handles a symbol in C++.
*/
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

/*
  cpp_define_name
*/
cpp_define_name[] {
        CompleteElement element(this);
        int line_pos = LT(1)->getLine();
        auto pos = LT(1)->getColumn() + LT(1)->getText().size();
} :
        {
            startNewMode(MODE_LOCAL);

            startElement(SCPP_MACRO_DEFN);
        }

        cpp_symbol

        (options { greedy = true; } :
            { line_pos == LT(1)->getLine() && pos == (unsigned)LT(1)->getColumn() }?
            cpp_define_parameter_list
        )*
;

/*
  cpp_define_parameter_list
*/
cpp_define_parameter_list[] {
        CompleteElement element(this);
        bool lastwasparam = false;
        bool foundparam = false;

        ENTRY_DEBUG
} :
        {
            // list of parameters
            startNewMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT);

            // start the parameter list element
            startElement(SPARAMETER_LIST);
        }

        // parameter list must include all possible parts since it is part of function detection
        LPAREN

        (
            {
                foundparam = true;

                if (!lastwasparam)
                    empty_element(SPARAMETER, !lastwasparam);

                lastwasparam = false;
            }

            {
                // we are in a parameter list; need to make sure we end it down to the start of the parameter list
                if (!inMode(MODE_PARAMETER | MODE_LIST | MODE_EXPECT))
                    endMode();
            }

            comma |

            cpp_define_parameter

            {
                foundparam = lastwasparam = true;
            }
        )*

        empty_element[SPARAMETER, !lastwasparam && foundparam]
        rparen[false]
;

/*
  cpp_define_parameter
*/
cpp_define_parameter[] { int type_count = 1; ENTRY_DEBUG } :
        {
            // end parameter correctly
            startNewMode(MODE_PARAMETER);

            // start the parameter statement
            startElement(SPARAMETER);
        }

        parameter_type_count[type_count]
;

/*
  cpp_define_value
*/
cpp_define_value[] { ENTRY_DEBUG } :
        {
            startElement(SCPP_MACRO_VALUE);
        }

        cpp_garbage
        (options { greedy = true; } : cpp_garbage)*
;

/*
  cpp_symbol_optional

  Handles an optional symbol in C++.
*/
cpp_symbol_optional[] { ENTRY_DEBUG } :
        (options { greedy = true; } : cpp_symbol)*
;

/*
  cpp_filename

  Handles a filename in C++.
*/
cpp_filename[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCPP_FILENAME);
        }

        (
            string_literal[false] |

            char_literal[false] |

            TEMPOPS
            (~(TEMPOPE | EOL))*
            TEMPOPE
        )
;

/*
  cpp_linenumber

  Handles a linenumber in C++.
*/
cpp_linenumber[] { SingleElement element(this); bool first = true; ENTRY_DEBUG } :
        (options { greedy = true; } :
            {
                if (first) {
                    startElement(SCPP_NUMBER);
                    first = false;
                }
            }

            literal[false]
        )*
;

/*
  cpp_literal

  Handles a literal in C++.
*/
cpp_literal[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SCPP_LITERAL);
        }

        (
            string_literal[false] |

            char_literal[false] |

            TEMPOPS
            (~(TEMPOPE | EOL))*
            TEMPOPE
        )
;

/*
  omp_directive
*/
omp_directive[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SOMP_DIRECTIVE);
        }

        OMP_OMP

        (options { generateAmbigWarnings = false; } :
            COMMA |

            { next_token() == LPAREN }?
            omp_clause |

            omp_name
        )*
;

/*
  omp_name
*/
omp_name[] { SingleElement element(this); ENTRY_DEBUG } :
        {
            startElement(SOMP_NAME);
        }

        cpp_garbage
;

/*
  omp_clause
*/
omp_clause[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SOMP_CLAUSE);
        }

        omp_name
        omp_argument_list
;

/*
  omp_argument_list
*/
omp_argument_list[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SOMP_ARGUMENT_LIST);
        }

        (
            { next_token() != RPAREN }?
            LPAREN
            omp_argument
            (COMMA omp_argument)*
            RPAREN |

            LPAREN
            RPAREN
        )
;

/*
  omp_argument
*/
omp_argument[] { CompleteElement element(this); ENTRY_DEBUG } :
        {
            startNewMode(MODE_LOCAL);

            startElement(SOMP_ARGUMENT);

            startElement(SOMP_EXPRESSION);
        }

        (~(RPAREN | COMMA))*
;
