// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file sax2_srcsax_handler.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#include <sax2_srcsax_handler.hpp>
#include <srcmlns.hpp>
#include <string>
#include <cstring>
#include <algorithm>

#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/tree.h>

#ifdef SRCSAX_DEBUG
    #define BASE_DEBUG fprintf(stderr, "BASE:  %s %s %d |%.*s| at pos %ld\n", __FILE__,  __FUNCTION__, __LINE__, 3, state->base, state->base - state->prevbase);
    #define SRCML_DEBUG(title, ch, len) fprintf(stderr, "%s:  %s %s %d |%.*s|\n", title, __FILE__,  __FUNCTION__, __LINE__, (int)len, ch);
    #define SRCSAX_DEBUG_BASE(title,m) fprintf(stderr, "%s: %s %s %d %s BASE: pos %ld |%.*s| \n", title, __FILE__, __FUNCTION__, __LINE__, m, state->base - state->prevbase, 3, state->base);
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

    sax.characters = sax.ignorableWhitespace = &characters_unit;

    sax.comment = &comment;
    sax.cdataBlock = &cdata_block;
    sax.processingInstruction = &processing_instruction;

    return sax;
}

// updates the state being used to extract XML directly from the context
// necessary because as libxml buffers are full, data is moved
static void update_ctx(void* ctx) {

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    if (state->prevconsumed != ctxt->input->consumed) {
        state->base -= ctxt->input->consumed - state->prevconsumed;
    }
    state->prevconsumed = ctxt->input->consumed;

    if (state->prevbase != ctxt->input->base) {
        state->base += ctxt->input->base - state->prevbase;
    }
    state->prevbase = ctxt->input->base;
}

// unit and root delayed-start processing
static int reparse_root(void* ctx) {

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return 0;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return 0;

    // Basically, reparse the root start tag, collected when first parsed
    xmlSAXHandler roottagsax;
    memset(&roottagsax, 0, sizeof(roottagsax));
    roottagsax.initialized    = XML_SAX2_MAGIC;
    xmlSetStructuredErrorFunc(ctx, [](void * userData,
                     xmlErrorPtr /* error */) {

        auto ctxt = (xmlParserCtxtPtr) userData;
        if (ctxt == nullptr)
            return;
        auto state = (sax2_srcsax_handler*) ctxt->_private;
        if (state == nullptr)
            return;
    });

    roottagsax.startElementNs = [](void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                     int nb_namespaces, const xmlChar** namespaces,
                     int nb_attributes, int /* nb_defaulted */, const xmlChar** attributes) {

        auto ctxt = (xmlParserCtxtPtr) ctx;
        if (ctxt == nullptr)
            return;
        auto state = (sax2_srcsax_handler*) ctxt->_private;
        if (state == nullptr)
            return;

        // call the upper-level start_root
        state->context->handler->start_root(state->context, (const char*) localname,
                            (const char*) prefix, (const char*) URI,
                            nb_namespaces, namespaces, nb_attributes, attributes);

        // call the upper-level start_unit for non-archives
        if (!state->context->is_archive)
            state->context->handler->start_unit(state->context, (const char*) localname,
                    (const char*) prefix, (const char*) URI, nb_namespaces, namespaces, nb_attributes, attributes);
    };

    xmlParserCtxtPtr context = xmlCreateMemoryParserCtxt(state->rootstarttag.data(), (int) state->rootstarttag.size());
    auto save_private = context->_private;
    context->_private = state;
    auto save_sax = context->sax;
    context->sax = &roottagsax;

    state->rootcalled = true;

    int status = xmlParseDocument(context);

    context->_private = save_private;
    context->sax = save_sax;
    xmlFreeParserCtxt(context);

    return status;
}

/**
 * start_document
 * @param ctx an xmlParserCtxtPtr
 *
 * SAX handler function for start of document.
 * Immediately calls supplied handlers function.
 */
