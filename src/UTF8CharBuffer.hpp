/*

  UTF8CharBuffer.hpp

  Copyright (C) 2008-2010  SDML (www.sdml.info)

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

  CharBuffer for antlr that uses a libxml input routines.
  This provides for any necessary encoding conversion to UTF8,
  so using libxml for XML output is direct.

  libxml stores data internally in UTF*
*/

#ifndef INCLUDE_UTF8CHARBUFFER_HPP
#define INCLUDE_UTF8CHARBUFFER_HPP

#include <antlr/CharBuffer.hpp>
#include <libxml/parser.h>
#include <cstring>

class UTF8FileError {};

class UTF8CharBuffer : public antlr::CharBuffer {
public:

  // size of the original character buffer
  static const unsigned int SRCBUFSIZE = 512;

  // Create a character buffer
  UTF8CharBuffer(const char* ifilename, const char* encoding);

  // Get the next character from the stream
  int getChar();

  // libxml context
  void* getContext() const;

  ~UTF8CharBuffer();

private:
  int pos;
  int size;
  bool eof;
  bool lastcr;
  xmlParserInputBufferPtr input;
};
#endif
