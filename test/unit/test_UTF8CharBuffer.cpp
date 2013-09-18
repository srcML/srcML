/*
  Test cases for UTF8CharBuffer.
 */
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <UTF8CharBuffer.hpp>

int main(int argc, char * argv[]) {

  /*

    UTF8CharBuffer(const char* ifilename, const char* encoding)

   */

  {

    UTF8CharBuffer utf8("test_UTF8CharBuffer.cpp", "UTF-8");

  }

  /*

    UTF8CharBuffer(const char* source, int asize, const char * encoding)

   */

  {

    UTF8CharBuffer("abc", 3, "UTF-8");

  }

  /*

    UTF8CharBuffer(xmlParserInputBufferPtr pinput, const char * encoding)

   */

  {

    xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem("abc", 3, XML_CHAR_ENCODING_NONE);
    UTF8CharBuffer(input, "UTF-8");

  }


  return 0;
}
