/*
  StreamMLParser.h

  Copyright (C) 2002, 2003, 2004, 2005  SDML (www.sdml.info)

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
*/


/*

  Adds markup language capabilities and stream parsing to a Parser
  class.  The stream parsing is added by the base class, StreamParser.
*/

#ifndef INCLUDED_STREAM_MLPARSER_H
#define INCLUDED_STREAM_MLPARSER_H

#include "StreamParser.h"

#include "srcMLToken.h"

template <typename Base>
class StreamMLParser : public StreamParser<Base> {

  // Follow example of ANTLR generated parsers
 public:

  StreamMLParser(antlr::TokenStream& lexer, int language)
    : StreamParser<Base>(lexer, language) { 

    Base::startUnit();
  }

 public:

  ~StreamMLParser() {}

 public:

  // starts an element
  void startElement(int id) {

    Base::state.push(id);
    pushSToken(id, true);
  }

  // starts an element
  void startNoSkipElement(int id) {

    Base::state.push(id);
    pushSToken(id, false);
  }

  // ends an element
  void endElement(int id) {

    pushEToken(id);
    Base::state.pop();
  }

  // starts an element
  void emptyElement(int id) {

    // push a empty element token
    pushToken(antlr::RefToken(new EmptyToken(id)));
  }

 private:

  // token skipped during parsing
  bool isSkipToken(int token_type) const {

    switch (token_type) {
    case Base::WS: 
    case Base::FORMFEED:
      return true;
      break;

    case Base::LINECOMMENT:

      return !StreamParser<Base>::inskip;
      break;

    case Base::BLOCKCOMMENT:

      return true;
      break;

    case Base::BLOCKCOMMENTEOL:

      return !StreamParser<Base>::inskip;
      break;

    case Base::EOL_BACKSLASH:

      return true;
      break;

    case Base::EOL:

      return !StreamParser<Base>::inskip;
      break;

    default:

      return false;
      break;
    }
  }

  // fills the token buffer with new tokens
  void fillTokenBuffer() {

    try {

      if (StreamParser<Base>::consumeSkippedToken()) {
	StreamParser<Base>::flushSkip();
	return;
      }

      // more partial parsing to do
      Base::start();

    } catch (const std::exception&) {

      // when an error occurs just insert an error element
      emptyElement(Base::SERROR_PARSE);
    }
  }

  // push the token onto the output token stream
  void pushToken(const antlr::RefToken& rtoken, bool flush = true) {
        
    // don't push any tokens during guessing stage
    if (Base::inputState->guessing != 0)
      return;

    // normal pushToken
    StreamParser<Base>::pushToken(rtoken, flush);
  } 

  /*
    Provide markup tag specific pushToken methods
  */

  // push the start token onto the output token stream
  void pushSToken(int token, bool flush = true) {

    // push a new start token
    pushToken(antlr::RefToken(new StartToken(token)), flush);
  } 

  // push the end token with this number onto the output token stream
  void pushEToken(int token) {

    // push a new end token
    pushToken(antlr::RefToken(new EndToken(token)), false);
  }
};

#endif
