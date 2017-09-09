/**
 *  @file sax2_srcsax_handler.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML SAX2 Framework.
 *
 * The srcML SAX2 Framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML SAX2 Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 * along with the srcML SAX2 Framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <sax2_srcsax_handler.hpp>
#include <srctools_windows.hpp>

#include <cstring>

/** Static sax handler for zero initializing in factory */
xmlSAXHandler sax2_srcml_handler_init;

// helper conversions for boost::optional<std::string>
inline const char* optional_to_c_str(const boost::optional<std::string>& s) {
    return s ? s->c_str() : 0;
}

/**
 * factory
 *
 * Create SAX handler.
 */
xmlSAXHandler srcsax_sax2_factory() {

    xmlSAXHandler sax = sax2_srcml_handler_init;

    sax.initialized    = XML_SAX2_MAGIC;

    sax.startDocument = &start_document;
    sax.endDocument = &end_document;

    sax.startElementNs = &start_root;
    sax.endElementNs = &end_element_ns;

    sax.characters = &characters_first;
    sax.ignorableWhitespace = &characters_first;

    sax.comment = &comment;
    sax.cdataBlock = &cdata_block;
    sax.processingInstruction = &processing_instruction;

    return sax;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * start_document
 * @param ctx an xmlParserCtxtPtr
 *
 * SAX handler function for start of document.
 * Immediately calls supplied handlers function.
 */
void start_document(void* ctx) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == NULL) return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    state->context->stack_size = 0;
    state->context->srcml_element_stack = 0;

    state->context->encoding = "UTF-8";
    if (ctxt->encoding && ctxt->encoding[0] != '\0')
        state->context->encoding = (const char *)ctxt->encoding;
    else if (ctxt->input)
        state->context->encoding = (const char *)ctxt->input->encoding;

    if (state->context->terminate) return;

    if (state->context->handler->start_document)
        state->context->handler->start_document(state->context);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

}

/**
 * end_document
 * @param ctx an xmlParserCtxtPtr
 *
 * SAX handler function for end of document.
 * Calls end_root if needed then
 * immediately calls supplied handlers function.
 */
void end_document(void* ctx) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == NULL) return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    const char* errmsg = 0;
    switch (ctxt->errNo) {
    case XML_ERR_DOCUMENT_END:
        errmsg = "Extra content at the end of the document";
        break;
    default:
        break;
    };

    if (errmsg) {
        fprintf(stderr, "srcml: %s\n", errmsg);
    }

    state->context->stack_size = 0;
    state->context->srcml_element_stack = 0;

    if (state->context->terminate) return;

    if (state->mode != END_ROOT && state->mode != START && state->context->handler->end_root)
        state->context->handler->end_root(state->context, optional_to_c_str(state->root.localname), optional_to_c_str(state->root.prefix), optional_to_c_str(state->root.URI));

    if (state->context->terminate) return;

    if (state->context->handler->end_document)
        state->context->handler->end_document(state->context);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

}

/**
 * start_root
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of root element.
 * Caches root info and immediately calls supplied handlers function.
 */
void start_root(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
               int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
               const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == NULL) return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    state->root = srcml_element(state->context, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);

    state->mode = ROOT;

    // handle nested units
    ctxt->sax->startElementNs = &start_element_ns_first;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

}

/**
 * start_element_ns_first
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of first element after root
 * Detects archive and acts accordingly.
 */
