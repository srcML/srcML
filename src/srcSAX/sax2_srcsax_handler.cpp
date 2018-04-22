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

#include <cstring>

#include <sax2_srcsax_handler.hpp>
#include <string>

/**
 * factory
 *
 * Create SAX handler.
 */
xmlSAXHandler srcsax_sax2_factory() {

    xmlSAXHandler sax;
    memset(&sax, 0, sizeof(sax));

    sax.initialized    = XML_SAX2_MAGIC;

    sax.startDocument = &start_document;
    sax.endDocument = &end_document;

    sax.startElementNs = &start_root_first;
    sax.endElementNs = &end_element;

    sax.characters = &characters_start;
    sax.ignorableWhitespace = &characters_start;

    sax.comment = &comment;
    sax.cdataBlock = &cdata_block;
    sax.processingInstruction = &processing_instruction;

    return sax;
}

static void skip_off(xmlParserCtxtPtr ctxt) {

	ctxt->sax->startElementNs = &start_element;
	ctxt->sax->characters = &characters_start;
	ctxt->sax->ignorableWhitespace = &characters_start;
	ctxt->sax->comment = &comment;
	ctxt->sax->cdataBlock = &cdata_block;
	ctxt->sax->processingInstruction = &processing_instruction;
}

static void skip_on(xmlParserCtxtPtr ctxt) {

	ctxt->sax->startElementNs = 0;
	ctxt->sax->characters = 0;
	ctxt->sax->ignorableWhitespace = 0;
	ctxt->sax->comment = 0;
	ctxt->sax->cdataBlock = 0;
	ctxt->sax->processingInstruction = 0;
}

static const xmlChar* UNIT_ENTRY = nullptr;
static const xmlChar* MACRO_LIST_ENTRY = nullptr;
static const xmlChar* ESCAPE_ENTRY = nullptr;

// updates the state being used to extract XML directly from the context
// necessary because as libxml buffers are full, data is moved
static void update_ctx(void* ctx) {

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    if (state->prevconsumed != ctxt->input->consumed) {
        state->base -= ctxt->input->consumed - state->prevconsumed;
    }
    state->prevconsumed = ctxt->input->consumed;

    if (state->prevbase != ctxt->input->base) {
        state->base += ctxt->input->base - state->prevbase;
    }
    state->prevbase = ctxt->input->base;
}

/**
 * start_document
 * @param ctx an xmlParserCtxtPtr
 *
 * SAX handler function for start of document.
 * Immediately calls supplied handlers function.
 */
