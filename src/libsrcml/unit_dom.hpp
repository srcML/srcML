/**
 * @file unit_dom.cpp
 *
 * @copyright Copyright (C) 2011-2014 SDML (www.srcML.org)
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

#include <srcMLHandler.hpp>

#include <string>
#include <vector>

/**
 * unit_dom
 *
 * Extends srcMLHandler to build a DOM for each unit separately
 * using libxml2 functions.  Inherit from to specify the start_output,
 * end_output, and apply functions.
 */
class unit_dom : public srcMLHandler {
public :

    /**
     * unit_dom
     * @param options list of srcML options
     *
     * Constructor.
     */
    unit_dom(OPTION_TYPE options) : rootsize(0), found(false), options(options), error(false) {}

    /**
     * ~unit_dom
     *
     * Destructor.
     */
    virtual ~unit_dom() {}

    /**
     * get_options
     *
     * Get method providing access to options.
     *
     * @returns the srcML options
     */
    virtual OPTION_TYPE get_options() const { return options; }

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
     * @returns true on success 0 on failure.
     */
    virtual bool apply() = 0;

    /**
     * end_output
     *
     * Pure virtual that is called exactly once at end of document.  Override for intended behavior.
     */
    virtual void end_output() = 0;

    /**
     * startDocument
     *
     * SAX handler function for start of document.
     * Starts creating the document and setup output for the units.
     */
    virtual void startDocument() {

        ctxt = get_control_handler().getCtxt();

        // apparently endDocument() can be called without startDocument() for an
        // empty element
        found = true;

        // setup output
        start_output();

        xmlSAX2StartDocument(ctxt);

    }

 
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
     * @param meta_tags vector of elements composed of metage tags defined after root tag
     *
     * SAX handler function for start of the root element.
     * Collect namespaces from root unit.  Start to build the tree if OPTION_APPLY_ROOT.
     */
    virtual void startRoot(const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                           int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                           const xmlChar** attributes, std::vector<srcMLElement> * meta_tags) {

        SAX2srcMLHandler* handler = (SAX2srcMLHandler*)get_control_handler().getCtxt()->_private;
        root = &handler->root;
        this->meta_tags = meta_tags;

        // record namespaces in an extensible list so we can add the per unit
        for (int i = 0; i < nb_namespaces; ++i) {

            data.push_back(namespaces[i * 2]);
            data.push_back(namespaces[i * 2 + 1]);

        }
        rootsize = data.size();

        // if we are building the entire tree, start now
        if (isoption(options, OPTION_APPLY_ROOT)) {

            xmlSAX2StartElementNs(ctxt, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes,
                                  nb_defaulted, attributes);

        }

    }

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
     * Start to create an individual unit, merging namespace details from the root (if it exists).
     */
    virtual void startUnit(const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                           int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                           const xmlChar** attributes) {

        // remove per-unit namespaces
        data.resize(rootsize);

        // combine namespaces from root and local to this unit
        for (int i = 0; i < nb_namespaces; ++i) {

            // make sure not already in
            bool found = false;
            for (std::vector<const xmlChar*>::size_type j = 0; j < data.size() / 2; ++j)
                if (xmlStrEqual(data[j * 2], namespaces[i * 2]) &&
                    xmlStrEqual(data[j * 2 + 1], namespaces[i * 2 + 1])) {
                    found = true;
                    break;
                }

            if (found)
                continue;

            data.push_back(namespaces[i * 2]);
            data.push_back(namespaces[i * 2 + 1]);
        }

        /*

          This should not be needed since start root should always be called.
          // if applying to entire archive, then just build this node
          if (isoption(options, OPTION_APPLY_ROOT)) {

          // if apply root and not archive then startRootUnit may not have been called
          static bool started = false;
          if(!is_archive && !started) xmlSAX2StartDocument(ctxt);
          started = true;
          xmlSAX2StartElementNs(ctxt, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes,
          nb_defaulted, attributes);

          return;
          }
        */

        // start the document for this unit
        //xmlSAX2StartDocument(ctxt);

        // start the unit (element) at the root using the merged namespaces
        xmlSAX2StartElementNs(ctxt, localname, prefix, URI, (int)(data.size() / 2),
                              &data[0], nb_attributes, nb_defaulted, attributes);

    }

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
     * Build start element nodes in unit tree.
     */
    virtual void startElementNs(const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                const xmlChar** attributes) {

        xmlSAX2StartElementNs(ctxt, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes);
    }

