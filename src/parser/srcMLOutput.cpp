/**
 * @file srcMLOutput.cpp
 *
 * @copyright Copyright (C) 2003-2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
  Output of the XML format based on srcMLParser
*/

#include "srcMLOutput.hpp"
#include "srcMLToken.hpp"
#include "srcmlns.hpp"
#include <srcml.h>
#include <boost/preprocessor/iteration/local.hpp>

#include "srcMLOutputPR.hpp"

#include <cstring>
#include <sstream>
#ifdef _MSC_BUILD
#include <io.h>
#define snprintf _snprintf
#endif

#define SRCML_OPTION_NO_REVISION ((unsigned long long)1 << 63)

/** 
 * anonymous enum for prefix positions
 */
enum { SRCML_SRC_NS_URI_POS,
       SRCML_CPP_NS_URI_POS,
       SRCML_ERR_NS_URI_POS,
       SRCML_EXT_LITERAL_NS_URI_POS,
       SRCML_EXT_OPERATOR_NS_URI_POS,
       SRCML_EXT_MODIFIER_NS_URI_POS,
       SRCML_EXT_POSITION_NS_URI_POS,
       SRCML_EXT_OPENMP_NS_URI_POS

};

/** name of element call map */
#define ELEMENT_MAP_CALL_NAME element_name

/** element map first type */
#define ELEMENT_MAP_FIRST_TYPE int

/** element map second type */
#define ELEMENT_MAP_SECOND_TYPE const char*

/** element map default operation */
#define ELEMENT_MAP_DEFAULT(s) template <ELEMENT_MAP_FIRST_TYPE n> inline ELEMENT_MAP_SECOND_TYPE \
    ELEMENT_MAP_CALL_NAME() { s }

/** element map call */
#define ELEMENT_MAP_CALL(t) ELEMENT_MAP_CALL_NAME <srcMLParserTokenTypes::t>()

/** element map */
#define ELEMENT_MAP(t, s) template <> inline ELEMENT_MAP_SECOND_TYPE ELEMENT_MAP_CALL(t) { return s; }

// map the token types to specific strings
/** namespace surrounding element mappings */
namespace {

    // base member
    ELEMENT_MAP_DEFAULT(return "";)

    // map tokens to strings
    // order matters if referring to previous definition
    ELEMENT_MAP(SUNIT, "unit")
    ELEMENT_MAP(COMMENT_START, "comment")
    ELEMENT_MAP(COMMENT_END, ELEMENT_MAP_CALL(COMMENT_START))
    ELEMENT_MAP(LINECOMMENT_START, ELEMENT_MAP_CALL(COMMENT_START))
    ELEMENT_MAP(LINECOMMENT_END, ELEMENT_MAP_CALL(COMMENT_START))
    ELEMENT_MAP(JAVADOC_COMMENT_START, ELEMENT_MAP_CALL(COMMENT_START))
    ELEMENT_MAP(DOXYGEN_COMMENT_START, ELEMENT_MAP_CALL(COMMENT_START))
    ELEMENT_MAP(LINE_DOXYGEN_COMMENT_START, ELEMENT_MAP_CALL(COMMENT_START))

    // No op
    ELEMENT_MAP(SNOP, "")

    // literal values
    ELEMENT_MAP(SSTRING,  "literal")
    ELEMENT_MAP(SCHAR,    "literal")
    ELEMENT_MAP(SLITERAL, "literal")
    ELEMENT_MAP(SBOOLEAN, "literal")
    ELEMENT_MAP(SNULL,    "literal")
    ELEMENT_MAP(SCOMPLEX, "literal")

    // operators
    ELEMENT_MAP(SOPERATOR, "operator")

    // type modifier
    ELEMENT_MAP(SMODIFIER, "modifier")

    // sub-statement elements
    ELEMENT_MAP(SNAME, "name")
    ELEMENT_MAP(SONAME, "")
    ELEMENT_MAP(SCNAME, "name")
    ELEMENT_MAP(STYPE, "type")
    ELEMENT_MAP(STYPEPREV, "type")
    ELEMENT_MAP(SCONDITION, "condition")
    ELEMENT_MAP(SBLOCK, "block")
    ELEMENT_MAP(SPSEUDO_BLOCK, "block")
    ELEMENT_MAP(SINDEX, "index")
    ELEMENT_MAP(SDECLTYPE, "decltype")
    ELEMENT_MAP(STYPENAME, "typename")
    ELEMENT_MAP(SATOMIC, "atomic")
    ELEMENT_MAP(SSTATIC_ASSERT_STATEMENT, "assert")
    ELEMENT_MAP(SGENERIC_SELECTION, "generic_selection")
    ELEMENT_MAP(SGENERIC_SELECTOR, "selector")
    ELEMENT_MAP(SGENERIC_ASSOCIATION_LIST, "association_list")
    ELEMENT_MAP(SGENERIC_ASSOCIATION, "association")

    ELEMENT_MAP(SEXPRESSION_STATEMENT, "expr_stmt")
    ELEMENT_MAP(SEXPRESSION, "expr")

    ELEMENT_MAP(SDECLARATION_STATEMENT, "decl_stmt")
    ELEMENT_MAP(SDECLARATION, "decl")
    ELEMENT_MAP(SDECLARATION_INITIALIZATION, "init")
    ELEMENT_MAP(SDECLARATION_RANGE, "range")

    ELEMENT_MAP(SBREAK_STATEMENT, "break")
    ELEMENT_MAP(SCONTINUE_STATEMENT, "continue")
    ELEMENT_MAP(SGOTO_STATEMENT, "goto")
    ELEMENT_MAP(SLABEL_STATEMENT, "label")

    ELEMENT_MAP(STYPEDEF,          "typedef")
    ELEMENT_MAP(SASM,              "asm")
    ELEMENT_MAP(SMACRO_CALL,       "macro")
    ELEMENT_MAP(SENUM,             "enum")
    ELEMENT_MAP(SENUM_DECLARATION, "enum_decl")

    ELEMENT_MAP(SIF_STATEMENT, "if")
    ELEMENT_MAP(STERNARY, "ternary")
    ELEMENT_MAP(STHEN, "then")
    ELEMENT_MAP(SELSE, "else")
    ELEMENT_MAP(SELSEIF, "elseif")

    ELEMENT_MAP(SWHILE_STATEMENT, "while")
    ELEMENT_MAP(STYPEOF, "typeof")
    ELEMENT_MAP(SDO_STATEMENT, "do")

    ELEMENT_MAP(SSWITCH, "switch")
    ELEMENT_MAP(SCASE, "case")
    ELEMENT_MAP(SDEFAULT, "default")

    ELEMENT_MAP(SFOR_STATEMENT, "for")
    ELEMENT_MAP(SFOREACH_STATEMENT, "foreach")
    ELEMENT_MAP(SFOR_CONTROL, "control")
    ELEMENT_MAP(SFOR_INITIALIZATION, "init")
    ELEMENT_MAP(SFOR_CONDITION, ELEMENT_MAP_CALL(SCONDITION))
    ELEMENT_MAP(SFOR_INCREMENT, "incr")
    ELEMENT_MAP(SFOR_LIKE_CONTROL, "init")

