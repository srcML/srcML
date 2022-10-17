// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcSAXController.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#include <srcSAXController.hpp>
#include <srcSAXHandler.hpp>
#include <cppCallbackAdapter.hpp>

#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * srcSAXController
 * @param input a parser input buffer
 *
 * Constructor
 */
srcSAXController::srcSAXController(std::unique_ptr<xmlParserInputBuffer> input) {

    context = srcsax_create_context_parser_input_buffer(std::move(input));
    if (context == NULL)
        throw std::string("File does not exist");
}

/**
 * ~srcSAXController
 *
 * Constructor
 */
srcSAXController::~srcSAXController() {

    if (context)
        srcsax_free_context(context);
}

/**
 * getCtxt
 *
 * Return the used parser context.
 */
srcsax_context * srcSAXController::getContext() {

    return context;
}

/**
 * parse
 * @param handler srcMLHandler with hooks for sax parsing
 *
 * Parse the xml document with the supplied hooks.
 */
void srcSAXController::parse(srcSAXHandler * handler) {

    handler->set_controller(this);

    cppCallbackAdapter adapter(handler);
    context->data = &adapter;
    srcsax_handler sax_handler = cppCallbackAdapter::factory();
    context->handler = &sax_handler;

    int status = srcsax_parse(context);

    context->data = 0;

    if (status != 0) {

        xmlErrorPtr ep = xmlCtxtGetLastError(context->libxml2_context);
        SAXError error = { std::string(ep->message), ep->code };

        throw error;
    }
}

