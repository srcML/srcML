/*
  UTF8CharBuffer.cpp

  Copyright (C) 2008-2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <iostream>
#include "UTF8CharBuffer.hpp"

UTF8CharBuffer::UTF8CharBuffer()
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), need_close(false) {}

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(const char * ifilename, const char * encoding)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), need_close(true) {

    if(!ifilename) throw UTF8FileError();

    input = fopen(ifilename, "r");

    if(!input) throw UTF8FileError();

    input_buffer = (char *)buffer;

}


UTF8CharBuffer::UTF8CharBuffer(const char * c_buffer, size_t size) 
    : antlr::CharBuffer(std::cin), input(0), pos(0), size((int)size), lastcr(false), need_close(false) {

    if(!c_buffer) throw UTF8FileError();

    input_buffer = (char *)c_buffer;
}    

UTF8CharBuffer::UTF8CharBuffer(FILE * file, const char * encoding)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), need_close(false) {

    if(!file) throw UTF8FileError();

    input = file;
    input_buffer = (char *)buffer;

}

UTF8CharBuffer::UTF8CharBuffer(int fd, const char * encoding)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), need_close(true) {

    if(fd < 0) throw UTF8FileError();

    input = fdopen(fd, "r");

    if(!input) throw UTF8FileError();

    input_buffer = (char *)buffer;

}
int UTF8CharBuffer::growBuffer() {

    if(!input) return -1;

    return fread(input_buffer, 1, SRCBUFSIZE, input);
}

/*
  Get the next character from the stream

  Grab characters one byte at a time from the input stream and place
  them in the original source encoding buffer.  Then convert from the
  original encoding to UTF-8 in the utf8 buffer.
*/
int UTF8CharBuffer::getChar() {

    // need to refill the buffer
    if (size == 0 || pos >= size) {

        // refill the buffer
        size = growBuffer();

        // found problem or eof
        if (size == -1 || size == 0)
            return -1;

        // start at the beginning
        pos = 0;
    }

    // individual 8-bit character to return
    int c = (int) input_buffer[pos++];

    // sequence "\r\n" where the '\r'
    // has already been converted to a '\n' so we need to skip over this '\n'
    if (lastcr && c == '\n') {
        lastcr = false;

        // might need to refill the buffer
        if (pos >= size) {

	    //buffer.clear();

            size = growBuffer();

            // found problem or eof
            if (size == -1 || size == 0)
                return -1;

            // start at the beginning
            pos = 0;
        }

        // certain to have a character
        c = (int)input_buffer[pos++];
    }

    // convert carriage returns to a line feed
    if (c == '\r') {
        lastcr = true;
        c = '\n';
    }

    return c;
}

UTF8CharBuffer::~UTF8CharBuffer() {

    if(need_close) fclose(input);
}
