/**
 * @file transform_units.hpp
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

#ifndef INCLUDED_TRANSFORM_UNITS_HPP
#define INCLUDED_TRANSFORM_UNITS_HPP

#include <libxml/parser.h>
#include <libxml/SAX2.h>

#include <srcSAXHandler.hpp>
#include <srcexfun.hpp>
#include <srcml_translator.hpp>

#include <string>
#include <vector>

template<typename T>
class save_restore {
public:
    save_restore(T& item) : saved_item(&item), saved_value(item) {}
    save_restore(bool save, T& item) {
        saved_item = save ? &item : 0;
        if (save)
            saved_value = item;
    }
    ~save_restore() { if (saved_item) *saved_item = saved_value; }
    operator T() { return saved_value; }
private:
    T* saved_item;
    T saved_value;
};

/**
 * transform_units
 *
 * Extends srcMLHandler to build a DOM for each unit separately
 * using libxml2 functions.  Inherit from to specify the start_output,
 * end_output, and apply functions.
 */
class transform_units : public srcSAXHandler {
public :

    /**
     * transform_units
     * @param options list of srcML options
     *
     * Constructor.
     */
    transform_units(OPTION_TYPE options, srcml_archive* oarchive)
        : apply_root(isoption(options, SRCML_OPTION_APPLY_ROOT)), oarchive(oarchive) {}

    /**
     * ~transform_units
     *
     * Destructor.
     */
    virtual ~transform_units() {}

protected:

    /**
     * start_output
     *
     * Pure virtual that is called exactly once at beginnning of document  Override for intended behavior.
     */
    virtual void start_output() {}

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
    virtual bool apply() { return true; }

    /**
     * end_output
     *
     * Pure virtual that is called exactly once at end of document.  Override for intended behavior.
     */
    virtual void end_output() {}

private:

    /**
     * startDocument
     *
     * SAX handler function for start of document.
     * Starts creating the document and setup output for the units.
     */
    virtual void startDocument() {

        ctxt = get_controller().getContext()->libxml2_context;

        // apparently endDocument() can be called without startDocument() for an
        // empty element
        found = true;

        // setup output
        start_output();
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
    virtual void endUnit(const char* /* localname */, const char* /* prefix */, const char* /* URI */) {

        if (!apply_root) {

            auto state = (sax2_srcsax_handler*) ctxt->_private;

            doc = xmlReadMemory(state->unitsrcml.c_str(), (int) state->unitsrcml.size(), 0, 0, 0);

            // apply the necessary processing
            if ((error = !apply()))
                stop_parser();

            xmlFreeDoc(doc);
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
    virtual void endRoot(const char* /* localname */, const char* /* prefix */, const char* /* URI */) {

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

    void outputResult(xmlNodePtr a_node) {

        bool is_archive = (oarchive->options & SRCML_OPTION_ARCHIVE) > 0;

        // remove src namespace
        xmlNsPtr hrefptr = xmlSearchNsByHref(a_node->doc, a_node, BAD_CAST SRCML_CPP_NS_URI);

        save_restore<xmlNsPtr> save(a_node->nsDef); //skip = is_archive ? xmlRemoveNs(a_node, hrefptr) : 0;
        save_restore<xmlNsPtr> nextsave(hrefptr, hrefptr->next);
        if (is_archive) {
            if (!hrefptr)
                a_node->nsDef = 0;
            else {
                a_node->nsDef = hrefptr;
                hrefptr->next = 0;
            }
        }

        oarchive->translator->add_unit(a_node, doc);
/*        
        // remove src namespace, needed for performing XSLT transformation, but not
        // needed for output of an embedded unit in an archive
        if ((oarchive->options & SRCML_OPTION_ARCHIVE) > 0) {
            // remove the first namespace, which is the srcML one
            // @todo Generalize this
            xmlNsPtr save = a_node->nsDef;
            a_node->nsDef = a_node->nsDef->next;
            xmlFreeNs(save);
        }

        oarchive->translator->add_unit(a_node, doc);
*/
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

    xmlDocPtr doc;

    srcml_archive* oarchive;
};

#endif