void start_document(void* ctx) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    // setup dictionary lookup for common elements
    UNIT_ENTRY = xmlDictLookup(ctxt->dict, (const xmlChar*) "unit", 4);

    MACRO_LIST_ENTRY = xmlDictLookup(ctxt->dict, (const xmlChar*) "macro_list", strlen("macro_list"));
    ESCAPE_ENTRY = xmlDictLookup(ctxt->dict, (const xmlChar*) "escape", strlen("escape"));

    // initialize internal sax buffer char*'s and counts
    state->base = ctxt->input->cur;
    state->prevconsumed = ctxt->input->consumed;
    state->prevbase = ctxt->input->base;

    // process any upper layer start document handling
    if (state->context->handler->start_document)
        state->context->handler->start_document(state->context);

    // get encoding from the input
    state->context->encoding = "UTF-8";
    if (ctxt->encoding && ctxt->encoding[0] != '\0')
        state->context->encoding = (const char *)ctxt->encoding;
    else if (ctxt->input)
        state->context->encoding = (const char *)ctxt->input->encoding;

    if (state->context->terminate)
        return;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
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
    fprintf(stderr, "BEGIN: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    // handle libxml errors
    const char* errmsg = 0;
    switch (ctxt->errNo) {
    case XML_ERR_DOCUMENT_END:
        errmsg = "Extra content at the end of the document";
        break;
    default:
        break;
    };

    if (errmsg) {
//        fprintf(stderr, "srcml: %s\n", errmsg);
    }

    if (state->context->terminate)
        return;

    // never found any content, so end the root
    if (state->mode != END_ROOT && state->mode != START)
        end_root(ctx, state->root.localname, state->root.prefix, state->root.URI);

    if (state->context->terminate)
        return;

    // process any upper layer end document handling
    if (state->context->handler->end_document)
        state->context->handler->end_document(state->context);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
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
void start_root_first(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
               int nb_namespaces, const xmlChar** namespaces,
               int nb_attributes, int nb_defaulted, const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    // cache the root data because we don't know if we are in an archive or not
    state->root = std::move(srcml_element(state->context, localname, prefix, URI,
                                                nb_namespaces, namespaces,
                                                nb_attributes, nb_defaulted, attributes));

    state->mode = ROOT;

    // handle nested units
    ctxt->sax->startElementNs = &start_element_start;

    state->endfirstelement = ctxt->input->cur + 1;
    state->root_start_tag = std::string((const char*) state->base, state->endfirstelement - state->base);
    state->base = state->endfirstelement;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
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
               int nb_namespaces, const xmlChar** namespaces,
               int nb_attributes, int /* nb_defaulted */, const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    // have to call this here because we need to first know if we are in an archive
    if (state->context->handler->start_root)
        state->context->handler->start_root(state->context, (const char*) localname, (const char*) prefix, (const char*) URI,
                                                            nb_namespaces, namespaces, 
                                                            nb_attributes, attributes);

    if (state->context->terminate)
        return;

    for (auto citr : state->meta_tags) {

        state->context->handler->meta_tag(state->context, (const char*) citr.localname, (const char*) citr.prefix, (const char*) citr.URI,
                                                          citr.nb_namespaces, citr.namespaces.data(),
                                                          citr.nb_attributes, citr.attributes.data());
        if (state->context->terminate)
            return;
    }

    // record namespace string in an extensible list so we can add the per unit
    if (state->collect_unit_body) {
        int ns_length = nb_namespaces * 2;
        for (int i = 0; i < ns_length; i += 2) {

            state->data.push_back(namespaces[i]);
            state->data.push_back(namespaces[i + 1]);
        }
        state->rootsize = state->data.size();

        state->rootnsstr.clear();
        for (size_t i = 0; i < state->data.size() / 2; ++i) {

            auto& d = state->data;

            state->rootnsstr += "xmlns";
            if (d[i * 2]) {
                state->rootnsstr += ":";
                state->rootnsstr += (const char*) d[i * 2];
            }
            state->rootnsstr += "=\"";
            state->rootnsstr += (const char*) d[i * 2 + 1];
            state->rootnsstr += "\" ";
        }
    }

    state->base = ctxt->input->cur;
    state->prev_start = false;
#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}
/**
 * start_element_start
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
void start_element_start(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                         int nb_namespaces, const xmlChar** namespaces,
                         int nb_attributes, int nb_defaulted, const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    std::string start_element_tag((const char*) state->base, ctxt->input->cur + 1 - state->base);
    state->base = ctxt->input->cur + 1;

    // if macros are found, then must return, but first save them if necessary
    if (localname == MACRO_LIST_ENTRY) {

        if (state->context->handler->meta_tag)
            state->meta_tags.emplace_back(srcml_element(state->context, localname, prefix, URI,
                                                     nb_namespaces, namespaces,
                                                     nb_attributes, nb_defaulted, attributes));
        return;
    }

    if (state->context->terminate)
        return;

    state->context->is_archive = state->is_archive = (localname == UNIT_ENTRY);

    // have to call this here because we need to first know if we are in an archive
    start_root(ctx, state->root.localname, state->root.prefix, state->root.URI,
                    state->root.nb_namespaces, state->root.namespaces.data(),
                    state->root.nb_attributes, 0, state->root.attributes.data());

    if (!state->is_archive) {

        state->mode = UNIT;
        state->unit_start_tag = std::move(state->root_start_tag);
        start_unit(ctx, state->root.localname, state->root.prefix, state->root.URI,
                        state->root.nb_namespaces, state->root.namespaces.data(),
                        state->root.nb_attributes, 0, state->root.attributes.data());
        state->unit_start_tag.clear();

        if (!state->characters.empty())
            characters_unit(ctx, (const xmlChar*) state->characters.c_str(), (int)state->characters.size());
      //  state->characters.clear();

        state->start_element_tag = std::move(start_element_tag);
        start_element(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, 0, attributes);
        state->start_element_tag.clear();

    } else {
        
        characters_root(ctx, (const xmlChar*) state->characters.c_str(), (int)state->characters.size());
//        state->characters.clear();

        state->mode = UNIT;
        state->unit_start_tag = start_element_tag;
        start_unit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, 0, attributes);
    }
    state->unit_start_tag.clear();

    if (state->context->terminate)
        return;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
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
               int nb_namespaces, const xmlChar** namespaces,
               int nb_attributes, int /* nb_defaulted */, const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    state->prev_start = false;

    update_ctx(ctx);

    // if not the first unit, need to extract the unit start tag
    if (state->collect_unit_body && state->unit_start_tag.empty()) {
        state->unit_start_tag = std::string((const char*) state->base, ctxt->input->cur - state->base + 1);
    }

    if (state->unit_start_tag.empty()) {
        state->base = ctxt->input->cur + 1;
    }

    if (state->collect_unit_body) {

        if (state->is_archive) {
            // find end of unit tag
            int pos = (int) (1 + strlen((const char*) localname) + (prefix ? strlen((const char*) prefix) + 1 : 0) + 1);

            // merge the namespaces from the root into this one
            // TODO: Only necessary for archive
            state->unitsrcml = state->unit_start_tag.substr(0, pos);
            state->unitsrcml += state->rootnsstr;
            state->unitsrcml += state->unit_start_tag.substr(pos);
        } else {
            state->unitsrcml = std::move(state->unit_start_tag);
        }
#ifdef UNITSRCML_DEBUG
        fprintf(stderr, "DEBUG:  %s %s %d state->unitsrcml: \n|%s|\n", __FILE__,  __FUNCTION__, __LINE__,  state->unitsrcml.c_str());
#endif
        state->content_begin = (int) state->unitsrcml.size();

#ifdef UNITSRCML_DEBUG
        std::string rootstr = state->unitsrcml.substr(0, state->content_begin - 1);
        fprintf(stderr, "DEBUG:  %s %s %d rootstr: |%s|\n", __FILE__,  __FUNCTION__, __LINE__,  rootstr.c_str());
#endif
    }

    if (state->context->terminate)
        return;

    ++state->unit_count;
//fprintf(stderr, "COUNT:  %s %s %d state->unit_count: %zd\n", __FILE__,  __FUNCTION__, __LINE__,  state->unit_count);

  //  state->mode = UNIT;


    if (state->context->handler->start_unit)
        state->context->handler->start_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI,
                                            nb_namespaces, namespaces,
                                            nb_attributes, attributes);

//    state->skip = true;
    skip_on(ctxt);

    if (!state->collect_unit_body)
    	return;

    // next start tag will be for a non-unit element
//    if (ctxt->sax->startElementNs)
        ctxt->sax->startElementNs = &start_element;

    // characters are for the unit
    ctxt->sax->ignorableWhitespace = ctxt->sax->characters = &characters_unit;

    state->unitsrc.clear();

    state->base = ctxt->input->cur + 1;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}

