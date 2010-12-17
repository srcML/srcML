#ifndef INCLUDED_LIBXML_CURL_IO_H
#define INCLUDED_LIBXML_CURL_IO_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

/**
 * sqlMatch:
 * @URI: an URI to test
 *
 * Check for an sql: query
 *
 * Returns 1 if yes and 0 if another Input module should be used
 */
int curlMatch(const char * URI);

/**
 * curlOpen:
 * @URI: an URI to test
 *
 * Return a pointer to the curl: query handler, in this example simply
 * the current pointer...
 *
 * Returns an Input context or NULL in case or error
 */
void* curlOpen(const char * URI);

/**
 * curlClose:
 * @context: the read context
 *
 * Close the curl: query handler
 *
 * Returns 0 or -1 in case of error
 */
int curlClose(void * context);

/**
 * curlRead:
 * @context: the read context
 * @buffer: where to store data
 * @len: number of bytes to read
 *
 * Implement an curl: query read.
 *
 * Returns the number of bytes read or -1 in case of error
 */
int curlRead(void * context, char * buffer, int len);

#endif
