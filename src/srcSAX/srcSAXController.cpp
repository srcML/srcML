/**
 * @file srcSAXController.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * srcSAX is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * srcSAX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <srcSAXController.hpp>
#include <srcSAXHandler.hpp>
#include <cppCallbackAdapter.hpp>

#include <sax2_srcsax_handler.hpp>

#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * srcSAXController
 * @param filename name of a file
 *
 * Constructor
 */
srcSAXController::srcSAXController(const char* filename, const char* encoding) {

    context = srcsax_create_context_filename(filename, encoding);
    if (context == NULL)
        throw std::string("File does not exist");
}

/**
 * srcSAXController
 * @param srcml_buffer a string buffer
 *
 * Constructor
 */
srcSAXController::srcSAXController(std::string srcml_buffer, const char* encoding) {

    context = srcsax_create_context_memory(srcml_buffer.c_str(), srcml_buffer.size(), encoding);
    if (context == NULL)
        throw std::string("File does not exist");
}

/**
 * srcSAXController
 * @param srcml_file a FILE for a srcML document
 *
 * Constructor
 */
srcSAXController::srcSAXController(FILE* srcml_file, const char* encoding) {

    context = srcsax_create_context_FILE(srcml_file, encoding);
    if (context == NULL)
        throw std::string("File does not exist");
}

/**
 * srcSAXController
 * @param srcml_fd a file descriptor for a srcML document
 *
 * Constructor
 */
srcSAXController::srcSAXController(int srcml_fd, const char* encoding) {

    context = srcsax_create_context_fd(srcml_fd, encoding);
    if (context == NULL)
        throw std::string("File does not exist");
}

/**
 * srcSAXController
 * @param srcml_context a general context for a srcML document
 * @param read_callback a read callback function
 * @param close_callback a close callback function
 *
 * Constructor
 */
srcSAXController::srcSAXController(void * srcml_context, int (*read_callback)(void * context, char* buffer, int len), int (*close_callback)(void * context), const char* encoding) {

    context = srcsax_create_context_io(srcml_context, read_callback, close_callback, encoding);
    if (context == NULL)
        throw std::string("File does not exist");
}

/**
 * srcSAXController
 * @param input a parser input buffer
 *
 * Constructor
 */
srcSAXController::srcSAXController(xmlParserInputBufferPtr input) {

    context = srcsax_create_context_parser_input_buffer(input);
    if (context == NULL)
        throw std::string("File does not exist");
}

/**
 * ~srcSAXController
 *
 * Constructor
 */
srcSAXController::~srcSAXController() {

    if (context)
        srcsax_free_context(context);
}

/**
 * getCtxt
 *
 * Return the used parser context.
 */
srcsax_context * srcSAXController::getContext() {

    return context;
}

/**
 * enable_startDocument
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables startDocument parsing.
 */
void srcSAXController::enable_startDocument(bool enable) {

    context->handler->start_document = enable ? cppCallbackAdapter::start_document : 0;
}

/**
 * enable_endDocument
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables endDocument parsing.
 */
void srcSAXController::enable_endDocument(bool enable) {

    context->handler->end_document = enable ? cppCallbackAdapter::end_document : 0;
}

/**
 * enable_startRoot
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables startRoot parsing.
 */
void srcSAXController::enable_startRoot(bool enable) {

    context->handler->start_root = enable ? cppCallbackAdapter::start_root : 0;
}

/**
 * enable_startUnit
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables startUnit parsing.
 */
void srcSAXController::enable_startUnit(bool enable) {

    context->handler->start_unit = enable ? cppCallbackAdapter::start_unit : 0;
}

/**
 * enable_startElement
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables startElement parsing.
 */
void srcSAXController::enable_startElement(bool enable) {

    context->handler->start_element = enable ? cppCallbackAdapter::start_element : 0;
}

/**
 * enable_endRoot
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables endRoot parsing.
 */
void srcSAXController::enable_endRoot(bool enable) {

    context->handler->end_root = enable ? cppCallbackAdapter::end_root : 0;
}

/**
 * enable_endUnit
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables endUnit parsing.
 */
void srcSAXController::enable_endUnit(bool enable) {

    context->handler->end_unit = enable ? cppCallbackAdapter::end_unit : 0;
}

/**
 * enable_endElement
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables endElement parsing.
 */
void srcSAXController::enable_endElement(bool enable) {

    context->handler->end_element = enable ? cppCallbackAdapter::end_element : 0;
}

/**
 * enable_charactersRoot
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables charactersRoot parsing.
 */
void srcSAXController::enable_charactersRoot(bool enable) {

    context->handler->characters_root = enable ? cppCallbackAdapter::characters_root : 0;
}

/**
 * enable_charactersUnit
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables charactersUnit parsing.
 */
void srcSAXController::enable_charactersUnit(bool enable) {

    context->handler->characters_unit = enable ? cppCallbackAdapter::characters_unit : 0;
}

/**
 * enable_metaTag
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables metaTag parsing.
 */
void srcSAXController::enable_metaTag(bool enable) {

    context->handler->meta_tag = enable ? cppCallbackAdapter::meta_tag : 0;
}

/**
 * enable_comment
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables comment parsing.
 */
void srcSAXController::enable_comment(bool enable) {

    context->handler->comment = enable ? cppCallbackAdapter::comment : 0;
}

/**
 * enable_cdataBlock
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables cdataBlock parsing.
 */
void srcSAXController::enable_cdataBlock(bool enable) {

    context->handler->cdata_block = enable ? cppCallbackAdapter::cdata_block : 0;
}

/**
 * enable_processingInstruction
 * @param enable bool indicate enable or disable SAX parsing.
 *
 * Enables or disables processingInstruction parsing.
 */
void srcSAXController::enable_processingInstruction(bool enable) {

    context->handler->processing_instruction = enable ? cppCallbackAdapter::processing_instruction : 0;
}

/**
 * parse
 * @param handler srcMLHandler with hooks for sax parsing
 *
 * Parse the xml document with the supplied hooks.
 */
void srcSAXController::parse(srcSAXHandler * handler) {

    handler->set_controller(this);

    cppCallbackAdapter adapter(handler);
    context->data = &adapter;
    srcsax_handler sax_handler = cppCallbackAdapter::factory();
    context->handler = &sax_handler;

    int status = srcsax_parse(context);

    context->data = 0;

    if (status != 0) {

        xmlErrorPtr ep = xmlCtxtGetLastError(context->libxml2_context);
        SAXError error = { std::string(ep->message), ep->code };

        throw error;
    }
}

