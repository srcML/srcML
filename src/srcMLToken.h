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
#include <antlr/TokenRefCount.hpp>

enum { STARTTOKEN = 0, ENDTOKEN = 50, EMPTYTOKEN = 75 };

class srcMLToken : public antlr::Token {

 public:

 srcMLToken()
   : Token(), category(-1) {
  }

 srcMLToken(int t)
   : Token(t), category(-1) {
  }

 srcMLToken(int t, int cat)
   : Token(t), category(cat) {
  }

 srcMLToken(int t, std::string& s)
   : Token(t, s), category(-1) {
  }

  static antlr::RefToken factory()
  {
    return antlr::RefToken(new srcMLToken());
  }

  // current text of the token
  virtual std::string getText() const { return text; }

  // set the current text of the token
  virtual void setText(const std::string& s) { text = s; }

  // current category
  virtual int getCategory() const { return category; }

  // set the current category
  virtual void setCategory(int cat) { category = cat; }

  // destructor
  virtual ~srcMLToken() {}

 private:
  int category;
  std::string text;
};

inline srcMLToken* EndTokenFactory(int token) {

  return new srcMLToken(token, ENDTOKEN);
}

inline srcMLToken* EmptyTokenFactory(int token) {

  return new srcMLToken(token, EMPTYTOKEN);
}

inline srcMLToken* StartTokenFactory(int token) {

  return new srcMLToken(token, STARTTOKEN);
}

inline srcMLToken const * const Token2srcMLToken(const antlr::RefToken& token) {

  //  return *token;
  return static_cast<const srcMLToken*>(&(*token));
}

inline bool isstart(const antlr::RefToken& token) {

  return Token2srcMLToken(token)->getCategory() != ENDTOKEN;

  return token->getLine() != ENDTOKEN;
}

inline bool isempty(const antlr::RefToken& token) {

  return Token2srcMLToken(token)->getCategory() == EMPTYTOKEN;
}

#endif
