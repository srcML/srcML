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

#ifndef LIBXML2_NEW_BUFFER

/** Use same accessor for buffer content with both libxml2 buffer types */
#define xmlBufContent(b) (b->content)

#endif

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
 * srcMLFdRead
 * @param context a srcMLFd context
 * @param buffer a buffer to write output of read
 * @param len number of bytes to read/size of buffer
 *
 * Wrapper around read to provide hashing.
 * Read len bytes from file and put in buffer.
 * Update hash with newly read data.
 */
int srcMLFdRead(void * context, char * buffer, int len) {

    int fd = (int)(size_t)context;

    size_t num_read = READ(fd, buffer, len);

    return (int)num_read;

}

/**
 * srcMLIORead
 * @param context a srcMLIO context
 * @param buffer a buffer to write output of read
 * @param len number of bytes to read/size of buffer
 *
 * Wrapper around provided read callback to provide hashing.
 * Read len bytes from file and put in buffer.
 * Update hash with newly read data.
 */
int srcMLIORead(void * context,  char * buffer, int len) {

    srcMLIO * sio = (srcMLIO *)context;
    size_t num_read = -1;
    if(sio->read_callback) num_read = sio->read_callback(sio->context, buffer, len);

    if(sio->ctx)
#ifdef _MSC_BUILD
        CryptHashData(*sio->ctx, (BYTE *)buffer, num_read, 0);
#else
        SHA1_Update(sio->ctx, buffer, (SHA_LONG)num_read);
#endif

    return (int)num_read;
}

/**
 * srcMLIOClose
 * @param context a srcMLIO context
 *
 * Wrapper around xmlFileClose.
 * Cleans up memory of srcMLIO and closes context.
 */
int srcMLIOClose(void * context) {

    srcMLIO * sio = (srcMLIO *)context;
    int ret = 0;
    if(sio->close_callback) ret = sio->close_callback(sio->context);

    delete sio;

    return ret;
}

/**
 * UTF8CharBuffer
 * @param ifilename input filename (complete path)
 * @param encoding input encoding
 * @param hash optional location to output hash of input (default = 0)
 *
 * Constructor.  Setup input from filename and hashing if needed.
 */
