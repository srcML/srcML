/**
 * @file StreamMLParser.hpp
 *
 * @copyright Copyright (C) 2002-2014 srcML, LLC. (www.srcML.org)
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
 *
 * Adds markup language capabilities and stream parsing to a Parser
 * class.  The stream parsing is added by the srcMLParser class, StreamParser.
 */

#ifndef INCLUDED_STREAM_MLPARSER_HPP
#define INCLUDED_STREAM_MLPARSER_HPP

#include <antlr/TokenStream.hpp>
#include "TokenStream.hpp"

/*
#include <srcml_types.hpp>
#include <srcml_macros.hpp>
#include <srcml.h>
*/
 
#include <list>
#include <cassert>

#include "srcMLToken.hpp"
#include "srcMLParser.hpp"
#include "ModeStack.hpp"

/**
 * StreamMLParser
 *
 * Parses source code file in a stream fashion.
 *
 * srcMLOutput pull(requests tokens) which progressively parses/translates
 * source code.
 */
class StreamMLParser : public srcMLParser, public TokenStream {

    // Follow example of ANTLR generated parsers
public:

    /**
     * StreamMLParser
     * @param lexer token stream lexer
     * @param language parsing language
     * @param parsing_options the parsing options
     *
     * Constructor.  Set up parser and start unit.
     */
    StreamMLParser(antlr::TokenStream& lexer, int language, OPTION_TYPE & parsing_options)
        : srcMLParser(lexer, language, parsing_options), options(parsing_options),
          inskip(false) {

        pouttb = &tb;
        pskiptb = &skiptb;

        srcMLParser::startUnit();
    }

    /**
     * ~StreamMLParser
     *
     * Destructor.
     */
    ~StreamMLParser() {}

    /**
     * startElement
     * @param id element to start
     *
     * Starts an element id (xml tag).
    */
    void startElement(int id) {

        srcMLParser::currentState().push(id);
        pushSTokenFlush(id);
    }

    /**
     * startNoSkipElement
     * @param id element to start
     *
     * Starts an element id (xml tag).  Do not output after skip tokens
     * (usually whitespace).
     */
    void startNoSkipElement(int id) {

        srcMLParser::currentState().push(id);
        pushSToken(id);
    }

    /**
     * endElement
     * @param id element to end
     *
     * End an element id (xml tag).
     */
    void endElement(int id) {

        if((srcMLParser::getMode() & srcMLParser::MODE_ISSUE_EMPTY_AT_POP).any()) 
            pushSToken(id);

        pushEToken(id);
        srcMLParser::currentState().pop();
    }

    /**
     * emptyElement
     * @param id issue empty element
     *
     * Issue an empty element id (xml tag).
     */
    void emptyElement(int id) {

        // push a empty element token
        pushTokenFlush(antlr::RefToken(EmptyTokenFactory(id)));
    }

    /**
     * addElement
     * @param id element to add
     *
     * Starts an element id that is output when mode ends.
     */
    void addElement(int id) {

        srcMLParser::currentState().push(id);
    }

private:

    /**
     * isSkipToken
     * @param token_type token type to test if is a skip token
     *
     * Predictate testing if token should be skipped during parsing (e.g. whitespace/comments/preprocessor).
     *
     * @returns if the token should be skipped.
     */
    bool isSkipToken(int token_type) const {

        switch (token_type) {

            // Always handled as white space and hidden from
            // parsing
        case srcMLParser::WS:
        case srcMLParser::CONTROL_CHAR:
        case srcMLParser::EOL_BACKSLASH:
        case srcMLParser::COMMENT_START:
        case srcMLParser::COMMENT_END:
        case srcMLParser::LINECOMMENT_END:
        case srcMLParser::COMMENT_TEXT:
            return true;
            break;

            // whether to handle a line comment start or an EOL
            // depends on whether we are in preprocssing handling or not
            // where the detection of the end of the preprocessing line
            // is needed (preprocessing lines end at EOL, or the start of
            // a line comment)
        case srcMLParser::LINECOMMENT_START:
        case srcMLParser::JAVADOC_COMMENT_START:
        case srcMLParser::DOXYGEN_COMMENT_START:
        case srcMLParser::LINE_DOXYGEN_COMMENT_START:
        case srcMLParser::EOL:

            return !inskip;
            break;

            // anything else is passed to the parser
        default:

            return false;
            break;
        }
    }

