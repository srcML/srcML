/**
 * @file UTF8CharBuffer.hpp
 *
 * @copyright Copyright (C) 2008-2019 srcML (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * CharBuffer for antlr that uses a iconv input routines.
 * This provides for any necessary encoding conversion to UTF8,
 * so using libxml for XML output is direct.
 *
 * libxml stores data internally in UTF*
 */

#ifndef INCLUDED_UTF8CHARBUFFER_HPP
#define INCLUDED_UTF8CHARBUFFER_HPP

#include <antlr/CharBuffer.hpp>
#include <string>
#include <iconv.h>
#include <sha1utilities.hpp>
#include <srcml.h>

#ifdef _MSC_VER
#    pragma warning(push,0)
#    pragma warning(disable : 4619)
#endif
#include <boost/optional.hpp>
#ifdef _MSC_VER
#    pragma warning(pop)
#endif

/**
 * UTF8FileError
 *
 * Class for thrown file error.
 */
class UTF8FileError {};

typedef ssize_t (*srcml_read_callback)(void * context, void * buffer, size_t len);
typedef int (*srcml_close_callback)(void * context);

/**
 * srcMLIO
 *
 * Data struct passed around for reading/closing of Generic IO.
 * Structure to hold context with callbacks to provide hashing.
 */
struct srcMLIO {

    /** hold void * context */
    void* context;

    /** provided read callback */
    srcml_read_callback read_callback;

    /** provided close callback */
    srcml_close_callback close_callback;
};

/**
 * UTF8CharBuffer
 *
 * Inherit form antlr CharBuffer to allow input from required
 * input sources filename, memory, FILE *, and file descriptor.
 * Also, calculates hash of input.
 */
class UTF8CharBuffer : public antlr::CharBuffer {
public:

    /** size of the original character buffer */
    static constexpr size_t SRCBUFSIZE = 1024;
    typedef void * (*srcml_open_callback)(const char * filename);

    // Create a character buffer
    UTF8CharBuffer(const char * ifilename, const char * encoding, bool hashneeded, boost::optional<std::string>& hash);
    UTF8CharBuffer(const char * c_buffer, size_t buffer_size, const char * encoding, bool hashneeded, boost::optional<std::string>& hash);
    UTF8CharBuffer(FILE * file, const char * encoding, bool hashneeded, boost::optional<std::string>& hash);
    UTF8CharBuffer(int fd, const char * encoding, bool hashneeded, boost::optional<std::string>& hash);
    UTF8CharBuffer(void * context, srcml_read_callback, srcml_close_callback, const char * encoding, bool hashneeded, boost::optional<std::string>& hash);

    // Get the next character from the stream
    int getChar();

    // Get the used encoding
    const std::string& getEncoding() const;

    int getLOC() { if (lastchar == '\n') return loc; else return loc + 1; }

    ~UTF8CharBuffer();

private:
    UTF8CharBuffer(const char* encoding, bool hashneeded, boost::optional<std::string>& hash, size_t outbuf_size);

    size_t readChars();

    /* position currently at in input buffer */
    size_t pos = 0;

    /** size of buffer to read from, either raw or cooked */
    size_t insize = 0;

    /** if last character was carriage return */
    bool lastcr = false;

    /** where to place computed hash */
    bool hashneeded = false;
    boost::optional<std::string>& hash;

    int loc = 0;

    int lastchar = 0;

    /** hash encoder */
    sha1::SHA1 ctx;

    /** raw character buffer */
    std::vector<char> raw;

    /** raw characters that were not converted due to an incomplete multibyte sequence */
    size_t inbytesleft = 0;

    /** cooked (encoded) characters */
    std::vector<char> cooked;

    /** number of encoded characters */
    size_t cooked_size = 0;

    /** store encoding for later queries */
    std::string encoding;

    /** iconv() encoding converter */
    iconv_t ic = nullptr;

    /** whether the encoding conversion is trivial (i.e., not needed) */
    int trivial = false;

    /** contacts and callbacks for read and close */
    srcMLIO sio;

    /** first time reading data */
    bool firstRead = true;
};
#endif
