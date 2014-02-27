/**
 * UTF8OutputSource.hpp
 *
 * Copyright (C) 2008-2014  SDML (www.srcML.org)
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

/**
 * CharBuffer for antlr that uses a iconv input routines.
 * This provides for any necessary encoding conversion to UTF8,
 * so using libxml for XML output is direct.
 *
 * libxml stores data internally in UTF*
 */

#ifndef INCLUDE_UTF8OutputSource_HPP
#define INCLUDE_UTF8OutputSource_HPP

#include <cstring>

#include <stdio.h>
#include <iconv.h>

class UTF8OutputSourceError {};


class UTF8OutputSource {
public:

    // size of the original character buffer
    static const size_t SRCBUFSIZE = 1024;

    // Create a character buffer
    UTF8OutputSource(const char * filename, const char * encoding);
    UTF8OutputSource(char ** src_buffer, size_t * src_size, const char * encoding);
    UTF8OutputSource(FILE * file, const char * encoding);
    UTF8OutputSource(int fd, const char * encoding);

    int write_string(const char * input, size_t input_size);

    ~UTF8OutputSource();

private:

    size_t growBuffer(const char * & raw_buffer, size_t size);
    void processEncoding(const char * encoding);

    FILE * output;
    char ** src_buffer;
    size_t * src_size;
    char * buffer;
    size_t allocated;

    iconv_t cd;
    unsigned char iconv_buffer[4 * SRCBUFSIZE];
};
#endif
