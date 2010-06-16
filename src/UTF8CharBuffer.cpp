#include <iostream>
#include "UTF8CharBuffer.h"

static std::istream& inp = std::cin;

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(const char* ifilename, const char* encoding)
	  : antlr::CharBuffer(inp), pos(0), size(0), eof(false), lastcr(false)
{
  // use a libxml2 parser input buffer to support URIs
  input = xmlParserInputBufferCreateFilename(ifilename, xmlParseCharEncoding(encoding));
}

/*
 Get the next character from the stream

 Grab characters one byte at a time from the input stream and place
 them in the original source encoding buffer.  Then convert from the
 original encoding to UTF-8 in the utf8 buffer.
*/
int UTF8CharBuffer::getChar() {

  static const xmlChar* content = 0;

  // need to refill the buffer
  if (size == 0 || pos >= size) {

    // previous fill found eof
    if (eof)
      return -1;

    // refill the buffer
    input->buffer->use = 0;
    size = xmlParserInputBufferGrow(input, SRCBUFSIZE);
	
    // found problem or eof
    if (size == -1 || size == 0) {
      eof = true;
      return -1;
    }

    // shorthand for content buffer
    content = input->buffer->content;

    // start at the beginning
    pos = 0;
  }

  // individual 8-bit character to return
  int c = (int) content[pos++];

  // sequence "\r\n" where the '\r'
  // has already been converted to a '\n'
  if (c == '\n' && lastcr) {
    lastcr = false;
    if (pos < size)
      c = (int) content[pos++];
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
