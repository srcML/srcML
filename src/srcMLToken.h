/*
  srcMLToken.h

  Copyright (C) 2004-2006  SDML (www.sdml.info)

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

  Specialized token for srcML markup elements.
*/

#ifndef INCLUDED_SRCMLTOKEN_H
#define INCLUDED_SRCMLTOKEN_H

#include "project.h"

#include <antlr/Token.hpp>

enum { STARTTOKEN = 0, ENDTOKEN = 50, EMPTYTOKEN = 75 };

template <int T>
class srcMLToken : public antlr::Token {

 public:

  srcMLToken(int t)
    : Token(t) {}

  int getLine() const { return T; }
};

typedef srcMLToken<ENDTOKEN> EndToken;
typedef srcMLToken<EMPTYTOKEN> EmptyToken;
typedef srcMLToken<STARTTOKEN> StartToken;

inline bool isstart(const antlr::RefToken& token) {

  return token->getLine() != ENDTOKEN;
}

inline bool isempty(const antlr::RefToken& token) {

  return token->getLine() == EMPTYTOKEN;
}

#endif
