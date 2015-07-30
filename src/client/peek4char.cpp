/**
 * @file peek4char.cpp
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

#include <peek4char.hpp>

int peek4char(FILE* fp, unsigned char data[]) {
    int size = 0;
    int c;
    if ((c = getc(fp)) != EOF) {
        data[0] = c;
        ++size;
        if ((c = getc(fp)) != EOF) {
            data[1] = c;
            ++size;
            if ((c = getc(fp)) != EOF) {
                data[2] = c;
                ++size;
                if ((c = getc(fp)) != EOF) {
                    data[3] = c;
                    ++size;
                    ungetc(data[3], fp);
                }
                ungetc(data[2], fp);
            }
            ungetc(data[1], fp);
        }
        ungetc(data[0], fp);
    }
    return size;
}
