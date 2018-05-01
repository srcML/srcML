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
#include <string>

#define SRCSAX_DEBUG

#ifdef SRCSAX_DEBUG
    #define BASE_DEBUG fprintf(stderr, "BASE:  %s %s %d |%.*s| at pos %ld\n", __FILE__,  __FUNCTION__, __LINE__, 3, state->base, state->base - state->prevbase); 
    #define SRCML_DEBUG(title, ch, len) fprintf(stderr, "%s:  %s %s %d |%.*s|\n", title, __FILE__,  __FUNCTION__, __LINE__, (int)len, ch); 
    #define SRCSAX_DEBUG_BASE(title,m) fprintf(stderr, "%s: %s %s %d BASE: |%.*s| pos %ld\n", title, __FILE__, __FUNCTION__, __LINE__, 3, state->base, state->base - state->prevbase);
    #define SRCSAX_DEBUG_START(m) SRCSAX_DEBUG_BASE("BEGIN",m)
    #define SRCSAX_DEBUG_END(m)   SRCSAX_DEBUG_BASE("END  ",m)
    #define SRCSAX_DEBUG_START_CHARS(ch,len) SRCML_DEBUG("BEGIN",ch,len);
    #define SRCSAX_DEBUG_END_CHARS(ch,len)   SRCML_DEBUG("END  ",ch,len);
#else
    #define BASE_DEBUG
    #define SRCML_DEBUG(title, ch, len)
    #define SRCSAX_DEBUG(title,m) 
    #define SRCSAX_DEBUG_START(m)
    #define SRCSAX_DEBUG_END(m)
    #define SRCSAX_DEBUG_START_CHARS(ch,len)
    #define SRCSAX_DEBUG_END_CHARS(ch,len)
#endif

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

    sax.startElementNs = &start_root;
    sax.endElementNs = &end_element;

    sax.characters = &save_characters;
    sax.ignorableWhitespace = &save_characters;

    sax.comment = &comment;
    sax.cdataBlock = &cdata_block;
    sax.processingInstruction = &processing_instruction;

    return sax;
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    // initialize internal sax buffer char*'s and counts
    state->base = ctxt->input->cur;
    state->prevconsumed = ctxt->input->consumed;
    state->prevbase = ctxt->input->base;

    SRCSAX_DEBUG_START();

    // save for dictionary lookup of common elements
    UNIT_ENTRY       = xmlDictLookup(ctxt->dict, (const xmlChar*) "unit", strlen("unit"));
    MACRO_LIST_ENTRY = xmlDictLookup(ctxt->dict, (const xmlChar*) "macro_list", strlen("macro_list"));
    ESCAPE_ENTRY     = xmlDictLookup(ctxt->dict, (const xmlChar*) "escape", strlen("escape"));

    // save the encoding from the input
    state->context->encoding = "UTF-8";
    if (ctxt->encoding && ctxt->encoding[0] != '\0')
        state->context->encoding = (const char *)ctxt->encoding;
    else if (ctxt->input)
        state->context->encoding = (const char *)ctxt->input->encoding;

    // process any upper layer start document handling
    if (state->context->handler->start_document)
        state->context->handler->start_document(state->context);

    SRCSAX_DEBUG_END();

    if (state->context->terminate)
        return;
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START();

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

    SRCSAX_DEBUG_END();
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
               int nb_attributes, int nb_defaulted, const xmlChar** attributes) {

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START(localname);

    // cache the root data because we don't know if we are in an archive or not
    state->root = std::move(srcml_element(state->context, localname, prefix, URI,
                                                nb_namespaces, namespaces,
                                                nb_attributes, nb_defaulted, attributes));

    state->mode = ROOT;

    // handle nested units
    ctxt->sax->startElementNs = &first_start_element;

    state->root_start_tag.assign((const char*) state->base, ctxt->input->cur + 1 - state->base);
    state->base = ctxt->input->cur + 1;

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

        state->rootnsstr.clear();
        int ns_length = nb_namespaces * 2;
        for (int i = 0; i < ns_length; i += 2) {

            state->rootnsstr += "xmlns";
            if (namespaces[i]) {
                state->rootnsstr += ":";
                state->rootnsstr += (const char*) namespaces[i];
            }
            state->rootnsstr += "=\"";
            state->rootnsstr += (const char*) namespaces[i + 1];
            state->rootnsstr += "\" ";
        }
    }

    state->base = ctxt->input->cur;

    SRCSAX_DEBUG_END(localname);
}

