/**
 * @file UTF8CharBuffer.cpp
 *
 * @copyright Copyright (C) 2008-2014 srcML, LLC. (www.srcML.org)
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


#include "UTF8CharBuffer.hpp"

#include <srcml_macros.hpp>
#include <sha1utilities.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>

#include <stdio.h>

#ifndef _MSC_BUILD
#include <unistd.h>
#else
#include <io.h>
#endif

#include <fcntl.h>

/**
 * srcMLIO
 *
 * Data struct passed around for reading/closing of Generic IO.
 * Structure to hold context with callbacks to provide hashing.
 */
struct srcMLIO {

    /** hold void * context */
    void * context;

    /** provided read callback */
    UTF8CharBuffer::srcml_read_callback read_callback;

    /** provided close callback */
    UTF8CharBuffer::srcml_close_callback close_callback;

#ifdef _MSC_BUILD
    /** mvcs hash context object */
    HCRYPTHASH * ctx;
#else
    /** openssl/CommonCrypto hash context */
    SHA_CTX * ctx;
#endif

};

/**
 * UTF8CharBuffer
 * @param ifilename input filename (complete path)
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from filename and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(const char * ifilename, const char * encoding, boost::optional<std::string> * hash)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), hash(hash), inbuf_size(0), outbuf_size(0) {

    if (!ifilename)
        throw UTF8FileError();

    // open the file
    fd = open(ifilename, O_RDONLY);
    if (fd == -1)
        throw UTF8FileError();

    sio = new srcMLIO();
    sio->context = (void*) (long) fd;
    sio->read_callback = [](void* context, char* buf, int size) -> int {
        return read((int)(long) context, buf, size);
    };
    sio->close_callback = [](void* context) -> int {
        return close((int)(long) context);
    };

    // setup encoding
    curinbuf = inbuf;
    init(encoding);
}

/**
 * UTF8CharBuffer
 * @param c_buffer byte input buffer
 * @param buffer_size size of input buffer (or length of input to use)
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from memory and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(const char * c_buffer, size_t buffer_size, const char * encoding, boost::optional<std::string> * hash)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size((int)buffer_size), lastcr(false), hash(hash), inbuf_size(0), outbuf_size(0) {

    if(!c_buffer) throw UTF8FileError();

    sio = new srcMLIO();
    sio->context = 0;
    sio->read_callback = [](void*, char*, int) -> int {
        // indicate EOF for read since we have already stored the data
        return 0;
    };
    sio->close_callback = 0;

    // initialize read buffers, encoding, and hash
    // instead of a read_callback, just setup the memory here
    curinbuf = c_buffer;
    size = buffer_size;
    inbuf_size = buffer_size;
    init(encoding);
}

/**
 * UTF8CharBuffer
 * @param file input FILE open for reading
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from FILE * and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(FILE * file, const char * encoding, boost::optional<std::string> * hash)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), hash(hash), inbuf_size(0), outbuf_size(0) {

    if (!file)
        throw UTF8FileError();

    sio = new srcMLIO();
    sio->context = (void*) (long) fileno(file);
    sio->read_callback = [](void* context, char* buf, int size) -> int {
        return read((int)(long) context, buf, size);
    };
    sio->close_callback = 0;

    // initialize read buffers, encoding, and hash
    curinbuf = inbuf;
    init(encoding);
}

/**
 * UTF8CharBuffer
 * @param fd a file descriptor open for reading
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from file descriptor and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(int fd, const char * encoding, boost::optional<std::string> * hash)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), hash(hash), inbuf_size(0), outbuf_size(0) {

    if (fd < 0)
        throw UTF8FileError();

    sio = new srcMLIO();
    sio->context = (void*) fd;
    sio->read_callback = [](void* context, char* buf, int size) -> int {
        return read((int)(long) context, buf, size);
    };
    sio->close_callback = 0;

    // setup encoding
    curinbuf = inbuf;
    init(encoding);
}

/**
 * UTF8CharBuffer
 * @param ifilename input filename (complete path)
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from filename and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(void * context, srcml_read_callback read_callback, srcml_close_callback close_callback,
     const char * encoding, boost::optional<std::string> * hash)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), hash(hash), inbuf_size(0), outbuf_size(0) {

    if (read_callback == 0 || context == 0)
        throw UTF8FileError();

    srcMLIO * sio = new srcMLIO();
    sio->context = context;
    sio->read_callback = read_callback;
    sio->close_callback = close_callback;

    // initialize read buffers, encoding, and hash
    curinbuf = inbuf;
    init(encoding);
}

/**
 * init
 * @param encoding the input encoding
 *
 * Helper function with common initialization for constructors.
 * Handles setup/detection of encoding.
 */
