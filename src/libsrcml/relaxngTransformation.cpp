/**
 * @file relaxngTransformation.cpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <relaxngTransformation.hpp>

/**
 * relaxngTransformation
 * @param options list of srcML options
 * @param stylesheet an relaxng stylesheet
 * @param params relaxng parameters
 *
 * Constructor.
 */
relaxngTransformation::relaxngTransformation(/* OPTION_TYPE& options, */ xmlDocPtr relaxng) {

    relaxng_parser_ctxt = decltype(relaxng_parser_ctxt)(xmlRelaxNGNewDocParserCtxt(relaxng));
    if (relaxng_parser_ctxt == nullptr)
        throw;

    rng = decltype(rng)(xmlRelaxNGParse(relaxng_parser_ctxt.get()));
    if (rng == nullptr)
        throw;

    rngctx = decltype(rngctx)(xmlRelaxNGNewValidCtxt(rng.get()));
    if (rngctx == nullptr)
        throw;
}

/**
 * apply
 *
 * Apply relaxng program, writing results.
 *
 * @returns true on success false on failure.
 */
TransformationResult relaxngTransformation::relaxngTransformation::apply(xmlDocPtr doc, int /* position */) const {

    // 0 means it validated
    int n = xmlRelaxNGValidateDoc(rngctx.get(), doc);
    if (n != 0)
        return TransformationResult();

    // transformation result is nodeset with single unit, and the unit is wrapped
    TransformationResult result(xmlXPathNodeSetCreate(xmlDocGetRootElement(doc)), true);
    result.nodeType = SRCML_RESULT_UNITS;
    return result;
}
