#ifndef INCLUDED_SRCML_READER_HANDLER_HPP
#define INCLUDED_SRCML_READER_HANDLER_HPP

#include <srcMLHandler.hpp>
#include <srcml_types.hpp>

#include <libxml/parser.h>
#include <stdio.h>
#include <Options.hpp>
#include <srcmlns.hpp>
#include <srcml.h>

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
 * srcMLReaderHandler
 *
 * Inherits from srcMLHandler to provide hooks into
 * SAX2 parsing. Provides starting and stoping using
 * threads.  Collects attributes, namespaces and srcML
 * from units.
 */
class srcMLReaderHandler : public srcMLHandler {

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
    std::vector<srcMLElement> * meta_tags;

public :

    /** Give access to membeers for srcMLSAX2Reader class */
    friend class srcMLSAX2Reader;

    /**
     * srcMLReaderHandler
     *
     * Constructor.  Sets up mutex, conditions and state.
     */
  srcMLReaderHandler() : unit(0), output_buffer(0), is_done(false), read_root(false),
			 collect_unit_attributes(false), collect_srcml(false), collect_src(false),
			 terminate(false), is_empty(false), wait_root(true), skip(false) {

        archive = srcml_create_archive();
        archive->prefixes.clear();
        archive->namespaces.clear();

    }

