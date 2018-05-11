/**
 * @file relaxng_units.cpp
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

#include <libxml/parser.h>
#include <libxml/relaxng.h>

#include <transform_units.hpp>
#include <relaxng_units.hpp>

#ifdef _MSC_BUILD
#include <io.h>
#endif

/**
 * relaxng_units
 * @param options list of srcML options
 * @param rngctx RelaxNG context pointer
 * @param fd file descriptor in which to write
 *
 * Constructor.
 */
relaxng_units::relaxng_units(OPTION_TYPE options, xmlDocPtr relaxng, srcml_archive* oarchive)
    : transform_units(options, oarchive), relaxng(relaxng) {
}

/**
 * start_output
 *
 * Pure virtual that is called exactly once at beginnning of document  Override for intended behavior.
 */
void relaxng_units::start_output() {

    relaxng_parser_ctxt = xmlRelaxNGNewDocParserCtxt(relaxng);
    rng = xmlRelaxNGParse(relaxng_parser_ctxt);
    rngctx = xmlRelaxNGNewValidCtxt(rng);
}

/**
 * apply
 *
 * Apply RelaxNG grammar writing results.
 * 
 * @returns true on success false on failure.
 */
bool relaxng_units::apply() {

    // validate
    int n = xmlRelaxNGValidateDoc(rngctx, doc);
    if (n != 0)
        return true;

    // output if it validates

    // get the root node of current unit
    xmlNodePtr node = xmlDocGetRootElement(doc);
    if (!node)
        return true;

    outputResult(node);

    return true;
}

/**
 * end_output
 *
 * Pure virtual that is called exactly once at end of document.  Override for intended behavior.
 */
void relaxng_units::end_output() {

    xmlRelaxNGFreeValidCtxt(rngctx);
    xmlRelaxNGFree(rng);
    xmlRelaxNGFreeParserCtxt(relaxng_parser_ctxt);
}

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
int srcml_relaxng(xmlParserInputBufferPtr input_buffer, xmlDocPtr relaxng, OPTION_TYPE options, srcml_archive* oarchive) {

    if (input_buffer == NULL || relaxng == NULL)
        return SRCML_STATUS_INVALID_ARGUMENT;

    relaxng_units process(options, relaxng, oarchive);
    srcSAXController control(input_buffer);

    try {

        control.parse(&process);

    } catch(SAXError error) {

        fprintf(stderr, "Error Parsing: %s\n", error.message.c_str());
    }

    return 0;
}
