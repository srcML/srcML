/**
 * @file UTF8CharBuffer.cpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
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

#include <UTF8CharBuffer.hpp>

#include <sha1utilities.hpp>
#include <iostream>
#include <fcntl.h>
#include <iterator>
#include <map>
#include <string>
#include <cstring>

#ifndef _MSC_BUILD
#include <unistd.h>
#else
#include <io.h>
#endif

namespace {
    template<typename T>
    struct Context {
        Context(T v) : value(v) {}
        T value;
    };

    // indicates if the two given encodings are basically the same,
    // with a trivial encoding process
    bool compatibleEncodings(const char* encoding1, const char* encoding2) {

        // setup encoder between the two encodings
        iconv_t ce = iconv_open(encoding1, encoding2);
        if (ce == (iconv_t) -1)
            return false;

        // see if encoding is trivial
        int trivial = false;
#if _LIBICONV_VERSION >= 0x0108
        iconvctl(ce, ICONV_TRIVIALP, &trivial);
#endif
        iconv_close(ce);

        return trivial;
    }
#if 0
    std::string estimateEncoding(const std::vector<char>& buffer) {

        bool ascii = true;
        for (const unsigned char& c : buffer) {
            // no ISO-8859-1 numbers in this range
            if (c > 0x7E && c < 0xA0)
                return "UTF-8";

            if (c > 0x7E)
                ascii = false;
        }

        if (ascii)
            return "ASCII";

        return "ISO-8859-1";
    }
#endif

    // some common aliases that libiconv does not accept
    std::map<std::string, std::string> encodingAliases = {
        { "UTF16", "UTF-16"},
        { "UCS2", "UCS-2"},
        { "UCS4", "UCS-4"},
    };

    std::string normalizeEncodingName(const char* name) {

        std::string str = name;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        auto search = encodingAliases.find(str);
        if (search == encodingAliases.end())
            return str;

        return search->second;
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
UTF8CharBuffer::UTF8CharBuffer(const char* encoding, bool hashneeded, boost::optional<std::string>& hash, size_t cooked_size)
    : antlr::CharBuffer(std::cin), hashneeded(hashneeded), hash(hash), cooked_size(cooked_size) {

    // may be null
    this->encoding = encoding ? normalizeEncodingName(encoding) : "";

    if (hashneeded) {
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
    sio.read_callback = 0;
    sio.close_callback = 0;

    // copy the data from the user parameter
    // would be nice to use directly, but would have to verify it is not deallocated after constructor call
    raw.insert(raw.begin(), c_buffer, c_buffer + buffer_size);
    insize = raw.size();

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

        size_t result = fread(buf, 1, insize, (FILE*) context);
        if (result == 0 && feof((FILE*) context) != 1)
            return -1;
        else
            return (size_t) result;
    };
    sio.close_callback = [](void*) -> int { return 0; };
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

    // read more data into raw (may already be data in there)
    if (insize == 0 || pos >= insize) {

        // create room for the raw characters
        raw.resize(SRCBUFSIZE);

        // use the provided callback
        // the entire input buffer may not be available because of incomplete multi-byte sequences
        // from a previous read
        ssize_t insize = sio.read_callback ? (int) sio.read_callback(sio.context, raw.data() + inbytesleft, raw.size() - inbytesleft) : 0;
        if (insize == -1) {
            fprintf(stderr, "Error reading: %s", strerror(errno));
            return 0;
        }

        // EOF
        if (insize == 0) {
            return 0;
        }

        // new size is the number of bytes read in, plus any incomplete multibyte sequences from previous
        raw.resize(insize + inbytesleft);
    }

    // hash only the read data, not the inbytesleft (from previous call)
    if (hashneeded) {
#ifdef _MSC_BUILD
        CryptHashData(crypt_hash, (BYTE *)raw.data() + inbytesleft, (DWORD) (raw.size() - inbytesleft), 0);
#else
        SHA1_Update(&ctx, raw.data() + inbytesleft, (SHA_LONG) (raw.size() - inbytesleft));
#endif
    }

    // assume nothing to skip over
    pos = 0;

    // setup encoding on first read of data
    if (firstRead) {

        // treat unsigned int field as just 4 bytes regardless of endianness
        // with 0 for any missing data
        union { unsigned char d[4]; uint32_t i; } data = { { 0, 0, 0, 0 } };
        for (size_t i = 0; i < 4 && i < raw.size(); ++i)
            data.d[i] = static_cast<unsigned char>(raw[i]);

        // check for UTF-8 BOM
        if ((data.i & 0x00FFFFFF) == 0x00BFBBEF) {

            // a trivial conversion, so BOM (Byte Order Mark) for UTF-8 has to be manually skipped
            pos += 3;

            // no encoding specified (by user) then UTF-8, otherwise check if it is compatible with UTF-8
            if (encoding.empty()) {
                encoding = "UTF-8";
            } else if (encoding != "UTF-8" && !compatibleEncodings(encoding.c_str(), "UTF-8")) {
                fprintf(stderr, "Warning: the encoding %s was specified, but the source code has a UTF-8 BOM\n", encoding.c_str());
            }
        }

        // auto-detect UTF-16 based on BOM
        // both UTF-16LE and UTF-16BE are determined automatically from BOM
        // and processed as UTF-16
        if ((data.i & 0x0000FFFF) == 0x0000FFFE || (data.i & 0x0000FFFF) == 0x0000FEFF) {

            // no encoding specified (by user) then UTF-16, otherwise check if it is compatible with UTF-16
            if (encoding.empty()) {
                encoding = "UTF-16";
            } else if (encoding != "UTF-16" && !compatibleEncodings(encoding.c_str(), "UTF-16")) {
                fprintf(stderr, "Warning: the encoding %s was specified, but the source code has a UTF-16 BOM\n", encoding.c_str());
            }
        }

        // auto-detect UTF-32 based on BOM
        // both UTF-32LE and UTF-32BE are determined automatically from BOM
        // and processed as UTF-32
        if (data.i == 0xFFFE0000 || data.i == 0xFEFF0000) {

            // no encoding specified (by user) then UTF-32, otherwise check if it is compatible with UTF-32
            if (encoding.empty()) {
                encoding = "UTF-32";
            } else if (encoding != "UTF-32" && !compatibleEncodings(encoding.c_str(), "UTF-32")) {
                fprintf(stderr, "Warning: the encoding %s was specified, but the source code has a UTF-32 BOM\n", encoding.c_str());
            }
        }

        // if no encoding found or specified, assume ISO-8859-1
        if (encoding.empty())
            encoding = "ISO-8859-1";

        // setup encoder from encoding to UTF-8
        ic = iconv_open("UTF-8", encoding.c_str());
        if (ic == (iconv_t) -1) {
            if (errno == EINVAL) {
                fprintf(stderr, "srcml: Conversion from encoding '%s' not supported\n\n", encoding.c_str());
                return 0;
            }
        }

        // see if this encoding to UTF-8 is trivial, if so we can use raw characters directly
#if _LIBICONV_VERSION >= 0x0108
        iconvctl(ic, ICONV_TRIVIALP, &trivial);
#else
        trivial = false;
#endif
    }
    firstRead = false;

    // for non-trivial conversions, convert from raw to cooked
    if (!trivial) {

        // raw input characters
        // after call to iconv(), linbuf will point to start of any
        // incomplete multibyte sequences that were not cooked
        char* linbuf = raw.data();
        inbytesleft = raw.size();

        // cooked (encoded in UTF-8) input characters
        // full output buffer is available since all previous characters have been processed
        cooked.resize(cooked_size);
        char* loutbuf = cooked.data();
        size_t outbytesleft = cooked.size();

        // convert from raw characters to cooked, encoded in UTF-8 characters
        size_t binsize = iconv(ic, &linbuf, &inbytesleft, &loutbuf, &outbytesleft);
        if (binsize == (size_t) -1) {
            fprintf(stderr, "%s\n", strerror(errno));
            return 0;
        }

        // number of bytes cooked is the total size minus the bytes that were "left", i.e., not used, by iconv()
        cooked.resize(cooked.size() - outbytesleft);

        // all of the input characters may not have been converted
        // as not all of their bytes read in (think bufferinsize of 5 with UTF-16 input)
        // so just move all of them to the start of the buffer
        if (inbytesleft)
            std::move(linbuf, linbuf + inbytesleft, raw.begin());
    }

    return trivial ? raw.size() : cooked.size();
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

    // has to be int to handle -1 return value
    unsigned char c = 0;

    // may need more characters
    if (insize == 0 || pos >= insize) {

        insize = readChars();
        if (insize == 0) {
            // EOF
            return -1;
        }
    }

    // read the next char either from the current input buffer (for a trivial, no-iconv needed)
    // or from the iconv'ed output buffer
    c = (trivial ? raw : cooked)[pos];
    ++pos;

    // sequence "\r\n" where the '\r'
    // has already been converted to a '\n' so we need to skip over this '\n'
    // so start over
    if (lastcr && c == '\n') {
        lastcr = false;
        return getChar();
    }
    lastcr = false;

    // convert carriage returns to a line feed
    if (c == '\r') {
        lastcr = true;
        c = '\n';
    }

    lastchar = c;
    if (lastchar == '\n')
        ++loc;

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
    if(sio.context) {
        sio.close_callback(sio.context);
    }

    if (ic)
        iconv_close(ic);

    if (hashneeded) {
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
        hash = outmd;
    }
}
