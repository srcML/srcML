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
#include <src_input_libarchive.hpp>
#include <boost/thread.hpp>
#include <parse_queue.hpp>
#include <write_request.hpp>
#include <write_queue.hpp>
#include <iomanip>
#include <boost/static_assert.hpp>

// Public consumption thread function
void srcml_consume(ParseQueue* queue, WriteQueue* wqueue) {

    while (true) {
        ParseRequest pr;
        queue->pop(pr);

        // Check if termination queue item has been found
        if (pr.position == 0)
            break;

        // build and parse
        srcml_unit* unit = 0;

        if (!pr.status) {
            unit = srcml_create_unit(pr.srcml_arch);
            if (pr.filename)
                srcml_unit_set_filename(unit, pr.filename->c_str());
            if (pr.directory)
                srcml_unit_set_directory(unit, pr.directory->c_str());
            if (pr.version)
                srcml_unit_set_version(unit, pr.version->c_str());
            srcml_unit_set_language(unit, pr.language.c_str());

            // compute the SHA1 has for this unit
            // based on the code as encoding in the original file
            unsigned char md[SHA_DIGEST_LENGTH];
            SHA1((const unsigned char*)&pr.buffer.front(), (SHA_LONG)pr.buffer.size(), md);

            // convert to hex ascii string
            static const char hexchar[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
            BOOST_STATIC_ASSERT_MSG(sizeof(hexchar) == 16, "Wrong size for hex conversion");
            const char outmd[] = {
                hexchar[md[0] >> 4], hexchar[md[0] & 0x0F],
                hexchar[md[1] >> 4], hexchar[md[1] & 0x0F],
                hexchar[md[2] >> 4], hexchar[md[2] & 0x0F],
                hexchar[md[3] >> 4], hexchar[md[3] & 0x0F],
                hexchar[md[4] >> 4], hexchar[md[4] & 0x0F],
                hexchar[md[5] >> 4], hexchar[md[5] & 0x0F],
                hexchar[md[6] >> 4], hexchar[md[6] & 0x0F],
                hexchar[md[7] >> 4], hexchar[md[7] & 0x0F],
                hexchar[md[8] >> 4], hexchar[md[8] & 0x0F],
                hexchar[md[9] >> 4], hexchar[md[9] & 0x0F],
                hexchar[md[10] >> 4], hexchar[md[10] & 0x0F],
                hexchar[md[11] >> 4], hexchar[md[11] & 0x0F],
                hexchar[md[12] >> 4], hexchar[md[12] & 0x0F],
                hexchar[md[13] >> 4], hexchar[md[13] & 0x0F],
                hexchar[md[14] >> 4], hexchar[md[14] & 0x0F],
                hexchar[md[15] >> 4], hexchar[md[15] & 0x0F],
                hexchar[md[16] >> 4], hexchar[md[16] & 0x0F],
                hexchar[md[17] >> 4], hexchar[md[17] & 0x0F],
                hexchar[md[18] >> 4], hexchar[md[18] & 0x0F],
                hexchar[md[19] >> 4], hexchar[md[19] & 0x0F],
                '\0'
            };
            BOOST_STATIC_ASSERT_MSG(sizeof(outmd)/sizeof(outmd[0]) == (SHA_DIGEST_LENGTH * 2 + 1),
                                    "Wrong size for SHA_DIGEST_LENGTH conversion");
            //            srcml_unit_set_hash(unit, outmd);

            if (!pr.disk_filename) {
                pr.status = srcml_parse_unit_memory(unit, &pr.buffer.front(), pr.buffer.size());
            } else {
                pr.status = srcml_parse_unit_filename(unit, pr.disk_filename->c_str());
            }
        }

        // write unit
        WriteRequest wr;
        wr.srcml_arch = pr.srcml_arch;
        wr.unit = unit;
        wr.position = pr.position;
        wr.filename = pr.filename;
        wr.status = pr.status;
        wqueue->push(wr);
    }
}
