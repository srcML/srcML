/**
 * @file unit_dom.hpp
 *
 * @copyright Copyright (C) 2011-2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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

#ifndef INCLUDED_UNIT_DOM_HPP
#define INCLUDED_UNIT_DOM_HPP

#include <libxml/SAX2.h>

#include <srcSAXHandler.hpp>

#include <string>
#include <vector>

/**
 * unit_dom
 *
 * Extends srcMLHandler to build a DOM for each unit separately
 * using libxml2 functions.  Inherit from to specify the start_output,
 * end_output, and apply functions.
 */
class unit_dom : public srcSAXHandler {
public :

    /**
     * unit_dom
     * @param options list of srcML options
     *
     * Constructor.
     */
    unit_dom(OPTION_TYPE options) : apply_root(isoption(options, SRCML_OPTION_APPLY_ROOT)) {}

    /**
     * ~unit_dom
     *
     * Destructor.
     */
    virtual ~unit_dom() {}

protected:

    /**
     * start_output
     *
     * Pure virtual that is called exactly once at beginnning of document  Override for intended behavior.
     */
    virtual void start_output() = 0;

    /**
     * apply
     *
     * Pure virtual that is called exactly once for each unit.  For an archive, not called on the root unit
     *
     * Formed unit combines namespaces from root and individual unit.  Full DOM of
     * individual unit is provided.  Cleanup of DOM unit is automatic.
     *
     * @returns true on success false on failure.
     */
    virtual bool apply() = 0;

    /**
     * end_output
     *
     * Pure virtual that is called exactly once at end of document.  Override for intended behavior.
     */
    virtual void end_output() = 0;

private:

    /**
     * startDocument
     *
     * SAX handler function for start of document.
     * Starts creating the document and setup output for the units.
     */
    virtual void startDocument() {

        ctxt = get_controller().getContext()->libxml2_context;

        auto state = (sax2_srcsax_handler*) ctxt->_private;
       // state->create_dom = true;

        // apparently endDocument() can be called without startDocument() for an
        // empty element
        found = true;

        // setup output
        start_output();
    }

    /**
     * processingInstruction
     * @param target the processing instruction target.
     * @param data the processing instruction data.
     *
     * A processing instruction has been parsed.
     * processing instruction in unit tree.
     */
    virtual void processingInstruction(const char* target, const char* data) {

        processing_instruction = std::pair<std::string, std::string>(target ? target : "", data ? data : "");
        //xmlSAX2ProcessingInstruction(ctxt, target, data);
    }

    /**
     * endUnit
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an unit.
     * End the construction of the unit tree, apply processing, and delete.
     */
    virtual void endUnit(const char* localname, const char* prefix, const char* URI) {

        if (!apply_root) {

            // apply the necessary processing
            if ((error = !apply()))
                stop_parser();
        }
    }

    /**
     * endRoot
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of the root element.
     * End the construction of the unit tree, apply processing, and delete.
     */
    virtual void endRoot(const char* localname, const char* prefix, const char* URI) {

        if (apply_root) {

            // apply the necessary processing
            if ((error = !apply()))
                stop_parser();
        }
    }

    /**
     * endDocument
     *
     * SAX handler function for end of document.
     */
    virtual void endDocument() {

        // endDocument can be called, even if startDocument was not for empty input
        if (!found || error)
            return;

        // end the output
        end_output();
    }

protected:

    /** we have started processing */
    bool found = false;

    /** srcML options */
    bool apply_root = false;

    /** found an error */
    bool error = false;

    /** the current parser ctxt */
    xmlParserCtxtPtr ctxt;

    /** The pre-root processing instruction */
    boost::optional<std::pair<std::string, std::string> > processing_instruction;
};

#endif
