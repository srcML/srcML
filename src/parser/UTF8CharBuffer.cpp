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

#include <sha1utilities.hpp>
#include <iostream>
#include <fcntl.h>

#ifndef _MSC_BUILD
#include <unistd.h>
#else
#include <io.h>
#endif


template<typename T>
struct Context {
    Context(T v) : value(v) {}
    T value;
};

/**
 * UTF8CharBuffer
 * @param ifilename input filename (complete path)
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from filename and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(const char* encoding, boost::optional<std::string>* hash, size_t outbuf_size)
    : antlr::CharBuffer(std::cin), hash(hash), outbuf_size(outbuf_size), spec_encoding(encoding) {

    // if no encoding specified, assume ISO-8859-1
    this->encoding = encoding ? encoding : "ISO-8859-1";

    // setup encoder from encoding to UTF-8
    ic = iconv_open("UTF8", this->encoding.c_str());
    if (ic == (iconv_t) -1) {
        if (errno == EINVAL) {
            fprintf(stderr, "Conversion from encoding '%s' not supported\n", this->encoding.c_str());
            exit(4);
        }
    }

    // see if this encoding to UTF-8 is trivial
    // meaning no conversion necessary
    iconvctl(ic, ICONV_TRIVIALP, &trivial);

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
 * UTF8CharBuffer
 * @param ifilename input filename (complete path)
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from filename and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(const char* ifilename, const char* encoding, boost::optional<std::string>* hash)
    : UTF8CharBuffer(encoding, hash, SRCBUFSIZE * 6) {

    if (!ifilename)
        throw UTF8FileError();

    // open the file
    fd = open(ifilename, O_RDONLY);
    if (fd == -1)
        throw UTF8FileError();

    // setup callbacks, wrappers around read() and close()
    sio.context = new Context<int>(fd);
    sio.read_callback = [](void* context, void* buf, size_t size) -> ssize_t {
        return read(static_cast<Context<int>*>(context)->value, buf, size);
    };
    sio.close_callback = [](void* context) -> int {
        int fd = static_cast<Context<int>*>(context)->value;
        delete static_cast<Context<int>*>(context);
        return close(fd);
    };
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
UTF8CharBuffer::UTF8CharBuffer(const char* c_buffer, size_t buffer_size, const char* encoding, boost::optional<std::string>* hash)
    : UTF8CharBuffer(encoding, hash, buffer_size * 6) {

    if (!c_buffer)
        throw UTF8FileError();

    // setup callbacks, null since we have the input buffer already
    sio.context = 0;
    sio.read_callback = [](void*, void*, size_t) -> ssize_t {
        // indicate EOF for read since we have already stored the data
        return 0;
    };
    sio.close_callback = 0;

    // instead of a read_callback, just setup the memory here
    curinbuf = c_buffer;
    size = buffer_size;
    inbuf_size = buffer_size;

    // since we already have all the data, need to hash and perform encoding
    size = readChars();
}

/**
 * UTF8CharBuffer
 * @param file input FILE open for reading
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from FILE * and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(FILE* file, const char* encoding, boost::optional<std::string>* hash)
    : UTF8CharBuffer(encoding, hash, SRCBUFSIZE * 6) {

    if (!file)
        throw UTF8FileError();

    // setup callbacks, mainly wrappers around fread() for FILE* converted to file descriptor
    sio.context = (void*) file;
    sio.read_callback = [](void* context, void* buf, size_t size) -> ssize_t {
        return fread(buf, 1, size, (FILE*) context);
    };
    sio.close_callback = 0;
}

/**
 * UTF8CharBuffer
 * @param fd a file descriptor open for reading
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from file descriptor and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(int fd, const char* encoding, boost::optional<std::string>* hash)
    : UTF8CharBuffer(encoding, hash, SRCBUFSIZE * 6) {

    if (fd < 0)
        throw UTF8FileError();

    // setup callbacks, wrappers around read()
    sio.context = new Context<int>(fd);
    sio.read_callback = [](void* context, void* buf, size_t size) -> ssize_t {
        return read(static_cast<Context<int>*>(context)->value, buf, size);
    };
    sio.close_callback = [](void* context) -> int {
        delete static_cast<Context<int>*>(context);
        return 0;
    };
}

/**
 * UTF8CharBuffer
 * @param ifilename input filename (complete path)
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from filename and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(void* context, srcml_read_callback read_callback, srcml_close_callback close_callback,
     const char* encoding, boost::optional<std::string>* hash)
    : UTF8CharBuffer(encoding, hash, SRCBUFSIZE * 6) {

    // requires only a read callback, not a close callback or a context
    if (read_callback == 0)
        throw UTF8FileError();

    sio.context = context;
    sio.read_callback = read_callback;
    sio.close_callback = close_callback;
}

/**
 * readChars
 *
 * Read and process the next sequence of data.
 */