void UTF8CharBuffer::init(const char * encoding) {

    // if no encoding specified, assume ISO-8859-1
    this->encoding = encoding ? encoding : "ISO-8859-1";

    // setup encoder from encoding to UTF-8
    ic = iconv_open("UTF8", this->encoding->c_str());
    if (ic == (iconv_t) -1) {
        fprintf(stderr, "%s", strerror(errno));
         throw UTF8FileError();
    }

    // see if this encoding to UTF-8 is trivial
    // meaning not conversion necessary
    int isit = 0;
    iconvctl(ic, ICONV_TRIVIALP, &isit);
    trivial = isit;

    // setup the buffer used, inbuf if no conversion, and outbuf if there is
    curbuf = trivial ? curinbuf : outbuf;
    inbuf_size = 0;
    outbuf_size = 0;
    size = 0;
    pos = 0;

/*
        // detect (and remove) BOMs for UTF8 and UTF16
        if ((size >= 3 || save_size >= 3) &&
            xmlBufContent(input->buffer)[0] == 0xEF &&
            xmlBufContent(input->buffer)[1] == 0xBB &&
            xmlBufContent(input->buffer)[2] == 0xBF) {
*/

    if(hash) {
#ifdef _MSC_BUILD
        BOOL success = CryptAcquireContext(&crypt_provider, NULL, NULL, PROV_RSA_FULL, 0);
        if(!success && GetLastError() == NTE_BAD_KEYSET)
            success = CryptAcquireContext(&crypt_provider, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);
        CryptCreateHash(crypt_provider, CALG_SHA1, 0, 0, &crypt_hash);
#else
        SHA1_Init(&ctx);
#endif
    }
}

/**
 * growBuffer
 *
 * Grow the libxml2 input buffer.  Read next sequence
 * of data.
 */
int UTF8CharBuffer::growBuffer() {

    // read more data into inbuf (may already be data in there)
    if (size == 0 || pos >= size) {
        size = sio->read_callback ? (int) sio->read_callback(sio->context, inbuf, SRCBUFSIZE) : 0;
        if (size == 0) {
            return 0;
        }
    }

    // hash the grown data
    if(hash) {
#ifdef _MSC_BUILD
        CryptHashData(crypt_hash, (BYTE *)curinbuf, size, 0);
#else
        SHA1_Update(&ctx, curinbuf, (SHA_LONG)size);
#endif
    }

    // for non-trivial conversions, convert from inbuf to outbuf
    if (!trivial) {
        size_t osize = SRCBUFSIZE;
        size_t isize = size;
        osize = SRCBUFSIZE;
        char* pi = inbuf;
        char* po = outbuf;
        size_t bsize = iconv(ic, &pi, &isize, &po, &osize);

        size = SRCBUFSIZE - (int) osize;
    }

    pos = 0;

    return size;
}

/**
 * getChar
 *
 * Overrides CharBuffer getChar,
 * 
 * Get the next character from the stream.
 *
 * Grab characters one byte at a time from the input stream and place
 * them in the original source encoding buffer.  Then convert from the
 * original encoding to UTF-8 in the utf8 buffer.
 *
 * @returns the character as an integer -1 if end of file.
 */
int UTF8CharBuffer::getChar() {

    // may need more characters
    if (size == 0 || pos >= size) {

        // read more data into inbuf
        size = growBuffer();
        if (size == 0) {
            // EOF
            return -1;
        }
    }

    char c = curbuf[pos];
    ++pos;

    // convert carriage returns to a line feed
    if (c == '\r') {
        lastcr = true;
        c = '\n';
    }

    // sequence "\r\n" where the '\r'
    // has already been converted to a '\n' so we need to skip over this '\n'
    if (false && lastcr && c == '\n') {
        lastcr = false;

        // may need more characters
        if (pos >= size) {

            // read more data into inbuf
            size = growBuffer();
            if (size == 0) {
                // EOF
                return -1;
            }
        }
        c = curbuf[pos];
        ++pos;
    }

    return c;
}

/**
 * getEncoding
 *
 * Get the source encoding used.
 *
 * @returns the used source encoding.
 */
const boost::optional<std::string> & UTF8CharBuffer::getEncoding() const {

    return encoding;

}

/**
 * ~UTF8CharBuffer
 *
 * Destructor.  Close/Free input. Finish hashing and
 * place in buffer if requested.
 */
UTF8CharBuffer::~UTF8CharBuffer() {

    if(!input) return;

    xmlFreeParserInputBuffer(input);
    input = 0;

    unsigned char md[20];

    if(hash) {
#ifdef _MSC_BUILD
        DWORD        SHA_DIGEST_LENGTH;
        DWORD        hash_length_size = sizeof(DWORD);
        CryptGetHashParam(crypt_hash, HP_HASHSIZE, (BYTE *)&SHA_DIGEST_LENGTH, &hash_length_size, 0);
        CryptGetHashParam(crypt_hash, HP_HASHVAL, (BYTE *)md, &SHA_DIGEST_LENGTH, 0);
        CryptDestroyHash(crypt_hash);
        CryptReleaseContext(crypt_provider, 0);
#else
        SHA1_Final(md, &ctx);
#endif

        const char outmd[] = { HEXCHARASCII(md), '\0'};
        *hash = outmd;

    }

}
