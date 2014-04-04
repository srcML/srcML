/**
 * @file srcml_consume.cpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

/*
  srcml_consume calls appropriate libsrcml functions for processing source file data
*/

#ifdef __MACH__
#include <CommonCrypto/CommonDigest.h>
#define SHA1(a,b,c) CC_SHA1(a,b,c)
#define SHA_DIGEST_LENGTH CC_SHA1_DIGEST_LENGTH
#define SHA_LONG CC_LONG
#else
#include <openssl/sha.h>
#endif
#include <srcml_consume.hpp>
#include <srcml.h>
#include <parse_request.hpp>
#include <parse_queue.hpp>
#include <write_queue.hpp>
#include <iomanip>
#include <boost/static_assert.hpp>
#include <sha1utilities.hpp>

// Public consumption thread function
void srcml_consume(ParseRequest* ppr, WriteQueue* wqueue) {

        // build and parse
    srcml_unit* unit = 0;

    int status = ppr->status;
    if (!ppr->status) {

        unit = srcml_create_unit(ppr->srcml_arch);
        if (ppr->filename)
            srcml_unit_set_filename(unit, ppr->filename->c_str());
        if (ppr->directory)
            srcml_unit_set_directory(unit, ppr->directory->c_str());
        if (ppr->version)
            srcml_unit_set_version(unit, ppr->version->c_str());
        srcml_unit_set_language(unit, ppr->language.c_str());

        // compute the SHA1 has for this unit
        // based on the code as encoding in the original file
        unsigned char md[SHA_DIGEST_LENGTH];
        SHA1((const unsigned char*)&ppr->buffer.front(), (SHA_LONG)ppr->buffer.size(), md);

        // convert to hex ascii string
        const char outmd[] = { HEXCHARASCII(md), '\0' };
        BOOST_STATIC_ASSERT_MSG(sizeof(outmd)/sizeof(outmd[0]) == (SHA_DIGEST_LENGTH * 2 + 1),
                                "Wrong size for SHA_DIGEST_LENGTH conversion");
        //srcml_unit_set_hash(unit, outmd);

        if (!ppr->disk_filename) {
            status = srcml_parse_unit_memory(unit, &ppr->buffer.front(), ppr->buffer.size());
        } else {
            status = srcml_parse_unit_filename(unit, ppr->disk_filename->c_str());
        }
    }

    // schedule for output
    ppr->unit = unit;
    wqueue->schedule(ppr);
}
