/*
  Test cases for UTF8CharBuffer.
 */
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <UTF8CharBuffer.hpp>

int main(int argc, char * argv[]) {

  /*

    getChar()

   */

  {

    UTF8CharBuffer utf8("test_UTF8CharBuffer.cpp", "UTF-8");
    assert(utf8.getChar() == '/');
    assert(utf8.getChar() == '*');
    assert(utf8.getChar() == '\n');

  }

  {

    UTF8CharBuffer utf8("abc", 3, "UTF-8");
    assert(utf8.getChar() == 'a');
    assert(utf8.getChar() == 'b');
    assert(utf8.getChar() == 'c');

  }

  {

    xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem("abc", 3, XML_CHAR_ENCODING_NONE);
    UTF8CharBuffer utf8(input, "UTF-8");
    //assert(utf8.getChar() == 'a');
    //assert(utf8.getChar() == 'b');
    //assert(utf8.getChar() == 'c');

  }


  return 0;
}
