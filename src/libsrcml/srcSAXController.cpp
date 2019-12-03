/**
 * @file srcSAXController.cpp
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

