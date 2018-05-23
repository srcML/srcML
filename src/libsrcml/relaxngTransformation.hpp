/**
 * @file relaxngTransformation.hpp
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

 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_RELAXNGTRANSFORMATION_HPP
#define INCLUDED_RELAXNGTRANSFORMATION_HPP

#include <libxml/parser.h>
#include <libxml/relaxng.h>

#include <vector>
#include <string>

#include <Transformation.hpp>

/**
 * relaxngTransformation
 *
 * Extends unit_dom to execute relaxng program and write results.
 */
class relaxngTransformation : public Transformation {
public :

    /**
     * relaxngTransformation
     * @param a_context_element an element that provides a context
     * @param options list of srcML options
     * @param stylesheet an relaxng stylesheet
     * @param params relaxng parameters
     *
     * Constructor.  Dynamically loads relaxng functions.
     */
    relaxngTransformation(/*OPTION_TYPE& options,*/ xmlDocPtr relaxng);

    /**
     * ~relaxngTransformation
     *
     * Destructor.  Closes dynamically loaded library.
     */
    virtual ~relaxngTransformation();

    /**
     * apply
     *
     * Apply relaxng program, writing results.
     * 
     * @returns true on success false on failure.
     */
    virtual xmlNodeSetPtr apply(xmlDocPtr doc, int position);

    virtual bool hasUnitWrapper() { return true; }

private :
    xmlRelaxNGValidCtxtPtr rngctx;
    xmlRelaxNGParserCtxtPtr relaxng_parser_ctxt;
    xmlRelaxNGPtr rng;
};

#endif
