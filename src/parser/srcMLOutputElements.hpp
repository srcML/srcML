// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcMLOutputElements.hpp
 *
 * @copyright Copyright (C) 2017-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Definition of srcML Elements, including element name, prefix, attributes, and custom processing
 *
 * Included to be able to use inlined srcMLOutput methods
 */

using TOKEN = srcMLParserTokenTypes;

const std::unordered_map<int, Element> srcMLOutput::process = {

    { TOKEN::SCOMMENT,                    { "comment",           SRC, "type",   "block",        0,      0 }},
    { TOKEN::SLINECOMMENT,                { "comment",           SRC, "type",    "line",        0,      0 }},
    { TOKEN::SLINE_DOXYGEN_COMMENT,       { "comment",           SRC, "type",    "line", "format", "doxygen" }},
    { TOKEN::SJAVADOC_COMMENT,            { "comment",           SRC, "type",   "block", "format", "javadoc" }},
    { TOKEN::SDOXYGEN_COMMENT,            { "comment",           SRC, "type",   "block", "format", "doxygen" }},

    { TOKEN::SSTRING,                     { "literal",           SRC, "type",  "string",     0,      0 }},
    { TOKEN::SCHAR,                       { "literal",           SRC, "type",    "char",     0,      0 }},
    { TOKEN::SLITERAL,                    { "literal",           SRC, "type",  "number",     0,      0 }},
    { TOKEN::SBOOLEAN,                    { "literal",           SRC, "type", "boolean",     0,      0 }},
    { TOKEN::SNULL,                       { "literal",           SRC, "type",    "null",     0,      0 }},
    { TOKEN::SCOMPLEX,                    { "literal",           SRC, "type", "complex",     0,      0 }},
    { TOKEN::SOPERATOR,                   { "operator",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SMODIFIER,                   { "modifier",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SNAME,                       { "name",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SONAME,                      { "",                  SRC,      0,         0,     0,      0 }},
    { TOKEN::SCNAME,                      { "name",              SRC,      0,         0,     0,      0 }},
    { TOKEN::STYPE,                       { "type",              SRC,      0,         0,     0,      0 }},

    { TOKEN::STYPEPREV,                   { "type",              SRC,  "ref",    "prev",     0,      0 }},
    { TOKEN::SCONDITION,                  { "condition",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SBLOCK,                      { "block",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SCONTENT,                    { "block_content",     SRC,      0,         0,     0,      0 }},
    { TOKEN::SPSEUDO_BLOCK,               { "block",             SRC, "type",  "pseudo",     0,      0 }},
    { TOKEN::SINDEX,                      { "index",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SDECLTYPE,                   { "decltype",          SRC,      0,         0,     0,      0 }},
    { TOKEN::STYPENAME,                   { "typename",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SATOMIC,                     { "atomic",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SSTATIC_ASSERT_STATEMENT,    { "assert",            SRC, "type",  "static",     0,      0 }},
    { TOKEN::SGENERIC_SELECTION,          { "generic_selection", SRC,      0,         0,     0,      0 }},
    { TOKEN::SGENERIC_SELECTOR,           { "selector",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SGENERIC_ASSOCIATION_LIST,   { "association_list",  SRC,      0,         0,     0,      0 }},
    { TOKEN::SGENERIC_ASSOCIATION,        { "association",       SRC,      0,         0,     0,      0 }},

    { TOKEN::SEXPRESSION_STATEMENT,       { "expr_stmt",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SEXPRESSION,                 { "expr",              SRC,      0,         0,     0,      0 }},

    { TOKEN::SDECLARATION_STATEMENT,      { "decl_stmt",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SDECLARATION,                { "decl",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SDECLARATION_INITIALIZATION, { "init",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SDECLARATION_RANGE,          { "range",             SRC,      0,         0,     0,      0 }},

    { TOKEN::SBREAK_STATEMENT,            { "break",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SCONTINUE_STATEMENT,         { "continue",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SGOTO_STATEMENT,             { "goto",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SLABEL_STATEMENT,            { "label",             SRC,      0,         0,     0,      0 }},

    { TOKEN::STYPEDEF,                    { "typedef",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SASM,                        { "asm",               SRC,      0,         0,     0,      0 }},
    { TOKEN::SMACRO_CALL,                 { "macro",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SENUM,                       { "enum",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SENUM_DECLARATION,           { "enum_decl",         SRC,      0,         0,     0,      0 }},

    { TOKEN::SIF_STATEMENT,               { "if_stmt",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SIF,                         { "if",                SRC,      0,         0,     0,      0 }},
    { TOKEN::STERNARY,                    { "ternary",           SRC,      0,         0,     0,      0 }},
    { TOKEN::STHEN,                       { "then",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SELSE,                       { "else",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SELSEIF,                     { "if",                SRC, "type",  "elseif",     0,      0 }},

    { TOKEN::SWHILE_STATEMENT,            { "while",             SRC,      0,         0,     0,      0 }},
    { TOKEN::STYPEOF,                     { "typeof",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SDO_STATEMENT,               { "do",                SRC,      0,         0,     0,      0 }},

    { TOKEN::SSWITCH,                     { "switch",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SCASE,                       { "case",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SDEFAULT,                    { "default",           SRC,      0,         0,     0,      0 }},

    { TOKEN::SFOR_STATEMENT,              { "for",               SRC,      0,         0,     0,      0 }},
    { TOKEN::SFOREACH_STATEMENT,          { "foreach",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SCONTROL,                    { "control",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SCONTROL_INITIALIZATION,     { "init",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SCONTROL_CONDITION,          { "condition",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SCONTROL_INCREMENT,          { "incr",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SFOR_LIKE_CONTROL,           { "init",              SRC,      0,         0,     0,      0 }},

    // functions
    { TOKEN::SFUNCTION_DEFINITION,        { "function",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SFUNCTION_DECLARATION,       { "function_decl",     SRC,      0,         0,     0,      0 }},
    { TOKEN::SFUNCTION_LAMBDA,            { "lambda",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SFUNCTION_SPECIFIER,         { "specifier",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SRETURN_STATEMENT,           { "return",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SFUNCTION_CALL,              { "call",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SSIZEOF_CALL,                { "sizeof",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SPARAMETER_LIST,             { "parameter_list",    SRC,      0,         0,     0,      0 }},
    { TOKEN::SPARAMETER,                  { "parameter",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SKRPARAMETER_LIST,           { "krparameter_list",  SRC,      0,         0,     0,      0 }},
    { TOKEN::SKRPARAMETER,                { "krparameter",       SRC,      0,         0,     0,      0 }},
    { TOKEN::SARGUMENT_LIST,              { "argument_list",     SRC,      0,         0,     0,      0 }},
    { TOKEN::SARGUMENT,                   { "argument",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SLAMBDA_CAPTURE,             { "capture",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SPSEUDO_PARAMETER_LIST,      { "parameter_list",    SRC, "type",  "pseudo",     0,      0 }},
    { TOKEN::SINDEXER_PARAMETER_LIST,     { "parameter_list",    SRC, "type", "indexer",     0,      0 }},

    // struct,  union
    { TOKEN::SSTRUCT,                     { "struct",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SSTRUCT_DECLARATION,         { "struct_decl",       SRC,      0,         0,     0,      0 }},
    { TOKEN::SUNION,                      { "union",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SUNION_DECLARATION,          { "union_decl",        SRC,      0,         0,     0,      0 }},

    // class
    { TOKEN::SCLASS,                      { "class",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SCLASS_DECLARATION,          { "class_decl",        SRC,      0,         0,     0,      0 }},
    { TOKEN::SPUBLIC_ACCESS,              { "public",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SPUBLIC_ACCESS_DEFAULT,      { "public",            SRC, "type", "default",     0,      0 }},
    { TOKEN::SPRIVATE_ACCESS,             { "private",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SPRIVATE_ACCESS_DEFAULT,     { "private",           SRC, "type", "default",     0,      0 }},
    { TOKEN::SPROTECTED_ACCESS,           { "protected",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SPROTECTED_ACCESS_DEFAULT,   { "protected",         SRC, "type", "default",     0,      0 }},

    // Qt
    { TOKEN::SSIGNAL_ACCESS,              { "signals",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SFOREVER_STATEMENT,          { "forever",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SEMIT_STATEMENT,             { "emit",              SRC,      0,         0,     0,      0 }},

    { TOKEN::SMEMBER_INITIALIZATION_LIST, { "member_init_list",  SRC,      0,         0,     0,      0 }},
    { TOKEN::SMEMBER_INITIALIZATION,      { "call",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SCONSTRUCTOR_DEFINITION,     { "constructor",       SRC,      0,         0,     0,      0 }},
    { TOKEN::SCONSTRUCTOR_DECLARATION,    { "constructor_decl",  SRC,      0,         0,     0,      0 }},
    { TOKEN::SDESTRUCTOR_DEFINITION,      { "destructor",        SRC,      0,         0,     0,      0 }},
    { TOKEN::SDESTRUCTOR_DECLARATION,     { "destructor_decl",   SRC,      0,         0,     0,      0 }},
    { TOKEN::SDERIVATION_LIST,            { "super_list",        SRC,      0,         0,     0,      0 }},
    { TOKEN::SDERIVATION,                 { "super",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SFRIEND,                     { "friend",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SCLASS_SPECIFIER,            { "specifier",         SRC,      0,         0,     0,      0 }},

    // extern definition
    { TOKEN::SEXTERN,                     { "extern",            SRC,      0,         0,     0,      0 }},

    // namespaces
    { TOKEN::SNAMESPACE,                  { "namespace",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SUSING_DIRECTIVE,            { "using",             SRC,      0,         0,     0,      0 }},

    // exception handling
    { TOKEN::STRY_BLOCK,                  { "try",               SRC,      0,         0,     0,      0 }},
    { TOKEN::SCATCH_BLOCK,                { "catch",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SFINALLY_BLOCK,              { "finally",           SRC,      0,         0,     0,      0 }},
    { TOKEN::STHROW_STATEMENT,            { "throw",             SRC,      0,         0,     0,      0 }},
    { TOKEN::STHROW_SPECIFIER,            { "throw",             SRC,      0,         0,     0,      0 }},
    { TOKEN::STHROW_SPECIFIER_JAVA,       { "throws",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SNOEXCEPT,                   { "noexcept",          SRC,      0,         0,     0,      0 }},

    // template
    { TOKEN::STEMPLATE,                   { "template",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SGENERIC_ARGUMENT,           { "argument",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SGENERIC_ARGUMENT_LIST,      { "argument_list",     SRC, "type", "generic",     0,      0 }},
    { TOKEN::STEMPLATE_PARAMETER,         { "parameter",         SRC,      0,         0,     0,      0 }},
    { TOKEN::STEMPLATE_PARAMETER_LIST,    { "parameter_list",    SRC,      0,         0,     0,      0 }},
    { TOKEN::SGENERIC_PARAMETER,          { "parameter",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SGENERIC_PARAMETER_LIST,     { "parameter_list",    SRC, "type", "generic",     0,      0 }},

    // cpp
    { TOKEN::SCPP_DIRECTIVE,              { "directive",         CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_FILENAME,               { "file",              CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_NUMBER,                 { "number",            CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_LITERAL,                { "literal",           CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_INCLUDE,                { "include",           CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_DEFINE,                 { "define",            CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_UNDEF,                  { "undef",             CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_LINE,                   { "line",              CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_IF,                     { "if",                CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_IFDEF,                  { "ifdef",             CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_IFNDEF,                 { "ifndef",            CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_ELSE,                   { "else",              CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_ELIF,                   { "elif",              CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_ENDIF,                  { "endif",             CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_THEN,                   { "then",              CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_PRAGMA,                 { "pragma",            CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_ERROR,                  { "error",             CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_WARNING,                { "warning",           CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_MACRO_DEFN,             { "macro",             CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_MACRO_VALUE,            { "value",             CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_EMPTY,                  { "empty",             CPP,      0,         0,     0,      0 }},

#ifdef DEBUG
    { TOKEN::SMARKER,                     { "marker",            SRC, "item",         0,     0,      0 }},
#endif

    // C# cpp
    { TOKEN::SCPP_REGION,                 { "region",            CPP,      0,         0,     0,      0 }},
    { TOKEN::SCPP_ENDREGION,              { "endregion",         CPP,      0,         0,     0,      0 }},

    // Objective-C cpp
    { TOKEN::SCPP_IMPORT,                 { "import",            CPP,      0,         0,     0,      0 }},

    // debug
    { TOKEN::SMARKER,                     { "marker",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SERROR_PARSE,                { "parse",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SERROR_MODE,                 { "mode",              SRC,      0,         0,     0,      0 }},

    // C#
    { TOKEN::SLOCK_STATEMENT,             { "lock",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SFIXED_STATEMENT,            { "fixed",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SCHECKED_STATEMENT,          { "checked",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SUNCHECKED_STATEMENT,        { "unchecked",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SUNSAFE_STATEMENT,           { "unsafe",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SUSING_STATEMENT,            { "using_stmt",        SRC,      0,         0,     0,      0 }},
    { TOKEN::SFUNCTION_DELEGATE,          { "delegate",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SEVENT,                      { "event",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SCONSTRAINT,                 { "constraint",        SRC,      0,         0,     0,      0 }},

    // Java elements
    { TOKEN::SEXTENDS,                    { "extends",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SIMPLEMENTS,                 { "implements",        SRC,      0,         0,     0,      0 }},
    { TOKEN::SIMPORT,                     { "import",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SPACKAGE,                    { "package",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SASSERT_STATEMENT,           { "assert",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SSYNCHRONIZED_STATEMENT,     { "synchronized",      SRC,      0,         0,     0,      0 }},
    { TOKEN::SINTERFACE,                  { "interface",         SRC,      0,         0,     0,      0 }},
    { TOKEN::SINTERFACE_DECLARATION,      { "interface_decl",    SRC,      0,         0,     0,      0 }},
    { TOKEN::SANNOTATION_DEFN,            { "annotation_defn",   SRC,      0,         0,     0,      0 }},
    { TOKEN::SSTATIC_BLOCK,               { "static",            SRC,      0,         0,     0,      0 }},

    // special characters
    { TOKEN::SATTRIBUTE,                  { "attribute",         SRC,      0,         0,     0,      0 }},
    { TOKEN::STARGET,                     { "target",            SRC,      0,         0,     0,      0 }},

    // linq
    { TOKEN::SLINQ,                       { "linq",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SFROM,                       { "from",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SSELECT,                     { "select",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SWHERE,                      { "where",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SLET,                        { "let",               SRC,      0,         0,     0,      0 }},
    { TOKEN::SORDERBY,                    { "orderby",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SGROUP,                      { "group",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SJOIN,                       { "join",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SIN,                         { "in",                SRC,      0,         0,     0,      0 }},
    { TOKEN::SON,                         { "on",                SRC,      0,         0,     0,      0 }},
    { TOKEN::SEQUALS,                     { "equals",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SBY,                         { "by",                SRC,      0,         0,     0,      0 }},
    { TOKEN::SINTO,                       { "into",              SRC,      0,         0,     0,      0 }},

    // special characters
    { TOKEN::CONTROL_CHAR,                { "escape",            SRC, "char",         0,     0,      0 }},

    { TOKEN::SANNOTATION,                 { "annotation",        SRC,      0,         0,     0,      0 }},

    // C++
    { TOKEN::SALIGNAS,                    { "alignas",           SRC,      0,         0,     0,      0 }},
    { TOKEN::SALIGNOF,                    { "alignof",           SRC,      0,         0,     0,      0 }},
    { TOKEN::STYPEID,                     { "typeid",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SSIZEOF_PACK,                { "sizeof",            SRC, "type",    "pack",     0,      0 }},
    { TOKEN::SENUM_CLASS,                 { "enum",              SRC, "type",   "class",     0,      0 }},
    { TOKEN::SENUM_CLASS_DECLARATION,     { "enum_decl",         SRC, "type",   "class",     0,      0 }},
    { TOKEN::SOPERATOR_FUNCTION,          { "function",          SRC, "type","operator",     0,      0 }},
    { TOKEN::SOPERATOR_FUNCTION_DECL,     { "function_decl",     SRC, "type","operator",     0,      0 }},
    { TOKEN::SREF_QUALIFIER,              { "ref_qualifier",     SRC,      0,         0,     0,      0 }},

    // Objective-C
    { TOKEN::SRECEIVER,                   { "receiver",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SMESSAGE,                    { "message",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SSELECTOR,                   { "selector",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SPROTOCOL_LIST,              { "protocol_list",       SRC,      0,         0,     0,      0 }},
    { TOKEN::SCATEGORY,                   { "category",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SPROTOCOL,                   { "protocol",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SREQUIRED_DEFAULT,           { "required",            SRC, "type", "default",     0,      0 }},
    { TOKEN::SREQUIRED,                   { "required",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SOPTIONAL,                   { "optional",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SPROPERTY,                   { "property",            SRC,      0,         0,     0,      0 }},
    { TOKEN::SATTRIBUTE_LIST,             { "attribute_list",      SRC,      0,         0,     0,      0 }},
    { TOKEN::SSYNTHESIZE,                 { "synthesize",          SRC,      0,         0,     0,      0 }},
    { TOKEN::SDYNAMIC,                    { "dynamic",             SRC,      0,         0,     0,      0 }},
    { TOKEN::SENCODE,                     { "encode",              SRC,      0,         0,     0,      0 }},
    { TOKEN::SAUTORELEASEPOOL,            { "autoreleasepool",     SRC,      0,         0,     0,      0 }},
    { TOKEN::SCOMPATIBILITY_ALIAS,        { "compatibility_alias", SRC,      0,         0,     0,      0 }},
    { TOKEN::SNIL,                        { "literal",             SRC, "type",     "nil",     0,      0 }},
    { TOKEN::SPROTOCOL_DECLARATION,       { "protocol_decl",       SRC,      0,         0,     0,      0 }},

    { TOKEN::SCLASS_INTERFACE,            { "class",       SRC, "type",      "@interface",     0,      0 }},
    { TOKEN::SCLASS_IMPLEMENTATION,       { "class",       SRC, "type", "@implementation",     0,      0 }},

    // casts
    { TOKEN::SCAST,                       { "cast",              SRC,      0,             0,   0,      0 }},
    { TOKEN::SCONST_CAST,                 { "cast",              SRC, "type",       "const",   0,      0 }},
    { TOKEN::SDYNAMIC_CAST,               { "cast",              SRC, "type",     "dynamic",   0,      0 }},
    { TOKEN::SREINTERPRET_CAST,           { "cast",              SRC, "type", "reinterpret",   0,      0 }},
    { TOKEN::SSTATIC_CAST,                { "cast",              SRC, "type",      "static",   0,      0 }},

    // srcMLOutput only
    { TOKEN::SPOSITION,                   { "position",          SRC,      0,         0,     0,      0 }},

    // Other
    { TOKEN::SCUDA_ARGUMENT_LIST,         { "argument_list",     SRC, "type",    "cuda",     0,      0 }},

    // OpenMP
    { TOKEN::SOMP_DIRECTIVE,              { "directive",         OMP,      0,         0,     0,      0 }},
    { TOKEN::SOMP_NAME,                   { "name",              OMP,      0,         0,     0,      0 }},
    { TOKEN::SOMP_CLAUSE,                 { "clause",            OMP,      0,         0,     0,      0 }},
    { TOKEN::SOMP_ARGUMENT_LIST,          { "argument_list",     OMP,      0,         0,     0,      0 }},
    { TOKEN::SOMP_ARGUMENT,               { "argument",          OMP,      0,         0,     0,      0 }},
    { TOKEN::SOMP_EXPRESSION,             { "expr",              OMP,      0,         0,     0,      0 }},

    //
    { TOKEN::SEMPTY,                      { "empty_stmt",        SRC,      0,         0,     0,      0 }},
};
