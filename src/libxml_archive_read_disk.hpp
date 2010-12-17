#ifndef INCLUDED_LIBXML_ARCHIVE_READ_DISK_H
#define INCLUDED_LIBXML_ARCHIVE_READ_DISK_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

// check if path has an archive extension
bool isArchiveReadDisk(const char * path);

// current file is an archive
bool isArchiveReadDisk();

// format (e.g., tar, cpio) of the current file
const char* archiveReadDiskFormat();

// compression (e.g., gz, bzip2) of the current file
const char* archiveReadDiskCompression();

// check if archive matches the protocol on the URI
int archiveReadDiskMatch(const char * URI);

const char* archiveReadDiskFilename(const char* URI);

// setup archive root for this URI
int archiveReadDiskStatus();

// setup archive for this URI
void* archiveReadDiskOpen(const char * URI);

// read from the URI
int archiveReadDisk(void * context, char * buffer, int len);

// close the open file
int archiveReadDiskClose(void * context);

// setup archive root for this URI
int archiveReadDiskStatus();

#endif