    /**
     * fillTokenBuffer
     *
     * Fills the token buffer with new tokens.
     * Invokes parser.
     */
    void fillTokenBuffer() {

        try {

            if (consumeSkippedToken()) {
                flushSkip();
                return;
            }

            // more partial parsing to do
            srcMLParser::start();

        } catch (const std::exception&) {

            // when an error occurs just insert an error element
            emptyElement(srcMLParser::SERROR_PARSE);
        }
    }

    /*
      Provide markup tag specific pushToken methods
    */

    /**
     * pushSToken
     * @param token token to push
     * @param flush do we flush output
     *
     * Push the start token token onto the output token stream flushing skipped tokens before output
     * if requested.
     */
    void pushSToken(int token) {

        // push a new start token
        pushToken(antlr::RefToken(StartTokenFactory(token)));
    }

    void pushSTokenFlush(int token) {

        // push a new start token
        pushTokenFlush(antlr::RefToken(StartTokenFactory(token)));
    }

    /**
     * pushEToken
     * @param token token to push
     * @param flush do we flush output
     *
     * Push the end token token onto the output token stream no flushing skipped tokens before output.
     */
    void pushEToken(int token) {

        // push a new end token
        pushToken(antlr::RefToken(EndTokenFactory(token)));
    }

    /**
     * consume
     *
     * Consume the current token.
     */
    void consume() {

        // push the token onto the correct output stream
        pushCorrectToken();

        // rest of consume process
        srcMLParser::consume();

        // consume any skipped tokens
        while (consumeSkippedToken())
            ;
    }

