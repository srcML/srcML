// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Mode.hpp
 *
 * @copyright Copyright (C) 2004-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#ifndef MODE_HPP
#define MODE_HPP

/* Set of mode flags */

/** any srcMLstatement (broad definition includes declarations, etc.) */
static constexpr srcMLState::MODE_TYPE MODE_STATEMENT                  = srcMLState::MODE_TYPE::bit(0);

/** list used for comma and right parentheses */
static constexpr srcMLState::MODE_TYPE MODE_LIST                       = srcMLState::MODE_TYPE::bit(1);

/** a particular mode is expected in the next start */
static constexpr srcMLState::MODE_TYPE MODE_EXPECT                     = srcMLState::MODE_TYPE::bit(2);

/** srcMLstatement may be nested inside of the current */
static constexpr srcMLState::MODE_TYPE MODE_DETECT_COLON               = srcMLState::MODE_TYPE::bit(3);

/** mode for within a templare */
static constexpr srcMLState::MODE_TYPE MODE_TEMPLATE                   = srcMLState::MODE_TYPE::bit(4);

/** mode for within a template parameter list */
static constexpr srcMLState::MODE_TYPE MODE_TEMPLATE_PARAMETER_LIST    = srcMLState::MODE_TYPE::bit(59);

/** an argument to a call */
static constexpr srcMLState::MODE_TYPE MODE_ARGUMENT                   = srcMLState::MODE_TYPE::bit(5);

/** mode for a namespace */
static constexpr srcMLState::MODE_TYPE MODE_NAMESPACE                  = srcMLState::MODE_TYPE::bit(6);

/** a parameter for a declaration/definition */
static constexpr srcMLState::MODE_TYPE MODE_PARAMETER                  = srcMLState::MODE_TYPE::bit(7);

/** expressions */
static constexpr srcMLState::MODE_TYPE MODE_EXPRESSION                 = srcMLState::MODE_TYPE::bit(8);

/** expecting a call (member initialization list) */
static constexpr srcMLState::MODE_TYPE MODE_CALL                       = srcMLState::MODE_TYPE::bit(9);

/**
 * setup for expecting a condition and detection of the end
 * of a condition at a left parentheses of the correct count
 */
 static constexpr srcMLState::MODE_TYPE MODE_CONDITION                  = srcMLState::MODE_TYPE::bit(10);

/** marks top of some sequence of operations mostly to stop ending modes */
 static constexpr srcMLState::MODE_TYPE MODE_TOP                        = srcMLState::MODE_TYPE::bit(11);

/** blocks that are not necessarily srcMLstatements */
 static constexpr srcMLState::MODE_TYPE MODE_BLOCK                      = srcMLState::MODE_TYPE::bit(12);

/** blocks that are not necessarily srcMLstatements */
 static constexpr srcMLState::MODE_TYPE MODE_BLOCK_CONTENT              = srcMLState::MODE_TYPE::bit(74);

/** mode for inititialization typically @code=<init>...</init>@endcode */
 static constexpr srcMLState::MODE_TYPE MODE_INIT                       = srcMLState::MODE_TYPE::bit(13);

/**
 * block tags from being issued.  Should be moved to
 * output handling
 */
 static constexpr srcMLState::MODE_TYPE MODE_FUNCTION_TAIL              = srcMLState::MODE_TYPE::bit(14);

/**
 * whether to parse the end of line character
 * used with preprocessor directives
 */
 static constexpr srcMLState::MODE_TYPE MODE_PARSE_EOL                  = srcMLState::MODE_TYPE::bit(15);

/** local mode only used within a grammar rule */
 static constexpr srcMLState::MODE_TYPE MODE_LOCAL                      = srcMLState::MODE_TYPE::bit(16);

/** Mode for a variable name */
 static constexpr srcMLState::MODE_TYPE MODE_VARIABLE_NAME              = srcMLState::MODE_TYPE::bit(17);

