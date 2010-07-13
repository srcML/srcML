/*
  Mode.cpp

  Copyright (C) 2002-2006  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Method definitions for inclusion in srcMLParser
*/

#include "Mode.h"

#ifdef __GNUC__
#define ull(a) a##ULL
#else
#define ull(a) a##i64
#endif

/* Set of mode flags */

const State::MODE_TYPE Mode::MODE_STATEMENT           = ull(0x0000000000000001);
const State::MODE_TYPE Mode::MODE_LIST                = ull(0x0000000000000002);
const State::MODE_TYPE Mode::MODE_EXPECT              = ull(0x0000000000000004);
const State::MODE_TYPE Mode::MODE_DETECT_COLON        = ull(0x0000000000000008);            
const State::MODE_TYPE Mode::MODE_TEMPLATE            = ull(0x0000000000000010);
const State::MODE_TYPE Mode::MODE_ARGUMENT            = ull(0x0000000000000020);
const State::MODE_TYPE Mode::MODE_NAMESPACE           = ull(0x0000000000000040);
const State::MODE_TYPE Mode::MODE_PARAMETER           = ull(0x0000000000000080);
const State::MODE_TYPE Mode::MODE_EXPRESSION          = ull(0x0000000000000100);
const State::MODE_TYPE Mode::MODE_CALL                = ull(0x0000000000000200);
const State::MODE_TYPE Mode::MODE_CONDITION           = ull(0x0000000000000400);
const State::MODE_TYPE Mode::MODE_TOP                 = ull(0x0000000000000800);
const State::MODE_TYPE Mode::MODE_BLOCK               = ull(0x0000000000001000);
const State::MODE_TYPE Mode::MODE_INIT                = ull(0x0000000000002000);
const State::MODE_TYPE Mode::MODE_FUNCTION_TAIL       = ull(0x0000000000004000);
const State::MODE_TYPE Mode::MODE_PARSE_EOL           = ull(0x0000000000008000);
const State::MODE_TYPE Mode::MODE_LOCAL               = ull(0x0000000000010000);
const State::MODE_TYPE Mode::MODE_VARIABLE_NAME       = ull(0x0000000000020000);
const State::MODE_TYPE Mode::MODE_IF                  = ull(0x0000000000040000);
const State::MODE_TYPE Mode::MODE_TOP_SECTION         = ull(0x0000000000080000);
const State::MODE_TYPE Mode::MODE_FOR_GROUP           = ull(0x0000000000100000);
const State::MODE_TYPE Mode::MODE_FOR_INITIALIZATION  = ull(0x0000000000200000);
const State::MODE_TYPE Mode::MODE_FOR_CONDITION       = ull(0x0000000000400000);
const State::MODE_TYPE Mode::MODE_FOR_INCREMENT       = ull(0x0000000000800000);
const State::MODE_TYPE Mode::MODE_PREPROC             = ull(0x0000000001000000);
const State::MODE_TYPE Mode::MODE_NEST                = ull(0x0000000002000000);
const State::MODE_TYPE Mode::MODE_EXPRESSION_BLOCK    = ull(0x0000000004000000);
const State::MODE_TYPE Mode::MODE_INTERNAL_END_PAREN  = ull(0x0000000008000000);
const State::MODE_TYPE Mode::MODE_ACCESS_REGION       = ull(0x0000000010000000);
const State::MODE_TYPE Mode::MODE_DO_STATEMENT        = ull(0x0000000020000000);
const State::MODE_TYPE Mode::MODE_IGNORE_TERMINATE    = ull(0x0000000040000000);
const State::MODE_TYPE Mode::MODE_SKIP_PARSE          = ull(0x0000000080000000);
const State::MODE_TYPE Mode::MODE_EXTERN              = ull(0x0000000100000000);
const State::MODE_TYPE Mode::MODE_CALLLIST            = ull(0x0000000200000000);
const State::MODE_TYPE Mode::MODE_INTERNAL_END_CURLY  = ull(0x0000000400000000);
const State::MODE_TYPE Mode::MODE_MACRO_CONTENTS      = ull(0x0000000800000000);
const State::MODE_TYPE Mode::MODE_GUESSING            = ull(0x0000001000000000);
const State::MODE_TYPE Mode::MODE_CLASS               = ull(0x0000002000000000);
const State::MODE_TYPE Mode::MODE_FUNCTION            = ull(0x0000004000000000);
const State::MODE_TYPE Mode::MODE_END_AT_BLOCK        = ull(0x0000008000000000);
const State::MODE_TYPE Mode::MODE_END_ONLY_AT_RPAREN  = ull(0x0000010000000000);
const State::MODE_TYPE Mode::MODE_END_AT_BLOCK_NO_TERMINATE = ull(0x0000020000000000);
const State::MODE_TYPE Mode::MODE_NEG_BLOCK           = ull(0x0000040000000000);
const State::MODE_TYPE Mode::MODE_IF_COND             = ull(0x0000080000000000);
const State::MODE_TYPE Mode::MODE_FUNCTION_NAME       = ull(0x0000100000000000);
const State::MODE_TYPE Mode::MODE_ELSE                = ull(0x0000200000000000);
const State::MODE_TYPE Mode::MODE_TYPEDEF             = ull(0x0000400000000000);
const State::MODE_TYPE Mode::MODE_DECL                = ull(0x0000800000000000);
const State::MODE_TYPE Mode::MODE_EAT_TYPE            = ull(0x0001000000000000);
const State::MODE_TYPE Mode::MODE_FUNCTION_PARAMETER  = ull(0x0002000000000000);
const State::MODE_TYPE Mode::MODE_EAT_VAR_TYPE        = ull(0x0004000000000000);
const State::MODE_TYPE Mode::MODE_DERIVED             = ull(0x0008000000000000);
const State::MODE_TYPE Mode::MODE_INNER_DECL          = ull(0x0010000000000000);
const State::MODE_TYPE Mode::MODE_REQUIRES            = ull(0x0020000000000000);
const State::MODE_TYPE Mode::MODE_CONCEPT             = ull(0x0040000000000000);


