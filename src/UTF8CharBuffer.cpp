#include <iostream>
#include "UTF8CharBuffer.h"
#include <libxml/xmlwriter.h>

/*
 Get the next character from the stream

 Grab characters one byte at a time from the input stream and place
 them in the original source encoding buffer.  Then convert from the
 original encoding to UTF-8 in the utf8 buffer.
*/
int UTF8CharBuffer::getChar() {

  // maybe no need to even be doing this, ever
  if (skipencoding)
    return CharBuffer::getChar();

  // load up the original source encoding buffer
  // if all out in the utf8 buffer
  if (pos == utf8buffer->use && !eof) {

    // fill up the input buffer starting from where
    // encoding left off
    while (buffer->use < SRCBUFSIZE) {
      int c = CharBuffer::getChar();
      if (c == -1) {
	eof = true;
	break;
      }

      // sequence "\r\n" where the '\r'
      // has already been converted to a '\n'
      if (c == '\n' && lastcr) {
	lastcr = false;
	continue;
      }

      // convert carriage returns to a line feed
      if (c == '\r') {
	lastcr = true;
	c = '\n';
      }

      buffer->content[buffer->use++] = (char) c;
    }

    // convert from the original source encoding to UTF-8
    utf8buffer->use = 0;
    int result = xmlCharEncInFunc(handler, utf8buffer, buffer);
    if (result < 0)
      throw "Source encoding error";

    // start grabbing UTF-8 characters at the beginning
    pos = 0;
  }

  // if we found eof and are all out of
  // buffer, then just get out
  if (eof && pos == utf8buffer->use)
    return -1;

  // return the next unused byte
  return utf8buffer->content[pos++];
}

UTF8CharBuffer::~UTF8CharBuffer() {

  xmlBufferFree(buffer);
  xmlBufferFree(utf8buffer);
}