    // functions
    ELEMENT_MAP(SFUNCTION_DEFINITION,    "function")
    ELEMENT_MAP(SFUNCTION_DECLARATION,   "function_decl")
    ELEMENT_MAP(SFUNCTION_LAMBDA,        "lambda")
    ELEMENT_MAP(SFUNCTION_SPECIFIER,     "specifier")
    ELEMENT_MAP(SRETURN_STATEMENT,       "return")
    ELEMENT_MAP(SFUNCTION_CALL,          "call")
    ELEMENT_MAP(SSIZEOF_CALL,            "sizeof")
    ELEMENT_MAP(SPARAMETER_LIST,         "parameter_list")
    ELEMENT_MAP(SPARAMETER,              "parameter")
    ELEMENT_MAP(SKRPARAMETER_LIST,       "krparameter_list")
    ELEMENT_MAP(SKRPARAMETER,            "krparameter")
    ELEMENT_MAP(SARGUMENT_LIST,          "argument_list")
    ELEMENT_MAP(SARGUMENT,               "argument")
    ELEMENT_MAP(SLAMBDA_CAPTURE,         "capture")
    ELEMENT_MAP(SPSEUDO_PARAMETER_LIST,  ELEMENT_MAP_CALL(SPARAMETER_LIST))
    ELEMENT_MAP(SINDEXER_PARAMETER_LIST, ELEMENT_MAP_CALL(SPARAMETER_LIST))

    // struct, union
    ELEMENT_MAP(SSTRUCT, "struct")
    ELEMENT_MAP(SSTRUCT_DECLARATION,   "struct_decl")
    ELEMENT_MAP(SUNION, "union")
    ELEMENT_MAP(SUNION_DECLARATION,    "union_decl")

    // class
    ELEMENT_MAP(SCLASS,                    "class")
    ELEMENT_MAP(SCLASS_DECLARATION,        "class_decl")
    ELEMENT_MAP(SPUBLIC_ACCESS,            "public")
    ELEMENT_MAP(SPUBLIC_ACCESS_DEFAULT,    "public")
    ELEMENT_MAP(SPRIVATE_ACCESS,           "private")
    ELEMENT_MAP(SPRIVATE_ACCESS_DEFAULT,   "private")
    ELEMENT_MAP(SPROTECTED_ACCESS,         "protected")
    ELEMENT_MAP(SPROTECTED_ACCESS_DEFAULT, "protected")

    // Qt
    ELEMENT_MAP(SSIGNAL_ACCESS,           "signals")
    ELEMENT_MAP(SFOREVER_STATEMENT,       "forever")
    ELEMENT_MAP(SEMIT_STATEMENT,          "emit")

    ELEMENT_MAP(SMEMBER_INITIALIZATION_LIST, "member_init_list")
    ELEMENT_MAP(SMEMBER_INITIALIZATION,      "call")
    ELEMENT_MAP(SCONSTRUCTOR_DEFINITION,     "constructor")
    ELEMENT_MAP(SCONSTRUCTOR_DECLARATION,    "constructor_decl")
    ELEMENT_MAP(SDESTRUCTOR_DEFINITION,      "destructor")
    ELEMENT_MAP(SDESTRUCTOR_DECLARATION,     "destructor_decl")
    ELEMENT_MAP(SDERIVATION_LIST,            "super")
    ELEMENT_MAP(SFRIEND,                     "friend")
    ELEMENT_MAP(SCLASS_SPECIFIER,            "specifier")

    // extern definition
    ELEMENT_MAP(SEXTERN, "extern")

    // namespaces
    ELEMENT_MAP(SNAMESPACE, "namespace")
    ELEMENT_MAP(SUSING_DIRECTIVE, "using")

    // exception handling
    ELEMENT_MAP(STRY_BLOCK,       "try")
    ELEMENT_MAP(SCATCH_BLOCK,     "catch")
    ELEMENT_MAP(SFINALLY_BLOCK,   "finally")
    ELEMENT_MAP(STHROW_STATEMENT, "throw")
    ELEMENT_MAP(STHROW_SPECIFIER, "throw")
    ELEMENT_MAP(STHROW_SPECIFIER_JAVA, "throws")
    ELEMENT_MAP(SNOEXCEPT, "noexcept")

    // template
    ELEMENT_MAP(STEMPLATE, "template")
    ELEMENT_MAP(SGENERIC_ARGUMENT,        ELEMENT_MAP_CALL(SARGUMENT))
    ELEMENT_MAP(SGENERIC_ARGUMENT_LIST,   ELEMENT_MAP_CALL(SARGUMENT_LIST))
    ELEMENT_MAP(STEMPLATE_PARAMETER,      ELEMENT_MAP_CALL(SPARAMETER))
    ELEMENT_MAP(STEMPLATE_PARAMETER_LIST, ELEMENT_MAP_CALL(SPARAMETER_LIST))
    ELEMENT_MAP(SGENERIC_PARAMETER,       ELEMENT_MAP_CALL(SPARAMETER))
    ELEMENT_MAP(SGENERIC_PARAMETER_LIST,  ELEMENT_MAP_CALL(SPARAMETER_LIST))

    // cpp
    ELEMENT_MAP(SCPP_DIRECTIVE,   "directive")
    ELEMENT_MAP(SCPP_FILENAME,    "file")
    ELEMENT_MAP(SCPP_NUMBER,      "number")
    ELEMENT_MAP(SCPP_LITERAL,     "literal")
    ELEMENT_MAP(SCPP_INCLUDE,     "include")
    ELEMENT_MAP(SCPP_DEFINE,      "define")
    ELEMENT_MAP(SCPP_UNDEF,       "undef")
    ELEMENT_MAP(SCPP_LINE,        "line")
    ELEMENT_MAP(SCPP_IF,          "if")
    ELEMENT_MAP(SCPP_IFDEF,       "ifdef")
    ELEMENT_MAP(SCPP_IFNDEF,      "ifndef")
    ELEMENT_MAP(SCPP_ELSE,        "else")
    ELEMENT_MAP(SCPP_ELIF,        "elif")
    ELEMENT_MAP(SCPP_ENDIF,       "endif")
    ELEMENT_MAP(SCPP_THEN,        "then")
    ELEMENT_MAP(SCPP_PRAGMA,      "pragma")
    ELEMENT_MAP(SCPP_ERROR,       "error")
    ELEMENT_MAP(SCPP_WARNING,     "warning")
    ELEMENT_MAP(SCPP_MACRO_DEFN,  "macro")
    ELEMENT_MAP(SCPP_MACRO_VALUE, "value")
    ELEMENT_MAP(SCPP_EMPTY,       "empty")

    // C# cpp
    ELEMENT_MAP(SCPP_REGION,    "region")
    ELEMENT_MAP(SCPP_ENDREGION, "endregion")

    // Objective-C cpp
    ELEMENT_MAP(SCPP_IMPORT,    "import")

    // debug
    ELEMENT_MAP(SMARKER,        "marker")
    ELEMENT_MAP(SERROR_PARSE,   "parse")
    ELEMENT_MAP(SERROR_MODE,    "mode")

    // C#
    ELEMENT_MAP(SLOCK_STATEMENT,      "lock")
    ELEMENT_MAP(SFIXED_STATEMENT,     "fixed")
    ELEMENT_MAP(SCHECKED_STATEMENT,   "checked")
    ELEMENT_MAP(SUNCHECKED_STATEMENT, "unchecked")
    ELEMENT_MAP(SUNSAFE_STATEMENT,    "unsafe")
    ELEMENT_MAP(SUSING_STATEMENT,     "using_stmt")
    ELEMENT_MAP(SFUNCTION_DELEGATE,   "delegate")
    ELEMENT_MAP(SEVENT,               "event")
    ELEMENT_MAP(SCONSTRAINT,          "constraint")

