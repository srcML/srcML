// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file sha1utilities.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 */

#ifndef SHA1_UTILITIES_HPP
#define SHA1_UTILITIES_HPP

#include <TinySHA1.hpp>
#include <type_traits>

static constexpr char hexchar[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
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
static_assert(sizeof(hexchar) == 16, "Wrong size for hex conversion");

#endif