/**
 * first_start_element
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
void first_start_element(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                         int nb_namespaces, const xmlChar** namespaces,
                         int nb_attributes, int nb_defaulted, const xmlChar** attributes) {
    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START(localname);

    // record start element position and size
    auto start_element_base = state->base;
    auto start_element_len = ctxt->input->cur + 1 - state->base;

    state->base = ctxt->input->cur + 1;

    // if macros are found, then must return, but first save them if necessary
    if (localname == MACRO_LIST_ENTRY) {

        state->meta_tags.emplace_back(srcml_element(state->context, localname, prefix, URI,
                                                     nb_namespaces, namespaces,
                                                     nb_attributes, nb_defaulted, attributes));
        return;
    }

    if (state->context->terminate)
        return;

    // we have an archive if the first element after the root is the <unit>
    state->context->is_archive = state->is_archive = (localname == UNIT_ENTRY);

    if (!state->is_archive) {

        // not an archive, so we end up calling start_unit() with the data used for start_root()
        state->mode = UNIT;
        state->unit_start_tag = std::move(state->root_start_tag);
        start_unit(ctx, state->root.localname, state->root.prefix, state->root.URI,
                        state->root.nb_namespaces, state->root.namespaces.data(),
                        state->root.nb_attributes, 0, state->root.attributes.data());
        state->unit_start_tag.clear();

        if (!state->characters.empty())
            characters_unit(ctx, (const xmlChar*) state->characters.c_str(), (int)state->characters.size());
        state->characters.clear();

        // use the parameters in this call to call the real start_element
        state->start_element_tag.assign((const char*) start_element_base, start_element_len);
        start_element(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, 0, attributes);
        state->start_element_tag.clear();

    } else {

        // since an archive, the characters found before this first nested element are
        // root characters 
        if (!state->characters.empty())
            characters_root(ctx, (const xmlChar*) state->characters.c_str(), (int)state->characters.size());
        state->characters.clear();

        // unit start tag with parameters
        state->mode = UNIT;
        state->unit_start_tag.assign((const char*) start_element_base, start_element_len);
        start_unit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, 0, attributes);
        state->unit_start_tag.clear();
    }

    SRCSAX_DEBUG_END(localname);

    if (state->context->terminate)
        return;
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START(localname);

    update_ctx(ctx);

    // unit_start_tag is empty means that we did not come from first_start_element()
    if (state->unit_start_tag.empty()) {
//        state->base = ctxt->input->cur + 1;
    }

    if (state->collect_unit_body) {

        if (state->is_archive) {
            // find end of unit tag
            int pos = (int) (1 + strlen((const char*) localname) + (prefix ? strlen((const char*) prefix) + 1 : 0) + 1);

            if (state->unit_start_tag.empty()) {
                // merge the namespaces from the root into this one
                // TODO: Only necessary for archive
                state->unitsrcml.assign((const char*) state->base, pos);
                state->unitsrcml.append(state->rootnsstr);
                state->unitsrcml.append((const char*) state->base + pos, ctxt->input->cur - state->base + 1 - pos);

            } else {

                // merge the namespaces from the root into this one
                // TODO: Only necessary for archive
                state->unitsrcml = state->unit_start_tag.substr(0, pos);
                state->unitsrcml += state->rootnsstr;
                state->unitsrcml += state->unit_start_tag.substr(pos);
            }
        } else {
            state->unitsrcml = std::move(state->unit_start_tag);
        }

        SRCML_DEBUG("UNIT", state->unitsrcml.c_str(), state->unitsrcml.size());

        state->content_begin = (int) state->unitsrcml.size();

        SRCML_DEBUG("CONTENT", state->unitsrcml.c_str(), state->content_begin - 1);
    }

    if (state->context->terminate)
        return;

    ++state->unit_count;

    if (state->context->handler->start_unit)
        state->context->handler->start_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI,
                                            nb_namespaces, namespaces,
                                            nb_attributes, attributes);
    ctxt->sax->startElementNs = 0;
    ctxt->sax->ignorableWhitespace = ctxt->sax->characters = 0;
    ctxt->sax->comment = 0;
    ctxt->sax->cdataBlock = 0;
    ctxt->sax->processingInstruction = 0;

    if (!state->collect_unit_body)
        return;

    // next start tag will be for a non-unit element
    ctxt->sax->startElementNs = &start_element;

    // characters are for the unit
    ctxt->sax->ignorableWhitespace = ctxt->sax->characters = &characters_unit;

    ctxt->sax->comment = &comment;
    ctxt->sax->cdataBlock = &cdata_block;
    ctxt->sax->processingInstruction = &processing_instruction;

    state->unitsrc.clear();

    state->base = ctxt->input->cur + 1;

    SRCSAX_DEBUG_END(localname);

    if (state->context->terminate)
        return;
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START(localname);

    state->mode = END_UNIT;

    if (!state->skip && state->context->handler->end_unit)
        state->context->handler->end_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI);

    ctxt->sax->startElementNs = &start_unit;

    if (state->collect_unit_body)
        ctxt->sax->ignorableWhitespace = ctxt->sax->characters = &characters_root;
    else
        ctxt->sax->ignorableWhitespace = ctxt->sax->characters = 0;

    state->base = ctxt->input->cur;

    SRCSAX_DEBUG_END(localname);

    if (state->context->terminate)
        return;
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START(localname);

    state->base = ctxt->input->cur;

    if (state->context->handler->end_root)
        state->context->handler->end_root(state->context, (const char *)localname, (const char *)prefix, (const char *)URI);

    SRCSAX_DEBUG_END(localname);
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START(localname);

    update_ctx(ctx);

    if (state->collect_unit_body) {

        if (state->start_element_tag.empty()) {

            if (state->base[0] == '>')
                state->base += 1;

            auto srcmllen = ctxt->input->cur + 1 - state->base;
            if (srcmllen < 0) {
                exit(1);
            }

            SRCML_DEBUG("BASE", (const char*) state->base, srcmllen);

            state->unitsrcml.append((const char*) state->base, srcmllen);
        } else {
            state->unitsrcml.append(state->start_element_tag);
        }

        SRCML_DEBUG("UNIT", state->unitsrcml.c_str(), state->unitsrcml.size());
    }

///    state->base = ctxt->input->cur + (state->start_element_tag.empty() ? 1 : 0);
    // Special element <escape char="0x0c"/> used to embed non-XML characters
    // extract the value of the char attribute and add to the src (text)
    if (localname == ESCAPE_ENTRY) {

        std::string svalue((const char *)attributes[0 * 5 + 3], attributes[0 * 5 + 4] - attributes[0 * 5 + 3]);

        char value = (int)strtol(svalue.c_str(), NULL, 0);

        state->unitsrc.append(1, value);

        return;
    }

    state->base = ctxt->input->cur;

    SRCSAX_DEBUG_END(localname);

    if (state->context->terminate)
        return;
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;  

    SRCSAX_DEBUG_START(localname);

    if (!state->collect_unit_body && localname != UNIT_ENTRY)
        return;
    
    update_ctx(ctx);

 //   fprintf(stderr, "DEBUG:  %s %s %d state->collect_unit_body: %zd\n", __FILE__,  __FUNCTION__, __LINE__,  state->collect_unit_body);
//    fprintf(stderr, "DEBUG:  %s %s %d state->unit_count: %zd\n", __FILE__,  __FUNCTION__, __LINE__,  state->unit_count);

    if (state->collect_unit_body) {

        if (state->base[0] == '>')
            state->base += 1;

        auto srcmllen = ctxt->input->cur - state->base;
        if (srcmllen < 0) {
            exit(1);
        }

        state->content_end = (int) state->unitsrcml.size() + 1;
        state->unitsrcml.append((const char*) state->base, srcmllen);

        SRCML_DEBUG("UNIT", state->unitsrcml.c_str(), state->unitsrcml.size());

        state->base = ctxt->input->cur;
    }

    if (localname == MACRO_LIST_ENTRY) {
        SRCSAX_DEBUG_END();
        return;
    }

    // plain end element
    if (localname != UNIT_ENTRY) {
        SRCSAX_DEBUG_END();
        return;
    }

    // the root is the only element so we never got this started
    if (state->mode == ROOT) {

        state->context->is_archive = state->is_archive = false;

        start_unit(ctx, state->root.localname, state->root.prefix, state->root.URI,
                        state->root.nb_namespaces, state->root.namespaces.data(),
                        state->root.nb_attributes, 0, state->root.attributes.data());

        if (!state->characters.empty())
            characters_unit(ctx, (const xmlChar*) state->characters.c_str(), (int)state->characters.size());
    }

    if (ctxt->nameNr == 2 || !state->is_archive) {

        end_unit(ctx, localname, prefix, URI);
    }

    if (ctxt->nameNr == 1) {

        state->mode = END_ROOT;

        end_root(ctx, localname, prefix, URI);
    }

    SRCSAX_DEBUG_END(localname);

    if (state->context->terminate)
        return;
}

/**
 * save_characters
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling before we
 * know if we have an archive or not.
 * Immediately calls supplied handlers function.
 */
