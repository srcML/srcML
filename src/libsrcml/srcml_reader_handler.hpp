/**
 * @file srcml_reader_handler.hpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_SRCML_READER_HANDLER_HPP
#define INCLUDED_SRCML_READER_HANDLER_HPP

#include <srcSAXHandler.hpp>
#include <sax2_srcsax_handler.hpp>

#include <srcml_element.hpp>
#include <srcml_types.hpp>
#include <srcml_macros.hpp>
#include <srcml.h>

#include <libxml/parser.h>
#include <stdio.h>
#include <srcmlns.hpp>

#include <string>
#include <vector>
#include <stack>

#include <cstring>

#include <mutex>
#include <condition_variable>
#include <boost/optional.hpp>

#define ATTR_LOCALNAME(pos) (pos * 5)
#define ATTR_PREFIX(pos) (pos * 5 + 1)
#define ATTR_URI(pos) (pos * 5 + 2)
#define ATTR_VALUE_START(pos) (pos * 5 + 3)
#define ATTR_VALUE_END(pos) (pos * 5 + 4)

/**
 * srcsax_attribute
 *
 * Data structure for a srcML/xml attribute
 */
struct attribute_t {

    /** attribute name */
    boost::optional<std::string> localname;

    /** attribute namespace prefix */
    boost::optional<std::string> prefix;

    /** attribute namespace uri */
    boost::optional<std::string> uri;

    /** attribute value */
    boost::optional<std::string> value;
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

    /** output buffer for direct src write */
    xmlOutputBufferPtr output_buffer = nullptr;

    /** has reached end of parsing*/
    bool is_done = false;
    /** has passed root*/
    bool read_root = false;
    /** stop after collecting unit attribute*/
    bool collect_unit_attributes = false;
    /** collect srcML as parse*/
    bool collect_srcml = false;
    /** bool collect src */
    bool collect_src = false;

    /** terminate */
    bool terminate = false;

    /** track if empty unit */
    bool is_empty = false;

    /** indicate if we need to wait on the root */
    bool wait_root = true;

    /** skip internal unit elements */
    bool skip = false;

    /** srcDiff namespace */
    bool issrcdiff = false;

    /** number of newlines in unit */
    int loc = 0;

    /** last character read in */
    char lastchar = 0;

    /**
     * meta_tag
     *
     * Store a meta tag for later output.
     */
     struct meta_tag {

        /** metatags localname */
        std::string localname;

        /** metatags prefix */
        std::string prefix;

        /** meta tags attributes */
        std::vector<const xmlChar*> attributes;

        /**
         * meta_tag
         * @param localname the meta tag name
         * @param prefix the meta tag prefix
         * @param num_attributes the number attributes on the meta tag
         * @param attributes the attributes on the meta tag
         *
         * Construct meta_tag from SAX data.
         */
        meta_tag(const char* localname, const char* prefix, int num_attributes, const xmlChar** attributes)
            : localname(localname), prefix(prefix) {

            this->attributes.reserve(num_attributes * 5);
            for (int pos = 0; pos < num_attributes * 5; ++pos) {
                this->attributes[pos] = attributes[pos];
            }
        }

        /**
         * meta_tag
         * @param other another meta_tag
         *
         * Copy constructor.
         */
        meta_tag(const meta_tag& other) {

            localname = other.localname;
            prefix = other.prefix;
            attributes = other.attributes;
        }

        /**
         * operator=
         * @param other another meta_tag
         *
         * Overloaded assignment operator
         * Returns the assigned to meta_tag
         */
        meta_tag& operator=(meta_tag& other) {

            swap(other);

            return *this;
        }

        /**
         * swap
         * @param other another meta_tag
         *
         * swap the contents of the meta tags.
         */
        void swap(meta_tag & other) {

            std::swap(localname, other.localname);
            std::swap(prefix, other.prefix);
            std::swap(attributes, other.attributes);
        }

        /**
         * ~meta_tag
         *
         * Destructor
         */
        ~meta_tag() {}
     };

    /** save meta tags to use when non-archive write unit */
    std::vector<meta_tag> meta_tags;

    /** srcDiff enum */
    enum srcdiff_operation { COMMON, DELETE, INSERT };

    /** srcDiff enum stack */
    std::stack<srcdiff_operation> srcdiff_stack;

    /** the srcdiff revision to extract */
    const boost::optional<size_t>& revision_number;

    std::string attribute_revision(const std::string& attribute) {

        if (!revision_number)
            return attribute;

        auto pos = attribute.find('|');
        if (pos == std::string::npos)
            return attribute;

        if (*revision_number == SRCDIFF_REVISION_ORIGINAL)
            return attribute.substr(0, pos);

        return attribute.substr(pos + 1);
    }

public :

