/**
 * @file srcml_reader_handler.hpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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

    /** save meta tags to use when non-archive write unit */
    std::vector<srcml_element> * meta_tags;

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

        archive = srcml_create_archive();

        srcml_archive_disable_option(archive, SRCML_OPTION_HASH);

    }

    /**
     * ~srcml_reader_handler
     *
     * Destructor, deletes mutex and conditions.
     */
    ~srcml_reader_handler() {

        srcml_free_archive(archive);
        if(unit) srcml_free_unit(unit);

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

        srcml_archive_set_encoding(archive, encoding ? encoding : "UTF-8");

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

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if(!is_archive) srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);

        // collect attributes
        for(int pos = 0; pos < num_attributes; ++pos) {

            std::string attribute = attributes[pos].localname;
            std::string value = attributes[pos].value;

            if(attribute == "language")
                srcml_archive_set_language(archive, value.c_str());
            else if(attribute == "filename")
                srcml_archive_set_filename(archive, value.c_str());
            else if(attribute == "dir")
                srcml_archive_set_directory(archive, value.c_str());
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
                    else if(option == "TERNARY")
                        archive->options |= SRCML_OPTION_TERNARY;

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

            srcml_archive_register_namespace(archive, prefix.c_str(), uri.c_str());

        }

        // pause
        // @todo this may need to change because, meta tags have separate call now
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            if(terminate) stop_parser();
            wait_root = false;
            cond.notify_all();
            cond.wait(lock);
            read_root = true;
        }

        if(terminate) stop_parser();

#ifdef DEBUG
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

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        unit = srcml_create_unit(archive);
        unit->unit = "";

        is_empty = true;

        // collect attributes
        for(int pos = 0; pos < num_attributes; ++ipos) {

            std::string attribute = attributes[pos].localname;
            std::string value = attributes[pos].value;

            if(attribute == "timestamp")
                srcml_unit_set_timestamp(unit, value.c_str());
            else if(attribute == "hash")
                srcml_unit_set_hash(unit, value.c_str());
            else if(attribute == "language")
                srcml_unit_set_language(unit, value.c_str());
            else if(attribute == "filename")
                srcml_unit_set_filename(unit, value.c_str());
            else if(attribute == "dir")
                srcml_unit_set_directory(unit, value.c_str());
            else if(attribute == "version")
                srcml_unit_set_version(unit, value.c_str());
            else if(attribute == "tabs" || attribute == "options" || attribute == "hash")
                ;
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

                if(meta_tags->size()) {

                    *unit->unit += ">";
                    is_empty = false;

                }

                for(std::vector<srcml_element>::size_type i = 0; i < meta_tags->size(); ++i) {


                    try {

                        srcml_element & element = meta_tags->at(i);
                        write_startTag(element.localname, element.prefix, element.nb_namespaces, element.namespaces,
                                       element.nb_attributes, element.attributes);
                        write_endTag(element.localname, element.prefix, true);

                    } catch(...) { /** @todo handle */ continue; }

                }

            }

        }

        if(terminate) stop_parser();

#ifdef DEBUG
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
    virtual void startElement((const char * localname, const char * prefix, const char * URI,
                                int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                                const struct srcsax_attribute * attributes) {

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if(collect_src && localname[0] == 'e' && localname[1] == 's'
           && strcmp((const char *)localname, "escape") == 0) {

            char value = (int)strtol((const char*) attributes[3], NULL, 0);

            charactersUnit((xmlChar *)&value, 1);


        }

        if(is_empty && collect_srcml) *unit->unit += ">";
        is_empty = true;

        if(collect_srcml) {

            write_startTag(localname, prefix, nb_namespaces, namespaces, nb_attributes, attributes);

        }

        if(terminate) stop_parser();

#ifdef DEBUG
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

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        {
            boost::unique_lock<boost::mutex> lock(mutex);
            if(terminate) stop_parser();
            is_done = true;
            cond.notify_all();
        }

        if(terminate) stop_parser();

#ifdef DEBUG
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

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

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

        srcml_free_unit(unit);
        unit = 0;

        if(terminate) stop_parser();

#ifdef DEBUG
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
    virtual void endElementNs(const char * localname, const char * prefix, const char * URI) {

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if(collect_srcml) {

            write_endTag(localname, prefix, is_empty);
        }

        is_empty = false;

        if(terminate) stop_parser();

#ifdef DEBUG
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

#ifdef DEBUG
        std::string chars;
        chars.append((const char *)ch, len);
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

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

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, chars.c_str());
#endif

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
     * @param localname tag name
     * @param prefix prefix for the tag
     * @param URI uri for tag
     * @param nb_namespaces number of xml namespaces
     * @param namespaces the prefix/namespaces pairs
     * @param nb_attributes number of attributes
     * @param nb_defaulted number defaulted attributes
     * @param attributes the attributes (name/prefix/uri/value start/value end/)
     *
     * Write out the start tag to the unit string.
     */
    void write_startTag(const char * localname, const char * prefix, int nb_namespaces,
                        const char ** namespaces, int nb_attributes, const char ** attributes) {

        *unit->unit += "<";
        if(prefix) {
            *unit->unit += (const char *)prefix;
            *unit->unit += ":";
        }
        *unit->unit += (const char *)localname;

        for(int i = 0, pos = 0; i < nb_namespaces; ++i, pos += 2) {

            if(is_archive && strcmp((const char *)localname, "unit") == 0 && strcmp((const char *)namespaces[pos + 1], SRCML_CPP_NS_URI) != 0)
                continue;

            *unit->unit += " xmlns";
            if(namespaces[pos]) {

                *unit->unit += ":";
                *unit->unit += (const char *)namespaces[pos];

            }

            *unit->unit += "=\"";
            *unit->unit += (const char *)namespaces[pos + 1];
            *unit->unit += "\"";

        }


        for(int i = 0, pos = 0; i < nb_attributes; ++i, pos += 5) {

            *unit->unit += " ";
            if(attributes[pos + 1]) {

                *unit->unit += (const char *)attributes[pos + 1];
                *unit->unit += ":";

            }
            *unit->unit += (const char *)attributes[pos];

            *unit->unit += "=\"";
            unit->unit->append((const char *)attributes[pos + 3], attributes[pos + 4] - attributes[pos + 3]);
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
