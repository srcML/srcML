#ifndef INCLUDED_LIBXML_ARCHIVE_IO_H
#define INCLUDED_LIBXML_ARCHIVE_IO_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

// check if path has an archive extension
bool isArchive(const char * path);

// current file is an archive
bool isArchive();

// format (e.g., tar, cpio) of the current file
const char* archiveFormat();

// compression (e.g., gz, bzip2) of the current file
const char* archiveCompression();

// check if archive matches the protocol on the URI
int archiveMatch(const char * URI);


const char* archiveFilename(const char* URI);

// setup archive root for this URI
int archiveStatus();

// setup archive for this URI
void* archiveOpen(const char * URI);

// read from the URI
int archiveRead(void * context, char * buffer, int len);

// close the open file
int archiveClose(void * context);

#endif