ssize_t UTF8CharBuffer::readChars() {

    // read more data into inbuf (may already be data in there)
    if (size == 0 || pos >= size) {
        // we use inbuf instead of curinbuf because curinbuf can point to inbuf, or a 
        // user-provided chunk of memory. Since already read in, no need to copy
        if (inbuf.empty())
            inbuf.resize(SRCBUFSIZE);
        curinbuf = inbuf.data();
        size = sio.read_callback ? (int) sio.read_callback(sio.context, inbuf.data() + oldcharsize, SRCBUFSIZE - oldcharsize) : 0;
        if (size == 0) {
            return 0;
        }
    }

    // hash the grown data
    if (hash) {
#ifdef _MSC_BUILD
        CryptHashData(crypt_hash, (BYTE *)curinbuf, size, 0);
#else
        SHA1_Update(&ctx, curinbuf, (SHA_LONG)size);
#endif
    }

    // for non-trivial conversions, convert from inbuf to outbuf
    if (!trivial) {

        if (outbuf.empty())
            outbuf.resize(outbuf_size);
        size_t osize = outbuf_size;
        size_t isize = size + oldcharsize;
        char* pi = const_cast<char*>(curinbuf);
        char* po = outbuf.data();
        size_t bsize = iconv(ic, &pi, &isize, &po, &osize);
        if (isize) {
            std::move(pi, pi + isize, inbuf.data());
            oldcharsize = isize;
        } else {
            oldcharsize = 0;
        }
        size = outbuf_size - (int) osize;
    }

    pos = 0;

    // skip over BOM for UTF8 and UTF16(?)
    if (firstRead && (size >= 3) &&
            static_cast<const unsigned char>((trivial ? curinbuf : outbuf.data())[0]) == 0xEF &&
            static_cast<const unsigned char>((trivial ? curinbuf : outbuf.data())[1]) == 0xBB &&
            static_cast<const unsigned char>((trivial ? curinbuf : outbuf.data())[2]) == 0xBF) {

        pos += 3;

        // if we guessed at the encoding, guess what? We have UTF-8!
        if (!spec_encoding) {

            // treat as UTF-8
            this->encoding = "UTF-8";
            trivial = true;
        }
    }
    firstRead = false;

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

    char c = 0;

    // primarily go through a single time, unless converting "\r\n" to "\n"
    for (int i = 0; i < 2; ++i) {

        // may need more characters
        if (size == 0 || pos >= size) {

            // read more data into inbuf
            size = readChars();
            if (size == 0) {
                // EOF
                return -1;
            }
        }

        c = (trivial ? curinbuf : outbuf.data())[pos];
        ++pos;

        // sequence "\r\n" where the '\r'
        // has already been converted to a '\n' so we need to skip over this '\n'
        // so another pass through the loop
        if (lastcr && c == '\n') {
            lastcr = false;
            continue;
        }

        // convert carriage returns to a line feed
        if (c == '\r') {
            lastcr = true;
            c = '\n';
        }

        break;
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
const std::string& UTF8CharBuffer::getEncoding() const {

    return encoding;
}

/**
 * ~UTF8CharBuffer
 *
 * Destructor.  Close/Free input. Finish hashing and
 * place in buffer if requested.
 */
UTF8CharBuffer::~UTF8CharBuffer() {

    if (hash) {
        unsigned char md[20];

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
