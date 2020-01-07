/**
 * @file srcMLOutputElements.hpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
    Definition of srcML Elements, including element name, prefix, attributes, and custom processing

    Included to be able to use inlined srcMLOutput methods
*/

const std::unordered_map<int, Element> srcMLOutput::process = {

    { SCOMMENT,                    { "comment",           SRC, "type",   "block",        0,      0 }},
    { SLINECOMMENT,                { "comment",           SRC, "type",    "line",        0,      0 }},
    { SLINE_DOXYGEN_COMMENT,       { "comment",           SRC, "type",    "line", "format", "doxygen" }},
    { SJAVADOC_COMMENT,            { "comment",           SRC, "type",   "block", "format", "javadoc" }},
    { SDOXYGEN_COMMENT,            { "comment",           SRC, "type",   "block", "format", "doxygen" }},

    { SSTRING,                     { "literal",           SRC, "type",  "string",     0,      0 }},
    { SCHAR,                       { "literal",           SRC, "type",    "char",     0,      0 }},
    { SLITERAL,                    { "literal",           SRC, "type",  "number",     0,      0 }},
    { SBOOLEAN,                    { "literal",           SRC, "type", "boolean",     0,      0 }},
    { SNULL,                       { "literal",           SRC, "type",    "null",     0,      0 }},
    { SCOMPLEX,                    { "literal",           SRC, "type", "complex",     0,      0 }},
    { SOPERATOR,                   { "operator",          SRC,      0,         0,     0,      0 }},
    { SMODIFIER,                   { "modifier",          SRC,      0,         0,     0,      0 }},
    { SNAME,                       { "name",              SRC,      0,         0,     0,      0 }},
    { SONAME,                      { "",                  SRC,      0,         0,     0,      0 }},
    { SCNAME,                      { "name",              SRC,      0,         0,     0,      0 }},
    { STYPE,                       { "type",              SRC,      0,         0,     0,      0 }},

    { STYPEPREV,                   { "type",              SRC,  "ref",    "prev",     0,      0 }},
    { SCONDITION,                  { "condition",         SRC,      0,         0,     0,      0 }},
    { SBLOCK,                      { "block",             SRC,      0,         0,     0,      0 }},
    { SCONTENT,                    { "block_content",     SRC,      0,         0,     0,      0 }},
    { SPSEUDO_BLOCK,               { "block",             SRC, "type",  "pseudo",     0,      0 }},
    { SINDEX,                      { "index",             SRC,      0,         0,     0,      0 }},
    { SDECLTYPE,                   { "decltype",          SRC,      0,         0,     0,      0 }},
    { STYPENAME,                   { "typename",          SRC,      0,         0,     0,      0 }},
    { SATOMIC,                     { "atomic",            SRC,      0,         0,     0,      0 }},
    { SSTATIC_ASSERT_STATEMENT,    { "assert",            SRC, "type",  "static",     0,      0 }},
    { SGENERIC_SELECTION,          { "generic_selection", SRC,      0,         0,     0,      0 }},
    { SGENERIC_SELECTOR,           { "selector",          SRC,      0,         0,     0,      0 }},
    { SGENERIC_ASSOCIATION_LIST,   { "association_list",  SRC,      0,         0,     0,      0 }},
    { SGENERIC_ASSOCIATION,        { "association",       SRC,      0,         0,     0,      0 }},

    { SEXPRESSION_STATEMENT,       { "expr_stmt",         SRC,      0,         0,     0,      0 }},
    { SEXPRESSION,                 { "expr",              SRC,      0,         0,     0,      0 }},

    { SDECLARATION_STATEMENT,      { "decl_stmt",         SRC,      0,         0,     0,      0 }},
    { SDECLARATION,                { "decl",              SRC,      0,         0,     0,      0 }},
    { SDECLARATION_INITIALIZATION, { "init",              SRC,      0,         0,     0,      0 }},
    { SDECLARATION_RANGE,          { "range",             SRC,      0,         0,     0,      0 }},

    { SBREAK_STATEMENT,            { "break",             SRC,      0,         0,     0,      0 }},
    { SCONTINUE_STATEMENT,         { "continue",          SRC,      0,         0,     0,      0 }},
    { SGOTO_STATEMENT,             { "goto",              SRC,      0,         0,     0,      0 }},
    { SLABEL_STATEMENT,            { "label",             SRC,      0,         0,     0,      0 }},

    { STYPEDEF,                    { "typedef",           SRC,      0,         0,     0,      0 }},
    { SASM,                        { "asm",               SRC,      0,         0,     0,      0 }},
    { SMACRO_CALL,                 { "macro",             SRC,      0,         0,     0,      0 }},
    { SENUM,                       { "enum",              SRC,      0,         0,     0,      0 }},
    { SENUM_DECLARATION,           { "enum_decl",         SRC,      0,         0,     0,      0 }},

    { SIF_STATEMENT,               { "if_stmt",           SRC,      0,         0,     0,      0 }},
    { SIF,                         { "if",                SRC,      0,         0,     0,      0 }},
    { STERNARY,                    { "ternary",           SRC,      0,         0,     0,      0 }},
    { STHEN,                       { "then",              SRC,      0,         0,     0,      0 }},
    { SELSE,                       { "else",              SRC,      0,         0,     0,      0 }},
    { SELSEIF,                     { "if",                SRC, "type",  "elseif",     0,      0 }},

    { SWHILE_STATEMENT,            { "while",             SRC,      0,         0,     0,      0 }},
    { STYPEOF,                     { "typeof",            SRC,      0,         0,     0,      0 }},
    { SDO_STATEMENT,               { "do",                SRC,      0,         0,     0,      0 }},

    { SSWITCH,                     { "switch",            SRC,      0,         0,     0,      0 }},
    { SCASE,                       { "case",              SRC,      0,         0,     0,      0 }},
    { SDEFAULT,                    { "default",           SRC,      0,         0,     0,      0 }},

    { SFOR_STATEMENT,              { "for",               SRC,      0,         0,     0,      0 }},
    { SFOREACH_STATEMENT,          { "foreach",           SRC,      0,         0,     0,      0 }},
    { SCONTROL,                    { "control",           SRC,      0,         0,     0,      0 }},
    { SCONTROL_INITIALIZATION,     { "init",              SRC,      0,         0,     0,      0 }},
    { SCONTROL_CONDITION,          { "condition",         SRC,      0,         0,     0,      0 }},
    { SCONTROL_INCREMENT,          { "incr",              SRC,      0,         0,     0,      0 }},
    { SFOR_LIKE_CONTROL,           { "init",              SRC,      0,         0,     0,      0 }},

    // functions
    { SFUNCTION_DEFINITION,        { "function",          SRC,      0,         0,     0,      0 }},
    { SFUNCTION_DECLARATION,       { "function_decl",     SRC,      0,         0,     0,      0 }},
    { SFUNCTION_LAMBDA,            { "lambda",            SRC,      0,         0,     0,      0 }},
    { SFUNCTION_SPECIFIER,         { "specifier",         SRC,      0,         0,     0,      0 }},
    { SRETURN_STATEMENT,           { "return",            SRC,      0,         0,     0,      0 }},
    { SFUNCTION_CALL,              { "call",              SRC,      0,         0,     0,      0 }},
    { SSIZEOF_CALL,                { "sizeof",            SRC,      0,         0,     0,      0 }},
    { SPARAMETER_LIST,             { "parameter_list",    SRC,      0,         0,     0,      0 }},
    { SPARAMETER,                  { "parameter",         SRC,      0,         0,     0,      0 }},
    { SKRPARAMETER_LIST,           { "krparameter_list",  SRC,      0,         0,     0,      0 }},
    { SKRPARAMETER,                { "krparameter",       SRC,      0,         0,     0,      0 }},
    { SARGUMENT_LIST,              { "argument_list",     SRC,      0,         0,     0,      0 }},
    { SARGUMENT,                   { "argument",          SRC,      0,         0,     0,      0 }},
    { SLAMBDA_CAPTURE,             { "capture",           SRC,      0,         0,     0,      0 }},
    { SPSEUDO_PARAMETER_LIST,      { "parameter_list",    SRC, "type",  "pseudo",     0,      0 }},
    { SINDEXER_PARAMETER_LIST,     { "parameter_list",    SRC, "type", "indexer",     0,      0 }},

    // struct,  union
    { SSTRUCT,                     { "struct",            SRC,      0,         0,     0,      0 }},
    { SSTRUCT_DECLARATION,         { "struct_decl",       SRC,      0,         0,     0,      0 }},
    { SUNION,                      { "union",             SRC,      0,         0,     0,      0 }},
    { SUNION_DECLARATION,          { "union_decl",        SRC,      0,         0,     0,      0 }},

    // class
    { SCLASS,                      { "class",             SRC,      0,         0,     0,      0 }},
    { SCLASS_DECLARATION,          { "class_decl",        SRC,      0,         0,     0,      0 }},
    { SPUBLIC_ACCESS,              { "public",            SRC,      0,         0,     0,      0 }},
    { SPUBLIC_ACCESS_DEFAULT,      { "public",            SRC, "type", "default",     0,      0 }},
    { SPRIVATE_ACCESS,             { "private",           SRC,      0,         0,     0,      0 }},
    { SPRIVATE_ACCESS_DEFAULT,     { "private",           SRC, "type", "default",     0,      0 }},
    { SPROTECTED_ACCESS,           { "protected",         SRC,      0,         0,     0,      0 }},
    { SPROTECTED_ACCESS_DEFAULT,   { "protected",         SRC, "type", "default",     0,      0 }},

    // Qt
    { SSIGNAL_ACCESS,              { "signals",           SRC,      0,         0,     0,      0 }},
    { SFOREVER_STATEMENT,          { "forever",           SRC,      0,         0,     0,      0 }},
    { SEMIT_STATEMENT,             { "emit",              SRC,      0,         0,     0,      0 }},

    { SMEMBER_INITIALIZATION_LIST, { "member_init_list",  SRC,      0,         0,     0,      0 }},
    { SMEMBER_INITIALIZATION,      { "call",              SRC,      0,         0,     0,      0 }},
    { SCONSTRUCTOR_DEFINITION,     { "constructor",       SRC,      0,         0,     0,      0 }},
    { SCONSTRUCTOR_DECLARATION,    { "constructor_decl",  SRC,      0,         0,     0,      0 }},
    { SDESTRUCTOR_DEFINITION,      { "destructor",        SRC,      0,         0,     0,      0 }},
    { SDESTRUCTOR_DECLARATION,     { "destructor_decl",   SRC,      0,         0,     0,      0 }},
    { SDERIVATION_LIST,            { "super_list",        SRC,      0,         0,     0,      0 }},
    { SDERIVATION,                 { "super",             SRC,      0,         0,     0,      0 }},
    { SFRIEND,                     { "friend",            SRC,      0,         0,     0,      0 }},
    { SCLASS_SPECIFIER,            { "specifier",         SRC,      0,         0,     0,      0 }},

    // extern definition
    { SEXTERN,                     { "extern",            SRC,      0,         0,     0,      0 }},

    // namespaces
    { SNAMESPACE,                  { "namespace",         SRC,      0,         0,     0,      0 }},
    { SUSING_DIRECTIVE,            { "using",             SRC,      0,         0,     0,      0 }},

    // exception handling
    { STRY_BLOCK,                  { "try",               SRC,      0,         0,     0,      0 }},
    { SCATCH_BLOCK,                { "catch",             SRC,      0,         0,     0,      0 }},
    { SFINALLY_BLOCK,              { "finally",           SRC,      0,         0,     0,      0 }},
    { STHROW_STATEMENT,            { "throw",             SRC,      0,         0,     0,      0 }},
    { STHROW_SPECIFIER,            { "throw",             SRC,      0,         0,     0,      0 }},
    { STHROW_SPECIFIER_JAVA,       { "throws",            SRC,      0,         0,     0,      0 }},
    { SNOEXCEPT,                   { "noexcept",          SRC,      0,         0,     0,      0 }},

    // template
    { STEMPLATE,                   { "template",          SRC,      0,         0,     0,      0 }},
    { SGENERIC_ARGUMENT,           { "argument",          SRC,      0,         0,     0,      0 }},
    { SGENERIC_ARGUMENT_LIST,      { "argument_list",     SRC, "type", "generic",     0,      0 }},
    { STEMPLATE_PARAMETER,         { "parameter",         SRC,      0,         0,     0,      0 }},
    { STEMPLATE_PARAMETER_LIST,    { "parameter_list",    SRC,      0,         0,     0,      0 }},
    { SGENERIC_PARAMETER,          { "parameter",         SRC,      0,         0,     0,      0 }},
    { SGENERIC_PARAMETER_LIST,     { "parameter_list",    SRC, "type", "generic",     0,      0 }},

    // cpp
    { SCPP_DIRECTIVE,              { "directive",         CPP,      0,         0,     0,      0 }},
    { SCPP_FILENAME,               { "file",              CPP,      0,         0,     0,      0 }},
    { SCPP_NUMBER,                 { "number",            CPP,      0,         0,     0,      0 }},
    { SCPP_LITERAL,                { "literal",           CPP,      0,         0,     0,      0 }},
    { SCPP_INCLUDE,                { "include",           CPP,      0,         0,     0,      0 }},
    { SCPP_DEFINE,                 { "define",            CPP,      0,         0,     0,      0 }},
    { SCPP_UNDEF,                  { "undef",             CPP,      0,         0,     0,      0 }},
    { SCPP_LINE,                   { "line",              CPP,      0,         0,     0,      0 }},
    { SCPP_IF,                     { "if",                CPP,      0,         0,     0,      0 }},
    { SCPP_IFDEF,                  { "ifdef",             CPP,      0,         0,     0,      0 }},
    { SCPP_IFNDEF,                 { "ifndef",            CPP,      0,         0,     0,      0 }},
    { SCPP_ELSE,                   { "else",              CPP,      0,         0,     0,      0 }},
    { SCPP_ELIF,                   { "elif",              CPP,      0,         0,     0,      0 }},
    { SCPP_ENDIF,                  { "endif",             CPP,      0,         0,     0,      0 }},
    { SCPP_THEN,                   { "then",              CPP,      0,         0,     0,      0 }},
    { SCPP_PRAGMA,                 { "pragma",            CPP,      0,         0,     0,      0 }},
    { SCPP_ERROR,                  { "error",             CPP,      0,         0,     0,      0 }},
    { SCPP_WARNING,                { "warning",           CPP,      0,         0,     0,      0 }},
    { SCPP_MACRO_DEFN,             { "macro",             CPP,      0,         0,     0,      0 }},
    { SCPP_MACRO_VALUE,            { "value",             CPP,      0,         0,     0,      0 }},
    { SCPP_EMPTY,                  { "empty",             CPP,      0,         0,     0,      0 }},

#if DEBUG
    { SMARKER,                     { "marker",            SRC, "item",         0,     0,      0 }},
#endif

    // C# cpp
    { SCPP_REGION,                 { "region",            CPP,      0,         0,     0,      0 }},
    { SCPP_ENDREGION,              { "endregion",         CPP,      0,         0,     0,      0 }},

    // Objective-C cpp
    { SCPP_IMPORT,                 { "import",            CPP,      0,         0,     0,      0 }},

    // debug
    { SMARKER,                     { "marker",            SRC,      0,         0,     0,      0 }},
    { SERROR_PARSE,                { "parse",             SRC,      0,         0,     0,      0 }},
    { SERROR_MODE,                 { "mode",              SRC,      0,         0,     0,      0 }},

    // C#
    { SLOCK_STATEMENT,             { "lock",              SRC,      0,         0,     0,      0 }},
    { SFIXED_STATEMENT,            { "fixed",             SRC,      0,         0,     0,      0 }},
    { SCHECKED_STATEMENT,          { "checked",           SRC,      0,         0,     0,      0 }},
    { SUNCHECKED_STATEMENT,        { "unchecked",         SRC,      0,         0,     0,      0 }},
    { SUNSAFE_STATEMENT,           { "unsafe",            SRC,      0,         0,     0,      0 }},
    { SUSING_STATEMENT,            { "using_stmt",        SRC,      0,         0,     0,      0 }},
    { SFUNCTION_DELEGATE,          { "delegate",          SRC,      0,         0,     0,      0 }},
    { SEVENT,                      { "event",             SRC,      0,         0,     0,      0 }},
    { SCONSTRAINT,                 { "constraint",        SRC,      0,         0,     0,      0 }},

    // Java elements
    { SEXTENDS,                    { "extends",           SRC,      0,         0,     0,      0 }},
    { SIMPLEMENTS,                 { "implements",        SRC,      0,         0,     0,      0 }},
    { SIMPORT,                     { "import",            SRC,      0,         0,     0,      0 }},
    { SPACKAGE,                    { "package",           SRC,      0,         0,     0,      0 }},
    { SASSERT_STATEMENT,           { "assert",            SRC,      0,         0,     0,      0 }},
    { SSYNCHRONIZED_STATEMENT,     { "synchronized",      SRC,      0,         0,     0,      0 }},
    { SINTERFACE,                  { "interface",         SRC,      0,         0,     0,      0 }},
    { SINTERFACE_DECLARATION,      { "interface_decl",    SRC,      0,         0,     0,      0 }},
    { SANNOTATION_DEFN,            { "annotation_defn",   SRC,      0,         0,     0,      0 }},
    { SSTATIC_BLOCK,               { "static",            SRC,      0,         0,     0,      0 }},

    // special characters
    { SATTRIBUTE,                  { "attribute",         SRC,      0,         0,     0,      0 }},
    { STARGET,                     { "target",            SRC,      0,         0,     0,      0 }},

    // linq
    { SLINQ,                       { "linq",              SRC,      0,         0,     0,      0 }},
    { SFROM,                       { "from",              SRC,      0,         0,     0,      0 }},
    { SSELECT,                     { "select",            SRC,      0,         0,     0,      0 }},
    { SWHERE,                      { "where",             SRC,      0,         0,     0,      0 }},
    { SLET,                        { "let",               SRC,      0,         0,     0,      0 }},
    { SORDERBY,                    { "orderby",           SRC,      0,         0,     0,      0 }},
    { SGROUP,                      { "group",             SRC,      0,         0,     0,      0 }},
    { SJOIN,                       { "join",              SRC,      0,         0,     0,      0 }},
    { SIN,                         { "in",                SRC,      0,         0,     0,      0 }},
    { SON,                         { "on",                SRC,      0,         0,     0,      0 }},
    { SEQUALS,                     { "equals",            SRC,      0,         0,     0,      0 }},
    { SBY,                         { "by",                SRC,      0,         0,     0,      0 }},
    { SINTO,                       { "into",              SRC,      0,         0,     0,      0 }},

    // special characters
    { CONTROL_CHAR,                { "escape",            SRC, "char",         0,     0,      0 }},

    { SANNOTATION,                 { "annotation",        SRC,      0,         0,     0,      0 }},

    // C++
    { SALIGNAS,                    { "alignas",           SRC,      0,         0,     0,      0 }},
    { SALIGNOF,                    { "alignof",           SRC,      0,         0,     0,      0 }},
    { STYPEID,                     { "typeid",            SRC,      0,         0,     0,      0 }},
    { SSIZEOF_PACK,                { "sizeof",            SRC, "type",    "pack",     0,      0 }},
    { SENUM_CLASS,                 { "enum",              SRC, "type",   "class",     0,      0 }},
    { SENUM_CLASS_DECLARATION,     { "enum_decl",         SRC, "type",   "class",     0,      0 }},
    { SOPERATOR_FUNCTION,          { "function",          SRC, "type","operator",     0,      0 }},
    { SOPERATOR_FUNCTION_DECL,     { "function_decl",     SRC, "type","operator",     0,      0 }},
    { SREF_QUALIFIER,              { "ref_qualifier",     SRC,      0,         0,     0,      0 }},

    // Objective-C
    { SRECEIVER,                   { "receiver",            SRC,      0,         0,     0,      0 }},
    { SMESSAGE,                    { "message",             SRC,      0,         0,     0,      0 }},
    { SSELECTOR,                   { "selector",            SRC,      0,         0,     0,      0 }},
    { SPROTOCOL_LIST,              { "protocol_list",       SRC,      0,         0,     0,      0 }},
    { SCATEGORY,                   { "category",            SRC,      0,         0,     0,      0 }},
    { SPROTOCOL,                   { "protocol",            SRC,      0,         0,     0,      0 }},
    { SREQUIRED_DEFAULT,           { "required",            SRC, "type", "default",     0,      0 }},
    { SREQUIRED,                   { "required",            SRC,      0,         0,     0,      0 }},
    { SOPTIONAL,                   { "optional",            SRC,      0,         0,     0,      0 }},
    { SPROPERTY,                   { "property",            SRC,      0,         0,     0,      0 }},
    { SATTRIBUTE_LIST,             { "attribute_list",      SRC,      0,         0,     0,      0 }},
    { SSYNTHESIZE,                 { "synthesize",          SRC,      0,         0,     0,      0 }},
    { SDYNAMIC,                    { "dynamic",             SRC,      0,         0,     0,      0 }},
    { SENCODE,                     { "encode",              SRC,      0,         0,     0,      0 }},
    { SAUTORELEASEPOOL,            { "autoreleasepool",     SRC,      0,         0,     0,      0 }},
    { SCOMPATIBILITY_ALIAS,        { "compatibility_alias", SRC,      0,         0,     0,      0 }},
    { SNIL,                        { "literal",             SRC, "type",     "nil",     0,      0 }},
    { SPROTOCOL_DECLARATION,       { "protocol_decl",       SRC,      0,         0,     0,      0 }},

    { SCLASS_INTERFACE,            { "class",       SRC, "type",      "@interface",     0,      0 }},
    { SCLASS_IMPLEMENTATION,       { "class",       SRC, "type", "@implementation",     0,      0 }},

    // casts
    { SCAST,                       { "cast",              SRC,      0,             0,   0,      0 }},
    { SCONST_CAST,                 { "cast",              SRC, "type",       "const",   0,      0 }},
    { SDYNAMIC_CAST,               { "cast",              SRC, "type",     "dynamic",   0,      0 }},
    { SREINTERPRET_CAST,           { "cast",              SRC, "type", "reinterpret",   0,      0 }},
    { SSTATIC_CAST,                { "cast",              SRC, "type",      "static",   0,      0 }},

    // srcMLOutput only
    { SPOSITION,                   { "position",          SRC,      0,         0,     0,      0 }},

    // Other
    { SCUDA_ARGUMENT_LIST,         { "argument_list",     SRC, "type",    "cuda",     0,      0 }},

    // OpenMP
    { SOMP_DIRECTIVE,              { "directive",         OMP,      0,         0,     0,      0 }},
    { SOMP_NAME,                   { "name",              OMP,      0,         0,     0,      0 }},
    { SOMP_CLAUSE,                 { "clause",            OMP,      0,         0,     0,      0 }},
    { SOMP_ARGUMENT_LIST,          { "argument_list",     OMP,      0,         0,     0,      0 }},
    { SOMP_ARGUMENT,               { "argument",          OMP,      0,         0,     0,      0 }},
    { SOMP_EXPRESSION,             { "expr",              OMP,      0,         0,     0,      0 }},

    //
    { SEMPTY,                      { "empty_stmt",        SRC,      0,         0,     0,      0 }},
};
