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
const srcMLState::MODE_TYPE ModeStack::MODE_STATEMENT                 (1ll << 0);
const srcMLState::MODE_TYPE ModeStack::MODE_LIST                      (1ll << 1);
const srcMLState::MODE_TYPE ModeStack::MODE_EXPECT                    (1ll << 2);
const srcMLState::MODE_TYPE ModeStack::MODE_DETECT_COLON              (1ll << 3);
const srcMLState::MODE_TYPE ModeStack::MODE_TEMPLATE                  (1ll << 4);
const srcMLState::MODE_TYPE ModeStack::MODE_ARGUMENT                  (1ll << 5);
const srcMLState::MODE_TYPE ModeStack::MODE_NAMESPACE                 (1ll << 6);
const srcMLState::MODE_TYPE ModeStack::MODE_PARAMETER                 (1ll << 7);
const srcMLState::MODE_TYPE ModeStack::MODE_EXPRESSION                (1ll << 8);
const srcMLState::MODE_TYPE ModeStack::MODE_CALL                      (1ll << 9);
const srcMLState::MODE_TYPE ModeStack::MODE_CONDITION                 (1ll << 10);
const srcMLState::MODE_TYPE ModeStack::MODE_TOP                       (1ll << 11);
const srcMLState::MODE_TYPE ModeStack::MODE_BLOCK                     (1ll << 12);
const srcMLState::MODE_TYPE ModeStack::MODE_INIT                      (1ll << 13);
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_TAIL             (1ll << 14);
const srcMLState::MODE_TYPE ModeStack::MODE_PARSE_EOL                 (1ll << 15);
const srcMLState::MODE_TYPE ModeStack::MODE_LOCAL                     (1ll << 16);
const srcMLState::MODE_TYPE ModeStack::MODE_VARIABLE_NAME             (1ll << 17);
const srcMLState::MODE_TYPE ModeStack::MODE_IF                        (1ll << 18);
const srcMLState::MODE_TYPE ModeStack::MODE_TOP_SECTION               (1ll << 19);
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL                   (1ll << 20);
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL_INITIALIZATION    (1ll << 21);
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL_CONDITION         (1ll << 22);
const srcMLState::MODE_TYPE ModeStack::MODE_CONTROL_INCREMENT         (1ll << 23);
const srcMLState::MODE_TYPE ModeStack::MODE_PREPROC                   (1ll << 24);
const srcMLState::MODE_TYPE ModeStack::MODE_NEST                      (1ll << 25);
const srcMLState::MODE_TYPE ModeStack::MODE_EXPRESSION_BLOCK          (1ll << 26);
const srcMLState::MODE_TYPE ModeStack::MODE_INTERNAL_END_PAREN        (1ll << 27);
const srcMLState::MODE_TYPE ModeStack::MODE_ACCESS_REGION             (1ll << 28);
const srcMLState::MODE_TYPE ModeStack::MODE_DO_STATEMENT              (1ll << 29);
const srcMLState::MODE_TYPE ModeStack::MODE_IGNORE_TERMINATE          (1ll << 30);
const srcMLState::MODE_TYPE ModeStack::MODE_USING                     (1ll << 31);
const srcMLState::MODE_TYPE ModeStack::MODE_EXTERN                    (1ll << 32);
const srcMLState::MODE_TYPE ModeStack::MODE_TRAILING_RETURN           (1ll << 33);
const srcMLState::MODE_TYPE ModeStack::MODE_INTERNAL_END_CURLY        (1ll << 34);
const srcMLState::MODE_TYPE ModeStack::MODE_ISSUE_EMPTY_AT_POP        (1ll << 35);
const srcMLState::MODE_TYPE ModeStack::MODE_INITIALIZATION_LIST       (1ll << 36);
const srcMLState::MODE_TYPE ModeStack::MODE_CLASS                     (1ll << 37);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_ENDIF              (1ll << 38);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_BLOCK              (1ll << 39);
const srcMLState::MODE_TYPE ModeStack::MODE_END_ONLY_AT_RPAREN        (1ll << 40);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_BLOCK_NO_TERMINATE (1ll << 41);
const srcMLState::MODE_TYPE ModeStack::MODE_ARGUMENT_LIST             (1ll << 42);
const srcMLState::MODE_TYPE ModeStack::MODE_ASSOCIATION_LIST          (1ll << 43);
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_NAME             (1ll << 44);
const srcMLState::MODE_TYPE ModeStack::MODE_ELSE                      (1ll << 45);
const srcMLState::MODE_TYPE ModeStack::MODE_TYPEDEF                   (1ll << 46);
const srcMLState::MODE_TYPE ModeStack::MODE_DECL                      (1ll << 47);
const srcMLState::MODE_TYPE ModeStack::MODE_EAT_TYPE                  (1ll << 48);
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_PARAMETER        (1ll << 49);
const srcMLState::MODE_TYPE ModeStack::MODE_TERNARY                   (1ll << 50);
const srcMLState::MODE_TYPE ModeStack::MODE_OBJECTIVE_C_CALL          (1ll << 51);
const srcMLState::MODE_TYPE ModeStack::MODE_INNER_DECL                (1ll << 52);
const srcMLState::MODE_TYPE ModeStack::MODE_SWITCH                    (1ll << 53);
const srcMLState::MODE_TYPE ModeStack::MODE_TERNARY_CONDITION         (1ll << 54);
const srcMLState::MODE_TYPE ModeStack::MODE_THEN                      (1ll << 55);
const srcMLState::MODE_TYPE ModeStack::MODE_IN_INIT                   (1ll << 56);
const srcMLState::MODE_TYPE ModeStack::MODE_TRY                       (1ll << 57);
const srcMLState::MODE_TYPE ModeStack::MODE_END_LIST_AT_BLOCK         (1ll << 58);
const srcMLState::MODE_TYPE ModeStack::MODE_TEMPLATE_PARAMETER_LIST   (1ll << 59);
const srcMLState::MODE_TYPE ModeStack::MODE_ONLY_END_TERMINATE        (1ll << 60);
const srcMLState::MODE_TYPE ModeStack::MODE_ENUM                      (1ll << 61);
const srcMLState::MODE_TYPE ModeStack::MODE_ANONYMOUS                 (1ll << 62);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_COMMA              (1ll << 63);

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
