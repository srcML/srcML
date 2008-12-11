#include <iostream>
#include "UTF8CharBuffer.h"
#include <xmlwriter.h>

// Get the next character from the stream
int UTF8CharBuffer::getChar() {

  // maybe no need to even be doing this, ever
  if (skipencoding)
    return CharBuffer::getChar();

  // load up the input, original character buffer if all out
  if (pos == utf8buffer->use && !eof) {

    // fill up the original character buffer stopping at eof
    int i;
    for (i = 0; i < SRCBUFSIZE; ++i) {
      int c = CharBuffer::getChar();
      if (c != -1) {
	buffer->content[i] = (char) c;
	++(buffer->use);
      } else {
	eof = true;
	break;
      }
    }

    // convert from the original source encoding to UTF-8
    utf8buffer->use = 0;
    int result = xmlCharEncInFunc(handler, utf8buffer, buffer);
    if (result < 0)
      throw "Source encoding error";

    // reset start of where we get characters
    pos = 0;
  }

  // if we found eof and are all out of
  // buffer, then just get out
  if (eof && pos == utf8buffer->use)
    return -1;

  // return the next unused byte
  return utf8buffer->content[pos++];
}
