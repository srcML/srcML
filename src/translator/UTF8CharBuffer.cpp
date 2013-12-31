/*
  UTF8CharBuffer.cpp

  Copyright (C) 2008-2013  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <iostream>
#include "UTF8CharBuffer.hpp"
#include <libxml/encoding.h>

#ifndef LIBXML2_NEW_BUFFER
#define xmlBufContent(b) (b->content)
#endif


UTF8CharBuffer::UTF8CharBuffer()
  : antlr::CharBuffer(std::cin), pos(0), size(0)/*, eof(false)*/, lastcr(false), free(true) {

    input = NULL;

}

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(const char* ifilename, const char* encoding)
  : antlr::CharBuffer(std::cin), pos(0), size(0)/*, eof(false)*/, lastcr(false), free(true)
{
  const char* enc = encoding;

  /* Use a libxml2 parser input buffer to support URIs.
     If an encoding is specified, then use it.  Otherwise, assume none, and
     try to figure it out later.
  */
  if (!(input = xmlParserInputBufferCreateFilename(ifilename,
                                                   enc ? xmlParseCharEncoding(enc) : XML_CHAR_ENCODING_NONE)))
    throw UTF8FileError();
  init(encoding);

}

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(const char* source, int asize, const char * encoding)
  : antlr::CharBuffer(std::cin), pos(0), size(asize)/*, eof(false)*/, lastcr(false), free(true)
{
  const char * enc = encoding;

  /* Use a libxml2 parser input buffer to support URIs.
     If an encoding is specified, then use it.  Otherwise, assume none, and
     try to figure it out later.
  */
  if (!(input = xmlParserInputBufferCreateMem(source, size, enc ? xmlParseCharEncoding(enc) : XML_CHAR_ENCODING_NONE)))
    throw UTF8FileError();

  /* If we do not have an encoding at the start, then there is no raw buffer created
     or used.  Unfortunately, we have to open the file with our encoding defined. */
  if(encoding && input->encoder) {
    pos = 0;
#ifdef LIBXML2_NEW_BUFFER
//    if(input->raw)
//      xmlBufferFree(input->raw);
    input->raw = input->buffer;
    input->rawconsumed = 0;
    xmlParserInputBufferPtr temp_parser = xmlAllocParserInputBuffer(XML_CHAR_ENCODING_8859_1);
    input->buffer = temp_parser->buffer;
    temp_parser->buffer = 0;
    xmlFreeParserInputBuffer(temp_parser);
    size = growBuffer();
#else
    if(input->raw)
      xmlBufferFree(input->raw);
    input->raw = input->buffer;
    input->rawconsumed = 0;
    input->buffer = xmlBufferCreate();
    size = growBuffer();
#endif
  }

  init(encoding);

}

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(xmlParserInputBufferPtr pinput, const char * encoding)
  : antlr::CharBuffer(std::cin), pos(0), size(0)/*, eof(false)*/, lastcr(false), input(pinput), free(false)
{
    //const char * enc = encoding;

  /* Use a libxml2 parser input buffer to support URIs.
     If an encoding is specified, then use it.  Otherwise, assume none, and
     try to figure it out later.
  */
  if (!input)
    throw UTF8FileError();

  size = pinput && pinput->buffer && xmlBufContent(pinput->buffer) ? strlen((const char *)xmlBufContent(pinput->buffer)) : 0;

  /* If we do not have an encoding at the start, then there is no raw buffer created
     or used.  Unfortunately, we have to open the file with our encoding defined. */
  if(encoding && input->encoder) {
    pos = 0;
#ifdef LIBXML2_NEW_BUFFER
//    if(input->raw)
//      xmlBufferFree(input->raw);
    input->raw = input->buffer;
    input->rawconsumed = 0;
    xmlParserInputBufferPtr temp_parser = xmlAllocParserInputBuffer(XML_CHAR_ENCODING_8859_1);
    input->buffer = temp_parser->buffer;
    temp_parser->buffer = 0;
    xmlFreeParserInputBuffer(temp_parser);
    size = growBuffer();
#else
    if(input->raw)
      xmlBufferFree(input->raw);
    input->raw = input->buffer;
    input->rawconsumed = 0;
    input->buffer = xmlBufferCreate();
    size = growBuffer();
#endif
  }

  init(encoding);

}