void save_characters(void* ctx, const xmlChar* ch, int len) {

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START_CHARS(ch, len);

    state->base = ctxt->input->cur;

    // cache the characters since we don't know if in unit or outer archive
    // note that characters() could be called more than once for a particular sequence of characters, so append
    state->characters.append((const char *)ch, len);

    SRCSAX_DEBUG_END_CHARS(ch, len);
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START_CHARS(ch, len);

    update_ctx(ctx);

    state->base = ctxt->input->cur;

    SRCSAX_DEBUG_END_CHARS(ch, len);

    if (state->context->terminate)
        return;
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START_CHARS(ch, len);

    if (state->base[0] == '>')
        state->base += 1;

    BASE_DEBUG;

    if (!state->collect_unit_body)
        return;

    state->unitsrc.append((const char*) ch, len);

    update_ctx(ctx);

    // append the characters in their raw state (unescaped ?)
    if (ctxt->input->cur - state->base == 0) {
        state->unitsrcml.append((const char*) ctxt->input->cur, len);
        state->base = ctxt->input->cur + len;
    } else {
        state->unitsrcml.append((const char*) state->base, ctxt->input->cur - state->base);
        state->base = ctxt->input->cur;
    }

    SRCML_DEBUG("UNIT", state->unitsrcml.c_str(), state->unitsrcml.size());

    BASE_DEBUG

    SRCSAX_DEBUG_START_CHARS(ch, len);

    if (state->context->terminate)
        return;
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START();

    // @todo Make sure we capture this for srcml collection

    SRCSAX_DEBUG_END();

    if (state->context->terminate)
        return;
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

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START();

    // @todo Make sure we capture this for srcml collection

    SRCSAX_DEBUG_END();

    if (state->context->terminate)
        return;
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
void processing_instruction(void* ctx, const xmlChar* /* target */, const xmlChar* /* data */) {

    if (ctx == nullptr)
        return;

    auto ctxt = (xmlParserCtxtPtr) ctx;
    auto state = (sax2_srcsax_handler*) ctxt->_private;

    SRCSAX_DEBUG_START();

    // @todo Make sure we capture this for srcml collection

    SRCSAX_DEBUG_END();

    if (state->context->terminate)
        return;
}
