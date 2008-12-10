#include <iostream>
#include "UTF8CharBuffer.h"
#include <xmlwriter.h>

// size of the original character buffer
const int SRCBUFSIZE = 512;

// size of the resulting utf-8 character buffer
const int UTF8BUFSIZE = SRCBUFSIZE * 4;

// Get the next character from the stream
int UTF8CharBuffer::getChar() {

  static xmlBufferPtr buffer = xmlBufferCreateSize(SRCBUFSIZE);
  static xmlBufferPtr utf8buffer = xmlBufferCreateSize(UTF8BUFSIZE);
  static unsigned int bufpos = 0;
  static bool eof = false;

  // maybe no need to even be doing this, ever
  if (skipencoding)
    return CharBuffer::getChar();

  // load up the input, original character buffer if all out
  if (!eof && bufpos == utf8buffer->use) {

    // fill up the original character buffer stopping at eof
    //    buffer->use = 0;
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
      std::cerr << "XMLCHARENCINFUNC ERROR: " << buffer->use << " " << result << std::endl;

    // reset start of where we get characters
    bufpos = 0;
  }

  // if we found eof and are all out of
  // buffer, then just get out
  if (eof && bufpos == utf8buffer->use)
    return -1;

  // return the next unused byte
  return utf8buffer->content[bufpos++];
}
