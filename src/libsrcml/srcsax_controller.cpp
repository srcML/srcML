// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcsax_controller.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#include <srcsax.hpp>
#include <sax2_srcsax_handler.hpp>

#include <libxml/parserInternals.h>

#include <functional>

/**
 * libxml_error
 *
 * Silence/catch/default libxml2 errors.
 */
static void libxml_error(void * /*ctx*/, const char* msg, ...) {

    va_list vl;
    va_start(vl, msg);
    vfprintf(stderr, msg, vl);
    va_end(vl);
}

/* srcsax_create_parser_context forward declaration */
static xmlParserCtxtPtr srcsax_create_parser_context(xmlParserInputBufferPtr buffer_input, xmlCharEncoding enc);

/**
 * srcsax_create_context_parser_input_buffer
 * @param srcml_context an opened context for opened srcML document
 * @param read_callback a read callback function
 * @close_callback a close callback function
 * @param encoding the files character encoding
 *
 * Create a srcsSAX context from a general context and read/close callbacks with the specified encoding.
 *
 * @returns srcsax_context context to be used for srcML parsing.
 */
srcsax_context* srcsax_create_context_parser_input_buffer(std::unique_ptr<xmlParserInputBuffer> input) {

    if (!input)
        return 0;

    const char* encoding = nullptr;

    srcsax_context* context = nullptr;
    try {
        context = new srcsax_context();
    } catch (...) {
        return 0;
    }

    context->input = std::move(input);

    xmlParserCtxtPtr libxml2_context = srcsax_create_parser_context(context->input.release(), encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);
    if (libxml2_context == nullptr) {
        delete context;
        return 0;
    }

    xmlGenericErrorFunc error_handler = (xmlGenericErrorFunc) libxml_error;
    xmlSetGenericErrorFunc(libxml2_context, error_handler);

    context->libxml2_context = libxml2_context;

    return context;
}

/**
 * srcsax_free_context
 * @param context a srcSAX context
 *
 * Free the resources associated with a srcsax_context as created
 * by a previous srcsax_create_context_*.
 */
void srcsax_free_context(srcsax_context* context) {

    if (context == 0)
        return;

    if (context->libxml2_context)
        xmlFreeParserCtxt(context->libxml2_context);

    delete context;
}

/**
 * srcsax_parse
 * @param context srcSAX context
 *
 * Parse the context using the provide sax handlers.
 * On error calls the error callback function before returning.
 *
 * @returns 0 on success -1 on error.
 */
int srcsax_parse(srcsax_context* context) {

    if (context == 0 || context->handler == 0)
        return -1;

    xmlSAXHandlerPtr save_sax = context->libxml2_context->sax;

    xmlSAXHandler sax = srcsax_sax2_factory();
    context->libxml2_context->sax = &sax;

    sax2_srcsax_handler state;
    state.context = context;
    context->libxml2_context->_private = &state;

    int status = xmlParseDocument(context->libxml2_context);

    context->libxml2_context->sax = save_sax;

    if (status != 0 && context->srcsax_error) {

        xmlErrorPtr ep = xmlCtxtGetLastError(context->libxml2_context);

        auto str_length = std::string_view(ep->message).size();
        ep->message[str_length - 1] = '\0';

        context->srcsax_error((const char *)ep->message, ep->code);
    }

    return status;
}

/**
 * srcsax_create_parser_context
 * @param buffer_input a parser input buffer
 *
 * Create a ctxt from a parser input buffer.
 * Modeled after function in libxml2.
 *
 * @returns xml parser ctxt
 */
xmlParserCtxtPtr srcsax_create_parser_context(xmlParserInputBufferPtr buffer_input, xmlCharEncoding enc) {

    if (buffer_input == 0)
        return 0;

    xmlParserCtxtPtr ctxt = xmlNewParserCtxt();
    if (ctxt == 0)
        return 0;

    xmlCtxtUseOptions(ctxt, XML_PARSE_COMPACT | XML_PARSE_HUGE | XML_PARSE_NODICT);

    xmlParserInputPtr input = xmlNewIOInputStream(ctxt, buffer_input, enc);
    if (input == 0) {
        xmlFreeParserCtxt(ctxt);
        return 0;
    }

    inputPush(ctxt, input);

    return ctxt;
}

/**
 * srcsax_stop_parser
 * @param context a srcSAX context
 *
 * Stop srcSAX parser.
 */
void srcsax_stop_parser(srcsax_context* context) {

    xmlParserCtxtPtr ctxt = context->libxml2_context;

    ctxt->sax->startDocument = 0;
    ctxt->sax->endDocument = 0;
    ctxt->sax->startElementNs = 0;
    ctxt->sax->endElementNs = 0;
    ctxt->sax->characters = 0;
    ctxt->sax->cdataBlock = 0;
    ctxt->sax->comment = 0;
    ctxt->sax->ignorableWhitespace = 0;

    xmlStopParser(ctxt);
}
