/**
 * @file Mode.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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
 
#include "ModeStack.hpp"

#ifdef __GNUC__

/** size of a Mode literal in GNU */
#define ull(a) a##ULL

#else

 /** size of a Mode literal in non-GNU */
#define ull(a) a##i64

#endif

/* Set of mode flags */
const srcMLState::MODE_TYPE ModeStack::MODE_STATEMENT                 = ull(0x0000000000000001);
const srcMLState::MODE_TYPE ModeStack::MODE_LIST                      = ull(0x0000000000000002);
const srcMLState::MODE_TYPE ModeStack::MODE_EXPECT                    = ull(0x0000000000000004);
const srcMLState::MODE_TYPE ModeStack::MODE_DETECT_COLON              = ull(0x0000000000000008);
const srcMLState::MODE_TYPE ModeStack::MODE_TEMPLATE                  = ull(0x0000000000000010);
const srcMLState::MODE_TYPE ModeStack::MODE_ARGUMENT                  = ull(0x0000000000000020);
const srcMLState::MODE_TYPE ModeStack::MODE_NAMESPACE                 = ull(0x0000000000000040);
const srcMLState::MODE_TYPE ModeStack::MODE_PARAMETER                 = ull(0x0000000000000080);
const srcMLState::MODE_TYPE ModeStack::MODE_EXPRESSION                = ull(0x0000000000000100);
const srcMLState::MODE_TYPE ModeStack::MODE_CALL                      = ull(0x0000000000000200);
const srcMLState::MODE_TYPE ModeStack::MODE_CONDITION                 = ull(0x0000000000000400);
const srcMLState::MODE_TYPE ModeStack::MODE_TOP                       = ull(0x0000000000000800);
const srcMLState::MODE_TYPE ModeStack::MODE_BLOCK                     = ull(0x0000000000001000);
const srcMLState::MODE_TYPE ModeStack::MODE_INIT                      = ull(0x0000000000002000);
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_TAIL             = ull(0x0000000000004000);
const srcMLState::MODE_TYPE ModeStack::MODE_PARSE_EOL                 = ull(0x0000000000008000);
const srcMLState::MODE_TYPE ModeStack::MODE_LOCAL                     = ull(0x0000000000010000);
const srcMLState::MODE_TYPE ModeStack::MODE_VARIABLE_NAME             = ull(0x0000000000020000);
const srcMLState::MODE_TYPE ModeStack::MODE_IF                        = ull(0x0000000000040000);
const srcMLState::MODE_TYPE ModeStack::MODE_TOP_SECTION               = ull(0x0000000000080000);
const srcMLState::MODE_TYPE ModeStack::MODE_FOR_GROUP                 = ull(0x0000000000100000);
const srcMLState::MODE_TYPE ModeStack::MODE_FOR_INITIALIZATION        = ull(0x0000000000200000);
const srcMLState::MODE_TYPE ModeStack::MODE_FOR_CONDITION             = ull(0x0000000000400000);
const srcMLState::MODE_TYPE ModeStack::MODE_FOR_INCREMENT             = ull(0x0000000000800000);
const srcMLState::MODE_TYPE ModeStack::MODE_PREPROC                   = ull(0x0000000001000000);
const srcMLState::MODE_TYPE ModeStack::MODE_NEST                      = ull(0x0000000002000000);
const srcMLState::MODE_TYPE ModeStack::MODE_EXPRESSION_BLOCK          = ull(0x0000000004000000);
const srcMLState::MODE_TYPE ModeStack::MODE_INTERNAL_END_PAREN        = ull(0x0000000008000000);
const srcMLState::MODE_TYPE ModeStack::MODE_ACCESS_REGION             = ull(0x0000000010000000);
const srcMLState::MODE_TYPE ModeStack::MODE_DO_STATEMENT              = ull(0x0000000020000000);
const srcMLState::MODE_TYPE ModeStack::MODE_IGNORE_TERMINATE          = ull(0x0000000040000000);
const srcMLState::MODE_TYPE ModeStack::MODE_USING                     = ull(0x0000000080000000);
const srcMLState::MODE_TYPE ModeStack::MODE_EXTERN                    = ull(0x0000000100000000);
const srcMLState::MODE_TYPE ModeStack::MODE_TRAILING_RETURN           = ull(0x0000000200000000);
const srcMLState::MODE_TYPE ModeStack::MODE_INTERNAL_END_CURLY        = ull(0x0000000400000000);
const srcMLState::MODE_TYPE ModeStack::MODE_ISSUE_EMPTY_AT_POP        = ull(0x0000000800000000);
// const srcMLState::MODE_TYPE ModeStack::MODE_GUESSING               = ull(0x0000001000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_CLASS                     = ull(0x0000002000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_ENDIF              = ull(0x0000004000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_BLOCK              = ull(0x0000008000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_END_ONLY_AT_RPAREN        = ull(0x0000010000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_BLOCK_NO_TERMINATE = ull(0x0000020000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_ARGUMENT_LIST             = ull(0x0000040000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_ASSOCIATION_LIST          = ull(0x0000080000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_NAME             = ull(0x0000100000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_ELSE                      = ull(0x0000200000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_TYPEDEF                   = ull(0x0000400000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_DECL                      = ull(0x0000800000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_EAT_TYPE                  = ull(0x0001000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_FUNCTION_PARAMETER        = ull(0x0002000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_TERNARY                   = ull(0x0004000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_OBJECTIVE_C_CALL          = ull(0x0008000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_INNER_DECL                = ull(0x0010000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_SWITCH                    = ull(0x0020000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_TERNARY_CONDITION         = ull(0x0040000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_THEN                      = ull(0x0080000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_IN_INIT                   = ull(0x0100000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_TRY                       = ull(0x0200000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_END_LIST_AT_BLOCK         = ull(0x0400000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_TEMPLATE_PARAMETER_LIST   = ull(0x0800000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_ONLY_END_TERMINATE        = ull(0x1000000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_ENUM                      = ull(0x2000000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_ANONYMOUS                 = ull(0x4000000000000000);
const srcMLState::MODE_TYPE ModeStack::MODE_END_AT_COMMA              = ull(0x8000000000000000);