/**
 * the if srcMLstatement includes some special processing
 * including starting a THEN element after the condition
 * and stopping the ending of srcMLstatements at the IF when
 * an ELSE is matched
 */
 static constexpr srcMLState::MODE_TYPE MODE_IF                         = srcMLState::MODE_TYPE::bit(18);

/**
 * for special sections inside of mode such as in
 *  classes and switch srcMLstatement blocks
 */
 static constexpr srcMLState::MODE_TYPE MODE_TOP_SECTION                = srcMLState::MODE_TYPE::bit(19);

/** in a for heading group i.e. for init/condition/increment & if init/condition */
 static constexpr srcMLState::MODE_TYPE MODE_CONTROL                    = srcMLState::MODE_TYPE::bit(20);

/** for initialization (in header) */
 static constexpr srcMLState::MODE_TYPE MODE_CONTROL_INITIALIZATION     = srcMLState::MODE_TYPE::bit(21);

/** for condition (in header) */
 static constexpr srcMLState::MODE_TYPE MODE_CONTROL_CONDITION          = srcMLState::MODE_TYPE::bit(22);

/** for increment (in header) */
 static constexpr srcMLState::MODE_TYPE MODE_CONTROL_INCREMENT          = srcMLState::MODE_TYPE::bit(23);

/** preprocessor mode */
 static constexpr srcMLState::MODE_TYPE MODE_PREPROC                    = srcMLState::MODE_TYPE::bit(24);

/** mode for nesting srcMLstatements */
 static constexpr srcMLState::MODE_TYPE MODE_NEST                       = srcMLState::MODE_TYPE::bit(25);

/** mode fore expression block */
 static constexpr srcMLState::MODE_TYPE MODE_EXPRESSION_BLOCK           = srcMLState::MODE_TYPE::bit(26);

/** mode marking to end at right parenthesis */
 static constexpr srcMLState::MODE_TYPE MODE_INTERNAL_END_PAREN         = srcMLState::MODE_TYPE::bit(27);

/** access regions in classes used for matching of */
 static constexpr srcMLState::MODE_TYPE MODE_ACCESS_REGION              = srcMLState::MODE_TYPE::bit(28);

/** mode for a do while srcMLstatement */
 static constexpr srcMLState::MODE_TYPE MODE_DO_STATEMENT               = srcMLState::MODE_TYPE::bit(29);

/** mode to ignore ; */
 static constexpr srcMLState::MODE_TYPE MODE_IGNORE_TERMINATE           = srcMLState::MODE_TYPE::bit(30);

/** mode for extern */
 static constexpr srcMLState::MODE_TYPE MODE_EXTERN                     = srcMLState::MODE_TYPE::bit(32);

/** mode to end at right curly */
 static constexpr srcMLState::MODE_TYPE MODE_INTERNAL_END_CURLY         = srcMLState::MODE_TYPE::bit(34);

/** mode for a class */
 static constexpr srcMLState::MODE_TYPE MODE_CLASS                      = srcMLState::MODE_TYPE::bit(37);

/** mode to end at block */
 static constexpr srcMLState::MODE_TYPE MODE_END_AT_BLOCK               = srcMLState::MODE_TYPE::bit(39);

/** mode to only end at right parentesis */
 static constexpr srcMLState::MODE_TYPE MODE_END_ONLY_AT_RPAREN         = srcMLState::MODE_TYPE::bit(40);

/** mode to end at a block and not expect ; after */
 static constexpr srcMLState::MODE_TYPE MODE_END_AT_BLOCK_NO_TERMINATE  = srcMLState::MODE_TYPE::bit(41);

/** mode for a function name */
 static constexpr srcMLState::MODE_TYPE MODE_FUNCTION_NAME              = srcMLState::MODE_TYPE::bit(44);

/** mode for a if then */
 static constexpr srcMLState::MODE_TYPE MODE_THEN                       = srcMLState::MODE_TYPE::bit(55);

/** mode for an else */
 static constexpr srcMLState::MODE_TYPE MODE_ELSE                       = srcMLState::MODE_TYPE::bit(45);

