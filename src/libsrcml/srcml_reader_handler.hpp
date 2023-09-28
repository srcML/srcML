// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_reader_handler.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_SRCML_READER_HANDLER_HPP
#define INCLUDED_SRCML_READER_HANDLER_HPP

#include <srcSAXHandler.hpp>
#include <sax2_srcsax_handler.hpp>

#include <srcml_types.hpp>
#include <srcml.h>

#include <unit_utilities.hpp>

#include <libxml/parser.h>
#include <stdio.h>
#include <srcmlns.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <stack>

#include <mutex>
#include <condition_variable>
#include <optional>

using namespace ::std::literals::string_view_literals;

#define ATTR_LOCALNAME(pos) (pos * 5)
#define ATTR_PREFIX(pos) (pos * 5 + 1)
#define ATTR_URI(pos) (pos * 5 + 2)
#define ATTR_VALUE_START(pos) (pos * 5 + 3)
#define ATTR_VALUE_END(pos) (pos * 5 + 4)

#undef SRCSAX_DEBUG

/**
 * srcsax_attribute
 *
 * Data structure for a srcML/xml attribute
 */
struct attribute_t {

    /** attribute name */
    std::optional<std::string> localname;

    /** attribute namespace prefix */
    std::optional<std::string> prefix;

    /** attribute namespace uri */
    std::optional<std::string> uri;

    /** attribute value */
    std::optional<std::string> value;
};

/**
 * srcml_reader_handler
 *
 * Inherits from srcMLHandler to provide hooks into
 * SAX2 parsing. Provides starting and stoping using
 * threads.  Collects attributes, namespaces and srcML
 * from units.
 */
class srcml_reader_handler : public srcSAXHandler {

private :

    /** mutex to halt both threads on */
    std::mutex mutex;

    /** sax stop/start condition */
    std::condition_variable cond;

    /** collected root language */
    srcml_archive* archive = nullptr;

    /** collected unit language */
    srcml_unit* unit = nullptr;

    /** has reached end of parsing*/
    bool is_done = false;
    /** has passed root*/
    bool read_root = false;
    /** stop after collecting unit attribute*/
    bool collect_unit_header = false;
    /** collect srcML as parse*/
    bool collect_unit_body = false;

    /** terminate */
    bool terminate = false;

    /** indicate if we need to wait on the root */
    bool wait_root = true;

    /** skip internal unit elements */
    bool skip = false;

public :

    /** Give access to members for srcml_sax2_reader class */
    friend class srcml_sax2_reader;

    /**
     * srcml_reader_handler
     *
     * Constructor.  Sets up mutex, conditions and state.
     */
    srcml_reader_handler() {
    }

    /**
     * ~srcml_reader_handler
     *
     * Destructor, deletes mutex and conditions.
     */
    virtual ~srcml_reader_handler() {
     }

    /**
     * stop_parser
     *
     * Stop the parser for threading
     */
    void stop_parser() {

        is_done = true;
        srcSAXHandler::stop_parser();
    }

    /**
     * wait
     *
     * Allows calling thread to wait until reached
     * end of unit.
     */
    void wait() {

        std::unique_lock<std::mutex> lock(mutex);

        if (is_done)
            return;

        if (wait_root)
            cond.wait(lock);
    }

    /**
     * resume
     *
     * Resume SAX2 execution.
     */
    void resume() {

        std::unique_lock<std::mutex> lock(mutex);

        cond.notify_one();
    }

    /**
     * resume_and_wait
     *
     * Atomic resume SAX2 execution then wait.
     */
    void resume_and_wait() {

        std::unique_lock<std::mutex> lock(mutex);
        cond.notify_one();
        if (is_done)
            return;

        cond.wait(lock);
    }

    /**
     * done
     *
     * Mark is done
     */
    void done() {

        is_done = true;

        cond.notify_one();
    }

