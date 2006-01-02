/*
  TokenParser.h

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

  To add a new token to the output token buffer, use pushToken:
  case1 : { pushToken(S); } ITEM { pushToken(E); }

  All whitespace tokens, WS, are passed along unchanged

  Will be overridden in stream parser version.
*/

#ifndef TOKENPARSER_H
#define TOKENPARSER_H

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

  virtual ~TokenParser() {}
};

#endif