/**
 * end_unit
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 *
 * SAX handler function for end of a unit
 */
void end_unit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    state->prev_start = false;

    state->mode = END_UNIT;

    if (!state->skip && state->context->handler->end_unit)
        state->context->handler->end_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI);

    ctxt->sax->startElementNs = &start_unit;

    if (state->collect_unit_body)
        ctxt->sax->ignorableWhitespace = ctxt->sax->characters = &characters_root;

    state->maxsize = state->maxsize < state->unitsrc.size() ? state->unitsrc.size() : state->maxsize;

    state->base = ctxt->input->cur;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}

/**
 * end_root
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 *
 * SAX handler function for end of a unit
 */
void end_root(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    state->prev_start = false;

    state->base = ctxt->input->cur;

    if (state->context->handler->end_root)
        state->context->handler->end_root(state->context, (const char *)localname, (const char *)prefix, (const char *)URI);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}

/**
 * start_element
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
void start_element(void* ctx, const xmlChar* localname, const xmlChar* /* prefix */, const xmlChar* /* URI */,
                    int /* nb_namespaces */, const xmlChar** /* namespaces */,
                    int /* nb_attributes */, int /* nb_defaulted */, const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    state->prev_start = true;

   // state->base += 1;

    update_ctx(ctx);

    if (state->collect_unit_body) {

        if (state->start_element_tag.empty()) {

            if (state->base[0] == '>')
                state->base += 1;

            auto srcmllen = ctxt->input->cur + 1 - state->base;
            if (srcmllen < 0) {
                exit(1);
            }
#ifdef UNITSRCML_DEBUG
            std::string curelement = std::string((const char*) state->base, srcmllen);
            fprintf(stderr, "DEBUG:  %s %s %d curelement: %s\n", __FILE__,  __FUNCTION__, __LINE__,  curelement.c_str());
#endif
            state->unitsrcml.append((const char*) state->base, srcmllen);
        } else {
            state->unitsrcml.append(state->start_element_tag);
        }
#ifdef UNITSRCML_DEBUG
        fprintf(stderr, "DEBUG:  %s %s %d state->unitsrcml: \n|%s|\n", __FILE__,  __FUNCTION__, __LINE__,  state->unitsrcml.c_str());
#endif
    }

///    state->base = ctxt->input->cur + (state->start_element_tag.empty() ? 1 : 0);
    // Special element <escape char="0x0c"/> used to embed non-XML characters
    // extract the value of the char attribute and add to the src (text)
    if (state->collect_unit_body && localname == ESCAPE_ENTRY) {

        std::string svalue((const char *)attributes[0 * 5 + 3], attributes[0 * 5 + 4] - attributes[0 * 5 + 3]);

        char value = (int)strtol(svalue.c_str(), NULL, 0);

        state->unitsrc.append(1, value);

        return;
    }

    state->base = ctxt->input->cur;

    if (state->context->terminate)
        return;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}

/**
 * end_element
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 *
 * SAX handler function for end of an element.
 * Detects end of unit and calls correct functions
 * for either end_root end_unit or end_element.
 */
