// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Mode.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#include <ModeStack.hpp>

/* Set of mode flags */
const srcMLState::MODE_TYPE ModeStack::MODE_STATEMENT                 (1ull << 0);
const srcMLState::MODE_TYPE ModeStack::MODE_LIST                      (1ull << 1);
const srcMLState::MODE_TYPE ModeStack::MODE_EXPECT                    (1ull << 2);
const srcMLState::MODE_TYPE ModeStack::MODE_DETECT_COLON              (1ull << 3);
const srcMLState::MODE_TYPE ModeStack::MODE_TEMPLATE                  (1ull << 4);
const srcMLState::MODE_TYPE ModeStack::MODE_ARGUMENT                  (1ull << 5);
const srcMLState::MODE_TYPE ModeStack::MODE_NAMESPACE                 (1ull << 6);
const srcMLState::MODE_TYPE ModeStack::MODE_PARAMETER                 (1ull << 7);
const srcMLState::MODE_TYPE ModeStack::MODE_EXPRESSION                (1ull << 8);
const srcMLState::MODE_TYPE ModeStack::MODE_CALL                      (1ull << 9);
const srcMLState::MODE_TYPE ModeStack::MODE_CONDITION                 (1ull << 10);
const srcMLState::MODE_TYPE ModeStack::MODE_TOP                       (1ull << 11);
const srcMLState::MODE_TYPE ModeStack::MODE_BLOCK                     (1ull << 12);
const srcMLState::MODE_TYPE ModeStack::MODE_INIT                      (1ull << 13);
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_TAIL             (1ull << 14);
const srcMLState::MODE_TYPE ModeStack::MODE_PARSE_EOL                 (1ull << 15);
const srcMLState::MODE_TYPE ModeStack::MODE_LOCAL                     (1ull << 16);
const srcMLState::MODE_TYPE ModeStack::MODE_VARIABLE_NAME             (1ull << 17);
const srcMLState::MODE_TYPE ModeStack::MODE_IF                        (1ull << 18);
const srcMLState::MODE_TYPE ModeStack::MODE_TOP_SECTION               (1ull << 19);
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL                   (1ull << 20);
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL_INITIALIZATION    (1ull << 21);
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL_CONDITION         (1ull << 22);
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL_INCREMENT         (1ull << 23);
const srcMLState::MODE_TYPE ModeStack::MODE_PREPROC                   (1ull << 24);
const srcMLState::MODE_TYPE ModeStack::MODE_NEST                      (1ull << 25);
const srcMLState::MODE_TYPE ModeStack::MODE_EXPRESSION_BLOCK          (1ull << 26);
const srcMLState::MODE_TYPE ModeStack::MODE_INTERNAL_END_PAREN        (1ull << 27);
const srcMLState::MODE_TYPE ModeStack::MODE_ACCESS_REGION             (1ull << 28);
const srcMLState::MODE_TYPE ModeStack::MODE_DO_STATEMENT              (1ull << 29);
const srcMLState::MODE_TYPE ModeStack::MODE_IGNORE_TERMINATE          (1ull << 30);
const srcMLState::MODE_TYPE ModeStack::MODE_USING                     (1ull << 31);
const srcMLState::MODE_TYPE ModeStack::MODE_EXTERN                    (1ull << 32);
const srcMLState::MODE_TYPE ModeStack::MODE_TRAILING_RETURN           (1ull << 33);
const srcMLState::MODE_TYPE ModeStack::MODE_INTERNAL_END_CURLY        (1ull << 34);
const srcMLState::MODE_TYPE ModeStack::MODE_ISSUE_EMPTY_AT_POP        (1ull << 35);
const srcMLState::MODE_TYPE ModeStack::MODE_INITIALIZATION_LIST       (1ull << 36);
const srcMLState::MODE_TYPE ModeStack::MODE_CLASS                     (1ull << 37);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_ENDIF              (1ull << 38);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_BLOCK              (1ull << 39);
const srcMLState::MODE_TYPE ModeStack::MODE_END_ONLY_AT_RPAREN        (1ull << 40);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_BLOCK_NO_TERMINATE (1ull << 41);
const srcMLState::MODE_TYPE ModeStack::MODE_ARGUMENT_LIST             (1ull << 42);
const srcMLState::MODE_TYPE ModeStack::MODE_ASSOCIATION_LIST          (1ull << 43);
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_NAME             (1ull << 44);
const srcMLState::MODE_TYPE ModeStack::MODE_ELSE                      (1ull << 45);
const srcMLState::MODE_TYPE ModeStack::MODE_TYPEDEF                   (1ull << 46);
const srcMLState::MODE_TYPE ModeStack::MODE_DECL                      (1ull << 47);
const srcMLState::MODE_TYPE ModeStack::MODE_EAT_TYPE                  (1ull << 48);
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_PARAMETER        (1ull << 49);
const srcMLState::MODE_TYPE ModeStack::MODE_TERNARY                   (1ull << 50);
const srcMLState::MODE_TYPE ModeStack::MODE_OBJECTIVE_C_CALL          (1ull << 51);
const srcMLState::MODE_TYPE ModeStack::MODE_INNER_DECL                (1ull << 52);
const srcMLState::MODE_TYPE ModeStack::MODE_SWITCH                    (1ull << 53);
const srcMLState::MODE_TYPE ModeStack::MODE_TERNARY_CONDITION         (1ull << 54);
const srcMLState::MODE_TYPE ModeStack::MODE_THEN                      (1ull << 55);
const srcMLState::MODE_TYPE ModeStack::MODE_IN_INIT                   (1ull << 56);
const srcMLState::MODE_TYPE ModeStack::MODE_TRY                       (1ull << 57);
const srcMLState::MODE_TYPE ModeStack::MODE_END_LIST_AT_BLOCK         (1ull << 58);
const srcMLState::MODE_TYPE ModeStack::MODE_TEMPLATE_PARAMETER_LIST   (1ull << 59);
const srcMLState::MODE_TYPE ModeStack::MODE_ONLY_END_TERMINATE        (1ull << 60);
const srcMLState::MODE_TYPE ModeStack::MODE_ENUM                      (1ull << 61);
const srcMLState::MODE_TYPE ModeStack::MODE_ANONYMOUS                 (1ull << 62);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_COMMA              (1ull << 63);

// Modes greater than 64-bit
const srcMLState::MODE_TYPE ModeStack::MODE_RANGED_FOR                  ("10000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_ASSOCIATION_TYPE           ("100000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_FRIEND                    ("1000000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_CLASS_NAME               ("10000000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_BODY           ("100000000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_TYPE          ("1000000000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_END_CONTROL           ("10000000000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_FOR_LIKE_LIST        ("100000000000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_CALL       ("1000000000000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_IF_STATEMENT       ("10000000000000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_BLOCK_CONTENT     ("100000000000000000000000000000000000000000000000000000000000000000000000000");
const srcMLState::MODE_TYPE ModeStack::MODE_NO_BLOCK_CONTENT ("1000000000000000000000000000000000000000000000000000000000000000000000000000");
