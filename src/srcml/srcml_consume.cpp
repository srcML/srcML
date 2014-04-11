/**
 * @file srcml_consume.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Thread-designed function to parse and create a unit.
 * Output is scheduled for writing on the write queue
 */

#include <srcml_consume.hpp>
#include <srcml.h>
#include <parse_request.hpp>
#include <write_queue.hpp>
#include <boost/static_assert.hpp>
#include <sha1utilities.hpp>

// creates initial unit, parses, and then sends unit to write queue
void srcml_consume(ParseRequest* request, WriteQueue* write_queue) {

    // error passthrough to output for proper output in trace
    if (request->status) {
        request->unit = 0;
        write_queue->schedule(request);
        return;
    }

    // NOTE: thread task cannot throw exception

    // construct and parse the unit
    srcml_unit* unit = 0;
    int status = SRCML_STATUS_OK;
    try {
        // create the unit start tag
        if (!(unit = srcml_create_unit(request->srcml_arch)))
            throw SRCML_STATUS_ERROR;

        // language attribute, required if from memory
        if ((status = srcml_unit_set_language(unit, request->language.c_str())) != SRCML_STATUS_OK)
            throw status;

        // (optional) directory attribute
        if (request->directory && ((status = srcml_unit_set_directory(unit, request->directory->c_str())) != SRCML_STATUS_OK))
            throw status;

        // (optional) filename attribute
        if (request->filename && ((status = srcml_unit_set_filename(unit, request->filename->c_str())) != SRCML_STATUS_OK))
            throw status;

        // (optional) version attribute
        if (request->version && ((status = srcml_unit_set_version(unit, request->version->c_str())) != SRCML_STATUS_OK))
            throw status;

        // sha1 attribute, if hash is on
        // sha1 value based on the code as encoded (source text encoding) in the original file
        if (srcml_archive_get_options(request->srcml_arch) & SRCML_OPTION_HASH) {

#ifdef _MSC_BUILD
            unsigned char md[20];
            /** msvc hash provider object */
            HCRYPTPROV   crypt_provider;
            BOOL success = CryptAcquireContext(&crypt_provider, NULL, NULL, PROV_RSA_FULL, 0);
            if(! success && GetLastError() == NTE_BAD_KEYSET)
                success = CryptAcquireContext(&crypt_provider, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);
            /** msvc hash object */
            HCRYPTHASH   crypt_hash;
            CryptCreateHash(crypt_provider, CALG_SHA1, 0, 0, &crypt_hash);
            CryptHashData(crypt_hash, (BYTE *)&request->buffer.front(), request->buffer.size(), 0);
            DWORD        SHA_DIGEST_LENGTH;
            DWORD        hash_length_size = sizeof(DWORD);
            CryptGetHashParam(crypt_hash, HP_HASHSIZE, (BYTE *)&SHA_DIGEST_LENGTH, &hash_length_size, 0);
            CryptGetHashParam(crypt_hash, HP_HASHVAL, (BYTE *)md, &SHA_DIGEST_LENGTH, 0);
            CryptDestroyHash(crypt_hash);
            CryptReleaseContext(crypt_provider, 0);
#else
            unsigned char md[SHA_DIGEST_LENGTH];
            if (SHA1((const unsigned char*)&request->buffer.front(), (SHA_LONG)request->buffer.size(), md) == 0)
                throw SRCML_STATUS_ERROR;
#endif

            const char outmd[] = { HEXCHARASCII(md), '\0' };

#ifndef _MSC_BUILD
            BOOST_STATIC_ASSERT_MSG(sizeof(outmd)/sizeof(outmd[0]) == (SHA_DIGEST_LENGTH * 2 + 1),
                "Wrong size for SHA_DIGEST_LENGTH conversion");
#endif
            
            srcml_unit_set_hash(unit, outmd);
        }

        // parse the buffer/file
        status = request->disk_filename ?
                                    srcml_parse_unit_filename(unit, request->disk_filename->c_str()) :
                                    srcml_parse_unit_memory(unit, &request->buffer.front(), request->buffer.size());
        if (status != SRCML_STATUS_OK)
            throw status;

    } catch (...) {
        std::cerr << "Error in constructing srcml\n";
        if (unit)
            srcml_free_unit(unit);
        unit = 0;
        if (request)
            delete request;
        request = 0;
        return;
    }

    // schedule unit for output
    request->unit = unit;
    request->status = status;
    write_queue->schedule(request);
}
