/**
 * @file libxml2_callback_wrappers.cpp
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

#include <libxml2_callback_wrappers.hpp>

/**
 * write_callback_wrapper
 * @param context the input context
 * @param buffer the buffer to write
 * @param len the number of bytes to write
 *
 * Wrapper around libsrcml write callback wrapper to conform to libxml2.
 *
 * @returns the number of bytes written or -1 in case of error.
 */
int write_callback_wrapper(void * context, const char * buffer, int len) {

	libxml2_write_context & write_context = *(libxml2_write_context *)context;

	return write_context.write_callback(write_context.context, buffer, (size_t)len);

}

/**
 * write_close_callback_wrapper
 * @param context the input context
 *
 * Wrapper around libsrcml close callback to close the write_callback_wrapper context.
 *
 * @returns 0 or -1 on error.
 */
int write_close_callback_wrapper(void * context) {

	libxml2_write_context & write_context = *(libxml2_write_context *)context;

	return write_context.close_callback(write_context.context);

}
