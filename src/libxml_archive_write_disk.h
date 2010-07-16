#ifndef INCLUDED_LIBXML_ARCHIVE_WRITE_H
#define INCLUDED_LIBXML_ARCHIVE_WRITE_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

// check if archive matches the protocol on the URI
int archiveWriteMatch(const char * URI);

void* archiveWriteRootOpen(const char* URI);

// setup archive for this URI
void* archiveWriteOpen(const char * URI);

// read from the URI
int archiveWrite(void * context, const char * buffer, int len);

// close the open file
int archiveWriteClose(void * context);

int archiveWriteRootClose(void * context);

#endif
