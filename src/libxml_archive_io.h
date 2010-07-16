#ifndef INCLUDED_LIBXML_ARCHIVE_IO_H
#define INCLUDED_LIBXML_ARCHIVE_IO_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

// check if path has an archive extension
bool isArchiveRead(const char * path);

// current file is an archive
bool isArchiveRead();

// format (e.g., tar, cpio) of the current file
const char* archiveReadFormat();

// compression (e.g., gz, bzip2) of the current file
const char* archiveReadCompression();

// check if archive matches the protocol on the URI
int archiveReadMatch(const char * URI);

// check if archive matches the protocol on the URI
int archiveWriteMatch(const char * URI);

const char* archiveReadFilename(const char* URI);

// setup archive root for this URI
int archiveReadStatus();

// setup archive for this URI
void* archiveReadOpen(const char * URI);

// read from the URI
int archiveRead(void * context, char * buffer, int len);

// close the open file
int archiveReadClose(void * context);

// setup archive root for this URI
int archiveReadStatus();

// setup archive for this URI
void* archiveWriteOpen(const char * URI);

// read from the URI
int archiveWrite(void * context, char * buffer, int len);

// close the open file
int archiveWriteClose(void * context);


#endif
