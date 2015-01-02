/**
 * @file srcSAXHandler.hpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * srcSAX is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * srcSAX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_SRCSAX_HANDLER_HPP
#define INCLUDED_SRCSAX_HANDLER_HPP

#include <srcSAXController.hpp>

#include <libxml/parser.h>

#include <vector>

/**
 * srcSAXHandler
 *
 * Base class that provides hooks for SAX processing.
 */
class srcSAXHandler {

private :

    /** Controller for parser */
    srcSAXController * controller;

protected:

    /** is the document an archive */
    bool is_archive;

    /** the current unit count */
    int unit_count;

    /** open srcML element stack */
    std::vector<std::string> srcml_element_stack;

    /** the xml documents encoding */
    const char * encoding;

public :

    /**
     * srcSAXHandler
     *
     * Default constructor default values to everything
     */
    srcSAXHandler() : controller(0), is_archive(false), unit_count(0), encoding(0) {}

    /**
     * set_controller
     * @param controller pointer to control class
     *
     * Used by srcSAXController to provide access to self
     * for such things as disabeling sax parsing.
     */
    void set_controller(srcSAXController * controller) {

        this->controller = controller;

    }

    /**
     * increment_unit_count
     *
     * Internally used to increment the count in SAX2srcSAXHandler.
     */
    void increment_unit_count() {

        ++unit_count;

    }

    /**
     * get_stack
     *
     * Used internally to update the stack.
     */
    std::vector<std::string> & get_stack() {

        return srcml_element_stack;

    }

    /**
     * get_controller
     *
     * Get the control handler.
     */
    srcSAXController & get_controller() {

        return *controller;

    }

    /**
     * stop_parser
     *
     * Stop the srcML parser.
     */
    void stop_parser() {

        srcsax_stop_parser(controller->getContext());

    } 

    /**
     * set_encoding
     * @param encoding set the encoding
     *
     * Used by SAX2srcSAXHandler when determined
     * encoding.  Set the input encoding if any.
     */
    void set_encoding(const char * encoding) {

        this->encoding = encoding;
    }

    /**
     * set_is_archive
     * @param is_archive is the srcML document an archive
     *
     * Used by SAX2srcSAXHandler when determined
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
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * SAX handler function for start of the root element.
     * Overide for desired behaviour.
     */
    virtual void startRoot(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {}

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
     * SAX handler function for start of an unit.
     * Overide for desired behaviour.
     */
    virtual void startUnit(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {}
#if 0
    /**
     * startFunction
     * @param name the function's name
     * @param return_type the function return type
     * @param parameter_list a list of the function parameters in struct containing (declaration.type/declaration.name)
     * @param is_decl indicates if the call is a function declaration (true) or definition (false)
     *
     * SAX handler function for start of function with prototype.
     * Accessing references after callback termination is undefined.
     *
     * Overide for desired behaviour.
     */
    virtual void startFunction(const std::string & name, const std::string & return_type, const std::vector<declaration> & parameter_list, bool is_decl) {}
#endif
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
     * SAX handler function for start of an element.
     * Overide for desired behaviour.
     */
    virtual void startElement(const char * localname, const char * prefix, const char * URI,
                                int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                                const struct srcsax_attribute * attributes) {}

    /**
     * endRoot
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of the root element.
     * Overide for desired behaviour.
     */
    virtual void endRoot(const char * localname, const char * prefix, const char * URI) {}

    /**
     * endUnit
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an unit.
     * Overide for desired behaviour.
     */
    virtual void endUnit(const char * localname, const char * prefix, const char * URI) {}
#if 0
    /**
     * endFunction
     *
     * SAX handler function for end of a function.
     * Overide for desired behaviour.
     */
    virtual void endFunction() {}
#endif
    /**
     * endElement
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an element.
     * Overide for desired behaviour.
     */
    virtual void endElement(const char * localname, const char * prefix, const char * URI) {}

    /**
     * charactersRoot
     * @param ch the characers
     * @param len number of characters
     *
     * SAX handler function for character handling at the root level.
     * Overide for desired behaviour.
     */
    virtual void charactersRoot(const char * ch, int len) {}

    /**
     * charactersUnit
     * @param ch the characers
     * @param len number of characters
     *
     * SAX handler function for character handling within a unit.
     * Overide for desired behaviour.
     */
    virtual void charactersUnit(const char * ch, int len) {}

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
                           const struct srcsax_attribute * attributes) {}

    /**
     * comment
     * @param value the comment content
     *
     * A comment has been parsed.
     * Overide for desired behaviour.
     */
    virtual void comment(const char * value) {}

    /**
     * cdataBlock
     * @param value the pcdata content
     * @param len the block length
     *
     * Called when a pcdata block has been parsed.
     * Overide for desired behaviour.
     */
    virtual void cdataBlock(const char * value, int len) {}

    /**
     * processingInstruction
     * @param target the processing instruction target.
     * @param data the processing instruction data.
     *
     * Called when a processing instruction has been parsed.
     * Overide for desired behaviour.
     */
    virtual void processingInstruction(const char * target, const char * data) {}

#pragma GCC diagnostic pop

};

#endif
