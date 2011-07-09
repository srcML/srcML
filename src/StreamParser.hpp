/*
  StreamParser.hpp

  Copyright (C) 2002-2010  SDML (www.sdml.info)

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

  Adds stream parsing capabilities to a Parser class.

  Fundamentally it provides a nextToken method so that another Parser
  can use this as a TokenStream.

  A new class is created with the template parameter Base and
  TokenStream as base classes.  This allows other Parsers to use it as
  a TokenStream in their constructor.

  It also implements the TokenStream nextToken method and provides
  pushToken methods to fill the token buffer.

  Parser grammars that use this class must also inherit from TokenParser.hpp
*/

#ifndef INCLUDED_STREAM_PARSER_H
#define INCLUDED_STREAM_PARSER_H

#include <antlr/TokenStream.hpp>
#include "TokenStream.hpp"

#include <list>
#include <cassert>

template <typename Base>
class StreamParser : public Base, public TokenStream {

 public:
  StreamParser(antlr::TokenStream& lexer, int language, int options)
    : Base(lexer, language, options), inskip(false), finaltoken(false), _lexer(lexer) /* , saveguess(false) */ {
    pouttb = &tb;
    pskiptb = &skiptb;
  }

 public:

  bool inskip;
  bool finaltoken;
  antlr::TokenStream& _lexer;

  // consume the current token
  void consume() {

    // push the token onto the correct output stream
    pushCorrectToken();
    /*
    // save the token
    if (saveguess) {
      guessingbuffer.push_back(Base::LT(1));
    }
    */
    // rest of consume process
    Base::consume();

    if (!finaltoken) {
      // consume any skipped tokens
      consumeSkippedTokens();
    }

    finaltoken = false;
  }

  inline void setFinalToken() {
    finaltoken = true;
  }

  inline void clearFinalToken() {
    finaltoken = false;
  }

  // consume the current token
  void consumeSkippedTokens() {

    // consume any skipped tokens
    while (consumeSkippedToken())
      ;
  }

  // consume a current token
  bool consumeSkippedToken() {

    // preprocessor (unless we already are in one)
    if (!inskip && Base::LA(1) == Base::PREPROC) {

      // start preprocessor handling
      inskip = true;

      // use preprocessor token buffers
      pouttb = &pretb;
      pskiptb = &skippretb;

      // parse preprocessor statement stopping at EOL
      Base::preprocessor();

      // flush remaining whitespace from preprocessor handling onto preprocessor buffer
      pretb.splice(pretb.end(), skippretb);

      // move back to normal buffer
      pskiptb = &skiptb;
      pouttb = &tb;

      // put preprocessor buffer into skipped buffer
      skiptb.splice(skiptb.end(), pretb);

      // stop preprocessor handling
      inskip = false;

      return true;
    }

    // actual whitespace
    if (isSkipToken(Base::LA(1))) {
      // consume skipped character
      consumeSkip();

      return true;
    }

    // no white space or preprocessor tokens were skipped
    return false;
  }

  // token skipped during parsing
  virtual bool isSkipToken(int) const = 0;

  // fills the token buffer with new tokens
  virtual void fillTokenBuffer() = 0;

  // consume the current skipped token
  void consumeSkip() {

    // skipped tokens are put on a special buffer
    pushSkipToken();

    // rest of consume process
    Base::consume();
  }

  // flush any skipped tokens to the output token stream
  void flushSkip() {
    skip2output();
  }

  void skip2output() {
    flushSkip(output());
  }

  int SkipBufferSize() {
    return skiptb.size();
  }

  // flush any skipped tokens to the output token stream
  void flushSkip(std::list<antlr::RefToken>& rf) {

    rf.splice(rf.end(), skip());
  }

  /*
    Provide TokenStream interface
  */

 public:

  // returns the next token in the output token stream
  const antlr::RefToken& nextToken() {

    // assume primed
    tb.pop_front();

    // fill the token buffer if it is empty
    if (tb.empty())
      fillTokenBuffer();

    // pop and send back the top token
    const antlr::RefToken& tok = tb.front();

    //std::cerr << tok->getType() << " " << tok->getText() << std::endl;

    return tok;
  }


  /*
  // guessing buffer
  std::vector<antlr::RefToken> guessingbuffer;

  bool saveguess;

  int getGuessToken(int pos) {
    return guessingbuffer[pos]->getType();
  }

  int getGuessSize() const {
    return guessingbuffer.size();
  }

  void startSaveGuess() {
    saveguess = true;
    guessingbuffer.clear();
  }

  void endSaveGuess() {
    saveguess = false;
    guessingbuffer.push_back(Base::LT(1));
  }

  unsigned int mark() {
    if (saveguess)
      guessingbuffer.clear();

    return Base::mark();
  }

  void rewind(unsigned int pos) {
    Base::rewind(pos);
  }
  */
 protected:

  // push the token onto the output token stream
  void pushToken(const antlr::RefToken& rtoken, bool flush = true) {

    // don't push any tokens during guessing stage
    if (Base::inputState->guessing)
      return;

    // if it isn't an end token flush whitespace tokens
    if (flush) {
      skip2output();
    }

    // push the new token into the token buffer
    output().push_back(rtoken);
  }

  std::list<antlr::RefToken>& output() {
    return *pouttb;
  }

  std::list<antlr::RefToken>& skip() {
    return *pskiptb;
  }

  // push the skipped token onto the output token stream
  void pushSkipToken(const antlr::RefToken& rtoken) {

    // don't push any tokens during guessing stage
    if (Base::inputState->guessing)
      return;

    // push the new token into the token buffer
    skip().push_back(rtoken);
  }

  inline void pushCorrectToken() {

    pushCorrectToken(Base::LT(1));
  }

  inline void pushCorrectToken(const antlr::RefToken& rtoken) {

    if (isSkipToken(Base::LA(1)))

      // push the token
      pushSkipToken();

    else

      // push the token
      pushToken();
  }

  // push the token onto the output token stream
  void pushToken() {

    pushToken(Base::LT(1));
  }

  // push the skipped token onto the output token stream
  void pushSkipToken() {

    pushSkipToken(Base::LT(1));
  }

 public:

  ~StreamParser() {}

  antlr::RefToken* CurrentToken() {

    return &(output().back());
  }

 protected:

  // token buffer
  std::list<antlr::RefToken> tb;

  // skipped token buffer
  std::list<antlr::RefToken> skiptb;

  // preprocessor buffer
  std::list<antlr::RefToken> pretb;

  // preprocessor skipped token buffer
  std::list<antlr::RefToken> skippretb;

  // current token buffer
  std::list<antlr::RefToken>* pouttb;

  // current skipped token buffer
  std::list<antlr::RefToken>* pskiptb;
};

#endif
