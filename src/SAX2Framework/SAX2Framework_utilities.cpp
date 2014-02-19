/**
 * @file SAX2Framework_utilities.cpp
 * @copyright
 *
 * Copyright (C) 2013  SDML (www.srcML.org)
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

#include <SAX2Framework_utilities.hpp>

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
 * @returns 0  on success and -1 on error.
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
 * srcMLCreateParserCtxt
 * @param buffer_input a parser input buffer
 *
 * Create a ctxt from a parser input buffer.
 * Modeled after function in libxml2.
 *
 * @returns xml parser ctxt
 */
xmlParserCtxtPtr
SAX2FrameworkCreateParserCtxt(xmlParserInputBufferPtr buffer_input) {
    xmlParserCtxtPtr ctxt;
    xmlParserInputPtr input;
    xmlParserInputBufferPtr buf;

    ctxt = xmlNewParserCtxt();
    if (ctxt == NULL)
        return(NULL);

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

void internal_stop_parser(xmlParserCtxtPtr ctxt) {

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
