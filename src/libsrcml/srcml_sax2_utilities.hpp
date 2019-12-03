/**
 * @file srcml_sax2_utilities.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef INCLUDED_SRCML_SAX2_UTILITIES_HPP
#define INCLUDED_SRCML_SAX2_UTILITIES_HPP

#include <srcml_types.hpp>
#include <srcml_macros.hpp>
#include <srcml.h>

#include <libxml/parserInternals.h>

/**
 * dlopen_libxslt
 *
 * Get a handle to libxslt dynamically loaded
 *
 * @returns Return handle on success and NULL on failure
 */
void* dlopen_libxslt();

/**
 * dlopen_libexslt
 *
 * Get a handle to libexslt dynamically loaded
 *
 * @returns Return handle on success and NULL on failure
 */
void* dlopen_libexslt();

int srcml_xpath(xmlParserInputBufferPtr input_buffer, const char * context_element, const char * xpath,
                const char * prefix, const char * uri, const char * element, const char * attr_prefix, const char * attr_uri, const char * attr_name, const char * attr_value,
                OPTION_TYPE options, srcml_archive* oarchive);

int srcml_relaxng(xmlParserInputBufferPtr input_buffer, xmlDocPtr relaxng, OPTION_TYPE options, srcml_archive* oarchive);

#endif