    /**
     * stop
     *
     * Stops SAX2 parsing Completely.  Parsing
     * Can not be restarted.
     */
    void stop() {

        {
            std::unique_lock<std::mutex> lock(mutex);

            terminate = true;
        }

        resume();
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * startDocument
     *
     * SAX handler function for start of document.
     * Overide for desired behaviour.
     */
    virtual void startDocument() {

        srcml_archive_set_xml_encoding(archive, encoding ? encoding : "UTF-8");
    }

    /**
     * startRoot
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Overidden startRoot to handle collection of root attributes. Stop before continue
     */
    virtual void startRoot(const char* /* localname */, const char* /* prefix */, const char* /* URI */,
                           int num_namespaces, const xmlChar** namespaces, int num_attributes,
                           const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if (!is_archive)
            archive->options &= ~SRCML_OPTION_ARCHIVE;

        // collect attributes
        for (int pos = 0; pos < num_attributes; ++pos) {
            std::string_view prefix = attributes[pos * 5 + 1] ? ((const char*) attributes[pos * 5 + 1]) : "";
            std::string_view attribute = (const char*) attributes[pos * 5];
            std::string value((const char *)attributes[pos * 5 + 3], static_cast<std::size_t>(attributes[pos * 5 + 4] - attributes[pos * 5 + 3]));

            // Note: these are ignore instead of placing in attributes.
            if (attribute == "timestamp"sv)
                ;
            else if (attribute == "language"sv)
                ;
            else if (attribute == "revision"sv)
                archive->revision = value;
            else if (attribute == "filename"sv)
                ;
            else if (attribute == "url"sv) {
                srcml_archive_set_url(archive, value.data());

            }
            else if (attribute == "version"sv)
                srcml_archive_set_version(archive, value.data());
            else if (attribute == "tabs"sv)
                archive->tabstop = static_cast<std::size_t>(atoi(value.data()));
            else if (attribute == "options"sv) {

                std::size_t commaPos = 0;
                std::size_t prevCommaPos = 0;
                while(prevCommaPos < value.size()) {

                    commaPos = value.find(",", commaPos);
                    std::string_view option(&value[prevCommaPos], commaPos - prevCommaPos);
                    prevCommaPos = commaPos;
                    ++prevCommaPos;

                    if (option == "XMLDECL"sv)
                        archive->options |= SRCML_OPTION_NO_XML_DECL;
                    else if (option == "NAMESPACEDECL"sv)
                        archive->options |= SRCML_OPTION_NAMESPACE_DECL;
                    else if (option == "CPP_TEXT_ELSE"sv)
                        archive->options |= SRCML_OPTION_CPP_TEXT_ELSE;
                    else if (option == "CPP_MARKUP_IF0"sv)
                        archive->options |= SRCML_OPTION_CPP_MARKUP_IF0;
                    else if (option == "LINE"sv)
                        archive->options |= SRCML_OPTION_LINE;
                }

            } else if (attribute == "hash"sv)
                ;
            else {

                archive->attributes.emplace_back("", prefix, attribute, value);
            }
        }

        // collect namespaces
        for (int pos = 0; pos < num_namespaces; ++pos) {

            std::string_view nsPrefix = (const char*) namespaces[pos * 2] ? (const char*) namespaces[pos * 2] : "";
            std::string nsURI = (const char*) namespaces[pos * 2 + 1] ? (const char*) namespaces[pos * 2 + 1] : "";

            srcml_uri_normalize(nsURI);

            srcml_archive_register_namespace(archive, nsPrefix.data(), nsURI.data());
        }

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
    }

    /**
     * startUnit
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Overidden startUnit to handle collection of Unit attributes and tag. Stop before continue
     * if collecting attributes.
     */
    virtual void startUnit(const char* /* localname */, const char* /* prefix */, const char* /* URI */,
                           int /* num_namespaces */, const xmlChar** /* namespaces */, int num_attributes,
                           const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        // pause
        if (!read_root) {

            {
                std::unique_lock<std::mutex> lock(mutex);

                if (terminate)
                    stop_parser();
                wait_root = false;
                cond.notify_one();
                cond.wait(lock);
                read_root = true;
            }

            if (terminate) {

                stop_parser();
                return;
            }
        }

        // collect attributes
        unit_update_attributes(unit, num_attributes, attributes);

        auto ctxt = (xmlParserCtxtPtr) get_controller().getContext()->libxml2_context;
        auto state = (sax2_srcsax_handler*) ctxt->_private;

        state->collect_unit_body = collect_unit_body;

        if (collect_unit_header) {

            // pause
            std::unique_lock<std::mutex> lock(mutex);
            if (terminate)
                stop_parser();

            cond.notify_one();
            cond.wait(lock);
        }

        state->loc = 0;

        state->collect_unit_body = collect_unit_body;

        if (terminate)
            stop_parser();

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
    }

    /**
     * endRoot
     * @param localname tag name
     * @param prefix prefix for the tag
     * @param URI uri for tag
     *
     * Overidden endRoot to indicate done with parsing and
        free any waiting process.
     */
    virtual void endRoot(const char* /* localname */, const char* /* prefix */, const char* /* URI */) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        {
            std::unique_lock<std::mutex> lock(mutex);
            if (terminate)
                stop_parser();
            is_done = true;
            cond.notify_one();
        }

        if (terminate)
            stop_parser();

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
    }

