// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TokenStream.hpp
 *
 * @copyright Copyright (C) 2004-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Adds stream parsing capabilities to a Parser class.
 *
 * Fundamentally it provides a nextToken method so that another Parser
 * can use this as a TokenStream.
 *
 * A new class is created with the template parameter Base and
 * TokenStream as base classes.  This allows other Parsers to use it as
 * a TokenStream in their constructor.
 *
 * It also implements the TokenStream nextToken method and provides
 * pushToken methods to fill the token buffer.
 *
 * Parser grammars that use this class must also inherit from TokenParser.hpp
 */

#ifndef INCLUDED_SRCMLTOKENSTREAM_HPP
#define INCLUDED_SRCMLTOKENSTREAM_HPP

#include <antlr/TokenStream.hpp>

/**
 * TokenStream
 *
 * Class for a token stream.
 */
class TokenStream {

public:

    /** abstract method for getting next token */
    virtual const antlr::RefToken& nextToken() = 0;

    /**
     * ~TokenStream
     *
     * Destructor.
     */
    virtual ~TokenStream() {}

};

#endif