/** mode for a typdef */
 static constexpr srcMLState::MODE_TYPE MODE_TYPEDEF                    = srcMLState::MODE_TYPE::bit(46);

/** mode for a declaration of some type */
 static constexpr srcMLState::MODE_TYPE MODE_DECL                       = srcMLState::MODE_TYPE::bit(47);

/** mode to consume the type names */
 static constexpr srcMLState::MODE_TYPE MODE_EAT_TYPE                   = srcMLState::MODE_TYPE::bit(48);

/** mode for funciton parameter */
 static constexpr srcMLState::MODE_TYPE MODE_FUNCTION_PARAMETER         = srcMLState::MODE_TYPE::bit(49);

/** mode for an internal decl */
 static constexpr srcMLState::MODE_TYPE MODE_INNER_DECL                 = srcMLState::MODE_TYPE::bit(52);

/** mode to mark in an init */
 static constexpr srcMLState::MODE_TYPE MODE_IN_INIT                    = srcMLState::MODE_TYPE::bit(56);

/** mode for a try */
 static constexpr srcMLState::MODE_TYPE MODE_TRY                        = srcMLState::MODE_TYPE::bit(57);

/** mode to end a list at a block */
 static constexpr srcMLState::MODE_TYPE MODE_END_LIST_AT_BLOCK          = srcMLState::MODE_TYPE::bit(58);

/** mode to end at ; */
 static constexpr srcMLState::MODE_TYPE MODE_ONLY_END_TERMINATE         = srcMLState::MODE_TYPE::bit(60);

/** mode for enum */
 static constexpr srcMLState::MODE_TYPE MODE_ENUM                       = srcMLState::MODE_TYPE::bit(61);

/** mode for anonymous item e.g. anonymous class */
 static constexpr srcMLState::MODE_TYPE MODE_ANONYMOUS                  = srcMLState::MODE_TYPE::bit(62);

/** mode to end at a comma */
 static constexpr srcMLState::MODE_TYPE MODE_END_AT_COMMA               = srcMLState::MODE_TYPE::bit(63);

/** mode for in a using */
 static constexpr srcMLState::MODE_TYPE MODE_USING                      = srcMLState::MODE_TYPE::bit(31);

/** mode for function trailing return */
 static constexpr srcMLState::MODE_TYPE MODE_TRAILING_RETURN            = srcMLState::MODE_TYPE::bit(33);

/** mode to issue an empty element at pop */
 static constexpr srcMLState::MODE_TYPE MODE_ISSUE_EMPTY_AT_POP         = srcMLState::MODE_TYPE::bit(35);

/** mode to end at preprocessor endif */
 static constexpr srcMLState::MODE_TYPE MODE_END_AT_ENDIF               = srcMLState::MODE_TYPE::bit(38);

/** mode for an argument list */
 static constexpr srcMLState::MODE_TYPE MODE_ARGUMENT_LIST              = srcMLState::MODE_TYPE::bit(42);

/** mode for an _Generic association list */
static constexpr srcMLState::MODE_TYPE MODE_ASSOCIATION_LIST           = srcMLState::MODE_TYPE::bit(43);

/** mode for an tenary */
static constexpr srcMLState::MODE_TYPE MODE_TERNARY                    = srcMLState::MODE_TYPE::bit(50);

/** mode for Objectie-C call */
static constexpr srcMLState::MODE_TYPE MODE_OBJECTIVE_C_CALL           = srcMLState::MODE_TYPE::bit(51);

/** mode for switch statement */
static constexpr srcMLState::MODE_TYPE MODE_SWITCH                     = srcMLState::MODE_TYPE::bit(53);

/** mode for ternary condition */
static constexpr srcMLState::MODE_TYPE MODE_TERNARY_CONDITION          = srcMLState::MODE_TYPE::bit(54);

/** mode for member initialization list */
static constexpr srcMLState::MODE_TYPE MODE_INITIALIZATION_LIST        = srcMLState::MODE_TYPE::bit(36);

