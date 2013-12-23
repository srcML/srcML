/*
  StreamMLParser.hpp

  Copyright (C) 2002-2013  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Adds markup language capabilities and stream parsing to a Parser
  class.  The stream parsing is added by the base class, StreamParser.
*/

#ifndef INCLUDED_STREAM_MLPARSER_HPP
#define INCLUDED_STREAM_MLPARSER_HPP

#include <antlr/TokenStream.hpp>
#include "TokenStream.hpp"

#include <list>
#include <cassert>

#include "srcMLToken.hpp"

template <typename Base>
class StreamMLParser : public Base, public TokenStream {

    // Follow example of ANTLR generated parsers
public:

    StreamMLParser(antlr::TokenStream& lexer, int language, int parsing_options = 0)
        : Base(lexer, language, parsing_options), options(parsing_options),
          inskip(false), _lexer(lexer) {

        pouttb = &tb;
        pskiptb = &skiptb;

        Base::startUnit();
	Base::macroList();
    }

    ~StreamMLParser() {}

    // starts an element
    void startElement(int id) {

        Base::currentState().push(id);
        pushSToken(id, true);
    }

    // starts an element
    void startNoSkipElement(int id) {

        Base::currentState().push(id);
        pushSToken(id, false);
    }

    // ends an element
    void endElement(int id) {

        pushEToken(id);
        Base::currentState().pop();
    }

    // starts an element
    void emptyElement(int id) {

        // push a empty element token
        pushToken(antlr::RefToken(EmptyTokenFactory(id)));
    }

private:

    // token skipped during parsing
    bool isSkipToken(int token_type) const {

        switch (token_type) {

            // Always handled as white space and hidden from
            // parsing
        case Base::WS:
        case Base::CONTROL_CHAR:
        case Base::EOL_BACKSLASH:
        case Base::COMMENT_START:
        case Base::COMMENT_END:
        case Base::LINECOMMENT_END:
        case Base::COMMENT_TEXT:
            return true;
            break;

            // whether to handle a line comment start or an EOL
            // depends on whether we are in preprocssing handling or not
            // where the detection of the end of the preprocessing line
            // is needed (preprocessing lines end at EOL, or the start of
            // a line comment)
        case Base::LINECOMMENT_START:
        case Base::JAVADOC_COMMENT_START:
	case Base::DOXYGEN_COMMENT_START:
	case Base::LINE_DOXYGEN_COMMENT_START:
        case Base::EOL:

            return !inskip;
            break;

            // anything else is passed to the parser
        default:

            return false;
            break;
        }
    }

    // fills the token buffer with new tokens
    void fillTokenBuffer() {

        try {

            if (consumeSkippedToken()) {
                flushSkip();
                return;
            }

            // more partial parsing to do
            Base::start();

        } catch (const std::exception&) {

            // when an error occurs just insert an error element
            emptyElement(Base::SERROR_PARSE);
        }
    }

    /*
      Provide markup tag specific pushToken methods
    */

    // push the start token onto the output token stream
    void pushSToken(int token, bool flush = true) {

        // push a new start token
        pushToken(antlr::RefToken(StartTokenFactory(token)), flush);
    }

    // push the end token with this number onto the output token stream
    void pushEToken(int token) {

        // push a new end token
        pushToken(antlr::RefToken(EndTokenFactory(token)), false);
    }

    // consume the current token
    void consume() {

        // push the token onto the correct output stream
        pushCorrectToken();

        // rest of consume process
        Base::consume();

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

        // macro call
	if (Base::LA(1) == Base::MACRO_NAME) {

	  if(inskip) {

	    Base::LT(1)->setType(Base::NAME);

	  } else {

	    inskip = true;

            // use preprocessor token buffers
            pouttb = &pretb;
            pskiptb = &skippretb;

            // parse macro_call
            Base::macro_pattern_call();

            // flush remaining whitespace from preprocessor handling onto preprocessor buffer
            pretb.splice(pretb.end(), skippretb);

            // move back to normal buffer
            pskiptb = &skiptb;
            pouttb = &tb;

            // put preprocessor buffer into skipped buffer
            skiptb.splice(skiptb.end(), pretb);

	  inskip = false;

	  }

	}

	if (isSkipToken(Base::LA(1))) {
            // skipped tokens are put on a special buffer
            pushSkipToken();

            // rest of consume process
            Base::consume();

            return true;
        }

        // no white space or preprocessor tokens were skipped
        return false;
    }

    // flush any skipped tokens to the output token stream
    void flushSkip() {
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

    // returns the next token in the output token stream
    const antlr::RefToken& nextToken() {

        // assume primed
        tb.pop_front();

        // fill the token buffer if it is empty
        if (tb.empty())
            fillTokenBuffer();

        // pop and send back the top token
        const antlr::RefToken& tok = tb.front();

        return tok;
    }

    // push the token onto the output token stream
    void pushToken(const antlr::RefToken& rtoken, bool flush = true) {

        // don't push any tokens during guessing stage
        if (Base::inputState->guessing)
            return;

        // if it isn't an end token flush whitespace tokens
        if (flush) {
            flushSkip(output());
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

    antlr::RefToken* CurrentToken() {

        return &(output().back());
    }

private:
    int options;
    bool inskip;
    antlr::TokenStream& _lexer;

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
