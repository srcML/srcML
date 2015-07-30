/**
 * @file sha1utilities.hpp
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
 */

#ifndef SHA1_UTILITIES_HPP
#define SHA1_UTILITIES_HPP

#ifdef _MSC_BUILD
#include <windows.h>
#include <Wincrypt.h>
#elif defined(__MACH__)
#include <CommonCrypto/CommonDigest.h>
#define SHA1(a,b,c) CC_SHA1(a,b,c)
#define SHA_DIGEST_LENGTH CC_SHA1_DIGEST_LENGTH
#define SHA_LONG CC_LONG
#else
#include <openssl/sha.h>
#endif

static const char hexchar[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
#define HEXCHARASCII(md) \
            hexchar[md[0]  >> 4], hexchar[md[0]  & 0x0F], \
            hexchar[md[1]  >> 4], hexchar[md[1]  & 0x0F], \
            hexchar[md[2]  >> 4], hexchar[md[2]  & 0x0F], \
            hexchar[md[3]  >> 4], hexchar[md[3]  & 0x0F], \
            hexchar[md[4]  >> 4], hexchar[md[4]  & 0x0F], \
            hexchar[md[5]  >> 4], hexchar[md[5]  & 0x0F], \
            hexchar[md[6]  >> 4], hexchar[md[6]  & 0x0F], \
            hexchar[md[7]  >> 4], hexchar[md[7]  & 0x0F], \
            hexchar[md[8]  >> 4], hexchar[md[8]  & 0x0F], \
            hexchar[md[9]  >> 4], hexchar[md[9]  & 0x0F], \
            hexchar[md[10] >> 4], hexchar[md[10] & 0x0F], \
            hexchar[md[11] >> 4], hexchar[md[11] & 0x0F], \
            hexchar[md[12] >> 4], hexchar[md[12] & 0x0F], \
            hexchar[md[13] >> 4], hexchar[md[13] & 0x0F], \
            hexchar[md[14] >> 4], hexchar[md[14] & 0x0F], \
            hexchar[md[15] >> 4], hexchar[md[15] & 0x0F], \
            hexchar[md[16] >> 4], hexchar[md[16] & 0x0F], \
            hexchar[md[17] >> 4], hexchar[md[17] & 0x0F], \
            hexchar[md[18] >> 4], hexchar[md[18] & 0x0F], \
            hexchar[md[19] >> 4], hexchar[md[19] & 0x0F]
BOOST_STATIC_ASSERT_MSG(sizeof(hexchar) == 16, "Wrong size for hex conversion");

#endif