/** mode for C++ ranged for */
static constexpr srcMLState::MODE_TYPE MODE_RANGED_FOR                 = srcMLState::MODE_TYPE::bit(64);

/** mode for associationt type */
static constexpr srcMLState::MODE_TYPE MODE_ASSOCIATION_TYPE           = srcMLState::MODE_TYPE::bit(65);

/** mode for friend */
static constexpr srcMLState::MODE_TYPE MODE_FRIEND                     = srcMLState::MODE_TYPE::bit(66);

/** mode for class header */
static constexpr srcMLState::MODE_TYPE MODE_CLASS_NAME                 = srcMLState::MODE_TYPE::bit(67);

/** mode for function body */
static constexpr srcMLState::MODE_TYPE MODE_FUNCTION_BODY              = srcMLState::MODE_TYPE::bit(68);

/** mode for function type */
static constexpr srcMLState::MODE_TYPE MODE_FUNCTION_TYPE              = srcMLState::MODE_TYPE::bit(69);

/** mode to mark end of for control for cppif duplication */
static constexpr srcMLState::MODE_TYPE MODE_END_CONTROL                = srcMLState::MODE_TYPE::bit(70);

/** mode for for-like statement */
static constexpr srcMLState::MODE_TYPE MODE_FOR_LIKE_LIST              = srcMLState::MODE_TYPE::bit(71);

/** mode for in function call */
static constexpr srcMLState::MODE_TYPE MODE_FUNCTION_CALL              = srcMLState::MODE_TYPE::bit(72);

/** mode for in function call */
static constexpr srcMLState::MODE_TYPE MODE_IF_STATEMENT               = srcMLState::MODE_TYPE::bit(73);

/** mode to exclude the block content tag */
static constexpr srcMLState::MODE_TYPE MODE_NO_BLOCK_CONTENT           = srcMLState::MODE_TYPE::bit(75);

/** mode for a property */
static constexpr srcMLState::MODE_TYPE MODE_PROPERTY                   = srcMLState::MODE_TYPE::bit(76);

/** mode for c-attribute processing */
static constexpr srcMLState::MODE_TYPE MODE_INCLUDE_ATTRIBUTE          = srcMLState::MODE_TYPE::bit(77);

// Python modes
static constexpr srcMLState::MODE_TYPE MODE_EXCLUDE_NO_PAREN_TUPLES_PY = srcMLState::MODE_TYPE::bit(78);

static constexpr srcMLState::MODE_TYPE MODE_COMPREHENSION_IF_PY        = srcMLState::MODE_TYPE::bit(79);

static constexpr srcMLState::MODE_TYPE MODE_TERNARY_CONTENT_PY         = srcMLState::MODE_TYPE::bit(80);

static constexpr srcMLState::MODE_TYPE MODE_WITH_EXPRESSION_PY         = srcMLState::MODE_TYPE::bit(81);

static constexpr srcMLState::MODE_TYPE MODE_LAMBDA_CONTENT_PY          = srcMLState::MODE_TYPE::bit(82);

static constexpr srcMLState::MODE_TYPE MODE_OPERATOR_PAREN_PY          = srcMLState::MODE_TYPE::bit(83);

static constexpr srcMLState::MODE_TYPE MODE_PARAMETER_LIST_PY          = srcMLState::MODE_TYPE::bit(84);

static constexpr srcMLState::MODE_TYPE MODE_TUPLE_NO_PAREN_PY          = srcMLState::MODE_TYPE::bit(85);

static constexpr srcMLState::MODE_TYPE MODE_COMPREHENSION_PY           = srcMLState::MODE_TYPE::bit(86);

static constexpr srcMLState::MODE_TYPE MODE_EXCEPT_ALIAS_PY            = srcMLState::MODE_TYPE::bit(87);

static constexpr srcMLState::MODE_TYPE MODE_FOR_CONTROL_PY             = srcMLState::MODE_TYPE::bit(88);

static constexpr srcMLState::MODE_TYPE MODE_ANNOTATION_PY              = srcMLState::MODE_TYPE::bit(89);

