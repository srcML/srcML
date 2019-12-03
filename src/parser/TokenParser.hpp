/**
 * @file TokenParser.hpp
 *
 * @copyright Copyright (C) 2003-2019 srcML, LLC. (www.srcML.org)
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
 */

/**
 * Adds methods for Island grammar parsing for antlr::Parser.
 * Is included in the desired passthrough parser grammar.
 *
 * Done because antlr doesn't really support inheritance only
 * grammar inheritance
 *
 * In order to use:
 *
 * Put a @code#include "tokenparser.h"@endcode in your grammar file
 *
 * Define a symbol(?) start.  Include in it all the cases
 * that you want to use:
 * start : (case1 | case2)
 * ;
 *
 * Any tokens that don't match a case reachable from start
 * will be passed along unchanged
 *
 * All whitespace tokens, WS, are passed along unchanged
 *
 * Will be overridden in stream parser version.
 */

#ifndef TOKENPARSER_HPP
#define TOKENPARSER_HPP

#include <iostream>
#include <antlr/TokenStream.hpp>
#include <list>

/**
 * TokenParser
 *
 * Interface for token parsing.
 */
class TokenParser {

public:

    /** abstract method for starting an element */
    virtual void startElement(int) = 0;

    /** abstract method for starting an element that starts before skipped tokens */
    virtual void startNoSkipElement(int) = 0;

    /** abstract method for ending an element */
    virtual void endElement(int) = 0;

    /** abstract method for starting an empty element */
    virtual void emptyElement(int) = 0;

    /** abstract method for starting adding an element */
    virtual void addElement(int) = 0;

    /** abstract method for completing any unended skipped elements */
    virtual void completeSkip() = 0;

    /** abstract method for flushing skipped */
    virtual void flushSkip() = 0;

    /** abstract method for SkipBufferSize */
    virtual int SkipBufferSize() = 0;

    /** abstract method for Current token */
    virtual antlr::RefToken* CurrentToken() = 0;

    /** abstract method for pausing the output of tokens */
    virtual void pauseStream() = 0;

    /** abstract method for resuming the output of tokens */
    virtual void resumeStream() = 0;

    /** abstract method for indicating if the stream is paused */
    virtual bool isPaused() = 0;

    /** abstract method for replacing start of stream with a NOP */
    virtual void nopStreamStart() = 0;

    /**
     * ~TokenParser
     *
     * Destructor.
     */
    virtual ~TokenParser() {}

};

#endif