void UTF8CharBuffer::init(const char * encoding) {

  /* If an encoding was not specified, then try to detect it.
     This is especially important for the BOM for UTF-8.
     If nothing is detected, then use ISO-8859-1 */
  if (!encoding) {

    // input enough characters to detect.
    // 4 is good because you either get 4 or some standard size which is probably larger (really)
    size = xmlParserInputBufferGrow(input, 4);

    // detect (and remove) BOMs for UTF8 and UTF16
    if (size >= 3 &&
        xmlBufContent(input->buffer)[0] == 0xEF &&
        xmlBufContent(input->buffer)[1] == 0xBB &&
        xmlBufContent(input->buffer)[2] == 0xBF) {

      pos = 3;

    } else {

      // assume ISO-8859-1 unless we can detect it otherwise
      xmlCharEncoding denc = XML_CHAR_ENCODING_8859_1;

      // now see if we can detect it
      xmlCharEncoding newdenc = xmlDetectCharEncoding(xmlBufContent(input->buffer), size);
      if (newdenc)
        denc = newdenc;

      /* Transform the data already read in */

      // since original encoding was NONE, no raw buffer was allocated, so use the regular buffer
      pos = 0;
      input->raw = input->buffer;
      input->rawconsumed = 0;

      // need a new regular buffer
#ifdef LIBXML2_NEW_BUFFER
      xmlParserInputBufferPtr temp_parser = xmlAllocParserInputBuffer(denc);
      input->buffer = temp_parser->buffer;
      temp_parser->buffer = 0;
      xmlFreeParserInputBuffer(temp_parser);
#else
      input->buffer = xmlBufferCreate();
#endif
      // setup the encoder being used
      input->encoder = xmlGetCharEncodingHandler(denc);

      // fill up the buffer with even more data
      size = growBuffer();
    }
  }

}


int UTF8CharBuffer::growBuffer() {

  return xmlParserInputBufferGrow(input, SRCBUFSIZE);
}

// libxml context
void* UTF8CharBuffer::getContext() const {

  return input ? input->context : 0;
}

/*
  Get the next character from the stream

  Grab characters one byte at a time from the input stream and place
  them in the original source encoding buffer.  Then convert from the
  original encoding to UTF-8 in the utf8 buffer.
*/
int UTF8CharBuffer::getChar() {

  if(!input) return getchar();

  // need to refill the buffer
  if (size == 0 || pos >= size) {

    // refill the buffer
#ifdef LIBXML2_NEW_BUFFER
    xmlBufShrink(input->buffer, size);
#else
    input->buffer->use = 0;
#endif
    size = xmlParserInputBufferGrow(input, SRCBUFSIZE);

    // found problem or eof
    if (size == -1 || size == 0)
      return -1;

    // start at the beginning
    pos = 0;
  }

  // individual 8-bit character to return
  int c = (int) xmlBufContent(input->buffer)[pos++];

  // sequence "\r\n" where the '\r'
  // has already been converted to a '\n' so we need to skip over this '\n'
  if (lastcr && c == '\n') {
    lastcr = false;

    // might need to refill the buffer
    if (pos >= size) {

      // refill the buffer
#ifdef LIBXML2_NEW_BUFFER
    xmlBufShrink(input->buffer, size);
#else
    input->buffer->use = 0;
#endif

      size = growBuffer();

      // found problem or eof
      if (size == -1 || size == 0)
        return -1;

      // start at the beginning
      pos = 0;
    }

    // certain to have a character
    c = (int) xmlBufContent(input->buffer)[pos++];
  }

  // convert carriage returns to a line feed
  if (c == '\r') {
    lastcr = true;
    c = '\n';
  }

  return c;
}

UTF8CharBuffer::~UTF8CharBuffer() {

  if(free)
    xmlFreeParserInputBuffer(input);
}
