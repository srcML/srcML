/**
 * @file test_UTF8CharBuffer.cpp
 * @copyright
 *
 * Copyright (C) 2013  SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

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
    assert(utf8.getChar() == '*');

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
    xmlFreeParserInputBuffer(input);
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
    xmlFreeParserInputBuffer(input);

  }


  return 0;
}
