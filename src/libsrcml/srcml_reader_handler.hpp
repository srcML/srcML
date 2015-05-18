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

#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/optional.hpp>
#pragma GCC diagnostic warning "-Wshorten-64-to-32"

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
    boost::mutex mutex;

    /** sax stop/start condition */
    boost::condition_variable cond;

    /** collected root language */
    srcml_archive * archive;

    /** collected unit language */
    srcml_unit * unit;

    /** output buffer for direct src write */
    xmlOutputBufferPtr output_buffer;

    /** has reached end of parsing*/
    bool is_done;
    /** has passed root*/
    bool read_root;
    /** stop after collecting unit attribute*/
    bool collect_unit_attributes;
    /** collect srcML as parse*/
    bool collect_srcml;
    /** bool collect src */
    bool collect_src;

    /** terminate */
    bool terminate;

    /** track if empty unit */
    bool is_empty;

    /** indicate if we need to wait on the root */
    bool wait_root;

    /** skip internal unit elements */
    bool skip;

    /**
     * meta_tag
     *
     * Store a meta tag for later output.
     */
     struct meta_tag {

        /** metatags localname */
        const char * localname;

        /** metatags prefix */
        const char * prefix;

        /** metatags number of attributes */
        int num_attributes;

        /** meta tags attributes */
        struct srcsax_attribute * attributes;

        /**
         * meta_tag
         * @param localname the meta tag name
         * @param prefix the meta tag prefix
         * @param num_attributes the number attributes on the meta tag
         * @param attributes the attributes on the meta tag
         *
         * Construct meta_tag from SAX data.
         */
        meta_tag(const char * localname, const char * prefix, int num_attributes, const struct srcsax_attribute * attributes) {

            this->localname = localname ? strdup(localname) : 0;
            this->prefix = prefix ? strdup(prefix) : 0;
            this->num_attributes = num_attributes;
            this->attributes = (struct srcsax_attribute *)calloc(num_attributes, sizeof(struct srcsax_attribute));
            for(int pos = 0; pos < num_attributes; ++pos) {

                this->attributes[pos].localname = attributes[pos].localname ? strdup(attributes[pos].localname) : 0;
                this->attributes[pos].prefix = attributes[pos].prefix ? strdup(attributes[pos].prefix) : 0;
                this->attributes[pos].uri = attributes[pos].uri ? strdup(attributes[pos].uri) : 0;
                this->attributes[pos].value = attributes[pos].value ? strdup(attributes[pos].value) : 0;

            }

        }

        /**
         * meta_tag
         * @param other another meta_tag
         *
         * Copy constructor.
         */
        meta_tag(const meta_tag & other) {

            this->localname = other.localname ? strdup(other.localname) : 0;
            this->prefix = other.prefix ? strdup(other.prefix) : 0;
            this->num_attributes = other.num_attributes;
            this->attributes = (struct srcsax_attribute *)calloc(other.num_attributes, sizeof(struct srcsax_attribute));
            for(int pos = 0; pos < other.num_attributes; ++pos) {

                this->attributes[pos].localname = other.attributes[pos].localname ? strdup(other.attributes[pos].localname) : 0;
                this->attributes[pos].prefix = other.attributes[pos].prefix ? strdup(other.attributes[pos].prefix) : 0;
                this->attributes[pos].uri = other.attributes[pos].uri ? strdup(other.attributes[pos].uri) : 0;
                this->attributes[pos].value = other.attributes[pos].value ? strdup(other.attributes[pos].value) : 0;

            }

        }


        /**
         * operator=
         * @param other another meta_tag
         *
         * Overloaded assignment operator
         * Returns the assigned to meta_tag
         */
        meta_tag & operator=(meta_tag other) {


            this->swap(other);
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
            std::swap(num_attributes, other.num_attributes);
            std::swap(attributes, other.attributes);

        }

        /**
         * ~meta_tag
         *
         * Destructor
         */
        ~meta_tag() {

            if(localname) free((void *)localname), localname = 0;
            if(prefix) free((void *)prefix), prefix = 0;

            if(attributes) {

                for(int pos = 0; pos < num_attributes; ++pos) {

                    if(attributes[pos].localname) free((void *)attributes[pos].localname);
                    if(attributes[pos].prefix) free((void *)attributes[pos].prefix);
                    if(attributes[pos].uri) free((void *)attributes[pos].uri);
                    if(attributes[pos].value) free((void *)attributes[pos].value);

                }

                free(attributes), attributes = 0;

            }

        }

     };

    /** save meta tags to use when non-archive write unit */
    std::vector<meta_tag> meta_tags;

    /** srcDiff enum */
    enum srcdiff_operation { COMMON, DELETE, INSERT };

    /** srcDiff enum stack */
    std::stack<srcdiff_operation> srcdiff_stack;

    /** original constant */
    static const size_t ORIGINAL = 0;

    /** modified constant */
    static const size_t MODIFIED = 1;

    /** the revision to extract */
    boost::optional<size_t> revision;

    std::string attribute_revision(const std::string & attribute) {

        if(!revision) return attribute;

        std::string::size_type pos = attribute.find('|');
        if(pos == std::string::npos) return attribute;

        if(*revision == ORIGINAL) return attribute.substr(0, pos);

        return attribute.substr(pos + 1, std::string::npos);

    }