    /** Give access to members for srcml_sax2_reader class */
    friend class srcml_sax2_reader;

    /**
     * srcml_reader_handler
     *
     * Constructor.  Sets up mutex, conditions and state.
     */
    srcml_reader_handler(const boost::optional<size_t>& revision_number)
        : revision_number(revision_number) {

        archive = srcml_archive_create();

        srcml_archive_disable_option(archive, SRCML_OPTION_HASH);
    }

    /**
     * ~srcml_reader_handler
     *
     * Destructor, deletes mutex and conditions.
     */
    ~srcml_reader_handler() {

        srcml_archive_free(archive);
        if (unit)
            srcml_unit_free(unit);
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

        cond.notify_all();
    }

    /**
     * resume_and_wait
     *
     * Atomic resume SAX2 execution then wait.
     */
    void resume_and_wait() {

        std::unique_lock<std::mutex> lock(mutex);
        cond.notify_all();
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
        
        cond.notify_all();
    }

    /**
     * stop
     *
     * Stops SAX2 parsing Completely.  Parsing
     * Can not be restarted.
     */
    void stop() {

        terminate = true;

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
    virtual void startRoot(const char* localname, const char* prefix, const char* URI,
                           int num_namespaces, const xmlChar** namespaces, int num_attributes,
                           const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if (!is_archive)
            srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);

        // collect attributes
        for (int pos = 0; pos < num_attributes; ++pos) {

            std::string attribute = (const char*) attributes[pos * 5];
            std::string value;
            value.append((const char *)attributes[pos * 5 + 3], attributes[pos * 5 + 4] - attributes[pos * 5 + 3]);
            value = attribute_revision(value);

            // Note: these are ignore instead of placing in attributes.
            if (attribute == "timestamp")
                ;
            else if (attribute == "hash")
                ;
            else if (attribute == "language")
                ;
            else if (attribute == "revision")
                archive->revision = value;
            else if (attribute == "filename")
                ;
            else if (attribute == "url")
                srcml_archive_set_url(archive, value.c_str());
            else if (attribute == "version")
                srcml_archive_set_version(archive, value.c_str());
            else if (attribute == "tabs")
                archive->tabstop = atoi(value.c_str());
            else if (attribute == "options") {

                while(!value.empty()) {

                    std::string::size_type pos = value.find(",");
                    std::string option = value.substr(0, pos);
                    if (pos == std::string::npos)
                        value = "";
                    else
                        value = value.substr(value.find(",") + 1);

                    if (option == "XMLDECL")
                        archive->options |= SRCML_OPTION_XML_DECL;
                    else if (option == "NAMESPACEDECL")
                        archive->options |= SRCML_OPTION_NAMESPACE_DECL;
                    else if (option == "CPP_TEXT_ELSE")
                        archive->options |= SRCML_OPTION_CPP_TEXT_ELSE;
                    else if (option == "CPP_MARKUP_IF0")
                        archive->options |= SRCML_OPTION_CPP_MARKUP_IF0;
                    else if (option == "LINE")
                        archive->options |= SRCML_OPTION_LINE;
                    else if (option == "CPPIF_CHECK")
                        archive->options |= SRCML_OPTION_CPPIF_CHECK;
                }

            } else if (attribute == "hash") 
                ;
            else {

                archive->attributes.push_back(attribute);
                archive->attributes.push_back(value);
            }
        }

        // collect namespaces
        for (int pos = 0; pos < num_namespaces; ++pos) {

            std::string prefix = (const char*) namespaces[pos * 2] ? (const char*) namespaces[pos * 2] : "";
            std::string uri = (const char*) namespaces[pos * 2 + 1] ? (const char*) namespaces[pos * 2 + 1] : "";

            srcml_uri_normalize(uri);

            if (uri == SRCML_DIFF_NS_URI)
                issrcdiff = true;

            srcml_archive_register_namespace(archive, prefix.c_str(), uri.c_str());
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
    virtual void startUnit(const char* localname, const char* prefix, const char* URI,
                           int num_namespaces, const xmlChar** namespaces, int num_attributes,
                           const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if (issrcdiff)
            srcdiff_stack.push(COMMON);

        // pause
        // @todo this may need to change because, meta tags have separate call now
        if (!read_root) {

            {                
                std::unique_lock<std::mutex> lock(mutex);

                if (terminate)
                    stop_parser();
                wait_root = false;
                cond.notify_all();
                cond.wait(lock);
                read_root = true;        
            }

            if (terminate) {

                stop_parser();
                return;
            }
        }

        unit = srcml_unit_create(archive);
        unit->unit = "";

        is_empty = true;

        // collect attributes
        for (int pos = 0; pos < num_attributes; ++pos) {

            std::string attribute = (const char*) attributes[pos * 5];
            std::string value((const char *)attributes[pos * 5 + 3], attributes[pos * 5 + 4] - attributes[pos * 5 + 3]);
            value = attribute_revision(value);
            
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
            else if (attribute == "version")
                srcml_unit_set_version(unit, value.c_str());
            else if (attribute == "tabs" || attribute == "options" || attribute == "hash")
                ;
            else if (attribute == "src-encoding")
                archive->options |= SRCML_OPTION_STORE_ENCODING, srcml_unit_set_src_encoding(unit, value.c_str());
            else {

                unit->attributes.push_back(attribute);
                unit->attributes.push_back(value);
            }
        }

        if (collect_unit_attributes) {

            // pause
            std::unique_lock<std::mutex> lock(mutex);
            if (terminate)
                stop_parser();

            cond.notify_all();
            cond.wait(lock);
        }

        if (skip) {
            get_controller().enable_startElement(false);
            get_controller().enable_charactersUnit(false);
            get_controller().enable_comment(false);
            get_controller().enable_cdataBlock(false);
        }

        if (collect_srcml) {

            write_startTag(localname, prefix, num_namespaces, namespaces, num_attributes, attributes);

            if (!is_archive) {

                if (meta_tags.size()) {

                    *unit->unit += ">";
                    is_empty = false;
                }

                for (std::vector<meta_tag>::size_type i = 0; i < meta_tags.size(); ++i) {

                    try {

                        meta_tag & meta_tag = meta_tags.at(i);
                        write_startTag(meta_tag.localname.c_str(), meta_tag.prefix.c_str(), 0, 0, (int)meta_tag.attributes.size(), meta_tag.attributes.data());
                        write_endTag(meta_tag.localname.c_str(), meta_tag.prefix.c_str(), true);

                    } catch(...) { /** @todo handle */ continue; }
                }
            }

            unit->content_begin = (int) unit->unit->size() + 1;
        }

        // number of newlines reset
        loc = 0;
        lastchar = 0;

        if (terminate)
            stop_parser();

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
    }

    /**
     * startElement
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Overidden startElementNs to handle collection of srcML elements.
     */
    virtual void startElement(const char* localname, const char* prefix, const char* URI,
                                int num_namespaces, const xmlChar** namespaces, int num_attributes,
                                const xmlChar** attributes) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if (issrcdiff) {

            if (issrcdiff && URI && is_srcml_namespace(URI, SRCML_DIFF_NS_URI)) {

                std::string local_name(localname);

                if (local_name == "common")
                    srcdiff_stack.push(COMMON);
                else if (local_name == "delete")
                    srcdiff_stack.push(DELETE);
                else if (local_name == "insert")
                    srcdiff_stack.push(INSERT);
            }

            if (issrcdiff && revision_number) {

                if (is_srcml_namespace(URI, SRCML_DIFF_NS_URI))
                    return;
                if (*revision_number == SRCDIFF_REVISION_ORIGINAL && srcdiff_stack.top() == INSERT)
                    return;
                if (*revision_number == SRCDIFF_REVISION_MODIFIED && srcdiff_stack.top() == DELETE)
                    return;
            }
        }

        if (collect_src && localname[0] == 'e' && localname[1] == 's'
           && strcmp((const char *)localname, "escape") == 0) {

            std::string svalue;
            svalue.append((const char *)attributes[0 * 5 + 3], attributes[0 * 5 + 4] - attributes[0 * 5 + 3]);

            char value = (int)strtol(svalue.c_str(), NULL, 0);

            charactersUnit(&value, 1);
        }

        if (is_empty && collect_srcml)
            *unit->unit += ">";
        is_empty = true;

        if (collect_srcml) {
            write_startTag(localname, prefix, num_namespaces, namespaces, num_attributes, attributes);
        }

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
    virtual void endRoot(const char* localname, const char* prefix, const char* URI) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
        if (!read_root) {

            {            
                std::unique_lock<std::mutex> lock(mutex);
                if (terminate)
                    stop_parser();
                wait_root = false;
                cond.notify_all();
                cond.wait(lock);
                read_root = true;
            }

            if (terminate) {

                stop_parser();
                return;
            }
        }

        {
            std::unique_lock<std::mutex> lock(mutex);
            if (terminate)
                stop_parser();
            is_done = true;
            cond.notify_all();
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
    virtual void endUnit(const char* localname, const char* prefix, const char* URI) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        // if the last character read in did not have a newline, add one to the loc
        if (lastchar && lastchar != '\n')
            ++loc;

        if (issrcdiff)
            srcdiff_stack.pop();

        if (skip) {

            get_controller().enable_startElement(true);
            get_controller().enable_charactersUnit(true);
            get_controller().enable_comment(true);
            get_controller().enable_cdataBlock(true);
        }

        if (collect_srcml) {

            unit->content_end = (int) unit->unit->size() + 1;
            
            write_endTag(localname, prefix, is_empty);
        }

        //if (is_empty) *unit->unit += ">";
        if (collect_srcml || collect_src) {

            // pause
            std::unique_lock<std::mutex> lock(mutex);
            if (terminate) stop_parser();
            cond.notify_all();
            cond.wait(lock);
        }

        is_empty = false;

        srcml_unit_free(unit);
        unit = 0;

        if (terminate)
            stop_parser();

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
    }

    /**
     * endElementNs
     * @param localname tag name
     * @param prefix prefix for the tag
     * @param URI uri for tag
     *
     * Overidden endElementNs to collect srcML.
     */
    virtual void endElement(const char* localname, const char* prefix, const char* URI) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if (issrcdiff) {
            
            if (!skip && URI && is_srcml_namespace(URI, SRCML_DIFF_NS_URI)) {

                std::string local_name(localname);
                if (local_name != "ws")
                    srcdiff_stack.pop();

            }

            if (revision_number) {

                if (is_srcml_namespace(URI, SRCML_DIFF_NS_URI)) return;
                if (*revision_number == SRCDIFF_REVISION_ORIGINAL && srcdiff_stack.top() == INSERT) return;
                if (*revision_number == SRCDIFF_REVISION_MODIFIED && srcdiff_stack.top() == DELETE) return;
            }
        }

        if (collect_srcml) {
            write_endTag(localname, prefix, is_empty);
        }

        is_empty = false;

        if (terminate)
            stop_parser();

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
    }

    /**
     * charactersUnit
     * @param ch the characters
     * @param len length of the characters
     *
     * Overidden charactersUnit to collect srcML.
     */
    virtual void charactersUnit(const char* ch, int len) {

#ifdef SRCSAX_DEBUG
        std::string chars;
        chars.append((const char *)ch, len);
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

        if (issrcdiff && revision_number) {

            if (*revision_number == SRCDIFF_REVISION_ORIGINAL && srcdiff_stack.top() == INSERT)
                return;
            if (*revision_number == SRCDIFF_REVISION_MODIFIED && srcdiff_stack.top() == DELETE)
                return;
        }        

        if (is_empty && collect_srcml)
            *unit->unit += ">";
        is_empty = false;

        // update LOC
        loc += std::count(ch, ch + len, '\n');

        // record the last character so we can determine final line
        if (len)
            lastchar = ch[len - 1];

        if (collect_src) {

            xmlOutputBufferWrite(output_buffer, len, (const char *)ch);

        } else {

            for (int i = 0; i < len; ++i) {
                char character = (char)ch[i];

                if (character == '&')
                    (*unit->unit) += "&amp;";
                else if (character == '<')
                    (*unit->unit) += "&lt;";
                else if (character == '>')
                    (*unit->unit) += "&gt;";
                else
                    (*unit->unit) += character;
            }
        }

        if (terminate)
            stop_parser();

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
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
    virtual void metaTag(const char* localname, const char* prefix, const char* URI,
                           int num_namespaces, const xmlChar** namespaces, int num_attributes,
                           const xmlChar** attributes) {

        if (strcmp(localname, "macro-list") == 0) {

            std::string token;
            std::string type;

            for (int pos = 0; pos < num_attributes; ++pos) {

                if (strcmp((const char*) attributes[ATTR_LOCALNAME(pos)], "token") == 0)
                    token.append((const char*) attributes[ATTR_VALUE_START(pos)], attributes[ATTR_VALUE_END(pos)] - attributes[ATTR_VALUE_START(pos)]);
                else if (strcmp((const char*) attributes[ATTR_LOCALNAME(pos)], "type") == 0)
                    type.append((const char*) attributes[ATTR_VALUE_START(pos)], attributes[ATTR_VALUE_END(pos)] - attributes[ATTR_VALUE_START(pos)]);
            }

            if (token != "" && type != "") {

                archive->user_macro_list.push_back(token);
                archive->user_macro_list.push_back(type);
            }
        }

        if (!is_archive) {

            meta_tags.push_back(meta_tag(localname, prefix, num_attributes, attributes));
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
        
        srcml_archive_set_processing_instruction(archive, (const char*)target, (const char *)data);

    }

#pragma GCC diagnostic pop

private :

#define NS_URI(pos) (pos * 2 + 1)
#define NS_PREFIX(pos) (pos * 2)

    /**
     * write_startTag
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Write out the start tag to the unit string.
     */
    void write_startTag(const char* localname, const char* prefix,
                           int num_namespaces, const xmlChar** namespaces, int /* num_attributes */,
                           const std::vector<attribute_t> attributes) {

        *unit->unit += "<";
        if (prefix) {
            *unit->unit += prefix;
            *unit->unit += ":";
        }
        *unit->unit += localname;

        for (int pos = 0; pos < num_namespaces; ++pos) {

            if (is_archive && strcmp(localname, "unit") == 0 && !is_srcml_namespace((const char*) namespaces[NS_URI(pos)], SRCML_CPP_NS_URI))
                continue;

            if (revision_number && is_srcml_namespace((const char*) namespaces[NS_URI(pos)], SRCML_DIFF_NS_URI))
                continue;

            *unit->unit += " xmlns";
            if (namespaces[NS_PREFIX(pos)]) {

                *unit->unit += ":";
                *unit->unit += (const char*) namespaces[NS_PREFIX(pos)];
            }

            *unit->unit += "=\"";
            *unit->unit += (const char*) namespaces[NS_URI(pos)];
            *unit->unit += "\"";
        }

        for (const auto& attr : attributes) {

            std::string value = attribute_revision(*attr.value);
            if (attr.value && *attr.value != "" && value == "")
                continue;

            *unit->unit += " ";
            if (attr.prefix) {

                *unit->unit += *attr.prefix;
                *unit->unit += ":";
            }
            *unit->unit += *attr.localname;

            *unit->unit += "=\"";
            *unit->unit += value;
            *unit->unit += "\"";
        }
        //*unit->unit += ">";
    }

   /**
     * write_startTag
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Write out the start tag to the unit string.
     */
    void write_startTag(const char* localname, const char* prefix,
                           int num_namespaces, const xmlChar** namespaces, int num_attributes,
                           const xmlChar** attributes) {

        *unit->unit += "<";
        if (prefix) {
            *unit->unit += prefix;
            *unit->unit += ":";
        }
        *unit->unit += localname;

        for (int pos = 0; pos < num_namespaces; ++pos) {

            if (is_archive && strcmp(localname, "unit") == 0 && !is_srcml_namespace((const char*) namespaces[NS_URI(pos)], SRCML_CPP_NS_URI))
                continue;

            if (revision_number && is_srcml_namespace((const char*) namespaces[NS_URI(pos)], SRCML_DIFF_NS_URI))
                continue;

            *unit->unit += " xmlns";
            if (namespaces[NS_PREFIX(pos)]) {

                *unit->unit += ":";
                *unit->unit += (const char*) namespaces[NS_PREFIX(pos)];
            }

            *unit->unit += "=\"";
            *unit->unit += (const char*) namespaces[NS_URI(pos)];
            *unit->unit += "\"";
        }

        for (int pos = 0; pos < num_attributes; ++pos) {

            std::string revision;
            revision.append((const char *)attributes[pos * 5 + 3], attributes[pos * 5 + 4] - attributes[pos * 5 + 3]);
            std::string value = attribute_revision(revision);
            if (revision != "" && value == "") continue;

            *unit->unit += " ";
            if (attributes[ATTR_PREFIX(pos)]) {

                *unit->unit += (const char*) attributes[ATTR_PREFIX(pos)];
                *unit->unit += ":";
            }
            *unit->unit += (const char*) attributes[ATTR_LOCALNAME(pos)];

            *unit->unit += "=\"";
            *unit->unit += value;
            *unit->unit += "\"";
        }
        //*unit->unit += ">";
    }

    /**
     * endElementNs
     * @param localname tag name
     * @param prefix prefix for the tag
     * @param URI uri for tag
     *
     * Write out the end tag to the unit string.
     */
    void write_endTag(const char* localname, const char* prefix, bool is_empty) {

        if (is_empty) {

            *unit->unit += "/>";
            return;
        }

        *unit->unit += "</";
        if (prefix) {

            *unit->unit += (const char *)prefix;
            *unit->unit += ":";
        }
        *unit->unit += (const char *)localname;

        *unit->unit += ">";
    }
};

#endif