    /**
     * consumeSkippedTokens
     *
     * Consume skip tokens if any and indicate if any where consumed.
     *
     * @returns if consumed any skip tokens.
     */
    bool consumeSkippedToken() {

        // preprocessor (unless we already are in one)
        if (isoption(options, SRCML_OPTION_CPP) && !inskip && srcMLParser::LA(1) == srcMLParser::PREPROC) {

            // start preprocessor handling
            inskip = true;

            // use preprocessor token buffers
            pouttb = &pretb;
            pskiptb = &skippretb;

            // parse preprocessor statement stopping at EOL
            try {

                srcMLParser::preprocessor();

            } catch(...) {}

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
        if (srcMLParser::LA(1) == srcMLParser::MACRO_NAME && !inskip) {

            inskip = true;

            // use preprocessor token buffers
            pouttb = &pretb;
            pskiptb = &skippretb;

            // parse macro_call
            srcMLParser::macro_pattern_call();

            // flush remaining whitespace from preprocessor handling onto preprocessor buffer
            pretb.splice(pretb.end(), skippretb);

            // move back to normal buffer
            pskiptb = &skiptb;
            pouttb = &tb;

            // put preprocessor buffer into skipped buffer
            skiptb.splice(skiptb.end(), pretb);

            inskip = false;
            return true;

        }

        if (!inskip && srcMLParser::LA(1) == srcMLParser::VISUAL_CXX_ASM) {

            // start preprocessor handling
            inskip = true;

            // use preprocessor token buffers
            pouttb = &pretb;
            pskiptb = &skippretb;

            // parse preprocessor statement stopping at EOL
            try {

                srcMLParser::visual_cxx_asm_declaration();

            } catch(...) {}

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

        if (isSkipToken(srcMLParser::LA(1))) {
            // skipped tokens are put on a special buffer
            pushSkipToken();

            // rest of consume process
            srcMLParser::consume();

            return true;
        }

        // no white space or preprocessor tokens were skipped
        return false;
    }

    /**
     * flushSkip
     *
     * Flush any skipped tokens to the output token stream.
     */
    void flushSkip() {
        flushSkip(output());
    }

    /**
     * SkipBufferSize
     *
     * Delegate to get the number of the skipped elements.
     *
     * @returns the number of skipped elements 
     */
    int SkipBufferSize() {
        return (int)skiptb.size();
    }

    /**
     * flushSkip
     * @param rf list of antlr tokens
     *
     * Flush any skipped tokens to the output token stream.
     */
    void flushSkip(std::list<antlr::RefToken>& rf) {

        rf.splice(rf.end(), skip());
    }

    /*
      Provide TokenStream interface
    */

    /**
     * nextToken
     * 
     * Get the next token in the output token stream.
     *
     * @returns the next token in the output token stream.
     */
    const antlr::RefToken& nextToken() {

        // assume primed
        tb.pop_front();

        // fill the token buffer if it is empty
        if (tb.empty())
            fillTokenBuffer();

        if(tb.empty())
            consume();

        // pop and send back the top token
        const antlr::RefToken& tok = tb.front();

        return tok;
    }

    /**
     * pushToken
     * @param rtoken token to push onto output stream
     *
     * Push the token onto the output token stream.
     */
    void pushToken(const antlr::RefToken& rtoken) {

        // don't push any tokens during guessing stage
        if (srcMLParser::inputState->guessing)
            return;

        // push the new token into the token buffer
        output().push_back(rtoken);
    }


    /**
     * pushTokenFlush
     * @param rtoken token to push onto output stream
     *
     * Push the token onto the output token stream flushing skip tokens
     */
    void pushTokenFlush(const antlr::RefToken& rtoken) {

        // don't push any tokens during guessing stage
        if (srcMLParser::inputState->guessing)
            return;

        // if it isn't an end token flush whitespace tokens
        flushSkip(output());

        // push the new token into the token buffer
        output().push_back(rtoken);
    }

    /**
     * output
     *
     * Get output buffer list.
     *
     * @returns the output buffer.
     */
    std::list<antlr::RefToken>& output() {
        return *pouttb;
    }

    /**
     * skip
     *
     * Get the skip buffer.
     *
     * @returns the skip buffer.
     */
    std::list<antlr::RefToken>& skip() {
        return *pskiptb;
    }

    /**
     * pushSkipToken
     * @param rtoken a skip token to push
     *
     * Push the skipped token onto the output token stream
     */
    void pushSkipToken(const antlr::RefToken& rtoken) {

        // don't push any tokens during guessing stage
        if (srcMLParser::inputState->guessing)
            return;

        // push the new token into the token buffer
        skip().push_back(rtoken);
    }

    /**
     * pushCorrectToken
     *
     * Push the current token to correct buffer.
     */
    inline void pushCorrectToken() {

        pushCorrectToken(srcMLParser::LT(1));
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * pushCorrecToken
     * @param rtoken a token to push (not used)
     *
     * Push the a token onto the correct buffer.
     */
    inline void pushCorrectToken(const antlr::RefToken& rtoken) {

        if (isSkipToken(srcMLParser::LA(1)))

            // push the token
            pushSkipToken();

        else

            // push the token
            pushToken();
    }

#pragma GCC diagnostic pop

    /**
     * pushToken
     *
     * Push the current token onto the output token stream.
     */
    void pushToken() {

        pushTokenFlush(srcMLParser::LT(1));
    }

    /**
     * pushSkipToken
     *
     * Push the current skip token onto the output token stream.
     */
    void pushSkipToken() {

        pushSkipToken(srcMLParser::LT(1));
    }

    /**
     * CurrentToken
     *
     * Get the current token (last added).
     *
     * @returns the current token (last added).
     */
    antlr::RefToken* CurrentToken() {

        return &(output().back());
    }

private:

    /** parser options */
    OPTION_TYPE & options;

    /** if in a skip */
    bool inskip;

    /** token buffer */
    std::list<antlr::RefToken> tb;

    /** skipped token buffer */
    std::list<antlr::RefToken> skiptb;

    /** preprocessor buffer */
    std::list<antlr::RefToken> pretb;

    /** preprocessor skipped token buffer */
    std::list<antlr::RefToken> skippretb;

    /** current token buffer */
    std::list<antlr::RefToken>* pouttb;

    /** current skipped token buffer */
    std::list<antlr::RefToken>* pskiptb;

};

#endif
