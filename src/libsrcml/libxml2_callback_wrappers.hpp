/**
 * @file libxml2_callback_wrappers.hpp
 *
 * @copyright Copyright (C) 2015 srcML, LLC. (www.srcML.org)
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

#ifndef INCLUDED_LIBXML2_CALLBACK_WRAPPERS_HPP
#define INCLUDED_LIBXML2_CALLBACK_WRAPPERS_HPP

#include <stddef.h>

struct libxml2_write_context {

	void * context;
	int (*write_callback)(void * context, const char * buffer, size_t len);
	int (*close_callback)(void * context);

};

struct libxml2_read_context {

	void * context;
	int (*read_callback)(void * context, char * buffer, size_t len);
	int (*close_callback)(void * context);

};

int write_callback_wrapper(void * context, const char * buffer, int len);
int write_close_callback_wrapper(void * context);

int read_callback_wrapper (void * context, char * buffer, int len);
int read_close_callback_wrapper(void * context);

#endif
