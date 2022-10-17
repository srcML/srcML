// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcSAXController.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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
