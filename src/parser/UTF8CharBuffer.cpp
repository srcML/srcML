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
UTF8CharBuffer::UTF8CharBuffer(const char* encoding, bool hashneeded, boost::optional<std::string>& hash, size_t outbuf_size)
    : antlr::CharBuffer(std::cin), hashneeded(hashneeded), hash(hash), outbuf_size(outbuf_size) {

    // may be null
    this->encoding = encoding ? encoding : "";

    if(hashneeded) {
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
UTF8CharBuffer::UTF8CharBuffer(const char* ifilename, const char* encoding, bool hashneeded, boost::optional<std::string>& hash)
    : UTF8CharBuffer(encoding, hashneeded, hash, SRCBUFSIZE * 4) {

    if (!ifilename)
        throw UTF8FileError();

    // open the file
    int fd = open(ifilename, O_RDONLY);
    if (fd == -1)
        throw UTF8FileError();

    // setup callbacks, wrappers around read() and close()
    sio.context = new Context<int>(fd);
    sio.read_callback = [](void* context, void* buf, size_t insize) -> ssize_t {
        return read(static_cast<Context<int>*>(context)->value, buf, insize);
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
UTF8CharBuffer::UTF8CharBuffer(const char* c_buffer, size_t buffer_size, const char* encoding, bool hashneeded, boost::optional<std::string>& hash)
    : UTF8CharBuffer(encoding, hashneeded, hash, buffer_size * 4) {

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
    insize = buffer_size;

    // since we already have all the data, need to hash and perform encoding
    insize = readChars();
}

/**
 * UTF8CharBuffer
 * @param file input FILE open for reading
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from FILE * and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(FILE* file, const char* encoding, bool hashneeded, boost::optional<std::string>& hash)
    : UTF8CharBuffer(encoding, hashneeded, hash, SRCBUFSIZE * 4) {

    if (!file)
        throw UTF8FileError();

    // setup callbacks, mainly wrappers around fread() for FILE* converted to file descriptor
    sio.context = (void*) file;
    sio.read_callback = [](void* context, void* buf, size_t insize) -> ssize_t {
        return fread(buf, 1, insize, (FILE*) context);
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
UTF8CharBuffer::UTF8CharBuffer(int fd, const char* encoding, bool hashneeded, boost::optional<std::string>& hash)
    : UTF8CharBuffer(encoding, hashneeded, hash, SRCBUFSIZE * 4) {

    if (fd < 0)
        throw UTF8FileError();

    // setup callbacks, wrappers around read()
    sio.context = new Context<int>(fd);
    sio.read_callback = [](void* context, void* buf, size_t insize) -> ssize_t {
        return read(static_cast<Context<int>*>(context)->value, buf, insize);
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
     const char* encoding, bool hashneeded, boost::optional<std::string>& hash)
    : UTF8CharBuffer(encoding, hashneeded, hash, SRCBUFSIZE * 4) {

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
    if (insize == 0 || pos >= insize) {

        // we use inbuf instead of curinbuf because curinbuf can point to inbuf, or a 
        // user-provided chunk of memory. Since already read in, no need to copy
        if (inbuf.empty())
            inbuf.resize(SRCBUFSIZE);
        curinbuf = inbuf.data();

        // use the provided callback
        // the entire input buffer may not be available because of incomplete multi-byte sequences
        // from a previous read
        insize = sio.read_callback ? (int) sio.read_callback(sio.context, inbuf.data() + inbytesleft, SRCBUFSIZE - inbytesleft) : 0;
        if (insize == 0) {
            return 0;
        }
    }

    // assume nothing to skip over
    pos = 0;

    // if we see a BOM, then we know we have to skip over it
    // additionally, unless the user stated a specific encoding,
    // we need to get rid of it
    if (firstRead) {

        // treat unsigned int field as just 4 bytes regardless of endianness
        // with 0 for any missing data
        union { unsigned char d[4]; uint32_t i; } data = { { 0, 0, 0, 0 } };
        for (int i = 0; i < insize; ++i)
            data.d[i] = static_cast<const unsigned char>(curinbuf[i]);

        // check for UTF-8 BOM
        if ((data.i & 0x00FFFFFF) == 0x00BFBBEF) {

            // a trivial conversion, so BOM (Byte Order Mark) for UTF-8 has to be manually removed
            pos += 3;

            // no encoding specified (by user) then UTF-8 it is
            if (encoding.empty())
                encoding = "UTF-8";
        }

        // auto-detect UTF-16 based on BOM
        // both UTF-16LE and UTF-16BE are determined automatically from BOM
        // and processed as UTF-16
        if ((data.i & 0x0000FFFF) == 0x0000FFFE ||
            (data.i & 0x0000FFFF) == 0x0000FEFF) {

            // no encoding found yet
            if (encoding.empty())
                encoding = "UTF-16";
        }

        // auto-detect UTF-32 based on BOM
        // both UTF-32LE and UTF-32BE are determined automatically from BOM
        // and processed as UTF-32
        if (data.i == 0xFFFE0000 ||
            data.i == 0xFEFF0000) {

            // no encoding found yet
            if (encoding.empty())
                encoding = "UTF-32";
        }

        // if no encoding found or specified, assume ISO-8859-1
        if (encoding.empty())
            encoding = "ISO-8859-1";

        // setup encoder from encoding to UTF-8
        ic = iconv_open("UTF8", encoding.c_str());
        if (ic == (iconv_t) -1) {
            if (errno == EINVAL) {
                fprintf(stderr, "Conversion from encoding '%s' not supported\n", encoding.c_str());
                exit(4);
            }
        }

        // see if this encoding to UTF-8 is trivial
        // meaning no conversion necessary
        iconvctl(ic, ICONV_TRIVIALP, &trivial);
    }
    firstRead = false;

    // hash the grown data
    if (hashneeded) {
#ifdef _MSC_BUILD
        CryptHashData(crypt_hash, (BYTE *)curinbuf, insize, 0);
#else
        SHA1_Update(&ctx, curinbuf, (SHA_LONG)insize);
#endif
    }

    // for non-trivial conversions, convert from inbuf to outbuf
    if (!trivial) {

        // only happens on first read
        // we do this here because this is the first place
        // where we know that the buffer is actually needed
        if (outbuf.empty())
            outbuf.resize(outbuf_size);

        // conversion input
        // input can be from inbuf, or user-supplied memory
        // user-supplied memory is const, so curinbuf is const,
        // therefore we need a non-const cast for passing to inconv()
        char* linbuf = const_cast<char*>(curinbuf);
        inbytesleft = insize + inbytesleft;

        // conversion output
        // full output buffer is available since all previous characters
        // have been returned
        char* loutbuf = outbuf.data();
        size_t outbytesleft = outbuf.size();

        // convert from input buffer to output buffer
        size_t binsize = iconv(ic, &linbuf, &inbytesleft, &loutbuf, &outbytesleft);
        if (binsize == (size_t) -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }

        // all of the input characters may not have been converted
        // as not all of their bytes read in (think bufferinsize of 5 with UTF-16 input)
        // so just move all of them to the start of the buffer
        if (inbytesleft)
            std::move(linbuf, linbuf + inbytesleft, inbuf.data());

        // number of bytes is the total output buffer insize minus 
        // the bytes that were "left", i.e., not used, by iconv()
        insize = outbuf.size() - outbytesleft;
    }

    return insize;
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
        if (insize == 0 || pos >= insize) {

            // read more data into inbuf
            insize = readChars();
            if (insize == 0) {
                // EOF
                return -1;
            }
        }

        // read the next char either from the current input buffer (for a trivial, no-iconv needed)
        // or from the iconv'ed output buffer
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

    if (hashneeded) {
        unsigned char md[20];

#ifdef _MSC_BUILD
        DWORD        SHA_DIGEST_LENGTH;
        DWORD        hash_length_size = insizeof(DWORD);
        CryptGetHashParam(crypt_hash, HP_HASHinSIZE, (BYTE *)&SHA_DIGEST_LENGTH, &hash_length_size, 0);
        CryptGetHashParam(crypt_hash, HP_HASHVAL, (BYTE *)md, &SHA_DIGEST_LENGTH, 0);
        CryptDestroyHash(crypt_hash);
        CryptReleaseContext(crypt_provider, 0);
#else
        SHA1_Final(md, &ctx);
#endif
        const char outmd[] = { HEXCHARASCII(md), '\0'};
        hash = outmd;
    }
}
