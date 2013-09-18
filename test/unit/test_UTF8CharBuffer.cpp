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

    xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem("abc", 3, xmlParseCharEncoding("UTF-8"));
    UTF8CharBuffer utf8(input, "UTF-8");
    assert(utf8.getChar() == 'a');
    assert(utf8.getChar() == 'b');
    assert(utf8.getChar() == 'c');

  }

  /*

    getContext()

   */

  {

    UTF8CharBuffer utf8("test_UTF8CharBuffer.cpp", "UTF-8");
    assert(utf8.getContext() != 0);

  }

  {

    UTF8CharBuffer utf8("abc", 3, "UTF-8");
    assert(utf8.getContext() != 0);

  }

  {

    xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem("abc", 3, xmlParseCharEncoding("UTF-8"));
    UTF8CharBuffer utf8(input, "UTF-8");
    assert(utf8.getContext() != 0);

  }


  return 0;
}
