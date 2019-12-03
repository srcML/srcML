    /**
 * @file srcSAXController.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 *
 * srcSAX is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * srcSAX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef INCLUDED_SRCSAX_CONTROLLER_HPP
#define INCLUDED_SRCSAX_CONTROLLER_HPP

class srcSAXHandler;
#include <srcsax.hpp>

#include <libxml/parser.h>
#include <libxml/parserInternals.h>

#include <string>

/**
 * SAXError
 *
 * Data struct to hold contents of an error.
 */
struct SAXError {

    /** error message */
    std::string message;

    /** error code */
    int error_code;
};

/**
 * srcSAXController
 *
 * Provides execution of sax with
 * given hooks.
 */
class srcSAXController {

private :

    // xmlParserCtxt
    srcsax_context* context = nullptr;

public :

    /**
     * srcSAXController
     * @param input a parser input buffer
     *
     * Constructor
     */
    srcSAXController(std::unique_ptr<xmlParserInputBuffer> input);

    /**
     * getCtxt
     *
     * Return the used parser context.
     */
    srcsax_context* getContext();

    /**
     * ~srcSAXController
     *
     * Destructor
     */
    ~srcSAXController();

    /**
     * parse
     * @param handler srcMLHandler with hooks for sax parsing
     *
     * Parse the xml document with the supplied hooks.
     */
    void parse(srcSAXHandler * handler);

    /**
     * stop_parser
     *
     * Stop parsing.
     */
    void stop_parser();
};

#endif
