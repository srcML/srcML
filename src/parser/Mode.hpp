/**
 * @file Mode.hpp
 *
 * @copyright Copyright (C) 2004-2014 srcML, LLC. (www.srcML.org)
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

#ifndef MODE_HPP
#define MODE_HPP

/* Set of mode flags */

/** any srcMLstatement (broad definition includes declarations, etc.) */
const static srcMLState::MODE_TYPE MODE_STATEMENT;

/** list used for comma and right parentheses */
const static srcMLState::MODE_TYPE MODE_LIST;

/** a particular mode is expected in the next start */
const static srcMLState::MODE_TYPE MODE_EXPECT;

/** srcMLstatement may be nested inside of the current */
const static srcMLState::MODE_TYPE MODE_DETECT_COLON;

/** mode for within a templare */
const static srcMLState::MODE_TYPE MODE_TEMPLATE;

/** mode for within a template parameter list */
const static srcMLState::MODE_TYPE MODE_TEMPLATE_PARAMETER_LIST;

/** an argument to a call */
const static srcMLState::MODE_TYPE MODE_ARGUMENT;

/** mode for a namespace */
const static srcMLState::MODE_TYPE MODE_NAMESPACE;

/** a parameter for a declaration/definition */
const static srcMLState::MODE_TYPE MODE_PARAMETER;

/** expressions */
const static srcMLState::MODE_TYPE MODE_EXPRESSION;

/** expecting a call (member initialization list) */
const static srcMLState::MODE_TYPE MODE_CALL;

/** 
 * setup for expecting a condition and detection of the end
 * of a condition at a left parentheses of the correct count 
 */
 const static srcMLState::MODE_TYPE MODE_CONDITION;

/** marks top of some sequence of operations mostly to stop ending modes */
 const static srcMLState::MODE_TYPE MODE_TOP;

/** blocks that are not necessarily srcMLstatements */
 const static srcMLState::MODE_TYPE MODE_BLOCK;

/** mode for inititialization typically @code=<init>...</init>@endcode */
 const static srcMLState::MODE_TYPE MODE_INIT;

/** 
 * block tags from being issued.  Should be moved to
 * output handling 
 */
 const static srcMLState::MODE_TYPE MODE_FUNCTION_TAIL;

/** 
 * whether to parse the end of line character
 * used with preprocessor directives 
 */
 const static srcMLState::MODE_TYPE MODE_PARSE_EOL;

/** local mode only used within a grammar rule */
 const static srcMLState::MODE_TYPE MODE_LOCAL;

/** Mode for a variable name */
 const static srcMLState::MODE_TYPE MODE_VARIABLE_NAME;

/** 
 * the if srcMLstatement includes some special processing
 * including starting a THEN element after the condition
 * and stopping the ending of srcMLstatements at the IF when
 * an ELSE is matched 
 */
 const static srcMLState::MODE_TYPE MODE_IF;

/**
 * for special sections inside of mode such as in
 *  classes and switch srcMLstatement blocks
 */
 const static srcMLState::MODE_TYPE MODE_TOP_SECTION;

/** in a for heading group i.e. for init/condition/increment */
 const static srcMLState::MODE_TYPE MODE_FOR_CONTROL;

/** for initialization (in header) */
 const static srcMLState::MODE_TYPE MODE_FOR_INITIALIZATION;

/** for condition (in header) */
 const static srcMLState::MODE_TYPE MODE_FOR_CONDITION;

/** for increment (in header) */
 const static srcMLState::MODE_TYPE MODE_FOR_INCREMENT;

/** preprocessor mode */
 const static srcMLState::MODE_TYPE MODE_PREPROC;

/** mode for nesting srcMLstatements */
 const static srcMLState::MODE_TYPE MODE_NEST;

/** mode fore expression block */
 const static srcMLState::MODE_TYPE MODE_EXPRESSION_BLOCK;

/** mode marking to end at right parenthesis @todo remove */
 const static srcMLState::MODE_TYPE MODE_INTERNAL_END_PAREN;

/** access regions in classes used for matching of */
 const static srcMLState::MODE_TYPE MODE_ACCESS_REGION;

/** mode for a do while srcMLstatement */
 const static srcMLState::MODE_TYPE MODE_DO_STATEMENT;

/** mode to ignore ; */
 const static srcMLState::MODE_TYPE MODE_IGNORE_TERMINATE;

