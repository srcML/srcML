/*
 * xmlsave.c: Implemetation of the document serializer
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parserInternals.h>
#include <libxml/tree.h>
#include <libxml/xmlsave.h>

xmlOutputBufferPtr xmlSaveGetBuffer(xmlSaveCtxtPtr ctxt);
