/**
 * @file UTF8CharBuffer.cpp
 *
 * @copyright Copyright (C) 2008-2014  SDML (www.srcML.org)
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
#include <UTF8Source.hpp>


UTF8Source::UTF8Source(const char * filename, const char * encoding) 
    : output(0), cd(0) {

    if(!filename) throw UTF8SourceError();

    output = fopen(filename, "w");

    if(!output) throw UTF8SourceError();

    processEncoding(encoding);

}

UTF8Source::UTF8Source(char ** buffer, size_t * size, const char * encoding)
    : output(0), cd(0) {

    if(!buffer) throw UTF8SourceError();

    processEncoding(encoding);

}

UTF8Source::UTF8Source(FILE * file, const char * encoding) 
    : output(file), cd(0) {

    if(!output) throw UTF8SourceError();

    processEncoding(encoding);

}

UTF8Source::UTF8Source(int fd, const char * encoding) 
    : output(0), cd(0) {

    if(fd < 0) throw UTF8SourceError();

    output = fdopen(fd, "w");

    if(!output) throw UTF8SourceError();

    processEncoding(encoding);

}

void UTF8Source::processEncoding(const char * encoding) {

    if(encoding && strcmp("UTF-8", encoding) != 0) {

	cd = iconv_open(encoding, "UTF-8");

    }

}

size_t UTF8Source::growBuffer(const char * & raw_buffer, size_t size) {

    size_t num_in_convert = size;
    size_t num_out_convert = 4 * size;

    // iconv incements buffers need temporary since static and get refilled
    unsigned char * output_buffer = iconv_buffer;
    iconv(cd, (char **)&raw_buffer, &num_in_convert, (char **)&output_buffer, &num_out_convert);

    return (4 * size) - num_out_convert;
}

/*

*/
int UTF8Source::write_string(const char * input, size_t input_size) {

    if(!cd) {

	fwrite(input, 1, input_size, output);
	return 1;

    }

    size_t pos = 0; 

    while(pos < input_size) {

	size_t size = input_size - pos;
	size_t num_to_convert = size < SRCBUFSIZE ? size : SRCBUFSIZE;
	size_t num_converted = growBuffer(input, num_to_convert);

	fwrite(iconv_buffer, 1, num_converted, output);

	pos += num_to_convert;

    }

    return 1;
}

UTF8Source::~UTF8Source() {

    if(cd) iconv_close(cd);

}
