/**
 * @file srcml_sax2_utilities.hpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

#ifndef INCLUDED_SRCML_SAX2_UTILITIES_HPP
#define INCLUDED_SRCML_SAX2_UTILITIES_HPP

#include <srcml_types.hpp>
#include <srcml_macros.hpp>
#include <srcml.h>

#include <libxml/parserInternals.h>

int srcml_extract_text(const char * input_buffer, size_t size, xmlOutputBufferPtr output_buffer, OPTION_TYPE options, int unit = 0);
/// @todo needs test cases
int srcml_extract_text_filename(const char * ifilename, const char * ofilename, const char * encoding, unsigned short compression, int unit = 0);
int srcml_xpath(xmlParserInputBufferPtr input_buffer, const char * context_element, const char * xpath,
				const char * prefix, const char * uri, const char * element, const char * attr_prefix, const char * attr_uri, const char * attr_name, const char * attr_value,
				OPTION_TYPE options, srcml_archive* out_archive);
#ifdef WITH_LIBXSLT
int srcml_xslt(xmlParserInputBufferPtr input_buffer, const char* context_element, xmlDocPtr xslt, const char* params[], int paramcount, OPTION_TYPE options, srcml_archive* out_archive);
#endif
int srcml_relaxng(xmlParserInputBufferPtr input_buffer, xmlDocPtr relaxng, OPTION_TYPE options, srcml_archive* out_archive);

#endif
