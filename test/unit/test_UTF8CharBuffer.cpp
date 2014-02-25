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
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <UTF8CharBuffer.hpp>

int main() {

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

    UTF8CharBuffer utf8("abc", 3);
    assert(utf8.getChar() == 'a');
    assert(utf8.getChar() == 'b');
    assert(utf8.getChar() == 'c');

  }


  {

    FILE * file = fopen("test_UTF8CharBuffer.cpp", "r");
    UTF8CharBuffer utf8(file, "UTF-8");
    assert(utf8.getChar() == '/');
    assert(utf8.getChar() == '*');
    assert(utf8.getChar() == '*');
    fclose(file);

  }

  {

    int fd = open("test_UTF8CharBuffer.cpp", O_RDONLY);
    UTF8CharBuffer utf8(fd, "UTF-8");
    assert(utf8.getChar() == '/');
    assert(utf8.getChar() == '*');
    assert(utf8.getChar() == '*');
    close(fd);

  }

  return 0;
}
