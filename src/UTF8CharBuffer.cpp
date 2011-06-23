/*
  UTF8CharBuffer.cpp

  Copyright (C) 2008-2011  SDML (www.sdml.info)                                                                                                                                                

  This file is part of the srcML translator.                                                                                                                                                   

  The srcML translator is free software; you can redistribute it and/or modify                                                                                                                 
  it under the terms of the GNU General Public License as published by                                                                                                                           the Free Software Foundation; either version 2 of the License, or                                                                                                                              (at your option) any later version.                                                                                                                                                                                                                                                                                                                                                           The srcML translator is distributed in the hope that it will be useful,                                                                                                                        but WITHOUT ANY WARRANTY; without even the implied warranty of                                                                                                                                 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                                                                                                                  GNU General Public License for more details.                                                                                                                                                                                                                                                                                                                                                  You should have received a copy of the GNU General Public License                                                                                                                              along with the srcML translator; if not, write to the Free Software                                                                                                                            Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <iostream>
#include "UTF8CharBuffer.hpp"
#include <libxml/encoding.h>

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(const char* ifilename, const char* encoding)
  : antlr::CharBuffer(std::cin), pos(0), size(0), eof(false), lastcr(false)
{
  // use a libxml2 parser input buffer to support URIs
  if (!(input = xmlParserInputBufferCreateFilename(ifilename, xmlParseCharEncoding(encoding))))
    throw UTF8FileError();
}

// libxml context
void* UTF8CharBuffer::getContext() const {

  return input->context;
}

/*
 Get the next character from the stream

 Grab characters one byte at a time from the input stream and place
 them in the original source encoding buffer.  Then convert from the
 original encoding to UTF-8 in the utf8 buffer.
*/
int UTF8CharBuffer::getChar() {

  // need to refill the buffer
  if (size == 0 || pos >= size) {

    // refill the buffer
    input->buffer->use = 0;
    size = xmlParserInputBufferGrow(input, SRCBUFSIZE);
	
    // found problem or eof
    if (size == -1 || size == 0)
      return -1;

    // start at the beginning
    pos = 0;
  }

  // individual 8-bit character to return
  int c = (int) input->buffer->content[pos++];

  // sequence "\r\n" where the '\r'
  // has already been converted to a '\n' so we need to skip over this '\n'
  if (lastcr && c == '\n') {
    lastcr = false;

    // might need to refill the buffer
    if (pos >= size) {

      // refill the buffer
      input->buffer->use = 0;
      size = xmlParserInputBufferGrow(input, SRCBUFSIZE);
	
      // found problem or eof
      if (size == -1 || size == 0)
        return -1;

      // start at the beginning
      pos = 0;
    }

    // certain to have a character
    c = (int) input->buffer->content[pos++];
  }

  // convert carriage returns to a line feed
  if (c == '\r') {
    lastcr = true;
    c = '\n';
  }

  return c;
}

UTF8CharBuffer::~UTF8CharBuffer() {

  xmlFreeParserInputBuffer(input);
}
