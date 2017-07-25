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
#include <cstring>
#include <sstream>
#ifdef _MSC_BUILD
#include <io.h>
#define snprintf _snprintf
#endif

#define SRCML_OPTION_NO_REVISION ((unsigned long long)1 << 63)

#define args srcMLOutput* pout, const antlr::RefToken& token, const char* name, const char* prefix

#pragma GCC diagnostic ignored "-Wunused-parameter"

const std::unordered_map<int, Element> srcMLOutput::process = {

    { SUNIT,  { "unit", SRC, 0, 0, [](args) { pout->processUnit(token); }}},

    { START_ELEMENT_TOKEN,  { "", SRC, 0, 0, [](args) { pout->processText(token); }}},

    { COMMENT_START, { "comment", SRC, 0, 0, [](args) {
        pout->processToken(token, name, prefix, "type", "block");
        pout->processTextPosition(token);
    }}},

   { COMMENT_END,   { "comment", SRC, 0, 0, [](args) { 

        pout->processText(token);

        // TODO: Not sure why this is needed. Part of COMMENT_END processing
        xmlTextWriterEndElement(pout->xout);
        --(pout->openelementcount);
    }}},

    { LINECOMMENT_START, { "comment", SRC, 0, 0, [](args) {
        pout->processToken(token, name, prefix, "type", "line");
        pout->processTextPosition(token);
    }}},

    { LINECOMMENT_END, { "comment", SRC, 0, 0, [](args) {

        auto size = token->getText().size();

        bool output = false;
        if (size > 1 || token->getText()[0] != '\n') {
            pout->processText(token);
            output = true;
        }

        xmlTextWriterEndElement(pout->xout);
        --(pout->openelementcount);

        if (size == 1 && !output)
            pout->processText(token);
    }}},


    { LINE_DOXYGEN_COMMENT_START, { "comment", SRC, 0, 0, [](args) {
        pout->processToken(token, name, prefix, "type", "line", "format", "doxygen");
        pout->processTextPosition(token);
    }}},

    { JAVADOC_COMMENT_START, { "comment", SRC, 0, 0, [](args) {
        pout->processToken(token, name, prefix, "type", "block", "format", "javadoc");
        pout->processTextPosition(token);
    }}},

    { DOXYGEN_COMMENT_START, { "comment", SRC, 0, 0, [](args) {
        pout->processToken(token, name, prefix, "type", "block", "format", "doxygen");
        pout->processTextPosition(token);
    }}},

    { SSTRING,                      { "literal",            SRC, "type", "string",  nullptr }},
    { SCHAR,                        { "literal",            SRC, "type", "char",    nullptr }},
    { SLITERAL,                     { "literal",            SRC, "type", "number",  nullptr }},
    { SBOOLEAN,                     { "literal",            SRC, "type", "boolean", nullptr }},
    { SNULL,                        { "literal",            SRC, "type", "null",    nullptr }},
    { SCOMPLEX,                     { "literal",            SRC, "type", "complex", nullptr }},
    { SOPERATOR,                    { "operator",           SRC,      0,         0, nullptr }},
    { SMODIFIER,                    { "modifier",           SRC,      0,         0, nullptr }},
    { SNAME,                        { "name",               SRC,      0,         0, nullptr }},
    { SONAME,                       { "",                   SRC,      0,         0, nullptr }},
    { SCNAME,                       { "name",               SRC,      0,         0, nullptr }},
    { STYPE,                        { "type",               SRC,      0,         0, nullptr }},

    { STYPEPREV,                    { "type",               SRC, "ref", "prev",     nullptr }},
    { SCONDITION,                   { "condition",          SRC,      0,         0, nullptr }},
    { SBLOCK,                       { "block",              SRC,      0,         0, nullptr }},
    { SPSEUDO_BLOCK,                { "block",              SRC, "type", "pseudo",  nullptr }},
    { SINDEX,                       { "index",              SRC,      0,         0, nullptr }},
    { SDECLTYPE,                    { "decltype",           SRC,      0,         0, nullptr }},
    { STYPENAME,                    { "typename",           SRC,      0,         0, nullptr }},
    { SATOMIC,                      { "atomic",             SRC,      0,         0, nullptr }},
    { SSTATIC_ASSERT_STATEMENT,     { "assert",             SRC, "type", "static",  nullptr }},
    { SGENERIC_SELECTION,           { "generic_selection",  SRC,      0,         0, nullptr }},
    { SGENERIC_SELECTOR,            { "selector",           SRC,      0,         0, nullptr }},
    { SGENERIC_ASSOCIATION_LIST,    { "association_list",   SRC,      0,         0, nullptr }},
    { SGENERIC_ASSOCIATION,         { "association",        SRC,      0,         0, nullptr }},


    { SEXPRESSION_STATEMENT,        { "expr_stmt",          SRC,      0,         0, nullptr }},
    { SEXPRESSION,                  { "expr",               SRC,      0,         0, nullptr }},

    { SDECLARATION_STATEMENT,       { "decl_stmt",          SRC,      0,         0, nullptr }},
    { SDECLARATION,                 { "decl",               SRC,      0,         0, nullptr }},
    { SDECLARATION_INITIALIZATION,  { "init",               SRC,      0,         0, nullptr }},
    { SDECLARATION_RANGE,           { "range",              SRC,      0,         0, nullptr }},

    { SBREAK_STATEMENT,             { "break",              SRC,      0,         0, nullptr }},
    { SCONTINUE_STATEMENT,          { "continue",           SRC,      0,         0, nullptr }},
    { SGOTO_STATEMENT,              { "goto",               SRC,      0,         0, nullptr }},
    { SLABEL_STATEMENT,             { "label",              SRC,      0,         0, nullptr }},

    { STYPEDEF,                     { "typedef",            SRC,      0,         0, nullptr }},
    { SASM,                         { "asm",                SRC,      0,         0, nullptr }},
    { SMACRO_CALL,                  { "macro",              SRC,      0,         0, nullptr }},
    { SENUM,                        { "enum",               SRC,      0,         0, nullptr }},
    { SENUM_DECLARATION,            { "enum_decl",          SRC,      0,         0, nullptr }},

    { SIF_STATEMENT,                { "if",                 SRC,      0,         0, nullptr }},
    { STERNARY,                     { "ternary",            SRC,      0,         0, nullptr }},
    { STHEN,                        { "then",               SRC,      0,         0, nullptr }},
    { SELSE,                        { "else",               SRC,      0,         0, nullptr }},
    { SELSEIF,                      { "elseif",             SRC,      0,         0, nullptr }},

    { SWHILE_STATEMENT,             { "while",              SRC,      0,         0, nullptr }},
    { STYPEOF,                      { "typeof",             SRC,      0,         0, nullptr }},
    { SDO_STATEMENT,                { "do",                 SRC,      0,         0, nullptr }},

    { SSWITCH,                      { "switch",             SRC,      0,         0, nullptr }},
    { SCASE,                        { "case",               SRC,      0,         0, nullptr }},
    { SDEFAULT,                     { "default",            SRC,      0,         0, nullptr }},

    { SFOR_STATEMENT,               { "for",                SRC,      0,         0, nullptr }},
    { SFOREACH_STATEMENT,           { "foreach",            SRC,      0,         0, nullptr }},
    { SFOR_CONTROL,                 { "control",            SRC,      0,         0, nullptr }},
    { SFOR_INITIALIZATION,          { "init",               SRC,      0,         0, nullptr }},
    { SFOR_CONDITION,               { "condition",          SRC,      0,         0, nullptr }},
    { SFOR_INCREMENT,               { "incr",               SRC,      0,         0, nullptr }},
    { SFOR_LIKE_CONTROL,            { "init",               SRC,      0,         0, nullptr }},

    // functions
    { SFUNCTION_DEFINITION,         { "function",           SRC,      0,         0, nullptr }},
    { SFUNCTION_DECLARATION,        { "function_decl",      SRC,      0,         0, nullptr }},
    { SFUNCTION_LAMBDA,             { "lambda",             SRC,      0,         0, nullptr }},
    { SFUNCTION_SPECIFIER,          { "specifier",          SRC,      0,         0, nullptr }},
    { SRETURN_STATEMENT,            { "return",             SRC,      0,         0, nullptr }},
    { SFUNCTION_CALL,               { "call",               SRC,      0,         0, nullptr }},
    { SSIZEOF_CALL,                 { "sizeof",             SRC,      0,         0, nullptr }},
    { SPARAMETER_LIST,              { "parameter_list",     SRC,      0,         0, nullptr }},
    { SPARAMETER,                   { "parameter",          SRC,      0,         0, nullptr }},
    { SKRPARAMETER_LIST,            { "krparameter_list",   SRC,      0,         0, nullptr }},
    { SKRPARAMETER,                 { "krparameter",        SRC,      0,         0, nullptr }},
    { SARGUMENT_LIST,               { "argument_list",      SRC,      0,         0, nullptr }},
    { SARGUMENT,                    { "argument",           SRC,      0,         0, nullptr }},
    { SLAMBDA_CAPTURE,              { "capture",            SRC,      0,         0, nullptr }},
    { SPSEUDO_PARAMETER_LIST,       {  "parameter_list",    SRC, "type",  "pseudo", nullptr }},
    { SINDEXER_PARAMETER_LIST,      { "parameter_list",     SRC, "type", "indexer", nullptr }},

    // struct,  union
    { SSTRUCT,                      { "struct",             SRC,      0,         0, nullptr }},
    { SSTRUCT_DECLARATION,          { "struct_decl",        SRC,      0,         0, nullptr }},
    { SUNION,                       { "union",              SRC,      0,         0, nullptr }},
    { SUNION_DECLARATION,           { "union_decl",         SRC,      0,         0, nullptr }},

    // class
    { SCLASS,                       { "class",              SRC,      0,         0, nullptr }},
    { SCLASS_DECLARATION,           { "class_decl",         SRC,      0,         0, nullptr }},
    { SPUBLIC_ACCESS,               { "public",             SRC,      0,         0, nullptr }},
    { SPUBLIC_ACCESS_DEFAULT,       { "public",             SRC, "type", "default", nullptr }},
    { SPRIVATE_ACCESS,              { "private",            SRC,      0,         0, nullptr }},
    { SPRIVATE_ACCESS_DEFAULT,      { "private",            SRC, "type", "default", nullptr }},
    { SPROTECTED_ACCESS,            { "protected",          SRC,      0,         0, nullptr }},
    { SPROTECTED_ACCESS_DEFAULT,    { "protected",          SRC, "type", "default", nullptr }},

    // Qt
    { SSIGNAL_ACCESS,               { "signals",            SRC,      0,         0, nullptr }},
    { SFOREVER_STATEMENT,           { "forever",            SRC,      0,         0, nullptr }},
    { SEMIT_STATEMENT,              { "emit",               SRC,      0,         0, nullptr }},

    { SMEMBER_INITIALIZATION_LIST,  { "member_init_list",   SRC,      0,         0, nullptr }},
    { SMEMBER_INITIALIZATION,       { "call",               SRC,      0,         0, nullptr }},
    { SCONSTRUCTOR_DEFINITION,      { "constructor",        SRC,      0,         0, nullptr }},
    { SCONSTRUCTOR_DECLARATION,     { "constructor_decl",   SRC,      0,         0, nullptr }},
    { SDESTRUCTOR_DEFINITION,       { "destructor",         SRC,      0,         0, nullptr }},
    { SDESTRUCTOR_DECLARATION,      { "destructor_decl",    SRC,      0,         0, nullptr }},
    { SDERIVATION_LIST,             { "super",              SRC,      0,         0, nullptr }},
    { SFRIEND,                      { "friend",             SRC,      0,         0, nullptr }},
    { SCLASS_SPECIFIER,             { "specifier",          SRC,      0,         0, nullptr }},

    // extern definition
    { SEXTERN,                      { "extern",             SRC,      0,         0, nullptr }},

    // namespaces
    { SNAMESPACE,                   { "namespace",          SRC,      0,         0, nullptr }},
    { SUSING_DIRECTIVE,             { "using",              SRC,      0,         0, nullptr }},

    // exception handling
    { STRY_BLOCK,                   { "try",                SRC,      0,         0, nullptr }},
    { SCATCH_BLOCK,                 { "catch",              SRC,      0,         0, nullptr }},
    { SFINALLY_BLOCK,               { "finally",            SRC,      0,         0, nullptr }},
    { STHROW_STATEMENT,             { "throw",              SRC,      0,         0, nullptr }},
    { STHROW_SPECIFIER,             { "throw",              SRC,      0,         0, nullptr }},
    { STHROW_SPECIFIER_JAVA,        { "throws",             SRC,      0,         0, nullptr }},
    { SNOEXCEPT,                    { "noexcept",           SRC,      0,         0, nullptr }},

    // template
    { STEMPLATE,                    { "template",           SRC,      0,         0, nullptr }},
    { SGENERIC_ARGUMENT,            { "argument",           SRC,      0,         0, nullptr }},
    { SGENERIC_ARGUMENT_LIST,       { "argument_list",      SRC, "type", "generic", nullptr }},
    { STEMPLATE_PARAMETER,          { "parameter",          SRC,      0,         0, nullptr }},
    { STEMPLATE_PARAMETER_LIST,     { "parameter_list",     SRC,      0,         0, nullptr }},
    { SGENERIC_PARAMETER,           { "parameter",          SRC,      0,         0, nullptr }},
    { SGENERIC_PARAMETER_LIST,      { "parameter_list",     SRC, "type", "generic", nullptr }},

    // cpp
    { SCPP_DIRECTIVE,               { "directive",          CPP,      0,         0, nullptr }},
    { SCPP_FILENAME,                { "file",               CPP,      0,         0, nullptr }},
    { SCPP_NUMBER,                  { "number",             CPP,      0,         0, nullptr }},
    { SCPP_LITERAL,                 { "literal",            CPP,      0,         0, nullptr }},
    { SCPP_INCLUDE,                 { "include",            CPP,      0,         0, nullptr }},
    { SCPP_DEFINE,                  { "define",             CPP,      0,         0, nullptr }},
    { SCPP_UNDEF,                   { "undef",              CPP,      0,         0, nullptr }},
    { SCPP_LINE,                    { "line",               CPP,      0,         0, nullptr }},
    { SCPP_IF,                      { "if",                 CPP,      0,         0, nullptr }},
    { SCPP_IFDEF,                   { "ifdef",              CPP,      0,         0, nullptr }},
    { SCPP_IFNDEF,                  { "ifndef",             CPP,      0,         0, nullptr }},
    { SCPP_ELSE,                    { "else",               CPP,      0,         0, nullptr }},
    { SCPP_ELIF,                    { "elif",               CPP,      0,         0, nullptr }},
    { SCPP_ENDIF,                   { "endif",              CPP,      0,         0, nullptr }},
    { SCPP_THEN,                    { "then",               CPP,      0,         0, nullptr }},
    { SCPP_PRAGMA,                  { "pragma",             CPP,      0,         0, nullptr }},
    { SCPP_ERROR,                   { "error",              CPP,      0,         0, nullptr }},
    { SCPP_WARNING,                 { "warning",            CPP,      0,         0, nullptr }},
    { SCPP_MACRO_DEFN,              { "macro",              CPP,      0,         0, nullptr }},
    { SCPP_MACRO_VALUE,             { "value",              CPP,      0,         0, nullptr }},
    { SCPP_EMPTY,                   { "empty",              CPP,      0,         0, nullptr }},

    // C# cpp
    { SCPP_REGION,                  { "region",             CPP,      0,         0, nullptr }},
    { SCPP_ENDREGION,               { "endregion",          CPP,      0,         0, nullptr }},

    // Objective-C cpp
    { SCPP_IMPORT,                  { "import",             CPP,      0,         0, nullptr }},

    // debug
    { SMARKER,                      { "marker",             SRC,      0,         0, nullptr }},
    { SERROR_PARSE,                 { "parse",              SRC,      0,         0, nullptr }},
    { SERROR_MODE,                  { "mode",               SRC,      0,         0, nullptr }},

    // C#
    { SLOCK_STATEMENT,              { "lock",               SRC,      0,         0, nullptr }},
    { SFIXED_STATEMENT,             { "fixed",              SRC,      0,         0, nullptr }},
    { SCHECKED_STATEMENT,           { "checked",            SRC,      0,         0, nullptr }},
    { SUNCHECKED_STATEMENT,         { "unchecked",          SRC,      0,         0, nullptr }},
    { SUNSAFE_STATEMENT,            { "unsafe",             SRC,      0,         0, nullptr }},
    { SUSING_STATEMENT,             { "using_stmt",         SRC,      0,         0, nullptr }},
    { SFUNCTION_DELEGATE,           { "delegate",           SRC,      0,         0, nullptr }},
    { SEVENT,                       { "event",              SRC,      0,         0, nullptr }},
    { SCONSTRAINT,                  { "constraint",         SRC,      0,         0, nullptr }},

    // Java elements
    { SEXTENDS,                     { "extends",            SRC,      0,         0, nullptr }},
    { SIMPLEMENTS,                  { "implements",         SRC,      0,         0, nullptr }},
    { SIMPORT,                      { "import",             SRC,      0,         0, nullptr }},
    { SPACKAGE,                     { "package",            SRC,      0,         0, nullptr }},
    { SASSERT_STATEMENT,            { "assert",             SRC,      0,         0, nullptr }},
    { SSYNCHRONIZED_STATEMENT,      { "synchronized",       SRC,      0,         0, nullptr }},
    { SINTERFACE,                   { "interface",          SRC,      0,         0, nullptr }},
    { SINTERFACE_DECLARATION,       { "interface_decl",     SRC,      0,         0, nullptr }},
    { SANNOTATION_DEFN,             { "annotation_defn",    SRC,      0,         0, nullptr }},
    { SSTATIC_BLOCK,                { "static",             SRC,      0,         0, nullptr }},

    // special characters
    { SATTRIBUTE,                   { "attribute",          SRC,      0,         0, nullptr }},
    { STARGET,                      { "target",             SRC,      0,         0, nullptr }},

    // linq
    { SLINQ,                        { "linq",               SRC,      0,         0, nullptr }},
    { SFROM,                        { "from",               SRC,      0,         0, nullptr }},
    { SSELECT,                      { "select",             SRC,      0,         0, nullptr }},
    { SWHERE,                       { "where",              SRC,      0,         0, nullptr }},
    { SLET,                         { "let",                SRC,      0,         0, nullptr }},
    { SORDERBY,                     { "orderby",            SRC,      0,         0, nullptr }},
    { SGROUP,                       { "group",              SRC,      0,         0, nullptr }},
    { SJOIN,                        { "join",               SRC,      0,         0, nullptr }},
    { SIN,                          { "in",                 SRC,      0,         0, nullptr }},
    { SON,                          { "on",                 SRC,      0,         0, nullptr }},
    { SEQUALS,                      { "equals",             SRC,      0,         0, nullptr }},
    { SBY,                          { "by",                 SRC,      0,         0, nullptr }},
    { SINTO,                        { "into",               SRC,      0,         0, nullptr }},

    // special characters
    { CONTROL_CHAR,                 { "escape",             SRC,      0,         0, [](args) {

        int n = token->getText()[0];
        char out[20 + 2 + 1];
        snprintf(out, 22, "0x%02x", n);

        pout->processToken(token, name, prefix, "char", out);

        xmlTextWriterEndElement(pout->xout);
        --(pout->openelementcount);
    }}},

    { SANNOTATION,                  { "annotation",         SRC,      0,         0, nullptr }},

    // C++
    { SALIGNAS,                     { "alignas",            SRC,      0,         0, nullptr }},
    { SALIGNOF,                     { "alignof",            SRC,      0,         0, nullptr }},
    { STYPEID,                      { "typeid",             SRC,      0,         0, nullptr }},
    { SSIZEOF_PACK,                 { "sizeof",             SRC, "type",    "pack", nullptr }},
    { SENUM_CLASS,                  { "enum",               SRC, "type",   "class", nullptr }},
    { SENUM_CLASS_DECLARATION,      { "enum_decl",          SRC, "type",   "class", nullptr }},
    { SOPERATOR_FUNCTION,           { "function",           SRC, "type","operator", nullptr }},
    { SOPERATOR_FUNCTION_DECL,      { "function_decl",      SRC, "type","operator", nullptr }},
    { SREF_QUALIFIER,               { "ref_qualifier",      SRC,      0,         0, nullptr }},

    // Objective-C
    { SRECEIVER,                    { "receiver",           SRC,      0,         0, nullptr }},
    { SMESSAGE,                     { "message",            SRC,      0,         0, nullptr }},
    { SSELECTOR,                    { "selector",           SRC,      0,         0, nullptr }},
    { SPROTOCOL_LIST,               { "protocol_list",      SRC,      0,         0, nullptr }},
    { SCATEGORY,                    { "category",           SRC,      0,         0, nullptr }},
    { SPROTOCOL,                    { "protocol",           SRC,      0,         0, nullptr }},
    { SREQUIRED_DEFAULT,            { "required",           SRC, "type", "default", nullptr }},
    { SREQUIRED,                    { "required",           SRC,      0,         0, nullptr }},
    { SOPTIONAL,                    { "optional",           SRC,      0,         0, nullptr }},
    { SPROPERTY,                    { "property",           SRC,      0,         0, nullptr }},
    { SATTRIBUTE_LIST,              { "attribute_list",     SRC,      0,         0, nullptr }},
    { SSYNTHESIZE,                  { "synthesize",         SRC,      0,         0, nullptr }},
    { SDYNAMIC,                     { "dynamic",            SRC,      0,         0, nullptr }},
    { SENCODE,                      { "encode",             SRC,      0,         0, nullptr }},
    { SAUTORELEASEPOOL,             { "autoreleasepool",    SRC,      0,         0, nullptr }},
    { SCOMPATIBILITY_ALIAS,         { "compatibility_alias",SRC,      0,         0, nullptr }},
    { SNIL,                         { "literal",            SRC, "type", "nil", nullptr }},
    { SCLASS_INTERFACE,             { "class",              SRC, "type", "@interface", nullptr }},
    { SCLASS_IMPLEMENTATION,        { "class",              SRC, "type", "@implementation", nullptr }},
    { SPROTOCOL_DECLARATION,        { "protocol_decl",      SRC,      0,         0, nullptr }},

    // casts
    { SCAST,                        { "cast",               SRC,      0,         0, nullptr }},
    { SCONST_CAST,                  { "cast",               SRC, "type",   "const", nullptr }},
    { SDYNAMIC_CAST,                { "cast",               SRC, "type", "dynamic", nullptr }},
    { SREINTERPRET_CAST,            { "cast",               SRC, "type", "reinterpret", nullptr }},
    { SSTATIC_CAST,                 { "cast",               SRC, "type",  "static", nullptr }},

    // srcMLOutput only
    { SPOSITION,                    { "position",           SRC,      0,         0, nullptr }},

    // Other
    { SCUDA_ARGUMENT_LIST,          { "argument_list",      SRC, "type", "cuda", nullptr }},

    // OpenMP
    { SOMP_DIRECTIVE,               { "directive",          OMP,      0,         0, nullptr }},
    { SOMP_NAME,                    { "name",               OMP,      0,         0, nullptr }},
    { SOMP_CLAUSE,                  { "clause",             OMP,      0,         0, nullptr }},
    { SOMP_ARGUMENT_LIST,           { "argument_list",      OMP,      0,         0, nullptr }},
    { SOMP_ARGUMENT,                { "argument",           OMP,      0,         0, nullptr }},
    { SOMP_EXPRESSION,              { "expr",               OMP,      0,         0, nullptr }},

    //
    { SEMPTY,                       { "empty_stmt",         SRC,      0,         0, nullptr }},

};

