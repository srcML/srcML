// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file unit_utilities.cpp
 *
 * @copyright Copyright (C) 2018-2019 srcML, LLC. (www.srcML.org)
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

#undef DELETE

enum { INSERT, DELETE, COMMON };

std::string extract_revision(const char* srcml, int size, int revision, bool text_only) {

    std::string_view DIFF_PREFIX = "diff:"sv;

    std::stack<int> mode;
    mode.push(COMMON);

    std::string news;
    const char* p = srcml;
    const char* lastp = p;
    while ((p = (const char*) memchr(p, '<', static_cast<size_t>(size - (p - srcml))))) {

        bool inmode = mode.top() == COMMON || (revision == 0 && mode.top() == DELETE) || (revision == 1 && mode.top() == INSERT);

        // output previous non-tag text
        if (inmode)
            news.append(lastp, static_cast<size_t>(p - lastp));

        auto sp = p;

        // skip to end of tag
        p = (const char*) memchr(p, '>', static_cast<size_t>(size - (p - srcml)));
        ++p;

        if (strncmp(sp + 1, DIFF_PREFIX.data(), DIFF_PREFIX.size()) == 0) {

            const char* tstart = sp + 1 + DIFF_PREFIX.size();

            if (strncmp(tstart, "delete", 6) == 0) {
                mode.push(DELETE);
            } else if (strncmp(tstart, "insert", 6) == 0) {
                mode.push(INSERT);
            } else {
                mode.push(COMMON);
            }
        }
        else if (*(sp + 1) == '/' && strncmp(sp + 2, DIFF_PREFIX.data(), DIFF_PREFIX.size()) == 0) {
            mode.pop();
        }
        else {
            if (inmode && !text_only)
                news.append(sp, static_cast<size_t>(p - sp));
        }

        lastp = p;
    }

    return news;
}

struct extract_context {
    std::string s;
    std::optional<int> revision;
    std::stack<int> mode;
};

// Extract source code from srcml
std::string extract_src(std::string_view srcml, std::optional<int> revision) {

    extract_context scontext;
    scontext.revision = revision;
    scontext.mode.push(COMMON);

    // parse the srcml collecting the (now needed) src
    xmlSAXHandler charactersax;
    memset(&charactersax, 0, sizeof(charactersax));
    charactersax.initialized    = XML_SAX2_MAGIC;

    charactersax.ignorableWhitespace = charactersax.characters = [](void* ctx, const xmlChar* ch, int len) {

        auto ctxt = (xmlParserCtxtPtr) ctx;
        if (ctxt == nullptr)
            return;
        auto scontext = (extract_context*) ctxt->_private;
        if (scontext == nullptr)
            return;

        scontext->s.append((const char*) ch, static_cast<size_t>(len));
    };

    charactersax.startElementNs = [](void* ctx, const xmlChar* localname, const xmlChar* /* prefix */, const xmlChar* URI,
                     int /* nb_namespaces */, const xmlChar** /* namespaces */,
                     int /* nb_attributes */, int /* nb_defaulted */, const xmlChar** attributes) {

        auto ctxt = (xmlParserCtxtPtr) ctx;
        if (ctxt == nullptr)
            return;
        auto scontext = (extract_context*) ctxt->_private;
        if (scontext == nullptr)
            return;

        if ("escape"sv == (const char*) localname && "http://www.srcML.org/srcML/src"sv == (const char*) URI) {
            std::string svalue((const char *)attributes[0 * 5 + 3], static_cast<std::size_t>(attributes[0 * 5 + 4] - attributes[0 * 5 + 3]));

            // use strtol() instead of atoi() since strtol() understands hex encoding of '0x0?'
            char value = (char)strtol(svalue.data(), NULL, 0);

            scontext->s.append(1, value);

        } else if (scontext->revision && SRCML_DIFF_NS_URI == (const char*) URI) {

            if ((const char*) localname == "INSERT"sv)
                scontext->mode.push(INSERT);
            else if ((const char*) localname == "DELETE"sv)
                scontext->mode.push(DELETE);
            else
                scontext->mode.push(COMMON);
        }
    };

    xmlParserCtxtPtr context = xmlCreateMemoryParserCtxt(srcml.data(), (int) srcml.size());
    auto save_private = context->_private;
    context->_private = &scontext;
    auto save_sax = context->sax;
    context->sax = &charactersax;

    xmlParseDocument(context);

    context->_private = save_private;
    context->sax = save_sax;

    xmlFreeParserCtxt(context);

    return scontext.s;
}

std::string_view attribute_revision(std::string_view attribute, int revision) {

    auto pos = attribute.find('|');
    if (pos == std::string::npos)
        return attribute;

    if (revision == SRCDIFF_REVISION_ORIGINAL)
        return attribute.substr(0, pos);

    return attribute.substr(pos + 1);
}