void start_element_ns_first(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                         int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    if (strcmp((const char *)localname, "macro-list") == 0) {

        if (state->context->handler->meta_tag)
            state->meta_tags.push_back(srcml_element(state->context, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes));

        return;
    }

    state->context->is_archive = state->is_archive = strcmp((const char *)localname, "unit") == 0;

    if (state->context->terminate)
        return;

    if (state->context->handler->start_root)
        state->context->handler->start_root(state->context, optional_to_c_str(state->root.localname), optional_to_c_str(state->root.prefix), optional_to_c_str(state->root.URI),
                                            state->root.nb_namespaces, state->root.namespaces.data(), state->root.nb_attributes,
                                            state->root.attributes.data());

    if (state->context->terminate)
        return;

    if (state->context->handler->meta_tag) {

        for (auto citr : state->meta_tags) {

            state->context->handler->meta_tag(state->context, optional_to_c_str(citr.localname), optional_to_c_str(citr.prefix), optional_to_c_str(citr.URI),
                                                citr.nb_namespaces, citr.namespaces.data(), citr.nb_attributes,
                                                citr.attributes.data()); // @todo fix so can pass
            if (state->context->terminate)
                return;
        }
    }

    if (!state->is_archive) {

        ++state->context->unit_count;

        state->mode = UNIT;

        if (state->context->handler->start_unit)
            state->context->handler->start_unit(state->context, optional_to_c_str(state->root.localname), optional_to_c_str(state->root.prefix), optional_to_c_str(state->root.URI),
                                                state->root.nb_namespaces, state->root.namespaces.data(), state->root.nb_attributes,
                                                state->root.attributes.data());

        if (state->context->terminate)
            return;

        if (state->context->handler->characters_unit)
            state->context->handler->characters_unit(state->context, state->characters.c_str(), (int)state->characters.size());

        if (state->context->terminate)
            return;

        if (state->context->handler->start_element)
            state->context->handler->start_element(state->context, (const char *)localname, (const char *)prefix, (const char *)URI,
                nb_namespaces, namespaces, nb_attributes, attributes);

    } else {
        
        if (state->context->handler->characters_root)
            state->context->handler->characters_root(state->context, state->characters.c_str(), (int)state->characters.size());

        ++state->context->unit_count;

        if (state->context->terminate)
            return;

        state->mode = UNIT;

        if (state->context->handler->start_unit)
            state->context->handler->start_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI,
                                                nb_namespaces, namespaces, nb_attributes, attributes);
    }

    if (state->context->terminate)
        return;

    if (ctxt->sax->startElementNs)
        ctxt->sax->startElementNs = &start_element_ns;

    if (ctxt->sax->characters) {

        ctxt->sax->characters = &characters_unit;
        ctxt->sax->ignorableWhitespace = &characters_unit;
    }

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}

/**
 * start_unit
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of an unit.
 * Immediately calls supplied handlers function.
 */
void start_unit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
               int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
               const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    if (state->context->terminate)
        return;

    ++state->context->unit_count;

    state->mode = UNIT;

    if (state->context->handler->start_unit)
        state->context->handler->start_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI,
            nb_namespaces, namespaces, nb_attributes, attributes);

    if (ctxt->sax->startElementNs)
        ctxt->sax->startElementNs = &start_element_ns;

    if (ctxt->sax->characters) {

        ctxt->sax->characters = &characters_unit;
        ctxt->sax->ignorableWhitespace = &characters_unit;
    }

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}

/**
 * start_element_ns
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of an element.
 * Immediately calls supplied handlers function.
 */
void start_element_ns(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                    const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    if (state->context->terminate)
        return;

    if (state->context->handler->start_element)
        state->context->handler->start_element(state->context, (const char *)localname, (const char *)prefix, (const char *)URI,
            nb_namespaces, 0, nb_attributes, attributes);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}

/**
 * end_element_ns
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 *
 * SAX handler function for end of an element.
 * Detects end of unit and calls correct functions
 * for either end_root end_unit or end_element_ns.
 */
