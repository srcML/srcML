/**
 * @file srcMLHandler.hpp
 *
 * @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
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

#ifndef INCLUDED_SRCMLHANDLER_HPP
#define INCLUDED_SRCMLHANDLER_HPP

#include <srcMLElement.hpp>
#include <srcMLControlHandler.hpp>

#include <libxml/parser.h>

#include <vector>

/**
 * srcMLHandler
 *
 * Base class that provides hooks for SAX processing.
 */
class srcMLHandler {

private :

    srcMLControlHandler * control_handler;

protected:
    bool is_archive;
    int unit_count;

public :
  
    /**
     * srcMLHandler
     *
     * Default constructor default values to everything
     */
    srcMLHandler() : control_handler(0), is_archive(false), unit_count(0) {}

    /**
     * set_control_handler
     * @param control_handler pointer to control class 
     *
     * Used by srcMLControlHandler to provide access to self
     * for such things as disabeling sax parsing.
     */
    void set_control_handler(srcMLControlHandler * control_handler) {

        this->control_handler = control_handler;

    }

    /**
     * increment_unit_count
     *
     * Internally used to increment the count in SAX2srcMLHandler.
     */
    void increment_unit_count() {

        ++unit_count;

    }

    /**
     * get_control_handler
     *
     * Get the control handler. 
     */
    srcMLControlHandler & get_control_handler() {
 
        return *control_handler;

    }

    /**
     * stop_parser
     *
     * Stop the srcML parser.
     */
    void stop_parser() {

        control_handler->getSAX().startDocument = 0;
        control_handler->getSAX().endDocument = 0;
        control_handler->getSAX().startElementNs = 0;
        control_handler->getSAX().endElementNs = 0;
        control_handler->getSAX().characters = 0;
        control_handler->getSAX().cdataBlock = 0;
        control_handler->getSAX().comment = 0;
        control_handler->getSAX().ignorableWhitespace = 0;

	xmlStopParser(control_handler->getCtxt());

    }

    /**
     * set_is_archive
     * @param is_archive is the srcML document an archive
     *
     * Used by SAX2srcMLHandler when determined
     * if an archive.  Sets if srcML document is an archive.
     */
    void set_is_archive(bool is_archive) {

        this->is_archive = is_archive;

    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * startDocument
     *
     * SAX handler function for start of document.
     * Overide for desired behaviour.
     */
    virtual void startDocument() {}

    /**
     * endDocument
     *
     * SAX handler function for end of document.
     * Overide for desired behaviour.
     */
    virtual void endDocument() {}

    /**
     * startRoot
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param nb_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param nb_attributes the number of attributes on the tag
     * @param nb_defaulted the number of defaulted attributes
     * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
     *
     * SAX handler function for start of the root element.
     * Overide for desired behaviour.
     */
    virtual void startRoot(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                           int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                           const xmlChar ** attributes, std::vector<srcMLElement> * meta_tags) {}

    /**
     * startUnit
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
     * Overide for desired behaviour.
     */
    virtual void startUnit(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                           int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                           const xmlChar ** attributes) {}

    /**
     * startElementNs
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
     * Overide for desired behaviour.
     */
    virtual void startElementNs(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                                int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                                const xmlChar ** attributes) {}

    /**
     * endRoot
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of the root element.
     * Overide for desired behaviour.
     */
    virtual void endRoot(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {}

    /**
     * endUnit
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an unit.
     * Overide for desired behaviour.
     */
    virtual void endUnit(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {}

    /**
     * endElementNs
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an element.
     * Overide for desired behaviour.
     */
    virtual void endElementNs(const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI) {}

    /**
     * charactersRoot
     * @param ch the characers
     * @param len number of characters
     *
     * SAX handler function for character handling at the root level.
     * Overide for desired behaviour.
     */
    virtual void charactersRoot(const xmlChar * ch, int len) {}

    /**
     * charactersUnit
     * @param ch the characers
     * @param len number of characters
     *
     * SAX handler function for character handling within a unit.
     * Overide for desired behaviour.
     */
    virtual void charactersUnit(const xmlChar * ch, int len) {}

    /**
     * comment
     * @param value the comment content
     *
     * A comment has been parsed.
     * Overide for desired behaviour.
     */
    virtual void comment(const xmlChar * value) {}

    /**
     * cdataBlock
     * @param value the pcdata content
     * @param len the block length
     *
     * Called when a pcdata block has been parsed.
     * Overide for desired behaviour.
     */
    virtual void cdataBlock(const xmlChar * value, int len) {}

#pragma GCC diagnostic pop

};

#endif