UTF8CharBuffer::UTF8CharBuffer(const char * ifilename, const char * encoding, boost::optional<std::string> * hash)
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), hash(hash) {

    if(!ifilename) throw UTF8FileError();

    void * file = xmlFileOpen(ifilename);
    if(!file) throw UTF8FileError();

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

    srcMLIO * sio = new srcMLIO();
    sio->context = file;
    sio->read_callback = xmlFileRead;
    sio->close_callback = xmlFileClose;

#ifdef _MSC_BUILD
    hash ? sio->ctx = &crypt_hash : 0;
#else
    hash ? sio->ctx = &ctx : 0;
#endif

    input = xmlParserInputBufferCreateIO(srcMLIORead, srcMLIOClose, sio,
                                         encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    if(!input) throw UTF8FileError();


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
    : antlr::CharBuffer(std::cin), input(0), pos(0), size((int)buffer_size), lastcr(false), hash(hash) {

    if(!c_buffer) throw UTF8FileError();

    if(hash) {
#ifdef _MSC_BUILD
        BOOL success = CryptAcquireContext(&crypt_provider, NULL, NULL, PROV_RSA_FULL, 0);
        if(!success && GetLastError() == NTE_BAD_KEYSET)
            success = CryptAcquireContext(&crypt_provider, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);
        CryptCreateHash(crypt_provider, CALG_SHA1, 0, 0, &crypt_hash);
        CryptHashData(crypt_hash, (BYTE *)c_buffer, buffer_size, 0);
#else
        SHA1_Init(&ctx);
        SHA1_Update(&ctx, c_buffer, (SHA_LONG)buffer_size);
#endif

    }

    bool use_init = true;
    if(size == 0) {

        input = xmlParserInputBufferCreateMem("\xff\xff\xff\xff", 1, xmlParseCharEncoding("UTF-8"));
        this->encoding = encoding ? encoding : "ISO-8859-1";
        use_init = false;

    } else {

        input = xmlParserInputBufferCreateMem(c_buffer, size, encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    }

    if(!input) throw UTF8FileError();

    /* Mem seems to skip encoding  force it */
    if(encoding && input->encoder) {
#ifdef LIBXML2_NEW_BUFFER
        input->raw = input->buffer;
        input->rawconsumed = 0;
        xmlParserInputBufferPtr temp_parser = xmlAllocParserInputBuffer(xmlParseCharEncoding(encoding));
        input->buffer = temp_parser->buffer;
        temp_parser->buffer = 0;
        xmlFreeParserInputBuffer(temp_parser);
        size = growBuffer();
#else
        if(input->raw)
            xmlBufferFree(input->raw);
        input->raw = input->buffer;
        input->rawconsumed = 0;
        input->buffer = xmlBufferCreate();
        size = growBuffer();
#endif
    }

    if(use_init) init(encoding);

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
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), hash(hash) {

    if(!file) throw UTF8FileError();

    if(hash) {
#ifdef _MSC_BUILD
        CryptAcquireContext(&crypt_provider, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);
        CryptCreateHash(crypt_provider, CALG_SHA1, 0, 0, &crypt_hash);
#else
        SHA1_Init(&ctx);
#endif
    }

    srcMLIO * sio = new srcMLIO();
    sio->context = file;
    sio->read_callback = xmlFileRead;
    sio->close_callback = 0;

#ifdef _MSC_BUILD
    hash ? sio->ctx= &crypt_hash : 0;
#else
    hash ? sio->ctx = &ctx : 0;
#endif

    input = xmlParserInputBufferCreateIO(srcMLIORead, srcMLIOClose, sio,
                                         encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    if(!input) throw UTF8FileError();

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
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), hash(hash) {

    if(fd < 0) throw UTF8FileError();

    if(hash) {
#ifdef _MSC_BUILD
        CryptAcquireContext(&crypt_provider, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);
        CryptCreateHash(crypt_provider, CALG_SHA1, 0, 0, &crypt_hash);
#else
        SHA1_Init(&ctx);
#endif
    }

    srcMLIO * sio = new srcMLIO();
    sio->context = (void *)fd;
    sio->read_callback = srcMLFdRead;
    sio->close_callback = 0;

#ifdef _MSC_BUILD
    hash ? sio->ctx = &crypt_hash : 0;
#else
    hash ? sio->ctx = &ctx : 0;
#endif

    input = xmlParserInputBufferCreateIO(srcMLIORead, srcMLIOClose, sio,
                                         encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    if(!input) throw UTF8FileError();

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
    : antlr::CharBuffer(std::cin), input(0), pos(0), size(0), lastcr(false), hash(hash) {

    if(read_callback == 0 || context == 0) throw UTF8FileError();

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

    srcMLIO * sio = new srcMLIO();
    sio->context = context;
    sio->read_callback = read_callback;
    sio->close_callback = close_callback;
    
#ifdef _MSC_BUILD
    hash ? sio->ctx = &crypt_hash : 0;
#else
    hash ? sio->ctx = &ctx : 0;
#endif

    input = xmlParserInputBufferCreateIO(srcMLIORead, srcMLIOClose, sio,
                                         encoding ? xmlParseCharEncoding(encoding) : XML_CHAR_ENCODING_NONE);

    if(!input) throw UTF8FileError();


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

    if(encoding) this->encoding = encoding;

    /* If an encoding was not specified, then try to detect it.
       This is especially important for the BOM for UTF-8.
       If nothing is detected, then use ISO-8859-1 */
    if (!encoding) {

        // need to save the mem buffer ssize
        int save_size = size;
        // input enough characters to detect.
        // 4 is good because you either get 4 or some standard size which is probably larger (really)
        size = xmlParserInputBufferGrow(input, 4);

        // detect (and remove) BOMs for UTF8 and UTF16
        if ((size >= 3 || save_size >= 3) &&
            xmlBufContent(input->buffer)[0] == 0xEF &&
            xmlBufContent(input->buffer)[1] == 0xBB &&
            xmlBufContent(input->buffer)[2] == 0xBF) {

            pos = 3;

            this->encoding = "UTF-8";

            // restore mem buffer size
            if(size == 0) size = save_size;

        } else {

            // assume ISO-8859-1 unless we can detect it otherwise
            xmlCharEncoding denc = XML_CHAR_ENCODING_8859_1;

            // now see if we can detect it
            xmlCharEncoding newdenc = xmlDetectCharEncoding(xmlBufContent(input->buffer), size);
            if (newdenc && newdenc != XML_CHAR_ENCODING_UTF8)
                denc = newdenc;

            /* Transform the data already read in */

            // since original encoding was NONE, no raw buffer was allocated, so use the regular buffer
            pos = 0;
            input->raw = input->buffer;
            input->rawconsumed = 0;

            // need a new regular buffer
#ifdef LIBXML2_NEW_BUFFER
            xmlParserInputBufferPtr temp_parser = xmlAllocParserInputBuffer(denc);
            input->buffer = temp_parser->buffer;
            temp_parser->buffer = 0;
            xmlFreeParserInputBuffer(temp_parser);
#else
            input->buffer = xmlBufferCreate();
#endif
            // setup the encoder being used
            input->encoder = xmlGetCharEncodingHandler(denc);

            this->encoding = xmlGetCharEncodingName(denc);

            // fill up the buffer with even more data
            size = growBuffer();
        }
    }

}

/**
 * growBuffer
 *
 * Grow the libxml2 input buffer.  Read next sequence
 * of data.
 */
int UTF8CharBuffer::growBuffer() {

    return xmlParserInputBufferGrow(input, SRCBUFSIZE);

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

    if(!input) return getchar();

    // need to refill the buffer
    if (size == 0 || pos >= size) {

        // refill the buffer
#ifdef LIBXML2_NEW_BUFFER
        xmlBufShrink(input->buffer, size);
#else
        input->buffer->use = 0;
#endif
        size = xmlParserInputBufferGrow(input, SRCBUFSIZE);

        // found problem or eof
        if (size == -1 || size == 0)
            return -1;

        // start at the beginning
        pos = 0;
    }

    // individual 8-bit character to return
    int c = (int) xmlBufContent(input->buffer)[pos++];

    // sequence "\r\n" where the '\r'
    // has already been converted to a '\n' so we need to skip over this '\n'
    if (lastcr && c == '\n') {
        lastcr = false;

        // might need to refill the buffer
        if (pos >= size) {

            // refill the buffer
#ifdef LIBXML2_NEW_BUFFER
            xmlBufShrink(input->buffer, size);
#else
            input->buffer->use = 0;
#endif

            size = growBuffer();

            // found problem or eof
            if (size == -1 || size == 0)
                return -1;

            // start at the beginning
            pos = 0;
        }

        // certain to have a character
        c = (int) xmlBufContent(input->buffer)[pos++];
    }

    // convert carriage returns to a line feed
    if (c == '\r') {
        lastcr = true;
        c = '\n';
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
