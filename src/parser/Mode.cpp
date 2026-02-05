// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Mode.cpp
 *
 * @copyright Copyright (C) 2014-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#include <ModeStack.hpp>

// Constant MODE_TYPE with bit set
static srcMLState::MODE_TYPE bit(size_t pos) {
    return srcMLState::MODE_TYPE{}.set(pos);
}

/* Set of mode flags */
const srcMLState::MODE_TYPE ModeStack::MODE_STATEMENT                 (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_LIST                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_EXPECT                    (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_DETECT_COLON              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TEMPLATE                  (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ARGUMENT                  (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_NAMESPACE                 (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_PARAMETER                 (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_EXPRESSION                (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CALL                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CONDITION                 (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TOP                       (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_BLOCK                     (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_INIT                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_TAIL             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_PARSE_EOL                 (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_LOCAL                     (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_VARIABLE_NAME             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_IF                        (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TOP_SECTION               (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL_INITIALIZATION    (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL_CONDITION         (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL_INCREMENT         (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_PREPROC                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_NEST                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_EXPRESSION_BLOCK          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_INTERNAL_END_PAREN        (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ACCESS_REGION             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_DO_STATEMENT              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_IGNORE_TERMINATE          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_USING                     (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_EXTERN                    (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TRAILING_RETURN           (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_INTERNAL_END_CURLY        (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ISSUE_EMPTY_AT_POP        (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_INITIALIZATION_LIST       (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CLASS                     (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_ENDIF              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_BLOCK              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_END_ONLY_AT_RPAREN        (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_BLOCK_NO_TERMINATE (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ARGUMENT_LIST             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ASSOCIATION_LIST          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_NAME             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ELSE                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TYPEDEF                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_DECL                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_EAT_TYPE                  (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_PARAMETER        (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TERNARY                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_OBJECTIVE_C_CALL          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_INNER_DECL                (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_SWITCH                    (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TERNARY_CONDITION         (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_THEN                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_IN_INIT                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TRY                       (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_END_LIST_AT_BLOCK         (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TEMPLATE_PARAMETER_LIST   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ONLY_END_TERMINATE        (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ENUM                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ANONYMOUS                 (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_COMMA              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_RANGED_FOR                (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ASSOCIATION_TYPE          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FRIEND                    (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CLASS_NAME                (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_BODY             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_TYPE             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_END_CONTROL               (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FOR_LIKE_LIST             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_CALL             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_IF_STATEMENT              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_BLOCK_CONTENT             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_NO_BLOCK_CONTENT          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_INCLUDE_ATTRIBUTE         (bit(__COUNTER__));

// Python modes
const srcMLState::MODE_TYPE ModeStack::MODE_EXCLUDE_NO_PAREN_TUPLES_PY (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_COMPREHENSION_IF_PY        (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TERNARY_CONTENT_PY         (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_WITH_EXPRESSION_PY         (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_LAMBDA_CONTENT_PY          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_OPERATOR_PAREN_PY          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_PARAMETER_LIST_PY          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TUPLE_NO_PAREN_PY          (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_COMPREHENSION_PY           (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_EXCEPT_ALIAS_PY            (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FOR_CONTROL_PY             (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ANNOTATION_PY              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_DICTIONARY_PY              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_SUPER_LIST_PY              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_WHILE_LOOP_PY              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_DECORATOR_PY               (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_SPECIFIER_PY               (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FOR_LOOP_PY                (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_RANGE_IN_PY                (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ASSERT_PY                  (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_EXCEPT_PY                  (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_LAMBDA_PY                  (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ALIAS_PY                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ARRAY_PY                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_INDEX_PY                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_RAISE_PY                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_SUPER_PY                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_TUPLE_PY                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_YIELD_PY                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CASE_PY                    (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FROM_PY                    (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_SET_PY                     (bit(__COUNTER__));

// JavaScript
const srcMLState::MODE_TYPE ModeStack::MODE_COMPUTED_GENERATOR_FUNCTION_JS (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_EXPRESSION_JS         (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CLASS_EXPRESSION_JS            (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_DECL_STATEMENT_JS              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_PARAMETER_LIST_JS              (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_CONSTRUCTOR_JS                 (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_FOR_CONTROL_JS                 (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_SUPER_LIST_JS                  (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_IIFE_CALL_JS                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_NAME_LIST_JS                   (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_PROPERTY_JS                    (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_EXTENDS_JS                     (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_EXPORT_JS                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_IMPORT_JS                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_LAMBDA_JS                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_OBJECT_JS                      (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_ARRAY_JS                       (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_DECL_JS                        (bit(__COUNTER__));
const srcMLState::MODE_TYPE ModeStack::MODE_WITH_JS                        (bit(__COUNTER__));

// Rust
const srcMLState::MODE_TYPE ModeStack::MODE_DECL_STMT_RS (bit(__COUNTER__));
