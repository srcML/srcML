/**
 * @file srcml_sax2_utilities.hpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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

#ifndef INCLUDDED_SRCML_SAX2_UTILITIES_HPP
#define INCLUDDED_SRCML_SAX2_UTILITIES_HPP

#include <UTF8OutputSource.hpp>

#include <Options.hpp>

#include <libxml/parserInternals.h>

int srcml_extract_text(const char * input_buffer, size_t size, UTF8OutputSource & output_handler, OPTION_TYPE options, int unit = 0);
int srcml_extract_text_filename(const char * ifilename, const char * ofilename, const char * encoding, OPTION_TYPE options, int unit = 0);
int srcml_xpath(xmlParserInputBufferPtr input_buffer, const char* context_element, const char* xpaths[], int fd, OPTION_TYPE options);
int srcml_xslt(xmlParserInputBufferPtr input_buffer, const char* context_element, const char* xslts[], const char* params[], int paramcount, int fd, OPTION_TYPE options);
int srcml_relaxng(xmlParserInputBufferPtr input_buffer, const char** xslts, int fd, OPTION_TYPE options);

// local function forward declarations
xmlParserCtxtPtr srcMLCreateMemoryParserCtxt(const char * buffer, int size);
int srcMLParseDocument(xmlParserCtxtPtr ctxt, bool allowendearly);
xmlParserCtxtPtr srcMLCreateParserCtxt(xmlParserInputBufferPtr buffer_input);

#endif