void end_element(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;  

    if (!state->collect_unit_body && ctxt->nodeNr > 2)
    	return;
    
    update_ctx(ctx);

    if (state->collect_unit_body) {

        if (state->prev_start) {
            state->base += 1;
        }

        auto srcmllen = ctxt->input->cur - state->base;
        if (srcmllen < 0) {
            exit(1);
        }

        state->content_end = (int) state->unitsrcml.size() + 1;
        state->unitsrcml.append((const char*) state->base, srcmllen);
#ifdef UNITSRCML_DEBUG
        std::string current((const char*) state->base, srcmllen);
        fprintf(stderr, "DEBUG:  %s %s %d current: |%s|\n", __FILE__,  __FUNCTION__, __LINE__,  current.c_str());
        fprintf(stderr, "DEBUG:  %s %s %d state->unitsrcml: \n|%s|\n", __FILE__,  __FUNCTION__, __LINE__,  state->unitsrcml.c_str());
#endif
        state->base = ctxt->input->cur;
    }

    state->prev_start = false;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "PARTIALEND: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

    if (localname == MACRO_LIST_ENTRY)
        return;

    // plain end element
    if (localname != UNIT_ENTRY)
    	return;

    // the root is the only element so we never got this started
    if (state->mode == ROOT) {

        state->context->is_archive = state->is_archive = false;

        start_root(ctx, state->root.localname, state->root.prefix, state->root.URI,
                        state->root.nb_namespaces, state->root.namespaces.data(),
                        state->root.nb_attributes, 0, state->root.attributes.data());

        start_unit(ctx, state->root.localname, state->root.prefix, state->root.URI,
                        state->root.nb_namespaces, state->root.namespaces.data(),
                        state->root.nb_attributes, 0, state->root.attributes.data());

        characters_unit(ctx, (const xmlChar*) state->characters.c_str(), (int)state->characters.size());
    }

    if (ctxt->nameNr == 2 || !state->is_archive) {

        end_unit(ctx, localname, prefix, URI);
    }

    if (ctxt->nameNr == 1) {

        state->mode = END_ROOT;

        end_root(ctx, localname, prefix, URI);
    }

    if (state->context->terminate)
        return;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
}

/**
 * characters_start
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling before we
 * know if we have an archive or not.
 * Immediately calls supplied handlers function.
 */
void characters_start(void* ctx, const xmlChar* ch, int len) {

#ifdef SRCSAX_DEBUG
    std::string chars;
    chars.append((const char *)ch, len);
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    state->prev_start = false;

    state->base = ctxt->input->cur;

    // cache the characters since we don't know if in unit or outer archive
    // note that characters() could be called more than once for a particular sequence of characters, so append
    state->characters.append((const char *)ch, len);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
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
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    state->prev_start = false;

    if (state->context->terminate)
        return;

    update_ctx(ctx);

    state->first_root_char = false;

    // since there are root characters, the end of the first element has to be adjusted to
    // include the whitespace
    state->endfirstelement += 2;
    state->base += 2;

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
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
    fprintf(stderr, "BEGIN: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    if (state->prev_start)
       state->base += 1;

    state->prev_start = false;

    update_ctx(ctx);

    if (!state->collect_unit_body)
        return;

    // append the characters in their raw state (unescaped ?)
    if (ctxt->input->cur - state->base == 0) {
        state->unitsrcml.append((const char*) ctxt->input->cur, len);
        state->base = ctxt->input->cur + len;
    } else {
        state->unitsrcml.append((const char*) state->base, ctxt->input->cur - state->base);
        state->base = ctxt->input->cur;
    }
#ifdef UNITSRCML_DEBUG
    fprintf(stderr, "DEBUG:  %s %s %d state->unitsrcml: \n|%s|\n", __FILE__,  __FUNCTION__, __LINE__,  state->unitsrcml.c_str());
#endif
    if (state->context->terminate)
        return;

    state->unitsrc.append((const char*) ch, len);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
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
void comment(void* ctx, const xmlChar* /* value */) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    state->prev_start = false;

    if (state->context->terminate)
        return;

    // @todo Make sure we capture this for srcml collection

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
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
void cdata_block(void* ctx, const xmlChar* /* value */, int /* len */) {

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "BEGIN: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    state->prev_start = false;

    if (state->context->terminate)
        return;

    // @todo Make sure we capture this for srcml collection

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
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
    fprintf(stderr, "BEGIN: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    state->prev_start = false;

    if (state->context->terminate)
        return;

    // @todo Make sure we capture this for srcml collection

    if (state->context->handler->processing_instruction)
        state->context->handler->processing_instruction(state->context, (const char *)target, (const char *)data);

#ifdef SRCSAX_DEBUG
    fprintf(stderr, "END: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
#endif
}
