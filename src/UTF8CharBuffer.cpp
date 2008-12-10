#include <iostream>
#include "UTF8CharBuffer.h"

// Get the next character from the stream
int UTF8CharBuffer::getChar() {
  static int buffer[4];
  static int bufcount = 0;
  static int bufpos = 0;
  static bool eof = false;

  // if we found eof and are all out of
  // buffer, then just get out
  if (eof && bufpos == bufcount)
    return -1;

  // fill up the buffer if needed
  if (bufpos == bufcount) {
    int i;
    for (i = 0; i < 4; ++i) {
      buffer[i] = CharBuffer::getChar();
      //      std::cout << "READ " << buffer[i] << std::endl;
      if (buffer[i] == -1) {
	eof = true;
	break;
      }

    }

    bufcount = i;
    bufpos = 0;
  }

  int stuff = buffer[bufpos++];

  //  std::cout << stuff << std::endl;
  return stuff;
}
