/**
 * @file srctools_windows.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
 *
 * srcTools are free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * srcTools are distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <srctools_windows.hpp>

#include <cstdlib>
#include <cstring>

#ifdef _MSC_BUILD

char * strndup(const char * source, size_t n) {

	if(source == 0) return 0;

	char * dup = (char *)malloc((n + 1) * sizeof(char));
	strncpy(dup, source, n);
	dup[n] = 0;

	return dup;

}

#endif