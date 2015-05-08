/**
 * @file srcml_consume.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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
#include <srcml_options.hpp>
#include <srcml_cli.hpp>
#include <string>
#include <boost/filesystem.hpp>

// creates initial unit, parses, and then sends unit to write queue
void srcml_consume(ParseRequest* request, WriteQueue* write_queue) {

    // error passthrough to output for proper output in trace
    if (request->status) {
        request->unit = 0;
        write_queue->schedule(request);
        return;
    }

    // NOTE: thread task cannot throw exception

    // global access to options
    bool isseparatearchive = SRCML_COMMAND_NOARCHIVE & SRCMLOptions::get();

    // current output archive
    srcml_archive* srcml_arch = request->srcml_arch;
    if (isseparatearchive) {
        srcml_arch = srcml_archive_clone(request->srcml_arch);
        srcml_archive_disable_option(srcml_arch, SRCML_OPTION_ARCHIVE);
        srcml_archive_enable_option(srcml_arch, SRCML_OPTION_HASH);

        //Build the output filename

        //Filenames from directories come in as full paths
        //Setup the directory on the filesystem
        boost::filesystem::create_directories(*request->disk_dir);

        size_t pos = request->filename->find_last_of("/\\");

        if (pos != std::string::npos) {
            ++pos;
        }
        else {
            pos = 0;
        }
        
        //Ensure that the directory path has a final "/" when appended to filename
        if (request->disk_dir->at(request->disk_dir->length() -1) != '/')
            *request->disk_dir += "/";

        std::string xml_filename = *request->disk_dir + request->filename->substr(pos) + ".xml";
        srcml_archive_write_open_filename(srcml_arch, xml_filename.c_str(), 0);
        request->srcml_arch = srcml_arch;
    }

    std::string original_filename;

    // construct and parse the unit
    srcml_unit* unit = request->unit;
    int status = SRCML_STATUS_OK;
    try {

        // create the unit start tag
        if (!unit)
            if (!(unit = srcml_unit_create(srcml_arch)))
                throw SRCML_STATUS_ERROR;

        // language attribute, required if from memory
        if ((status = srcml_unit_set_language(unit, request->language.c_str())) != SRCML_STATUS_OK)
            throw status;

        // (optional) directory attribute
        if (!(srcml_archive_get_options(srcml_arch) & SRCML_OPTION_ARCHIVE) && request->url && ((status = srcml_unit_set_url(unit, request->url->c_str())) != SRCML_STATUS_OK))
            throw status;

        // (optional) filename attribute
        if (request->filename) {

            original_filename = *request->filename;
            
            // Cleanup filename
            while (request->filename->at(0) == '.' || request->filename->at(0) == '/') {
                request->filename->erase(0,1);
            }
            
            if ((status = srcml_unit_set_filename(unit, request->filename->c_str())) != SRCML_STATUS_OK)
                throw status;
        }

        // (optional) version attribute
        if (request->version && ((status = srcml_unit_set_version(unit, request->version->c_str())) != SRCML_STATUS_OK))
            throw status;

        if (request->time_stamp)
            srcml_unit_set_timestamp(unit, request->time_stamp->c_str());

        // sha1 attribute, if hash is on
        // sha1 value based on the code as encoded (source text encoding) in the original file
        if (!request->disk_filename && (SRCML_COMMAND_NOARCHIVE & SRCMLOptions::get())) {
        
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

//            fprintf(stderr, "DEBUG:  %s %s %d DATA: %s\n", __FILE__,  __FUNCTION__, __LINE__, outmd);
            
            srcml_unit_set_hash(unit, outmd);
        }
        
        // parse the buffer/file (unless it is already form a srcml archive)
        if (request->disk_filename)
            status = srcml_unit_parse_filename(unit, request->disk_filename->c_str());
        else if (!request->unit)
            status = srcml_unit_parse_memory(unit, &request->buffer.front(), request->buffer.size());

        if (status != SRCML_STATUS_OK)
            // FIXME: Cannot throw exception from thread
            throw status;

    } catch (...) {

        fprintf(stderr, "srcml: Unable to open file %s\n", original_filename.c_str());
        if (unit)
            srcml_unit_free(unit);
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
