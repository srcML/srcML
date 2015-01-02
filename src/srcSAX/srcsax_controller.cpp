/**
 * @file srcsax_controller.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <srcsax.h>
#include <sax2_srcsax_handler.hpp>

#include <libxml/parserInternals.h>

#include <cstring>

/** 
 * libxml_error
 *
 * Silence/catch/default libxml2 errors.
 */
static void libxml_error(void * /*ctx*/, const char * /*msg*/, ...) {}

/* srcsax_create_parser_context forward declaration */
static xmlParserCtxtPtr srcsax_create_parser_context(xmlParserInputBufferPtr buffer_input);

#ifdef LIBXML2_NEW_BUFFER
struct _xmlBuf {
    xmlChar *content;           /* The buffer content UTF8 */
    unsigned int compat_use;    /* for binary compatibility */
    unsigned int compat_size;   /* for binary compatibility */
    xmlBufferAllocationScheme alloc; /* The realloc method */
    xmlChar *contentIO;         /* in IO mode we may have a different base */
    size_t use;                 /* The buffer size used */
    size_t size;                /* The buffer size */
    xmlBufferPtr buffer;        /* wrapper for an old buffer */
    int error;                  /* an error code if a failure occured */
};

#define _CHECK_COMPAT(buf)                      \
    if (buf->size != (size_t) buf->compat_size) \
        if (buf->compat_size < INT_MAX)         \
            buf->size = buf->compat_size;       \
    if (buf->use != (size_t) buf->compat_use)   \
        if (buf->compat_use < INT_MAX)          \
            buf->use = buf->compat_use;

/**
 * xmlBufResetInput
 * @param buf XML buffer
 * @param input XML parser input
 *
 * Function is taken from libxml2.
 *
 * @returns 0 on success and -1 on error.
 */
int
_xmlBufResetInput(xmlBuf * buf, xmlParserInputPtr input) {
    if ((input == NULL) || (buf == NULL) || (buf->error))
        return(-1);
    _CHECK_COMPAT(buf)
        input->base = input->cur = buf->content;
    input->end = &buf->content[buf->use];
    return(0);

}
#else
/**
 * xmlBufResetInput
 * @param buf XML buffer
 * @param input XML parser input
 *
 * Function is taken fro libxml2.
 *
 * @returns 0 on success and -1 on error.
 */
int
_xmlBufResetInput(xmlBuffer * buf, xmlParserInputPtr input) {
    if ((input == NULL) || (buf == NULL))
        return -1;
    input->base = input->buf->buffer->content;
    input->cur = input->buf->buffer->content;
    input->end = &input->buf->buffer->content[input->buf->buffer->use];
    return 0;
}

#endif

/**
 * srcsax_controller_init
 *
 * Internal method to initialize the controller module.
 */
static void srcsax_controller_init() {

    static bool initialized = false;

    if(initialized) return;

    xmlGenericErrorFunc error_handler = (xmlGenericErrorFunc) libxml_error;
    initGenericErrorDefaultFunc(&error_handler);
    initialized = true;

}

/**
 * srcsax_create_context_inner
 * @param input a libxml2 parser input buffer
 *
 * A helper function that creates the srcSAX context and does error handling.
 * With a supplied xmlParserInputBufferPtr.
 * 
 * @returns srcsax_context context to be used for srcML parsing.
 */
static struct srcsax_context * srcsax_create_context_inner(xmlParserInputBufferPtr input, int free_input) {

    if(input == 0) return 0;

    struct srcsax_context * context = (struct srcsax_context *)malloc(sizeof(struct srcsax_context));

    if(context == 0) {

        xmlFreeParserInputBuffer(input);
        return 0;

    }

    memset(context, 0, sizeof(struct srcsax_context));
    context->input = input;
    context->free_input = free_input;

    xmlParserCtxtPtr libxml2_context = srcsax_create_parser_context(context->input);

    if(libxml2_context == NULL) {

        xmlFreeParserInputBuffer(input);
        free(context);
        return 0;

    }

    /** @todo this does not make sense */
    libxml2_context->_private = context;

    context->libxml2_context = libxml2_context;

    context->terminate = 0;

    return context;

}

/**
 * srcsax_create_context_filename
 * @param filename a filename
 * @param encoding the files character encoding
 *
 * Open the filename with the specified encoding and return a srcSAX context for parsing.
 *
 * @returns srcsax_context context to be used for srcML parsing.
 */
struct srcsax_context * srcsax_create_context_filename(const char * filename, const char * encoding) {

    if(filename == 0) return 0;

    srcsax_controller_init();

    xmlParserInputBufferPtr input =
        xmlParserInputBufferCreateFilename(filename, encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    return srcsax_create_context_inner(input, 1);

}

/**
 * srcsax_create_context_memory
 * @param buffer a buffer of memory
 * @param buffer_size the size of the buffer/amount of buffer to use
 * @param encoding the files character encoding
 *
 * Create a srcsSAX context from the supplied buffer using the provided amount and encoding.
 *
 * @returns srcsax_context context to be used for srcML parsing.
 */
struct srcsax_context * srcsax_create_context_memory(const char * buffer, size_t buffer_size, const char * encoding) {

    if(buffer == 0 || buffer_size == 0) return 0;

    srcsax_controller_init();

