/**
 * UTF8CharBuffer.hpp
 *
 * Copyright (C) 2008-2014 SDML (www.srcML.org)
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

#ifndef INCLUDED_UTF8CHARBUFFER_HPP
#define INCLUDED_UTF8CHARBUFFER_HPP

#include <antlr/CharBuffer.hpp>
#include <cstring>

#include <stdio.h>

#include <libxml/xmlIO.h>

#ifdef _MSC_BUILD
#include <windows.h>
#include <Wincrypt.h>
#elif defined(__MACH__)
#include <CommonCrypto/CommonDigest.h>

/** Use same SHA_CTX symbol for openssl and CommonCrypto  */
#define SHA_CTX CC_SHA1_CTX

/** Use same SHA1_Init symbol for openssl and CommonCrypto  */
#define SHA1_Init CC_SHA1_Init

/** Use same SHA1_Update symbol for openssl and CommonCrypto  */
#define SHA1_Update CC_SHA1_Update

/** Use same SHA1_Final symbol for openssl and CommonCrypto  */
#define SHA1_Final CC_SHA1_Final

/** Use same LONG symbol for openssl and CommonCrypto  */
#define SHA_LONG CC_LONG

/** Use same SHA_DIGEST_LENGTH symbol for openssl and CommonCrypto  */
#define SHA_DIGEST_LENGTH CC_SHA1_DIGEST_LENGTH
#else
#include <openssl/sha.h>
#endif

#include <boost/optional.hpp>

/**
 * UTF8FileError
 *
 * Class for thrown file error.
 */
class UTF8FileError {};

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
    static const size_t SRCBUFSIZE = 1024;
    typedef void * (*srcml_open_callback)(const char * filename);
    typedef int (*srcml_read_callback)(void * context, char * buffer, int len);
    typedef int (*srcml_close_callback)(void * context);

    // Create a character buffer
    UTF8CharBuffer(const char * ifilename, const char * encoding, boost::optional<std::string> * hash);
    UTF8CharBuffer(const char * c_buffer, size_t buffer_size, const char * encoding, boost::optional<std::string> * hash);
    UTF8CharBuffer(FILE * file, const char * encoding, boost::optional<std::string> * hash);
    UTF8CharBuffer(int fd, const char * encoding, boost::optional<std::string> * hash);
    UTF8CharBuffer(void * context, srcml_read_callback read_callback, srcml_close_callback close_callback, const char * encoding, boost::optional<std::string> * hash);

    // Get the next character from the stream
    int getChar();

    // Get the used encoding
    const boost::optional<std::string> & getEncoding() const;

    ~UTF8CharBuffer();

private:

    int growBuffer();
    void init(const char * encoding);

    /** xml input handles encodings */
    xmlParserInputBufferPtr input;

    /** Store encoding for later queries */
    boost::optional<std::string> encoding;

    /* position currently at in input buffer */
    int pos;

    /** size of read in buffer */
    int size;

    /** if last character was carriage return */
    bool lastcr;

    /** where to place computed hash */
    boost::optional<std::string> * hash;

#ifdef _MSC_BUILD
    /** msvc hash provider object */
    HCRYPTPROV   crypt_provider;
    /** msvc hash object */
    HCRYPTHASH   crypt_hash;
#else
    /** openssl/CommonCrypto hash context */
    SHA_CTX ctx;
#endif

};
#endif
