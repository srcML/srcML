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

#include <transform_units.hpp>

#ifdef _MSC_BUILD
#include <io.h>
#endif

/**
 * relaxng_units
 *
 * Extends unit_dom to execute RelaxNG grammar and write results.
 */
class relaxng_units : public transform_units {
public :

    /**
     * relaxng_units
     * @param options list of srcML options
     * @param rngctx RelaxNG context pointer
     * @param fd file descriptor in which to write
     *
     * Constructor.
     */
    relaxng_units(OPTION_TYPE options, xmlDocPtr relaxng, srcml_archive* oarchive);

    /**
     * start_output
     *
     * Pure virtual that is called exactly once at beginnning of document  Override for intended behavior.
     */
    virtual void start_output();

    /**
     * apply
     *
     * Apply RelaxNG grammar writing results.
     * 
     * @returns true on success false on failure.
     */
    virtual bool apply();

    /**
     * end_output
     *
     * Pure virtual that is called exactly once at end of document.  Override for intended behavior.
     */
    virtual void end_output();

private :

    xmlRelaxNGValidCtxtPtr rngctx;
    xmlDocPtr relaxng;
    xmlRelaxNGParserCtxtPtr relaxng_parser_ctxt;
    xmlRelaxNGPtr rng;
};

/**
 * srcml_relaxng
 * @param input_buffer a parser input buffer
 * @param relaxng xmlDocPtr containing a RelaxNG schema
 * @param fd output file descriptor
 * @param options srcml options
 *
 * RelaxNG evaluation of the nested units.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_relaxng(xmlParserInputBufferPtr input_buffer, xmlDocPtr relaxng, OPTION_TYPE options, srcml_archive* oarchive);

#endif