    /**
     * ~srcMLReaderHandler
     *
     * Destructor, deletes mutex and conditions.
     */
    ~srcMLReaderHandler() {}

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
     * startRoot
     * @param localname tag name
     * @param prefix prefix for the tag
     * @param URI uri for tag
     * @param nb_namespaces number of xml namespaces
     * @param namespaces the prefix/namespaces pairs
     * @param nb_attributes number of attributes
     * @param nb_defaulted number defaulted attributes
     * @param attributes the attributes (name/prefix/uri/value start/value end/)
     *
     * Overidden startRoot to handle collection of root attributes. Stop before continue
     */
    virtual void startRoot(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                           int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                           const xmlChar ** attributes, std::vector<srcMLElement> * meta_tags) {

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        if(!is_archive) srcml_archive_disable_option(archive, SRCML_OPTION_ARCHIVE);

        // collect attributes
        for(int i = 0, pos = 0; i < nb_attributes; ++i, pos += 5) {

            std::string attribute = (const char *)attributes[pos];
            std::string value = "";
            value.append((const char *)attributes[pos + 3], attributes[pos + 4] - attributes[pos + 3]);

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
                        archive->options |= OPTION_XMLDECL;
                    if(option == "NAMESPACEDECL")
                        archive->options |= OPTION_NAMESPACEDECL;
                    if(option == "CPP_TEXT_ELSE")
                        archive->options |= OPTION_CPP_TEXT_ELSE;
                    if(option == "CPP_MARKUP_IF0")
                        archive->options |= OPTION_CPP_MARKUP_IF0;
                    if(option == "EXPRESSION")
                        archive->options |= OPTION_EXPRESSION;
                    if(option == "NAMESPACE")
                        archive->options |= OPTION_NAMESPACE;
                    if(option == "LINE")
                        archive->options |= OPTION_LINE;
                    if(option == "MACRO_PATTERN")
                        archive->options |= OPTION_MACRO_PATTERN;
                    if(option == "MACRO_LIST")
                        archive->options |= OPTION_MACRO_LIST;
                    if(option == "NESTIF")
                        archive->options |= OPTION_NESTIF;
                    if(option == "CPPIF_CHECK")
                        archive->options |= OPTION_CPPIF_CHECK;

                }

            } else {
                archive->attributes.push_back(attribute);
                archive->attributes.push_back(value);
            }

        }

        // collect namespaces
        for(int i = 0, pos = 0; i < nb_namespaces; ++i, pos += 2) {

            std::string prefix = namespaces[pos] ? (const char *)namespaces[pos] : "";
            std::string ns = namespaces[pos + 1] ? (const char *)namespaces[pos + 1] : "";

            if(ns == SRCML_CPP_NS_URI) {

                if(archive->language != 0) {

                    if(*archive->language == "C++" || *archive->language == "C")
                        archive->options |= SRCML_OPTION_CPP | SRCML_OPTION_CPP_NOMACRO;
                    else if(*archive->language == "C#")
                        archive->options |= SRCML_OPTION_CPP_NOMACRO;
                    //else
                    //options |= SRCML_OPTION_CPP;
                }

            } else if(ns == SRCML_ERR_NS_URI)
                archive->options |= SRCML_OPTION_DEBUG;
            else if(ns == SRCML_EXT_LITERAL_NS_URI)
                archive->options |= SRCML_OPTION_LITERAL;
            else if(ns == SRCML_EXT_OPERATOR_NS_URI)
                archive->options |= SRCML_OPTION_OPERATOR;
            else if(ns == SRCML_EXT_MODIFIER_NS_URI)
                archive->options |= SRCML_OPTION_MODIFIER;
            else if(ns == SRCML_EXT_POSITION_NS_URI)
                archive->options |= SRCML_OPTION_POSITION;

            std::vector<std::string>::size_type index;
            try {

                for(index = 0; index < archive->prefixes.size(); ++index)

                    if(archive->namespaces.at(index) == ns) {

                        archive->prefixes.at(index) = prefix;
                        break;
                    }

            } catch(...) {}

            if(index == archive->prefixes.size()) {
                archive->prefixes.push_back(prefix);
                archive->namespaces.push_back(ns);
            }


        }

        this->meta_tags = meta_tags;

        // collect meta_data from tags
        for(std::vector<srcMLElement>::size_type i = 0; i < meta_tags->size(); ++i) {

            try {

                srcMLElement & element = meta_tags->at(i);

                std::string token;
                std::string type;
                for(int i = 0, pos = 0; i < element.nb_attributes; ++i, pos += 5) {

                    std::string attribute = (const char *)element.attributes[pos];
                    std::string value = "";
                    value.append((const char *)element.attributes[pos + 3], element.attributes[pos + 4] - element.attributes[pos + 3]);

                    if(attribute == "token")
                        token = value;
                    else if(attribute == "type")
                        type = value;

                }

                if(token != "" && type != "") {

                    archive->user_macro_list.push_back(token);
                    archive->user_macro_list.push_back(type);

                }

            } catch(...) { /* @todo actually quit */continue; }


        }

        // pause
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
     * @param localname tag name
     * @param prefix prefix for the tag
     * @param URI uri for tag
     * @param nb_namespaces number of xml namespaces
     * @param namespaces the prefix/namespaces pairs
     * @param nb_attributes number of attributes
     * @param nb_defaulted number defaulted attributes
     * @param attributes the attributes (name/prefix/uri/value start/value end/)
     *
     * Overidden startUnit to handle collection of Unit attributes and tag. Stop before continue
     * if collecting attributes.
     */
    virtual void startUnit(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                           int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                           const xmlChar ** attributes) {

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

        unit = srcml_create_unit(archive);
        unit->unit = "";

        is_empty = true;

        // collect attributes
        for(int i = 0, pos = 0; i < nb_attributes; ++i, pos += 5) {

            std::string attribute = (const char *)attributes[pos];
            std::string value = "";
            value.append((const char *)attributes[pos + 3], attributes[pos + 4] - attributes[pos + 3]);

            if(attribute == "language")
                srcml_unit_set_language(unit, value.c_str());
            else if(attribute == "filename")
                srcml_unit_set_filename(unit, value.c_str());
            else if(attribute == "dir")
                srcml_unit_set_directory(unit, value.c_str());
            else if(attribute == "version")
                srcml_unit_set_version(unit, value.c_str());

        }

        if(collect_unit_attributes) {

            // pause
            boost::unique_lock<boost::mutex> lock(mutex);
            if(terminate) stop_parser();
            cond.notify_all();
            cond.wait(lock);

        }

        if(collect_srcml) {

            write_startTag(localname, prefix, nb_namespaces, namespaces, nb_attributes, attributes);

            if(!is_archive) {

                if(meta_tags->size()) {

                    *unit->unit += ">";
                    is_empty = false;

                }

                for(std::vector<srcMLElement>::size_type i = 0; i < meta_tags->size(); ++i) {


                    try {

                        srcMLElement & element = meta_tags->at(i);
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
     * startElementNs
     * @param localname tag name
     * @param prefix prefix for the tag
     * @param URI uri for tag
     * @param nb_namespaces number of xml namespaces
     * @param namespaces the prefix/namespaces pairs
     * @param nb_attributes number of attributes
     * @param nb_defaulted number defaulted attributes
     * @param attributes the attributes (name/prefix/uri/value start/value end/)
     *
     * Overidden startElementNs to handle collection of srcML elements.
     */
    virtual void startElementNs(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                                int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                                const xmlChar ** attributes) {

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

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
    virtual void endRoot(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {

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
    virtual void endUnit(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {

#ifdef DEBUG
        fprintf(stderr, "HERE: %s %s %d '%s'\n", __FILE__, __FUNCTION__, __LINE__, (const char *)localname);
#endif

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
    virtual void endElementNs(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {

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
    virtual void charactersUnit(const xmlChar * ch, int len) {

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
    void write_startTag(const xmlChar * localname, const xmlChar * prefix, int nb_namespaces,
                        const xmlChar ** namespaces, int nb_attributes, const xmlChar ** attributes) {

        *unit->unit += "<";
        if(prefix) {
            *unit->unit += (const char *)prefix;
            *unit->unit += ":";
        }
        *unit->unit += (const char *)localname;

        for(int i = 0, pos = 0; i < nb_namespaces; ++i, pos += 2) {

            if(is_archive && strcmp((const char *)namespaces[pos + 1], SRCML_CPP_NS_URI) != 0)
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
    void write_endTag(const xmlChar * localname, const xmlChar * prefix, bool is_empty) {

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
