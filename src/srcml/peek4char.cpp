/**
 * @file peek4char.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

#include <peek4char.hpp>


void peek4char(FILE* fp, unsigned char data[], ssize_t* psize) {
    *psize = 0;
    int c;
    if ((c = getc(fp)) != EOF) {
        data[0] = c;
        ++(*psize);
        if ((c = getc(fp)) != EOF) {
            data[1] = c;
            ++(*psize);
            if ((c = getc(fp)) != EOF) {
                data[2] = c;
                ++(*psize);
                if ((c = getc(fp)) != EOF) {
                    data[3] = c;
                    ++(*psize);
                }
            }
        }
    }

    for (ssize_t i = (*psize) - 1; i >= 0; --i)
        ungetc(data[i], fp);
}