static constexpr srcMLState::MODE_TYPE MODE_DICTIONARY_PY              = srcMLState::MODE_TYPE::bit(90);

static constexpr srcMLState::MODE_TYPE MODE_SUPER_LIST_PY              = srcMLState::MODE_TYPE::bit(91);

static constexpr srcMLState::MODE_TYPE MODE_WHILE_LOOP_PY              = srcMLState::MODE_TYPE::bit(92);

static constexpr srcMLState::MODE_TYPE MODE_DECORATOR_PY               = srcMLState::MODE_TYPE::bit(93);

static constexpr srcMLState::MODE_TYPE MODE_SPECIFIER_PY               = srcMLState::MODE_TYPE::bit(94);

static constexpr srcMLState::MODE_TYPE MODE_FOR_LOOP_PY                = srcMLState::MODE_TYPE::bit(95);

static constexpr srcMLState::MODE_TYPE MODE_RANGE_IN_PY                = srcMLState::MODE_TYPE::bit(96);

static constexpr srcMLState::MODE_TYPE MODE_ASSERT_PY                  = srcMLState::MODE_TYPE::bit(97);

static constexpr srcMLState::MODE_TYPE MODE_EXCEPT_PY                  = srcMLState::MODE_TYPE::bit(98);

static constexpr srcMLState::MODE_TYPE MODE_LAMBDA_PY                  = srcMLState::MODE_TYPE::bit(99);

static constexpr srcMLState::MODE_TYPE MODE_ALIAS_PY                   = srcMLState::MODE_TYPE::bit(100);

static constexpr srcMLState::MODE_TYPE MODE_ARRAY_PY                   = srcMLState::MODE_TYPE::bit(101);

static constexpr srcMLState::MODE_TYPE MODE_INDEX_PY                   = srcMLState::MODE_TYPE::bit(102);

static constexpr srcMLState::MODE_TYPE MODE_RAISE_PY                   = srcMLState::MODE_TYPE::bit(103);

static constexpr srcMLState::MODE_TYPE MODE_SUPER_PY                   = srcMLState::MODE_TYPE::bit(104);

static constexpr srcMLState::MODE_TYPE MODE_TUPLE_PY                   = srcMLState::MODE_TYPE::bit(105);

static constexpr srcMLState::MODE_TYPE MODE_YIELD_PY                   = srcMLState::MODE_TYPE::bit(106);

static constexpr srcMLState::MODE_TYPE MODE_CASE_PY                    = srcMLState::MODE_TYPE::bit(107);

static constexpr srcMLState::MODE_TYPE MODE_FROM_PY                    = srcMLState::MODE_TYPE::bit(108);

static constexpr srcMLState::MODE_TYPE MODE_SET_PY                     = srcMLState::MODE_TYPE::bit(109);

// JavaScript modes
static constexpr srcMLState::MODE_TYPE MODE_FUNCTION_EXPRESSION_JS     = srcMLState::MODE_TYPE::bit(110);

static constexpr srcMLState::MODE_TYPE MODE_GLOBAL_CONTEXT_CALL_JS     = srcMLState::MODE_TYPE::bit(111);

static constexpr srcMLState::MODE_TYPE MODE_OBJECT_DESTRUCTURE_JS      = srcMLState::MODE_TYPE::bit(112);

static constexpr srcMLState::MODE_TYPE MODE_ARRAY_DESTRUCTURE_JS       = srcMLState::MODE_TYPE::bit(113);

static constexpr srcMLState::MODE_TYPE MODE_COMPUTED_FUNCTION_JS       = srcMLState::MODE_TYPE::bit(114);

static constexpr srcMLState::MODE_TYPE MODE_CLASS_EXPRESSION_JS        = srcMLState::MODE_TYPE::bit(115);

static constexpr srcMLState::MODE_TYPE MODE_DECL_DESTRUCTURE_JS        = srcMLState::MODE_TYPE::bit(116);