#pragma GCC diagnostic warning "-Wunused-parameter"

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
      openelementcount(0), tabsize(ts), depth(0)
{
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
void srcMLOutput::initNamespaces(const std::vector<std::string>& prefix, const std::vector<std::string>& uri) {

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

    for (std::vector<std::string>::size_type outer_pos = 0; outer_pos < uri.size(); ++outer_pos) {

        // find where the new URI is in the default URI list, or not
        auto posit = std::find(num2uri.begin(), num2uri.end(), uri[outer_pos]);
        if (posit != num2uri.end()) {

            // update the default prefix
            num2prefix[std::distance(num2uri.begin(), posit)] = prefix[outer_pos];

        } else {

            // create a new entry for this URI
            num2prefix.push_back(prefix[outer_pos]);
            num2uri.push_back(uri[outer_pos]);
        }
    }

    // keep track of which num2's were used
    num2used.resize(num2uri.size(), false);

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
inline void srcMLOutput::srcMLTextWriterStartElement(xmlTextWriter* xout, const xmlChar* s) {

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
 * @param aline to convert to string
 *
 * Convert the line to string attribute.
 *
 * @returns the line as a string.
 */
void srcMLOutput::outputPosition() {

    if (end_position_output)
        return;

    const char * position_localname = "position";
    const char* prefix = num2prefix[3].c_str();
    num2used[3] = true;

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST position_localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST position_localname, 0);

    xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST std::to_string(last_line).c_str());

    if(isoption(options,SRCML_OPTION_LINE))
        xmlTextWriterWriteAttribute(xout, BAD_CAST line2Attribute.c_str(), BAD_CAST std::to_string(last_line2).c_str());

    xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST std::to_string(last_column).c_str());

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
 * outputXMLDecl
 *
 * Output the starting xml declaration.
 */
void srcMLOutput::outputXMLDecl() {

    // issue the xml declaration, but only if we want to
    if (depth == 0 && isoption(options, SRCML_OPTION_XML_DECL))
        xmlTextWriterStartDocument(xout, XML_VERSION, xml_encoding, XML_DECLARATION_STANDALONE);
}

/**
 * outputPreRootProcessingInstruction
 *
 * Output a pre-root preprocessing instruction.
 */
void srcMLOutput::outputPreRootProcessingInstruction() {

    if (depth == 0 && processing_instruction) {

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
        (false && depth == 0) && isoption(options, SRCML_OPTION_OPENMP) ? SRCML_EXT_OPENMP_NS_URI : 0,
    };

    // output the namespaces
    // record length of namespaces list
    ns_list_size = 0;
    for (unsigned int i = 0; i < sizeof(ns) / sizeof(ns[0]); ++i) {

        if (i == 0 && depth > 0)
            continue;

        if (!ns[i])
            continue;

        std::string prefix = "xmlns";
        if (num2prefix[i][0] != '\0') {
            prefix += ':';
            prefix += num2prefix[i];
        }

        ns_list_size += prefix.size();
        ns_list_size += strlen(ns[i]);
        ns_list_size += 4;

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

    // recording which namespaces are used on this unit
    num2used.resize(num2prefix.size(), false);

    const char * prefix = num2prefix[0].c_str();
    std::string maintag = prefix ? prefix : "";
    if (!maintag.empty())
        maintag += ":";
    maintag += "unit";

    // start of main tag
    srcMLTextWriterStartElement(xout, BAD_CAST /* type2name(SUNIT) */ maintag.c_str());

    // record where unit start tag name ends
    start_ns_pos = 1 + (int) maintag.size() + 1;

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

    if (output_macrolist)
        outputMacroList();

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

        // output the namespaces
        // record length of namespaces list
        reduced_ns = "";
        // Note: Skipping first namespace (srcML)
        for (unsigned int i = 1; i < num2prefix.size(); ++i) {

            if (!num2used[i])
                continue;

            std::string prefix = "xmlns";
            if (num2prefix[i][0] != '\0') {
                prefix += ':';
                prefix += num2prefix[i];
            }

            reduced_ns += prefix;
            reduced_ns += "=\"";
            reduced_ns += num2uri[i];
            reduced_ns += "\" ";
        }
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

    xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST std::to_string(token->getLine()).c_str());

    xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST std::to_string(token->getColumn()).c_str());

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

    xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST std::to_string(token->getLine() & 0xFFFF).c_str());
    xmlTextWriterWriteAttribute(xout, BAD_CAST line2Attribute.c_str(), BAD_CAST std::to_string(token->getLine() >> 16).c_str());

    xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST std::to_string(token->getColumn()).c_str());

    last_line = token->getLine() & 0xFFFF;
    last_line2 = token->getLine() >> 16;
    last_column = token->getColumn() + (int)token->getText().size();

    end_position_output = false;

    processText(token->getText());
}