    // Java elements
    ELEMENT_MAP(SEXTENDS,                "extends")
    ELEMENT_MAP(SIMPLEMENTS,             "implements")
    ELEMENT_MAP(SIMPORT,                 "import")
    ELEMENT_MAP(SPACKAGE,                "package")
    ELEMENT_MAP(SASSERT_STATEMENT,       "assert")
    ELEMENT_MAP(SSYNCHRONIZED_STATEMENT, "synchronized")
    ELEMENT_MAP(SINTERFACE,              "interface")
    ELEMENT_MAP(SINTERFACE_DECLARATION,  "interface_decl")
    ELEMENT_MAP(SANNOTATION_DEFN,        "annotation_defn")
    ELEMENT_MAP(SSTATIC_BLOCK,           "static")

    // special characters
    ELEMENT_MAP(SATTRIBUTE,   "attribute")
    ELEMENT_MAP(STARGET,      "target")

    // linq
    ELEMENT_MAP(SLINQ,   "linq")
    ELEMENT_MAP(SFROM, "from")
    ELEMENT_MAP(SSELECT,   "select")
    ELEMENT_MAP(SWHERE,   "where")
    ELEMENT_MAP(SLET,   "let")
    ELEMENT_MAP(SORDERBY,   "orderby")
    ELEMENT_MAP(SGROUP,   "group")
    ELEMENT_MAP(SJOIN,   "join")
    ELEMENT_MAP(SIN,   "in")
    ELEMENT_MAP(SON,   "on")
    ELEMENT_MAP(SEQUALS,   "equals")
    ELEMENT_MAP(SBY,   "by")
    ELEMENT_MAP(SINTO,   "into")

    // special characters
    ELEMENT_MAP(CONTROL_CHAR,   "escape")
    ELEMENT_MAP(SANNOTATION,    "annotation")

    // C++
    ELEMENT_MAP(SALIGNAS,                "alignas")
    ELEMENT_MAP(SALIGNOF,                "alignof")
    ELEMENT_MAP(STYPEID,                 "typeid")
    ELEMENT_MAP(SSIZEOF_PACK,            "sizeof")
    ELEMENT_MAP(SENUM_CLASS,             "enum")
    ELEMENT_MAP(SENUM_CLASS_DECLARATION, "enum_decl")
    ELEMENT_MAP(SOPERATOR_FUNCTION,      "function")
    ELEMENT_MAP(SOPERATOR_FUNCTION_DECL, "function_decl")
    ELEMENT_MAP(SREF_QUALIFIER,          "ref_qualifier")

    // Objective-C
    ELEMENT_MAP(SRECEIVER,             "receiver")
    ELEMENT_MAP(SMESSAGE,              "message")
    ELEMENT_MAP(SSELECTOR,             "selector")
    ELEMENT_MAP(SPROTOCOL_LIST,        "protocol_list")
    ELEMENT_MAP(SCATEGORY,             "category")
    ELEMENT_MAP(SPROTOCOL,             "protocol")
    ELEMENT_MAP(SREQUIRED_DEFAULT,     "required")
    ELEMENT_MAP(SREQUIRED,             "required")
    ELEMENT_MAP(SOPTIONAL,             "optional")
    ELEMENT_MAP(SPROPERTY,             "property")
    ELEMENT_MAP(SATTRIBUTE_LIST,       "attribute_list")
    ELEMENT_MAP(SSYNTHESIZE,           "synthesize")
    ELEMENT_MAP(SDYNAMIC,              "dynamic")
    ELEMENT_MAP(SENCODE,               "encode")
    ELEMENT_MAP(SAUTORELEASEPOOL,      "autoreleasepool")
    ELEMENT_MAP(SCOMPATIBILITY_ALIAS,  "compatibility_alias")
    ELEMENT_MAP(SNIL,                  "literal")
    ELEMENT_MAP(SCLASS_INTERFACE,      "class")
    ELEMENT_MAP(SCLASS_IMPLEMENTATION, "class")
    ELEMENT_MAP(SPROTOCOL_DECLARATION, "protocol_decl")

    // casts
    ELEMENT_MAP(SCAST,             "cast")
    ELEMENT_MAP(SCONST_CAST,       "cast")
    ELEMENT_MAP(SDYNAMIC_CAST,     "cast")
    ELEMENT_MAP(SREINTERPRET_CAST, "cast")
    ELEMENT_MAP(SSTATIC_CAST,      "cast")

    // srcMLOutput only
    ELEMENT_MAP(SPOSITION, "position")

    // Other
    ELEMENT_MAP(SCUDA_ARGUMENT_LIST,  ELEMENT_MAP_CALL(SARGUMENT_LIST))

    // OpenMP
    ELEMENT_MAP(SOMP_DIRECTIVE,    "directive")
    ELEMENT_MAP(SOMP_NAME,         "name")
    ELEMENT_MAP(SOMP_CLAUSE,       "clause")
    ELEMENT_MAP(SOMP_ARGUMENT_LIST, ELEMENT_MAP_CALL(SARGUMENT_LIST))
    ELEMENT_MAP(SOMP_ARGUMENT,      ELEMENT_MAP_CALL(SARGUMENT))
    ELEMENT_MAP(SOMP_EXPRESSION,    ELEMENT_MAP_CALL(SEXPRESSION))

    //
    ELEMENT_MAP(SEMPTY,         "empty_stmt")


}

#undef ELEMENT_MAP_CALL_NAME
#undef ELEMENT_MAP_FIRST_TYPE
#undef ELEMENT_MAP_SECOND_TYPE

/** element call name */
#define ELEMENT_MAP_CALL_NAME element_prefix

/** element map first type */
#define ELEMENT_MAP_FIRST_TYPE int

/** element map second type */
#define ELEMENT_MAP_SECOND_TYPE int

/** element map default operation */
#define ELEMENT_MAP_DEFAULT(s) template <ELEMENT_MAP_FIRST_TYPE n> inline ELEMENT_MAP_SECOND_TYPE \
    ELEMENT_MAP_CALL_NAME() { s }

/** element map call */
#define ELEMENT_MAP_CALL(t) ELEMENT_MAP_CALL_NAME <srcMLParserTokenTypes::t>()

/** element map */
#define ELEMENT_MAP(t, s) template <> inline ELEMENT_MAP_SECOND_TYPE ELEMENT_MAP_CALL(t) { return s; }

/** namespace containing prefix mappings */
namespace {

    // default is the srcML namespace
    ELEMENT_MAP_DEFAULT(return SRCML_SRC_NS_URI_POS;)

    // cpp namespace
/** set Boost local macro */
#define BOOST_PP_LOCAL_MACRO(n) template<> inline int element_prefix<n + TOKEN_SCPP_DIRECTIVE>() { return SRCML_CPP_NS_URI_POS; }

    //  #define BOOST_PP_LOCAL_MACRO(n) ELEMENT_MAP(n, SRCML_CPP_NS_URI_POS),
/** set Boost macro limits */
#define BOOST_PP_LOCAL_LIMITS (0, TOKEN_SCPP_ENDIF - TOKEN_SCPP_DIRECTIVE)

#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS

    // literal namespace
    ELEMENT_MAP(SSTRING,   SRCML_EXT_LITERAL_NS_URI_POS)
    ELEMENT_MAP(SCHAR,     SRCML_EXT_LITERAL_NS_URI_POS)
    ELEMENT_MAP(SLITERAL,  SRCML_EXT_LITERAL_NS_URI_POS)
    ELEMENT_MAP(SBOOLEAN,  SRCML_EXT_LITERAL_NS_URI_POS)
    ELEMENT_MAP(SNULL,     SRCML_EXT_LITERAL_NS_URI_POS)
    ELEMENT_MAP(SCOMPLEX,  SRCML_EXT_LITERAL_NS_URI_POS)
    ELEMENT_MAP(SNIL,      SRCML_EXT_LITERAL_NS_URI_POS)