static constexpr srcMLState::MODE_TYPE MODE_DECL_STATEMENT_JS          = srcMLState::MODE_TYPE::bit(117);

static constexpr srcMLState::MODE_TYPE MODE_PARAMETER_LIST_JS          = srcMLState::MODE_TYPE::bit(118);

static constexpr srcMLState::MODE_TYPE MODE_IGNORE_LABEL_JS            = srcMLState::MODE_TYPE::bit(119);

static constexpr srcMLState::MODE_TYPE MODE_LCURLY_BLOCK_JS            = srcMLState::MODE_TYPE::bit(120);

static constexpr srcMLState::MODE_TYPE MODE_CONSTRUCTOR_JS             = srcMLState::MODE_TYPE::bit(121);

static constexpr srcMLState::MODE_TYPE MODE_FOR_CONTROL_JS             = srcMLState::MODE_TYPE::bit(122);

static constexpr srcMLState::MODE_TYPE MODE_IIFE_INNER_JS              = srcMLState::MODE_TYPE::bit(123);

static constexpr srcMLState::MODE_TYPE MODE_INNER_NAME_JS              = srcMLState::MODE_TYPE::bit(124);

static constexpr srcMLState::MODE_TYPE MODE_OUTER_NAME_JS              = srcMLState::MODE_TYPE::bit(125);

static constexpr srcMLState::MODE_TYPE MODE_SUPER_LIST_JS              = srcMLState::MODE_TYPE::bit(126);

static constexpr srcMLState::MODE_TYPE MODE_DECL_INIT_JS               = srcMLState::MODE_TYPE::bit(127);

static constexpr srcMLState::MODE_TYPE MODE_IIFE_CALL_JS               = srcMLState::MODE_TYPE::bit(128);

static constexpr srcMLState::MODE_TYPE MODE_NAME_LIST_JS               = srcMLState::MODE_TYPE::bit(129);

static constexpr srcMLState::MODE_TYPE MODE_PROPERTY_JS                = srcMLState::MODE_TYPE::bit(130);

static constexpr srcMLState::MODE_TYPE MODE_EXTENDS_JS                 = srcMLState::MODE_TYPE::bit(131);

static constexpr srcMLState::MODE_TYPE MODE_EXPORT_JS                  = srcMLState::MODE_TYPE::bit(132);

static constexpr srcMLState::MODE_TYPE MODE_IMPORT_JS                  = srcMLState::MODE_TYPE::bit(133);

static constexpr srcMLState::MODE_TYPE MODE_LAMBDA_JS                  = srcMLState::MODE_TYPE::bit(134);

static constexpr srcMLState::MODE_TYPE MODE_OBJECT_JS                  = srcMLState::MODE_TYPE::bit(135);

static constexpr srcMLState::MODE_TYPE MODE_RETURN_JS                  = srcMLState::MODE_TYPE::bit(136);

static constexpr srcMLState::MODE_TYPE MODE_ARRAY_JS                   = srcMLState::MODE_TYPE::bit(137);

static constexpr srcMLState::MODE_TYPE MODE_DECL_JS                    = srcMLState::MODE_TYPE::bit(138);

static constexpr srcMLState::MODE_TYPE MODE_WITH_JS                    = srcMLState::MODE_TYPE::bit(139);

// TypeScript modes (appears as a namespace prefix)
static constexpr srcMLState::MODE_TYPE MODE_TEMPLATE_ARGUMENT_TS       = srcMLState::MODE_TYPE::bit(140);

static constexpr srcMLState::MODE_TYPE MODE_EXPRESSION_COLON_TS        = srcMLState::MODE_TYPE::bit(141);

static constexpr srcMLState::MODE_TYPE MODE_DECL_STATEMENT_TS          = srcMLState::MODE_TYPE::bit(142);

static constexpr srcMLState::MODE_TYPE MODE_GENERIC_LAMBDA_TS          = srcMLState::MODE_TYPE::bit(143);