    /**
     * endUnit
     * @param localname tag name
     * @param prefix prefix for the tag
     * @param URI uri for tag
     *
     * Overidden endUnit to collect srcml and stop parsing.  Clear collect srcML after pause.
     */
    virtual void endUnit(const char* /* localname */, const char* /* prefix */, const char* /* URI */) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        auto ctxt = (xmlParserCtxtPtr) get_controller().getContext()->libxml2_context;
        auto state = (sax2_srcsax_handler*) ctxt->_private;

        if (collect_unit_body) {

            if (!state->unitsrc.empty() && state->unitsrc.back() != '\n')
                ++state->loc;

            unit->content_begin = state->content_begin;
            unit->content_end = state->content_end;
            unit->insert_begin = state->insert_begin;
            unit->insert_end = state->insert_end;
            unit->srcml = std::move(state->unitsrcml);
            unit->src = std::move(state->unitsrc);
            unit->loc = state->loc;

            // update provisional cpp prefix
            if (state->cpp_prefix) {

                // namespaces probably aren't create yet
                if (!unit->namespaces) {
                    unit->namespaces = starting_namespaces;
                }

                // set the found prefix, plus mark it as used
                auto it = findNSURI(*unit->namespaces, SRCML_CPP_NS_URI);
                if (it != unit->namespaces->end()) {
                    it->flags |= NS_USED;
                } else {
                    unit->namespaces->emplace_back(state->cpp_prefix->data(), SRCML_CPP_NS_URI, NS_USED | NS_STANDARD);
                }
            }

            // pause
            std::unique_lock<std::mutex> lock(mutex);
            if (terminate) stop_parser();
            cond.notify_one();
            cond.wait(lock);
        }

        if (terminate)
            stop_parser();

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
    }

    /**
     * metaTag
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes\
     *
     * SAX handler function for a meta tags.
     * Overide for desired behaviour.
     */
    virtual void metaTag(const char* localname, const char* /* prefix */, const char* /* URI */,
                           int /* num_namespaces */, const xmlChar** /* namespaces */, int num_attributes,
                           const xmlChar** attributes) {

        if (localname == "macro-list"sv) {

            std::string token;
            std::string type;

            for (int pos = 0; pos < num_attributes; ++pos) {

                if ((const char*) attributes[ATTR_LOCALNAME(pos)] == "token"sv)
                    token.append((const char*) attributes[ATTR_VALUE_START(pos)], static_cast<std::size_t>(attributes[ATTR_VALUE_END(pos)] - attributes[ATTR_VALUE_START(pos)]));
                else if ((const char*) attributes[ATTR_LOCALNAME(pos)] == "type"sv)
                    type.append((const char*) attributes[ATTR_VALUE_START(pos)], static_cast<std::size_t>(attributes[ATTR_VALUE_END(pos)] - attributes[ATTR_VALUE_START(pos)]));
            }

            if (token != "" && type != "") {

                archive->user_macro_list.push_back(std::move(token));
                archive->user_macro_list.push_back(std::move(type));
            }

        }
    }

    /**
     * processingInstruction
     * @param target the processing instruction target.
     * @param data the processing instruction data.
     *
     * Overrident processingInstruction to collect srcML.
     */
    virtual void processingInstruction(const char* target, const char* data) {

        srcml_archive_set_processing_instruction(archive, (const char*)target, (const char*)data);

    }

#pragma GCC diagnostic pop
};

#endif
