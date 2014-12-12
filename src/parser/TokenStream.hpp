/**
 * @file TokenStream.hpp
 *
 * @copyright Copyright (C) 2004-2010 srcML, LLC. (www.srcML.org)
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
 */

/**
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