/** mode for extern */
 const static srcMLState::MODE_TYPE MODE_EXTERN;

/** mode to end at right curly */
 const static srcMLState::MODE_TYPE MODE_INTERNAL_END_CURLY;

/** mode for a class */
 const static srcMLState::MODE_TYPE MODE_CLASS;

/** mode to end at block */
 const static srcMLState::MODE_TYPE MODE_END_AT_BLOCK;

/** mode to only end at right parentesis */
 const static srcMLState::MODE_TYPE MODE_END_ONLY_AT_RPAREN;

/** mode to end at a block and not expect ; after */
 const static srcMLState::MODE_TYPE MODE_END_AT_BLOCK_NO_TERMINATE;

/** mode for a function name */
 const static srcMLState::MODE_TYPE MODE_FUNCTION_NAME;

/** mode for a if then */
 const static srcMLState::MODE_TYPE MODE_THEN;

/** mode for an else */    
 const static srcMLState::MODE_TYPE MODE_ELSE;

/** mode for a typdef */
 const static srcMLState::MODE_TYPE MODE_TYPEDEF;

/** mode for a declaration of some type */
 const static srcMLState::MODE_TYPE MODE_DECL;

/** mode to consume the type names */
 const static srcMLState::MODE_TYPE MODE_EAT_TYPE;

/** mode for funciton parameter */
 const static srcMLState::MODE_TYPE MODE_FUNCTION_PARAMETER;

/** mode for an internal decl */
 const static srcMLState::MODE_TYPE MODE_INNER_DECL;

/** mode to mark in an init */
 const static srcMLState::MODE_TYPE MODE_IN_INIT;

/** mode for a try */
 const static srcMLState::MODE_TYPE MODE_TRY;

/** mode to end a list at a block */
 const static srcMLState::MODE_TYPE MODE_END_LIST_AT_BLOCK;

/** mode to end at ; */
 const static srcMLState::MODE_TYPE MODE_ONLY_END_TERMINATE;

/** mode for enum */
 const static srcMLState::MODE_TYPE MODE_ENUM;

/** mode for anonymous item e.g. anonymous class */
 const static srcMLState::MODE_TYPE MODE_ANONYMOUS;

/** mode to end at a comma */
 const static srcMLState::MODE_TYPE MODE_END_AT_COMMA;

/** mode for in a using */
 const static srcMLState::MODE_TYPE MODE_USING;

/** mode for function trailing return */
 const static srcMLState::MODE_TYPE MODE_TRAILING_RETURN;

/** mode to issue an empty element at pop */
 const static srcMLState::MODE_TYPE MODE_ISSUE_EMPTY_AT_POP;

/** mode to end at preprocessor endif */
 const static srcMLState::MODE_TYPE MODE_END_AT_ENDIF;

/** mode for an argument list */
 const static srcMLState::MODE_TYPE MODE_ARGUMENT_LIST;

/** mode for an _Generic association list */
const static srcMLState::MODE_TYPE MODE_ASSOCIATION_LIST;

/** mode for an tenary */
const static srcMLState::MODE_TYPE MODE_TERNARY;

/** mode for Objectie-C call */
const static srcMLState::MODE_TYPE MODE_OBJECTIVE_C_CALL;

/** mode for switch statement */
const static srcMLState::MODE_TYPE MODE_SWITCH;

/** mode for ternary condition */
const static srcMLState::MODE_TYPE MODE_TERNARY_CONDITION;

/** mode for member initialization list */
const static srcMLState::MODE_TYPE MODE_INITIALIZATION_LIST;

/** mode for C++ ranged for */
const static srcMLState::MODE_TYPE MODE_RANGED_FOR;

/** mode for associationt type */
const static srcMLState::MODE_TYPE MODE_ASSOCIATION_TYPE;

/** mode for friend */
const static srcMLState::MODE_TYPE MODE_FRIEND;

/** mode for class header */
const static srcMLState::MODE_TYPE MODE_CLASS_NAME;

/** mode for function body */
const static srcMLState::MODE_TYPE MODE_FUNCTION_BODY;

/** mode for function type */
const static srcMLState::MODE_TYPE MODE_FUNCTION_TYPE;

/** mode to mark end of for control for cppif duplication */
const static srcMLState::MODE_TYPE MODE_END_FOR_CONTROL;

#endif
