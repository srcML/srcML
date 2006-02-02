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

/* Set of mode flags */

const State::MODE_TYPE Mode::MODE_STATEMENT           = 0x0000000000000001ULL;
const State::MODE_TYPE Mode::MODE_LIST                = 0x0000000000000002ULL;
const State::MODE_TYPE Mode::MODE_EXPECT              = 0x0000000000000004ULL;
const State::MODE_TYPE Mode::MODE_DETECT_COLON        = 0x0000000000000008ULL;            
const State::MODE_TYPE Mode::MODE_TEMPLATE            = 0x0000000000000010ULL;
const State::MODE_TYPE Mode::MODE_ARGUMENT            = 0x0000000000000020ULL;
const State::MODE_TYPE Mode::MODE_NAMESPACE           = 0x0000000000000040ULL;
const State::MODE_TYPE Mode::MODE_PARAMETER           = 0x0000000000000080ULL;
const State::MODE_TYPE Mode::MODE_EXPRESSION          = 0x0000000000000100ULL;
const State::MODE_TYPE Mode::MODE_CALL                = 0x0000000000000200ULL;
const State::MODE_TYPE Mode::MODE_CONDITION           = 0x0000000000000400ULL;
const State::MODE_TYPE Mode::MODE_TOP                 = 0x0000000000000800ULL;
const State::MODE_TYPE Mode::MODE_BLOCK               = 0x0000000000001000ULL;
const State::MODE_TYPE Mode::MODE_INIT                = 0x0000000000002000ULL;
const State::MODE_TYPE Mode::MODE_FUNCTION_TAIL       = 0x0000000000004000ULL;
const State::MODE_TYPE Mode::MODE_PARSE_EOL           = 0x0000000000008000ULL;
const State::MODE_TYPE Mode::MODE_LOCAL               = 0x0000000000010000ULL;
const State::MODE_TYPE Mode::MODE_VARIABLE_NAME       = 0x0000000000020000ULL;
const State::MODE_TYPE Mode::MODE_IF                  = 0x0000000000040000ULL;
const State::MODE_TYPE Mode::MODE_TOP_SECTION         = 0x0000000000080000ULL;
const State::MODE_TYPE Mode::MODE_FOR_GROUP           = 0x0000000000100000ULL;
const State::MODE_TYPE Mode::MODE_FOR_INITIALIZATION  = 0x0000000000200000ULL;
const State::MODE_TYPE Mode::MODE_FOR_CONDITION       = 0x0000000000400000ULL;
const State::MODE_TYPE Mode::MODE_FOR_INCREMENT       = 0x0000000000800000ULL;
const State::MODE_TYPE Mode::MODE_PREPROC             = 0x0000000001000000ULL;
const State::MODE_TYPE Mode::MODE_NEST                = 0x0000000002000000ULL;
const State::MODE_TYPE Mode::MODE_EXPRESSION_BLOCK    = 0x0000000004000000ULL;
const State::MODE_TYPE Mode::MODE_INTERNAL_END_PAREN  = 0x0000000008000000ULL;
const State::MODE_TYPE Mode::MODE_ACCESS_REGION       = 0x0000000010000000ULL;
const State::MODE_TYPE Mode::MODE_DO_STATEMENT        = 0x0000000020000000ULL;
const State::MODE_TYPE Mode::MODE_IGNORE_TERMINATE    = 0x0000000040000000ULL;
const State::MODE_TYPE Mode::MODE_SKIP_PARSE          = 0x0000000080000000ULL;
const State::MODE_TYPE Mode::MODE_EXTERN              = 0x0000000100000000ULL;
const State::MODE_TYPE Mode::MODE_CALLLIST            = 0x0000000200000000ULL;
const State::MODE_TYPE Mode::MODE_INTERNAL_END_CURLY  = 0x0000000400000000ULL;
const State::MODE_TYPE Mode::MODE_MACRO_CONTENTS      = 0x0000000800000000ULL;
const State::MODE_TYPE Mode::MODE_GUESSING            = 0x0000001000000000ULL;
const State::MODE_TYPE Mode::MODE_CLASS               = 0x0000002000000000ULL;
const State::MODE_TYPE Mode::MODE_FUNCTION            = 0x0000004000000000ULL;
const State::MODE_TYPE Mode::MODE_END_AT_BLOCK        = 0x0000008000000000ULL;
const State::MODE_TYPE Mode::MODE_END_ONLY_AT_RPAREN  = 0x0000010000000000ULL;
const State::MODE_TYPE Mode::MODE_END_AT_BLOCK_NO_TERMINATE = 0x0000020000000000ULL;
const State::MODE_TYPE Mode::MODE_NEG_BLOCK           = 0x0000040000000000ULL;
const State::MODE_TYPE Mode::MODE_IF_COND             = 0x0000080000000000ULL;
const State::MODE_TYPE Mode::MODE_FUNCTION_NAME       = 0x0000100000000000ULL;
const State::MODE_TYPE Mode::MODE_ELSE                = 0x0000200000000000ULL;

State::MODE_TYPE Mode::getMode() const {

  return state.getMode();
}

State::MODE_TYPE Mode::getTransparentMode() const {

  return state.getTransparentMode();
}

void Mode::startNewMode(const State::MODE_TYPE& m) {

  state.startNewMode(m);
}

void Mode::endCurrentMode() {

  // remove the mode
  state.endCurrentMode();
}

void Mode::endCurrentMode(const State::MODE_TYPE& m) {

  // remove the mode
  state.endCurrentMode(m);
}

void Mode::endLastMode() {

  // remove the mode
  state.endLastMode();
}

void Mode::endTopMode() {

  // remove the mode
  state.endCurrentMode();
}

void Mode::endCurrentModeSafely(const State::MODE_TYPE& m) {

  // remove the mode
  state.endCurrentModeSafely(m);
}

void Mode::setMode(const State::MODE_TYPE& m) {

  state.setMode(m);
}

void Mode::clearMode(const State::MODE_TYPE& m) {

  state.clearMode(m);
}

void Mode::replaceMode(const State::MODE_TYPE& o, const State::MODE_TYPE& n) {

  state.replaceMode(o, n);
}

bool Mode::inMode(const State::MODE_TYPE& m) const {

  return state.inMode(m);
}

bool Mode::inTransparentMode(const State::MODE_TYPE& m) const {

  return state.inTransparentMode(m);
}

// end elements down to a specific mode
void Mode::endDownToMode(const State::MODE_TYPE& mode) {

  if (!state.inTransparentMode(mode))
      return;

  while (state.size() > 1 && !state.inMode(mode)) {
      endCurrentMode();
  }
}

// end elements down to a specific mode
void Mode::endDownToFirstMode(const State::MODE_TYPE& mode) {

  if (state.getTransparentMode() & (mode == 0))
      return;

  while (state.size() > 1 && (state.getMode() & mode) == 0) {
      endCurrentMode();
  }
}

// end elements down to a specific mode
void Mode::endDownOverMode(const State::MODE_TYPE& mode) {

  while (state.size() > 1 && state.inMode(mode)) {
      endCurrentMode();
  }
}
