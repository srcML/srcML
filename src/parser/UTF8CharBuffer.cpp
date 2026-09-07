// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file UTF8CharBuffer.cpp
 *
 * @copyright Copyright (C) 2008-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#include <UTF8CharBuffer.hpp>

#include <algorithm>
#include <sha1utilities.hpp>
#include <iostream>
#include <fcntl.h>
#include <iterator>
#include <map>
#include <string>
#include <stdio.h>
#include <limits.h>

#ifdef _MSC_VER
    #include <io.h>
#else
    #include <sys/types.h>
    #include <sys/uio.h>
    #include <unistd.h>
#endif

using namespace ::std::literals::string_view_literals;

namespace {

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

        return trivial != 0;
    }
    // indicates whether the buffer is valid UTF-8, which includes plain ASCII
    // note that the fallback encoding cannot be detected this way, as every byte
    // sequence is valid ISO-8859-1, so it is only ever a fallback for what is not UTF-8
    // continued indicates that the data may start inside a multibyte sequence whose
    // lead byte was in the data read before it
    bool validUTF8(const std::vector<char>& buffer, bool continued = false) {

        const unsigned char* pc = reinterpret_cast<const unsigned char*>(buffer.data());
        const unsigned char* last = pc + buffer.size();

        // at most three continuation bytes can belong to a sequence that started earlier
        if (continued)
            for (int i = 0; i < 3 && pc < last && (*pc & 0xC0) == 0x80; ++i)
                ++pc;

        while (pc < last) {

            // ASCII, i.e., a single-byte sequence
            if (*pc < 0x80) {
                ++pc;
                continue;
            }

            // number of continuation bytes required by this lead byte
            // 0xC0 and 0xC1 are overlong, 0xF5 and above are past U+10FFFF,
            // and 0x80 - 0xBF is a continuation byte without a lead byte
            int following = 0;
            if (*pc >= 0xC2 && *pc <= 0xDF)
                following = 1;
            else if ((*pc & 0xF0) == 0xE0)
                following = 2;
            else if (*pc >= 0xF0 && *pc <= 0xF4)
                following = 3;
            else
                return false;

            // a sequence cut off by the end of the buffer is not an error,
            // as the rest of it is in the data read next
            if (last - pc <= following)
                return true;

            for (int i = 1; i <= following; ++i)
                if ((pc[i] & 0xC0) != 0x80)
                    return false;

            // overlong encodings, UTF-16 surrogates, and values past U+10FFFF
            // that the lead byte alone does not rule out
            if (following == 2 && ((pc[0] == 0xE0 && pc[1] < 0xA0) || (pc[0] == 0xED && pc[1] >= 0xA0)))
                return false;
            if (following == 3 && ((pc[0] == 0xF0 && pc[1] < 0x90) || (pc[0] == 0xF4 && pc[1] >= 0x90)))
                return false;

            pc += following + 1;
        }

        return true;
    }

    // some common aliases that libiconv does not accept
    std::map<std::string_view, std::string_view> encodingAliases = {
        { "UTF16", "UTF-16"},
        { "UCS2", "UCS-2"},
        { "UCS4", "UCS-4"},
    };

    std::string normalizeEncodingName(const char* name) {

        std::string str = name;
        std::transform(str.begin(), str.end(), str.begin(), [](char c){ return static_cast<char>(std::toupper(c)); });

        auto search = encodingAliases.find(str);
        if (search == encodingAliases.end())
            return str;

        return std::string(search->second);
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
UTF8CharBuffer::UTF8CharBuffer(const char* encoding, bool hashneeded, std::optional<std::string>& hash, size_t cooked_size)
    : antlr::CharBuffer(std::cin), hashneeded(hashneeded), hash(hash), cooked_size(cooked_size) {

    // may be null
    this->encoding = encoding ? normalizeEncodingName(encoding) : "";

    ctx.reset(new sha1::SHA1());
}

/**
 * UTF8CharBuffer
 * @param ifilename input filename (complete path)
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from filename and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(const char* ifilename, const char* encoding, bool hashneeded, std::optional<std::string>& hash)
    : UTF8CharBuffer(encoding, hashneeded, hash, SRCBUFSIZE * 4) {

    if (!ifilename)
        throw UTF8FileError();

    // open the file
    intptr_t fd = open(ifilename, O_RDONLY);
    if (fd == -1) {
        throw UTF8FileError();
    }

    // setup callbacks, wrappers around read() and close()
    sio.context = reinterpret_cast<void*>(fd);
    sio.read_callback = [](void* context, void* buf, size_t insize) -> ssize_t {
        if (insize > UINT_MAX)
            return -1;
        return read(static_cast<int>(reinterpret_cast<intptr_t>(context)), buf, static_cast<unsigned int>(insize));
    };
    sio.close_callback = [](void* context) -> int {
        int fd = static_cast<int>(reinterpret_cast<intptr_t>(context));
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
UTF8CharBuffer::UTF8CharBuffer(const char* c_buffer, size_t buffer_size, const char* encoding, bool hashneeded, std::optional<std::string>& hash)
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
UTF8CharBuffer::UTF8CharBuffer(FILE* file, const char* encoding, bool hashneeded, std::optional<std::string>& hash)
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
            return (ssize_t) result;
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
UTF8CharBuffer::UTF8CharBuffer(int fd, const char* encoding, bool hashneeded, std::optional<std::string>& hash)
    : UTF8CharBuffer(encoding, hashneeded, hash, SRCBUFSIZE * 4) {

    if (fd < 0)
        throw UTF8FileError();

    // setup callbacks, wrappers around read()
    sio.context = reinterpret_cast<void*>(static_cast<intptr_t>(fd));
    sio.read_callback = [](void* context, void* buf, size_t readsize) -> ssize_t {
        if (readsize > UINT_MAX)
            return -1;
        return read(static_cast<int>(reinterpret_cast<intptr_t>(context)), buf, static_cast<unsigned int>(readsize));
    };
    sio.close_callback = [](void* /* context */) -> int {
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
     const char* encoding, bool hashneeded, std::optional<std::string>& hash)
    : UTF8CharBuffer(encoding, hashneeded, hash, SRCBUFSIZE * 4) {

    // requires only a read callback, not a close callback or a context
    if (read_callback == 0)
        throw UTF8FileError();

    sio.context = context;
    sio.read_callback = read_callback;
    sio.close_callback = close_callback;
}

