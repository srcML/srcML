#ifndef INCLUDED_LIBXML_ARCHIVE_READ_H
#define INCLUDED_LIBXML_ARCHIVE_READ_H

// current file is an archive
bool isArchiveRead(void* context);

bool isAnythingOpen(void* context);

bool isArchiveFirst(void* context);

// format (e.g., tar, cpio) of the current file
const char* archiveReadFormat(void* context);

// compression (e.g., gz, bzip2) of the current file
const char* archiveReadCompression(void* context);

// check if archive matches the protocol on the URI
int archiveReadMatch(const char* URI);

const char* archiveReadFilename(void* context);

// setup archive root for this URI
int archiveReadStatus(void* context);

bool archiveIsDir(void* context);

// setup archive for this URI
void* archiveReadOpen(const char* URI);

// read from the URI
int archiveRead(void* context, char* buffer, int len);

// close the open file
int archiveReadClose(void* context);

// close the open file
int archiveReadClose(void* context);

// setup archive root for this URI
int archiveReadStatus(void* context);

#endif
