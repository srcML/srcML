#ifndef INCLUDED_LIBXML_ARCHIVE_IO_H
#define INCLUDED_LIBXML_ARCHIVE_IO_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

// check if archive matches the protocol on the URI
int archiveMatch(const char * URI);

// setup archive root for this URI
void* archiveOpenRoot(const char * URI);

// setup archive root for this URI
bool archiveGood();

// setup archive for this URI
void* archiveOpen(const char * URI);

// read from the URI
int archiveRead(void * context, char * buffer, int len);

// close the open file
int archiveClose(void * context);

// close the open root file
int archiveCloseRoot(void * context);

#endif
