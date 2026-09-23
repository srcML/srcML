
// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file unit_utilities.cpp
 *
 * @copyright Copyright (C) 2018-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <unit_utilities.hpp>
#include <libxml/parserInternals.h>
#include <stack>
#include <cstring>
#include <string_view>

using namespace ::std::literals::string_view_literals;

// Update unit attributes with xml parsed attributes
void unit_update_attributes(srcml_unit* unit, int num_attributes, const xmlChar** attributes) {

    // collect attributes
    for (int pos = 0; pos < num_attributes; ++pos) {

        std::string_view attribute = (const char*) attributes[pos * 5];
        std::string_view prefix = attributes[pos * 5 + 1] ? ((const char*) attributes[pos * 5 + 1]) : "";
        std::string value((const char *)attributes[pos * 5 + 3], static_cast<size_t>(attributes[pos * 5 + 4] - attributes[pos * 5 + 3]));
        if (attribute == "timestamp"sv)
            srcml_unit_set_timestamp(unit, value.data());
        else if (attribute == "hash"sv)
            srcml_unit_set_hash(unit, value.data());
        else if (attribute == "language"sv)
            srcml_unit_set_language(unit, value.data());
        else if (attribute == "revision"sv)
            unit->revision = value;
        else if (attribute == "filename"sv)
            srcml_unit_set_filename(unit, value.data());
        else if (attribute == "url"sv)
            unit->url = value;
        else if (attribute == "version"sv)
            srcml_unit_set_version(unit, value.data());
        else if (attribute == "tabs"sv || attribute == "options"sv)
            ;
        else {

            // add custom attribute
            addAttribute(unit->attributes, "", prefix, attribute, value);
        }
    }
}

// Extract source code from srcml
std::string extract_src(std::string_view srcml) {

    std::string src;

    // parse the srcml collecting the (now needed) src
    xmlSAXHandler charactersax;
    memset(&charactersax, 0, sizeof(charactersax));
    charactersax.initialized    = XML_SAX2_MAGIC;

    charactersax.ignorableWhitespace = charactersax.characters = [](void* ctx, const xmlChar* ch, int len) {

        auto ctxt = (xmlParserCtxtPtr) ctx;
        if (ctxt == nullptr)
            return;
        auto src = (std::string*) ctxt->_private;
        if (src == nullptr)
            return;

        src->append((const char*) ch, static_cast<size_t>(len));
    };

    charactersax.startElementNs = [](void* ctx, const xmlChar* localname, const xmlChar* /* prefix */, const xmlChar* URI,
                     int /* nb_namespaces */, const xmlChar** /* namespaces */,
                     int /* nb_attributes */, int /* nb_defaulted */, const xmlChar** attributes) {

        auto ctxt = (xmlParserCtxtPtr) ctx;
        if (ctxt == nullptr)
            return;
        auto src = (std::string*) ctxt->_private;
        if (src == nullptr)
            return;

        if ("escape"sv == (const char*) localname && "http://www.srcML.org/srcML/src"sv == (const char*) URI) {
            std::string svalue((const char *)attributes[0 * 5 + 3], static_cast<std::size_t>(attributes[0 * 5 + 4] - attributes[0 * 5 + 3]));

            // use strtol() instead of atoi() since strtol() understands hex encoding of '0x0?'
            char value = (char)strtol(svalue.data(), NULL, 0);

            src->append(1, value);

        }
    };

    xmlParserCtxtPtr context = xmlCreateMemoryParserCtxt(srcml.data(), (int) srcml.size());
    auto save_private = context->_private;
    context->_private = &src;
    auto save_sax = context->sax;
    context->sax = &charactersax;

    xmlParseDocument(context);

    context->_private = save_private;
    context->sax = save_sax;

    xmlFreeParserCtxt(context);

    return src;
}
