/**
 * @file relaxng_units.hpp
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

#ifndef INCLUDED_RELAXNG_UNITS_HPP
#define INCLUDED_RELAXNG_UNITS_HPP

#include <libxml/parser.h>
#include <libxml/relaxng.h>

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
 * relaxng_units
 *
 * Extends unit_dom to execute RelaxNG grammar and write results.
 */
class relaxng_units : public unit_dom {
public :

    /**
     * relaxng_units
     * @param options list of srcML options
     * @param rngctx RelaxNG context pointer
     * @param fd file descriptor in which to write
     *
     * Constructor.
     */
    relaxng_units(OPTION_TYPE options, xmlRelaxNGValidCtxtPtr rngctx, srcml_archive* oarchive)
        : unit_dom(options), rngctx(rngctx), oarchive(oarchive) {
    }

    /**
     * ~relaxng_units
     *
     * Destructor.
     */
    virtual ~relaxng_units() {}

    /**
     * start_output
     *
     * Create output buffer.
     */
    virtual void start_output() {}

    /**
     * apply
     *
     * Apply RelaxNG grammar writing results.
     * 
     * @returns true on success false on failure.
     */
    virtual bool apply() {

        // validate
        int n = xmlRelaxNGValidateDoc(rngctx, ctxt->myDoc);
        if (n != 0)
            return true;

        // output if it validates

        // get the root node of current unit
        xmlNodePtr node = xmlDocGetRootElement(ctxt->myDoc);
        if (!node)
            return true;

        outputResult(node);

        return true;
    }

    virtual void outputResult(xmlNodePtr a_node) {

        static xmlBufferPtr lbuffer = xmlBufferCreate();
        int size = xmlNodeDump(lbuffer, ctxt->myDoc, a_node, 0, 0);
        if (size == 0)
            return;

        oarchive->translator->add_unit_raw((const char*) xmlBufferContent(lbuffer), size);

        xmlBufferEmpty(lbuffer);
    }

    /**
     * end_output
     *
     * Finish the archive and close buffer.
     */
    virtual void end_output() {}

private :

    xmlRelaxNGValidCtxtPtr rngctx;
    srcml_archive* oarchive;

};

#endif
