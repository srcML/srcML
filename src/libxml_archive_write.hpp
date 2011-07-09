/*
  libxml_archive_write.hpp

  Copyright (C) 2010  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef INCLUDED_LIBXML_ARCHIVE_WRITE_H
#define INCLUDED_LIBXML_ARCHIVE_WRITE_H

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>

void archiveWriteOutputFormat(const char* format);

// check if archive matches the protocol on the URI
int archiveWriteMatch_src2srcml(const char * URI);

// check if archive matches the protocol on the URI
int archiveWriteMatch_srcml2src(const char * URI);

void* archiveWriteRootOpen(const char* URI);

// setup archive for this URI
void* archiveWriteOpen(const char * URI);

// read from the URI
int archiveWrite(void * context, const char * buffer, int len);

// close the open file
int archiveWriteClose(void * context);

int archiveWriteRootClose(void * context);

#endif
