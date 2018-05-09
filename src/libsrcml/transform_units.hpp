/**
 * @file transform_units.hpp
 *
 * @copyright Copyright (C) 2008-2014 srcML, LLC. (www.srcML.org)
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

/** size of string then the literal */
#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s
 /** literal followed by its size */
#define LITERALPLUSSIZE(s) s, sizeof(s) - 1

#include <srcexfun.hpp>

#include <unit_dom.hpp>

#ifdef _MSC_BUILD
#include <io.h>
#endif

/**
 * transform_units
 *
 * Extends unit_dom to execute RelaxNG grammar and write results.
 */
class transform_units : public unit_dom {
public :

    /**
     * transform_units
     * @param options list of srcML options
     * @param rngctx RelaxNG context pointer
     * @param oarchive srcML archive to write contents to
     *
     * Constructor.
     */
    transform_units(OPTION_TYPE options, srcml_archive* oarchive)
        : unit_dom(options), oarchive(oarchive) {
    }

    /**
     * ~transform_units
     *
     * Destructor.
     */
    virtual ~transform_units() {}

    virtual void outputResult(xmlNodePtr a_node) {

        bool is_archive = (oarchive->options & SRCML_OPTION_ARCHIVE) > 0;

        // remove src namespace
        xmlNsPtr hrefptr = xmlSearchNsByHref(a_node->doc, a_node, BAD_CAST SRCML_CPP_NS_URI);

        xmlNsPtr save = a_node->nsDef; //skip = is_archive ? xmlRemoveNs(a_node, hrefptr) : 0;
        xmlNsPtr nextsave = hrefptr ? hrefptr->next : 0;
        if (is_archive) {
        	if (!hrefptr)
		        a_node->nsDef = 0;
		    else {
		    	a_node->nsDef = hrefptr;
		    	hrefptr->next = 0;
		    }
        }

        oarchive->translator->add_unit(a_node, ctxt->myDoc);

        if (save)
	        a_node->nsDef = save;

	    if (nextsave)
	    	hrefptr->next = nextsave;
    }

    // removes the namespace from the element
    xmlNsPtr* xmlRemoveNs(xmlNodePtr a_node, xmlNsPtr hrefptr) {

        if (!hrefptr)
            return 0;

        xmlNsPtr* skip = 0;
        for (xmlNsPtr* pns = &a_node->nsDef; pns; pns = &((*pns)->next)) {
            if ((*pns) == hrefptr) {
                skip = pns;
                *skip = (*skip)->next;
                break;
            }
        }

        return skip;
    }

public :

    srcml_archive* oarchive;

};

#endif
