/**
 * @file SAX2Framework_utilities.hpp
 * @copyright
 *
 * Copyright (C) 2013  SDML (www.srcML.org)
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

#include <libxml/parserInternals.h>

#ifndef INCLUDDED_SAX2FRAMEWORK_UTILITIES_HPP
#define INCLUDDED_SAX2FRAMEWORK_UTILITIES_HPP

xmlParserCtxtPtr SAX2FrameworkCreateParserCtxt(xmlParserInputBufferPtr buffer_input);

void internal_stop_parser(xmlParserCtxtPtr ctxt);

#define CHECK_COPY(ORIGINAL, COPY) if(ORIGINAL && !COPY) { fprintf(stderr, "ERROR allocating memory"); internal_stop_parser(ctxt); return; }

#endif
