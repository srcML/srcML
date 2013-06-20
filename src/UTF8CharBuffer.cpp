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

    // input enough characters to detect.
    // 4 is good because you either get 4 or some standard size which is probably larger (really)
    size = xmlParserInputBufferGrow(input, 4);

    // detect (and remove) BOMs for UTF8 and UTF16
    if (size >= 3 &&

#ifdef LIBXML2_NEW_BUFFER
        xmlBufContent(input->buffer)[0] == 0xEF &&
        xmlBufContent(input->buffer)[1] == 0xBB &&
        xmlBufContent(input->buffer)[2] == 0xBF) {
#else
        input->buffer->content[0] == 0xEF &&
        input->buffer->content[1] == 0xBB &&
        input->buffer->content[2] == 0xBF) {
#endif
      pos = 3;

    } else {

      // assume ISO-8859-1 unless we can detect it otherwise
      xmlCharEncoding denc = XML_CHAR_ENCODING_8859_1;

      // now see if we can detect it
#ifdef LIBXML2_NEW_BUFFER
      xmlCharEncoding newdenc = xmlDetectCharEncoding(xmlBufContent(input->buffer), size);
#else
      xmlCharEncoding newdenc = xmlDetectCharEncoding(input->buffer->content, size);
#endif
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

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(const char* source, const char * encoding, bool is_source)
  : antlr::CharBuffer(std::cin), pos(0), size(0), eof(false), lastcr(false)
{
  const char * enc = encoding;
  size = strlen(source);
  /* Use a libxml2 parser input buffer to support URIs.
     If an encoding is specified, then use it.  Otherwise, assume none, and
     try to figure it out later.
  */
  if (!(input = xmlParserInputBufferCreateMem(source, size, enc ? xmlParseCharEncoding(enc) : XML_CHAR_ENCODING_NONE)))
    throw UTF8FileError();

  if(encoding) {
    pos = 0;
    input->raw = input->buffer;
    input->rawconsumed = 0;
    input->buffer = xmlBufferCreate();
    size = growBuffer();
  }

  /* If an encoding was not specified, then try to detect it.
     This is especially important for the BOM for UTF-8.
     If nothing is detected, then use ISO-8859-1 */
  if (!encoding) {

    // input enough characters to detect.
    // 4 is good because you either get 4 or some standard size which is probably larger (really)
    size = xmlParserInputBufferGrow(input, 4);

    // detect (and remove) BOMs for UTF8 and UTF16
    if (size >= 3 &&

#ifdef LIBXML2_NEW_BUFFER
        xmlBufContent(input->buffer)[0] == 0xEF &&
        xmlBufContent(input->buffer)[1] == 0xBB &&
        xmlBufContent(input->buffer)[2] == 0xBF) {
#else
        input->buffer->content[0] == 0xEF &&
        input->buffer->content[1] == 0xBB &&
        input->buffer->content[2] == 0xBF) {
#endif
      pos = 3;

    } else {

      // assume ISO-8859-1 unless we can detect it otherwise
      xmlCharEncoding denc = XML_CHAR_ENCODING_8859_1;

      // now see if we can detect it
#ifdef LIBXML2_NEW_BUFFER
      xmlCharEncoding newdenc = xmlDetectCharEncoding(xmlBufContent(input->buffer), size);
#else
      xmlCharEncoding newdenc = xmlDetectCharEncoding(input->buffer->content, size);
#endif
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
#ifdef LIBXML2_NEW_BUFFER
    xmlBufShrink(input->buffer, 0);
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
#ifdef LIBXML2_NEW_BUFFER
  int c = (int) xmlBufContent(input->buffer)[pos++];
#else
  int c = (int) input->buffer->content[pos++];
#endif

  // sequence "\r\n" where the '\r'
  // has already been converted to a '\n' so we need to skip over this '\n'
  if (lastcr && c == '\n') {
    lastcr = false;

    // might need to refill the buffer
    if (pos >= size) {

      // refill the buffer
#ifdef LIBXML2_NEW_BUFFER
    xmlBufShrink(input->buffer, 0);
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
#ifdef LIBXML2_NEW_BUFFER
    c = (int) xmlBufContent(input->buffer)[pos++];
#else
    c = (int) input->buffer->content[pos++];
#endif

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