void start_document(void* ctx) {

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    // initialize internal sax buffer char*'s and counts
    state->base = ctxt->input->cur;
    state->prevconsumed = ctxt->input->consumed;
    state->prevbase = ctxt->input->base;

    SRCSAX_DEBUG_START("");

    // save for dictionary lookup of common elements
    state->UNIT_ENTRY       = xmlDictLookup(ctxt->dict, (const xmlChar*) "unit", (int)"unit"sv.size());
    state->MACRO_LIST_ENTRY = xmlDictLookup(ctxt->dict, (const xmlChar*) "macro-list", (int)"macro-list"sv.size());
    state->ESCAPE_ENTRY     = xmlDictLookup(ctxt->dict, (const xmlChar*) "escape", (int)"escape"sv.size());

    // save the encoding from the input
    state->context->encoding = "UTF-8";
    if (ctxt->encoding && ctxt->encoding[0] != '\0')
        state->context->encoding = (const char *)ctxt->encoding;
    else if (ctxt->input)
        state->context->encoding = (const char *)ctxt->input->encoding;

    // process any upper layer start document handling
    state->context->handler->start_document(state->context);

    SRCSAX_DEBUG_END("");
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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    update_ctx(ctx);

    SRCSAX_DEBUG_START("");

    // process any upper layer end document handling
    state->context->handler->end_document(state->context);

    SRCSAX_DEBUG_END("");
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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    update_ctx(ctx);

    SRCSAX_DEBUG_START(localname);

    state->mode = ROOT;

    // save the root start tag because we are going to parse it again to generate proper start_root() and start_unit()
    // calls after we know whether this is an archive or not
    state->rootstarttag.reserve(static_cast<std::size_t>(ctxt->input->cur - state->base + 2));
    state->rootstarttag.assign((const char*) state->base, static_cast<std::size_t>(ctxt->input->cur - state->base));
    state->rootstarttag += '/';
    state->rootstarttag += '>';

    // record namespace string in an extensible list so we can add the per unit
    if (state->collect_unit_body) {

        // precalculate length
        int ns_length = nb_namespaces * 2;

        // Convert to string_view
        std::vector<std::string_view> namespaceStrings(static_cast<std::size_t>(ns_length));
        for (std::size_t i = 0; i < static_cast<std::size_t>(ns_length); ++i) {
            namespaceStrings[i] = namespaces[i] ? (const char*) namespaces[i] : "";
        }

        std::size_t size = 0;
        for (std::size_t i = 0; i < static_cast<std::size_t>(ns_length); i += 2) {

            // state->rootnsstr += "xmlns";
            size += 5;
            if (namespaces[i]) {
                // state->rootnsstr += ":";
                // state->rootnsstr += (const char*) namespaces[i];
                size += 1 + namespaceStrings[i].size();
            }
            // state->rootnsstr += "=\"";
            // state->rootnsstr += (const char*) namespaces[i + 1];
            // state->rootnsstr += "\" ";
            size += 2 + namespaceStrings[i + 1].size() + 2;
        }

        state->rootnsstr.reserve(size);

        for (int i = 0; i < ns_length; i += 2) {

            for (auto c : "xmlns"sv)
                state->rootnsstr += c;

            if (namespaces[i]) {
                state->rootnsstr += ':';
                state->rootnsstr += namespaceStrings[static_cast<std::size_t>(i)];
            }
            for (auto c : "=\""sv)
                state->rootnsstr += c;
            state->rootnsstr += namespaceStrings[static_cast<std::size_t>(i) + 1];
            for (auto c : "\" "sv)
                state->rootnsstr += c;
        }
    }

    SRCML_DEBUG("UNIT", state->unitsrcml.data(), state->unitsrcml.size());

    SRCSAX_DEBUG_END(localname);

    // for empty units we need to call the upper-level handling
    // and not delay it
    bool isempty = ctxt->input->cur[0] == '/';
    if (isempty)
        state->context->is_archive = false;

    // call the upper-level start_root when an empty element
    if (isempty) {
        state->rootcalled = true;
        state->context->handler->start_root(state->context, (const char*) localname,
                            (const char*) prefix, (const char*) URI,
                            nb_namespaces, namespaces, nb_attributes, attributes);
    }

    // assume this is not a solo unit, but delay calling the upper levels until we are sure
    state->callupper = false;
    start_unit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, 0, attributes);
    state->callupper = true;
    state->mode = ROOT;

    // call the upper-level start_unit for non-archives
    if (isempty && !state->context->is_archive) {
        state->context->handler->start_unit(state->context, (const char*) localname,
                (const char*) prefix, (const char*) URI, nb_namespaces, namespaces, nb_attributes, attributes);
    }

    // handle nested units
    ctxt->sax->startElementNs = &first_start_element;
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
                         int nb_attributes, int /* nb_defaulted */, const xmlChar** attributes) {

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    SRCSAX_DEBUG_START(localname);

    // if macros are found, then must return, process first
    // but stay in first_start_element, since this can be between root unit and nested unit
    if (localname == state->MACRO_LIST_ENTRY) {

        state->context->handler->meta_tag(state->context, (const char*) localname, (const char*) prefix, (const char*) URI,
                                          nb_namespaces, namespaces, nb_attributes, attributes);
        return;
    }

    // archive when the first element after the root is <unit>
    state->context->is_archive = (localname == state->UNIT_ENTRY);

    // turn off first_start_element() handling
    ctxt->sax->startElementNs = &start_element;

    SRCSAX_DEBUG_END(localname);

    // call the delayed upper-level callbacks for starting a root and a unit
    // waited because we did not know yet if this was an archive
    // Basically, reparse the root start tag, collected when first parsed
    reparse_root(ctx);

    // decide if this start element is for a unit (archive), or just a regular element (solo unit)
    if (state->context->is_archive) {

        // restart unit count due to call of start_unit() in start_root() when we assumed a solo unit
        state->unit_count = 0;

        state->loc = 0;

        // unit starts for real, discarding previous start_unit() in start_root()
        start_unit(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, 0, attributes);

    } else {

        // pass on the parameters to the regular start element
        start_element(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, 0, attributes);
    }
    state->mode = UNIT;
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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    // collect cpp prefix
    state->cpp_prefix = std::nullopt;
    for (int i = 0; i < nb_namespaces; ++i) {

        if ((const char*) namespaces[i * 2 + 1] == SRCML_CPP_NS_URI) {
            state->cpp_prefix = namespaces[i * 2] ? (const char*) namespaces[i * 2] : "";
        }
    }

    update_ctx(ctx);

    SRCSAX_DEBUG_START(localname);

    state->mode = UNIT;

    state->unit_count += 1;

    std::string_view localnameString(localname ? (const char*) localname : "");
    std::string_view prefixString(prefix ? (const char*) prefix : "");

    // find end of unit tag name, e.g., end of "<unit" or "<src:unit"
    int pos = (int) (1 + localnameString.size() + (!prefixString.empty() ? prefixString.size() + 1 : 0) + 1);

    if (pos >= 0) {
        // merge the namespaces from the root into this one
        state->unitsrcml.reserve(ctxt->input->cur - state->base + 1 + (state->context->is_archive ? state->rootnsstr.size() : 0));
        state->unitsrcml.assign((const char*) state->base, static_cast<std::size_t>(pos));
        state->insert_begin = (int) state->unitsrcml.size();
        if (state->context->is_archive) {
            state->unitsrcml.append(state->rootnsstr);
            state->insert_end = (int) state->unitsrcml.size();
        }

        state->unitsrcml.append((const char*) state->base + pos, static_cast<std::size_t>(ctxt->input->cur - state->base + 1 - pos));

        if (!state->context->is_archive) {
            std::string_view s = state->unitsrcml;
            auto xmlnsPos = s.find("xmlns");
            auto firstquote = s.find("\"", xmlnsPos + 1);
            auto secondquote = s.find("\"", firstquote + 1);
            state->insert_end = (int) secondquote + 2;
        }
    }

    SRCML_DEBUG("UNIT", state->unitsrcml.data(), state->unitsrcml.size());

    // where the content begins, past the start unit tag
    state->content_begin = (int) state->unitsrcml.size();

    // update position
    state->base = ctxt->input->cur + 1;

    // upper-level start unit handling
    // note: In order to nop this, it is set to 0 sometimes, so have to check
    if (state->callupper)
        state->context->handler->start_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI,
                                            nb_namespaces, namespaces,
                                            nb_attributes, attributes);

    // assuming not collecting the unit body
    ctxt->sax->startElementNs = 0;
    ctxt->sax->ignorableWhitespace = ctxt->sax->characters = 0;
    ctxt->sax->comment = 0;
    ctxt->sax->cdataBlock = 0;
    ctxt->sax->processingInstruction = 0;

    if (!state->collect_unit_body)
        return;

    // next start tag will be for a non-unit element
    ctxt->sax->startElementNs = &start_element;
    ctxt->sax->ignorableWhitespace = ctxt->sax->characters = &characters_unit;
    ctxt->sax->comment = &comment;
    ctxt->sax->cdataBlock = &cdata_block;
    ctxt->sax->processingInstruction = &processing_instruction;

    // start to collect source
    state->unitsrc.clear();

    SRCSAX_DEBUG_END(localname);
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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    SRCSAX_DEBUG_START(localname);

    SRCML_DEBUG("UNIT", state->unitsrcml.data(), state->unitsrcml.size());

    state->mode = END_UNIT;

    state->context->handler->end_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI);

    ctxt->sax->startElementNs = &start_unit;

    ctxt->sax->ignorableWhitespace = ctxt->sax->characters = &characters_root;

    SRCSAX_DEBUG_END(localname);
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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    SRCSAX_DEBUG_START(localname);

    if (!state->rootcalled) {

        // call the delayed upper-level callbacks for starting a root and a unit
        // waited because we did not know yet if this was an archive
        // Basically, reparse the root start tag, collected when first parsed
        reparse_root(ctx);

        state->context->handler->end_unit(state->context, (const char *)localname, (const char *)prefix, (const char *)URI);
    }

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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    update_ctx(ctx);

    SRCSAX_DEBUG_START(localname);

    if (state->collect_unit_body) {

        // end previous start element
        if (state->base[0] == '>') {
            state->unitsrcml += '>';
            state->base += 1;
        }

        auto srcmllen = ctxt->input->cur - state->base;
        if (srcmllen < 0) {
            return;
        }

        SRCML_DEBUG("BASE", (const char*) state->base, static_cast<std::size_t>(srcmllen));

        state->unitsrcml.append((const char*) state->base, static_cast<std::size_t>(srcmllen));

        SRCML_DEBUG("UNIT", state->unitsrcml.data(), state->unitsrcml.size());

        // Special element <escape char="0x0c"/> used to embed non-XML characters
        // extract the value of the char attribute and add to the src (text)
        if (localname == state->ESCAPE_ENTRY) {

            std::string_view svalue((const char *)attributes[0 * 5 + 3], static_cast<std::size_t>(attributes[0 * 5 + 4] - attributes[0 * 5 + 3]));

            // use strtol() instead of atoi() since strtol() understands hex encoding of '0x0?'
            char value = (char)strtol(svalue.data(), NULL, 0);

            state->unitsrc.append(1, value);
        }
    }
    state->base = ctxt->input->cur;

    SRCSAX_DEBUG_END(localname);
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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    update_ctx(ctx);

    SRCSAX_DEBUG_START(localname);

    // collect end element tag
    if (state->collect_unit_body) {

        auto srcmllen = ctxt->input->cur - state->base;
        if (srcmllen < 0) {
            fprintf(stderr, "srcml: Internal error");
            return;
        }

        state->content_end = (int) state->unitsrcml.size() + 1;
        state->unitsrcml.append((const char*) state->base, static_cast<std::size_t>(srcmllen));

        SRCML_DEBUG("UNIT", state->unitsrcml.data(), state->unitsrcml.size());
    }

    state->base = ctxt->input->cur;

    SRCSAX_DEBUG_END(localname);

    // plain end element
    if (localname != state->UNIT_ENTRY) {
        return;
    }

    // At this point, we have the end of a unit

    if (ctxt->nameNr == 2 || !state->context->is_archive) {

        end_unit(ctx, localname, prefix, URI);
    }

    if (ctxt->nameNr == 1) {

        state->mode = END_ROOT;

        end_root(ctx, localname, prefix, URI);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * characters_root
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling at the root level.
 * Immediately calls supplied handlers function.
 */
#ifdef _MSC_VER
#   pragma warning (push)
#   pragma warning (disable : 4100)
#endif
void characters_root(void* ctx, const xmlChar* ch, int len) {

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    update_ctx(ctx);

    SRCSAX_DEBUG_START_CHARS(ch, len);

    // skip over
    state->base = ctxt->input->cur;

    SRCSAX_DEBUG_END_CHARS(ch, len);
}
#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#pragma GCC diagnostic pop

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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    update_ctx(ctx);

    SRCSAX_DEBUG_START_CHARS(ch, len);

    BASE_DEBUG;

    if (!state->collect_unit_body)
        return;

    state->unitsrc.append((const char*) ch, static_cast<size_t>(len));

    state->loc += (int) std::count((const char*) ch, (const char*) ch + len, '\n');

    update_ctx(ctx);

    // end previous start element
    if (state->base[0] == '>') {
        state->unitsrcml += '>';
        state->base += 1;
    }

    // libxml2 handles things in the background differently for whitespace and escaped characters
    // using a different buffer. For POS (Plain Old Strings), it uses the original buffer
    if (ch == ctxt->input->cur) {

        // plain old strings
        state->unitsrcml.append((const char*) ch, static_cast<std::size_t>(len));

        // libxml2 passes ctxt->input->cur as ch, so then must increment to len
        state->base = ctxt->input->cur + len;

    } else {

        // whitespace and escaped characters
        if (*ch == '<') {
            for (auto c : "&lt;"sv)
                state->unitsrcml += c;
        } else if (*ch == '>') {
            for (auto c : "&gt;"sv)
                state->unitsrcml += c;
        } else if (*ch == '&') {
            for (auto c : "&amp;"sv)
                state->unitsrcml += c;
        } else {
            state->unitsrcml.append((const char*) ch, static_cast<std::size_t>(len));
        }

        // ctxt->input->cur is incremented before the call
        state->base = ctxt->input->cur;
    }

    SRCML_DEBUG("UNIT", state->unitsrcml.data(), state->unitsrcml.size());

    BASE_DEBUG

    SRCSAX_DEBUG_END_CHARS(ch, len);
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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    update_ctx(ctx);

    SRCSAX_DEBUG_START("");

    if (state->collect_unit_body) {

        // take the value but note it could be part of inter-unit
        for (auto c : "<!--"sv)
            state->unitsrcml += c;
        state->unitsrcml.append((const char*) value);
        for (auto c : "-->"sv)
            state->unitsrcml += c;
    }

    state->base = ctxt->input->cur;

    SRCSAX_DEBUG_END("");
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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    update_ctx(ctx);

    SRCSAX_DEBUG_START("");

    // append CDATA
    if (state->collect_unit_body) {

        // xml can get raw
        state->unitsrcml.append((const char*) state->base, static_cast<std::size_t>(ctxt->input->cur - state->base));

        // CDATA is character data
        state->unitsrc.append((const char*) value, static_cast<std::size_t>(len));

        state->base = ctxt->input->cur;
    }

    SRCSAX_DEBUG_END("");
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

    auto ctxt = (xmlParserCtxtPtr) ctx;
    if (ctxt == nullptr)
        return;
    auto state = (sax2_srcsax_handler*) ctxt->_private;
    if (state == nullptr)
        return;

    update_ctx(ctx);

    SRCSAX_DEBUG_START("");

    if (state->collect_unit_body) {

        state->unitsrcml.append((const char*) state->base, static_cast<std::size_t>(ctxt->input->cur - state->base));

        state->base = ctxt->input->cur;
    }

    SRCSAX_DEBUG_END("");
}
