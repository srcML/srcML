/*
  UTF8CharBuffer.cpp

  Copyright (C) 2008-2011  SDML (www.sdml.info)

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

#include <iostream>
#include "UTF8CharBuffer.hpp"
#include <libxml/encoding.h>

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(const char* ifilename, const char* encoding)
  : antlr::CharBuffer(std::cin), pos(0), size(0), eof(false), lastcr(false)
{
  const char* enc = encoding;

  /* Use a libxml2 parser input buffer to support URIs.
     If an encoding is specified, then use it.  Otherwise, assume none, and
     try to figure it out later.
  */
  if (!(input = xmlParserInputBufferCreateFilename(ifilename,
                                                   enc ? xmlParseCharEncoding(enc) : XML_CHAR_ENCODING_NONE)))
    throw UTF8FileError();

  /* If an encoding was not specified, then try to detect it.
     This is especially important for the BOM for UTF-8.
     If nothing is detected, then use ISO-8859-1 */
  if (!encoding) {

    // assume ISO-8859-1 unless we can detect it otherwise
    xmlCharEncoding denc = XML_CHAR_ENCODING_8859_1;

    // input enough characters to detect.
    // 4 is good because you either get 4 or some standard size which is probably larger (really)
    size = xmlParserInputBufferGrow(input, 4);

    int shrink = 0;

    // detect (and remove) BOMs for UTF8 and UTF16
    if (size >= 3 &&
        input->buffer->content[0] == 0xEF &&
        input->buffer->content[1] == 0xBB &&
        input->buffer->content[2] == 0xBF) {

      denc = XML_CHAR_ENCODING_UTF8;
      shrink = 3;

    } else if (size >= 2 &&
               input->buffer->content[0] == 0xFE &&
               input->buffer->content[1] == 0xFF) {

      denc = XML_CHAR_ENCODING_UTF16BE;
      shrink = 2;

    } else if (size >= 2 &&
               input->buffer->content[0] == 0xFF &&
               input->buffer->content[1] == 0xFE) {

      denc = XML_CHAR_ENCODING_UTF16LE;
      shrink = 2;
    }

    /* Transform the data already read in */

    // since original encoding was NONE, no raw buffer was allocated, so use the regular buffer
    //    if (shrink)
    //      xmlBufferShrink(input->buffer, shrink);
    pos = shrink;
    input->raw = input->buffer;
    input->rawconsumed = 0;

    // need a new regular buffer
    input->buffer = xmlBufferCreate();

    // setup the encoder being used
    input->encoder = xmlGetCharEncodingHandler(denc);

    // convert the characters to the new encoding
    int nbchars = xmlCharEncInFunc(input->encoder, input->buffer, input->raw);
  }
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