public :

    /** Give access to membeers for srcml_sax2_reader class */
    friend class srcml_sax2_reader;

    /**
     * srcml_reader_handler
     *
     * Constructor.  Sets up mutex, conditions and state.
     */
    srcml_reader_handler() : unit(0), output_buffer(0), is_done(false), read_root(false),
         collect_unit_attributes(false), collect_srcml(false), collect_src(false),
         terminate(false), is_empty(false), wait_root(true), skip(false) {

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
        if(unit) srcml_unit_free(unit);

    }

    /**
     * wait
     *
     * Allows calling thread to wait until reached
     * end of unit.
     */
    void wait() {

        boost::unique_lock<boost::mutex> lock(mutex);

        if(is_done) return;

        if(wait_root) cond.wait(lock);

    }

    /**
     * resume
     *
     * Resume SAX2 execution.
     */
    void resume() {

        boost::unique_lock<boost::mutex> lock(mutex);
        cond.notify_all();

    }

    /**
     * resume_and_wait
     *
     * Atomic resume SAX2 execution then wait.
     */
    void resume_and_wait() {

        boost::unique_lock<boost::mutex> lock(mutex);
        cond.notify_all();
        if(is_done) return;

        cond.wait(lock);

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
    virtual void startRoot(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if(!is_archive) srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);

        // collect attributes
        for(int pos = 0; pos < num_attributes; ++pos) {

            std::string attribute = attributes[pos].localname;
            std::string value = attributes[pos].value;

            // Note: these are ignore instead of placing in attributes.
            if(attribute == "timestamp")
                ;
            else if(attribute == "hash")
                ;
            else if(attribute == "language")
                ;
            else if(attribute == "revision")
                archive->revision = value;
            else if(attribute == "filename")
                ;
            else if(attribute == "url")
                srcml_archive_set_url(archive, value.c_str());
            else if(attribute == "version")
                srcml_archive_set_version(archive, value.c_str());
            else if(attribute == "tabs")
                archive->tabstop = atoi(value.c_str());
            else if(attribute == "options") {

                while(!value.empty()) {

                    std::string::size_type pos = value.find(",");
                    std::string option = value.substr(0, pos);
                    if(pos == std::string::npos)
                        value = "";
                    else
                        value = value.substr(value.find(",") + 1);

                    if(option == "XMLDECL")
                        archive->options |= SRCML_OPTION_XML_DECL;
                    else if(option == "NAMESPACEDECL")
                        archive->options |= SRCML_OPTION_NAMESPACE_DECL;
                    else if(option == "CPP_TEXT_ELSE")
                        archive->options |= SRCML_OPTION_CPP_TEXT_ELSE;
                    else if(option == "CPP_MARKUP_IF0")
                        archive->options |= SRCML_OPTION_CPP_MARKUP_IF0;
                    else if(option == "EXPRESSION")
                        archive->options |= SRCML_OPTION_EXPRESSION;
                    else if(option == "LINE")
                        archive->options |= SRCML_OPTION_LINE;
                    else if(option == "NESTIF")
                        archive->options |= SRCML_OPTION_NESTIF;
                    else if(option == "CPPIF_CHECK")
                        archive->options |= SRCML_OPTION_CPPIF_CHECK;
                    else if(option == "WRAP_TEMPLATE")
                        archive->options |= SRCML_OPTION_WRAP_TEMPLATE;
                    else if(option == "OPERATOR_TERNARY")
                        archive->options &= ~SRCML_OPTION_TERNARY;

                }

            } else if(attribute == "hash") 
                ;
            else {

                archive->attributes.push_back(attribute);
                archive->attributes.push_back(value);

            }

        }

        // collect namespaces
        for(int pos = 0; pos < num_namespaces; ++pos) {

            std::string prefix = namespaces[pos].prefix ? namespaces[pos].prefix : "";
            std::string uri = namespaces[pos].uri ? namespaces[pos].uri  : "";
            srcml_uri_normalize(uri);

            if(uri == SRCML_CPP_NS_URI) {

                if(archive->language != 0) {

                    if(*archive->language == "C++" || *archive->language == "C" || *archive->language == "Objective-C")
                        archive->options |= SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO;
                    else if(*archive->language == "C#")
                        archive->options |= SRCML_OPTION_CPP_NOMACRO;

                }

            } else if(uri == SRCML_ERR_NS_URI)
                archive->options |= SRCML_OPTION_DEBUG;
            else if(uri == SRCML_EXT_LITERAL_NS_URI)
                archive->options |= SRCML_OPTION_LITERAL;
            else if(uri == SRCML_EXT_OPERATOR_NS_URI)
                archive->options |= SRCML_OPTION_OPERATOR;
            else if(uri == SRCML_EXT_MODIFIER_NS_URI)
                archive->options |= SRCML_OPTION_MODIFIER;
            else if(uri == SRCML_EXT_POSITION_NS_URI)
                archive->options |= SRCML_OPTION_POSITION;
            else if(uri == SRCML_EXT_OPENMP_NS_URI)
                archive->options |= SRCML_OPTION_OPENMP;

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
    virtual void startUnit(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        srcdiff_stack.push(COMMON);

        // pause
        // @todo this may need to change because, meta tags have separate call now
        if(!read_root) {

            {
                
                boost::unique_lock<boost::mutex> lock(mutex);
                if(terminate) stop_parser();
                wait_root = false;
                cond.notify_all();
                cond.wait(lock);
                read_root = true;
        
            }

            if(terminate) {

                stop_parser();
                return;

            }

        }

        unit = srcml_unit_create(archive);
        unit->unit = "";

        is_empty = true;

        // collect attributes
        for(int pos = 0; pos < num_attributes; ++pos) {

            std::string attribute = attributes[pos].localname;
            std::string value = attribute_revision(attributes[pos].value);

            if(attribute == "timestamp")
                srcml_unit_set_timestamp(unit, value.c_str());
            else if(attribute == "hash")
                srcml_unit_set_hash(unit, value.c_str());
            else if(attribute == "language")
                srcml_unit_set_language(unit, value.c_str());
            else if(attribute == "revision")
                unit->revision = value;
            else if(attribute == "filename")
                srcml_unit_set_filename(unit, value.c_str());
            else if(attribute == "url")
                srcml_unit_set_url(unit, value.c_str());
            else if(attribute == "version")
                srcml_unit_set_version(unit, value.c_str());
            else if(attribute == "tabs" || attribute == "options" || attribute == "hash")
                ;
            else if(attribute == "src-encoding")
                archive->options |= SRCML_OPTION_STORE_ENCODING, srcml_unit_set_src_encoding(unit, value.c_str());
            else {

                unit->attributes.push_back(attribute);
                unit->attributes.push_back(value);

            }

        }

        if(collect_unit_attributes) {

            // pause
            boost::unique_lock<boost::mutex> lock(mutex);
            if(terminate) stop_parser();
            cond.notify_all();
            cond.wait(lock);

        }

        if(skip) {

            get_controller().enable_startElement(false);
            get_controller().enable_charactersUnit(false);
            get_controller().enable_comment(false);
            get_controller().enable_cdataBlock(false);

        }

        if(collect_srcml) {

            write_startTag(localname, prefix, num_namespaces, namespaces, num_attributes, attributes);

            if(!is_archive) {

                if(meta_tags.size()) {

                    *unit->unit += ">";
                    is_empty = false;

                }

                for(std::vector<meta_tag>::size_type i = 0; i < meta_tags.size(); ++i) {


                    try {

                        meta_tag & meta_tag = meta_tags.at(i);
                        write_startTag(meta_tag.localname, meta_tag.prefix, 0, 0, meta_tag.num_attributes, meta_tag.attributes);
                        write_endTag(meta_tag.localname, meta_tag.prefix, true);

                    } catch(...) { /** @todo handle */ continue; }

                }

            }

        }

        if(terminate) stop_parser();

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
    virtual void startElement(const char * localname, const char * prefix, const char * URI,
                                int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                                const struct srcsax_attribute * attributes) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if(URI && std::string(URI) == SRCML_DIFF_NS_URI) {

            std::string local_name(localname);

            if(local_name == "common")
                srcdiff_stack.push(COMMON);
            else if(local_name == "delete")
                srcdiff_stack.push(DELETE);
            else
                srcdiff_stack.push(INSERT);

        }

        if(revision) {

            if(std::string(URI) == SRCML_DIFF_NS_URI) return;
            if(*revision == ORIGINAL && srcdiff_stack.top() == INSERT) return;
            if(*revision == MODIFIED && srcdiff_stack.top() == DELETE) return;

        }

        if(collect_src && localname[0] == 'e' && localname[1] == 's'
           && strcmp((const char *)localname, "escape") == 0) {

            char value = (int)strtol((const char*) attributes[0].value, NULL, 0);

            charactersUnit(&value, 1);


        }

        if(is_empty && collect_srcml) *unit->unit += ">";
        is_empty = true;

        if(collect_srcml) {

            write_startTag(localname, prefix, num_namespaces, namespaces, num_attributes, attributes);

        }

        if(terminate) stop_parser();

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
     * Overidden endRoot to indicate done with parsing and free any waiting process.
     */
    virtual void endRoot(const char * localname, const char * prefix, const char * URI) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif
        if(!read_root) {

            {
                
                boost::unique_lock<boost::mutex> lock(mutex);
                if(terminate) stop_parser();
                wait_root = false;
                cond.notify_all();
                cond.wait(lock);
                read_root = true;
        
            }

            if(terminate) {

                stop_parser();
                return;

            }

        }

        {
            boost::unique_lock<boost::mutex> lock(mutex);
            if(terminate) stop_parser();
            is_done = true;
            cond.notify_all();
        }

        if(terminate) stop_parser();

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
    virtual void endUnit(const char * localname, const char * prefix, const char * URI) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        srcdiff_stack.pop();

        if(skip) {

            get_controller().enable_startElement(true);
            get_controller().enable_charactersUnit(true);
            get_controller().enable_comment(true);
            get_controller().enable_cdataBlock(true);

        }


        //if(is_empty) *unit->unit += ">";
        if(collect_srcml) {

            write_endTag(localname, prefix, is_empty);

        }

        if(collect_srcml || collect_src) {

            // pause
            boost::unique_lock<boost::mutex> lock(mutex);
            if(terminate) stop_parser();
            cond.notify_all();
            cond.wait(lock);

        }

        is_empty = false;

        srcml_unit_free(unit);
        unit = 0;

        if(terminate) stop_parser();

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
    virtual void endElement(const char * localname, const char * prefix, const char * URI) {

#ifdef SRCSAX_DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if(!skip && URI && std::string(URI) == SRCML_DIFF_NS_URI)
            srcdiff_stack.pop();

        if(revision) {

            if(std::string(URI) == SRCML_DIFF_NS_URI) return;
            if(*revision == ORIGINAL && srcdiff_stack.top() == INSERT) return;
            if(*revision == MODIFIED && srcdiff_stack.top() == DELETE) return;

        }

        if(collect_srcml) {

            write_endTag(localname, prefix, is_empty);
        }

        is_empty = false;

        if(terminate) stop_parser();

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
    virtual void charactersUnit(const char * ch, int len) {

#ifdef SRCSAX_DEBUG
        std::string chars;
        chars.append((const char *)ch, len);
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

        if(revision) {

            if(*revision == ORIGINAL && srcdiff_stack.top() == INSERT) return;
            if(*revision == MODIFIED && srcdiff_stack.top() == DELETE) return;

        }        

        if(is_empty && collect_srcml) *unit->unit += ">";
        is_empty = false;

        if(collect_src) {

            xmlOutputBufferWrite(output_buffer, len, (const char *)ch);

        } else {

            for(int i = 0; i < len; ++i) {
                char character = (char)ch[i];

                if(character == '&')
                    (*unit->unit) += "&amp;";
                else if(character == '<')
                    (*unit->unit) += "&lt;";
                else if(character == '>')
                    (*unit->unit) += "&gt;";
                else
                    (*unit->unit) += character;
            }

        }

        if(terminate) stop_parser();

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
    virtual void metaTag(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {

        if(strcmp(localname, "macro-list") == 0) {

            std::string token("");
            std::string type("");

            for(int pos = 0; pos < num_attributes; ++pos) {

                if(strcmp(attributes[pos].localname, "token") == 0)
                    token = attributes[pos].value;
                else if(strcmp(attributes[pos].localname, "type") == 0)
                    type = attributes[pos].value;


            }

            if(token != "" && type != "") {

                archive->user_macro_list.push_back(token);
                archive->user_macro_list.push_back(type);

            }

        }

        if(!is_archive) {

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
    virtual void processingInstruction(const char * target, const char * data) {
        
        srcml_archive_set_processing_instruction(archive, (const char*)target, (const char *)data);

    }

#pragma GCC diagnostic pop

private :

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
    void write_startTag(const char * localname, const char * prefix,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {

        *unit->unit += "<";
        if(prefix) {
            *unit->unit += prefix;
            *unit->unit += ":";
        }
        *unit->unit += localname;

        for(int pos = 0; pos < num_namespaces; ++pos) {

            if(is_archive && strcmp(localname, "unit") == 0 && !is_srcml_namespace(namespaces[pos].uri, SRCML_CPP_NS_URI))
                continue;

            *unit->unit += " xmlns";
            if(namespaces[pos].prefix) {

                *unit->unit += ":";
                *unit->unit += namespaces[pos].prefix;

            }

            *unit->unit += "=\"";
            *unit->unit += namespaces[pos].uri;
            *unit->unit += "\"";

        }

        for(int pos = 0; pos < num_attributes; ++pos) {

            *unit->unit += " ";
            if(attributes[pos].prefix) {

                *unit->unit += attributes[pos].prefix;
                *unit->unit += ":";

            }
            *unit->unit += attributes[pos].localname;

            *unit->unit += "=\"";
            *unit->unit += attribute_revision(attributes[pos].value);
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
    void write_endTag(const char * localname, const char * prefix, bool is_empty) {

        if(is_empty) {

            *unit->unit += "/>";
            return;

        }

        *unit->unit += "</";
        if(prefix) {

            *unit->unit += (const char *)prefix;
            *unit->unit += ":";

        }
        *unit->unit += (const char *)localname;

        *unit->unit += ">";

    }


};



#endif
