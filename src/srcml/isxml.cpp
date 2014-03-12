/**
 * @file isxml.cpp
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

#include <isxml.hpp>
#include <boost/static_assert.hpp>

// determine if XML from first four bytes in almost any encoding
bool isxml(unsigned char ar[], ssize_t size) {

    BOOST_STATIC_ASSERT_MSG(sizeof(unsigned int) == 4, "Size of unsigned int needs to be 4");

    if (size < 4)
        return false;

    // treat unsigned int field as just 4 bytes regardless of endianness
    union { unsigned int i; unsigned char d[4]; } data;
    data.d[0] = ar[0];
    data.d[1] = ar[1];
    data.d[2] = ar[2];
    data.d[3] = ar[3];

    // determine if XML by looking for xml declaration in first four bytes in almost any encoding
    return
        (data.i == 0x3C000000) || // UCS-4BE
        (data.i == 0x0000003C) || // UCS-4LE
        (data.i == 0x003C0000) || // UCS-4_2143
        (data.i == 0x00003C00) || // UCS-4_3412
        (data.i == 0x94A76F4C) || // EBCDIC
        (data.i == 0x6D783F3C) || // UTF-8
        (data.i == 0x003F003C) || // UTF-16LE
        (data.i == 0x3F003C00);   // UTF-16BE
}

