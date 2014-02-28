/**
 * @file UTF8CharBuffer.cpp
 *
 * @copyright Copyright (C) 2008-2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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

#include <iostream>
#include "UTF8CharBuffer.hpp"

// Create a character buffer
UTF8CharBuffer::UTF8CharBuffer(const char * ifilename, const char * encoding)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), total_size(0), lastcr(false), need_close(true), cd(0) {

    if(!ifilename) throw UTF8FileError();

    input = fopen(ifilename, "r");

    if(!input) throw UTF8FileError();

    raw_buffer = (char *)buffer;

    processEncoding(encoding);
	
}


UTF8CharBuffer::UTF8CharBuffer(const char * c_buffer, size_t buffer_size, const char * encoding) 
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), total_size((int)buffer_size), lastcr(false), need_close(false), cd(0) {

    if(!c_buffer) throw UTF8FileError();

    raw_buffer = (char *)c_buffer;

    processEncoding(encoding);

}    

UTF8CharBuffer::UTF8CharBuffer(FILE * file, const char * encoding)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), total_size(0), lastcr(false), need_close(false), cd(0) {

    if(!file) throw UTF8FileError();

    input = file;
    raw_buffer = (char *)buffer;

    processEncoding(encoding);

}

UTF8CharBuffer::UTF8CharBuffer(int fd, const char * encoding)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), total_size(0), lastcr(false), need_close(true), cd(0) {

    if(fd < 0) throw UTF8FileError();

    input = fdopen(fd, "r");

    if(!input) throw UTF8FileError();

    raw_buffer = (char *)buffer;

    processEncoding(encoding);

}

void UTF8CharBuffer::processEncoding(const char * encoding) {

    input_buffer = (unsigned char *)raw_buffer;
    if(encoding && strcmp("UTF-8", encoding) != 0) {

	cd = iconv_open("UTF-8", encoding);
	input_buffer = (unsigned char *)iconv_buffer;

    }

}

size_t UTF8CharBuffer::convertEncodings(size_t num_to_convert) {


    size_t num_in_convert = num_to_convert;
    size_t num_out_convert = 4 * num_to_convert;

    // iconv incements buffers need temporary since static and get refilled
    unsigned char * input_buf = input_buffer;
    char * raw = raw_buffer;
#if !defined(WIN32)
    iconv(cd, &raw, &num_in_convert, (char **)&input_buf, &num_out_convert);
#else
    iconv(cd, (const char **)&raw, &num_in_convert, (char **)&input_buf, &num_out_convert);
#endif

    return (4 * num_to_convert) - num_out_convert;

}

int UTF8CharBuffer::growBuffer() {

    if(!input && !total_size) return 0;

    // raw_buffer == input_buffer when not converting and using c string
    if(!input && !cd) input_buffer += size;

    // read in the next amount
    size_t num_read = (size_t)total_size > SRCBUFSIZE ? SRCBUFSIZE : (size_t)total_size;
    if(input) num_read = fread(raw_buffer, 1, SRCBUFSIZE, input);

    if(num_read <= 0) return (int)num_read;

    size_t num_converted = num_read;
    if(cd) num_converted = convertEncodings(num_read);

    // if from c string update starting position for raw memory
    if(!input) {

	raw_buffer += num_read;

	total_size -= num_read;

    }

    return (int)num_converted;
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
    int c = (int)input_buffer[pos++];

    // sequence "\r\n" where the '\r'
    // has already been converted to a '\n' so we need to skip over this '\n'
    if (lastcr && c == '\n') {
        lastcr = false;

        // might need to refill the buffer
        if (pos >= size) {

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
