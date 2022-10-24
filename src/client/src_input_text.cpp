// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_input_text.cpp
 *
 * @copyright Copyright (C) 2015-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <src_input_text.hpp>
#include <srcml_options.hpp>
#include <src_input_libarchive.hpp>
#include <src_prefix.hpp>
#include <cstring>

static int hex2decimal(unsigned char c) {

    switch (c) {
        case 'a':
        case 'A': return 10;
        case 'b':
        case 'B': return 11;
        case 'c':
        case 'C': return 12;
        case 'd':
        case 'D': return 13;
        case 'e':
        case 'E': return 14;
        case 'f':
        case 'F': return 15;
        default: return c - '0';
    }
}

static bool isodigit(char c) {
    return c >= '0' && c <= '7';
}

// Convert input to a ParseRequest and assign request to the processing queue
int src_input_text(ParseQueue& queue,
    srcml_archive* srcml_arch,
    const srcml_request_t& srcml_request,
    const srcml_input_src& input) {

    std::string raw_text = src_prefix_resource(input.resource);
    const char* ptext = raw_text.data();

    // process text, which may have more than one input due to use of ASCII NUL ('\0')
    int count = 0;
    while (ptext) {

        // form the parsing request
        ParseRequest request;

        if (option(SRCML_COMMAND_NOARCHIVE))
            request.disk_dir = srcml_request.output_filename.resource;

        request.filename = srcml_request.att_filename;
        request.url = srcml_request.att_url;
        request.version = srcml_request.att_version;
        request.srcml_arch = srcml_arch;
        request.language = srcml_request.att_language ? *srcml_request.att_language : "";

        if (request.language.empty())
            if (const char* l = srcml_archive_check_extension(srcml_arch, request.filename->data()))
                request.language = l;

        request.status = 0;

        // fill up the parse request buffer
        if (!request.status) {

            // copy from the text directly into a buffer
            // perform newline and tab expansion
            while (ptext) {

                // find up to an escape
                const char* epos = strchr(ptext, '\\');
                if (!epos) {
                    break;
                }
                // append up to the special char
                request.buffer.insert(request.buffer.end(), ptext, epos);

                // append the special character
                ++epos;
                switch (*epos) {
                case 'n':
                    request.buffer.push_back('\n');
                    break;
                case 't':
                    request.buffer.push_back('\t');
                    break;
                case 'f':
                    request.buffer.push_back('\f');
                    break;
                case 'a':
                    request.buffer.push_back('\a');
                    break;
                case 'b':
                    request.buffer.push_back('\b');
                    break;
                /* \e not directly supported in C, but echo command does */
                case 'e':
                    request.buffer.push_back('\x1B');
                    break;
                case 'r':
                    request.buffer.push_back('\r');
                    break;
                case 'v':
                    request.buffer.push_back('\v');
                    break;
                // byte with hex value from 1 to 2 charcters
                case 'x':
                {
                    int value = 0;
                    int offset = 0;
                    while (offset < 2 && isxdigit(*(epos + offset + 1))) {
                        value = hex2decimal((unsigned char) *(epos + offset + 1)) + 16 * value;
                        ++offset;
                    }
                    if (offset == 0) {
                        request.buffer.push_back('\\');
                        request.buffer.push_back('x');
                        break;
                    }

                    if (value == 0) {
                        ptext = epos + offset + 1;

                        srcml_archive_disable_solitary_unit(srcml_arch);

                        goto end;
                    }

                    request.buffer.push_back((char) value);

                    epos += offset;
                    break;
                }
                // byte with octal value from 1 to 3 characters
                // Note: GNU echo documentation says that the \0 is required, but
                // the actual implementation accepts \1, \2, ..., \7
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                {
                    int value = 0;
                    int offset = *epos == '0' ? 1 : 0;
                    /*
                        Spec 6.4.4.4 Character constants:

                        octal-escape-sequence:
                        \ octal-digit
                        \ octal-digit octal-digit
                        \ octal-digit octal-digit octal-digit

                        However, man echo (man gecho) states:

                        \0NNN  byte with octal value NNN (1 to 3 digits)

                        So, we will allow both
                    */
                    int maxlength = *epos == '0' ? 4 : 3;
                    while (offset < maxlength && isodigit(*(epos + offset))) {
                        value = (*(epos + offset) - '0') + 8 * value;
                        ++offset;
                    }
                    if (offset == 0) {
                        request.buffer.push_back('\\');
                        request.buffer.push_back('0');
                        break;
                    }

                    if (value == 0) {
                        ptext = epos + offset;

                        srcml_archive_disable_solitary_unit(srcml_arch);

                        goto end;
                    }

                    request.buffer.push_back(static_cast<char>(value));

                    epos += offset - 1;
                    break;
                }
                default:
                    request.buffer.push_back('\\');
                    request.buffer.push_back(*(epos));
                }
                ptext = epos + 1;
            }

            // finished with no '\\' remaining, so flush buffer
            request.buffer.insert(request.buffer.end(), ptext, ptext + strlen(ptext));
            ptext = 0;
        }

        // schedule for parsing
end:    count += 1;

        queue.schedule(std::move(request));
    }

    return count;
}
