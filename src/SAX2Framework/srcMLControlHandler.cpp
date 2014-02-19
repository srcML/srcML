/*
  srcMLControlHandler.cpp

  Copyright (C) 2004-2013  SDML (www.srcML.org)

  This file is part of the srcML SAX2 Framework.

  The srcML SAX2 Framework is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML SAX2 Framework is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML SAX2 Framework; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <srcMLControlHandler.hpp>

#include <SAX2Framework_utilities.hpp>

#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void libxml_error(void *ctx, const char *msg, ...) {}

#pragma GCC diagnostic pop

void srcml_control_handler_init() {

    static bool initialized = false;

    if(initialized) return;

    xmlGenericErrorFunc error_handler = (xmlGenericErrorFunc) libxml_error;
    initGenericErrorDefaultFunc(&error_handler);
    initialized = true;

}

/**
 * srcMLControlHandler
 * @param filename name of a file
 *
 * Constructor
 */
srcMLControlHandler::srcMLControlHandler(const char * filename) : sax2_handler(), pop_input(false) {

    srcml_control_handler_init();

    ctxt = xmlCreateURLParserCtxt(filename, XML_PARSE_COMPACT | XML_PARSE_HUGE);
    if(ctxt == NULL) throw std::string("File does not exist");
    sax = factory();

}


/**
 * srcMLControlHandler
 * @param input pointer to a parser input buffer
 *
 * Constructor
 */
srcMLControlHandler::srcMLControlHandler(xmlParserInputBufferPtr input) : sax2_handler(), pop_input(true) {

    srcml_control_handler_init();

    ctxt = SAX2FrameworkCreateParserCtxt(input);
    if(ctxt == NULL) throw std::string("File does not exist");
    sax = factory();

}

/**
 * ~srcMLControlHandler
 *
 * Constructor
 */
srcMLControlHandler::~srcMLControlHandler() {

    if(pop_input) inputPop(ctxt);
    if(ctxt) xmlFreeParserCtxt(ctxt);

}

/**
 * getSAX
 *
 * Return the used sax handler.
 */
const xmlSAXHandler & srcMLControlHandler::getSAX() const {

    return sax;

}

/**
 * enable_startDocument
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables startDocument parsing.
 */
void srcMLControlHandler::enable_startDocument(bool enable) {

    if(enable) sax.startDocument = startDocument;
    else sax.startDocument = 0;

}

/**
 * enable_endDocument
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables endDocument parsing.
 */
void srcMLControlHandler::enable_endDocument(bool enable) {

    if(enable) sax.endDocument = endDocument;
    else sax.endDocument = 0;

}

/**
 * enable_startElementNs
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables startElementNs parsing.
 */
void srcMLControlHandler::enable_startElementNs(bool enable) {

    if(enable) sax.startElementNs = startRoot;
    else sax.startElementNs = 0;

}

/**
 * enable_endElementNs
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables endElementNs parsing.
 */
void srcMLControlHandler::enable_endElementNs(bool enable) {

    if(enable) sax.endElementNs = endElementNs;
    else sax.endElementNs = 0;

}

/**
 * enable_characters
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables characters parsing.
 */
void srcMLControlHandler::enable_characters(bool enable) {

    if(enable) sax.characters = charactersFirst;
    else sax.characters = 0;

}

/**
 * enable_comment
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables comment parsing.
 */
void srcMLControlHandler::enable_comment(bool enable) {

    if(enable) sax.comment = comment;
    else sax.comment = 0;

}

/**
 * enable_cdataBlock
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables cdataBlock parsing.
 */
void srcMLControlHandler::enable_cdataBlock(bool enable) {

    if(enable) sax.cdataBlock = cdataBlock;
    else sax.cdataBlock = 0;

}

/**
 * parse
 * @param handler srcMLHandler with hooks for sax parsing
 *
 * Parse the xml document with the supplied hooks.
 */
void srcMLControlHandler::parse(srcMLHandler * handler) {

    sax2_handler.process = handler;

    xmlSAXHandlerPtr save_sax = ctxt->sax;
    ctxt->sax = &sax;
    ctxt->_private = &sax2_handler;

    int status = xmlParseDocument(ctxt);
    ctxt->sax = save_sax;

    if(status != 0) {

        xmlErrorPtr ep = xmlCtxtGetLastError(ctxt);

        size_t str_length = strlen(ep->message);
        ep->message[str_length - 1] = '\0';
        SAXError error = { std::string((const char *)ep->message), ep->code };
        throw error;
    }
    ctxt->sax = save_sax;

}