    // operator namespace
    ELEMENT_MAP(SOPERATOR, SRCML_EXT_OPERATOR_NS_URI_POS)

    // modifier namespace
    ELEMENT_MAP(SMODIFIER, SRCML_EXT_MODIFIER_NS_URI_POS)

    // position namespace
    ELEMENT_MAP(SPOSITION, SRCML_EXT_POSITION_NS_URI_POS)

    // OpenMP namespace
    ELEMENT_MAP(SOMP_DIRECTIVE,     SRCML_EXT_OPENMP_NS_URI_POS)
    ELEMENT_MAP(SOMP_NAME,          SRCML_EXT_OPENMP_NS_URI_POS)
    ELEMENT_MAP(SOMP_CLAUSE,        SRCML_EXT_OPENMP_NS_URI_POS)
    ELEMENT_MAP(SOMP_ARGUMENT_LIST, SRCML_EXT_OPENMP_NS_URI_POS)
    ELEMENT_MAP(SOMP_ARGUMENT,      SRCML_EXT_OPENMP_NS_URI_POS)
    ELEMENT_MAP(SOMP_EXPRESSION,    SRCML_EXT_OPENMP_NS_URI_POS)

}

/**
 * srcMLOutput
 * @param ints a token stream
 * @param filename if output to a file
 * @param language the unit language
 * @param xml_enc output encoding
 * @param op output operations
 * @param ts tabstop size
 * @param output_buffer if output is to a output buffer
 *
 * Constructor. Handles all outputs.
 */
srcMLOutput::srcMLOutput(TokenStream* ints,
                         xmlOutputBuffer * output_buffer,
                         const char* language,
                         const char* xml_enc,
                         OPTION_TYPE& op,
                         const std::vector<std::string> & attributes,
                         boost::optional<std::pair<std::string, std::string> > processing_instruction,
                         size_t ts)
    : last_line(0), last_line2(0), last_column(0), end_position_output(false), input(ints), xout(0), output_buffer(output_buffer), unit_language(language), unit_url(0), unit_filename(0),
      unit_version(0), options(op), xml_encoding(xml_enc), unit_attributes(attributes), processing_instruction(processing_instruction),
      openelementcount(0), curline(0), curcolumn(0), tabsize(ts), depth(0), 
      debug_time_start(boost::posix_time::microsec_clock::universal_time())
{

    if(!isoption(options, SRCML_OPTION_OPTIONAL_MARKUP)) {

        ElementPrefix[SSTRING]   = SRCML_SRC_NS_URI_POS;
        ElementPrefix[SCHAR]     = SRCML_SRC_NS_URI_POS;
        ElementPrefix[SLITERAL]  = SRCML_SRC_NS_URI_POS;
        ElementPrefix[SBOOLEAN]  = SRCML_SRC_NS_URI_POS;
        ElementPrefix[SNULL]     = SRCML_SRC_NS_URI_POS;
        ElementPrefix[SCOMPLEX]  = SRCML_SRC_NS_URI_POS;
        ElementPrefix[SNIL]      = SRCML_SRC_NS_URI_POS;
        ElementPrefix[SOPERATOR] = SRCML_SRC_NS_URI_POS;
        ElementPrefix[SMODIFIER] = SRCML_SRC_NS_URI_POS;

    }

    if (isoption(options, SRCML_OPTION_POSITION) && isoption(options, SRCML_OPTION_LINE))
      num2process[2] = &srcMLOutput::processTextPositionLine;
    else if (isoption(options, SRCML_OPTION_POSITION))
      num2process[2] = &srcMLOutput::processTextPosition;

}

/**
 * initWriter
 *
 * Initializes output xmlWriter.  Supports delayed initialization.
 */
int srcMLOutput::initWriter() {

    // open the output text writer stream
    xout = xmlNewTextWriter(output_buffer);
    if (!xout) {

        fprintf(stderr, "src2srcml: " "Unable to open output buffer\n");
        return SRCML_STATUS_ERROR;
        
    }

    return SRCML_STATUS_OK;

}

/**
 * initNamespaces
 *
 * Initialize the output namespaces.
 */
void srcMLOutput::initNamespaces(const std::vector<std::string> & prefix, const std::vector<std::string> & uri) {

    num2prefix.push_back(SRCML_SRC_NS_PREFIX_DEFAULT);
    num2prefix.push_back(SRCML_CPP_NS_PREFIX_DEFAULT);
    num2prefix.push_back(SRCML_ERR_NS_PREFIX_DEFAULT);
    num2prefix.push_back(SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT);
    num2prefix.push_back(SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT);
    num2prefix.push_back(SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT);
    num2prefix.push_back(SRCML_EXT_POSITION_NS_PREFIX_DEFAULT);
    num2prefix.push_back(SRCML_EXT_OPENMP_NS_PREFIX_DEFAULT);

    num2uri.push_back(SRCML_SRC_NS_URI);
    num2uri.push_back(SRCML_CPP_NS_URI);
    num2uri.push_back(SRCML_ERR_NS_URI);
    num2uri.push_back(SRCML_EXT_LITERAL_NS_URI);
    num2uri.push_back(SRCML_EXT_OPERATOR_NS_URI);
    num2uri.push_back(SRCML_EXT_MODIFIER_NS_URI);
    num2uri.push_back(SRCML_EXT_POSITION_NS_URI);
    num2uri.push_back(SRCML_EXT_OPENMP_NS_URI);

    for(std::vector<std::string>::size_type outer_pos = 0; outer_pos < uri.size(); ++ outer_pos) {

        std::vector<std::string>::size_type pos;
        for(pos = 0; pos < num2uri.size() && num2uri[pos] != uri[outer_pos]; ++pos)
            ;

        if(pos < num2uri.size()) {

            num2prefix[pos] = prefix[outer_pos];

        } else {

            num2prefix.push_back(prefix[outer_pos]);
            num2uri.push_back(uri[outer_pos]);

        }

    }

    // setup attributes names for line/column position if used
    if (isoption(options, SRCML_OPTION_POSITION)) {

        lineAttribute = num2prefix[SRCML_EXT_POSITION_NS_URI_POS];
        lineAttribute += ":line";

        line2Attribute = num2prefix[SRCML_EXT_POSITION_NS_URI_POS];
        line2Attribute += ":line2";

        columnAttribute = num2prefix[SRCML_EXT_POSITION_NS_URI_POS];
        columnAttribute += ":column";
    }

}

/**
 * ~srcMLOutput
 *
 * Destructor.  Closes output.
 */
srcMLOutput::~srcMLOutput() {

    close();
}

/**
 * close
 *
 * Close/finish the output.
 */
void srcMLOutput::close() {

    if (xout) {

        xmlTextWriterEndDocument(xout);
        xmlFreeTextWriter(xout);
        xout = 0;
        output_buffer = 0;

    } else if(output_buffer) {

        xmlOutputBufferClose(output_buffer);
        output_buffer = 0;

    }
}

/**
 * srcMLTextWriterStartElement
 * @param xout xml writer
 * @param s name of element to start
 *
 * Start output of element s in writer xout.
 */
void srcMLOutput::srcMLTextWriterStartElement(xmlTextWriter* xout, const xmlChar* s) {

    xmlTextWriterStartElement(xout, s);
    ++openelementcount;
}

/**
 * srcMLTextWriterEndElement
 * @param xout xml writer
 *
 * End element in writer xout.
 */
void srcMLOutput::srcMLTextWriterEndElement(xmlTextWriter* xout) {

    xmlTextWriterEndElement(xout);
    --openelementcount;
}

/**
 * lineAttributeValue
 * @param token the token to output
 *
 * Convert the current tokens line to string attribute.
 *
 * @returns the line as a string.
 */
