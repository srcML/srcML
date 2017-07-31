/**
 * @file srcMLOutputElements.hpp
 *
 * @copyright Copyright (C) 2017 srcML, LLC. (www.srcML.org)
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
    Definition of srcML Elements, including element name, prefix, attributes, and custom processing

    Included to be able to use inlined srcMLOutput methods
*/

#define params srcMLOutput* pout, const antlr::RefToken& token, const char* name, const char* prefix

#pragma GCC diagnostic ignored "-Wunused-parameter"

const std::unordered_map<int, Element> srcMLOutput::process = {

    { SUNIT,  { "unit", SRC, 0, 0, [](params) { pout->processUnit(token); }}},

    { START_ELEMENT_TOKEN,  { 0, SRC, 0, 0, nullptr }},

    { COMMENT_START, { "comment", SRC, 0, 0, [](params) {
        pout->processToken(token, name, prefix, "type", "block");
        pout->processText(token);
    }}},

   { COMMENT_END,   { "comment", SRC, 0, 0, [](params) { 

        pout->processText(token);

        // TODO: Not sure why this is needed. Part of COMMENT_END processing
        xmlTextWriterEndElement(pout->xout);
        --(pout->openelementcount);
    }}},

    { LINECOMMENT_START, { "comment", SRC, 0, 0, [](params) {
        pout->processToken(token, name, prefix, "type", "line");
        pout->processText(token);
    }}},

    { LINECOMMENT_END, { "comment", SRC, 0, 0, [](params) {

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


    { LINE_DOXYGEN_COMMENT_START, { "comment", SRC, 0, 0, [](params) {
        pout->processToken(token, name, prefix, "type", "line", "format", "doxygen");
        pout->processText(token);
    }}},

    { JAVADOC_COMMENT_START, { "comment", SRC, 0, 0, [](params) {
        pout->processToken(token, name, prefix, "type", "block", "format", "javadoc");
        pout->processText(token);
    }}},

    { DOXYGEN_COMMENT_START, { "comment", SRC, 0, 0, [](params) {
        pout->processToken(token, name, prefix, "type", "block", "format", "doxygen");
        pout->processText(token);
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

#if DEBUG
    { SMARKER,                      { "comment", SRC, 0, 0, [](params) {
        pout->processToken(token, name, prefix, "location", token->getText().c_str());
    }}},
#endif

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
    { CONTROL_CHAR,                 { "escape",             SRC,      0,         0, [](params) {

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

#undef params
