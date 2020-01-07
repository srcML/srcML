/**
 * @file StreamMLParser.hpp
 *
 * @copyright Copyright (C) 2002-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Adds markup language capabilities and stream parsing to a Parser
 * class.  The stream parsing is added by the srcMLParser class, StreamParser.
 */

#ifndef INCLUDED_STREAM_MLPARSER_HPP
#define INCLUDED_STREAM_MLPARSER_HPP

#include <antlr/TokenStream.hpp>
#include "TokenStream.hpp"

#include <deque>
#include <stack>
#include <cassert>

#include "srcMLToken.hpp"
#include "srcMLParser.hpp"

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
        : srcMLParser(lexer, language, parsing_options), options(parsing_options) {

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
    void startElement(int id) final {

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
    void startNoSkipElement(int id) final {

        srcMLParser::currentState().push(id);
        pushSToken(id);
    }

    /**
     * endElement
     * @param id element to end
     *
     * End an element id (xml tag).
     */
    void endElement(int id) final {

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
    void emptyElement(int id) final {

        // push a empty element token
        auto gentoken = antlr::RefToken(EmptyTokenFactory(id));

        pushTokenFlush(gentoken);
    }

    /**
     * addElement
     * @param id element to add
     *
     * Starts an element id that is output when mode ends.
     */
    void addElement(int id) final {

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
    bool isSkipToken(int token_type) {

        // Always handled as white space and hidden from
        // parsing
        if (srcMLParser::whitespace_token_set.member(token_type))
            return true;

        // whether to handle a line comment start or an EOL
        // depends on whether we are in preprocssing handling or not
        // where the detection of the end of the preprocessing line
        // is needed (preprocessing lines end at EOL, or the start of
        // a line comment)
        if (token_type == srcMLParser::EOL)
            return !inskip;

        // anything else is passed to the parser
        return false;
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
    inline void pushSToken(int token) {

        // push a new start token
        auto ntoken = antlr::RefToken(StartTokenFactory(token));
        ntoken->setLine(LT(1)->getLine());
        ntoken->setColumn(LT(1)->getColumn());

        if (isoption(options, SRCML_OPTION_POSITION)) {
            ends.emplace(ntoken);
        }

        pushToken(ntoken);
    }

    inline void pushSSkipToken(int token) {

        // push a new start token
        auto ntoken = antlr::RefToken(StartTokenFactory(token));
        ntoken->setLine(LT(1)->getLine());
        ntoken->setColumn(LT(1)->getColumn());

        if (isoption(options, SRCML_OPTION_POSITION)) {
            ends.emplace(ntoken);
        }

        pushSkipToken(ntoken);
    }

    inline void pushSTokenFlush(int token) {

        // push a new start token
        auto ntoken = antlr::RefToken(StartTokenFactory(token));
        ntoken->setLine(LT(1)->getLine());
        ntoken->setColumn(LT(1)->getColumn());

        // save the start position of a full type for any following previous types
        if (token == srcMLParser::STYPE) {
            lasttypestartline = LT(1)->getLine();
            lasttypestartcolumn = LT(1)->getColumn();
        }

        // previous type get start positions from previous, well type
        if (token == srcMLParser::STYPEPREV) {
            ntoken->setLine(lasttypestartline);
            ntoken->setColumn(lasttypestartcolumn);
        }

        if (isoption(options, SRCML_OPTION_POSITION)) {
            ends.emplace(ntoken);
        }

        pushTokenFlush(ntoken);
    }

    /**
     * pushEToken
     * @param token token to push
     * @param flush do we flush output
     *
     * Push the end token token onto the output token stream no flushing skipped tokens before output.
     */
    inline void pushEToken(int token) {

        // push a new end token
        pushToken(antlr::RefToken(EndTokenFactory(token)));

        // end position info is needed from the matching last end token
        // that was enqueued
        if (isoption(options, SRCML_OPTION_POSITION)) {
            srcMLToken* qetoken = static_cast<srcMLToken*>(&(*std::move(ends.top())));

            qetoken->endline = lastline;
            qetoken->endcolumn = lastcolumn;

            if (token == srcMLParser::STYPE) {
                lasttypeendline = lastline;
                lasttypeendcolumn = lastcolumn;
            }

            if (token == srcMLParser::STYPEPREV) {
                qetoken->endline = lasttypeendline;
                qetoken->endcolumn = lasttypeendcolumn;
            }

            ends.pop();
        }
    }

    inline void pushESkipToken(int token) {

        // push a new end token
        pushSkipToken(antlr::RefToken(EndTokenFactory(token)));

        // end position info is needed from the matching last end token
        // that was enqueued
        if (isoption(options, SRCML_OPTION_POSITION)) {
            srcMLToken* qetoken = static_cast<srcMLToken*>(&(*std::move(ends.top())));
            qetoken->endline = slastline;
            qetoken->endcolumn = slastcolumn;
            ends.pop();
        }
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

        // record for end position of start elements
        lastline = LT(1)->getLine();
        lastcolumn = LT(1)->getColumn() - 1;

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

        if (isSkipToken(srcMLParser::LA(1))) {

            switch (srcMLParser::LA(1)) {
            case srcMLParser::CONTROL_CHAR:
            {
                antlr::RefToken controlElement = EmptyTokenFactory(LA(1));
                int n = LT(1)->getText()[0];
                char outar[20 + 2 + 1];
                snprintf(outar, 22, "0x%02x", n);
                controlElement->setText(outar);
                pushSkipToken(controlElement);
                srcMLParser::consume();

                break;
            }
            case srcMLParser::BLOCK_COMMENT_START:

                pushSSkipToken(srcMLParser::SCOMMENT);
                pushSkipToken();
                srcMLParser::consume();

                open_comments.push(srcMLParser::SCOMMENT);

                break;

            case srcMLParser::LINE_DOXYGEN_COMMENT_START:

                pushSSkipToken(srcMLParser::SLINE_DOXYGEN_COMMENT);
                pushSkipToken();
                srcMLParser::consume();

                open_comments.push(srcMLParser::SLINE_DOXYGEN_COMMENT);

                break;

            case srcMLParser::LINE_DOXYGEN_COMMENT_END:

                open_comments.pop();

                if (srcMLParser::LT(1)->getText().back() != '\n') {
                    pushSkipToken();
                    srcMLParser::consume();
                    slastcolumn = LT(1)->getColumn() - 1;
                    slastline = LT(1)->getLine();
                    pushESkipToken(srcMLParser::SLINE_DOXYGEN_COMMENT);
                } else {
                    slastcolumn = LT(1)->getColumn() - 1;
                    slastline = LT(1)->getLine();
                    pushESkipToken(srcMLParser::SLINE_DOXYGEN_COMMENT);
                    pushSkipToken();
                    srcMLParser::consume();
                }

                break;

            case srcMLParser::DOXYGEN_COMMENT_START:

                pushSSkipToken(srcMLParser::SDOXYGEN_COMMENT);
                pushSkipToken();
                srcMLParser::consume();

                open_comments.push(srcMLParser::SDOXYGEN_COMMENT);

                break;

            case srcMLParser::WHOLE_COMMENT:

                pushSSkipToken(srcMLParser::SCOMMENT);
                pushSkipToken();
                srcMLParser::consume();
                slastcolumn = LT(1)->getColumn() - 1;
                slastline = LT(1)->getLine();
                pushESkipToken(srcMLParser::SCOMMENT);

                break;

            case srcMLParser::BLOCK_COMMENT_END:

                open_comments.pop();

                pushSkipToken();
                srcMLParser::consume();
                slastcolumn = LT(1)->getColumn() - 1;
                slastline = LT(1)->getLine();
                pushESkipToken(srcMLParser::SCOMMENT);

                break;

            case srcMLParser::DOXYGEN_COMMENT_END:

                open_comments.pop();

                pushSkipToken();
                srcMLParser::consume();
                slastcolumn = LT(1)->getColumn() - 1;
                slastline = LT(1)->getLine();
                pushESkipToken(srcMLParser::SDOXYGEN_COMMENT);

                break;

            case srcMLParser::JAVADOC_COMMENT_END:

                open_comments.pop();

                pushSkipToken();
                srcMLParser::consume();
                slastcolumn = LT(1)->getColumn() - 1;
                slastline = LT(1)->getLine();
                pushESkipToken(srcMLParser::SJAVADOC_COMMENT);

                break;

            case srcMLParser::JAVADOC_COMMENT_START:

                pushSSkipToken(srcMLParser::SJAVADOC_COMMENT);
                pushSkipToken();
                srcMLParser::consume();

                open_comments.push(srcMLParser::SJAVADOC_COMMENT);

                break;

            case srcMLParser::LINE_COMMENT_START:

                pushSSkipToken(srcMLParser::SLINECOMMENT);
                pushSkipToken();
                srcMLParser::consume();

                open_comments.push(srcMLParser::SLINECOMMENT);

                break;

            case srcMLParser::LINE_COMMENT_END:

                open_comments.pop();

                if (srcMLParser::LT(1)->getText().back() != '\n') {
                    pushSkipToken();
                    srcMLParser::consume();
                    slastcolumn = LT(1)->getColumn() - 1;
                    slastline = LT(1)->getLine();
                    pushESkipToken(srcMLParser::SLINECOMMENT);
                } else {
                    slastcolumn = LT(1)->getColumn() - 1;
                    slastline = LT(1)->getLine();
                    pushESkipToken(srcMLParser::SLINECOMMENT);
                    pushSkipToken();
                    srcMLParser::consume();
                }

                break;

            default:
                // skipped tokens are put on a special buffer
                pushSkipToken();
                srcMLParser::consume();

                break;
            }

            return true;
        }

        if (inskip)
            return false;

        // preprocessor (unless we already are in one)
        if (isoption(options, SRCML_OPTION_CPP) && srcMLParser::LA(1) == srcMLParser::PREPROC) {

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
            pretb.insert(pretb.end(), std::make_move_iterator(skippretb.begin()), std::make_move_iterator(skippretb.end()));
            skippretb.clear();

            // move back to normal buffer
            pskiptb = &skiptb;
            pouttb = &tb;

            // put preprocessor buffer into skipped buffer
            skiptb.insert(skiptb.end(), std::make_move_iterator(pretb.begin()), std::make_move_iterator(pretb.end()));
            pretb.clear();

            // stop preprocessor handling
            inskip = false;

            return true;
        }

        // macro call
        if (srcMLParser::LA(1) == srcMLParser::MACRO_NAME) {

            inskip = true;

            // use preprocessor token buffers
            pouttb = &pretb;
            pskiptb = &skippretb;

            // parse macro_call
            srcMLParser::macro_pattern_call();

            // flush remaining whitespace from preprocessor handling onto preprocessor buffer
            pretb.insert(pretb.end(), std::make_move_iterator(skippretb.begin()), std::make_move_iterator(skippretb.end()));
            skippretb.clear();

            // move back to normal buffer
            pskiptb = &skiptb;
            pouttb = &tb;

            // put preprocessor buffer into skipped buffer
            skiptb.insert(skiptb.end(), std::make_move_iterator(pretb.begin()), std::make_move_iterator(pretb.end()));
            pretb.clear();

            inskip = false;
            return true;

        }

        if (srcMLParser::LA(1) == srcMLParser::VISUAL_CXX_ASM) {

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
            pretb.insert(pretb.end(), std::make_move_iterator(skippretb.begin()), std::make_move_iterator(skippretb.end()));
            skippretb.clear();

            // move back to normal buffer
            pskiptb = &skiptb;
            pouttb = &tb;

            // put preprocessor buffer into skipped buffer
            skiptb.insert(skiptb.end(), std::make_move_iterator(pretb.begin()), std::make_move_iterator(pretb.end()));
            pretb.clear();

            // stop preprocessor handling
            inskip = false;

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
    inline void flushSkip() final {
        flushSkip(output());
    }

    /**
     * SkipBufferSize
     *
     * Delegate to get the number of the skipped elements.
     *
     * @returns the number of skipped elements
     */
    inline int SkipBufferSize() final {
        return (int)skiptb.size();
    }

    /**
     * flushSkip
     * @param rf list of antlr tokens
     *
     * Flush any skipped tokens to the output token stream.
     */
    inline void flushSkip(std::deque<antlr::RefToken>& rf) {

        rf.insert(rf.end(), std::make_move_iterator(skip().begin()), std::make_move_iterator(skip().end()));
        skip().clear();
    }

    inline void completeSkip() {

        if (!open_comments.empty()) {
            slastcolumn = LT(1)->getColumn() - 1;
            slastline = LT(1)->getLine();
            pushESkipToken(open_comments.top());
            open_comments.pop();
        }
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

        if (tb.empty())
            consume();

        while (paused)
            fillTokenBuffer();

        // to calculate end position, need to buffer until end token in reached
        // to prevent infinite loops, consume is called if no progress is made on the
        // current token, e.g., double max const();
        if (isoption(options, SRCML_OPTION_POSITION)) {
            auto curline = LT(1)->getLine();
            auto curcolumn = LT(1)->getColumn();
            while (ends.size() > 1) {
                fillTokenBuffer();
                if (LT(1)->getLine() == curline && LT(1)->getColumn() == curcolumn)
                    consume();
                curline = LT(1)->getLine();
                curcolumn = LT(1)->getColumn();
            }
        }

        // pop and send back the top token
        const antlr::RefToken& tok = std::move(tb.front());

        return tok;
    }

    /**
     * pushToken
     * @param rtoken token to push onto output stream
     *
     * Push the token onto the output token stream.
     */
    inline void pushToken(const antlr::RefToken& rtoken) {

        // don't push any tokens during guessing stage
        if (srcMLParser::inputState->guessing)
            return;

        // push the new token into the token buffer
        output().emplace_back(rtoken);
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
        output().emplace_back(rtoken);
    }

    /**
     * output
     *
     * Get output buffer list.
     *
     * @returns the output buffer.
     */
    inline std::deque<antlr::RefToken>& output() {
        return *pouttb;
    }

    /**
     * skip
     *
     * Get the skip buffer.
     *
     * @returns the skip buffer.
     */
    inline std::deque<antlr::RefToken>& skip() {
        return *pskiptb;
    }

    /**
     * pushSkipToken
     * @param rtoken a skip token to push
     *
     * Push the skipped token onto the output token stream
     */
    inline void pushSkipToken(const antlr::RefToken& rtoken) {

        // don't push any tokens during guessing stage
        if (srcMLParser::inputState->guessing)
            return;

        // push the new token into the token buffer
        skip().emplace_back(rtoken);
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
    inline void pushToken() {

        pushTokenFlush(srcMLParser::LT(1));
    }

    /**
     * pushSkipToken
     *
     * Push the current skip token onto the output token stream.
     */
    inline void pushSkipToken() {

        pushSkipToken(srcMLParser::LT(1));
    }

    /**
     * CurrentToken
     *
     * Get the current token (last added).
     *
     * @returns the current token (last added).
     */
    inline antlr::RefToken* CurrentToken() final {

        return &(output().back());
    }

    antlr::RefToken pausetoken = nullptr;

    /** abstract method for pausing the output of tokens */
    void pauseStream() final {
        pausetoken = *CurrentToken();
        paused = true;
    }

    /** abstract method for resuming the output of tokens */
    void resumeStream() final {
        pausetoken = nullptr;
        paused = false;
    }

    /** abstract method for indicating if the stream is paused */
    virtual bool isPaused() final {
        return paused;
    }

    /** abstract method for replacing start of stream with a NOP */
    void nopStreamStart() final {

        if (!paused)
            return;

        if (pausetoken->getType() != output().back()->getType())
            return;

        pausetoken->setType(SNOP);
        output().back()->setType(SNOP);

        resumeStream();
    }

private:

    // record position of text elements
    int lastline = 0;
    int lastcolumn = 0;

    // record position for comment elements
    int slastline = 0;
    int slastcolumn = 0;

    // record position for <type prev=""/>
    int lasttypeendline = 0;
    int lasttypeendcolumn = 0;
    int lasttypestartline = 0;
    int lasttypestartcolumn = 0;

    /** parser options */
    OPTION_TYPE & options;

    /** if in a skip */
    bool inskip = false;

    /** token buffer */
    std::deque<antlr::RefToken> tb;

    /** skipped token buffer */
    std::deque<antlr::RefToken> skiptb;

    /** preprocessor buffer */
    std::deque<antlr::RefToken> pretb;

    /** preprocessor skipped token buffer */
    std::deque<antlr::RefToken> skippretb;

    /** current token buffer */
    std::deque<antlr::RefToken>* pouttb;

    /** current skipped token buffer */
    std::deque<antlr::RefToken>* pskiptb;

    /** any output is paused */
    bool paused = false;

    /** open position elements */
    std::stack<antlr::RefToken> ends;

    /** open comments */
    std::stack<int> open_comments;
};

#endif
