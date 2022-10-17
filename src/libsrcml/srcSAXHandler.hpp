// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcSAXHandler.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_SRCSAXHANDLER_HPP
#define INCLUDED_SRCSAXHANDLER_HPP

#include <srcSAXController.hpp>

/**
 * srcSAXHandler
 *
 * Base class that provides hooks for SAX processing.
 */
class srcSAXHandler {

private :

    /** Controller for parser */
    srcSAXController* controller = nullptr;

protected:

    /** is the document an archive */
    bool is_archive = false;

    /** the current unit count */
    int unit_count = 0;

    /** the xml documents encoding */
    const char* encoding = 0;

public :

    /**
     * set_controller
     * @param controller pointer to control class
     *
     * Used by srcSAXController to provide access to self
     * for such things as disabling sax parsing.
     */
    void set_controller(srcSAXController* newController) {

        controller = newController;
    }

    /**
     * get_controller
     *
     * Get the control handler.
     */
    srcSAXController& get_controller() {

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
    void set_encoding(const char* newEncoding) {

        encoding = newEncoding;
    }

    /**
     * set_is_archive
     * @param is_archive is the srcML document an archive
     *
     * Used by SAX2srcSAXHandler when determined
     * if an archive.  Sets if srcML document is an archive.
     */
    void set_is_archive(bool newIsArchive) {

        is_archive = newIsArchive;
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
    virtual void startRoot(const char* /* localname */, const char* /* prefix */, const char* /* URI */,
                           int /* num_namespaces */, const xmlChar** /* namespaces */, int /* num_attributes */,
                           const xmlChar** /* attributes */) {}

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
    virtual void startUnit(const char* /* localname */, const char* /* prefix */, const char* /* URI */,
                           int /* num_namespaces */, const xmlChar** /* namespaces */, int /* num_attributes */,
                           const xmlChar** /* attributes */) {}
    /**
     * endRoot
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of the root element.
     * Overide for desired behaviour.
     */
    virtual void endRoot(const char* /* localname */, const char* /* prefix */, const char* /* URI */) {}

    /**
     * endUnit
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an unit.
     * Overide for desired behaviour.
     */
    virtual void endUnit(const char* /* localname */, const char* /* prefix */, const char* /* URI */) {}

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
    virtual void metaTag(const char* /* localname */, const char* /* prefix */, const char* /* URI */,
                           int /* num_namespaces */, const xmlChar** /* namespaces */, int /* num_attributes */,
                           const xmlChar** /* attributes */) {}

    /**
     * processingInstruction
     * @param target the processing instruction target.
     * @param data the processing instruction data.
     *
     * Called when a processing instruction has been parsed.
     * Overide for desired behaviour.
     */
    virtual void processingInstruction(const char* /* target */, const char* /* data */) {}

    /*
    * destructor
    */
    virtual ~srcSAXHandler() = default;

#pragma GCC diagnostic pop

};

#endif
