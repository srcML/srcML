// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcsax.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_SRCSAX_HPP
#define INCLUDED_SRCSAX_HPP

#include <srcsax_handler.hpp>

#include <libxml/parser.h>
#include <libxml2_utilities.hpp>

/**
 * srcsax_context
 *
 * Context data structure passed between callbacks.
 */
class srcsax_context {
public:
    /** user provided data */
    void* data = nullptr;

    /** srcSAX handler callbacks */
    srcsax_handler* handler = nullptr;

    /** error callback need to figure this one out probably message and errorcode. or struct.  Might not need, but might be nice to avoid libxml2 stuff */
    void (*srcsax_error)(const char* message, int error_code);

    /** is the document an archive */
    bool is_archive;

    /** the xml documents encoding */
    const char* encoding = nullptr;

    /* Internal context handling NOT FOR PUBLIC USE */

    /** xml parser input buffer */
    std::unique_ptr<xmlParserInputBuffer> input;

    /** internally used libxml2 context */
    xmlParserCtxtPtr libxml2_context = nullptr;
};

/* srcSAX context creation/open functions */
srcsax_context* srcsax_create_context_parser_input_buffer(std::unique_ptr<xmlParserInputBuffer> input);

/* srcSAX free function */
void srcsax_free_context(srcsax_context * context);

/* srcSAX parse function */
int srcsax_parse(srcsax_context * context);

/* srcSAX terminate parse function */
void srcsax_stop_parser(srcsax_context* context);

#endif