/**
 * setEncoding
 * @param name the encoding to convert from
 *
 * Setup the converter from the given encoding to UTF-8, replacing any current one.
 *
 * @returns whether the conversion is supported
 */
bool UTF8CharBuffer::setEncoding(std::string_view name) {

    // a copy, as the name may be a view of the current encoding
    std::string next(name);

    // keep any current converter until the new one is known to work
    iconv_t nic = iconv_open("UTF-8", next.data());
    if (nic == (iconv_t) -1) {
        fprintf(stderr, "srcml: Conversion from encoding '%s' not supported\n\n", next.data());
        return false;
    }

    if (ic)
        iconv_close(ic);
    ic = nic;
    encoding = std::move(next);

    // see if this encoding to UTF-8 is trivial, if so we can use raw characters directly
#if _LIBICONV_VERSION >= 0x0108
    iconvctl(ic, ICONV_TRIVIALP, &trivial);
#else
    trivial = false;
#endif

    return true;
}

/**
 * setFallbackEncoding
 *
 * Switch a detected encoding to the one to use for data that turned out not to be UTF-8.
 *
 * @returns whether the conversion is supported
 */
bool UTF8CharBuffer::setFallbackEncoding() {

    if (!setEncoding("ISO-8859-1"sv))
        return false;

    // every byte sequence is valid ISO-8859-1, so there is nothing left to correct
    detected = false;

    return true;
}

/**
 * readChars
 *
 * Read and process the next sequence of data.
 */