const char * srcMLOutput::lineAttributeValue(const antlr::RefToken& token) {

    snprintf(out, 20, "%d", token->getLine());

    return out;
}

/**
 * columnAttributeValue
 * @param token the token to output
 *
 * Convert the current tokens column to string attribute.
 *
 * @returns the column as a string.
  */
const char * srcMLOutput::columnAttributeValue(const antlr::RefToken& token) {

    snprintf(out, 20, "%d", token->getColumn());

    return out;
}

/**
 * lineAttributeValue
 * @param aline to convert to string
 *
 * Convert the line to string attribute.
 *
 * @returns the line as a string.
 */
const char * srcMLOutput::lineAttributeValue(int aline) {

    snprintf(out, 20, "%d", aline);

    return out;
}

/**
 * columnAttributeValue
 * @param acolumn to convert to string
 *
 * Convert the column to string attribute.
 *
 * @returns the column as a string.
 */
const char * srcMLOutput::columnAttributeValue(int acolumn) {

    snprintf(out, 20, "%d", acolumn);

    return out;
}

/**
 * lineAttributeValue
 * @param aline to convert to string
 *
 * Convert the line to string attribute.
 *
 * @returns the line as a string.
 */
void srcMLOutput::outputPosition() {

    if(end_position_output) return;

    const char * position_localname = "position";
    const char* prefix = num2prefix[(int)ElementPrefix[SPOSITION]].c_str();

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST position_localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST position_localname, 0);


    xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST lineAttributeValue(last_line));

    if(isoption(options,SRCML_OPTION_LINE))
        xmlTextWriterWriteAttribute(xout, BAD_CAST line2Attribute.c_str(), BAD_CAST lineAttributeValue(last_line2));

    xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST columnAttributeValue(last_column));

    xmlTextWriterEndElement(xout);

    end_position_output = true;

}

/**
 * setTokenStream
 * @param ints token stream to set to.
 *
 * Set the token stream.
 */
void srcMLOutput::setTokenStream(TokenStream& ints) {

    input = &ints;
}

/**
 * consume
 * @param language unit language attribute
 * @param revision unit revision attribute
 * @param url unit url attribute
 * @param filename unit filename attribute
 * @param version unit version attribute
 * @param timestamp unit timestamp attribute
 * @param hash unit hash attribute
 *
 * Start consumption of tokens/parsing of source code with unit attributes.
 */
void srcMLOutput::consume(const char* language, const char* revision, const char* url, const char* filename,
                          const char* version, const char* timestamp, const char* hash, const char* encoding) {

    // store attributes so that first occurrence of unit element will be correct
    unit_revision = revision;
    unit_url = url;
    unit_filename = filename;
    unit_version = version;
    unit_timestamp = timestamp;
    unit_language = language;
    unit_hash = hash;
    unit_encoding = encoding;

    if (!isoption(options, SRCML_OPTION_INTERACTIVE)) {

        // consume all input until EOF
        while (consume_next() != antlr::Token::EOF_TYPE) {}

    } else {

        // consume all input until EOF
        while (consume_next() != antlr::Token::EOF_TYPE) {

            // in interactive mode flush after each token is discovered
            xmlTextWriterFlush(xout);
        }
    }
}

/**
 * consume_next
 *
 * Consume/get and output the next token.
 *
 * @returns the time of consumed token
 */
int srcMLOutput::consume_next() {

    const antlr::RefToken& token = input->nextToken();

    outputToken(token);

    return token->getType();
}

/**
 * processEscape
 * @param token escape the token in srcML escape tag
 *
 * Outputs the token in a srcML escape tag.
 */
void srcMLOutput::processEscape(const antlr::RefToken& token) {

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

    ++openelementcount;

    int n = token->getText()[0];

    char out[20 + 2 + 1];
    snprintf(out, 22, "0x%x", n);

    xmlTextWriterWriteAttribute(xout, BAD_CAST "char", BAD_CAST out);

    xmlTextWriterEndElement(xout);
    --openelementcount;
}

/**
 * processStaticAssert
 * @param token the assert token
 *
 * Outputs a assert tag with static attribute.
 */
 void srcMLOutput::processStaticAssert(const antlr::RefToken& token) {

    processOptional(token, "type", "static");

 }

/**
 * processClassInterface
 * @param token the class token
 *
 * Outputs a class tag with @interface type attribute.
 */
 void srcMLOutput::processClassInterface(const antlr::RefToken& token) {

    processOptional(token, "type", "@interface");

 }

/**
 * processClassImplementation
 * @param token the class token
 *
 * Outputs a class tag with @interface type attribute.
 */
 void srcMLOutput::processClassImplementation(const antlr::RefToken& token) {

    processOptional(token, "type", "@implementation");

 }


/**
 * outputXMLDecl
 *
 * Output the starting xml declaration.
 */
void srcMLOutput::outputXMLDecl() {

    // issue the xml declaration, but only if we want to
    if(depth == 0 && isoption(options, SRCML_OPTION_XML_DECL)) xmlTextWriterStartDocument(xout, XML_VERSION, xml_encoding, XML_DECLARATION_STANDALONE);

}

/**
 * outputPreRootProcessingInstruction
 *
 * Output a pre-root preprocessing instruction.
 */
void srcMLOutput::outputPreRootProcessingInstruction() {

    if(depth == 0 && processing_instruction) {

        xmlTextWriterStartPI(xout, BAD_CAST processing_instruction->first.c_str());
        xmlTextWriterWriteString(xout, BAD_CAST processing_instruction->second.c_str());
        xmlTextWriterEndPI(xout);
        xmlTextWriterWriteString(xout, BAD_CAST "\n");

    }

}

/**
 * outputNamesapces
 * @param xout the xml writer to write namespaces
 * @param options the current set options
 * @param depth the depth in the archive number of output units
 * @param outer is this an outer unit or inner unit
 *
 * Output the namespaces on the units.
 */
void srcMLOutput::outputNamespaces(xmlTextWriterPtr xout, const OPTION_TYPE& options, int depth) {

    // figure out which namespaces are needed
    char const * const ns[] = {

        // main srcML namespace declaration always used
        (depth == 0) ? SRCML_SRC_NS_URI : 0,

        // main cpp namespace declaration
        isoption(options, SRCML_OPTION_CPP) && (isoption(options, SRCML_OPTION_ARCHIVE) == !(depth == 0)) ? SRCML_CPP_NS_URI : 0,

        // optional debugging xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_DEBUG)    ? SRCML_ERR_NS_URI : 0,

        // optional literal xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_OPTIONAL_MARKUP) && isoption(options, SRCML_OPTION_LITERAL)  ? SRCML_EXT_LITERAL_NS_URI : 0,

        // optional operator xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_OPTIONAL_MARKUP) && isoption(options, SRCML_OPTION_OPERATOR) ? SRCML_EXT_OPERATOR_NS_URI : 0,

        // optional modifier xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_OPTIONAL_MARKUP) && isoption(options, SRCML_OPTION_MODIFIER) ? SRCML_EXT_MODIFIER_NS_URI : 0,

        // optional position xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_POSITION) ? SRCML_EXT_POSITION_NS_URI : 0,

        // optional position xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_OPENMP) ? SRCML_EXT_OPENMP_NS_URI : 0,

    };

    // output the namespaces
    for (unsigned int i = 0; i < sizeof(ns) / sizeof(ns[0]); ++i) {
        if (!ns[i])
            continue;

        std::string prefix = "xmlns";
        if (num2prefix[i][0] != '\0') {
            prefix += ':';
            prefix += num2prefix[i];
        }

        xmlTextWriterWriteAttribute(xout, BAD_CAST prefix.c_str(), BAD_CAST ns[i]);
    }

    if(depth == 0) {

        for(std::vector<std::string>::size_type pos =  SRCML_EXT_OPENMP_NS_URI_POS + 1; pos < num2prefix.size(); ++pos) {

            std::string prefix = "xmlns";
            if (num2prefix[pos][0] != '\0') {
                prefix += ':';
                prefix += num2prefix[pos];
         }
        
            xmlTextWriterWriteAttribute(xout, BAD_CAST prefix.c_str(), BAD_CAST num2uri[pos].c_str());
            
        }

    }

}

