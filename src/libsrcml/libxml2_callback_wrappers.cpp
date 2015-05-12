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

/**
 * read_callback_wrapper
 * @param context the input context
 * @param buffer the buffer to read into
 * @param len the number of bytes to read
 *
 * Wrapper around libsrcml read callback wrapper to conform to libxml2.
 *
 * @returns the number of bytes read or -1 in case of error.
 */
int read_callback_wrapper(void * context, char * buffer, int len) {

	libxml2_read_context & read_context = *(libxml2_read_context*)context;

	return read_context.read_callback(read_context.context, buffer, (size_t)len);

}

/**
 * read_close_callback_wrapper
 * @param context the input context
 *
 * Wrapper around libsrcml close callback to close the read_callback_wrapper context.
 *
 * @returns 0 or -1 on error.
 */
int read_close_callback_wrapper(void * context) {

	libxml2_read_context & read_context = *(libxml2_read_context*)context;

	return read_context.close_callback(read_context.context);

}
