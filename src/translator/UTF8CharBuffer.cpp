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
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), need_close(false), cd(0) {}

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(const char * ifilename, const char * encoding)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), need_close(true), cd(0) {

    if(!ifilename) throw UTF8FileError();

    input = fopen(ifilename, "r");

    if(!input) throw UTF8FileError();

    raw_buffer = (char *)buffer;

    if(encoding && strcmp("UTF-8", encoding) != 0) {

	cd = iconv_open("UTF-8", encoding);
	input_buffer = (unsigned char *)iconv_buffer;

    } else
	input_buffer = (unsigned char *)raw_buffer;
	
}


UTF8CharBuffer::UTF8CharBuffer(const char * c_buffer, size_t size, const char * encoding) 
    : antlr::CharBuffer(std::cin), input(0), pos(0), size((int)size), lastcr(false), need_close(false), cd(0) {

    if(!c_buffer) throw UTF8FileError();

    raw_buffer = (char *)c_buffer;

    if(encoding && strcmp("UTF-8", encoding) != 0) {

	cd = iconv_open("UTF-8", encoding);
	input_buffer = (unsigned char *)iconv_buffer;

    } else
	input_buffer = (unsigned char *)raw_buffer;

}    

UTF8CharBuffer::UTF8CharBuffer(FILE * file, const char * encoding)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), need_close(false), cd(0) {

    if(!file) throw UTF8FileError();

    input = file;
    raw_buffer = (char *)buffer;

    if(encoding && strcmp("UTF-8", encoding) != 0) {

	cd = iconv_open("UTF-8", encoding);
	input_buffer = (unsigned char *)iconv_buffer;

    } else
	input_buffer = (unsigned char *)raw_buffer;

}

UTF8CharBuffer::UTF8CharBuffer(int fd, const char * encoding)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), need_close(true), cd(0) {

    if(fd < 0) throw UTF8FileError();

    input = fdopen(fd, "r");

    if(!input) throw UTF8FileError();

    raw_buffer = (char *)buffer;

    if(encoding && strcmp("UTF-8", encoding) != 0) {

	cd = iconv_open("UTF-8", encoding);
	input_buffer = (unsigned char *)iconv_buffer;

    } else
	input_buffer = (unsigned char *)raw_buffer;

}

int UTF8CharBuffer::growBuffer() {

    if(!input) return -1;

    size_t num_read = fread(raw_buffer, 1, SRCBUFSIZE, input);

    if(num_read <= 0) return (int)num_read;

    size_t num_in_convert = num_read;
    size_t num_out_convert = 4 * num_read;

    size_t num_convert = num_read;
    if(cd) num_convert = iconv(cd, &raw_buffer, &num_in_convert, (char **)&input_buffer, &num_out_convert);

    return (int)num_convert;
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

    if(cd) iconv_close(cd);
}
