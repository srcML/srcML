/*
  TokenParser.hpp

  Copyright (C) 2003-2010  SDML (www.sdml.info)

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

  Adds methods for Island grammar parsing for antlr::Parser.
  Is included in the desired passthrough parser grammar.

  Done because antlr doesn't really support inheritance only
  grammar inheritance

  In order to use:

  Put a #include "tokenparser.h" in your grammar file

  Define a symbol(?) start.  Include in it all the cases
  that you want to use:
  start : (case1 | case2)
  ;

  Any tokens that don't match a case reachable from start
  will be passed along unchanged

  All whitespace tokens, WS, are passed along unchanged

  Will be overridden in stream parser version.
*/

#ifndef TOKENPARSER_HPP
#define TOKENPARSER_HPP

#include <iostream>
#include <antlr/TokenStream.hpp>
#include <list>

class TokenParser {

 public:
  // starts an element
  virtual void startElement(int) = 0;

  // starts an element that starts before skipped tokens
  virtual void startNoSkipElement(int) = 0;

  // ends an element
  virtual void endElement(int) = 0;

  // starts an element
  virtual void emptyElement(int) = 0;

  virtual void flushSkip() = 0;

  virtual int SkipBufferSize() = 0;

  virtual antlr::RefToken* CurrentToken() = 0;

  virtual void setFinalToken() = 0;

  virtual void consumeSkippedTokens() = 0;

  virtual bool checkOption(int) = 0;

  virtual ~TokenParser() {}
};

#endif