srcMLState& Mode::currentState() {

  return pstate->currentState();
}
/*
int Mode::size() const {

  return pstate->size();
}
*/
int Mode::getParen() const {

  return pstate->getParen();
}

void Mode::incParen() {

  pstate->incParen();
}

void Mode::decParen() {

  pstate->decParen();
}

int Mode::getTypeCount() const {

  return pstate->getTypeCount();
}

void Mode::setTypeCount(int n) {

  pstate->setTypeCount(n);
}

void Mode::incTypeCount() {

  pstate->incTypeCount();
}

void Mode::decTypeCount() {

  pstate->decTypeCount();
}

State::MODE_TYPE Mode::getMode() const {

  return pstate->getMode();
}

State::MODE_TYPE Mode::getPrevMode() const {

  return pstate->getMode();
}

State::MODE_TYPE Mode::getTransparentMode() const {

  return pstate->getTransparentMode();
}

void Mode::startNewMode(const State::MODE_TYPE& m) {

  pstate->startNewMode(m);
}

void Mode::endCurrentMode() {

  // remove the mode
  pstate->endCurrentMode();
}

void Mode::endCurrentMode(const State::MODE_TYPE& m) {

  // remove the mode
  pstate->endCurrentMode(m);
}

void Mode::endLastMode() {

  // remove the mode
  pstate->endLastMode();
}

void Mode::endTopMode() {

  // remove the mode
  pstate->endCurrentMode();
}

void Mode::endCurrentModeSafely(const State::MODE_TYPE& m) {

  // remove the mode
  pstate->endCurrentModeSafely(m);
}

void Mode::setMode(const State::MODE_TYPE& m) {

  pstate->setMode(m);
}

void Mode::clearMode(const State::MODE_TYPE& m) {

  pstate->clearMode(m);
}

void Mode::replaceMode(const State::MODE_TYPE& o, const State::MODE_TYPE& n) {

  pstate->replaceMode(o, n);
}

bool Mode::inMode(const State::MODE_TYPE& m) const {

  return pstate->inMode(m);
}

bool Mode::inPrevMode(const State::MODE_TYPE& m) const {

  return pstate->inPrevMode(m);
}

bool Mode::inTransparentMode(const State::MODE_TYPE& m) const {

  return pstate->inTransparentMode(m);
}

// end elements down to a specific mode
void Mode::endDownToMode(const State::MODE_TYPE& mode) {

  if (!pstate->inTransparentMode(mode))
      return;

  while (pstate->size() > 1 && !pstate->inMode(mode)) {
      endCurrentMode();
  }
}

// end elements down to a specific mode
void Mode::endDownToFirstMode(const State::MODE_TYPE& mode) {

  //  if (pstate->getTransparentMode() & (mode == 0))
  //      return;

  while (pstate->size() > 1 && (pstate->getMode() & mode) == 0) {
      endCurrentMode();
  }
}

// end elements down to a specific mode
void Mode::endDownOverMode(const State::MODE_TYPE& mode) {

  while (pstate->size() > 1 && pstate->inMode(mode)) {
      endCurrentMode();
  }
}