    /**
     * endElementNs
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an element.
     * Build end element nodes in unit tree.
     */
    virtual void endElementNs(const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

        xmlSAX2EndElementNs(ctxt, localname, prefix, URI);
    }

    /**
     * charactersUnit
     * @param ch the characers
     * @param len number of characters
     *
     * SAX handler function for character handling within a unit.
     * Characters in unit tree.
     */
    virtual void charactersUnit(const xmlChar* ch, int len) {

        xmlSAX2Characters(ctxt, ch, len);
    }

    /**
     * charactersRoot
     * @param ch the characers
     * @param len number of characters
     *
     * SAX handler function for character handling at the root level.
     * Characters in unit tree.
     */
    virtual void charactersRoot(const xmlChar* ch, int len) {

        if(isoption(options, OPTION_APPLY_ROOT))
            xmlSAX2Characters(ctxt, ch, len);
    }

    /**
     * cdataBlock
     * @param value the pcdata content
     * @param len the block length
     *
     * Called when a pcdata block has been parsed.
     * CDATA block in unit tree.
     */
    virtual void cdatablock(const xmlChar* ch, int len) {

        xmlSAX2CDataBlock(ctxt, ch, len);
    }

     /**
     * comment
     * @param value the comment content
     *
     * A comment has been parsed.
     * Comments in unit tree.
     */
    virtual void comments(const xmlChar* ch) {

        xmlSAX2Comment(ctxt, ch);
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
    virtual void endUnit(const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

        // finish building the unit tree
        xmlSAX2EndElementNs(ctxt, localname, prefix, URI);

        // End the document and free it if applied to unit individually
        if(!isoption(options, OPTION_APPLY_ROOT)) {
            xmlSAX2EndDocument(ctxt);

            // apply the necessary processing
            if ((error = !apply()))
                stop_parser();

            // free up the document that has this particular unit
            xmlNodePtr aroot = ctxt->myDoc->children;
            xmlUnlinkNode(ctxt->myDoc->children);
            xmlFreeNodeList(aroot);
            ctxt->myDoc->children = 0;

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
    virtual void endRoot(const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

        if(isoption(options, OPTION_APPLY_ROOT)) {

            // finish building the unit tree
            xmlSAX2EndElementNs(ctxt, localname, prefix, URI);

            // End the document and free it if applied to unit individually
            xmlSAX2EndDocument(ctxt);

            // apply the necessary processing
            if ((error = !apply()))
                stop_parser();

            // free up the document that has this particular unit
            xmlNodePtr aroot = ctxt->myDoc->children;
            xmlUnlinkNode(ctxt->myDoc->children);
            xmlFreeNodeList(aroot);
            ctxt->myDoc->children = 0;

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
        /*
        // end the entire input document and run apply if applied to root.
        if (isoption(options, OPTION_APPLY_ROOT)) {
        xmlSAX2EndDocument(ctxt);

        if ((error = !apply()))
        stop_parser();

        xmlNodePtr onode = xmlDocGetRootElement(ctxt->myDoc);
        onode->name = NULL;

        }*/

        // free up the document that has this particular unit
        xmlFreeDoc(ctxt->myDoc);
        ctxt->myDoc = 0;

        // end the output
        end_output();

    }

protected:

    /** Root namespaces */
    std::vector<const xmlChar*> data;

    /** Size of data */
    std::vector<const xmlChar*>::size_type rootsize;

    /* we have started processing */
    bool found;

    /** srcML options */
    OPTION_TYPE options;

    /** found an error */
    bool error;

    /** the current parser ctxt */
    xmlParserCtxtPtr ctxt;

    /**  The root element */
    srcMLElement * root;

    /**  The meta tags for the root element */
    std::vector<srcMLElement> * meta_tags;

};

#endif
