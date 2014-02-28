/**
 * @file UTF8CharBuffer.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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
#include <UTF8OutputSource.hpp>


UTF8OutputSource::UTF8OutputSource(const char * filename, const char * to_encoding,
				   const char * from_encoding) 
    : output(0), src_buffer(0), src_size(0), buffer(0), allocated(0), cd(0) {

    if(!filename) throw UTF8OutputSourceError();

    output = fopen(filename, "w");

    if(!output) throw UTF8OutputSourceError();

    processEncoding(to_encoding, from_encoding);

}

UTF8OutputSource::UTF8OutputSource(char ** src_buffer, size_t * src_size, const char * to_encoding,
				   const char * from_encoding)
    : output(0), src_buffer(src_buffer), src_size(src_size), buffer(0), allocated(0), cd(0) {

    if(!src_buffer || !src_size) throw UTF8OutputSourceError();

    buffer = (char *)malloc(4 * SRCBUFSIZE + 1);
    (*src_size) = 0;
    allocated = 4 * SRCBUFSIZE;

    processEncoding(to_encoding, from_encoding);

}

UTF8OutputSource::UTF8OutputSource(FILE * file, const char * to_encoding,
				   const char * from_encoding) 
    : output(file), src_buffer(0), src_size(0), buffer(0), allocated(0), cd(0) {

    if(!output) throw UTF8OutputSourceError();

    processEncoding(to_encoding, from_encoding);

}

UTF8OutputSource::UTF8OutputSource(int fd, const char * to_encoding,
				   const char * from_encoding)
    : output(0), src_buffer(0), src_size(0), buffer(0), allocated(0), cd(0) {

    if(fd < 0) throw UTF8OutputSourceError();

    output = fdopen(fd, "w");

    if(!output) throw UTF8OutputSourceError();

    processEncoding(to_encoding, from_encoding);

}

void UTF8OutputSource::processEncoding(const char * to_encoding, const char * from_encoding) {

    if(to_encoding && strcmp(from_encoding, to_encoding) != 0) {

	cd = iconv_open(to_encoding, from_encoding);

    }

}

size_t UTF8OutputSource::growBuffer(const char * & raw_buffer, size_t size) {

    size_t num_in_convert = size;
    size_t num_out_convert = 4 * size;

    // iconv incements buffers need temporary since static and get refilled
    unsigned char * output_buffer = iconv_buffer;
    iconv(cd, (char **)&raw_buffer, &num_in_convert, (char **)&output_buffer, &num_out_convert);

    return (4 * size) - num_out_convert;
}

void growString(char * & str, size_t & allocated, size_t used, size_t needed) {

    if(needed < (allocated - used)) return;

    size_t new_size = (used + needed) * 2;
    char * new_str = (char *)malloc(new_size + 1);
    strncpy(new_str, str, used);
    free(str);
    str = new_str;
    allocated = new_size;

}

/*

*/
int UTF8OutputSource::writeString(const char * input, size_t input_size) {

    if(!cd && !output) {

	growString(buffer, allocated, *src_size, input_size);
	strncpy(buffer + *src_size, input, input_size); 
	*src_size += input_size;

	return 1;

    }

    if(!cd && output) {

	fwrite(input, 1, input_size, output);
	return 1;

    }

    size_t pos = 0; 

    while(pos < input_size) {

	size_t size = input_size - pos;
	size_t num_to_convert = size < SRCBUFSIZE ? size : SRCBUFSIZE;
	errno = 0;
	size_t num_converted = growBuffer(input, num_to_convert);

	if(errno) return 0;

	if(output)
	    fwrite(iconv_buffer, 1, num_converted, output);
	else {

	    growString(buffer, allocated, *src_size, num_converted);
	    strncpy(buffer + *src_size, (const char *)iconv_buffer, num_converted); 
	    *src_size += num_converted;

	}	    

	pos += num_to_convert;

    }

    return 1;
}

UTF8OutputSource::~UTF8OutputSource() {

    if(cd) iconv_close(cd);

    if(output) fclose(output);

    if(src_buffer) {

	buffer[*src_size] = 0;
	(*src_buffer) = buffer;

    }

}