size_t UTF8CharBuffer::readChars() {

    // read more data into raw (may already be data in there)
    if (insize == 0 || pos >= insize) {

        // create room for the raw characters
        raw.resize(SRCBUFSIZE);

        // use the provided callback
        // the entire input buffer may not be available because of incomplete multi-byte sequences
        // from a previous read
        auto readsize = sio.read_callback ? (int) sio.read_callback(sio.context, raw.data() + inbytesleft, raw.size() - inbytesleft) : 0;
        if (readsize == -1) {
            fprintf(stderr, "Error reading: %s", strerror(errno));
            return 0;
        }

        // EOF
        if (readsize == 0) {
            return 0;
        }

        // new size is the number of bytes read in, plus any incomplete multibyte sequences from previous
        raw.resize(readsize + inbytesleft);
    }

    // hash only the read data, not the inbytesleft (from previous call)
    if (hashneeded) {
        ctx->processBytes(raw.data() + inbytesleft, raw.size() - inbytesleft);
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
            } else if (encoding != "UTF-8"sv && !compatibleEncodings(encoding.data(), "UTF-8")) {
                fprintf(stderr, "Warning: the encoding %s was specified, but the source code has a UTF-8 BOM\n", encoding.data());
            }
        }

        // auto-detect UTF-16 based on BOM
        // both UTF-16LE and UTF-16BE are determined automatically from BOM
        // and processed as UTF-16
        if ((data.i & 0x0000FFFF) == 0x0000FFFE || (data.i & 0x0000FFFF) == 0x0000FEFF) {

            // no encoding specified (by user) then UTF-16, otherwise check if it is compatible with UTF-16
            if (encoding.empty()) {
                encoding = "UTF-16";
            } else if (encoding != "UTF-16"sv && !compatibleEncodings(encoding.data(), "UTF-16")) {
                fprintf(stderr, "Warning: the encoding %s was specified, but the source code has a UTF-16 BOM\n", encoding.data());
            }
        }

        // auto-detect UTF-32 based on BOM
        // both UTF-32LE and UTF-32BE are determined automatically from BOM
        // and processed as UTF-32
        if (data.i == 0xFFFE0000 || data.i == 0xFEFF0000) {

            // no encoding specified (by user) then UTF-32, otherwise check if it is compatible with UTF-32
            if (encoding.empty()) {
                encoding = "UTF-32";
            } else if (encoding != "UTF-32"sv && !compatibleEncodings(encoding.data(), "UTF-32")) {
                fprintf(stderr, "Warning: the encoding %s was specified, but the source code has a UTF-32 BOM\n", encoding.data());
            }
        }

        // no encoding specified and no BOM, so detect it from the data itself
        // only UTF-8 can be identified from the data, so it is a guess that later data
        // may correct, while the ISO-8859-1 fallback is never corrected, as it accepts
        // any byte sequence at all
        // this is a detection over the first read only, so it may be corrected below
        if (encoding.empty()) {
            detected = validUTF8(raw);
            encoding = detected ? "UTF-8"sv : "ISO-8859-1"sv;
        }

        // setup encoder from encoding to UTF-8
        if (!setEncoding(encoding))
            return 0;
    }
    bool firstData = firstRead;
    firstRead = false;

    // a trivial conversion passes the raw characters straight through, so they are
    // checked here instead of relying on the conversion to reject them
    // the first data read of a detected encoding was already checked by the detection
    // above, and any data after the first may start inside a multibyte sequence that
    // the data before it started
    if (trivial && !(firstData && detected) && !validUTF8(raw, !firstData)) {

        // a detected encoding is only a guess based on the data read so far,
        // so data that is not valid means the guess was wrong
        if (detected) {
            if (!setFallbackEncoding())
                return 0;

        // a specified encoding is not a guess, so data that is not valid in it
        // is an error, as passing it through would produce srcML that is not valid XML
        } else {
            fprintf(stderr, "srcml: Input is not valid '%s'\n\n", encoding.data());
            return 0;
        }
    }

    // for non-trivial conversions, convert from raw to cooked
    if (!trivial) {

        // raw input characters
        // after call to iconv(), linbuf will point to start of any
        // incomplete multibyte sequences that were not cooked
        auto linbuf = raw.data();
        inbytesleft = raw.size();

        // cooked (encoded in UTF-8) input characters
        // full output buffer is available since all previous characters have been processed
        cooked.resize(cooked_size);
        auto loutbuf = cooked.data();
        size_t outbytesleft = cooked.size();

        // convert from raw characters to cooked, encoded in UTF-8 characters
        size_t binsize = iconv(ic, &linbuf, &inbytesleft, &loutbuf, &outbytesleft);

        // a detected encoding is only a guess based on the data read so far,
        // so an invalid sequence in later data means the guess was wrong
        // fall back and convert this data again
        if (binsize == (size_t) -1 && errno == EILSEQ && detected) {

            if (!setFallbackEncoding())
                return 0;

            linbuf = raw.data();
            inbytesleft = raw.size();
            loutbuf = cooked.data();
            outbytesleft = cooked.size();

            binsize = iconv(ic, &linbuf, &inbytesleft, &loutbuf, &outbytesleft);
        }

        // an incomplete multibyte sequence at the end of the data is not an error,
        // as the rest of it is in the data read next, and the bytes of it are kept
        // in inbytesleft and moved to the start of the buffer below
        if (binsize == (size_t) -1 && errno != EINVAL) {
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

        // the data read was only part of a multibyte sequence, so nothing could be
        // converted from it, and more data is needed before there is a character
        // to return, as a count of zero here would be taken for the end of the input
        if (cooked.empty() && inbytesleft) {
            insize = 0;
            return readChars();
        }
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
    unsigned char c = static_cast<unsigned char>((trivial ? raw : cooked)[pos]);
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
std::string_view UTF8CharBuffer::getEncoding() const {

    return encoding;
}

/**
 * ~UTF8CharBuffer
 *
 * Destructor.  Close/Free input. Finish hashing and
 * place in buffer if requested.
 */
UTF8CharBuffer::~UTF8CharBuffer() {

    if (sio.context && sio.close_callback) {
        sio.close_callback(sio.context);
    }

    if (ic)
        iconv_close(ic);

    if (hashneeded) {
        uint8_t md[20];
        ctx->getDigestBytes(md);
        const char outmd[] = { HEXCHARASCII(md), '\0'};
        hash = outmd;
    }
}