/**
 * startUnit
 * @param language the language attribute
 * @param revision what version of srcML
 * @param url the url attribute
 * @param filename the filename attribute
 * @param version the version attribute
 * @param timestamp the timestamp attribute
 * @param hash the hash attribute
 * @param encoding the encoding attribute
 * @param outer is this an outer or inner unit
 *
 * Output the start of a unit tag.
 */
void srcMLOutput::startUnit(const char* language, const char* revision,
                            const char* url, const char* filename,
                            const char* version, const char* timestamp,
                            const char* hash,
                            const char* encoding,
                            const std::vector<std::string> & attributes,
                            bool output_macrolist) {

    const char * prefix = num2prefix[0].c_str();
    std::string maintag = prefix ? prefix : "";
    if (!maintag.empty())
        maintag += ":";
    maintag += "unit";

    // start of main tag
    srcMLTextWriterStartElement(xout, BAD_CAST /* type2name(SUNIT) */ maintag.c_str());

    // outer units have namespaces
    if (/* outer && */ isoption(options, SRCML_OPTION_NAMESPACE_DECL)) {
        outputNamespaces(xout, options, depth);
    }

    // setting up for tabs, even if not used
    std::ostringstream stabs;
    std::string tabattribute;
    if (isoption(options, SRCML_OPTION_POSITION)) {
        stabs << tabsize;
        tabattribute = num2prefix[SRCML_EXT_POSITION_NS_URI_POS];
        tabattribute.append(":tabs");
    }

    std::string soptions;
    std::string SEP;
    //if(isoption(options, SRCML_OPTION_XML_DECL))        { soptions = "XMLDECL"; }
    //if(isoption(options, SRCML_OPTION_NAMESPACE_DECL))  { if(soptions != "") SEP = ","; soptions += SEP + "NAMESPACEDECL"; }
    if(isoption(options, SRCML_OPTION_CPP_TEXT_ELSE))  { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "CPP_TEXT_ELSE"; }
    if(isoption(options, SRCML_OPTION_CPP_MARKUP_IF0)) { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "CPP_MARKUP_IF0"; }
    if(isoption(options, SRCML_OPTION_EXPRESSION))     { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "EXPRESSION"; }
    if(isoption(options, SRCML_OPTION_LINE))           { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "LINE"; }
    if(isoption(options, SRCML_OPTION_NESTIF))         { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "NESTIF"; }
    if(isoption(options, SRCML_OPTION_CPPIF_CHECK))    { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "CPPIF_CHECK"; }
    if(isoption(options, SRCML_OPTION_WRAP_TEMPLATE))  { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "WRAP_TEMPLATE"; }
    if(!isoption(options, SRCML_OPTION_TERNARY))       { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "OPERATOR_TERNARY"; }

    std::string stab = stabs.str();

    // list of attributes
    const char* const attrs[][2] = {

        { UNIT_ATTRIBUTE_REVISION, !isoption(options, SRCML_OPTION_NO_REVISION) ? revision : 0 },

        // language attribute
        { UNIT_ATTRIBUTE_LANGUAGE, language },

        // url attribute
        { UNIT_ATTRIBUTE_URL, url },

        // filename attribute
        { UNIT_ATTRIBUTE_FILENAME, filename },

        // version attribute
        { UNIT_ATTRIBUTE_VERSION, version },

        // position tab setting
        { tabattribute.c_str(), isoption(options, SRCML_OPTION_POSITION) ? stab.c_str() : 0 },

        // timestamp attribute
        { UNIT_ATTRIBUTE_TIMESTAMP, timestamp },

        // hash attribute
        { UNIT_ATTRIBUTE_HASH, hash },

        // source encoding attribute
        { UNIT_ATTRIBUTE_SOURCE_ENCODING, isoption(options, SRCML_OPTION_STORE_ENCODING) ? encoding : 0 },

        { UNIT_ATTRIBUTE_OPTIONS,  depth == 0 && (isoption(options, SRCML_OPTION_NESTIF)
         || isoption(options, SRCML_OPTION_CPPIF_CHECK) || isoption(options, SRCML_OPTION_WRAP_TEMPLATE) || !isoption(options, SRCML_OPTION_TERNARY)) ? soptions.c_str() : 0 },

    };

    // output attributes
    for (unsigned int i = 0; i < sizeof(attrs) / sizeof(attrs[0]); ++i) {
        if (!attrs[i][1])
            continue;

        xmlTextWriterWriteAttribute(xout, BAD_CAST attrs[i][0], BAD_CAST attrs[i][1]);
    }

    for(std::vector<std::string>::size_type pos = 0; pos < attributes.size(); pos += 2) {

        xmlTextWriterWriteAttribute(xout, BAD_CAST attributes[pos].c_str(), BAD_CAST attributes[pos + 1].c_str());

    }

    if(output_macrolist) outputMacroList();

    ++depth;
}

/**
 * setMacroList
 * @param list the user defined macro list
 *
 * Set the macro list to use for output.
 */
void srcMLOutput::setMacroList(std::vector<std::string> & list) {
    user_macro_list = list;
}

/**
 * outputMacroList
 *
 * Output the stored user defined macro list meta data.
 */
void srcMLOutput::outputMacroList() {

    for(std::vector<std::string>::size_type i = 0; i < user_macro_list.size(); i += 2) {

        xmlTextWriterStartElement(xout, BAD_CAST "macro-list");
        xmlTextWriterWriteAttribute(xout, BAD_CAST "token", BAD_CAST user_macro_list[i].c_str());
        xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST user_macro_list[i + 1].c_str());
        xmlTextWriterEndElement(xout);

    }

}

/**
 * processUnit
 * @param token token to output
 *
 * Callback to process/output unit token.
 */
void srcMLOutput::processUnit(const antlr::RefToken& token) {

    if (isstart(token)) {

        // keep track of number of open elements
        openelementcount = 0;
        startUnit(unit_language, unit_revision, unit_url, unit_filename, unit_version, unit_timestamp, unit_hash, unit_encoding, unit_attributes, !isoption(options, SRCML_OPTION_ARCHIVE));

    } else {

        // end anything still open, including the unit
        while (openelementcount > 0)
            srcMLTextWriterEndElement(xout);

        // leave a blank line before next nested unit even the last one
        if (isoption(options, SRCML_OPTION_ARCHIVE))
            processText("\n\n", 2);
    }
}

/**
 * processText
 * @param str text to output
 *
 * Callback to process/output text.
 */
void srcMLOutput::processText(const std::string& str) {

    xmlTextWriterWriteRawLen(xout, BAD_CAST (unsigned char*) str.data(), (int)str.size());
}

/**
 * processText
 * @param s string to output
 * @param size of bytes to output
 *
 * Callback to process/output text outputting size bytes.
 */
void srcMLOutput::processText(const char* s, int size) {

    xmlTextWriterWriteRawLen(xout, BAD_CAST (unsigned char*) s, size);
}

/**
 * processText
 * @param token token to output as text
 *
 * Callback to process/output token as text.
 */
void srcMLOutput::processText(const antlr::RefToken& token) {

    processText(token->getText());
}