void end_element_ns(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;  

    if (!state->context->handler->end_element && ctxt->nameNr > 2)
        return;

    if (ctxt->nameNr <= 2 && strcmp((const char *)localname, "unit") == 0) {

        if (state->mode == ROOT) {

            state->context->is_archive = state->is_archive = false;

            if (state->context->terminate)
                return;

            if (state->context->handler->start_root)
                state->context->handler->start_root(state->context, optional_to_c_str(state->root.localname), optional_to_c_str(state->root.prefix), optional_to_c_str(state->root.URI),
                                                    state->root.nb_namespaces, state->root.namespaces.data(), state->root.nb_attributes,
                                                    state->root.attributes.data());

            if (state->context->terminate)
                return;

            if (state->context->handler->meta_tag) {

                for (auto citr : state->meta_tags) {

                    state->context->handler->meta_tag(state->context, citr.localname->c_str(), (const char *)optional_to_c_str(citr.prefix), optional_to_c_str(citr.URI),
                                                        citr.nb_namespaces, citr.namespaces.data(), citr.nb_attributes,
                                                        citr.attributes.data()); // @todo fix so can pass
                    if (state->context->terminate)
                        return;
                }
            }

            if (state->context->handler->start_unit)
                state->context->handler->start_unit(state->context, optional_to_c_str(state->root.localname), optional_to_c_str(state->root.prefix), optional_to_c_str(state->root.URI),
                                                    state->root.nb_namespaces, state->root.namespaces.data(), state->root.nb_attributes,
                                                    state->root.attributes.data());

            if (state->context->terminate)
                return;

            if (!state->characters.empty() && state->context->handler->characters_unit)
                state->context->handler->characters_unit(state->context, state->characters.c_str(), (int)state->characters.size());

        }

        if (state->context->terminate)
            return;

        if (ctxt->sax->startElementNs == &start_unit) {

            state->mode = END_ROOT;

            if (state->context->handler->end_root)
                state->context->handler->end_root(state->context, (const char *)localname, (const char *)prefix, (const char *)URI);

        } else {

            state->mode = END_UNIT;

            if (state->context->handler->end_unit)
                state->context->handler->end_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI);

            if (ctxt->sax->startElementNs)
                ctxt->sax->startElementNs = &start_unit;

            if (ctxt->sax->characters) {

                ctxt->sax->characters = &characters_root;
                ctxt->sax->ignorableWhitespace = &characters_root;
            }
        }

    } else {

        if (state->context->terminate)
            return;

        if (localname[0] == 'm' && localname[1] == 'a' && strcmp((const char *)localname, "macro-list") == 0)
            return;

        if (state->context->handler->end_element)
            state->context->handler->end_element(state->context, (const char *)localname, (const char *)prefix, (const char *)URI);
    }

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}

/**
 * characters_first
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling before we
 * know if we have an archive or not.
 * Immediately calls supplied handlers function.
 */
void characters_first(void* ctx, const xmlChar* ch, int len) {

#ifdef SRCSAX_DEBUG
    std::string chars;
    chars.append((const char *)ch, len);
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    state->characters.append((const char *)ch, len);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif
}

/**
 * characters_root
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling at the root level.
 * Immediately calls supplied handlers function.
 */
void characters_root(void* ctx, const xmlChar* ch, int len) {

#ifdef SRCSAX_DEBUG
    std::string chars;
    chars.append((const char *)ch, len);
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    if (state->context->terminate)
        return;

    if (state->context->handler->characters_root)
        state->context->handler->characters_root(state->context, (const char *)ch, len);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif
}

/**
 * characters_unit
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling within a unit.
 * Immediately calls supplied handlers function.
 */
void characters_unit(void* ctx, const xmlChar* ch, int len) {

#ifdef SRCSAX_DEBUG
    std::string chars;
    chars.append((const char *)ch, len);
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    if (state->context->terminate)
        return;

    if (state->context->handler->characters_unit)
        state->context->handler->characters_unit(state->context, (const char *)ch, len);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif
}

/**
 * comment
 * @param ctx an xmlParserCtxtPtr
 * @param value the comment content
 *
 * A comment has been parsed.
 * Immediately calls supplied handlers function.
 */
void comment(void* ctx, const xmlChar* value) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    if (state->context->terminate)
        return;

    if (state->context->handler->comment)
        state->context->handler->comment(state->context, (const char *)value);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
}

/**
 * cdata_block
 * @param ctx an xmlParserCtxtPtr
 * @param value the pcdata content
 * @param len the block length
 *
 * Called when a pcdata block has been parsed.
 * Immediately calls supplied handlers function.
 */
void cdata_block(void* ctx, const xmlChar* value, int len) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    if (state->context->terminate)
        return;

    if (state->context->handler->cdata_block)
        state->context->handler->cdata_block(state->context, (const char *)value, len);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
}

/**
 * processing_instruction
 * @param ctx an xmlParserCtxtPtr
 * @param target the processing instruction target.
 * @param data the processing instruction data.
 *
 * Called when a processing instruction has been parsed.
 * Immediately calls supplied handlers function.
 */
void processing_instruction(void* ctx, const xmlChar* target, const xmlChar* data) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == NULL)
        return;

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    sax2_srcsax_handler* state = (sax2_srcsax_handler *) ctxt->_private;

    if (state->context->terminate)
        return;

    if (state->context->handler->processing_instruction)
        state->context->handler->processing_instruction(state->context, (const char *)target, (const char *)data);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "HERE: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
}

#pragma GCC diagnostic pop