inline void srcMLOutput::processToken(const antlr::RefToken& token, const char* name, const char* prefix, const char* attr_name, const char* attr_value) {

    processToken(token, name, prefix, attr_name, attr_value, 0, 0);
}

void srcMLOutput::processToken(const antlr::RefToken& token, const char* name, const char* prefix, const char* attr_name1, const char* attr_value1,
                                const char* attr_name2, const char* attr_value2) {

    if (name[0] == 0)
        return;

    if (isstart(token) || isempty(token)) {
        if (prefix[0] == 0)
            xmlTextWriterStartElement(xout, BAD_CAST name);
        else
            xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST name, 0);
        ++openelementcount;

        if (attr_name1)
            xmlTextWriterWriteAttribute(xout, BAD_CAST attr_name1, BAD_CAST attr_value1);

        if (attr_name2)
            xmlTextWriterWriteAttribute(xout, BAD_CAST attr_name2, BAD_CAST attr_value2);

    } 

    if (!isstart(token) || isempty(token)) {

        if (isoption(options, SRCML_OPTION_POSITION) && !isempty(token))
            outputPosition();

        xmlTextWriterEndElement(xout);
        --openelementcount;
    }
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
    int position = ElementPrefix[token->getType()];
    const char* prefix = num2prefix[position].c_str();
    num2used[position] = true;

    processToken(token, localname, prefix, "location", token->getText().c_str());
}

#endif

/**
 * outputToken
 * @param token token to output
 *
 * Get callback that output token and output.
 */
inline void srcMLOutput::outputToken(const antlr::RefToken& token) {

    auto search = process.find(token->getType());
    if (search != process.end() && search->second.name) {
        const Element& eparts = search->second;
        if (eparts.process)
            eparts.process(this, token, eparts.name, num2prefix[eparts.prefix].c_str());
        else
            processToken(token, eparts.name, num2prefix[eparts.prefix].c_str(), eparts.attr_name, eparts.attr_value);

        return;
    }

    processText(token);
}

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