/**
 * processTextPosition
 * @param token token to output as text
 *
 * Callback to process/output token as text with position information.
 */
void srcMLOutput::processTextPosition(const antlr::RefToken& token) {

    xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST lineAttributeValue(token));

    xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST columnAttributeValue(token));

    last_line = token->getLine();
    last_column = token->getColumn() + (int)token->getText().size();

    end_position_output = false;

    processText(token->getText());
}

/**
 * processTextPositionLine
 * @param token token to output as text
 *
 * Callback to process/output token as text with position information using @code#line@endcode embedded in half of set line.
 */
void srcMLOutput::processTextPositionLine(const antlr::RefToken& token) {

    xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST lineAttributeValue(token->getLine() & 0xFFFF));
    xmlTextWriterWriteAttribute(xout, BAD_CAST line2Attribute.c_str(), BAD_CAST lineAttributeValue(token->getLine() >> 16));

    xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST columnAttributeValue(token));

    last_line = token->getLine() & 0xFFFF;
    last_line2 = token->getLine() >> 16;
    last_column = token->getColumn() + (int)token->getText().size();

    end_position_output = false;

    processText(token->getText());
}

/**
 * processAccess
 * @param token token to output
 *
 * Callback to process/output token for access region.
 */
void srcMLOutput::processAccess(const antlr::RefToken& token) {
    static const char* type_default = "default";

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (!isstart(token)) {
        processToken(token);
        return;
    }

    // start the element
    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

    ++openelementcount;

    xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST type_default);

    // end the element right away if empty
    if (isempty(token)) {
        xmlTextWriterEndElement(xout);
        --openelementcount;
    }
}


/**
 * processPseudoBlock
 * @param token token to output
 *
 * Callback to process/output token for pseudo block.
 */
void srcMLOutput::processPseudoBlock(const antlr::RefToken& token) {

    static const char* type_pseudo = "pseudo";

    processOptional(token, "type", type_pseudo);

}

/**
 * processToken
 * @param token token to output
 *
 * Callback to process/output generic token.
 */
void srcMLOutput::processToken(const antlr::RefToken& token) {

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (localname[0] == 0)
        return;

    if (isstart(token) || isempty(token)) {
        if (prefix[0] == 0)
            xmlTextWriterStartElement(xout, BAD_CAST localname);
        else
            xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);
        ++openelementcount;

        if(isoption(options, SRCML_OPTION_DEBUG_TIMER)) {

            std::string time = to_simple_string(boost::posix_time::microsec_clock::universal_time() - debug_time_start);
            xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_TIMESTAMP, BAD_CAST time.c_str());

        }

    }

    if (!isstart(token) || isempty(token)) {

        if(isoption(options, SRCML_OPTION_POSITION) && !isempty(token))
            outputPosition();

        xmlTextWriterEndElement(xout);
        --openelementcount;

    }
}

/**
 * processTypePrevious
 * @param token type token with previous reference.
 *
 * Callback to process/output generic token.
 */
void srcMLOutput::processTypePrevious(const antlr::RefToken& token) {

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

    xmlTextWriterWriteAttribute(xout, BAD_CAST "ref", BAD_CAST "prev");

    if(isoption(options, SRCML_OPTION_DEBUG_TIMER)) {

        std::string time = to_simple_string(boost::posix_time::microsec_clock::universal_time() - debug_time_start);
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_TIMESTAMP, BAD_CAST time.c_str());

    }
    xmlTextWriterEndElement(xout);

}

/**
 * processJavadocCommentStart
 * @param token token to output as Javadoc comment
 *
 * Callback to process/output token as Javadoc comment.
 */
void srcMLOutput::processJavadocCommentStart(const antlr::RefToken& token) {
    static const char* BLOCK_COMMENT_ATTR = "block";
    static const char* JAVADOC_COMMENT_ATTR = "javadoc";

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

    ++openelementcount;

    xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST BLOCK_COMMENT_ATTR);
    xmlTextWriterWriteAttribute(xout, BAD_CAST "format", BAD_CAST JAVADOC_COMMENT_ATTR);

    ((*this).*num2process[2])(token);
}

/**
 * processDoxygenCommentStart
 * @param token token to output as Doxygen comment
 *
 * Callback to process/output token as Doxygen comment (block).
 */
void srcMLOutput::processDoxygenCommentStart(const antlr::RefToken& token) {
    static const char* BLOCK_COMMENT_ATTR = "block";
    static const char* DOXYGEN_COMMENT_ATTR = "doxygen";

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

    ++openelementcount;

    xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST BLOCK_COMMENT_ATTR);
    xmlTextWriterWriteAttribute(xout, BAD_CAST "format", BAD_CAST DOXYGEN_COMMENT_ATTR);

    ((*this).*num2process[2])(token);
}

/**
 * processDoxygenLineCommentStart
 * @param token token to output as Doxygen comment
 *
 * Callback to process/output token as Doxygen comment (line).
 */
void srcMLOutput::processLineDoxygenCommentStart(const antlr::RefToken& token) {
    static const char* LINE_COMMENT_ATTR = "line";
    static const char* DOXYGEN_COMMENT_ATTR = "doxygen";

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

    ++openelementcount;

    xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST LINE_COMMENT_ATTR);
    xmlTextWriterWriteAttribute(xout, BAD_CAST "format", BAD_CAST DOXYGEN_COMMENT_ATTR);

    ((*this).*num2process[2])(token);
}

/**
 * processCommentStart
 * @param token token to output as comment
 *
 * Callback to process/output token as block comment.
 */
void srcMLOutput::processBlockCommentStart(const antlr::RefToken& token) {
    static const char* BLOCK_COMMENT_ATTR = "block";

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

    ++openelementcount;

    xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST BLOCK_COMMENT_ATTR);

    ((*this).*num2process[2])(token);
}

/**
 * processLineCommentStart
 * @param token token to output as comment
 *
 * Callback to process/output token as line comment.
 */
void srcMLOutput::processLineCommentStart(const antlr::RefToken& token) {
    static const char* const LINE_COMMENT_ATTR = "line";

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

    ++openelementcount;

    xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST LINE_COMMENT_ATTR);

    ((*this).*num2process[2])(token);
}

/**
 * processEndLineToken
 * @param token token to output as end of line
 *
 * Callback to process/output token as end of line (think ends tag that end at end of line).
 */
void srcMLOutput::processEndLineToken(const antlr::RefToken& token) {

    std::string::size_type size = token->getText().size();

    bool output = false;
    if (size > 1 || token->getText()[0] != '\n') {
        processText(token);
        output = true;
    }


    xmlTextWriterEndElement(xout);
    --openelementcount;

    if (size == 1 && !output)
        processText(token);
}

/**
 * processEndBlockToken
 * @param token token to output as end of block
 *
 * Callback to process/output token as end of block (also seems to end the block tag).
 */
void srcMLOutput::processEndBlockToken(const antlr::RefToken& token) {

    processText(token);

    xmlTextWriterEndElement(xout);
    --openelementcount;
}

/**
 * processOptional
 * @param token token to output optional markup
 * @param attr_name name of attribute
 * @param attr_value value of attribute
 *
 * Callback to process/output token as for optional markup
 */
void srcMLOutput::processOptional(const antlr::RefToken& token, const char* attr_name, const char* attr_value) {

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (isstart(token)) {
        if (prefix[0] == 0)
            xmlTextWriterStartElement(xout, BAD_CAST localname);
        else
            xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

        ++openelementcount;
        if(attr_name)
            xmlTextWriterWriteAttribute(xout, BAD_CAST attr_name, BAD_CAST attr_value);
    } else {

        if(isoption(options, SRCML_OPTION_POSITION) && !isempty(token))
            outputPosition();

        xmlTextWriterEndElement(xout);
        --openelementcount;
    }
}

