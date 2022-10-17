// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file relaxngTransformation.cpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
relaxngTransformation::relaxngTransformation(/* OPTION_TYPE& options, */ xmlDocPtr relaxng)
    : relaxng_parser_ctxt(xmlRelaxNGNewDocParserCtxt(relaxng)), rng(xmlRelaxNGParse(relaxng_parser_ctxt.get())) {

    if (relaxng_parser_ctxt == nullptr)
        throw;

    if (rng == nullptr)
        throw;
}

/**
 * apply
 *
 * Apply relaxng program, writing results.
 *
 * @returns true on success false on failure.
 */
TransformationResult relaxngTransformation::apply(xmlDocPtr doc, int /* position */) const {

    std::unique_ptr<xmlRelaxNGValidCtxt> rngctx(xmlRelaxNGNewValidCtxt(rng.get()));
    if (rngctx == nullptr)
        throw;

    // 0 means it validated
    int n = xmlRelaxNGValidateDoc(rngctx.get(), doc);
    if (n != 0)
        return TransformationResult();

    // transformation result is nodeset with single unit, and the unit is wrapped
    TransformationResult result(xmlXPathNodeSetCreate(xmlDocGetRootElement(doc)), true);
    result.nodeType = SRCML_RESULT_UNITS;
    return result;
}
