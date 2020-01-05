/**
 * @file unit_utilities.cpp
 *
 * @copyright Copyright (C) 2018-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <unit_utilities.hpp>
#include <libxml/parserInternals.h>
#include <stack>

// Update unit attributes with xml parsed attributes
void unit_update_attributes(srcml_unit* unit, int num_attributes, const xmlChar** attributes) {

    // collect attributes
    for (int pos = 0; pos < num_attributes; ++pos) {

        std::string attribute = (const char*) attributes[pos * 5];
        std::string value((const char *)attributes[pos * 5 + 3], attributes[pos * 5 + 4] - attributes[pos * 5 + 3]);

        if (attribute == "timestamp")
            srcml_unit_set_timestamp(unit, value.c_str());
        else if (attribute == "hash")
            srcml_unit_set_hash(unit, value.c_str());
        else if (attribute == "language")
            srcml_unit_set_language(unit, value.c_str());
        else if (attribute == "revision")
            unit->revision = value;
        else if (attribute == "filename")
            srcml_unit_set_filename(unit, value.c_str());
        else if (attribute == "url")
            unit->url = value;
        else if (attribute == "version")
            srcml_unit_set_version(unit, value.c_str());
        else if (attribute == "tabs" || attribute == "options" || attribute == "hash")
            ;
        else {
            // if we already have the attribute, then just update the value
            // otherwise create a new one
            bool found = false;
            for (size_t i = 0; i < unit->attributes.size(); i += 2) {
                if (unit->attributes[i] == attribute) {
                    found = true;
                    unit->attributes[i + 1] = value;
                    break;
                }
            }
            if (!found) {
                unit->attributes.push_back(attribute);
                unit->attributes.push_back(value);
            }
        }
    }
}

enum { INSERT, DELETE, COMMON};

std::string extract_revision(const char* srcml, int size, int revision, bool text_only) {

    const char* DIFF_PREFIX = "diff:";

    std::stack<int> mode;
    mode.push(COMMON);

    std::string news;
    const char* p = srcml;
    const char* lp = p;
    while ((p = (const char*) memchr(p, '<', size - (p - srcml)))) {

        bool inmode = mode.top() == COMMON || (revision == 0 && mode.top() == DELETE) || (revision == 1 && mode.top() == INSERT);

        // output previous non-tag text
        if (inmode)
            news.append(lp, p - lp);

        auto sp = p;

        // skip to end of tag
        p = (const char*) memchr(p, '>', size - (p - srcml));
        ++p;

        if (strncmp(sp + 1, DIFF_PREFIX, strlen(DIFF_PREFIX)) == 0) {

            const char* tstart = sp + 1 + strlen(DIFF_PREFIX);

            if (strncmp(tstart, "delete", 6) == 0) {
                mode.push(DELETE);
            } else if (strncmp(tstart, "insert", 6) == 0) {
                mode.push(INSERT);
            } else {
                mode.push(COMMON);
            }
        }
        else if (*(sp + 1) == '/' && strncmp(sp + 2, DIFF_PREFIX, strlen(DIFF_PREFIX)) == 0) {
            mode.pop();
        }
        else {
            if (inmode && !text_only)
                news.append(sp, p - sp);
        }

        lp = p;
    }

    return news;
}

struct extract_context {
    std::string s;
    boost::optional<int> revision;
    std::stack<int> mode;
};

// Extract source code from srcml
std::string extract_src(const std::string& srcml, boost::optional<int> revision) {

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

        scontext->s.append((const char*) ch, len);
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

        if (strcmp((const char*) localname, "escape") == 0 && strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0) {
            std::string svalue((const char *)attributes[0 * 5 + 3], attributes[0 * 5 + 4] - attributes[0 * 5 + 3]);

            char value = (int)strtol(svalue.c_str(), NULL, 0);

            scontext->s.append(1, value);

        } else if (scontext->revision && strcmp((const char*) URI, SRCML_DIFF_NS_URI) == 0) {

            if (std::string((const char*) localname) == "INSERT")
                scontext->mode.push(INSERT);
            else if (std::string((const char*) localname) == "DELETE")
                scontext->mode.push(DELETE);
            else
                scontext->mode.push(COMMON);
        }
    };

    xmlParserCtxtPtr context = xmlCreateMemoryParserCtxt(srcml.c_str(), (int) srcml.size());
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

std::string attribute_revision(const std::string& attribute, int revision) {

    auto pos = attribute.find('|');
    if (pos == std::string::npos)
        return attribute;

    if (revision == SRCDIFF_REVISION_ORIGINAL)
        return attribute.substr(0, pos);

    return attribute.substr(pos + 1);
}
