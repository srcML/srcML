/**
 * @file isxml.cpp
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

#include <isxml.hpp>
#include <boost/static_assert.hpp>
#include <stdint.h>
#include <peek4char.hpp>

// determine if XML from first four bytes in almost any encoding
bool isxml(unsigned char ar[], int size);

// determine if XML
bool isxml(FILE* infd) {

    // determine if the input is srcML or src from the first up-to 4 bytes
    unsigned char data[4];

    return isxml(data, peek4char(infd, data));
}

// determine if XML from first four bytes in almost any encoding
bool isxml(unsigned char ar[], int size) {

    // bail if not valid array
    if (!ar)
        return false;

    // xml is always at least 4 bytes for xml declaration, i.e., '<?xm'
    // or for xml without xml declaration, i.e., '<uni'
    if (size < 4)
        return false;

    // treat unsigned int field as just 4 bytes regardless of endianness
    union { unsigned char d[4]; uint32_t i; } data = { { ar[0], ar[1], ar[2], ar[3] } };
    // data.d[0] = ar[0]; data.d[1] = ar[1]; data.d[2] = ar[2]; data.d[3] = ar[3];

    // determine if XML by looking for xml declaration (i.e., '<?xm')
    // in first four bytes in almost any encoding
//    fprintf(stderr, "DEBUG:  %s %s %d DATA: %x\n", __FILE__,  __FUNCTION__, __LINE__, data.i);

    return
        (data.i == 0x6D783F3C) || // '<?xm' UTF-8
        (data.i == 0x696E753C) || // '<uni' UTF-8
        (data.i == 0x003F003C) || // '<?xm' UTF-16LE
        (data.i == 0x0075003C) || // '<uni' UTF-16LE
        (data.i == 0x3F003C00) || // '<?xm' UTF-16BE
        (data.i == 0x75003C00) || // '<uni' UTF-16BE
        (data.i == 0x3C000000) || // <?xm' UCS-4BE
//        (data.i == 0x3C000000) || // '<uni' UCS-4BE
        (data.i == 0x0000003C) || // '<?xm' UCS-4LE
//        (data.i == 0x0000003C) || // '<uni' UCS-4LE
        (data.i == 0x003C0000) || // '<?xm' UCS-4_2143
//        (data.i == 0x003C0000) || // '<uni' UCS-4_2143
        (data.i == 0x00003C00) || // '<?xm' UCS-4_3412
//        (data.i == 0x00003C00) || // '<uni' UCS-4_3412
        (data.i == 0x94A76F4C) || // '<?xm' EBCDIC
//        (data.i == 0x94A76F4C) || // '<uni' EBCDIC
        false;
}