    xmlParserInputBufferPtr input =
        xmlParserInputBufferCreateMem(buffer, (int)buffer_size, encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    return srcsax_create_context_inner(input, 1);

}

/**
 * srcsax_create_context_FILE
 * @param srcml_file an opened file containing srcML
 * @param encoding the files character encoding
 *
 * Create a srcsSAX context from the supplied FILE using the provided encoding.
 *
 * @returns srcsax_context context to be used for srcML parsing.
 */
struct srcsax_context * srcsax_create_context_FILE(FILE * srcml_file, const char * encoding) {

    if(srcml_file == 0) return 0;

    srcsax_controller_init();

    xmlParserInputBufferPtr input =
        xmlParserInputBufferCreateFile(srcml_file, encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    return srcsax_create_context_inner(input, 1);

}

/**
 * srcsax_create_context_fd
 * @param srcml_fd an opened file descriptor containing srcML
 * @param encoding the files character encoding
 *
 * Create a srcsSAX context from the supplied file descriptor using the provided encoding.
 *
 * @returns srcsax_context context to be used for srcML parsing.
 */
struct srcsax_context * srcsax_create_context_fd(int srcml_fd, const char * encoding) {

    if(srcml_fd < 0) return 0;

    srcsax_controller_init();

    xmlParserInputBufferPtr input =
        xmlParserInputBufferCreateFd(srcml_fd, encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    return srcsax_create_context_inner(input, 1);

}

/**
 * srcsax_create_context_io
 * @param srcml_context an opened context for opened srcML document
 * @param read_callback a read callback function
 * @close_callback a close callback function
 * @param encoding the files character encoding
 *
 * Create a srcsSAX context from a general context and read/close callbacks with the specified encoding.
 *
 * @returns srcsax_context context to be used for srcML parsing.
 */
struct srcsax_context * srcsax_create_context_io(void * srcml_context, int (*read_callback)(void * context, char * buffer, int len), int (*close_callback)(void * context), const char * encoding) {

    if(srcml_context == 0 || read_callback == 0) return 0;

    srcsax_controller_init();

    xmlParserInputBufferPtr input =
        xmlParserInputBufferCreateIO(read_callback, close_callback, srcml_context, encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    return srcsax_create_context_inner(input, 1);

}

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
struct srcsax_context * srcsax_create_context_parser_input_buffer(xmlParserInputBufferPtr input) {

    if(input == 0) return 0;

    srcsax_controller_init();

    return srcsax_create_context_inner(input, 0);

}

/**
 * srcsax_free_context
 * @param context a srcSAX context
 *
 * Free the resources associated with a srcsax_context as created
 * by a previous srcsax_create_context_*.
 */
void srcsax_free_context(struct srcsax_context * context) {

    if(context == 0) return;

    xmlParserInputPtr stream = inputPop(context->libxml2_context);
    stream->buf = 0;
    xmlFreeInputStream(stream);
    if(context->libxml2_context) xmlFreeParserCtxt(context->libxml2_context);
    if(context->free_input) xmlFreeParserInputBuffer(context->input);

    free(context);

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
int srcsax_parse(struct srcsax_context * context) {

    if(context == 0 || context->handler == 0) return -1;

    xmlSAXHandlerPtr save_sax = context->libxml2_context->sax;
    xmlSAXHandler sax = srcsax_sax2_factory();
    context->libxml2_context->sax = &sax;

    sax2_srcsax_handler state;
    state.context = context;
    context->libxml2_context->_private = &state;

    int status = xmlParseDocument(context->libxml2_context);

    context->libxml2_context->sax = save_sax;

    if(status != 0) {

        xmlErrorPtr ep = xmlCtxtGetLastError(context->libxml2_context);

        size_t str_length = strlen(ep->message);
        ep->message[str_length - 1] = '\0';

        if(context->srcsax_error)
            context->srcsax_error((const char *)ep->message, ep->code);

    }

    return status;

}

/**
 * srcsax_parse
 * @param context srcSAX context
 * @param handler sax callback handlers
 *
 * Parse the context using the provide sax handlers.
 * On error calls the error callback function before returning.
 *
 * @returns 0 on success -1 on error.
 */
int srcsax_parse_handler(struct srcsax_context * context, struct srcsax_handler * handler) {

    if(context == 0) return -1;

    context->handler = handler;

    return srcsax_parse(context);

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
xmlParserCtxtPtr
srcsax_create_parser_context(xmlParserInputBufferPtr buffer_input) {
    xmlParserCtxtPtr ctxt;
    xmlParserInputPtr input;
    xmlParserInputBufferPtr buf;

    ctxt = xmlNewParserCtxt();
    if (ctxt == NULL)
        return(NULL);

    xmlCtxtUseOptions(ctxt, XML_PARSE_COMPACT | XML_PARSE_HUGE | XML_PARSE_NODICT);

    buf = buffer_input;
    if (buf == NULL) {
        xmlFreeParserCtxt(ctxt);
        return(NULL);
    }

    input = xmlNewInputStream(ctxt);
    if (input == NULL) {
        xmlFreeParserCtxt(ctxt);
        return(NULL);
    }

    input->filename = NULL;
    input->buf = buf;
    _xmlBufResetInput(input->buf->buffer, input);

    inputPush(ctxt, input);

    return(ctxt);
}

/**
 * srcsax_stop_parser
 * @param context a srcSAX context
 *
 * Stop srcSAX parser.
 */
void srcsax_stop_parser(struct srcsax_context * context) {

    context->terminate = 1;

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
