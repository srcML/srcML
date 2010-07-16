#ifndef INCLUDED_LIBXML_ARCHIVE_READ_HTTP_H
#define INCLUDED_LIBXML_ARCHIVE_READ_HTTP_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

// check if path has an archive extension
bool isArchiveReadHttp(const char * path);

// current file is an archive
bool isArchiveReadHttp();

// format (e.g., tar, cpio) of the current file
const char* archiveReadHttpFormat();

// compression (e.g., gz, bzip2) of the current file
const char* archiveReadHttpCompression();

// check if archive matches the protocol on the URI
int archiveReadHttpMatch(const char * URI);

const char* archiveReadHttpFilename(const char* URI);

// setup archive root for this URI
int archiveReadHttpStatus();

// setup archive for this URI
void* archiveReadHttpOpen(const char * URI);

// read from the URI
int archiveReadHttp(void * context, char * buffer, int len);

// close the open file
int archiveReadHttpClose(void * context);

// setup archive root for this URI
int archiveReadHttpStatus();

#endif