static constexpr srcMLState::MODE_TYPE MODE_LAMBDA_AS_TYPE_TS          = srcMLState::MODE_TYPE::bit(144);

static constexpr srcMLState::MODE_TYPE MODE_FUNCTION_DECL_TS           = srcMLState::MODE_TYPE::bit(145);

static constexpr srcMLState::MODE_TYPE MODE_CONSTRAINT_TS              = srcMLState::MODE_TYPE::bit(146);

static constexpr srcMLState::MODE_TYPE MODE_TYPE_ARRAY_TS              = srcMLState::MODE_TYPE::bit(147);

static constexpr srcMLState::MODE_TYPE MODE_DECORATOR_TS               = srcMLState::MODE_TYPE::bit(148);

static constexpr srcMLState::MODE_TYPE MODE_INTERFACE_TS               = srcMLState::MODE_TYPE::bit(149);

static constexpr srcMLState::MODE_TYPE MODE_NAMESPACE_TS               = srcMLState::MODE_TYPE::bit(150);

static constexpr srcMLState::MODE_TYPE MODE_DECLARE_TS                 = srcMLState::MODE_TYPE::bit(151);

static constexpr srcMLState::MODE_TYPE MODE_MIXINS_TS                  = srcMLState::MODE_TYPE::bit(152);

static constexpr srcMLState::MODE_TYPE MODE_INDEX_TS                   = srcMLState::MODE_TYPE::bit(153);

static constexpr srcMLState::MODE_TYPE MODE_TYPE_TS                    = srcMLState::MODE_TYPE::bit(154);


// CMake modes
static constexpr srcMLState::MODE_TYPE MODE_PAREN_ENDS_STATEMENT_CMAKE = srcMLState::MODE_TYPE::bit(155);

static constexpr srcMLState::MODE_TYPE MODE_BLOCK_STATEMENT_CMAKE      = srcMLState::MODE_TYPE::bit(156);

static constexpr srcMLState::MODE_TYPE MODE_WHILE_LOOP_CMAKE           = srcMLState::MODE_TYPE::bit(157);

static constexpr srcMLState::MODE_TYPE MODE_PROPAGATE_CMAKE            = srcMLState::MODE_TYPE::bit(158);

static constexpr srcMLState::MODE_TYPE MODE_PROPAGATE_STATEMENT_CMAKE  = srcMLState::MODE_TYPE::bit(159);

static constexpr srcMLState::MODE_TYPE MODE_ENDTOKEN_CMAKE             = srcMLState::MODE_TYPE::bit(160);

static constexpr srcMLState::MODE_TYPE MODE_FUNCTION_CMAKE             = srcMLState::MODE_TYPE::bit(161);

static constexpr srcMLState::MODE_TYPE MODE_COMMAND_CMAKE              = srcMLState::MODE_TYPE::bit(162);

static constexpr srcMLState::MODE_TYPE MODE_FOREACH_CMAKE              = srcMLState::MODE_TYPE::bit(163);

static constexpr srcMLState::MODE_TYPE MODE_INCLUDE_CMAKE              = srcMLState::MODE_TYPE::bit(164);

static constexpr srcMLState::MODE_TYPE MODE_OPTION_CMAKE               = srcMLState::MODE_TYPE::bit(165);

static constexpr srcMLState::MODE_TYPE MODE_MACRO_CMAKE                = srcMLState::MODE_TYPE::bit(166);

static constexpr srcMLState::MODE_TYPE MODE_EXPRESSION_NAME_CMAKE      = srcMLState::MODE_TYPE::bit(167);

static constexpr srcMLState::MODE_TYPE MODE_EXPRESSION_ENV_CMAKE       = srcMLState::MODE_TYPE::bit(168);

static constexpr srcMLState::MODE_TYPE MODE_EXPRESSION_CACHE_CMAKE     = srcMLState::MODE_TYPE::bit(169);

static constexpr srcMLState::MODE_TYPE MODE_EXPRESSION_GENERATOR_CMAKE = srcMLState::MODE_TYPE::bit(170);

#endif
