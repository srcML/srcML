/**
 * @file SAX2Framework_utilities.cpp
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

#include <SAX2Framework_utilities.hpp>

/**
 * srcMLCreateParserCtxt
 * @param buffer_input a parser input buffer
 *
 * Create a ctxt from a parser input buffer.
 * Modeled after function in libxml2.
 *
 * @returns xml parser ctxt
 */
xmlParserCtxtPtr
SAX2FrameworkCreateParserCtxt(xmlParserInputBufferPtr buffer_input, const char * encoding) {
    xmlParserCtxtPtr ctxt;
    xmlParserInputPtr input;

    ctxt = xmlNewParserCtxt();
    if (ctxt == NULL) return NULL;

    input = xmlNewIOInputStream(ctxt, buffer_input, xmlParseCharEncoding(encoding));

    inputPush(ctxt, input);

    return ctxt;
}

void internal_stop_parser(xmlParserCtxtPtr ctxt) {

    ctxt->sax->startDocument = 0;
    ctxt->sax->endDocument = 0;
    ctxt->sax->startElementNs = 0;
    ctxt->sax->endElementNs = 0;
    ctxt->sax->characters = 0;
    ctxt->sax->cdataBlock = 0;
    ctxt->sax->comment = 0;
    ctxt->sax->ignorableWhitespace = 0;

    xmlStopParser(ctxt);

}