/**
 * processString
 * @param token token to output as string literal
 *
 * Callback to process/output token as string literal.
 */
void srcMLOutput::processString(const antlr::RefToken& token) {

    processOptional(token, "type", "string");
}

/**
 * processChar
 * @param token token to output as char` literal
 *
 * Callback to process/output token as char literal.
 */
 void srcMLOutput::processChar(const antlr::RefToken& token) {

    processOptional(token, "type", "char");
}

/**
 * processLiteral
 * @param token token to output as number literal
 *
 * Callback to process/output token as number literal.
 */
void srcMLOutput::processLiteral(const antlr::RefToken& token) {

    processOptional(token, "type", "number");
}

/**
 * processBoolean
 * @param token token to output as boolean literal
 *
 * Callback to process/output token as boolean literal.
 */
void srcMLOutput::processBoolean(const antlr::RefToken& token) {

    processOptional(token, "type", "boolean");
}

/**
 * processNull
 * @param token token to output as null literal
 *
 * Callback to process/output token as null literal.
 */
void srcMLOutput::processNull(const antlr::RefToken& token) {

    processOptional(token, "type", "null");
}

/**
 * processNull
 * @param token token to output as nil literal
 *
 * Callback to process/output token as nil literal.
 */
void srcMLOutput::processNil(const antlr::RefToken& token) {

    processOptional(token, "type", "nil");
}

/**
 * processComplex
 * @param token token to output as complex number literal
 *
 * Callback to process/output token as complex number literal.
 */
void srcMLOutput::processComplex(const antlr::RefToken& token) {

    processOptional(token, "type", "complex");
}

/**
 * processGenericArgumentList
 * @param token token to output as generic argument list
 *
 * Callback to process/output token as generic argument list.
 */
void srcMLOutput::processGenericArgumentList(const antlr::RefToken& token) {

    processOptional(token, "type", "generic");

}

/**
 * processGenericParameterList
 * @param token token to output as generic parameter list
 *
 * Callback to process/output token as generic parameter list.
 */
void srcMLOutput::processGenericParameterList(const antlr::RefToken& token) {

    processOptional(token, "type", "generic");

}

/**
 * processCast
 * @param token token to output as cast
 *
 * Callback to process/output token as cast.
 */
void srcMLOutput::processCast(const antlr::RefToken& token) {

    if(token->getType() == SCAST)
        processOptional(token, 0, 0);
    else if(token->getType() == SCONST_CAST)
        processOptional(token, "type", "const");
    else if(token->getType() == SDYNAMIC_CAST)
        processOptional(token, "type", "dynamic");
    else if(token->getType() == SREINTERPRET_CAST)
        processOptional(token, "type", "reinterpret");
    else if(token->getType() == SSTATIC_CAST)
        processOptional(token, "type", "static");
    else
        processOptional(token, 0, 0);

}

/**
 * processEnumClass
 * @param token token to output as enum class
 *
 * Callback to process/output token as enum class.
 */
void srcMLOutput::processEnumClass(const antlr::RefToken& token) {

    processOptional(token, "type", "class");

}

/**
 * processOperatorFunction
 * @param token token to output as operator function
 *
 * Callback to process/output token as operator function.
 */
void srcMLOutput::processOperatorFunction(const antlr::RefToken& token) {

    processOptional(token, "type", "operator");

}

/**
 * processPseudoParameterList
 * @param token token to output as pseudo parameter list
 *
 * Callback to process/output token as pseudo parameter list.
 */
void srcMLOutput::processPseudoParameterList(const antlr::RefToken& token) {

    processOptional(token, "type", "pseudo");

}

/**
 * processIndexerParameterList
 * @param token token to output as indexer parameter list
 *
 * Callback to process/output token as indexer parameter list.
 */
void srcMLOutput::processIndexerParameterList(const antlr::RefToken& token) {

    processOptional(token, "type", "indexer");

}

/**
 * processSizeofPack
 * @param token token to output as parameter pack sizeof
 *
 * Callback to process/output token as parameter pack sizeof
 */
void srcMLOutput::processSizeofPack(const antlr::RefToken& token) {

    processOptional(token, "type", "pack");

}

/**
 * processCudaArgumentList
 * @param token token to output as cuda argument list
 *
 * Callback to process/output token as cuda argument list.
 */
void srcMLOutput::processCudaArgumentList(const antlr::RefToken& token) {

    processOptional(token, "type", "cuda");

}

#if DEBUG

/**
 * processMarker
 * @param token token to output as marker
 *
 * Callback to process/output token as debug marker.
 */
void srcMLOutput::processMarker(const antlr::RefToken& token) {

    const char* localname = ElementNames[token->getType()];
    const char* prefix = num2prefix[(int)ElementPrefix[token->getType()]].c_str();

    if (localname[0] == 0)
        return;

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST localname, 0);

    ++openelementcount;

    xmlTextWriterWriteAttribute(xout, BAD_CAST "location", BAD_CAST token->getText().c_str());

    xmlTextWriterEndElement(xout);
    --openelementcount;
}

#endif

/**
 * outputToken
 * @param token token to output
 *
 * Get callback that output token and output.
 */
inline void srcMLOutput::outputToken(const antlr::RefToken& token) {

    // use the array of pointers to methods to call the correct output routine
    ((*this).*(num2process[(int)process_table[token->getType()]]))(token);
}

/** element names array */
const char* const srcMLOutput::ElementNames[] = {

    // fill the array in order of token numbers
/** set boost macro */
#define BOOST_PP_LOCAL_MACRO(n)   element_name<n>(),

/** set boost macro limits */
#define BOOST_PP_LOCAL_LIMITS     (0, TOKEN_END_ELEMENT_TOKEN - 1)

#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS

    // fill the array in order of token numbers
/** set boost macro */
#define BOOST_PP_LOCAL_MACRO(n)   element_name<256 + 1 + n>(),

/** set boost macro limits */
#define BOOST_PP_LOCAL_LIMITS     (0, TOKEN_END_ELEMENT_TOKEN - 1 - 256)

#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
};

/** element prefix number */
int srcMLOutput::ElementPrefix[] = {

    // fill the array with the prefixes
/** set boost macro */
#define BOOST_PP_LOCAL_MACRO(n)   element_prefix<n>(),

/** set boost macro limits */
#define BOOST_PP_LOCAL_LIMITS     (0, TOKEN_END_ELEMENT_TOKEN - 1)

#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS

    // fill the array in order of token numbers
/** set boost macro */
#define BOOST_PP_LOCAL_MACRO(n)   element_prefix<256 + 1 + n>(),

/** set boost macro limits */
#define BOOST_PP_LOCAL_LIMITS     (0, TOKEN_END_ELEMENT_TOKEN - 1 - 256)

#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS
};

/**
 * setOutputBuffer
 * @param output_buffer an output buffer
 *
 * Set to output to output_buffer.  Should be called before initWriter.
 * @todo see if still needed.
 */
void srcMLOutput::setOutputBuffer(xmlOutputBufferPtr output_buffer) {

    this->output_buffer = output_buffer;

}

/**
 * getWriter
 *
 * Get the current writer. @todo see if still needed.
 */
xmlTextWriter * srcMLOutput::getWriter() {

    return xout;
}

/**
 * setDepth
 * @param thedepth depth to set to
 *
 * Set the current depth to thedepth @todo see if still needed.
 */
void srcMLOutput::setDepth(int thedepth) {

    depth = thedepth;
}
