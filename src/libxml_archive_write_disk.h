#ifndef INCLUDED_LIBXML_ARCHIVE_WRITE_DISK_H
#define INCLUDED_LIBXML_ARCHIVE_WRITE_DISK_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

// check if archive matches the protocol on the URI
int archiveWriteDiskMatch(const char * URI);

void* archiveWriteDiskRootOpen(const char* URI);

// setup archive for this URI
void* archiveWriteDiskOpen(const char * URI);

// read from the URI
int archiveWriteDisk(void * context, const char * buffer, int len);

// close the open file
int archiveWriteDiskClose(void * context);

int archiveWriteDiskRootClose(void * context);

#endif
